/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "AscensionWildcard.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include <algorithm>
#include <array>
#include <list>

namespace
{
enum CorruptedBladeSpells : uint32
{
    SPELL_CORRUPTED_BLADE = 281492,
    SPELL_CORRUPTED_BLADE_SPREAD = 281493,
    SPELL_SINISTER_STRIKE = 1752
};

class wildcard_corrupted_blade : public AllSpellScript
{
public:
    wildcard_corrupted_blade() : AllSpellScript("wildcard_corrupted_blade", {ALLSPELLHOOK_ON_HIT_RESULT}) { }

    void OnSpellHitResult(Spell* spell, Unit* source, uint8 miss, uint32, uint32, bool) override
    {
        if (miss != SPELL_MISS_NONE)
            return;

        Unit* caster = spell->GetCaster();
        Player* player = caster ? caster->ToPlayer() : nullptr;
        SpellInfo const* info = spell->GetSpellInfo();
        if (!player || !AscensionWildcard::IsWildcardHero(player) || spell->IsTriggered() ||
            !player->HasAura(SPELL_CORRUPTED_BLADE, player->GetGUID()) ||
            (sSpellMgr->GetFirstSpellInChain(info->Id) != SPELL_SINISTER_STRIKE &&
                (info->SpellFamilyName != SPELLFAMILY_ROGUE || !(info->SpellFamilyFlags[2] & 0x80000000))))
            return;

        SpellInfo const* spread = sSpellMgr->GetSpellInfo(SPELL_CORRUPTED_BLADE_SPREAD);
        if (!source || !source->IsAlive() || !player->IsValidAttackTarget(source) || !spread)
            return;

        std::array<uint32, 2> roots = {spread->Effects[EFFECT_0].TriggerSpell,
            spread->Effects[EFFECT_1].TriggerSpell};
        float const radius = spread->Effects[EFFECT_0].CalcRadius(player);
        std::list<Unit*> targets;
        Acore::AnyUnitInObjectRangeCheck check(source, radius);
        Acore::UnitListSearcher<Acore::AnyUnitInObjectRangeCheck> search(source, targets, check);
        Cell::VisitObjects(source, search, radius);
        targets.remove_if([player, source, radius](Unit* target)
        {
            return target == source || !target->IsAlive() || target->IsTotem() ||
                !player->IsValidAttackTarget(target) || !source->InSamePhase(target) ||
                !source->IsWithinDistInMap(target, radius) || !source->IsWithinLOSInMap(target) ||
                (target->IsPlayer() && (target->HasUnitState(UNIT_STATE_CONTROLLED | UNIT_STATE_ROOT) ||
                    target->HasBreakableByDamageCrowdControlAura()));
        });
        targets.sort([source](Unit* first, Unit* second)
        {
            float const firstDistance = source->GetExactDist(first);
            float const secondDistance = source->GetExactDist(second);
            return firstDistance != secondDistance ? firstDistance < secondDistance :
                first->GetGUID() < second->GetGUID();
        });
        if (spread->MaxAffectedTargets && targets.size() > spread->MaxAffectedTargets)
            targets.resize(spread->MaxAffectedTargets);

        for (uint32 root : roots)
        {
            Aura* original = source->GetAuraOfRankedSpell(root, player->GetGUID());
            if (!original || original->GetDuration() <= 0)
                continue;
            for (Unit* target : targets)
                if (Aura* copy = player->AddAura(original->GetId(), target))
                {
                    copy->SetStackAmount(original->GetStackAmount());
                    copy->SetMaxDuration(original->GetMaxDuration());
                    copy->SetDuration(original->GetDuration());
                    for (uint8 index = 0; index < MAX_SPELL_EFFECTS; ++index)
                        if (AuraEffect* effect = copy->GetEffect(index))
                            if (AuraEffect const* from = original->GetEffect(index))
                                effect->ChangeAmount(from->GetAmount());
                }
        }
    }
};
}

void AddSC_AscensionWildcardCorruptedBlade()
{
    new wildcard_corrupted_blade();
}
