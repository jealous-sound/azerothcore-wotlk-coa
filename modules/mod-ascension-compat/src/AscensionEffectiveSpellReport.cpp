/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU
 * AGPL v3 license:
 * https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

// Read-only diagnostic report of the spell primitives that custom-class abilities run at HEAD.
// It reads SpellInfo after SpellInfo corrections, module OnLoadSpellCustomAttr contracts, spell_proc,
// spell_bonus_data, spell_linked_spell and spell_script_names have loaded, and compares every slot with
// its raw Spell.dbc row. Output schema: apps/coa-gameplay-test/spell_report.py.

#include "AscensionCoATalentData.h"
#include "AscensionCustomClassData.h"
#include "AscensionLiveBaselineData.h"
#include "AscensionRacialAbilities.h"
#include "AscensionSpellProgressionData.h"
#include "AscensionTalentReplacementData.h"
#include "AscensionTaughtAbilityData.h"
#include "Chat.h"
#include "CommandScript.h"
#include "Config.h"
#include "DBCStores.h"
#include "GameTime.h"
#include "GitRevision.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "RaceMgr.h"
#include "Spell.h"
#include "SpellAuraDefines.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "StringFormat.h"
#include "WorldScript.h"

#include <utf8.h>

#include <array>
#include <deque>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <locale>
#include <map>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

// Dispatch tables defined in SpellEffects.cpp and SpellAuraEffects.cpp.
extern pEffect SpellEffects[TOTAL_SPELL_EFFECTS];
extern pAuraEffectHandler AuraEffectHandler[TOTAL_AURAS];

using namespace Acore::ChatCommands;

namespace
{
constexpr char const* ReportSchema = "coa-effective-spell-report-v1";
constexpr uint8 MaxClosureDepth = 6;
// Same candidate rule as the offline census: a MiscValue that names an existing spell on an
// extended-range effect or aura.
constexpr int32 FirstMiscSpellCandidate = 10000;
constexpr uint32 FirstExtendedAura = SPELL_AURA_ASCENSION_MOD_ABSORB_AMOUNT_PCT;
constexpr int32 FirstPrivateOverrideSelector = 20000;
constexpr int32 LastPrivateOverrideSelector = 20017;

enum RootSource : uint8
{
    SOURCE_CLASS_SPELL,
    SOURCE_LIVE_BASELINE_SPELL,
    SOURCE_LIVE_BASELINE_PROFICIENCY,
    SOURCE_CLASS_PROFICIENCY,
    SOURCE_TALENT_PROFICIENCY,
    SOURCE_UNRESOLVED_TRAINER_SPELL,
    SOURCE_COA_TALENT,
    SOURCE_COA_AUTOMATIC_ENTRY,
    SOURCE_PROGRESSION_RANK,
    SOURCE_TAUGHT_ABILITY,
    SOURCE_TALENT_REPLACEMENT,
    SOURCE_RACIAL_SKILL_LINE,
    SOURCE_PLAYER_CREATE,
    SOURCE_DBC_TALENT,
    SOURCE_LEARN_SPELL_EFFECT,
    SOURCE_CLASS_SKILL_LINE,
    SOURCE_SKILL_LINE_CLASS_MASK,
    MAX_ROOT_SOURCES
};

constexpr std::array<char const*, MAX_ROOT_SOURCES> RootSourceNames =
{
    "class_spell",
    "live_baseline_spell",
    "live_baseline_proficiency",
    "class_proficiency",
    "talent_proficiency",
    "unresolved_trainer_spell",
    "coa_talent",
    "coa_automatic_entry",
    "progression_rank",
    "taught_ability",
    "talent_replacement",
    "racial_skill_line",
    "player_create",
    "dbc_talent",
    "learn_spell_effect",
    "class_skill_line",
    "skill_line_class_mask"
};

enum SlotField : uint8
{
    FIELD_EFFECT,
    FIELD_AURA,
    FIELD_BASE_POINTS,
    FIELD_DIE_SIDES,
    FIELD_MISC,
    FIELD_MISC_B,
    FIELD_TRIGGER,
    FIELD_TARGET_A,
    FIELD_TARGET_B,
    FIELD_CLASS_MASK,
    MAX_SLOT_FIELDS
};

constexpr std::array<char const*, MAX_SLOT_FIELDS> SlotFieldNames =
{
    "effect", "aura", "base_points", "die_sides", "misc", "misc_b", "trigger", "target_a", "target_b", "class_mask"
};

uint32 ClassBit(uint32 classId)
{
    return classId < MAX_CLASSES && IsAscensionClass(uint8(classId)) ? uint32(1) << (classId - 1) : 0;
}

uint32 CustomClassMask()
{
    uint32 mask = 0;
    for (uint32 classId = CLASS_BARBARIAN; classId <= CLASS_SPIRIT_MAGE; ++classId)
        mask |= ClassBit(classId);
    return mask;
}

struct Root
{
    uint32 Sources = 0;
    uint32 Classes = 0;
    // False when every source is a skill line alone, without a module grant, talent or creation record.
    bool Grant = false;
};

void AddRoot(std::map<uint32, Root>& roots, uint32 spellId, RootSource source, uint32 classes)
{
    if (!spellId || !classes)
        return;
    Root& root = roots[spellId];
    root.Sources |= uint32(1) << source;
    root.Classes |= classes;
    root.Grant = root.Grant || (source != SOURCE_CLASS_SKILL_LINE && source != SOURCE_SKILL_LINE_CLASS_MASK);
}

bool IsVanitySkill(uint32 skillId)
{
    return skillId == SKILL_MOUNTS || skillId == SKILL_COMPANIONS;
}

std::map<uint32, Root> CollectRoots()
{
    std::map<uint32, Root> roots;
    for (auto const& entry : AscensionCompatData::ClassSpells)
        AddRoot(roots, entry.SpellId, SOURCE_CLASS_SPELL, ClassBit(entry.ClassId));
    for (auto const& entry : AscensionLiveBaseline::Spells)
        AddRoot(roots, entry.SpellId, SOURCE_LIVE_BASELINE_SPELL, ClassBit(entry.ClassId));
    for (auto const& entry : AscensionLiveBaseline::Proficiencies)
        AddRoot(roots, entry.SpellId, SOURCE_LIVE_BASELINE_PROFICIENCY, ClassBit(entry.ClassId));
    for (auto const& entry : AscensionCompatData::ClassProficiencies)
        AddRoot(roots, entry.SpellId, SOURCE_CLASS_PROFICIENCY, ClassBit(entry.ClassId));
    for (auto const& entry : AscensionCompatData::TalentProficiencies)
        AddRoot(roots, entry.ProficiencySpellId, SOURCE_TALENT_PROFICIENCY, ClassBit(entry.ClassId));
    for (auto const& entry : AscensionCompatData::UnresolvedTrainerSpells)
        AddRoot(roots, entry.SpellId, SOURCE_UNRESOLVED_TRAINER_SPELL, ClassBit(entry.ClassId));
    for (auto const& entry : AscensionCompatData::CoATalentEntries)
    {
        RootSource const source = (entry.AECost || entry.TECost) ? SOURCE_COA_TALENT : SOURCE_COA_AUTOMATIC_ENTRY;
        for (std::size_t index = 0; index < entry.SpellIds.size() && index < std::size_t(entry.SpellCount); ++index)
            AddRoot(roots, entry.SpellIds[index], source, ClassBit(entry.ClassId));
    }
    for (auto const& rank : AscensionProgression::Ranks)
        AddRoot(roots, rank.SpellId, SOURCE_PROGRESSION_RANK, ClassBit(rank.ClassId));
    for (auto const& entry : AscensionCompatData::TaughtAbilities)
        AddRoot(roots, entry.SpellId, SOURCE_TAUGHT_ABILITY, ClassBit(entry.ClassId));
    for (auto const& entry : AscensionCompatData::TalentReplacements)
        for (auto const& rank : entry.Ranks)
            AddRoot(roots, rank.SpellId, SOURCE_TALENT_REPLACEMENT, ClassBit(entry.ClassId));

    for (auto const& skill : AscensionRacialAbilities::Skills)
        for (SkillLineAbilityEntry const* ability : GetSkillLineAbilitiesBySkillLine(skill.SkillId))
            for (uint8 classId = CLASS_BARBARIAN; classId <= CLASS_SPIRIT_MAGE; ++classId)
                if (AscensionRacialAbilities::CanLearn(*ability, skill.RaceId, classId))
                    AddRoot(roots, ability->Spell, SOURCE_RACIAL_SKILL_LINE, ClassBit(classId));

    for (uint32 raceId = 1; raceId < RaceMgr::GetMaxRaces(); ++raceId)
        for (uint32 classId = CLASS_BARBARIAN; classId <= CLASS_SPIRIT_MAGE; ++classId)
            if (PlayerInfo const* info = sObjectMgr->GetPlayerInfo(raceId, classId))
            {
                for (uint32 spellId : info->customSpells)
                    AddRoot(roots, spellId, SOURCE_PLAYER_CREATE, ClassBit(classId));
                for (uint32 spellId : info->castSpells)
                    AddRoot(roots, spellId, SOURCE_PLAYER_CREATE, ClassBit(classId));
            }

    uint32 const customClasses = CustomClassMask();
    for (TalentEntry const* talent : sTalentStore)
        if (TalentTabEntry const* tab = sTalentTabStore.LookupEntry(talent->TalentTab))
            for (uint32 spellId : talent->RankID)
                AddRoot(roots, spellId, SOURCE_DBC_TALENT, tab->ClassMask & customClasses);

    for (auto const& skill : AscensionLiveBaseline::Skills)
    {
        SkillLineEntry const* line = sSkillLineStore.LookupEntry(skill.SkillId);
        if (!line || line->categoryId != SKILL_CATEGORY_CLASS || IsVanitySkill(skill.SkillId))
            continue;
        for (SkillLineAbilityEntry const* ability : GetSkillLineAbilitiesBySkillLine(skill.SkillId))
            if (!ability->ClassMask || (ability->ClassMask & ClassBit(skill.ClassId)))
                AddRoot(roots, ability->Spell, SOURCE_CLASS_SKILL_LINE, ClassBit(skill.ClassId));
    }
    for (SkillLineAbilityEntry const* ability : sSkillLineAbilityStore)
        if (!IsVanitySkill(ability->SkillLine))
            AddRoot(roots, ability->Spell, SOURCE_SKILL_LINE_CLASS_MASK, ability->ClassMask & customClasses);

    // Player::_addTalentAurasAndSpells and learnSpell teach SPELL_EFFECT_LEARN_SPELL targets with the spell.
    std::deque<uint32> pending;
    for (auto const& entry : roots)
        pending.push_back(entry.first);
    while (!pending.empty())
    {
        uint32 const spellId = pending.front();
        pending.pop_front();
        SpellInfo const* info = sSpellMgr->GetSpellInfo(spellId);
        if (!info)
            continue;
        Root const parent = roots[spellId];
        for (SpellEffectInfo const& effect : info->Effects)
        {
            if (effect.Effect != SPELL_EFFECT_LEARN_SPELL || !effect.TriggerSpell)
                continue;
            Root& child = roots[effect.TriggerSpell];
            Root const before = child;
            child.Sources |= uint32(1) << SOURCE_LEARN_SPELL_EFFECT;
            child.Classes |= parent.Classes;
            child.Grant = child.Grant || parent.Grant;
            if (child.Sources != before.Sources || child.Classes != before.Classes || child.Grant != before.Grant)
                pending.push_back(effect.TriggerSpell);
        }
    }
    return roots;
}

struct Edge
{
    uint32 To;
    int8 Slot;
    char const* Kind;
    bool Raw;
};

bool IsMiscSpellCandidate(uint32 effect, uint32 aura, int32 misc)
{
    bool const extendedEffect = effect >= SPELL_EFFECT_REMOVE_AURA && effect <= SPELL_EFFECT_ASCENSION_LAST;
    bool const extendedAura = aura >= FirstExtendedAura && aura <= SPELL_AURA_ASCENSION_LAST;
    return (extendedEffect || extendedAura) && misc >= FirstMiscSpellCandidate &&
        sSpellMgr->GetSpellInfo(uint32(misc)) != nullptr;
}

struct LinkedLookup
{
    char const* Kind;
    int32 Key;
};

// The runtime lookups: Spell::_cast (cast), Spell::DoTriggersOnSpellHit (hit),
// Aura::HandleAuraSpecificMods (aura apply, and remove on -id).
std::array<LinkedLookup, 4> LinkedLookups(uint32 spellId)
{
    return {{
        { "cast", int32(spellId) },
        { "hit", int32(spellId) + SPELL_LINK_HIT },
        { "aura", int32(spellId) + SPELL_LINK_AURA },
        { "remove", -int32(spellId) }
    }};
}

class EdgeCache
{
public:
    std::vector<Edge> const& Get(SpellInfo const* info)
    {
        auto itr = _edges.find(info->Id);
        if (itr != _edges.end())
            return itr->second;
        std::vector<Edge> edges;
        SpellEntry const* raw = sSpellStore.LookupEntry(info->Id);
        for (uint8 index = 0; index < MAX_SPELL_EFFECTS; ++index)
        {
            // A slot without an effect never executes, so its payload fields are not edges.
            SpellEffectInfo const& effect = info->Effects[index];
            uint32 const trigger = effect.Effect ? effect.TriggerSpell : 0;
            if (trigger)
                edges.push_back({ trigger, int8(index), "trigger", false });
            bool const miscEdge = effect.Effect &&
                IsMiscSpellCandidate(effect.Effect, effect.ApplyAuraName, effect.MiscValue);
            if (miscEdge)
                edges.push_back({ uint32(effect.MiscValue), int8(index), "misc", false });
            if (!raw || !raw->Effect[index])
                continue;
            if (raw->EffectTriggerSpell[index] && raw->EffectTriggerSpell[index] != trigger)
                edges.push_back({ raw->EffectTriggerSpell[index], int8(index), "raw_trigger", true });
            int32 const rawMisc = raw->EffectMiscValue[index];
            if (IsMiscSpellCandidate(raw->Effect[index], raw->EffectApplyAuraName[index], rawMisc) &&
                (!miscEdge || rawMisc != effect.MiscValue))
                edges.push_back({ uint32(rawMisc), int8(index), "raw_misc", true });
        }
        for (LinkedLookup const& lookup : LinkedLookups(info->Id))
            if (std::vector<int32> const* linked = sSpellMgr->GetSpellLinked(lookup.Key))
                for (int32 target : *linked)
                    if (target > 0)
                        edges.push_back({ uint32(target), -1, lookup.Kind, false });
        return _edges.emplace(info->Id, std::move(edges)).first->second;
    }

private:
    std::unordered_map<uint32, std::vector<Edge>> _edges;
};

struct Reach
{
    bool Effective = false;
    bool Grant = false;
    uint8 Depth = 0;
    uint32 From = 0;
    int8 Slot = -1;
    char const* Kind = "root";
    uint32 Classes = 0;
    std::vector<uint32> Roots;
};

struct MissingSpell
{
    uint32 From;
    int8 Slot;
    char const* Kind;
};

// Pass one follows effective edges only. Pass two also follows raw DBC trigger/misc edges that a correction or
// contract rewrote, and records spells that only those raw edges reach.
void WalkClosure(std::map<uint32, Root> const& roots, bool includeRaw, EdgeCache& cache,
    std::map<uint32, Reach>& reached, std::map<uint32, MissingSpell>& missing)
{
    for (auto const& entry : roots)
    {
        // Named variables rather than structured bindings: the lambda below captures them.
        uint32 const rootId = entry.first;
        Root const& root = entry.second;
        std::unordered_map<uint32, uint8> depths;
        std::deque<uint32> queue;
        auto visit = [&](uint32 spellId, uint8 depth, uint32 from, int8 slot, char const* kind)
        {
            if (!depths.emplace(spellId, depth).second)
                return;
            if (!sSpellMgr->GetSpellInfo(spellId))
            {
                missing.try_emplace(spellId, MissingSpell{ from, slot, kind });
                return;
            }
            queue.push_back(spellId);
            auto [itr, inserted] = reached.try_emplace(spellId);
            Reach& reach = itr->second;
            if (includeRaw && reach.Effective)
                return;
            if (!includeRaw)
                reach.Effective = true;
            if (inserted || depth < reach.Depth)
            {
                reach.Depth = depth;
                reach.From = from;
                reach.Slot = slot;
                reach.Kind = kind;
            }
            reach.Roots.push_back(rootId);
            reach.Classes |= root.Classes;
            reach.Grant = reach.Grant || root.Grant;
        };
        visit(rootId, 0, 0, -1, "root");
        while (!queue.empty())
        {
            uint32 const spellId = queue.front();
            queue.pop_front();
            uint8 const depth = depths[spellId];
            if (depth >= MaxClosureDepth)
                continue;
            for (Edge const& edge : cache.Get(sSpellMgr->GetSpellInfo(spellId)))
                if (includeRaw || !edge.Raw)
                    visit(edge.To, uint8(depth + 1), spellId, edge.Slot, edge.Kind);
        }
    }
}

struct SlotValues
{
    std::array<int64, FIELD_CLASS_MASK> Scalars{};
    std::array<uint32, 3> ClassMask{};
};

SlotValues EffectiveValues(SpellEffectInfo const& effect)
{
    SlotValues values;
    values.Scalars = { int64(effect.Effect), int64(effect.ApplyAuraName), effect.BasePoints, effect.DieSides,
        effect.MiscValue, effect.MiscValueB, int64(effect.TriggerSpell), int64(effect.TargetA.GetTarget()),
        int64(effect.TargetB.GetTarget()) };
    for (uint8 part = 0; part < 3; ++part)
        values.ClassMask[part] = effect.SpellClassMask[part];
    return values;
}

SlotValues RawValues(SpellEntry const& entry, uint8 index)
{
    SlotValues values;
    values.Scalars = { int64(entry.Effect[index]), int64(entry.EffectApplyAuraName[index]),
        entry.EffectBasePoints[index], entry.EffectDieSides[index], entry.EffectMiscValue[index],
        entry.EffectMiscValueB[index], int64(entry.EffectTriggerSpell[index]),
        int64(entry.EffectImplicitTargetA[index]), int64(entry.EffectImplicitTargetB[index]) };
    for (uint8 part = 0; part < 3; ++part)
        values.ClassMask[part] = entry.EffectSpellClassMask[index][part];
    return values;
}

uint32 DifferentFields(SlotValues const& effective, SlotValues const& raw)
{
    uint32 fields = 0;
    for (uint8 field = 0; field < FIELD_CLASS_MASK; ++field)
        if (effective.Scalars[field] != raw.Scalars[field])
            fields |= uint32(1) << field;
    if (effective.ClassMask != raw.ClassMask)
        fields |= uint32(1) << FIELD_CLASS_MASK;
    return fields;
}

char const* EffectTableKind(uint32 effect)
{
    if (effect >= TOTAL_SPELL_EFFECTS)
        return "out_of_range";
    pEffect const handler = SpellEffects[effect];
    if (!handler)
        return "missing";
    if (handler == &Spell::EffectNULL)
        return "null";
    if (handler == &Spell::EffectUnused)
        return "unused";
    return "handler";
}

// Effect 0 slots fail SpellEffectInfo::IsEffect and are never dispatched.
char const* EffectHandlerKind(uint32 effect)
{
    return effect ? EffectTableKind(effect) : "none";
}

char const* AuraHandlerKind(uint32 aura)
{
    if (aura >= TOTAL_AURAS)
        return "out_of_range";
    pAuraEffectHandler const handler = AuraEffectHandler[aura];
    if (!handler)
        return "missing";
    if (handler == &AuraEffect::HandleNULL)
        return "null";
    if (handler == &AuraEffect::HandleUnused)
        return "unused";
    if (handler == &AuraEffect::HandleNoImmediateEffect)
        return "no_immediate";
    return "handler";
}

bool HandlerSentinelsDistinct()
{
    pEffect const effectNull = &Spell::EffectNULL;
    pEffect const effectUnused = &Spell::EffectUnused;
    pAuraEffectHandler const auraNull = &AuraEffect::HandleNULL;
    pAuraEffectHandler const auraUnused = &AuraEffect::HandleUnused;
    pAuraEffectHandler const auraDeferred = &AuraEffect::HandleNoImmediateEffect;
    return effectNull != effectUnused && auraNull != auraUnused && auraNull != auraDeferred &&
        auraUnused != auraDeferred;
}

bool IsSilentKind(std::string_view kind)
{
    return kind == "null" || kind == "unused" || kind == "missing" || kind == "out_of_range";
}

bool IsAppliedKind(std::string_view kind)
{
    return kind != "none" && kind != "not_applied";
}

struct SlotRow
{
    uint8 Index = 0;
    SlotValues Effective;
    SlotValues Raw;
    uint32 RawFields = 0;
    char const* EffectHandler = "none";
    char const* AuraHandler = "none";
    bool TriggerAura = false;
};

struct LinkedRow
{
    char const* Kind;
    std::vector<int32> Spells;
    std::vector<uint32> KeyAliases;
};

struct SpellRow
{
    uint32 Id = 0;
    SpellInfo const* Info = nullptr;
    Reach const* Reached = nullptr;
    bool HasRaw = false;
    uint32 RawProcFlags = 0;
    uint32 RawProcChance = 0;
    bool ProcEntry = false;
    bool BonusData = false;
    std::vector<std::string> Scripts;
    std::vector<LinkedRow> Linked;
    std::vector<SlotRow> Slots;
};

// Other spells whose runtime spell_linked_spell key equals this key (SPELL_LINKED_MAX_SPELLS offsets).
std::vector<uint32> LinkedKeyAliases(uint32 spellId, char const* kind)
{
    std::vector<uint32> aliases;
    std::string_view const name = kind;
    int64 const id = spellId;
    if (name == "remove")
    {
        for (int64 type = 1; type <= 2; ++type)
            if (id - SPELL_LINKED_MAX_SPELLS * type > 0 &&
                sSpellMgr->GetSpellInfo(uint32(id - SPELL_LINKED_MAX_SPELLS * type)))
                aliases.push_back(uint32(id - SPELL_LINKED_MAX_SPELLS * type));
        return aliases;
    }
    int64 const ownType = name == "cast" ? 0 : (name == "hit" ? 1 : 2);
    int64 const key = id + SPELL_LINKED_MAX_SPELLS * ownType;
    for (int64 type = 0; type <= 2; ++type)
        if (type != ownType && key - SPELL_LINKED_MAX_SPELLS * type > 0 &&
            sSpellMgr->GetSpellInfo(uint32(key - SPELL_LINKED_MAX_SPELLS * type)))
            aliases.push_back(uint32(key - SPELL_LINKED_MAX_SPELLS * type));
    return aliases;
}

SpellRow BuildSpellRow(uint32 spellId, Reach const& reach)
{
    SpellRow row;
    row.Id = spellId;
    row.Info = sSpellMgr->GetSpellInfo(spellId);
    row.Reached = &reach;
    SpellEntry const* raw = sSpellStore.LookupEntry(spellId);
    row.HasRaw = raw != nullptr;
    if (raw)
    {
        row.RawProcFlags = raw->ProcFlags;
        row.RawProcChance = raw->ProcChance;
    }
    row.ProcEntry = sSpellMgr->GetSpellProcEntry(spellId) != nullptr;
    row.BonusData = sSpellMgr->GetSpellBonusData(spellId) != nullptr;
    SpellScriptsBounds const bounds = sObjectMgr->GetSpellScriptsBounds(spellId);
    for (auto itr = bounds.first; itr != bounds.second; ++itr)
        row.Scripts.push_back(sObjectMgr->GetScriptName(itr->second));
    for (LinkedLookup const& lookup : LinkedLookups(spellId))
        if (std::vector<int32> const* linked = sSpellMgr->GetSpellLinked(lookup.Key))
            row.Linked.push_back({ lookup.Kind, *linked, LinkedKeyAliases(spellId, lookup.Kind) });

    for (uint8 index = 0; index < MAX_SPELL_EFFECTS; ++index)
    {
        SpellEffectInfo const& effect = row.Info->Effects[index];
        if (!effect.Effect && (!raw || !raw->Effect[index]))
            continue;
        SlotRow slot;
        slot.Index = index;
        slot.Effective = EffectiveValues(effect);
        slot.Raw = raw ? RawValues(*raw, index) : slot.Effective;
        slot.RawFields = DifferentFields(slot.Effective, slot.Raw);
        slot.EffectHandler = EffectHandlerKind(effect.Effect);
        if (effect.IsAura())
            slot.AuraHandler = AuraHandlerKind(effect.ApplyAuraName);
        else if (effect.ApplyAuraName)
            slot.AuraHandler = "not_applied";
        slot.TriggerAura = effect.ApplyAuraName && SpellMgr::IsTriggerAura(effect.ApplyAuraName);
        row.Slots.push_back(slot);
    }
    return row;
}

using Counter = std::map<std::string, uint32>;

struct Metrics
{
    uint32 Spells = 0;
    uint32 Slots = 0;
    uint32 SilentSlots = 0;
    uint32 NoImmediateAuraSlots = 0;
    uint32 DummySlotsWithoutBindings = 0;
    uint32 ProcAuraSpellsWithoutProcEntry = 0;
    uint32 ProcAuraSlotsWithoutProcEntry = 0;
    uint32 TriggerAuraSpellsWithoutProcEntry = 0;
    uint32 SpellmodOpOutOfRangeSlots = 0;
    uint32 Aura112PrivateSelectorSlots = 0;
    uint32 Aura112ConvertedSelectorSlots = 0;
    uint32 RewrittenSlots = 0;
    uint32 RewrittenSpells = 0;
    uint32 SpellsWithScripts = 0;
    uint32 SpellsWithBonusData = 0;
    uint32 SpellsWithProcEntry = 0;
    uint32 LinkedKeysWithAliases = 0;
    Counter EffectHandlers;
    Counter AuraHandlers;
    Counter SilentByPrimitive;
    Counter Rewrites;
    Counter Masking;
};

void Accumulate(Metrics& metrics, SpellRow const& row)
{
    ++metrics.Spells;
    bool const bound = !row.Scripts.empty();
    bool procAura = false;
    bool triggerAura = false;
    bool rewritten = false;
    if (row.HasRaw && row.RawProcFlags != row.Info->ProcFlags)
    {
        ++metrics.Rewrites["proc_flags"];
        rewritten = true;
        if (row.RawProcFlags && !row.Info->ProcFlags)
            ++metrics.Masking["proc_flags_zeroed"];
    }
    if (row.HasRaw && row.RawProcChance != row.Info->ProcChance)
    {
        ++metrics.Rewrites["proc_chance"];
        rewritten = true;
    }
    for (SlotRow const& slot : row.Slots)
    {
        ++metrics.Slots;
        int64 const effect = slot.Effective.Scalars[FIELD_EFFECT];
        int64 const aura = slot.Effective.Scalars[FIELD_AURA];
        int64 const misc = slot.Effective.Scalars[FIELD_MISC];
        bool const applied = IsAppliedKind(slot.AuraHandler);
        if (effect)
            ++metrics.EffectHandlers[slot.EffectHandler];
        if (applied)
            ++metrics.AuraHandlers[slot.AuraHandler];
        bool silent = false;
        if (effect && IsSilentKind(slot.EffectHandler))
        {
            ++metrics.SilentByPrimitive[Acore::StringFormat("effect {}", effect)];
            silent = true;
        }
        if (applied && IsSilentKind(slot.AuraHandler))
        {
            ++metrics.SilentByPrimitive[Acore::StringFormat("aura {}", aura)];
            silent = true;
        }
        if (silent)
            ++metrics.SilentSlots;
        if (std::string_view(slot.AuraHandler) == "no_immediate")
            ++metrics.NoImmediateAuraSlots;
        bool const dummy = effect == SPELL_EFFECT_DUMMY ||
            (applied && (aura == SPELL_AURA_DUMMY || aura == SPELL_AURA_PERIODIC_DUMMY));
        if (dummy && !bound)
            ++metrics.DummySlotsWithoutBindings;
        bool const procTrigger = applied && (aura == SPELL_AURA_PROC_TRIGGER_SPELL ||
            aura == SPELL_AURA_PROC_TRIGGER_DAMAGE || aura == SPELL_AURA_PROC_TRIGGER_SPELL_WITH_VALUE);
        if (procTrigger)
        {
            procAura = true;
            if (!row.ProcEntry)
                ++metrics.ProcAuraSlotsWithoutProcEntry;
        }
        triggerAura = triggerAura || (applied && slot.TriggerAura);
        if (applied && (aura == SPELL_AURA_ADD_FLAT_MODIFIER || aura == SPELL_AURA_ADD_PCT_MODIFIER) &&
            (misc < 0 || misc >= MAX_SPELLMOD))
            ++metrics.SpellmodOpOutOfRangeSlots;
        if (applied && aura == SPELL_AURA_OVERRIDE_CLASS_SCRIPTS)
        {
            if (misc >= FirstPrivateOverrideSelector && misc <= LastPrivateOverrideSelector)
                ++metrics.Aura112PrivateSelectorSlots;
            if (misc >= ASCENSION_STATE_MASKED_CRIT && misc <= ASCENSION_STATE_MASKED_AND_AUTO_CRIT)
                ++metrics.Aura112ConvertedSelectorSlots;
        }
        if (slot.RawFields)
        {
            ++metrics.RewrittenSlots;
            rewritten = true;
            for (uint8 field = 0; field < MAX_SLOT_FIELDS; ++field)
                if (slot.RawFields & (uint32(1) << field))
                    ++metrics.Rewrites[SlotFieldNames[field]];
            int64 const rawEffect = slot.Raw.Scalars[FIELD_EFFECT];
            int64 const rawAura = slot.Raw.Scalars[FIELD_AURA];
            if (rawEffect && !effect)
                ++metrics.Masking["effect_cleared"];
            if (rawEffect && rawEffect != SPELL_EFFECT_DUMMY && effect == SPELL_EFFECT_DUMMY)
                ++metrics.Masking["effect_to_dummy"];
            if (rawAura && rawAura != SPELL_AURA_DUMMY && aura == SPELL_AURA_DUMMY && applied)
                ++metrics.Masking["aura_to_dummy"];
            if (slot.Raw.Scalars[FIELD_TRIGGER] && !slot.Effective.Scalars[FIELD_TRIGGER])
                ++metrics.Masking["trigger_cleared"];
        }
    }
    if (procAura && !row.ProcEntry)
        ++metrics.ProcAuraSpellsWithoutProcEntry;
    if (triggerAura && !row.ProcEntry)
        ++metrics.TriggerAuraSpellsWithoutProcEntry;
    if (rewritten)
        ++metrics.RewrittenSpells;
    if (bound)
        ++metrics.SpellsWithScripts;
    if (row.BonusData)
        ++metrics.SpellsWithBonusData;
    if (row.ProcEntry)
        ++metrics.SpellsWithProcEntry;
    for (LinkedRow const& linked : row.Linked)
        if (!linked.KeyAliases.empty())
            ++metrics.LinkedKeysWithAliases;
}

std::string JsonString(std::string_view text)
{
    std::string valid;
    if (!utf8::is_valid(text.begin(), text.end()))
    {
        utf8::replace_invalid(text.begin(), text.end(), std::back_inserter(valid));
        text = valid;
    }
    std::string result = "\"";
    for (char character : text)
    {
        unsigned char const byte = static_cast<unsigned char>(character);
        if (character == '"' || character == '\\')
        {
            result += '\\';
            result += character;
        }
        else if (byte < 0x20)
            result += Acore::StringFormat("\\u{:04x}", uint32(byte));
        else
            result += character;
    }
    result += '"';
    return result;
}

template <typename Container>
void WriteArray(std::ostream& output, Container const& values)
{
    output << '[';
    bool separator = false;
    for (auto const& value : values)
    {
        if (separator)
            output << ',';
        separator = true;
        output << value;
    }
    output << ']';
}

void WriteCounter(std::ostream& output, Counter const& counter)
{
    output << '{';
    bool separator = false;
    for (auto const& [key, value] : counter)
    {
        if (separator)
            output << ',';
        separator = true;
        output << JsonString(key) << ':' << value;
    }
    output << '}';
}

void WriteMetrics(std::ostream& output, Metrics const& metrics)
{
    output << "{\"spells\":" << metrics.Spells << ",\"slots\":" << metrics.Slots
           << ",\"silent_slots\":" << metrics.SilentSlots
           << ",\"no_immediate_aura_slots\":" << metrics.NoImmediateAuraSlots
           << ",\"dummy_slots_without_bindings\":" << metrics.DummySlotsWithoutBindings
           << ",\"proc_aura_spells_without_proc_entry\":" << metrics.ProcAuraSpellsWithoutProcEntry
           << ",\"proc_aura_slots_without_proc_entry\":" << metrics.ProcAuraSlotsWithoutProcEntry
           << ",\"trigger_aura_spells_without_proc_entry\":" << metrics.TriggerAuraSpellsWithoutProcEntry
           << ",\"spellmod_op_out_of_range_slots\":" << metrics.SpellmodOpOutOfRangeSlots
           << ",\"aura112_private_selector_slots\":" << metrics.Aura112PrivateSelectorSlots
           << ",\"aura112_converted_selector_slots\":" << metrics.Aura112ConvertedSelectorSlots
           << ",\"rewritten_slots\":" << metrics.RewrittenSlots
           << ",\"rewritten_spells\":" << metrics.RewrittenSpells
           << ",\"spells_with_scripts\":" << metrics.SpellsWithScripts
           << ",\"spells_with_bonus_data\":" << metrics.SpellsWithBonusData
           << ",\"spells_with_proc_entry\":" << metrics.SpellsWithProcEntry
           << ",\"linked_keys_with_aliases\":" << metrics.LinkedKeysWithAliases
           << ",\"effect_handlers\":";
    WriteCounter(output, metrics.EffectHandlers);
    output << ",\"aura_handlers\":";
    WriteCounter(output, metrics.AuraHandlers);
    output << ",\"silent_by_primitive\":";
    WriteCounter(output, metrics.SilentByPrimitive);
    output << ",\"rewrites\":";
    WriteCounter(output, metrics.Rewrites);
    output << ",\"masking\":";
    WriteCounter(output, metrics.Masking);
    output << '}';
}

void WriteClasses(std::ostream& output, uint32 classes)
{
    std::vector<uint32> ids;
    for (uint32 classId = CLASS_BARBARIAN; classId <= CLASS_SPIRIT_MAGE; ++classId)
        if (classes & ClassBit(classId))
            ids.push_back(classId);
    WriteArray(output, ids);
}

void WriteSlot(std::ostream& output, SlotRow const& slot)
{
    output << "{\"slot\":" << uint32(slot.Index);
    for (uint8 field = 0; field < FIELD_CLASS_MASK; ++field)
        output << ",\"" << SlotFieldNames[field] << "\":" << slot.Effective.Scalars[field];
    output << ",\"class_mask\":";
    WriteArray(output, slot.Effective.ClassMask);
    output << ",\"effect_handler\":\"" << slot.EffectHandler << "\",\"aura_handler\":\"" << slot.AuraHandler
           << "\",\"trigger_aura\":" << (slot.TriggerAura ? "true" : "false") << ",\"raw\":{";
    bool separator = false;
    for (uint8 field = 0; field < MAX_SLOT_FIELDS; ++field)
    {
        if (!(slot.RawFields & (uint32(1) << field)))
            continue;
        if (separator)
            output << ',';
        separator = true;
        output << '"' << SlotFieldNames[field] << "\":";
        if (field == FIELD_CLASS_MASK)
            WriteArray(output, slot.Raw.ClassMask);
        else
            output << slot.Raw.Scalars[field];
    }
    output << "}}";
}

void WriteSpell(std::ostream& output, SpellRow const& row)
{
    Reach const& reach = *row.Reached;
    output << "{\"id\":" << row.Id << ",\"name\":" << JsonString(row.Info->SpellName[0] ? row.Info->SpellName[0] : "")
           << ",\"effective\":" << (reach.Effective ? "true" : "false") << ",\"depth\":" << uint32(reach.Depth)
           << ",\"via\":";
    if (reach.Depth)
        output << "{\"from\":" << reach.From << ",\"slot\":" << int32(reach.Slot) << ",\"kind\":\"" << reach.Kind
               << "\"}";
    else
        output << "null";
    output << ",\"root_scope\":\"" << (reach.Grant ? "grant" : "skill_line") << "\",\"classes\":";
    WriteClasses(output, reach.Classes);
    output << ",\"roots\":";
    WriteArray(output, reach.Roots);
    output << ",\"proc_flags\":" << row.Info->ProcFlags << ",\"proc_chance\":" << row.Info->ProcChance
           << ",\"proc_entry\":" << (row.ProcEntry ? "true" : "false")
           << ",\"bonus_data\":" << (row.BonusData ? "true" : "false") << ",\"scripts\":[";
    for (std::size_t index = 0; index < row.Scripts.size(); ++index)
        output << (index ? "," : "") << JsonString(row.Scripts[index]);
    output << "],\"rank\":";
    if (SpellChainNode const* node = sSpellMgr->GetSpellChainNode(row.Id))
        output << "{\"first\":" << (node->first ? node->first->Id : 0)
               << ",\"prev\":" << (node->prev ? node->prev->Id : 0)
               << ",\"next\":" << (node->next ? node->next->Id : 0)
               << ",\"last\":" << (node->last ? node->last->Id : 0) << ",\"rank\":" << uint32(node->rank) << '}';
    else
        output << "null";
    output << ",\"linked\":[";
    for (std::size_t index = 0; index < row.Linked.size(); ++index)
    {
        LinkedRow const& linked = row.Linked[index];
        output << (index ? "," : "") << "{\"kind\":\"" << linked.Kind << "\",\"spells\":";
        WriteArray(output, linked.Spells);
        output << ",\"key_aliases\":";
        WriteArray(output, linked.KeyAliases);
        output << '}';
    }
    output << "],\"raw\":";
    if (!row.HasRaw)
        output << "null";
    else
    {
        output << '{';
        bool separator = false;
        if (row.RawProcFlags != row.Info->ProcFlags)
        {
            output << "\"proc_flags\":" << row.RawProcFlags;
            separator = true;
        }
        if (row.RawProcChance != row.Info->ProcChance)
            output << (separator ? "," : "") << "\"proc_chance\":" << row.RawProcChance;
        output << '}';
    }
    output << ",\"slots\":[";
    for (std::size_t index = 0; index < row.Slots.size(); ++index)
    {
        if (index)
            output << ',';
        WriteSlot(output, row.Slots[index]);
    }
    output << "]}";
}

std::string WriteReport(std::filesystem::path const& path)
{
    if (!HandlerSentinelsDistinct())
        throw std::runtime_error("handler sentinels share an address; handler kinds cannot be classified");

    std::map<uint32, Root> const roots = CollectRoots();
    EdgeCache cache;
    std::map<uint32, Reach> reached;
    std::map<uint32, MissingSpell> missing;
    WalkClosure(roots, false, cache, reached, missing);
    WalkClosure(roots, true, cache, reached, missing);

    std::array<uint32, MAX_ROOT_SOURCES> rootsBySource{};
    uint32 missingRoots = 0;
    for (auto const& [spellId, root] : roots)
    {
        if (!sSpellMgr->GetSpellInfo(spellId))
            ++missingRoots;
        for (uint8 source = 0; source < MAX_ROOT_SOURCES; ++source)
            if (root.Sources & (uint32(1) << source))
                ++rootsBySource[source];
    }

    std::vector<SpellRow> rows;
    rows.reserve(reached.size());
    Metrics all;
    Metrics grant;
    std::map<uint32, Metrics> perClass;
    uint32 rawOnly = 0;
    for (auto const& [spellId, reach] : reached)
    {
        rows.push_back(BuildSpellRow(spellId, reach));
        if (!reach.Effective)
        {
            ++rawOnly;
            continue;
        }
        Accumulate(all, rows.back());
        if (reach.Grant)
            Accumulate(grant, rows.back());
        for (uint32 classId = CLASS_BARBARIAN; classId <= CLASS_SPIRIT_MAGE; ++classId)
            if (reach.Classes & ClassBit(classId))
                Accumulate(perClass[classId], rows.back());
    }

    std::filesystem::path temporary = path;
    temporary += ".tmp";
    {
        std::ofstream output(temporary, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!output)
            throw std::runtime_error("cannot open " + temporary.string());
        output.imbue(std::locale::classic());

        output << "{\"schema\":\"" << ReportSchema << "\",\"server\":" << JsonString(GitRevision::GetFullVersion())
               << ",\"generated_unix\":" << GameTime::GetGameTime().count()
               << ",\"max_depth\":" << uint32(MaxClosureDepth)
               << ",\"limits\":[\"Roots: module grant tables, player create spells, DBC talents, racial and class "
                  "skill lines and SPELL_EFFECT_LEARN_SPELL targets; other C++ grants are not roots\","
                  "\"Edges: effective TriggerSpell, MiscValue spell candidates on effects 164-198 and auras 317-366, "
                  "positive spell_linked_spell runtime lookups; C++ CastSpell literals and summon AI casts are not "
                  "followed\",\"Bindings: spell_script_names only; global hooks and core id switches are invisible\","
                  "\"proc_entry includes spell_proc rows and entries generated from DBC ProcFlags\"],";

        output << "\"dispatch\":{\"effects\":[";
        for (uint32 effect = 0; effect < TOTAL_SPELL_EFFECTS; ++effect)
            output << (effect ? "," : "") << '"' << EffectTableKind(effect) << '"';
        output << "],\"auras\":[";
        for (uint32 aura = 0; aura < TOTAL_AURAS; ++aura)
            output << (aura ? "," : "") << '"' << AuraHandlerKind(aura) << '"';
        output << "],\"trigger_auras\":[";
        bool separator = false;
        for (uint32 aura = 0; aura < TOTAL_AURAS; ++aura)
            if (SpellMgr::IsTriggerAura(aura))
            {
                output << (separator ? "," : "") << aura;
                separator = true;
            }
        output << "]},\n\"roots\":[\n";
        separator = false;
        for (auto const& [spellId, root] : roots)
        {
            output << (separator ? ",\n" : "") << "{\"spell\":" << spellId << ",\"exists\":"
                   << (sSpellMgr->GetSpellInfo(spellId) ? "true" : "false")
                   << ",\"grant\":" << (root.Grant ? "true" : "false") << ",\"classes\":";
            separator = true;
            WriteClasses(output, root.Classes);
            output << ",\"sources\":[";
            bool sourceSeparator = false;
            for (uint8 source = 0; source < MAX_ROOT_SOURCES; ++source)
                if (root.Sources & (uint32(1) << source))
                {
                    output << (sourceSeparator ? "," : "") << '"' << RootSourceNames[source] << '"';
                    sourceSeparator = true;
                }
            output << "]}";
        }

        output << "\n],\n\"summary\":{\"roots\":" << roots.size() << ",\"missing_roots\":" << missingRoots
               << ",\"roots_by_source\":{";
        for (uint8 source = 0; source < MAX_ROOT_SOURCES; ++source)
            output << (source ? "," : "") << '"' << RootSourceNames[source] << "\":" << rootsBySource[source];
        output << "},\"closure_spells\":" << rows.size() << ",\"raw_only_spells\":" << rawOnly
               << ",\"missing_spells\":" << missing.size() << ",\"all\":";
        WriteMetrics(output, all);
        output << ",\"grant\":";
        WriteMetrics(output, grant);
        output << ",\"per_class\":{";
        separator = false;
        for (auto const& [classId, metrics] : perClass)
        {
            output << (separator ? "," : "") << '"' << classId << "\":{\"spells\":" << metrics.Spells
                   << ",\"silent_slots\":" << metrics.SilentSlots
                   << ",\"dummy_slots_without_bindings\":" << metrics.DummySlotsWithoutBindings
                   << ",\"proc_aura_spells_without_proc_entry\":" << metrics.ProcAuraSpellsWithoutProcEntry
                   << ",\"rewritten_spells\":" << metrics.RewrittenSpells << '}';
            separator = true;
        }
        output << "}},\n\"missing_spells\":[";
        separator = false;
        for (auto const& [spellId, entry] : missing)
        {
            output << (separator ? "," : "") << "{\"id\":" << spellId << ",\"from\":" << entry.From
                   << ",\"slot\":" << int32(entry.Slot) << ",\"kind\":\"" << entry.Kind << "\"}";
            separator = true;
        }
        output << "],\n\"spells\":[\n";
        for (std::size_t index = 0; index < rows.size(); ++index)
        {
            if (index)
                output << ",\n";
            WriteSpell(output, rows[index]);
        }
        output << "\n]}\n";
        output.close();
        if (!output)
            throw std::runtime_error("cannot write " + temporary.string());
    }
    std::filesystem::rename(temporary, path);

    return Acore::StringFormat("{} roots ({} missing), {} closure spells ({} raw-only), {} missing spells, {} slots, "
        "{} silent slots, {} dummy slots without bindings, {} proc-aura spells without spell_proc, {} rewritten slots",
        roots.size(), missingRoots, rows.size(), rawOnly, missing.size(), all.Slots, all.SilentSlots,
        all.DummySlotsWithoutBindings, all.ProcAuraSpellsWithoutProcEntry, all.RewrittenSlots);
}
}

class AscensionEffectiveSpellReportCommands final : public CommandScript
{
public:
    AscensionEffectiveSpellReportCommands() : CommandScript("AscensionEffectiveSpellReportCommands") { }

    ChatCommandTable GetCommands() const override
    {
        // ChatCommandBuilder retains a reference to its child table.
        static ChatCommandTable const coaCommands =
        {
            { "spellreport", HandleSpellReportCommand, SEC_CONSOLE, Console::Yes }
        };
        static ChatCommandTable const commands =
        {
            { "coa", coaCommands }
        };
        return commands;
    }

    static bool HandleSpellReportCommand(ChatHandler* handler, Tail file)
    {
        if (handler->GetSession())
        {
            handler->SendErrorMessage("The effective spell report is available from the server console only.");
            return false;
        }
        std::string_view path = file;
        while (!path.empty() && path.back() == ' ')
            path.remove_suffix(1);
        if (path.empty())
        {
            handler->SendErrorMessage("Usage: coa spellreport <output.json>");
            return false;
        }
        try
        {
            std::string const summary = WriteReport(std::filesystem::path(std::string(path)));
            handler->PSendSysMessage("Effective spell report written to {}: {}", path, summary);
            return true;
        }
        catch (std::exception const& error)
        {
            handler->SendErrorMessage("Effective spell report failed: {}", error.what());
            return false;
        }
    }
};

class AscensionEffectiveSpellReportWorld final : public WorldScript
{
public:
    AscensionEffectiveSpellReportWorld()
        : WorldScript("AscensionEffectiveSpellReportWorld", { WORLDHOOK_ON_STARTUP }) { }

    void OnStartup() override
    {
        std::string const file = sConfigMgr->GetOption<std::string>("AscensionCompat.SpellReport.StartupFile", "",
            false);
        if (file.empty())
            return;
        try
        {
            // Write outside the log macro, which skips its arguments when the level is disabled.
            std::string const summary = WriteReport(std::filesystem::path(file));
            LOG_INFO("module.ascension_compat", "Effective spell report written to {}: {}", file, summary);
        }
        catch (std::exception const& error)
        {
            LOG_ERROR("module.ascension_compat", "Effective spell report failed: {}", error.what());
        }
    }
};

void AddAscensionEffectiveSpellReportScripts()
{
    new AscensionEffectiveSpellReportCommands();
    new AscensionEffectiveSpellReportWorld();
}
