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

    // The pre-fix code path only ever reaches the free Cast() helper below, never a
    // destination. GetAura is unused by this fixture (no aura is pre-applied).
    Aura* GetAura(uint32, ObjectGuid) { return nullptr; }
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

    // ACTUAL_BLOCK

    if (!g_unitTargetedCasts.empty())
    {
        std::fprintf(stderr,
            "FAIL: Shield Beacon helper cast as a plain unit-targeted spell (spell %u). Its effect "
            "is TARGET_DEST_DYNOBJ_ALLY, which needs an explicit destination or Spell::CheckDst "
            "silently falls back to the caster's own position instead of the ally being buffed.\n",
            g_unitTargetedCasts.front());
        return 1;
    }

    if (me->destCasts.empty())
    {
        std::fprintf(stderr, "FAIL: Shield Beacon helper was never cast with an explicit destination.\n");
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

    std::printf("PASS: Shield Beacon helper 801256 is cast with an explicit destination at the "
                "ally's position, matching its TARGET_DEST_DYNOBJ_ALLY effect target.\n");
    return 0;
}
