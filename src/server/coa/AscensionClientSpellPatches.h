#ifndef ASCENSION_CLIENT_SPELL_PATCHES_H
#define ASCENSION_CLIENT_SPELL_PATCHES_H

#include "Define.h"
#include <array>
#include <mutex>
#include <unordered_map>
#include <unordered_set>

namespace Ascension
{
    class ClientSpellPatches
    {
    public:
        using Selector = std::array<uint32, 3>;

        static ClientSpellPatches& Instance()
        {
            static ClientSpellPatches patches;
            return patches;
        }

        void Register(uint32 spellId, Selector const& selector)
        {
            std::lock_guard lock(_mutex);
            Selector& existing = _selectors[spellId];
            for (std::size_t word = 0; word < existing.size(); ++word)
                existing[word] |= selector[word];
        }

        std::unordered_set<uint32> GetIds() const
        {
            std::lock_guard lock(_mutex);
            std::unordered_set<uint32> ids;
            for (auto const& [spellId, selector] : _selectors)
                ids.insert(spellId);
            return ids;
        }

        Selector GetSelector(uint32 spellId) const
        {
            std::lock_guard lock(_mutex);
            auto const found = _selectors.find(spellId);
            return found == _selectors.end() ? Selector{} : found->second;
        }

    private:
        mutable std::mutex _mutex;
        std::unordered_map<uint32, Selector> _selectors;
    };
}

#endif
