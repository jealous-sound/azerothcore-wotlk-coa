/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "GridNotifiers.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include <list>

namespace
{
enum ReliquarySpells : uint32
{
    SPELL_SOUL_BOLT = 500627
};

// Ascension's server script is not in the archive. The retained spell data gives three "Attack N Reaper
// Skull Bolt" missiles (500618-500620) that trigger Soul Bolt, one per Reaped Soul, and a tooltip that
// launches them "forward over 5 seconds"; the interval, line length and width are local choices.
constexpr uint32 BOLT_COUNT = 3;
constexpr uint32 BOLT_INTERVAL_MS = 2500;
constexpr float BOLT_LENGTH = 40.0f;
constexpr float BOLT_WIDTH = 4.0f;

class ReliquaryBolts : public BasicEvent
{
public:
    ReliquaryBolts(Unit* caster, uint32 remaining) : _caster(caster), _remaining(remaining) { }

    bool Execute(uint64 /*time*/, uint32 /*diff*/) override
    {
        if (!_caster->IsAlive())
            return true;

        std::list<Unit*> targets;
        Acore::AnyUnfriendlyUnitInObjectRangeCheck check(_caster, _caster, BOLT_LENGTH);
        Acore::UnitListSearcher<Acore::AnyUnfriendlyUnitInObjectRangeCheck> searcher(_caster, targets, check);
        Cell::VisitObjects(_caster, searcher, BOLT_LENGTH);
        for (Unit* target : targets)
            if (_caster->HasInLine(target, target->GetCombatReach(), BOLT_WIDTH) && _caster->IsWithinLOSInMap(target))
                _caster->CastSpell(target, SPELL_SOUL_BOLT, true);

        if (--_remaining)
            _caster->m_Events.AddEventAtOffset(new ReliquaryBolts(_caster, _remaining), Milliseconds(BOLT_INTERVAL_MS));
        return true;
    }

private:
    Unit* _caster;
    uint32 _remaining;
};

class spell_reaper_reliquary_of_the_lost : public SpellScript
{
    PrepareSpellScript(spell_reaper_reliquary_of_the_lost);

    void Launch()
    {
        Unit* caster = GetCaster();
        caster->m_Events.AddEventAtOffset(new ReliquaryBolts(caster, BOLT_COUNT), 0ms);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_reaper_reliquary_of_the_lost::Launch);
    }
};
}

void AddSC_AscensionReaperReliquary()
{
    RegisterSpellScript(spell_reaper_reliquary_of_the_lost);
}
