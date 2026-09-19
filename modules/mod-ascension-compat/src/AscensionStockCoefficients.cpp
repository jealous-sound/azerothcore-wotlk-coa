/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionStockCoefficientData.h"
#include "ScriptMgr.h"
#include "SpellInfo.h"

#include <algorithm>

namespace
{
// Spell.dbc f229-231 (EffectBonusMultiplier) is the stock 3.3.5a spell-power coefficient column. CoA
// never wired it up: it authors coefficients in tooltip formula text instead, and the shipped column
// agrees with that text on 6.2% of the CoA effect slots carrying both. AzerothCore still reads it as
// the default coefficient whenever spell_bonus_data holds no row (Unit::SpellDamageBonusDone and
// Unit::SpellHealingBonusDone), so a 2008 value scales CoA content that never asked for it. Of the
// live CoA damaging and healing spells carrying one, none agreed with its own tooltip within 5%:
// some run several times high, some low, and many scale off a stat their tooltip never mentions.
//
// Clearing the field makes spell_bonus_data the only coefficient channel for CoA content. Where a row
// exists the damage and healing paths already overwrite the field's value with the row's, so this is a
// no-op there and carries no dependency on the load order between the two.
//
// Scope is the generated list, not an id range: the CoA bands also hold copies of stock records whose
// coefficients are genuine. Stock spells, Reborn clone bands and offset copies are excluded by the
// generator, so this pass is a membership test, never a heuristic.
class ascension_stock_coefficients : public GlobalScript
{
public:
    ascension_stock_coefficients() : GlobalScript("ascension_stock_coefficients",
        {GLOBALHOOK_ON_LOAD_SPELL_CUSTOM_ATTR}) { }

    void OnLoadSpellCustomAttr(SpellInfo* info) override
    {
        if (!info || !std::binary_search(AscensionCompatData::StockCoefficientSpells.begin(),
                AscensionCompatData::StockCoefficientSpells.end(), info->Id))
            return;

        for (SpellEffectInfo& effect : info->Effects)
            effect.BonusMultiplier = 0.0f;
    }
};
}

void AddAscensionStockCoefficientScripts()
{
    new ascension_stock_coefficients();
}
