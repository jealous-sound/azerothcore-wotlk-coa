#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <limits>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
using uint8 = std::uint8_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;
using int32 = std::int32_t;
// ENUMS
constexpr uint32 EFFECT_0 = 0, ALLSPELLHOOK_ON_HIT_RESULT = 7, UNITHOOK_MODIFY_MELEE_DAMAGE = 11;
struct ObjectGuid
{
    uint64 value = 0;
    auto operator<=>(ObjectGuid const&) const = default;
};
struct Player;
struct Unit;
struct Heal
{
    Unit* healer;
    Unit* victim;
    uint32 amount;
    bool operator==(Heal const&) const = default;
};
std::vector<Heal> heals;
struct Aura { };
struct Unit
{
    virtual ~Unit() = default;
    virtual Player* ToPlayer() { return nullptr; }
    ObjectGuid guid;
    bool alive = true, inWorld = true;
    std::map<std::pair<uint32, ObjectGuid>, Aura> auras;
    explicit Unit(uint64 raw) : guid{raw} { }
    ObjectGuid GetGUID() const { return guid; }
    bool IsAlive() const { return alive; }
    bool IsInWorld() const { return inWorld; }
    bool HasAura(uint32 id, ObjectGuid owner) const { return auras.contains({id, owner}); }
    Aura* AddAura(uint32 id, Unit* target) { return &target->auras[{id, guid}]; }
    static int32 DealHeal(Unit* healer, Unit* victim, uint32 amount)
    {
        heals.push_back({healer, victim, amount});
        return int32(amount);
    }
};
struct Player : Unit
{
    uint8 cls = CLASS_SON_OF_ARUGAL;
    using Unit::Unit;
    Player* ToPlayer() override { return this; }
    uint8 getClass() const { return cls; }
};
struct SpellEffectInfo
{
    int32 value = 0;
    int32 CalcValue(Unit const*) const { return value; }
};
struct SpellInfo
{
    uint32 Id = 0;
    std::array<SpellEffectInfo, 3> Effects{};
};
struct SpellManager
{
    std::map<uint32, SpellInfo> rows;
    SpellInfo const* GetSpellInfo(uint32 id) const
    {
        auto it = rows.find(id);
        return it == rows.end() ? nullptr : &it->second;
    }
} manager;
auto sSpellMgr = &manager;
struct Spell
{
    Unit* caster = nullptr;
    SpellInfo info;
    Unit* GetCaster() const { return caster; }
    SpellInfo const* GetSpellInfo() const { return &info; }
};
struct AllSpellScript;
struct UnitScript;
std::vector<std::tuple<AllSpellScript*, std::string, std::set<uint32>>> spellScripts;
std::vector<std::tuple<UnitScript*, std::string, std::set<uint32>>> unitScripts;
struct AllSpellScript
{
    AllSpellScript(char const* name, std::initializer_list<uint32> hooks)
    {
        spellScripts.emplace_back(this, name, hooks);
    }
    virtual ~AllSpellScript() = default;
    virtual void OnSpellHitResult(Spell*, Unit*, uint8, uint32, uint32, bool) { }
};
struct UnitScript
{
    UnitScript(char const* name, bool addToScripts, std::initializer_list<uint32> hooks)
    {
        assert(addToScripts);
        unitScripts.emplace_back(this, name, hooks);
    }
    virtual ~UnitScript() = default;
    virtual void ModifyMeleeDamage(Unit*, Unit*, uint32&) { }
};
// SOURCE
constexpr uint32 BITE_WOUND = 706654, BITE_WOUND_PASSIVE = 532612, UNRELATED_SPELL = 501698;
constexpr std::array<uint32, 10> BLOODFANG_BITES = {
    800156, 501695, 501696, 501697, 503613, 503614, 503615, 572549, 572550, 572551 };
struct Fixture
{
    Player bloodmage{1}, other{2};
    Unit enemy{3}, creature{4};
    AllSpellScript* bite = nullptr;
    UnitScript* leech = nullptr;
    Fixture()
    {
        heals.clear();
        manager.rows = {{BITE_WOUND, {BITE_WOUND, {}}}, {BITE_WOUND_PASSIVE, {BITE_WOUND_PASSIVE, {}}}};
        SetPercent(15);
        bite = std::get<0>(spellScripts.at(0));
        leech = std::get<0>(unitScripts.at(0));
    }
    void SetPercent(int32 percent) { manager.rows.at(BITE_WOUND_PASSIVE).Effects[EFFECT_0].value = percent; }
    bool Bites(Unit* caster, Unit* target, uint32 id = BLOODFANG_BITES[0], uint8 miss = SPELL_MISS_NONE)
    {
        enemy.auras.clear();
        bloodmage.auras.clear();
        Spell spell;
        spell.caster = caster;
        spell.info.Id = id;
        bite->OnSpellHitResult(&spell, target, miss, 0, 0, false);
        size_t const applied = enemy.auras.size() + bloodmage.auras.size();
        assert(applied == 0 || (applied == 1 && target && target->HasAura(BITE_WOUND, bloodmage.guid)));
        return applied == 1;
    }
    std::vector<Heal> Melee(Unit* attacker, Unit* target, uint32 dealt)
    {
        heals.clear();
        uint32 damage = dealt;
        leech->ModifyMeleeDamage(target, attacker, damage);
        assert(damage == dealt);
        return heals;
    }
};
void RegistersHooks()
{
    AddSC_AscensionBloodmageBiteWound();
    assert(spellScripts.size() == 1 && unitScripts.size() == 1);
    assert(std::get<1>(spellScripts[0]) == "bloodmage_bite_wound");
    assert(std::get<2>(spellScripts[0]) == std::set<uint32>{ALLSPELLHOOK_ON_HIT_RESULT});
    assert(std::get<1>(unitScripts[0]) == "bloodmage_bite_wound_leech");
    assert(std::get<2>(unitScripts[0]) == std::set<uint32>{UNITHOOK_MODIFY_MELEE_DAMAGE});
}
void BloodfangBiteAppliesBiteWound()
{
    Fixture fixture;
    for (uint32 rank : BLOODFANG_BITES)
        assert(fixture.Bites(&fixture.bloodmage, &fixture.enemy, rank));
    assert(!fixture.Bites(&fixture.bloodmage, &fixture.enemy, UNRELATED_SPELL));
    for (uint8 miss : {SPELL_MISS_MISS, SPELL_MISS_DODGE, SPELL_MISS_PARRY, SPELL_MISS_IMMUNE, SPELL_MISS_REFLECT})
        assert(!fixture.Bites(&fixture.bloodmage, &fixture.enemy, BLOODFANG_BITES[0], miss));
    assert(!fixture.Bites(&fixture.bloodmage, nullptr));
    assert(!fixture.Bites(&fixture.bloodmage, &fixture.bloodmage));
    assert(!fixture.Bites(&fixture.creature, &fixture.enemy));
    assert(!fixture.Bites(nullptr, &fixture.enemy));
    fixture.enemy.alive = false;
    assert(!fixture.Bites(&fixture.bloodmage, &fixture.enemy));
    fixture.enemy.alive = true;
    for (bool* required : {&fixture.bloodmage.alive, &fixture.bloodmage.inWorld})
    {
        *required = false;
        assert(!fixture.Bites(&fixture.bloodmage, &fixture.enemy));
        *required = true;
    }
    fixture.bloodmage.cls = CLASS_WARRIOR;
    assert(!fixture.Bites(&fixture.bloodmage, &fixture.enemy));
    fixture.bloodmage.cls = CLASS_SON_OF_ARUGAL;
    manager.rows.erase(BITE_WOUND);
    assert(!fixture.Bites(&fixture.bloodmage, &fixture.enemy));
}
void MeleeAgainstBiteWoundHealsBloodmage()
{
    Fixture fixture;
    Player& bloodmage = fixture.bloodmage;
    Unit& enemy = fixture.enemy;
    std::vector<Heal> const none;
    assert(fixture.Melee(&bloodmage, &enemy, 200) == none);
    fixture.other.AddAura(BITE_WOUND, &enemy);
    assert(fixture.Melee(&bloodmage, &enemy, 200) == none);
    bloodmage.AddAura(BITE_WOUND, &enemy);
    assert(fixture.Melee(&bloodmage, &enemy, 200) == std::vector<Heal>({{&bloodmage, &bloodmage, 30}}));
    assert(fixture.Melee(&bloodmage, &enemy, 7) == std::vector<Heal>({{&bloodmage, &bloodmage, 1}}));
    assert(fixture.Melee(&bloodmage, &enemy, 6) == none);
    assert(fixture.Melee(&bloodmage, &enemy, 0) == none);
    assert(fixture.Melee(&bloodmage, nullptr, 200) == none);
    assert(fixture.Melee(&fixture.creature, &enemy, 200) == none);
    assert(fixture.Melee(nullptr, &enemy, 200) == none);
    for (bool* required : {&bloodmage.alive, &bloodmage.inWorld})
    {
        *required = false;
        assert(fixture.Melee(&bloodmage, &enemy, 200) == none);
        *required = true;
    }
    bloodmage.cls = CLASS_WARRIOR;
    assert(fixture.Melee(&bloodmage, &enemy, 200) == none);
    bloodmage.cls = CLASS_SON_OF_ARUGAL;
    fixture.SetPercent(250);
    uint32 const largest = std::numeric_limits<uint32>::max();
    assert(fixture.Melee(&bloodmage, &enemy, largest) == std::vector<Heal>({{&bloodmage, &bloodmage, largest}}));
    fixture.SetPercent(-15);
    assert(fixture.Melee(&bloodmage, &enemy, 200) == none);
    fixture.SetPercent(15);
    manager.rows.erase(BITE_WOUND_PASSIVE);
    assert(fixture.Melee(&bloodmage, &enemy, 200) == none);
}
int main()
{
    RegistersHooks();
    BloodfangBiteAppliesBiteWound();
    MeleeAgainstBiteWoundHealsBloodmage();
}
