/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"

namespace
{
enum StormbringerTalentSpells : uint32
{
    SPELL_CLOUDBURST = 801838,
    SPELL_CLOUDBURST_KNOCKBACK = 802385
};

class stormbringer_talent_casts : public AllSpellScript
{
public:
    stormbringer_talent_casts() : AllSpellScript("stormbringer_talent_casts", {ALLSPELLHOOK_ON_CAST}) { }

    void OnSpellCast(Spell* spell, Unit* caster, SpellInfo const* info, bool) override
    {
        Player* player = caster ? caster->ToPlayer() : nullptr;
        if (player && player->getClass() == CLASS_STORMBRINGER && info->SpellFamilyName == 22 &&
            info->Id == SPELL_CLOUDBURST && !spell->IsTriggered())
            // The active spell has a zero-radius dummy. Its separate native
            // helper supplies the ten-yard area and authored knockback speeds.
            player->CastSpell(player, SPELL_CLOUDBURST_KNOCKBACK, true);
    }
};
}

void AddSC_AscensionStormbringerTalents()
{
    new stormbringer_talent_casts();
}
