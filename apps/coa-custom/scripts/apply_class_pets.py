# Core patch: Wardrobe "class pet" incarnations (categories 33-37) for the vanilla classes.
# Ascension's classless realm gives each pet-summoning spell a Wardrobe slot: Call Pet (33, hunter), Summon Demon
# (34, warlock), Raise Undead (35), Call Dragonkin (36, mage), Raise Elemental (37, shaman). Their appearances
# name a creature (Appearances.dbc field 3) instead of a model. The look is put on the class's real pets:
#   hunter pet -> 33; warlock demons -> their own slot (25-29), else 34; Water Elemental -> 36;
#   Fire/Earth Elemental, Spirit Wolves -> 37; Death Knight ghoul / gargoyle -> 35.
import sys, pathlib

core = pathlib.Path(sys.argv[1] if len(sys.argv) > 1 else r'C:\CoA-Build\core')
coa = core / 'src/server/coa/AscensionCompat.cpp'


def patch(path, old, new, key):
    text = path.read_text(encoding='utf-8')
    if key in text:
        return
    assert text.count(old) == 1, (path.name, old[:80])
    path.write_text(text.replace(old, new), encoding='utf-8', newline='')


patch(coa, '''        return (category >= 17 && category <= 31) || (category >= 65 && category <= 68);
    }
''', '''        return (category >= 17 && category <= 31) || IsClassPetCategory(category) || (category >= 65 && category <= 68);
    }

    // Call Pet / Summon Demon / Raise Undead / Call Dragonkin / Raise Elemental: the look is a creature.
    static bool IsClassPetCategory(uint32 category)
    {
        return category >= 33 && category <= 37;
    }
''', key='static bool IsClassPetCategory(uint32 category)')

patch(coa, '''        uint32 const appearanceId = state->ActiveAppearances[category];
        auto const itr = _incarnationDisplays.find(appearanceId);
        return appearanceId && itr != _incarnationDisplays.end() ? itr->second : 0;''',
      '''        uint32 const appearanceId = state->ActiveAppearances[category];
        if (!appearanceId)
            return 0;
        auto const itr = _incarnationDisplays.find(appearanceId);
        if (itr != _incarnationDisplays.end())
            return itr->second;
        auto const creature = _incarnationCreatures.find(appearanceId);
        if (creature != _incarnationCreatures.end())
            if (CreatureTemplate const* creatureTemplate = sObjectMgr->GetCreatureTemplate(creature->second))
                if (CreatureModel const* model = creatureTemplate->GetFirstValidModel())
                    return model->CreatureDisplayID;
        return 0;''', key='_incarnationCreatures.find(appearanceId)')

patch(coa, '''      if (IsIncarnationCategory(record.GetUInt32(5)) &&
          sCreatureDisplayInfoStore.LookupEntry(record.GetUInt32(8)))
        _incarnationDisplays[appearanceId] = record.GetUInt32(8);''',
      '''      if (IsClassPetCategory(record.GetUInt32(5)))
        _incarnationCreatures[appearanceId] = record.GetUInt32(3); // creature whose look is collected
      else if (IsIncarnationCategory(record.GetUInt32(5)) &&
          sCreatureDisplayInfoStore.LookupEntry(record.GetUInt32(8)))
        _incarnationDisplays[appearanceId] = record.GetUInt32(8);''', key='_incarnationCreatures[appearanceId] = record.GetUInt32(3);')

patch(coa, '''    _incarnationDisplays.clear();
''', '''    _incarnationDisplays.clear();
    _incarnationCreatures.clear();
''', key='_incarnationCreatures.clear();')

patch(coa, '''  std::unordered_map<uint32, uint32> _incarnationDisplays; // AscensionIncarnation: appearance -> model
''', '''  std::unordered_map<uint32, uint32> _incarnationDisplays; // AscensionIncarnation: appearance -> model
  std::unordered_map<uint32, uint32> _incarnationCreatures; // class pet incarnations: appearance -> creature
''', key='_incarnationCreatures; // class pet incarnations')

patch(coa, '''void ApplyAscensionCreatureIncarnation(Creature* creature)
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
    {''', '''// Class pet slot (Wardrobe categories 33-37) worn by a vanilla class's own pets and guardians.
static uint32 ClassPetCategoryForCreature(Creature* creature)
{
    if (Pet* pet = creature->ToPet())
        if (pet->getPetType() == HUNTER_PET)
            return 33;                                   // Call Pet
    uint32 entry = creature->GetEntry();
    if (entry > 1100000)
        entry -= 1100000;                                // Bronzebeard copies
    switch (entry)
    {
        case 416: case 1860: case 1863: case 417: case 17252:
        case 89: case 11859:     return 34;              // Summon Demon (warlock demons, Infernal, Doomguard)
        case 26125: case 27829:  return 35;              // Raise Undead (ghoul, gargoyle)
        case 510:                return 36;              // Call Dragonkin (Water Elemental)
        case 15438: case 15352:
        case 29264:              return 37;              // Raise Elemental (Fire/Earth Elemental, Spirit Wolf)
        default:                 return 0;
    }
}

void ApplyAscensionCreatureIncarnation(Creature* creature)
{
    if (!creature || !ascensionCompatConfig.GetConfigValue<bool>(AscensionCompatConfig::ENABLED))
        return;

    uint32 const category = IncarnationCategoryForCreature(creature->GetEntry());
    uint32 const classPetCategory = ClassPetCategoryForCreature(creature);
    if (!category && !classPetCategory)
        return;

    Player* owner = creature->GetCharmerOrOwnerPlayerOrPlayerItself();
    if (!owner)
        return;

    AscensionCollectionService& collection = AscensionCollectionService::Instance();
    uint32 model = category ? collection.GetIncarnationDisplay(owner, category) : 0;
    if (!model && classPetCategory)
        model = collection.GetIncarnationDisplay(owner, classPetCategory);
    if (model)
    {''', key='static uint32 ClassPetCategoryForCreature(Creature* creature)')

text = coa.read_text(encoding='utf-8')
if '#include "ObjectMgr.h"' not in text:
    patch(coa, '#include "Pet.h"\n', '#include "Pet.h"\n#include "ObjectMgr.h"\n', key='#include "ObjectMgr.h"')
# Hunter pets are saved with their model: only wear the look (native model kept), and drop it when cleared.
patch(coa, '''    if (model)
    {
        creature->SetNativeDisplayId(model);
        creature->SetDisplayId(model);
    }
}

class AscensionIncarnationPetScript''', '''    Pet* pet = creature->ToPet();
    if (pet && pet->getPetType() == HUNTER_PET)
    {
        creature->SetDisplayId(model ? model : creature->GetNativeDisplayId());
        return;
    }
    if (model)
    {
        creature->SetNativeDisplayId(model);
        creature->SetDisplayId(model);
    }
}

class AscensionIncarnationPetScript''', key='creature->SetDisplayId(model ? model : creature->GetNativeDisplayId());')
print('class pet incarnation patch applied')
