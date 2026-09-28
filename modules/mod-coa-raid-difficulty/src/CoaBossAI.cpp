/*
 * A boss AI that reads its fight from the database.
 *
 * The schedules are measured from combat logs: 42 logs, 89 pulls, 31 kills.
 * Three findings shape
 * this file:
 *
 *  - Intervals are the same on every difficulty; only the numbers change. The
 *    numbers come from SpellDifficulty.dbc, which the core already resolves by
 *    map difficulty whenever a spell is cast (SpellMgr::GetSpellIdForDifficulty).
 *    So a row names one cast, and the right tier happens by itself. The four
 *    spell columns exist only for the few casts whose id genuinely differs by
 *    tier in the logs.
 *
 *  - Most of Ascension's boss casts are dummies. Living Bomb 2105701 has one
 *    effect, SPELL_EFFECT_DUMMY, and does nothing; on the live server a script
 *    reacted to it and cast 2105702. A row carries that second spell in
 *    `effect`, and this AI casts it the moment the dummy completes. An
 *    interrupted cast never gets there, which is the point of casting the dummy
 *    first instead of the effect straight away.
 *
 *  - A few spells hang off health, not a clock: Sulfuron's Conflagrate always
 *    came at 49-50% health, whenever that was. Those rows carry hp_pct and fire
 *    once.
 *
 * It is a BossAI, not a plain ScriptedAI, and that matters: BossAI reports
 * engage and death to the instance script. Without it Molten Core never learns
 * that a boss is down - no Majordomo, no Ragnaros, and Golemagg's Core Ragers
 * never stand down.
 *
 * Deliberately thin otherwise. Nothing here knows about ranges, conditions or
 * adds; a boss that needs that keeps its own script.
 */

#include "Creature.h"
#include "DBCEnums.h"
#include "DatabaseEnv.h"
#include "Field.h"
#include "InstanceScript.h"
#include "Log.h"
#include "Map.h"
#include "ObjectAccessor.h"
#include "QueryResult.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellInfo.h"
#include "SpellMgr.h"

#include <unordered_map>
#include <vector>

namespace
{
    enum TargetMode : uint8
    {
        TARGET_TANK = 0,
        TARGET_RANDOM_NON_TANK = 1,
        TARGET_SELF = 2,
        TARGET_AREA = 3         // cast at the tank; the spell's own targeting makes it an area
    };

    struct ScheduleRow
    {
        uint32 spell[MAX_RAID_DIFFICULTY];
        uint32 effect;          // cast when the dummy completes, 0 none
        uint32 firstMs;
        uint32 periodMs;        // 0 casts once
        uint8 hpPct;            // > 0: once below this health instead of on a clock
        uint8 target;
    };

    struct BossData
    {
        uint32 bossId = 0;
        uint32 berserkMs = 0;
        std::vector<ScheduleRow> rows;
    };

    std::unordered_map<uint32, BossData> g_bosses;

    // A difficulty variant is its base entry plus 100000, 200000 or 300000.
    uint32 BaseEntry(uint32 entry)
    {
        return entry % 100000;
    }

    void LoadSchedules()
    {
        g_bosses.clear();

        if (QueryResult result = WorldDatabase.Query("SELECT entry, boss_id, berserk_ms FROM coa_boss"))
        {
            do
            {
                Field* f = result->Fetch();
                BossData& boss = g_bosses[f[0].Get<uint32>()];
                boss.bossId = f[1].Get<uint32>();
                boss.berserkMs = f[2].Get<uint32>();
            } while (result->NextRow());
        }

        uint32 rows = 0;
        if (QueryResult result = WorldDatabase.Query(
                "SELECT entry, spell_d0, spell_d1, spell_d2, spell_d3, effect, first_ms, period_ms, hp_pct, target "
                "FROM coa_boss_schedule ORDER BY entry, idx"))
        {
            do
            {
                Field* f = result->Fetch();
                uint32 const entry = f[0].Get<uint32>();

                ScheduleRow row{};
                for (uint8 i = 0; i < MAX_RAID_DIFFICULTY; ++i)
                    row.spell[i] = f[1 + i].Get<uint32>();
                row.effect = f[5].Get<uint32>();
                row.firstMs = f[6].Get<uint32>();
                row.periodMs = f[7].Get<uint32>();
                row.hpPct = f[8].Get<uint8>();
                row.target = f[9].Get<uint8>();

                g_bosses[entry].rows.push_back(row);
                ++rows;
            } while (result->NextRow());
        }

        LOG_INFO("server.loading", ">> Loaded {} boss schedule rows for {} bosses",
                 rows, uint32(g_bosses.size()));
    }

    struct coa_boss_ai : public BossAI
    {
        coa_boss_ai(Creature* creature, BossData const* data)
            : BossAI(creature, data->bossId), _data(data) { }

        void Reset() override
        {
            BossAI::Reset();
            _events.Reset();
            _pending.clear();
            _hpDone.assign(_data ? _data->rows.size() : 0, false);
        }

        void JustEngagedWith(Unit* who) override
        {
            BossAI::JustEngagedWith(who);
            if (!_data)
                return;

            // Row n is event n + 1. Zero is not usable as an event id:
            // ExecuteEvent returns it to mean that nothing is due.
            for (uint32 i = 0; i < _data->rows.size(); ++i)
                if (!_data->rows[i].hpPct)
                    _events.ScheduleEvent(i + 1, Milliseconds(_data->rows[i].firstMs));

            if (_data->berserkMs)
                _events.ScheduleEvent(EVENT_BERSERK, Milliseconds(_data->berserkMs));
        }

        void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType type, SpellSchoolMask school) override
        {
            BossAI::DamageTaken(attacker, damage, type, school);
            if (!_data)
                return;

            for (uint32 i = 0; i < _data->rows.size(); ++i)
            {
                ScheduleRow const& row = _data->rows[i];
                if (row.hpPct && !_hpDone[i] && me->HealthBelowPctDamaged(row.hpPct, damage))
                {
                    _hpDone[i] = true;
                    // Next update, not here: casting from inside a damage
                    // callback can interrupt the cast that is doing the damage.
                    _events.ScheduleEvent(i + 1, 0ms);
                }
            }
        }

        void OnSpellCast(SpellInfo const* spell) override
        {
            BossAI::OnSpellCast(spell);

            // The dummy finished: cast what it stood for, at the same target.
            auto it = _pending.find(spell->Id);
            if (it == _pending.end())
                return;

            Pending const pending = it->second;
            _pending.erase(it);

            Unit* target = pending.target.IsEmpty() ? nullptr : ObjectAccessor::GetUnit(*me, pending.target);
            if (!target || !target->IsAlive())
                target = me->GetVictim();
            if (target)
                me->CastSpell(target, pending.effect, true);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim() || !_data)
                return;

            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = _events.ExecuteEvent())
            {
                if (eventId == EVENT_BERSERK)
                {
                    DoCastSelf(SPELL_BERSERK, true);
                    continue;
                }

                uint32 const index = eventId - 1;
                if (index >= _data->rows.size())
                    continue;

                ScheduleRow const& row = _data->rows[index];
                if (uint32 spell = SpellFor(row))
                    Cast(row, spell);

                if (row.periodMs && !row.hpPct)
                    _events.ScheduleEvent(eventId, Milliseconds(row.periodMs));

                // One cast per update. A second one would interrupt the first.
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    break;
            }

            DoMeleeAttackIfReady();
        }

    private:
        enum
        {
            EVENT_BERSERK = 0xFFFF,
            SPELL_BERSERK = 26662
        };

        struct Pending
        {
            uint32 effect;
            ObjectGuid target;
        };

        uint32 SpellFor(ScheduleRow const& row) const
        {
            uint8 const mode = uint8(me->GetMap()->GetSpawnMode());
            uint32 const spell = mode < MAX_RAID_DIFFICULTY ? row.spell[mode] : 0;
            return spell ? spell : row.spell[0];
        }

        Unit* TargetFor(ScheduleRow const& row)
        {
            switch (row.target)
            {
                case TARGET_SELF:
                    return me;
                case TARGET_RANDOM_NON_TANK:
                    // Falls back to the tank when there is no one else.
                    if (Unit* other = SelectTarget(SelectTargetMethod::Random, 0, 0.0f, true, false))
                        return other;
                    return me->GetVictim();
                case TARGET_TANK:
                case TARGET_AREA:
                default:
                    return me->GetVictim();
            }
        }

        void Cast(ScheduleRow const& row, uint32 spell)
        {
            Unit* target = TargetFor(row);
            if (!target)
                return;

            if (row.effect)
            {
                // The id the core will actually cast, after difficulty; that is
                // the id OnSpellCast will see.
                uint32 const resolved = sSpellMgr->GetSpellIdForDifficulty(spell, me);
                _pending[resolved] = { row.effect, target->GetGUID() };
            }

            if (me->CastSpell(target, spell, false) != SPELL_CAST_OK)
                _pending.erase(sSpellMgr->GetSpellIdForDifficulty(spell, me));
        }

        BossData const* _data;
        EventMap _events;
        std::unordered_map<uint32, Pending> _pending;
        std::vector<bool> _hpDone;
    };

    class coa_boss_ai_script : public CreatureScript
    {
    public:
        coa_boss_ai_script() : CreatureScript("coa_boss_ai") { }

        CreatureAI* GetAI(Creature* creature) const override
        {
            // Without a row in coa_boss there is no encounter id, and a BossAI
            // with a made-up one would report the wrong boss to the instance.
            // The core falls back to its default AI.
            auto it = g_bosses.find(BaseEntry(creature->GetEntry()));
            if (it == g_bosses.end())
            {
                LOG_ERROR("scripts", "coa_boss_ai: creature {} has no row in coa_boss", creature->GetEntry());
                return nullptr;
            }
            return new coa_boss_ai(creature, &it->second);
        }
    };

    class coa_boss_schedule_loader : public WorldScript
    {
    public:
        coa_boss_schedule_loader() : WorldScript("coa_boss_schedule_loader") { }

        void OnAfterConfigLoad(bool /*reload*/) override
        {
            LoadSchedules();
        }
    };
}

void AddCoaBossAIScripts()
{
    new coa_boss_ai_script();
    new coa_boss_schedule_loader();
}
