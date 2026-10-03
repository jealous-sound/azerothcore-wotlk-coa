#ifndef ASCENSION_CLIENT_SPELL_PATCHES_H
#define ASCENSION_CLIENT_SPELL_PATCHES_H

#include "Define.h"
#include <mutex>
#include <unordered_set>

namespace Ascension
{
    class ClientSpellPatches
    {
    public:
        static ClientSpellPatches& Instance()
        {
            static ClientSpellPatches patches;
            return patches;
        }

        void Register(uint32 spellId)
        {
            std::lock_guard lock(_mutex);
            _spellIds.insert(spellId);
        }

        std::unordered_set<uint32> GetIds() const
        {
            std::lock_guard lock(_mutex);
            return _spellIds;
        }

    private:
        mutable std::mutex _mutex;
        std::unordered_set<uint32> _spellIds;
    };
}

#endif
