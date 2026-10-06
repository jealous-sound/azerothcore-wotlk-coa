# Adds Wardrobe incarnations to the core: the model picked in the Wardrobe's Incarnations tab is
# applied to the matching shapeshift form, and the Venomancer's forms wear the druid incarnations.
# Run from anywhere: python apply_incarnations.py C:\CoA-Build\core
import sys, pathlib

core = pathlib.Path(sys.argv[1] if len(sys.argv) > 1 else r'C:\CoA-Build\core')
coa = core / 'src/server/coa/AscensionCompat.cpp'
unit = core / 'src/server/game/Entities/Unit/Unit.cpp'
auras = core / 'src/server/game/Spells/Auras/SpellAuraEffects.cpp'
header = core / 'src/server/game/Entities/Unit/AscensionIncarnation.h'

MARK = 'AscensionIncarnation'


def patch(path, old, new, count=1, key=None):
    text = path.read_text(encoding='utf-8')
    if (MARK in text and new.strip() in text) or (key and key in text):
        return
    assert text.count(old) == count, (path.name, old[:60], text.count(old))
    path.write_text(text.replace(old, new), encoding='utf-8', newline='')


header.write_text('''#ifndef ASCENSION_INCARNATION_H
#define ASCENSION_INCARNATION_H

#include "Define.h"

class Player;

/// Model of the Wardrobe incarnation the player wears for this shapeshift form or form spell,
/// or 0 when none is selected (the form keeps its own model).
uint32 GetAscensionIncarnationDisplay(Player const* player, uint32 form, uint32 spellId);

/// Re-applies the incarnation model when the player changes it while shapeshifted.
void RefreshAscensionIncarnationDisplay(Player* player);

#endif
''', encoding='utf-8')

# --- AscensionCompat.cpp: keep the model of every incarnation appearance ---
patch(coa, '''    _appearances.clear();
''', '''    _appearances.clear();
    _incarnationDisplays.clear();
''')
patch(coa, '''      uint32 displayId = record.GetUInt32(3);
''', '''      uint32 displayId = record.GetUInt32(3);
      // AscensionIncarnation: creature-type incarnations carry their model in field 8.
      if (IsIncarnationCategory(record.GetUInt32(5)) &&
          sCreatureDisplayInfoStore.LookupEntry(record.GetUInt32(8)))
        _incarnationDisplays[appearanceId] = record.GetUInt32(8);
''')
patch(coa, '''  std::unordered_map<uint32, AppearanceInfo> _appearances;
''', '''  std::unordered_map<uint32, AppearanceInfo> _appearances;
  std::unordered_map<uint32, uint32> _incarnationDisplays; // AscensionIncarnation: appearance -> model
''')
patch(coa, '''class AscensionCollectionService {
public:
''', '''class AscensionCollectionService {
public:
    // AscensionIncarnation: Wardrobe categories that dress a shapeshift form or a pet.
    static bool IsIncarnationCategory(uint32 category)
    {
        return (category >= 17 && category <= 31) || (category >= 65 && category <= 68);
    }

    uint32 GetIncarnationDisplay(Player const* player, uint32 category)
    {
        if (category >= APPEARANCE_CATEGORY_COUNT)
            return 0;
        std::shared_ptr<PlayerCollectionState> state = GetState(player);
        if (!state)
            return 0;
        uint32 const appearanceId = state->ActiveAppearances[category];
        auto const itr = _incarnationDisplays.find(appearanceId);
        return appearanceId && itr != _incarnationDisplays.end() ? itr->second : 0;
    }

''')
# refresh when the selection changes and when the state arrives at login
patch(coa, '''    state->ActiveAppearances = requested;
    SaveActiveAppearances(player, *state);
''', '''    state->ActiveAppearances = requested;
    SaveActiveAppearances(player, *state);
    RefreshAscensionIncarnationDisplay(player);
''')
patch(coa, '''      _playerStates[player->GetGUID().GetCounter()] = state;
    }
''', '''      _playerStates[player->GetGUID().GetCounter()] = state;
    }
    RefreshAscensionIncarnationDisplay(player);
''')
patch(coa, '''void AddAscensionCompatScripts() {
''', key='static uint32 IncarnationCategoryFor(', new='''// AscensionIncarnation ---------------------------------------------------------------------
// Wardrobe category a form wears. The Venomancer's forms borrow the druid ones.
static uint32 IncarnationCategoryFor(uint32 form, uint32 spellId)
{
    switch (spellId)
    {
        case 800841: return 18; // Spider Form      -> Cat Form
        case 803183: return 17; // Beetle Form      -> Bear Form
        case 520307: return 19; // Venomwing Form   -> Travel Form
        case 803212: return 20; // Sea Serpent Form -> Aquatic Form
        case 800912: return 22; // Vizier Form      -> Moonkin Form
        default: break;
    }

    switch (form)
    {
        case FORM_BEAR:
        case FORM_DIREBEAR:      return 17;
        case FORM_CAT:           return 18;
        case FORM_TRAVEL:        return 19;
        case FORM_AQUA:          return 20;
        case FORM_FLIGHT:
        case FORM_FLIGHT_EPIC:   return 21;
        case FORM_MOONKIN:       return 22;
        case FORM_TREE:          return 23;
        case FORM_GHOSTWOLF:     return 24;
        case FORM_METAMORPHOSIS: return 31;
        default:                 return 0;
    }
}

uint32 GetAscensionIncarnationDisplay(Player const* player, uint32 form, uint32 spellId)
{
    if (!player || !ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED))
        return 0;

    uint32 const category = IncarnationCategoryFor(form, spellId);
    return category ? AscensionCollectionService::Instance().GetIncarnationDisplay(player, category) : 0;
}

void RefreshAscensionIncarnationDisplay(Player* player)
{
    if (!player || !player->IsInWorld())
        return;

    // Shapeshift forms
    Unit::AuraEffectList const& shapeshifts = player->GetAuraEffectsByType(SPELL_AURA_MOD_SHAPESHIFT);
    if (!shapeshifts.empty() && !player->getTransForm())
    {
        if (uint32 model = player->GetModelForForm(player->GetShapeshiftForm(), shapeshifts.front()->GetId()))
            player->SetDisplayId(model);
        return;
    }

    // Forms that are a transform (Sea Serpent Form)
    if (uint32 transform = player->getTransForm())
        if (uint32 model = GetAscensionIncarnationDisplay(player, FORM_NONE, transform))
            player->SetDisplayId(model);
}

void AddAscensionCompatScripts() {
''')
patch(coa, '''#include "AscensionCollectionModelData.h"
''', '''#include "AscensionCollectionModelData.h"
#include "AscensionIncarnation.h"
#include "SpellAuraEffects.h"
''')

# --- Unit.cpp: forms use the incarnation model first ---
patch(unit, '''    if (IsPlayer())
    {
        if (uint32 ModelId = sObjectMgr->GetModelForShapeshift(form, ToPlayer()))
            return ModelId;
    }
''', '''    if (IsPlayer())
    {
        // AscensionIncarnation: the Wardrobe incarnation replaces the form's model.
        if (uint32 incarnation = GetAscensionIncarnationDisplay(ToPlayer(), form, spellId))
            return incarnation;

        if (uint32 ModelId = sObjectMgr->GetModelForShapeshift(form, ToPlayer()))
            return ModelId;
    }
''')
patch(unit, '#include "Unit.h"\n', '#include "Unit.h"\n#include "AscensionIncarnation.h"\n')

# --- SpellAuraEffects.cpp: transform-based forms (Sea Serpent Form) too ---
patch(auras, '''                    if (uint32 modelid = ObjectMgr::ChooseDisplayId(ci)->CreatureDisplayID)
                        model_id = modelid;                     // Will use the default model here
''', '''                    if (uint32 modelid = ObjectMgr::ChooseDisplayId(ci)->CreatureDisplayID)
                        model_id = modelid;                     // Will use the default model here

                    // AscensionIncarnation: a form that is a transform wears its Wardrobe incarnation.
                    if (Player* player = target->ToPlayer())
                        if (uint32 incarnation = GetAscensionIncarnationDisplay(player, FORM_NONE, GetId()))
                            model_id = incarnation;
''')
patch(auras, '#include "SpellAuraEffects.h"\n', '#include "SpellAuraEffects.h"\n#include "AscensionIncarnation.h"\n')
print('incarnation patch applied to', core)

# --- stage 2: summoned pets, CoA spell forms -----------------------------------------------
patch(coa, key='IncarnationCategoryForCreature', old='''        case FORM_METAMORPHOSIS: return 31;
        default:                 return 0;
    }
}
''', new='''        case FORM_METAMORPHOSIS: return 31;
        case 55:                 return 66; // Pyromancer Draconic Form
        case 50:                 return 67; // Necromancer Lich Form (CoA)
        default:                 return 0;
    }
}

// Wardrobe category a summoned creature wears (normal and Bronzebeard summons).
static uint32 IncarnationCategoryForCreature(uint32 entry)
{
    switch (entry)
    {
        case 416:   case 1100416: return 25; // Imp
        case 1860:  case 1101860: return 26; // Voidwalker
        case 1863:  case 1101863: return 27; // Succubus
        case 417:   case 1100417: return 28; // Felhunter
        case 17252: case 1117252: return 29; // Felguard
        case 1793:                return 68; // Shadowhound
        default:                  return 0;
    }
}

void ApplyAscensionCreatureIncarnation(Creature* creature)
{
    if (!creature || !ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED))
        return;

    uint32 const category = IncarnationCategoryForCreature(creature->GetEntry());
    if (!category)
        return;

    Player* owner = creature->GetCharmerOrOwnerPlayerOrPlayerItself();
    if (!owner)
        return;

    if (uint32 model = AscensionCollectionService::Instance().GetIncarnationDisplay(owner, category))
    {
        creature->SetNativeDisplayId(model);
        creature->SetDisplayId(model);
    }
}

class AscensionIncarnationPetScript : public PetScript
{
public:
    AscensionIncarnationPetScript() : PetScript("AscensionIncarnationPetScript", { PETHOOK_ON_PET_ADD_TO_WORLD }) { }

    void OnPetAddToWorld(Pet* pet) override { ApplyAscensionCreatureIncarnation(pet); }
};

class AscensionIncarnationCreatureScript : public AllCreatureScript
{
public:
    AscensionIncarnationCreatureScript() : AllCreatureScript("AscensionIncarnationCreatureScript") { }

    void OnCreatureAddWorld(Creature* creature) override
    {
        if (creature->IsSummon())
            ApplyAscensionCreatureIncarnation(creature);
    }
};
''')
patch(coa, key='    // Summoned pet', old='''    // Shapeshift forms
    Unit::AuraEffectList const& shapeshifts''', new='''    // Summoned pet
    if (Pet* pet = player->GetPet())
        if (pet->IsInWorld())
            ApplyAscensionCreatureIncarnation(pet);

    // Shapeshift forms
    Unit::AuraEffectList const& shapeshifts''')
patch(coa, key='new AscensionIncarnationPetScript', old='''  new AscensionCompatAllCreatureScript();
}
''', new='''  new AscensionCompatAllCreatureScript();
  new AscensionIncarnationPetScript();
  new AscensionIncarnationCreatureScript();
}
''')
print('incarnation stage 2 applied')

patch(coa, '''#include "AscensionIncarnation.h"
#include "SpellAuraEffects.h"
''', '''#include "AscensionIncarnation.h"
#include "SpellAuraEffects.h"
#include "Pet.h"
''')
print('pet include applied')
