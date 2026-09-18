#include <cassert>
#include <cmath>
#include <cstdint>
#include <list>
#include <map>
#include <set>
#include <utility>
#include <vector>

using uint32 = std::uint32_t;
using int32 = std::int32_t;
using ObjectGuid = uint32;
using WeaponAttackType = uint32;
constexpr uint32 RANGED_ATTACK = 2, UNIT_STATE_CONTROLLED = 1, UNIT_STATE_CASTING = 2;
constexpr uint32 SPELL_AURA_MOD_PACIFY = 3, SPELL_AURA_MOD_PACIFY_SILENCE = 4;
constexpr uint32 UNIT_FLAG_PLAYER_CONTROLLED = 1, UNIT_FLAG_NON_ATTACKABLE = 2, UNIT_FLAG_TAXI_FLIGHT = 4;
constexpr uint32 UNIT_FLAG_NOT_ATTACKABLE_1 = 8, UNIT_FLAG_NON_ATTACKABLE_2 = 16;
constexpr uint32 WORLD_TRIGGER = 12999, TYPEMASK_GAMEOBJECT = 1, TYPEMASK_DYNAMICOBJECT = 2, UNIT_FIELD_BYTES_2 = 0;
constexpr int REP_NEUTRAL = 4, REP_HOSTILE = 2;

struct Unit;
struct WorldObject
{
    Unit* unit = nullptr;
    Unit* ToUnit() const { return unit; }
    bool isType(uint32) const { return false; }
};

struct Unit : WorldObject
{
    Unit() { unit = this; }
    uint32 guid = 0, entry = 0, flags = 0, pvp = 0;
    float x = 0;
    bool alive = true, friendly = false, hostile = false, visible = true, los = true;
    bool immuneNPC = false, immunePC = false, m_ControlledByPlayer = false;
    int32 m_attackTimer[3]{};
    float m_modAttackSpeedPct[3]{1, 1, 1};
    bool IsAlive() const { return alive; }
    uint32 GetEntry() const { return entry; }
    uint32 GetGUID() const { return guid; }
    uint32 GetAttackTime(uint32) const { return 2000; }
    uint32 GetByteValue(uint32, uint32) const { return pvp; }
    void SetByteValue(uint32, uint32, uint32 value) { pvp = value; }
    void SetUnitFlag(uint32 flag) { flags |= flag; }
    bool HasUnitFlag(uint32 mask) const { return (flags & mask) != 0; }
    bool IsImmuneToNPC() const { return immuneNPC; }
    bool IsImmuneToPC() const { return immunePC; }
    int GetReactionTo(Unit const* target) const { return hostile || target->hostile ? REP_HOSTILE : REP_NEUTRAL; }
    float GetPositionX() const { return x; }
    float GetPositionY() const { return 0; }
    float GetPositionZ() const { return 0; }
    void resetAttackTimer(WeaponAttackType);
    bool NativeAttackAdmission(Unit const* target) const
    {
        WorldObject const* obj = nullptr;
        // ACTUAL_ADMISSION
        return true;
    }
};

// ACTUAL_TIMER

struct Player : Unit
{
    Player() { flags = UNIT_FLAG_PLAYER_CONTROLLED; guid = 1; }
    Unit* victim = nullptr;
    Unit* selected = nullptr;
    std::set<Unit*> combat, m_Controlled;
    bool IsValidAttackTarget(Unit* target) const
    {
        return target && !target->friendly && NativeAttackAdmission(target);
    }
    bool IsHostileTo(Unit* target) const { return target->hostile; }
    bool IsInCombatWith(Unit* target) const { return combat.contains(target); }
    Unit* GetVictim() const { return victim; }
    Unit* GetSelectedUnit() const { return selected; }
    uint32 GetFaction() const { return 1; }
};

Player* currentOwner = nullptr;
Player* Owner(Unit* unit) { return unit && unit == currentOwner ? currentOwner : nullptr; }

struct Creature : Unit
{
    Creature() { entry = 50046; }
    uint32 state = 0, owner = 0;
    bool upgraded = false, pacified = false;
    Unit* facing = nullptr;
    std::vector<std::pair<uint32, float>> shots;
    void SetOwnerGUID(uint32 id) { owner = id; }
    void SetFaction(uint32) { }
    bool HasUnitState(uint32 mask) const { return (state & mask) != 0; }
    bool HasAuraType(uint32) const { return pacified; }
    bool HasAura(uint32 id) const { return id == 706692 && upgraded; }
    bool isAttackReady(uint32 type) const { return m_attackTimer[type] <= 0; }
    bool IsWithinDistInMap(Unit* target, float range) const { return std::abs(x - target->x) <= range; }
    bool CanSeeOrDetect(Unit* target) const { return target->visible; }
    bool IsWithinLOSInMap(Unit* target) const { return target->los; }
    float GetExactDist(Unit* target) const { return std::abs(x - target->x); }
    void SetFacingToObject(Unit* target) { facing = target; }
    void CastSpell(float destX, float, float, uint32 id, bool) { shots.emplace_back(id, destX); }
};

struct SpellInfo
{
    uint32 SpellFamilyName = 34;
    bool positive = false;
    bool IsPositive() const { return positive; }
    float GetMaxRange(bool, Creature*) const { return 45; }
} info;
struct Manager
{
    SpellInfo const* GetSpellInfo(uint32 id) const { assert(id == 706689); return &info; }
} manager;
auto sSpellMgr = &manager;
std::map<uint32, Unit*> units;
std::list<Unit*> neighborhood;
namespace ObjectAccessor
{
Unit* GetUnit(Creature const&, uint32 guid) { return units.contains(guid) ? units[guid] : nullptr; }
}
std::list<Unit*> Nearby(Unit*, float) { return neighborhood; }
void Cast(Creature* caster, Unit* target, uint32 id)
{
    if (caster->NativeAttackAdmission(target))
        caster->shots.emplace_back(id, target->x);
}

// ACTUAL_TURRET

struct TinkerState
{
    ObjectGuid focus = 0, observedVictim = 0;
    std::set<ObjectGuid> summons;
} tinkerState;
TinkerState& State(Player*) { return tinkerState; }

// ACTUAL_NOTIFY_ATTACK
// ACTUAL_NOTIFY_SPELL_ATTACK
// ACTUAL_OBSERVE_ATTACK

struct Device
{
    Creature* me;
    uint32 focus = 0, owner = 0;
    // ACTUAL_INITIALIZATION
    // ACTUAL_TARGET
    // ACTUAL_UPDATE
};

int main()
{
    Player player;
    player.pvp = 5;
    currentOwner = &player;
    Creature turret;
    Device ai{&turret};
    Unit unregistered, mobA, mobB, mobC;
    mobA.guid = 2;
    mobA.x = 10;
    mobA.hostile = true;
    mobB.guid = 3;
    mobB.x = 20;
    mobB.hostile = true;
    mobC.guid = 4;
    mobC.x = 30;
    mobC.hostile = true;
    units = {{2, &mobA}, {3, &mobB}, {4, &mobC}};
    neighborhood = {&mobA, &mobB, &mobC};

    // Nearby hostiles and selected targets are not attack evidence.
    player.selected = &mobA;
    ObserveAttack(&player);
    assert(!ai.TurretTarget(&player));

    // A mob attacking the player first must not wake an idle turret.
    player.combat.insert(&mobA);
    ObserveAttack(&player);
    assert(!ai.TurretTarget(&player));

    ai.IsSummonedBy(&player);

    // A deliberate player melee/ranged attack is observed from the player's actual victim.
    player.combat.clear();
    player.selected = nullptr;
    player.victim = &mobA;
    ObserveAttack(&player);
    assert(ai.TurretTarget(&player) == &mobA);
    unregistered.flags = UNIT_FLAG_NON_ATTACKABLE;
    assert(!turret.NativeAttackAdmission(&unregistered));
    unregistered.flags = 0;
    ai.UpdateTurret(&player);
    assert(turret.facing == &mobA);
    if (turret.shots.size() != 1) // Reproduces #89 before the control flags are initialized.
        return 89;
    assert(turret.m_ControlledByPlayer && turret.pvp == player.pvp);
    assert(player.m_Controlled.contains(&turret));
    assert(turret.m_attackTimer[RANGED_ATTACK] == 2000);
    ai.UpdateTurret(&player);
    assert(turret.shots.size() == 1);

    mobA.immuneNPC = true;
    assert(turret.NativeAttackAdmission(&mobA));
    mobA.immunePC = true;
    assert(!turret.NativeAttackAdmission(&mobA));
    mobA.immunePC = false;
    mobA.flags = UNIT_FLAG_NON_ATTACKABLE;
    assert(!turret.NativeAttackAdmission(&mobA));
    mobA.flags = 0;

    // An unrelated nearby hostile is ignored while the player attacks A.
    player.combat.insert(&mobB);
    player.selected = &mobB;
    ObserveAttack(&player);
    assert(ai.TurretTarget(&player) == &mobA);

    // Rocket Launcher and other explicit hostile Tinker spells notify the same focus path.
    SpellInfo rocket;
    rocket.SpellFamilyName = 34;
    rocket.positive = false;
    assert(NotifySpellAttack(&player, &rocket, &mobB));
    assert(State(&player).focus == mobB.guid);
    assert(ai.TurretTarget(&player) == &mobB);
    turret.m_attackTimer[RANGED_ATTACK] = 0;
    turret.m_modAttackSpeedPct[RANGED_ATTACK] = 0.5f;
    ai.UpdateTurret(&player);
    assert(turret.shots.size() == 2 && turret.facing == &mobB);
    assert(turret.m_attackTimer[RANGED_ATTACK] == 1000);

    // A stale melee victim cannot override the newer explicit spell target.
    player.victim = &mobA;
    ObserveAttack(&player);
    assert(ai.TurretTarget(&player) == &mobB);

    // A new deliberate melee/ranged attack replaces the spell target.
    player.victim = &mobC;
    ObserveAttack(&player);
    assert(State(&player).focus == mobC.guid);
    assert(ai.TurretTarget(&player) == &mobC);

    // Selection and combat membership still do not create a target.
    player.victim = nullptr;
    player.selected = &mobA;
    player.combat.clear();
    ObserveAttack(&player);
    assert(State(&player).focus == mobC.guid);
    assert(ai.TurretTarget(&player) == &mobC);

    // Positive or non-Tinker spells do not notify the attack focus.
    SpellInfo positive;
    positive.SpellFamilyName = 34;
    positive.positive = true;
    SpellInfo otherFamily;
    otherFamily.SpellFamilyName = 1;
    otherFamily.positive = false;
    assert(!NotifySpellAttack(&player, &positive, &mobA));
    assert(!NotifySpellAttack(&player, &otherFamily, &mobA));
    assert(State(&player).focus == mobC.guid);

    // An invalid most-recent explicit target goes idle instead of falling back to nearby hostiles.
    assert(NotifySpellAttack(&player, &rocket, &mobB));
    mobB.alive = false;
    assert(!ai.TurretTarget(&player));
    mobB.alive = true;
    mobB.x = 100;
    assert(!ai.TurretTarget(&player));
    mobB.x = 20;
    mobB.visible = false;
    assert(!ai.TurretTarget(&player));
    mobB.visible = true;
    mobB.los = false;
    assert(!ai.TurretTarget(&player));
    mobB.los = true;

    turret.m_attackTimer[RANGED_ATTACK] = 0;
    turret.pacified = true;
    ai.UpdateTurret(&player);
    assert(turret.shots.size() == 2);
    turret.pacified = false;
    turret.state = UNIT_STATE_CONTROLLED;
    ai.UpdateTurret(&player);
    assert(turret.shots.size() == 2);
    turret.state = 0;
    turret.upgraded = true;
    player.victim = &mobA;
    ObserveAttack(&player);
    ai.UpdateTurret(&player);
    assert((turret.shots.back() == std::pair<uint32, float>(706694, mobA.x)));

    Creature beacon, orphan;
    beacon.entry = 50037;
    Device other{&beacon}, missing{&orphan};
    other.IsSummonedBy(&player);
    missing.IsSummonedBy(nullptr);
    assert(!beacon.m_ControlledByPlayer && !beacon.HasUnitFlag(UNIT_FLAG_PLAYER_CONTROLLED));
    assert(!orphan.owner && !orphan.m_ControlledByPlayer);
}
