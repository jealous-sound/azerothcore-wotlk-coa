#include <cstdint>
#include <cstdio>
#include <initializer_list>

using uint32 = std::uint32_t;
using uint8 = std::uint8_t;

// Mirrors the one field DynObjAura::FillTargetMap actually reads off TargetB
// (src/server/game/Spells/SpellInfo.h's SpellImplicitTargetInfo).
enum Targets : uint32
{
    TARGET_DEST_DYNOBJ_ALLY_STUB = 29,
};

class SpellImplicitTargetInfo
{
public:
    SpellImplicitTargetInfo() : _target(Targets(0)) {}
    SpellImplicitTargetInfo(uint32 target) : _target(Targets(target)) {}
    Targets GetTarget() const { return _target; }
private:
    Targets _target;
};

constexpr uint32 EFFECT_0 = 0;
constexpr uint32 MAX_SPELL_EFFECTS = 3;
constexpr uint32 TARGET_DEST_DYNOBJ_ALLY = TARGET_DEST_DYNOBJ_ALLY_STUB;

struct SpellEffectInfo
{
    SpellImplicitTargetInfo TargetA, TargetB;
};

struct SpellInfo
{
    uint32 Id = 0, SpellFamilyName = 34;
    SpellEffectInfo Effects[MAX_SPELL_EFFECTS];
};

int main()
{
    bool ok = true;
    for (uint32 id : {801256u, 803804u, 803805u, 803806u, 803807u, 803808u})
    {
        SpellInfo spell;
        spell.Id = id;
        spell.Effects[EFFECT_0].TargetA = SpellImplicitTargetInfo(TARGET_DEST_DYNOBJ_ALLY);
        // TargetB starts unset, as authored: this is what the reported bug leaves it at.

        SpellInfo* info = &spell;

        // ACTUAL_BLOCK

        if (info->Effects[EFFECT_0].TargetB.GetTarget() != TARGET_DEST_DYNOBJ_ALLY)
        {
            std::fprintf(stderr,
                "FAIL: spell %u effect 0 TargetB is %u, not TARGET_DEST_DYNOBJ_ALLY (%u). "
                "DynObjAura::FillTargetMap only special-cases TargetB for this target, so the "
                "periodic area search falls back to an attackable-target (enemy) check and the "
                "armor aura never reaches the ally.\n",
                id, uint32(info->Effects[EFFECT_0].TargetB.GetTarget()), TARGET_DEST_DYNOBJ_ALLY);
            ok = false;
        }
    }

    if (!ok)
        return 1;

    std::printf("PASS: Shield Beacon helper spells 801256/803804-803808 set effect 0's TargetB to "
                "TARGET_DEST_DYNOBJ_ALLY, so DynObjAura::FillTargetMap uses the friendly-target search.\n");
    return 0;
}
