/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "Creature.h"
#include "MotionMaster.h"
#include "Random.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <numbers>

namespace
{
enum DeadminesSawBladeData : uint32
{
    NPC_SNEEDS_SHREDDER = 642,
    NPC_SNEED = 643,
    NPC_CAPTAIN_GREENSKIN = 647,
    SPELL_ASCENSION_POISONED_HARPOON = 2102585,
    SPELL_BUZZING_SAW_BLADE = 2102564,
    POINT_SAW_ORBIT = 1,
    SAW_MOVE_INTERVAL_MS = 250,
    SAW_BOSS_LOST_MS = 3000,
    SAW_DIP_DELAY_MIN_MS = 6000,
    SAW_DIP_DELAY_MAX_MS = 10000,
    SAW_DIP_DURATION_MS = 4000
};

constexpr std::array<uint32, 2> SawBosses = { NPC_SNEEDS_SHREDDER, NPC_SNEED };
constexpr float SawBossRange = 80.0f;
constexpr float SawOrbitRadius = 5.0f;
constexpr float SawDipRadius = 2.0f;
constexpr float SawSpeed = 3.0f;
constexpr float SawLookaheadSeconds = 0.6f;
constexpr float SawMinStepRadius = 2.0f;

struct npc_ascension_buzzing_saw_blade : ScriptedAI
{
    explicit npc_ascension_buzzing_saw_blade(Creature* creature) : ScriptedAI(creature),
        _clockwise(urand(0, 1) == 1), _moveTimer(0), _lostTimer(0),
        _dipDelay(urand(SAW_DIP_DELAY_MIN_MS, SAW_DIP_DELAY_MAX_MS)), _dipElapsed(0), _started(false) { }

    void AttackStart(Unit*) override { }
    void MoveInLineOfSight(Unit*) override { }
    void EnterEvadeMode(EvadeReason) override { }

    void IsSummonedBy(WorldObject*) override
    {
        Start();
    }

    void UpdateAI(uint32 diff) override
    {
        Start();

        Creature* boss = FindBoss();
        if (!boss)
        {
            _lostTimer += diff;
            if (_lostTimer >= SAW_BOSS_LOST_MS)
                me->DespawnOrUnsummon();
            return;
        }
        _lostTimer = 0;

        AdvanceDip(diff);

        _moveTimer = _moveTimer > diff ? _moveTimer - diff : 0;
        if (_moveTimer)
            return;
        _moveTimer = SAW_MOVE_INTERVAL_MS;
        Orbit(boss);
    }

private:
    void Start()
    {
        if (_started)
            return;
        _started = true;
        me->SetReactState(REACT_PASSIVE);
        me->CastSpell(me, SPELL_BUZZING_SAW_BLADE, true);
    }

    Creature* FindBoss() const
    {
        for (uint32 entry : SawBosses)
            if (Creature* boss = me->FindNearestCreature(entry, SawBossRange, true))
                if (boss->IsInCombat())
                    return boss;
        return nullptr;
    }

    void AdvanceDip(uint32 diff)
    {
        if (_dipElapsed)
        {
            _dipElapsed += diff;
            if (_dipElapsed >= SAW_DIP_DURATION_MS)
            {
                _dipElapsed = 0;
                _dipDelay = urand(SAW_DIP_DELAY_MIN_MS, SAW_DIP_DELAY_MAX_MS);
            }
            return;
        }
        _dipDelay = _dipDelay > diff ? _dipDelay - diff : 0;
        if (!_dipDelay)
            _dipElapsed = 1;
    }

    float CurrentRadius() const
    {
        float const dip = _dipElapsed ? std::sin(std::numbers::pi_v<float> * float(_dipElapsed) / float(SAW_DIP_DURATION_MS)) : 0.0f;
        return SawOrbitRadius - (SawOrbitRadius - SawDipRadius) * dip;
    }

    void Orbit(Creature* boss)
    {
        float const dx = me->GetPositionX() - boss->GetPositionX();
        float const dy = me->GetPositionY() - boss->GetPositionY();
        float const distance = std::max(std::hypot(dx, dy), SawMinStepRadius);
        float const step = SawSpeed * SawLookaheadSeconds / distance;
        float const angle = std::atan2(dy, dx) + (_clockwise ? -step : step);
        float const radius = CurrentRadius();
        float const x = boss->GetPositionX() + radius * std::cos(angle);
        float const y = boss->GetPositionY() + radius * std::sin(angle);
        float z = boss->GetPositionZ();
        me->UpdateAllowedPositionZ(x, y, z);
        me->GetMotionMaster()->MovePoint(POINT_SAW_ORBIT, x, y, z, FORCED_MOVEMENT_NONE, SawSpeed, 0.0f, false);
    }

    bool _clockwise;
    uint32 _moveTimer;
    uint32 _lostTimer;
    uint32 _dipDelay;
    uint32 _dipElapsed;
    bool _started;
};

class spell_ascension_greenskin_poisoned_harpoon : public SpellScript
{
    PrepareSpellScript(spell_ascension_greenskin_poisoned_harpoon);

    bool Load() override
    {
        Unit* caster = GetCaster();
        return caster && caster->GetEntry() == NPC_CAPTAIN_GREENSKIN;
    }

    bool Validate(SpellInfo const*) override
    {
        return ValidateSpellInfo({ SPELL_ASCENSION_POISONED_HARPOON });
    }

    void ThrowHarpoon()
    {
        PreventHitAura();
        PreventHitDamage();
        if (Unit* target = GetHitUnit())
            GetCaster()->CastSpell(target, SPELL_ASCENSION_POISONED_HARPOON, true);
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_ascension_greenskin_poisoned_harpoon::ThrowHarpoon);
    }
};
}

void AddSC_AscensionDeadminesSawBlades()
{
    RegisterCreatureAI(npc_ascension_buzzing_saw_blade);
    RegisterSpellScript(spell_ascension_greenskin_poisoned_harpoon);
}
