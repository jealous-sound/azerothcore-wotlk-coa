# Core patch for the extra races and the Bloodmage incarnations.
#  * racial abilities: a racial skill line can belong to several races (the new races borrow the
#    racials of a close base race), so CanLearn checks the (race, skill) pair
#  * Bloodmage: Eternal Curse / Accursed Form wear the Cat Form incarnations, Inner Demon wears Metamorphosis
import sys, pathlib

core = pathlib.Path(sys.argv[1] if len(sys.argv) > 1 else r'C:\CoA-Build\core')
racial = core / 'src/server/coa/AscensionRacialAbilities.h'
player_h = core / 'src/server/game/Entities/Player/Player.h'
coa = core / 'src/server/coa/AscensionCompat.cpp'


def patch(path, old, new, key):
    text = path.read_text(encoding='utf-8')
    if key in text:
        return
    assert text.count(old) == 1, (path.name, old[:60])
    path.write_text(text.replace(old, new), encoding='utf-8', newline='')


# race id -> racial skill lines it borrows (see gen_races.py RACES)
BORROW = {
    9: ['SKILL_ORC_RACIAL', 'SKILL_ORC_RACIAL_LEGACY'], 12: ['SKILL_RACIAL_TROLL'], 13: ['SKILL_RACIAL_NIGHT_ELF'],
    14: ['SKILL_RACIAL_BLOODELF'], 15: ['SKILL_RACIAL_DWARVEN'], 16: ['SKILL_RACIAL_HUMAN'], 17: ['SKILL_RACIAL_TAUREN'],
    18: ['SKILL_RACIAL_HUMAN'], 19: ['SKILL_RACIAL_TROLL'], 20: ['SKILL_RACIAL_DRAENEI', 'SKILL_DRAENEI_RACIAL_COA'],
    21: ['SKILL_RACIAL_UNDED'], 22: ['SKILL_RACIAL_DRAENEI', 'SKILL_DRAENEI_RACIAL_COA'],
    23: ['SKILL_ORC_RACIAL', 'SKILL_ORC_RACIAL_LEGACY'], 24: ['SKILL_RACIAL_TROLL'], 25: ['SKILL_RACIAL_TROLL'],
    26: ['SKILL_RACIAL_UNDED'], 27: ['SKILL_RACIAL_DWARVEN'], 28: ['SKILL_RACIAL_TROLL'], 29: ['SKILL_RACIAL_TAUREN'],
    30: ['SKILL_RACIAL_UNDED'], 31: ['SKILL_RACIAL_UNDED'],
}
rows = [(r, s) for r, skills in BORROW.items() for s in skills]
entries = ''.join('    {%d, %s}, // extra race\n' % (r, s) for r, s in rows)

patch(racial, '''inline constexpr std::array<RacialSkill, 12> Skills =
{{
''', '''inline constexpr std::array<RacialSkill, %d> Skills =
{{
%s''' % (12 + len(rows), entries), key='// extra race')
patch(racial, '''    {RACE_DRAENEI, SKILL_DRAENEI_RACIAL_COA}
}};''', '''    {RACE_DRAENEI, SKILL_DRAENEI_RACIAL_COA}
}};

constexpr bool HasRacialSkill(uint8 raceId, uint32 skillId)
{
    for (RacialSkill const& skill : Skills)
        if (skill.RaceId == raceId && skill.SkillId == skillId)
            return true;
    return false;
}''', key='constexpr bool HasRacialSkill(')
patch(racial, '''    if (!raceId || GetRace(ability.SkillLine) != raceId || !IsAscensionClass(classId))''',
      '''    if (!raceId || !HasRacialSkill(raceId, ability.SkillLine) || !IsAscensionClass(classId))''',
      key='!HasRacialSkill(raceId, ability.SkillLine)')

patch(coa, '''        case 800912: return 22; // Vizier Form      -> Moonkin Form
''', '''        case 800912: return 22; // Vizier Form      -> Moonkin Form
        case 562572: return 18; // Bloodmage Accursed Form -> Cat Form
        case 800157:            // Bloodmage Eternal Curse (tank form) -> Metamorphosis
        case 804518:            // Bloodmage Eternal Curse (shapeshift)
        case 804216: return 31; // Bloodmage Inner Demon   -> Metamorphosis
''', key='case 804216: return 31;')
# race display ids (player-ready model copies) go above 65535; uint16 truncated them -> wrong look, or a
# login crash (CreatureDisplayInfo assert in GetCollisionHeight) when the truncated id does not exist
patch(player_h, '''    uint16 displayId_m{0};
    uint16 displayId_f{0};''', '''    uint32 displayId_m{0}; // custom race display ids go above 65535
    uint32 displayId_f{0};''', key='uint32 displayId_m{0};')
print('race/bloodmage core patch applied')
