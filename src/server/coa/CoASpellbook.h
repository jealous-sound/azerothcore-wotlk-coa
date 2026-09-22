#ifndef COA_SPELLBOOK_H
#define COA_SPELLBOOK_H

#include <cstdint>

class Player;

namespace CoASpellbook
{
    constexpr std::uint16_t SMSG_PATCH_SPELL_CUSTOM_ATTR = 0x05F4;

    struct Provider
    {
        std::uint32_t (*RowCount)(Player*) = nullptr;
        bool (*OffersSpell)(Player*, std::uint32_t) = nullptr;
        bool (*CoversSpell)(Player*, std::uint32_t) = nullptr;
    };

    void SetProvider(Provider provider);
    double RowCount(Player* player);
    double OffersSpell(Player* player, std::uint32_t spellId);
    double CoversSpell(Player* player, std::uint32_t spellId);
}

#endif
