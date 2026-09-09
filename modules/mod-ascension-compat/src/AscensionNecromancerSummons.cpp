/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionNecromancer.h"
#include "AscensionNecromancerData.h"
#include "Creature.h"
#include "DBCStores.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "TemporarySummon.h"
#include <algorithm>

namespace AscensionNecromancer
{
namespace
{
bool Stationary(uint32 entry)
{
    return entry == 50132 || entry == 542064 || entry == 575091;
}
uint32 AttackSpell(uint32 entry)
{
    switch (entry)
    {
    case 50067:
        return 707598;
    case 50075:
        return 801513;
    case 50076:
        return 801516;
    case 500650:
        return 822074;
    case 50323:
        return 822074;
    case 50177:
        return 801513;
    case 500483:
    case 500484:
        return 801513;
    default:
        return 0;
    }
}
} // namespace
bool Responds(uint32 entry, uint32 command)
{
    switch (command)
    {
    case 504020:
    case 504489:
        return entry == 50075;
    case 504021:
        return entry == 50073;
    case 504052:
        return entry == 50067;
    case 504318:
        return entry == 50078;
    case 504319:
        return entry == 50076;
    case 504334:
    case 504905:
        return entry == 50065 || entry == 51065;
    case 504860:
        return entry == 50323;
    case 504862:
    case 504864:
        return entry == 500650;
    case 504050:
    case 504863:
        return entry == 50115;
    case 504316:
    case 504865:
        return entry == 50068;
    case 500991:
    case 504868:
    case 504907:
    case 504908:
    case 805871:
        return !Stationary(entry);
    default:
        return false;
    }
}
void Scale(Player* player, Creature* minion, uint8 cost, float& inheritedSpeed)
{
    // Local reconstruction: linear Life Force weighting, with ordinary guardian
    // aura/stat modifiers retained.
    float weight = float(std::max<uint8>(1, cost));
    float healthFraction = minion->GetHealthPct() / 100.0f;
    float manaFraction =
        minion->GetMaxPower(POWER_MANA) ? float(minion->GetPower(POWER_MANA)) / minion->GetMaxPower(POWER_MANA) : 1.0f;
    float stamina = (player->GetLevel() * 3.0f + player->GetStat(STAT_STAMINA) * 0.3f) * weight;
    float intellect = player->GetStat(STAT_INTELLECT);
    float spellPower = float(std::max(player->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FROST),
                                      player->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SHADOW)));
    minion->SetLevel(player->GetLevel());
    minion->SetStatFlatModifier(UNIT_MOD_STAT_STAMINA, BASE_VALUE, stamina);
    minion->SetStatFlatModifier(UNIT_MOD_STAT_INTELLECT, BASE_VALUE, intellect * 0.3f * weight);
    minion->UpdateStats(STAT_STAMINA);
    minion->UpdateStats(STAT_INTELLECT);
    minion->SetStatFlatModifier(UNIT_MOD_HEALTH, BASE_VALUE,
                                50.0f * player->GetLevel() * weight + minion->GetStat(STAT_STAMINA) * 10);
    minion->UpdateMaxHealth();
    minion->SetHealth(std::max(1u, uint32(healthFraction * minion->GetMaxHealth())));
    minion->setPowerType(POWER_MANA);
    minion->SetStatFlatModifier(UNIT_MOD_MANA, BASE_VALUE, player->GetLevel() * 30.0f + intellect * 5 * weight);
    minion->UpdateMaxPower(POWER_MANA);
    minion->SetPower(POWER_MANA, int32(manaFraction * minion->GetMaxPower(POWER_MANA)));
    float ap = (intellect + std::max(0.0f, spellPower)) * 0.4f * weight;
    if (AuraEffect* inherited = minion->GetAuraEffect(805015, EFFECT_1))
        inherited->ChangeAmount(int32((intellect + std::max(0.0f, spellPower)) * 0.2f * weight));
    minion->SetStatFlatModifier(UNIT_MOD_ATTACK_POWER, BASE_VALUE, ap);
    minion->SetStatFlatModifier(UNIT_MOD_ATTACK_POWER_RANGED, BASE_VALUE, ap);
    minion->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, player->GetLevel() * weight);
    minion->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, player->GetLevel() * weight * 1.5f);
    minion->SetBaseWeaponDamage(RANGED_ATTACK, MINDAMAGE, player->GetLevel() * weight);
    minion->SetBaseWeaponDamage(RANGED_ATTACK, MAXDAMAGE, player->GetLevel() * weight * 1.5f);
    minion->UpdateAttackPowerAndDamage();
    minion->UpdateAttackPowerAndDamage(true);
    for (uint8 school = 0; school < MAX_SPELL_SCHOOL; ++school)
    {
        float value = player->GetResistance(SpellSchools(school)) * (school ? 0.4f : 0.35f);
        minion->SetStatFlatModifier(UnitMods(UNIT_MOD_RESISTANCE_START + school), BASE_VALUE, value);
        minion->UpdateResistances(school);
    }
    minion->m_modMeleeHitChance = player->m_modMeleeHitChance;
    minion->m_modRangedHitChance = player->m_modRangedHitChance;
    minion->m_modSpellHitChance = player->m_modSpellHitChance;
    minion->m_baseSpellCritChance = int32(player->GetFloatValue(PLAYER_SPELL_CRIT_PERCENTAGE1 + SPELL_SCHOOL_FROST)) +
                                    minion->GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT) +
                                    minion->GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
    float speed = std::max(0.1f, player->GetFloatValue(UNIT_MOD_CAST_SPEED));
    minion->SetFloatValue(UNIT_MOD_CAST_SPEED, minion->GetFloatValue(UNIT_MOD_CAST_SPEED) * speed / inheritedSpeed);
    inheritedSpeed = speed;
    // Native haste auras multiply the base period independently, so inheritance
    // never compounds each refresh.
    minion->SetAttackTime(BASE_ATTACK, uint32(2000 * speed));
    minion->SetAttackTime(RANGED_ATTACK, uint32(2000 * speed));
}

bool Summon(Player* player, uint32 spell, Unit* target, Position const& position, int32 duration)
{
    if (!player || !player->IsAlive() || !player->IsInWorld())
        return false;
    SpellInfo const* info = sSpellMgr->GetSpellInfo(spell);
    if (!info)
        return false;
    uint8 cost = Cost(player, spell);
    if (cost && int32(Capacity(player)) - Used(player) < cost)
        return false;
    int32 lifetime = duration ? duration : info->GetDuration();
    bool created = false;
    for (auto const& row : NecromancerSummons)
        if (row.spell == spell)
            for (uint8 i = 0; i < std::min<uint8>(row.count, 16); ++i)
            {
                // Check every creature, including multi-summon and triggered paths.
                // IsSummonedBy registers each successful creation synchronously.
                if (cost && int32(Capacity(player)) - Used(player) < cost)
                {
                    Sync(player);
                    return created;
                }
                Position point = position;
                player->MovePositionToFirstCollision(point, 1.5f + i * 0.5f, float(i) * 2.4f);
                // Authored 61 is a non-pet guardian: native controlled-list cleanup and
                // effect 190 work for the army.
                auto properties = sSummonPropertiesStore.LookupEntry(Stationary(row.creature) ? 64 : 61);
                if (!properties)
                    return created;
                TempSummon* unit = player->GetMap()->SummonCreature(row.creature, point, properties,
                                                                    lifetime > 0 ? uint32(lifetime) : 0, player, spell);
                if (!unit)
                    continue;
                if (!IsMinion(player, unit))
                {
                    unit->DespawnOrUnsummon();
                    continue;
                }
                unit->SetTempSummonType(lifetime > 0 ? TEMPSUMMON_TIMED_DESPAWN : TEMPSUMMON_DEAD_DESPAWN);
                created = true;
                unit->GetMotionMaster()->Clear();
                if (row.creature == 523032)
                {
                    Position end = point;
                    unit->MovePositionToFirstCollision(end, 25.0f, 0.0f);
                    unit->GetMotionMaster()->MovePoint(1, end);
                }
                else if (Stationary(row.creature))
                    unit->GetMotionMaster()->MoveIdle();
                else if (target && player->IsValidAttackTarget(target) && !player->HasAura(500983))
                    unit->AI()->AttackStart(target);
                else
                    unit->GetMotionMaster()->MoveFollow(player, 2.0f, float(i) * 2.4f);
            }
    Sync(player);
    return created;
}

void Order(Player* player, Unit* target, uint32 spell)
{
    if (!target || !player->IsValidAttackTarget(target))
        target = player->GetVictim();
    if (!target || !player->IsValidAttackTarget(target))
        target = ObjectAccessor::GetUnit(*player, player->GetTarget());
    if (!target || !player->IsValidAttackTarget(target) || player->HasAura(500983))
        return;
    State(player).focus = target->GetGUID();
    for (Creature* minion : Minions(player))
    {
        if (!Responds(minion->GetEntry(), spell) || !minion->IsWithinDistInMap(target, 60.0f) ||
            !minion->IsWithinLOSInMap(target))
            continue;
        minion->AI()->SetGUID(target->GetGUID(), 1);
        minion->AI()->SetData(1, spell);
        minion->AI()->DoAction(1);
    }
}
} // namespace AscensionNecromancer

namespace
{
using namespace AscensionNecromancer;
class npc_ascension_necromancer : public ScriptedAI
{
  public:
    explicit npc_ascension_necromancer(Creature* creature) : ScriptedAI(creature) {}
    ObjectGuid _owner;
    ObjectGuid _target;
    EventMap _events;
    uint32 _command = 0;
    uint32 _spell = 0;
    uint8 _cost = 0;
    bool _exploded = false;
    float _inheritedSpeed = 1.0f;

    void IsSummonedBy(WorldObject* summoner) override
    {
        Player* player = summoner ? summoner->ToPlayer() : nullptr;
        if (!player || player->getClass() != CLASS_NECROMANCER)
            return;
        _owner = player->GetGUID();
        _spell = me->GetUInt32Value(UNIT_CREATED_BY_SPELL);
        _cost = Cost(player, _spell);
        // Native/helper summon paths must obey the same limit. Reject before
        // applying army auras or letting the new creature enter combat.
        if (_cost && int32(Capacity(player)) - Used(player) < _cost)
        {
            me->DespawnOrUnsummon();
            return;
        }
        me->SetOwnerGUID(_owner);
        me->SetCreatorGUID(_owner);
        me->SetFaction(player->GetFaction());
        me->SetReactState(REACT_DEFENSIVE);
        me->SetCombatMovement(!Stationary(me->GetEntry()));
        State(player).minions.push_back({me->GetGUID(), _spell, _cost});
        me->AddAura(805015, me);
        Scale(player, me, _cost, _inheritedSpeed);
        me->SetHealth(me->GetMaxHealth());
        me->SetPower(POWER_MANA, me->GetMaxPower(POWER_MANA));
        if (_cost && player->HasAura(504866))
            player->CastCustomSpell(505224, SPELLVALUE_BASE_POINT0, int32(me->CountPctFromMaxHealth(40)), me, true);
        if (me->GetEntry() == 50073)
            Cast(player, me, 805290);
        for (uint32 ward : {680388, 681460, 681529})
            if (Aura const* active = player->GetAura(ward))
                if (Aura* copy = player->AddAura(ward, me))
                    copy->SetDuration(active->GetDuration());
        if (me->GetEntry() == 50132)
        {
            ObjectGuid previous = State(player).phylactery;
            State(player).phylactery = me->GetGUID();
            if (Creature* old = ObjectAccessor::GetCreature(*me, previous))
                old->DespawnOrUnsummon();
        }
        _events.ScheduleEvent(1, 1ms);
        _events.ScheduleEvent(2, 2s);
        if (me->GetEntry() == 50133 || me->GetEntry() == 50303)
            _events.ScheduleEvent(3, Milliseconds(sSpellMgr->GetSpellInfo(807640)->Effects[1].Amplitude));
        if (me->GetEntry() == 542064)
            _events.ScheduleEvent(4, 2s);
        if (me->GetEntry() == 542065)
        {
            me->ToTempSummon()->SetTempSummonType(TEMPSUMMON_CORPSE_TIMED_DESPAWN);
            _events.ScheduleEvent(7, 500ms);
        }
        if (me->GetEntry() == 575091)
        {
            if (!player->HasSpell(807098))
                player->learnSpell(807098, true);
            auto previous = State(player).minions;
            for (auto const& row : previous)
                if (row.guid != me->GetGUID())
                    if (Creature* circle = ObjectAccessor::GetCreature(*player, row.guid))
                        if (circle->GetEntry() == 575091 && circle->GetOwnerGUID() == player->GetGUID())
                            circle->DespawnOrUnsummon();
        }
    }
    void SetGUID(ObjectGuid const& guid, int32 key) override
    {
        if (key == 1)
            _target = guid;
    }
    void SetData(uint32 key, uint32 value) override
    {
        if (key == 1)
            _command = value;
    }
    void AttackStart(Unit* target) override
    {
        Player* player = Owner(me);
        if (player && !Stationary(me->GetEntry()) && !player->HasAura(500983) && target &&
            player->IsValidAttackTarget(target))
            ScriptedAI::AttackStart(target);
    }
    void DoAction(int32 action) override
    {
        Player* player = Owner(me);
        Unit* target = ObjectAccessor::GetUnit(*me, _target);
        if (action != 1 || !player || player->HasAura(500983) || !target || !target->IsAlive() ||
            !Responds(me->GetEntry(), _command) || me->HasUnitState(UNIT_STATE_CONTROLLED))
            return;
        AttackStart(target);
        if (_command == 500991)
            return;
        uint32 entry = me->GetEntry();
        if ((_command == 504316 && entry != 50068) || (_command == 504489 && entry != 50075) ||
            (_command == 504050 && entry != 50115))
            return;
        if (_command == 504316)
        {
            target->GetMotionMaster()->MoveJump(me->GetPosition(), 24.0f, 8.0f);
            Cast(me, target, 800043);
            return;
        }
        if (_command == 504489)
        {
            for (Unit* unit : Nearby(target, 8.0f))
                if (player->IsValidAttackTarget(unit))
                    Cast(me, unit, 504845);
            return;
        }
        if (_command == 504050)
        {
            Cast(me, me, 801534);
            return;
        }
        switch (entry)
        {
        case 50065:
        case 51065:
            Cast(me, target, 570042);
            if (entry == 51065)
                Cast(me, target, 570216);
            break;
        case 50073:
            Cast(me, target, 801514);
            break;
        case 50068:
            Copy(me, me, 504022, std::max(1, Amount(504022, 0, player) + Amount(505223, 1, player)));
            break;
        case 50115:
            _events.RescheduleEvent(5, 3s);
            break;
        case 50078:
            me->GetMotionMaster()->MoveJump(target->GetPosition(), 24.0f, 8.0f);
            _events.RescheduleEvent(6, 500ms);
            break;
        case 500650:
        {
            uint32 mana = std::min<uint32>(target->GetPower(POWER_MANA), std::min(target->GetMaxPower(POWER_MANA) / 10,
                                                                                  me->GetMaxPower(POWER_MANA) / 5));
            target->ModifyPower(POWER_MANA, -int32(mana));
            if (_command != 504864)
                Copy(me, target, 505225, mana);
            break;
        }
        case 50309:
            Cast(me, me, 801412);
            break;
        case 503030:
        case 503031:
        case 503032:
        case 523032:
            me->GetMotionMaster()->MoveCharge(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());
            Cast(me, me, 504022);
            break;
        case 50133:
        case 50303:
            Cast(me, target, 801518);
            break;
        default:
            if (uint32 ability = AttackSpell(entry))
                Cast(me, target, ability);
            else
                Cast(me, me, 504022);
            break;
        }
    }
    void UpdateAI(uint32 diff) override
    {
        Player* player = ObjectAccessor::FindPlayer(_owner);
        if (!player || !player->IsAlive() || !player->IsInMap(me) || !player->InSamePhase(me))
        {
            me->DespawnOrUnsummon();
            return;
        }
        _events.Update(diff);
        while (uint32 event = _events.ExecuteEvent())
        {
            if (event == 1)
            {
                Scale(player, me, Cost(player, _spell), _inheritedSpeed);
                bool passive = player->HasAura(500983) || Stationary(me->GetEntry());
                me->SetReactState(passive                   ? REACT_PASSIVE
                                  : player->HasAura(500982) ? REACT_AGGRESSIVE
                                                            : REACT_DEFENSIVE);
                if (passive)
                {
                    me->AttackStop();
                    me->InterruptNonMeleeSpells(false);
                    me->GetMotionMaster()->MoveIdle();
                }
                else if (!me->GetVictim() && me->GetEntry() != 523032)
                {
                    Unit* target = ObjectAccessor::GetUnit(*me, State(player).focus);
                    if (!target || !player->IsValidAttackTarget(target))
                        target = player->GetVictim();
                    if (!target && !player->getAttackers().empty())
                        target = *player->getAttackers().begin();
                    if (!target && player->HasAura(500982))
                        for (Unit* unit : Nearby(me, 20.0f))
                            if (player->IsValidAttackTarget(unit) && me->IsWithinLOSInMap(unit))
                            {
                                target = unit;
                                break;
                            }
                    if (target)
                        AttackStart(target);
                    else if (!me->IsWithinDistInMap(player, 5.0f))
                        me->GetMotionMaster()->MoveFollow(player, 2.0f, PET_FOLLOW_ANGLE);
                }
                if (me->GetEntry() == 50132 && player->HasAura(500730) && me->IsWithinDistInMap(player, 3.0f))
                {
                    State(player).shade = false;
                    player->RemoveAurasDueToSpell(500730);
                    player->RemoveAurasDueToSpell(500729);
                    player->SetHealth(player->CountPctFromMaxHealth(40));
                    me->DespawnOrUnsummon();
                    return;
                }
                _events.ScheduleEvent(1, 1s);
            }
            if (event == 2)
            {
                if (!player->HasAura(500983) && !me->HasUnitState(UNIT_STATE_CONTROLLED))
                {
                    if (me->GetEntry() == 50068)
                        Cast(me, me, 802353);
                    if (Unit* victim = me->GetVictim())
                        if (uint32 ability = AttackSpell(me->GetEntry()))
                            Cast(me, victim, ability);
                }
                _events.ScheduleEvent(2, 3s);
            }
            if (event == 3 && !_exploded)
            {
                _exploded = true;
                if (me->GetEntry() == 523032)
                    Cast(me, me, 707010);
                else
                    Cast(me, me, 500585);
                me->DespawnOrUnsummon(100ms);
            }
            if (event == 4)
            {
                // Cast the actual helper so its ground damage and corpse summon share
                // one application.
                player->CastSpell(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 500365, true);
                _events.ScheduleEvent(4, 3s);
            }
            if (event == 5 && !player->HasAura(500983) && !me->HasUnitState(UNIT_STATE_CONTROLLED))
                if (Unit* victim = ObjectAccessor::GetUnit(*me, _target))
                    Cast(me, victim, 505229);
            if (event == 6 && !player->HasAura(500983) && !me->HasUnitState(UNIT_STATE_CONTROLLED))
                if (Unit* victim = ObjectAccessor::GetUnit(*me, _target))
                    if (me->IsWithinMeleeRange(victim))
                        for (uint8 hit = 0; hit < (victim->HealthBelowPct(35) ? 5 : 2); ++hit)
                            me->AttackerStateUpdate(victim, BASE_ATTACK, true);
            if (event == 7)
            {
                me->ToTempSummon()->SetTempSummonType(TEMPSUMMON_CORPSE_TIMED_DESPAWN);
                Unit::Kill(me, me);
                return;
            }
        }
        if (me->GetEntry() == 523032 && !_exploded)
            for (Unit* nearby : Nearby(me, 2.5f))
                if (player->IsValidAttackTarget(nearby))
                {
                    _events.RescheduleEvent(3, 1ms);
                    break;
                }
        if (!player->HasAura(500983) && !Stationary(me->GetEntry()) && UpdateVictim())
            DoMeleeAttackIfReady();
    }
};

class spell_ascension_necromancer_summon : public SpellScript
{
    PrepareSpellScript(spell_ascension_necromancer_summon);
    bool _summoned = false;
    void SummonEffect(SpellEffIndex index)
    {
        bool selected = false;
        for (auto const& row : NecromancerSummons)
            selected |= row.spell == GetSpellInfo()->Id && row.effect == index;
        if (!selected)
            return;
        Player* player = Owner(GetCaster());
        if (!player)
            return;
        PreventHitDefaultEffect(index);
        if (_summoned)
            return;
        _summoned = true;
        Position position = GetExplTargetDest() ? GetExplTargetDest()->GetPosition() : player->GetPosition();
        Summon(player, GetSpellInfo()->Id, GetExplTargetUnit(), position);
    }
    void Register() override
    {
        // Native SUMMON executes at HIT. Launch suppression is cleared by
        // PrepareScriptHitHandlers and would allow a second, unbudgeted summon.
        OnEffectHit += SpellEffectFn(spell_ascension_necromancer_summon::SummonEffect, EFFECT_ALL, SPELL_EFFECT_ANY);
    }
};
} // namespace
void AddAscensionNecromancerSummonScripts()
{
    RegisterCreatureAI(npc_ascension_necromancer);
    RegisterSpellScript(spell_ascension_necromancer_summon);
}
