#include <cstdint>
#include <cstdio>
#include <vector>

using uint32 = std::uint32_t;
using int32 = std::int32_t;
using ObjectGuid = std::uint64_t;

struct Aura
{
    int32 duration = -1;
    void SetDuration(int32 value) { duration = value; }
};

struct Unit
{
    ObjectGuid guid = 0;
    float x = 0, y = 0, z = 0;
    bool inWorld = true;

    float GetPositionX() const { return x; }
    float GetPositionY() const { return y; }
    float GetPositionZ() const { return z; }
    bool IsInWorld() const { return inWorld; }
    ObjectGuid GetGUID() const { return guid; }

    struct DestCast { float x, y, z; uint32 spell; bool triggered; };
    std::vector<DestCast> destCasts;
    void CastSpell(float px, float py, float pz, uint32 spell, bool triggered)
    {
        destCasts.push_back({px, py, pz, spell, triggered});
    }

    // Simulates whether the DynamicObject from an earlier cast has already applied its
    // aura to this unit (real application happens asynchronously via the object's own
    // periodic area search, not synchronously inside CastSpell).
    Aura* appliedAura = nullptr;
    Aura* GetAura(uint32, ObjectGuid) { return appliedAura; }
};

// Mirrors AscensionTinker.cpp's Cast(): a plain unit-targeted, triggered cast with no
// destination. The pre-fix Shield Beacon branch called only this.
std::vector<uint32> g_unitTargetedCasts;
void Cast(Unit* caster, Unit* target, uint32 id)
{
    if (caster && target && caster->IsInWorld())
        g_unitTargetedCasts.push_back(id);
}

int main()
{
    Unit allyObj;
    allyObj.guid = 2;
    allyObj.x = 100.0f;
    allyObj.y = 200.0f;
    allyObj.z = 5.0f;
    Unit* ally = &allyObj;

    Unit meObj;
    meObj.guid = 1;
    // Deliberately offset from the ally: TARGET_DEST_DYNOBJ_ALLY's persistent area aura
    // must center on the ally, not on the device that casts it.
    meObj.x = 100.0f;
    meObj.y = 190.0f;
    meObj.z = 5.0f;
    Unit* me = &meObj;

    uint32 entry = 50036; // Shield Beacon rank 1

    // Tick 1: the ally has no application yet.
    {
    // ACTUAL_BLOCK
    }

    if (!g_unitTargetedCasts.empty())
    {
        std::fprintf(stderr,
            "FAIL: Shield Beacon helper cast as a plain unit-targeted spell (spell %u). Its effect "
            "is TARGET_DEST_DYNOBJ_ALLY, which needs an explicit destination or Spell::CheckDst "
            "silently falls back to the caster's own position instead of the ally being buffed.\n",
            g_unitTargetedCasts.front());
        return 1;
    }

    if (me->destCasts.size() != 1)
    {
        std::fprintf(stderr,
            "FAIL: expected exactly one destination-targeted cast on tick 1 (the ally had no "
            "existing application), got %zu.\n", me->destCasts.size());
        return 1;
    }

    Unit::DestCast const& cast = me->destCasts.front();
    if (cast.spell != 801256)
    {
        std::fprintf(stderr, "FAIL: expected helper spell 801256 for entry 50036, got %u\n", cast.spell);
        return 1;
    }
    if (!cast.triggered)
    {
        std::fprintf(stderr, "FAIL: helper cast must be triggered (no cast bar/GCD on a device tick).\n");
        return 1;
    }
    if (cast.x != ally->x || cast.y != ally->y || cast.z != ally->z)
    {
        std::fprintf(stderr,
            "FAIL: destination (%f,%f,%f) does not match the ally's position (%f,%f,%f); the "
            "persistent-area-aura would center on the device instead of the ally.\n",
            cast.x, cast.y, cast.z, ally->x, ally->y, ally->z);
        return 1;
    }

    // Simulate the DynamicObject's own periodic area search having applied the aura by
    // the next device tick, then run the exact same block again for tick 2.
    Aura appliedAura;
    ally->appliedAura = &appliedAura;

    // Tick 2: the ally already has the aura applied.
    {
    // ACTUAL_BLOCK
    }

    if (me->destCasts.size() != 1)
    {
        std::fprintf(stderr,
            "FAIL: Shield Beacon helper was (re)cast on tick 2 although the ally already had the "
            "aura applied (%zu total destination casts). Each cast creates its own independent "
            "DynObjAura, which does not replace an existing application from the same spell/caster, "
            "so casting again stacks another +armor application on top instead of refreshing it.\n",
            me->destCasts.size());
        return 1;
    }
    if (appliedAura.duration != 2000)
    {
        std::fprintf(stderr,
            "FAIL: the ally's existing aura was not refreshed to 2000ms on tick 2 (duration=%d); it "
            "would expire and drop the buff instead of being kept alive.\n", appliedAura.duration);
        return 1;
    }

    std::printf("PASS: Shield Beacon helper 801256 is cast with an explicit destination at the "
                "ally's position on first application, and is refreshed (not restacked) on later "
                "ticks while the ally still has it.\n");
    return 0;
}
