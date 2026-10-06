import os
from pathlib import Path
import re
import runpy
import shutil
import subprocess
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
method = runpy.run_path(str(HERE.parent / "client_compat/run.py"))["method"]

HARNESS = r'''
#include <cassert>
#include <cmath>
#include <cstdint>
#include <list>
#include <utility>
#include <vector>
using uint8 = std::uint8_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;
// ENUMS
enum SpellEffIndex : uint8 { EFFECT_0 = 0 };
struct ObjectGuid
{
    uint64 value = 0;
    bool operator==(ObjectGuid const& other) const { return value == other.value; }
};
struct Position
{
    float x = 0.0f, y = 0.0f, z = 0.0f, o = 0.0f;
    Position(float px = 0.0f, float py = 0.0f, float pz = 0.0f, float po = 0.0f) : x(px), y(py), z(pz), o(po) { }
    float GetExactDist(Position const& other) const
    { return std::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) + (z - other.z) * (z - other.z)); }
    bool operator==(Position const& other) const { return x == other.x && y == other.y && z == other.z && o == other.o; }
};
struct Player;
struct Unit
{
    Position pos;
    ObjectGuid guid;
    virtual ~Unit() = default;
    virtual Player* ToPlayer() { return nullptr; }
    ObjectGuid GetGUID() const { return guid; }
};
struct CreatureAI
{
    ReactStates const* reactOfMe = nullptr;
    std::vector<Unit*> targets;
    std::vector<ReactStates> reactsAtAttack;
    std::vector<std::pair<uint32, Unit const*>> talks;
    void AttackStart(Unit* who)
    {
        assert(reactOfMe);
        targets.push_back(who);
        reactsAtAttack.push_back(*reactOfMe);
    }
    void Talk(uint8 id, Unit const* whisperTarget) { talks.emplace_back(id, whisperTarget); }
};
struct Creature : Unit
{
    uint32 entry = 0;
    bool alive = true, combat = false;
    ReactStates react = REACT_AGGRESSIVE;
    ObjectGuid summoner;
    CreatureAI ai;
    bool IsAlive() const { return alive; }
    bool IsInCombat() const { return combat; }
    void SetReactState(ReactStates state) { react = state; }
    CreatureAI* AI()
    {
        ai.reactOfMe = &react;
        return &ai;
    }
    ObjectGuid GetSummonerGUID() const { return summoner; }
};
struct TempSummon : Creature
{
    TempSummonType type = TEMPSUMMON_MANUAL_DESPAWN;
    uint32 duration = 0;
    bool personal = false;
};
struct GameObject
{
    Position pos;
    bool spawned = true;
    int despawns = 0;
    Position GetPosition() const { return pos; }
    bool isSpawned() const { return spawned; }
    void DespawnOrUnsummon() { ++despawns; spawned = false; }
};
struct SummonPropertiesEntry;
struct Map
{
    std::list<TempSummon> creatures;
};
constexpr float PlayerCombatReach = DEFAULT_COMBAT_REACH;
struct Player : Unit
{
    uint32 mapId = 1;
    QuestStatus moonwell = QUEST_STATUS_NONE;
    QuestStatus petals = QUEST_STATUS_NONE;
    Map* map = nullptr;
    std::vector<uint32> credits;
    std::vector<std::pair<GameObject*, uint32>> casts;
    Player* ToPlayer() override { return this; }
    QuestStatus GetQuestStatus(uint32 questId) const
    {
        assert(questId == 1660015 || questId == 1660017);
        return questId == 1660015 ? moonwell : petals;
    }
    uint32 GetMapId() const { return mapId; }
    bool IsWithinDist3d(Position const* target, float dist) const
    { return pos.GetExactDist(*target) < dist + PlayerCombatReach; }
    void GetCreatureListWithEntryInGrid(std::list<Creature*>& found, uint32 entry, float range) const
    {
        for (TempSummon& creature : map->creatures)
            if (creature.entry == entry && pos.GetExactDist(creature.pos) <= range)
                found.push_back(&creature);
    }
    TempSummon* SummonCreature(uint32 entry, Position const& at, TempSummonType type, uint32 duration, uint32 vehicleId,
                               SummonPropertiesEntry const* properties, bool visibleBySummonerOnly) const
    {
        assert(!vehicleId && !properties);
        TempSummon& summon = map->creatures.emplace_back();
        summon.entry = entry;
        summon.pos = at;
        summon.type = type;
        summon.duration = duration;
        summon.summoner = guid;
        summon.personal = visibleBySummonerOnly;
        return &summon;
    }
    void KilledMonsterCredit(uint32 entry) { credits.push_back(entry); }
    int CastSpell(GameObject* target, uint32 spellId, bool triggered)
    {
        assert(!triggered);
        casts.emplace_back(target, spellId);
        return 0;
    }
};

// SOURCE

struct Script
{
    Unit* caster = nullptr;
    GameObject* hitGObj = nullptr;
    bool prevented = false;
    Unit* GetCaster() { return caster; }
    GameObject* GetHitGObj() { return hitGObj; }
    void PreventHitDefaultEffect(SpellEffIndex effIndex) { assert(effIndex == EFFECT_0); prevented = true; }
// METHODS
};

using Talks = std::vector<std::pair<uint32, Unit const*>>;
using Casts = std::vector<std::pair<GameObject*, uint32>>;

Player MakePlayer(Map& map, uint64 guid)
{
    Player player;
    player.guid.value = guid;
    player.map = &map;
    player.pos = RemnantRise;
    player.moonwell = QUEST_STATUS_INCOMPLETE;
    return player;
}

void MoonwellGates()
{
    Map map;
    Player player = MakePlayer(map, 1);
    Script script;
    script.caster = &player;

    for (QuestStatus status : { QUEST_STATUS_NONE, QUEST_STATUS_COMPLETE, QUEST_STATUS_FAILED, QUEST_STATUS_REWARDED })
    {
        player.moonwell = status;
        assert(script.CheckAtEyrieMoonwell() == SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW);
    }
    player.moonwell = QUEST_STATUS_INCOMPLETE;
    assert(script.CheckAtEyrieMoonwell() == SPELL_CAST_OK);
    assert(RemnantRise.GetExactDist(EyrieMoonwell) < 1.5f);

    player.pos = Position(EyrieMoonwell.x + 9.4f, EyrieMoonwell.y, EyrieMoonwell.z);
    assert(script.CheckAtEyrieMoonwell() == SPELL_CAST_OK);
    player.pos = Position(EyrieMoonwell.x + 9.6f, EyrieMoonwell.y, EyrieMoonwell.z);
    assert(script.CheckAtEyrieMoonwell() == SPELL_FAILED_NOT_HERE);
    player.pos = Position(10823.8f, 493.3f, 1396.14f);
    assert(script.CheckAtEyrieMoonwell() == SPELL_FAILED_NOT_HERE);
    player.pos = RemnantRise;
    player.mapId = 0;
    assert(script.CheckAtEyrieMoonwell() == SPELL_FAILED_NOT_HERE);

    Creature npc;
    npc.pos = RemnantRise;
    Script foreign;
    foreign.caster = &npc;
    assert(foreign.CheckAtEyrieMoonwell() == SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW);
    foreign.RaiseAberrantFleshRemnant(EFFECT_0);
    assert(foreign.prevented && map.creatures.empty());
}

void RemnantPerPlayer()
{
    Map map;
    Player first = MakePlayer(map, 1);
    Player second = MakePlayer(map, 2);
    Script mine, theirs;
    mine.caster = &first;
    theirs.caster = &second;

    mine.RaiseAberrantFleshRemnant(EFFECT_0);
    assert(mine.prevented && map.creatures.size() == 1);
    TempSummon& remnant = map.creatures.back();
    assert(remnant.entry == 161783 && remnant.duration == 30000);
    assert(remnant.type == TEMPSUMMON_TIMED_DESPAWN_OOC_ALIVE);
    assert(remnant.react == REACT_DEFENSIVE);
    assert(remnant.ai.reactsAtAttack == std::vector<ReactStates>{ REACT_DEFENSIVE });
    assert(remnant.pos == RemnantRise);
    assert(remnant.summoner == first.guid && remnant.personal);
    assert((remnant.ai.talks == Talks{ { 0, &first } }));
    assert(remnant.ai.targets == std::vector<Unit*>{ &first });

    mine.RaiseAberrantFleshRemnant(EFFECT_0);
    assert(map.creatures.size() == 1 && remnant.ai.targets.size() == 2 && remnant.ai.talks.size() == 1);
    remnant.combat = true;
    mine.RaiseAberrantFleshRemnant(EFFECT_0);
    assert(map.creatures.size() == 1 && remnant.ai.targets.size() == 2);

    theirs.RaiseAberrantFleshRemnant(EFFECT_0);
    assert(map.creatures.size() == 2);
    TempSummon& theirRemnant = map.creatures.back();
    assert(theirRemnant.summoner == second.guid && theirRemnant.ai.targets == std::vector<Unit*>{ &second });
    assert(theirRemnant.react == REACT_DEFENSIVE && theirRemnant.type == TEMPSUMMON_TIMED_DESPAWN_OOC_ALIVE);
    assert((theirRemnant.ai.talks == Talks{ { 0, &second } }));
    assert(remnant.ai.targets.size() == 2);

    remnant.combat = false;
    theirs.RaiseAberrantFleshRemnant(EFFECT_0);
    assert(map.creatures.size() == 2 && theirRemnant.ai.targets.size() == 2 && remnant.ai.targets.size() == 2);
    mine.RaiseAberrantFleshRemnant(EFFECT_0);
    assert(map.creatures.size() == 2 && remnant.ai.targets.size() == 3 && theirRemnant.ai.targets.size() == 2);

    remnant.alive = false;
    mine.RaiseAberrantFleshRemnant(EFFECT_0);
    assert(map.creatures.size() == 3 && map.creatures.back().summoner == first.guid);
    assert(map.creatures.back().ai.targets == std::vector<Unit*>{ &first });

    map.creatures.back().pos = Position(RemnantRise.x + 25.0f, RemnantRise.y, RemnantRise.z);
    mine.RaiseAberrantFleshRemnant(EFFECT_0);
    assert(map.creatures.size() == 4);

    for (TempSummon& raised : map.creatures)
    {
        assert(raised.react == REACT_DEFENSIVE);
        for (ReactStates react : raised.ai.reactsAtAttack)
            assert(react == REACT_DEFENSIVE);
    }
}

void NewbornRescue()
{
    Map map;
    Player player = MakePlayer(map, 1);
    GameObject baby;
    baby.pos = Position(10827.7f, 448.38f, 1386.465f, 2.0f);
    Script script;
    script.caster = &player;
    script.hitGObj = &baby;

    for (QuestStatus status : { QUEST_STATUS_NONE, QUEST_STATUS_COMPLETE, QUEST_STATUS_REWARDED })
    {
        player.petals = status;
        assert(script.OnGossipHello(&player, &baby));
        assert(player.casts.empty());
    }
    player.petals = QUEST_STATUS_INCOMPLETE;
    assert(script.OnGossipHello(&player, &baby));
    assert((player.casts == Casts{ { &baby, 256715 } }));
    assert(player.credits.empty() && map.creatures.empty() && baby.spawned);

    for (QuestStatus status : { QUEST_STATUS_NONE, QUEST_STATUS_COMPLETE, QUEST_STATUS_REWARDED })
    {
        player.petals = status;
        script.TransformNewborn(EFFECT_0);
        assert(player.credits.empty() && map.creatures.empty() && baby.spawned && !baby.despawns);
    }
    player.petals = QUEST_STATUS_INCOMPLETE;
    script.TransformNewborn(EFFECT_0);
    assert(player.credits == std::vector<uint32>{ 161846 });
    assert(map.creatures.size() == 1);
    TempSummon& butterfly = map.creatures.back();
    assert(butterfly.entry == 161847 && butterfly.type == TEMPSUMMON_TIMED_DESPAWN && butterfly.duration == 600000);
    assert(butterfly.summoner == player.guid && butterfly.personal && butterfly.pos == baby.pos);
    assert(baby.despawns == 1 && !baby.spawned);

    script.TransformNewborn(EFFECT_0);
    assert(player.credits.size() == 1 && map.creatures.size() == 1 && baby.despawns == 1);

    baby.spawned = true;
    Creature npc;
    Script foreign;
    foreign.caster = &npc;
    foreign.hitGObj = &baby;
    foreign.TransformNewborn(EFFECT_0);
    script.hitGObj = nullptr;
    script.TransformNewborn(EFFECT_0);
    assert(player.credits.size() == 1 && map.creatures.size() == 1 && baby.despawns == 1);
}

int main()
{
    MoonwellGates();
    RemnantPerPlayer();
    NewbornRescue();
    return 0;
}
'''


def build_harness(source):
    shared = (ROOT / "src/server/shared/SharedDefines.h").read_text(encoding="utf-8")
    quests = (ROOT / "src/server/game/Quests/QuestDef.h").read_text(encoding="utf-8")
    objects = (ROOT / "src/server/game/Entities/Object/Object.h").read_text(encoding="utf-8")
    units = (ROOT / "src/server/game/Entities/Unit/Unit.h").read_text(encoding="utf-8")
    defines = (ROOT / "src/server/game/Entities/Object/ObjectDefines.h").read_text(encoding="utf-8")
    enums = "".join(method(text, "enum " + name) + ";\n" for text, name in (
        (shared, "SpellCastResult"), (quests, "QuestStatus"), (objects, "TempSummonType"), (units, "ReactStates")))
    enums += re.search(r"#define DEFAULT_COMBAT_REACH .*\n", defines).group(0)
    start = source.index("enum VultureCultMoonwell")
    body = source[start:source.index("class spell_coa_channeling_blessing_of_the_moon")]
    methods = "".join(method(source, signature).replace(") override\n", ")\n", 1) + "\n" for signature in (
        "SpellCastResult CheckAtEyrieMoonwell()", "void RaiseAberrantFleshRemnant(",
        "bool OnGossipHello(Player* player, GameObject* baby)", "void TransformNewborn("))
    return HARNESS.replace("// ENUMS", enums).replace("// SOURCE", body).replace("// METHODS", methods)


def compile_and_run(code):
    with tempfile.TemporaryDirectory(prefix="coa-vulture-cult-moonwell-", ignore_cleanup_errors=True) as directory:
        out = Path(directory)
        cpp, exe = out / "moonwell.cpp", out / "moonwell.exe"
        cpp.write_text(code, encoding="utf-8")
        msvc = shutil.which("cl")
        if msvc is None and os.environ.get("VCToolsInstallDir"):
            msvc = str(Path(os.environ["VCToolsInstallDir"]) / "bin/Hostx64/x64/cl.exe")
        if msvc:
            command = [msvc, "/nologo", "/std:c++20", "/EHsc", "/W4", "/WX", "/utf-8", "/UNDEBUG",
                       str(cpp), "/Fe" + str(exe)]
        else:
            compiler = shutil.which(os.environ.get("CXX", "g++"))
            assert compiler, "Run from a Visual Studio developer prompt or set CXX to a C++20 compiler."
            command = [compiler, "-std=c++20", "-Wall", "-Wextra", "-Werror", str(cpp), "-o", str(exe)]
        subprocess.run(command, cwd=out, check=True, timeout=120)
        subprocess.run([str(exe)], cwd=out, check=True, timeout=15)


def check_binding(source):
    moonwell = "spell_coa_channeling_blessing_of_the_moon"
    newborn = "spell_coa_rescuing_a_newborn"
    baby = "go_coa_baby_night_elf"
    assert f"OnCheckCast += SpellCheckCastFn({moonwell}::CheckAtEyrieMoonwell);" in source
    assert re.search(rf"OnEffectHit \+= SpellEffectFn\({moonwell}::RaiseAberrantFleshRemnant, EFFECT_0,\s+"
                     r"SPELL_EFFECT_SEND_EVENT\);", source)
    assert re.search(rf"OnEffectHitTarget \+= SpellEffectFn\({newborn}::TransformNewborn, EFFECT_0,\s+"
                     r"SPELL_EFFECT_DUMMY\);", source)
    assert f'GameObjectScript("{baby}")' in source
    for registration in (f"RegisterSpellScript({moonwell});", f"RegisterSpellScript({newborn});", f"new {baby}();"):
        assert registration in source, registration
    loader = (ROOT / "src/server/coa/CoAScriptLoader.cpp").read_text(encoding="utf-8")
    assert loader.count("AddSC_AscensionVultureCultQuests();") == 2
    sql = (ROOT / "data/sql/updates/pending_db_world/rev_20260924_10_coa_teldrassil_vulture_cult.sql").read_text(
        encoding="utf-8")
    assert "DELETE FROM `spell_script_names` WHERE `spell_id` IN (256707, 256715);" in sql
    assert f"(256707, '{moonwell}')" in sql and f"(256715, '{newborn}')" in sql
    assert "event_scripts" not in sql
    assert re.search(rf"^\(2300526, 10, \d+, 'Baby Night Elf', '[^']*', [\d.]+, '', (\d+, ){{24}}'{baby}'\)",
                     sql, re.MULTILINE)
    smart_deletes = re.findall(r"DELETE FROM `smart_scripts` WHERE `entryorguid` IN \(([^)]*)\) AND `source_type` = 1;",
                               sql)
    assert any("2300526" in keys.split(", ") for keys in smart_deletes)
    assert not re.search(r"^\(2300526, 1, ", sql, re.MULTILINE)
    assert re.search(r"^\(161783, 0, 0, '%s rises from the depths of the Moonwell!', 42, ", sql, re.MULTILINE)


def main():
    source = (ROOT / "src/server/coa/AscensionVultureCultQuests.cpp").read_text(encoding="utf-8")
    check_binding(source)
    compile_and_run(build_harness(source))
    print("PASS: quest, map and moonwell-reach gates, event 3839 prevented, one remnant per player raised at the pool "
          "with its rise line whispered to its summoner, personal to and attacking its summoner, defensive so it never "
          "aggroes on sight and gone 30 s after it idles, re-aimed only by its own summoner, never blocked or taken by "
          "another player's remnant, raised again after death or out of reach; the newborn rescue cast and effect "
          "only while the quest is incomplete, credit, a personal butterfly at the baby and the baby's despawn once "
          "per spawn; and the spell, gameobject and migration bindings")


if __name__ == "__main__":
    main()
