/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuras.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"

namespace
{
constexpr uint32 AdvantageCompanions[] = {704337, 801429, 801700};

class aura_ascension_ranger_advantage : public AuraScript
{
    PrepareAuraScript(aura_ascension_ranger_advantage);

    bool Validate(SpellInfo const*) override { return ValidateSpellInfo({704337, 801429, 801700}); }

    bool Load() override
    {
        return GetCaster() && GetCaster() == GetUnitOwner() && GetCaster()->IsPlayer() &&
            GetCaster()->getClass() == CLASS_RANGER;
    }

    void Sync(AuraEffect const*, AuraEffectHandleModes)
    {
        Unit* player = GetTarget();
        for (uint32 id : AdvantageCompanions)
        {
            Aura* companion = player->GetAura(id, player->GetGUID());
            if (!companion)
                companion = player->AddAura(id, player);
            if (companion && companion->GetStackAmount() != GetStackAmount())
                companion->SetStackAmount(GetStackAmount());
        }
    }

    void Clear(AuraEffect const*, AuraEffectHandleModes)
    {
        for (uint32 id : AdvantageCompanions)
            GetTarget()->RemoveAurasDueToSpell(id, GetTarget()->GetGUID());
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(aura_ascension_ranger_advantage::Sync,
            EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        AfterEffectRemove += AuraEffectRemoveFn(aura_ascension_ranger_advantage::Clear,
            EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class ranger_secondary_hits : public AllSpellScript
{
public:
    ranger_secondary_hits() : AllSpellScript("ranger_secondary_hits",
        {ALLSPELLHOOK_ON_BEFORE_EFFECTS, ALLSPELLHOOK_ON_HIT_RESULT}) { }

    void OnSpellBeforeEffects(Spell* spell, Unit* caster, SpellInfo const* info) override
    {
        if (caster->IsPlayer() && caster->getClass() == CLASS_RANGER && info->SpellFamilyName == 27 &&
            !spell->IsTriggered() && sSpellMgr->GetFirstSpellInChain(info->Id) == 804712)
            if (Aura const* advantage = caster->GetAura(804329, caster->GetGUID()))
                spell->SetScriptValue(801935, 2000 * advantage->GetStackAmount());
    }

    void OnSpellHitResult(Spell* spell, Unit* target, uint8 miss, uint32, uint32, bool) override
    {
        Player* player = spell->GetCaster()->ToPlayer();
        if (!player || player->getClass() != CLASS_RANGER || spell->GetSpellInfo()->SpellFamilyName != 27 ||
            miss != SPELL_MISS_NONE || !target || target == player || player->IsFriendlyTo(target))
            return;
        if (!spell->IsTriggered() && sSpellMgr->GetFirstSpellInChain(spell->GetSpellInfo()->Id) == 804712 &&
            !spell->GetScriptValue(804712))
        {
            spell->SetScriptValue(804712, 1);
            if (uint64 duration = spell->GetScriptValue(801935))
                if (SpellInfo const* flare = sSpellMgr->GetSpellInfo(801935))
                {
                    SpellCastTargets targets;
                    targets.SetUnitTarget(target);
                    targets.SetDst(target->GetPosition());
                    CustomSpellValues values;
                    values.AddSpellMod(SPELLVALUE_AURA_DURATION, int32(duration));
                    player->CastSpell(targets, flare, &values, TRIGGERED_FULL_MASK);
                }
        }
        if (spell->GetSpellInfo()->Id != 803105 || !target->IsAlive() ||
            !target->HasAuraState(AURA_STATE_BLEEDING) || spell->GetScriptValue(803106))
            return;
        spell->SetScriptValue(803106, 1);
        player->CastSpell(target, 803106, true);
    }
};

class ranger_secondary_contracts : public GlobalScript
{
public:
    ranger_secondary_contracts() : GlobalScript("ranger_secondary_contracts",
        {GLOBALHOOK_ON_LOAD_SPELL_CUSTOM_ATTR}) { }

    void OnLoadSpellCustomAttr(SpellInfo* info) override
    {
        if (!info || info->SpellFamilyName != 27)
            return;
        if (info->Id == 560805)
            info->ExcludeTargetAuraSpell = 570167;
        if (info->Id == 801935)
            info->UseRangedAttackPowerForDamage = true;
        for (uint32 id : AdvantageCompanions)
            if (info->Id == id)
            {
                info->AttributesCu &= ~SPELL_ATTR0_CU_FORCE_AURA_SAVING;
                info->AttributesCu |= SPELL_ATTR0_CU_AURA_CANNOT_BE_SAVED;
            }
    }
};
}

void AddSC_AscensionRangerSecondary()
{
    RegisterSpellScript(aura_ascension_ranger_advantage);
    new ranger_secondary_hits();
    new ranger_secondary_contracts();
}
