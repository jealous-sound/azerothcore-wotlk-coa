/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include <algorithm>
#include <iterator>

namespace
{
constexpr uint32 SPELL_ATTACK_BOLTS[] = {500618, 500619, 500620};
constexpr uint32 SPELL_VISUAL_BOLTS[] = {500621, 500622, 500623};
constexpr uint32 SPELL_REAPED_SOUL = 500363;
constexpr uint32 SPELL_STORED_BOLTS = 500629;
constexpr uint32 BOLT_INTERVAL_MS = 2500;

class ReliquaryBolts : public BasicEvent
{
public:
    ReliquaryBolts(Unit* caster, uint32 index, uint32 bolts)
        : _caster(caster), _index(index), _bolts(bolts) { }

    bool Execute(uint64, uint32) override
    {
        if (!_caster->IsAlive())
            return true;

        _caster->CastSpell(_caster, SPELL_ATTACK_BOLTS[_index], true);
        _caster->RemoveAurasDueToSpell(SPELL_VISUAL_BOLTS[_index]);

        if (Aura* stored = _caster->GetAura(SPELL_STORED_BOLTS, _caster->GetGUID()))
            stored->ModStackAmount(-1);

        if (++_index < _bolts)
            _caster->m_Events.AddEventAtOffset(new ReliquaryBolts(_caster, _index, _bolts),
                Milliseconds(BOLT_INTERVAL_MS));
        return true;
    }

private:
    Unit* _caster;
    uint32 _index;
    uint32 _bolts;
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
        uint32 const bolts = std::min<uint32>(_souls ? _souls : 1, std::size(SPELL_ATTACK_BOLTS));

        if (Aura* stored = caster->AddAura(SPELL_STORED_BOLTS, caster))
            stored->SetStackAmount(uint8(bolts));

        caster->m_Events.AddEventAtOffset(new ReliquaryBolts(caster, 0, bolts),
            Milliseconds(BOLT_INTERVAL_MS));

        for (uint32 index = 0; index < bolts; ++index)
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
