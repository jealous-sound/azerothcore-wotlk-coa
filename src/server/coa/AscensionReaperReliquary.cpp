/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include <iterator>

namespace
{
constexpr uint32 SPELL_ATTACK_BOLTS[] = {500618, 500619, 500620};
constexpr uint32 SPELL_VISUAL_BOLTS[] = {500621, 500622, 500623};
constexpr uint32 SPELL_REAPED_SOUL = 500363;
constexpr uint32 BOLT_INTERVAL_MS = 2500;

class ReliquaryBolts : public BasicEvent
{
public:
    ReliquaryBolts(Unit* caster, uint32 index) : _caster(caster), _index(index) { }

    bool Execute(uint64, uint32) override
    {
        if (!_caster->IsAlive())
            return true;

        _caster->CastSpell(_caster, SPELL_ATTACK_BOLTS[_index], true);

        if (++_index < std::size(SPELL_ATTACK_BOLTS))
            _caster->m_Events.AddEventAtOffset(new ReliquaryBolts(_caster, _index), Milliseconds(BOLT_INTERVAL_MS));
        return true;
    }

private:
    Unit* _caster;
    uint32 _index;
};

class spell_reaper_reliquary_of_the_lost : public SpellScript
{
    PrepareSpellScript(spell_reaper_reliquary_of_the_lost);

    uint8 _souls = 0;

    void RememberSouls()
    {
        Unit* caster = GetCaster();
        Aura const* souls = caster->GetAura(SPELL_REAPED_SOUL, caster->GetGUID());
        _souls = souls ? souls->GetStackAmount() : 0;
    }

    void Launch()
    {
        Unit* caster = GetCaster();
        caster->m_Events.AddEventAtOffset(new ReliquaryBolts(caster, 0), 0ms);

        for (uint8 index = 0; index < std::size(SPELL_VISUAL_BOLTS) && index < _souls; ++index)
            caster->CastSpell(caster, SPELL_VISUAL_BOLTS[index], true);
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_reaper_reliquary_of_the_lost::RememberSouls);
        AfterCast += SpellCastFn(spell_reaper_reliquary_of_the_lost::Launch);
    }
};
}

void AddSC_AscensionReaperReliquary()
{
    RegisterSpellScript(spell_reaper_reliquary_of_the_lost);
}
