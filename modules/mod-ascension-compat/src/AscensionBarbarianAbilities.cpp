/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionBarbarianCompletion.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include <algorithm>
#include <set>
#include <vector>

namespace
{
using namespace AscensionBarbarian;

class spell_ascension_barbarian_ability : public SpellScript
{
    PrepareSpellScript(spell_ascension_barbarian_ability);
    std::set<ObjectGuid> _hit;
    std::vector<ObjectGuid> _line;
    Position _endpoint;
    bool _hasEndpoint = false;

    void SuppressStun(SpellEffIndex index)
    {
        if (Family(GetSpellInfo(), 1, 64) && GetCaster()->HasAura(807183))
            PreventHitDefaultEffect(index);
    }

    void Select(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();
        targets.remove_if([&](WorldObject* object)
        {
            return !caster->HasInLine(object, object->GetCombatReach(), 1.5f);
        });
        targets.sort([&](WorldObject* left, WorldObject* right)
        {
            float a = caster->GetExactDist2d(left), b = caster->GetExactDist2d(right);
            return a == b ? left->GetGUID() < right->GetGUID() : a < b;
        });
        _line.clear();
        for (WorldObject* object : targets)
            _line.push_back(object->GetGUID());
        // Snapshot the landing point; later target movement cannot move the explosion.
        float range = GetSpellInfo()->Effects[EFFECT_0].CalcRadius(caster);
        _endpoint = targets.empty() ? caster->GetNearPosition(range, 0.0f) : targets.back()->GetPosition();
        _hasEndpoint = true;
    }

    void Damage()
    {
        if (!Family(GetSpellInfo(), 1, 256))
            return;
        auto itr = std::find(_line.begin(), _line.end(), GetHitUnit()->GetGUID());
        if (itr != _line.end())
        {
            // Missed or immune targets do not weaken the spear. Count the native
            // successful hit selections in distance order, not GUID dispatch order.
            auto const* hits = GetSpell()->GetUniqueTargetInfo();
            auto preceding = std::count_if(_line.begin(), itr, [&](ObjectGuid guid)
            {
                return std::any_of(hits->begin(), hits->end(), [guid](TargetInfo const& hit)
                {
                    return hit.targetGUID == guid && hit.missCondition == SPELL_MISS_NONE && (hit.effectMask & 5);
                });
            });
            // Local reconstruction: lose 10% per intervening hit, with a 10% floor.
            float multiplier = std::max(0.1f, 1.0f - 0.1f * float(preceding));
            SetHitDamage(int32(GetHitDamage() * multiplier));
        }
    }

    void Hit()
    {
        Player* player = Owner(GetCaster());
        Unit* target = GetHitUnit();
        if (!player || !target || !GetHitDamage() || GetSpell()->IsTriggered() || target == player ||
            player->IsFriendlyTo(target) || !_hit.insert(target->GetGUID()).second)
            return;
        uint32 id = GetSpellInfo()->Id;
        if (Whirl(id))
        {
            if (player->GetWeaponForAttack(OFF_ATTACK, true))
            {
                SpellCastTargets targets;
                targets.SetUnitTarget(target);
                CustomSpellValues values;
                values.AddSpellMod(SPELLVALUE_MELEE_ATTACK_TYPE, OFF_ATTACK);
                values.AddSpellMod(SPELLVALUE_BASE_POINT0, GetSpellInfo()->Effects[EFFECT_0].CalcValue(player));
                // The flat rank term is applied once, by the main hand. The authored
                // off-hand helper's old extra +20 and repeated fixed bonus are removed.
                values.AddSpellMod(SPELLVALUE_BASE_POINT1, 0);
                player->CastSpell(targets, sSpellMgr->GetSpellInfo(805232), &values, TRIGGERED_FULL_MASK);
            }
            if (_hit.size() >= 3 && player->HasAura(560563) && !player->HasSpellCooldown(560563))
            {
                uint32 cooldown = player->GetSpellCooldownDelay(id);
                if (cooldown)
                {
                    player->RemoveSpellCooldown(id, true);
                    player->AddSpellCooldown(560563, 0, cooldown);
                }
            }
        }
        if (Family(GetSpellInfo(), 1, 2) && player->HasAura(704876))
        {
            Unit* victim = player->GetVictim();
            if (victim && victim->IsAlive() && player->IsWithinMeleeRange(victim))
                player->HandleProcExtraAttackFor(victim, 2);
        }
    }

    void Finish()
    {
        Player* player = Owner(GetCaster());
        if (!player || GetSpell()->IsTriggered())
            return;
        if (Family(GetSpellInfo(), 1, 256) && _hasEndpoint)
            player->CastSpell(_endpoint.GetPositionX(), _endpoint.GetPositionY(),
                _endpoint.GetPositionZ(), 255846, true);
        if (Family(GetSpellInfo(), 1, 1) && player->HasAura(705208))
            if (Unit* pet = Ancestor(player))
                pet->CastSpell(pet, 804756, true);
        if (Whirl(GetSpellInfo()->Id) || Family(GetSpellInfo(), 1, 16))
            if (Unit* pet = Ancestor(player))
                pet->CastSpell(pet, 804756, true);
        if (Family(GetSpellInfo(), 2, 2097152) && player->HasAura(705221))
        {
            std::list<Unit*> units;
            Acore::AnyUnitInObjectRangeCheck check(player, 30.0f);
            Acore::UnitListSearcher<Acore::AnyUnitInObjectRangeCheck> searcher(player, units, check);
            Cell::VisitObjects(player, searcher, 30.0f);
            units.remove_if([&](Unit* unit)
            {
                return unit == player || !unit->IsPlayer() || !unit->IsAlive() || !player->IsInRaidWith(unit) ||
                    !player->IsFriendlyTo(unit) || !player->IsWithinLOSInMap(unit);
            });
            units.sort([&](Unit* left, Unit* right)
            {
                return player->GetExactDist(left) < player->GetExactDist(right);
            });
            if (!units.empty())
                if (SpellInfo const* helper = sSpellMgr->GetSpellInfo(GetSpellInfo()->Effects[EFFECT_0].TriggerSpell))
                    player->CastCustomSpell(573077, SPELLVALUE_BASE_POINT0,
                        helper->Effects[EFFECT_0].CalcValue(player), units.front(), TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        SpellInfo const* info = sSpellMgr->GetSpellInfo(m_scriptSpellId);
        if (Family(info, 1, 64))
        {
            OnEffectLaunch += SpellEffectFn(spell_ascension_barbarian_ability::SuppressStun,
                EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
            OnEffectLaunchTarget += SpellEffectFn(spell_ascension_barbarian_ability::SuppressStun,
                EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
        }
        if (Family(info, 1, 256))
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ascension_barbarian_ability::Select,
                EFFECT_ALL, TARGET_UNIT_CONE_ENEMY_24);
            OnHit += SpellHitFn(spell_ascension_barbarian_ability::Damage);
        }
        AfterHit += SpellHitFn(spell_ascension_barbarian_ability::Hit);
        AfterCast += SpellCastFn(spell_ascension_barbarian_ability::Finish);
    }
};

class aura_ascension_barbarian_volley : public AuraScript
{
    PrepareAuraScript(aura_ascension_barbarian_volley);
    uint8 _shots = 0;

    void Tick(AuraEffect const* effect)
    {
        PreventDefaultAction();
        Player* player = Owner(GetCaster());
        Unit* target = GetTarget();
        if (!player || !target->IsAlive() || !player->IsValidAttackTarget(target) || _shots >= 3)
            return;
        player->CastSpell(target, GetId() == 707584 ? 520541 : 807236, true, nullptr, effect);
        if (++_shots == 3 && GetId() == 707584)
            player->GetMotionMaster()->MoveJumpTo(float(M_PI), 15.0f, 7.5f);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(aura_ascension_barbarian_volley::Tick,
            EFFECT_ALL, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};
}

void AddAscensionBarbarianAbilityScripts()
{
    RegisterSpellScript(spell_ascension_barbarian_ability);
    RegisterSpellScript(aura_ascension_barbarian_volley);
}
