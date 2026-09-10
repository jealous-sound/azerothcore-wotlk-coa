/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license:
 * https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#ifndef COA_BUG_REPORT_SERVICE_H
#define COA_BUG_REPORT_SERVICE_H

#include <algorithm>
#include <charconv>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

namespace CoABugReport
{
    constexpr std::size_t MaxPayload = 12000;
    constexpr std::size_t ChunkSize = 180;
    constexpr std::string_view Prefix = "COABUG\t";

    inline bool ValidId(std::string_view id)
    {
        return id.size() >= 16 && id.size() <= 48 && std::all_of(id.begin(), id.end(), [](char c)
        {
            return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
        });
    }

    inline bool Number(std::string_view text, uint32_t& value)
    {
        auto const result = std::from_chars(text.data(), text.data() + text.size(), value);
        return !text.empty() && result.ec == std::errc() && result.ptr == text.data() + text.size();
    }

    // World-thread service. No network, database, credentials or retained Player pointers.
    // Files are private to the administrator and relay; clients supply only a validated request ID.
    class Service
    {
    public:
        explicit Service(std::filesystem::path directory, uint32_t cooldown = 120)
            : _directory(std::move(directory)), _cooldown(std::max(60u, cooldown)) { }

        std::string Handle(uint32_t account, std::string_view message, std::string const& context, uint64_t now)
        {
            if (message == "H|1")
                return "H|1";

            if (message.size() < 3 || message[1] != '|')
                return {};

            auto const end = message.find('|', 2);
            std::string const id(message.substr(2, end == std::string_view::npos ? end : end - 2));
            if (!ValidId(id))
                return {};

            // Bound disk/status requests too, including clients bypassing the addon's pacing.
            std::erase_if(_traffic, [now](auto const& pair) { return now > pair.second.second + 10; });
            if (!_traffic.contains(account) && _traffic.size() >= 4096)
                return {};
            auto& traffic = _traffic[account];
            if (traffic.second != now)
                traffic = { 0, now };
            if (++traffic.first > 24)
                return {};

            std::string const key = std::to_string(account) + "-" + id;
            std::string const response = "S|" + id + "|";
            std::string_view const tail = end == std::string_view::npos ? std::string_view() : message.substr(end + 1);
            auto const failure = [&id](char const* code) { return "E|" + id + "|" + code; };

            try
            {
                // Bound idle partial uploads and cooldown accounting independently of relogging.
                std::erase_if(_uploads, [now](auto const& pair) { return now > pair.second.updated + 120; });
                std::erase_if(_lastSubmit, [now, this](auto const& pair)
                {
                    return now > pair.second + _cooldown;
                });

                auto const request = _directory / (key + ".report");
                if (std::filesystem::exists(request))
                {
                    if (message[0] == 'Q' || message[0] == 'B' || message[0] == 'C')
                        return response + ReadStatus(key);
                    return failure("queued");
                }

                if (message[0] == 'Q' && tail.empty())
                    return response + "missing";

                if (message[0] == 'B')
                {
                    uint32_t size = 0;
                    if (!Number(tail, size) || size < 4 || size > MaxPayload)
                        return failure("invalid");
                    if (_lastSubmit.contains(account))
                        return failure("cooldown");
                    auto const found = _uploads.find(account);
                    if (found != _uploads.end())
                    {
                        if (found->second.id != id || found->second.expected != size)
                            return failure("busy");
                        found->second.updated = now;
                        return "A|" + id + "|" + std::to_string(found->second.sequence);
                    }
                    if (_uploads.size() >= 128 || _lastSubmit.size() >= 4096)
                        return failure("busy");
                    _uploads.emplace(account, Upload{id, size, 0, {}, {}, now});
                    return "A|" + id + "|0";
                }

                auto const found = _uploads.find(account);
                if (found == _uploads.end() || found->second.id != id)
                    return failure("missing");
                Upload& upload = found->second;
                upload.updated = now;

                if (message[0] == 'D')
                {
                    auto const split = tail.find('|');
                    uint32_t sequence = 0;
                    if (split == std::string_view::npos || !Number(tail.substr(0, split), sequence))
                        return failure("invalid");
                    auto const data = tail.substr(split + 1);
                    if (data.empty() || data.size() > ChunkSize)
                        return failure("invalid");
                    if (sequence == upload.sequence && data == upload.lastChunk)
                        return "A|" + id + "|" + std::to_string(sequence);
                    if (sequence != upload.sequence + 1 || upload.payload.size() + data.size() > upload.expected)
                        return failure("sequence");
                    upload.payload += data;
                    upload.lastChunk = data;
                    upload.sequence = sequence;
                    return "A|" + id + "|" + std::to_string(sequence);
                }

                if (message[0] != 'C' || !tail.empty())
                    return failure("invalid");
                if (upload.payload.size() != upload.expected || !ValidPayload(upload.payload))
                    return failure("invalid");

                // Publish a complete file in one rename. The relay ignores .part files.
                auto const temporary = _directory / (key + ".part");
                {
                    std::ofstream output(temporary, std::ios::binary | std::ios::trunc);
                    output << "COABUG1\n" << upload.payload << context;
                    output.flush();
                    if (!output)
                        return failure("storage");
                    output.close();
                    if (!output)
                        return failure("storage");
                }
                std::filesystem::rename(temporary, request);
                _lastSubmit[account] = now;
                _uploads.erase(found);
                return response + "queued";
            }
            catch (std::filesystem::filesystem_error const&)
            {
                return failure("storage");
            }
        }

        static bool ValidPayload(std::string const& payload)
        {
            auto const split = payload.find('\n');
            if (split == std::string::npos || split < 3 || split > 200 || split + 2 >= payload.size())
                return false;
            if (payload.find_first_of("\r\t") < split)
                return false;
            return std::all_of(payload.begin(), payload.end(), [](unsigned char c)
            {
                return c >= 32 || c == '\n' || c == '\r' || c == '\t';
            });
        }

    private:
        struct Upload
        {
            std::string id;
            uint32_t expected;
            uint32_t sequence;
            std::string payload;
            std::string lastChunk;
            uint64_t updated;
        };

        std::string ReadStatus(std::string const& key) const
        {
            auto const path = _directory / (key + ".status");
            if (!std::filesystem::exists(path))
                return "queued";
            if (std::filesystem::file_size(path) > 80)
                return "unavailable";
            std::ifstream input(path, std::ios::binary);
            std::string status;
            std::getline(input, status);
            if (!status.empty() && status.back() == '\r')
                status.pop_back();
            if (status == "queued" || status == "blocked" || status == "uncertain" || status == "failed")
                return status;
            uint32_t issue = 0;
            if (status.starts_with("created|") && Number(std::string_view(status).substr(8), issue) && issue)
                return status;
            return "unavailable";
        }

        std::filesystem::path _directory;
        uint32_t _cooldown;
        std::unordered_map<uint32_t, Upload> _uploads;
        std::unordered_map<uint32_t, uint64_t> _lastSubmit;
        std::unordered_map<uint32_t, std::pair<uint32_t, uint64_t>> _traffic;
    };
}

#endif
