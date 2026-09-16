/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ClientDBC.h"
#include "gtest/gtest.h"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace
{
std::string WriteFile(std::string const& suffix, std::vector<uint8> const& bytes)
{
    testing::TestInfo const* test = testing::UnitTest::GetInstance()->current_test_info();
    std::filesystem::path const path =
        std::filesystem::temp_directory_path() / (std::string(test->name()) + suffix + ".dbc");
    std::ofstream(path.string(), std::ios::binary).write(reinterpret_cast<char const*>(bytes.data()), bytes.size());
    return path.string();
}

void Append(std::vector<uint8>& bytes, uint32 value)
{
    uint8 raw[sizeof(value)];
    std::memcpy(raw, &value, sizeof(value));
    bytes.insert(bytes.end(), raw, raw + sizeof(raw));
}

// Two 13-byte records, like custom client tables that mix DWORD and byte fields:
// [id, name offset, float] followed by one byte.
std::vector<uint8> MixedLayoutFile()
{
    std::string const strings("\0Alpha\0", 7);
    std::vector<uint8> bytes;
    for (uint32 value : { 0x43424457u, 2u, 4u, 13u, uint32(strings.size()) })
        Append(bytes, value);

    uint32 half;
    float const halfValue = 0.5f;
    std::memcpy(&half, &halfValue, sizeof(half));
    for (uint32 value : { 7u, 1u, half })
        Append(bytes, value);
    bytes.push_back(3);
    for (uint32 value : { 8u, 0xFF00FEu, 0u })
        Append(bytes, value);
    bytes.push_back(4);

    bytes.insert(bytes.end(), strings.begin(), strings.end());
    return bytes;
}
}

TEST(ClientDBCTest, ReadsDWordsBytesAndStrings)
{
    std::string const path = WriteFile("", MixedLayoutFile());
    ClientDBC dbc;
    ASSERT_TRUE(dbc.Load(path, 3));
    ASSERT_EQ(dbc.GetRecordCount(), 2u);

    ClientDBC::Record first = dbc.GetRecord(0);
    EXPECT_EQ(first.GetUInt32(0), 7u);
    EXPECT_EQ(first.GetString(1), "Alpha");
    EXPECT_FLOAT_EQ(first.GetFloat(2), 0.5f);
    EXPECT_EQ(first.GetUInt8(12), 3u);

    ClientDBC::Record second = dbc.GetRecord(1);
    EXPECT_EQ(second.GetUInt32(0), 8u);
    EXPECT_EQ(second.GetString(1), "");
    EXPECT_EQ(second.GetUInt8(12), 4u);
    std::filesystem::remove(path);
}

TEST(ClientDBCTest, RejectsShortRecordsAndBadFiles)
{
    std::vector<uint8> bytes = MixedLayoutFile();
    std::string const valid = WriteFile("Valid", bytes);
    ClientDBC dbc;
    EXPECT_FALSE(dbc.Load(valid, 4));
    EXPECT_EQ(dbc.GetRecordCount(), 0u);

    bytes.pop_back();
    std::string const truncated = WriteFile("Truncated", bytes);
    EXPECT_FALSE(dbc.Load(truncated, 3));
    EXPECT_FALSE(dbc.Load(valid + ".missing", 3));

    std::filesystem::remove(valid);
    std::filesystem::remove(truncated);
}
