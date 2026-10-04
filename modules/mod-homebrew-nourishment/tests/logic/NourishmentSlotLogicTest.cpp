#include "../../src/NourishmentSlotLogic.h"

#include <array>
#include <cassert>
#include <cstdint>

int main()
{
    constexpr uint64_t now = 1000;
    std::array<NourishmentSlotEntry, 3> empty{};
    auto first = SelectNourishmentSlot(empty, 117, now);
    assert(first.index == 0 && !first.refreshExistingItem && !first.replaceExisting);

    std::array<NourishmentSlotEntry, 3> partial{{
        {117, 2000}, {0, 0}, {0, 0}
    }};
    auto refresh = SelectNourishmentSlot(partial, 117, now);
    assert(refresh.index == kNourishmentSlotCount);
    auto second = SelectNourishmentSlot(partial, 159, now);
    assert(second.index == 1 && !second.replaceExisting);

    std::array<NourishmentSlotEntry, 3> expired{{
        {117, 2000}, {159, 900}, {414, 3000}
    }};
    auto reuseExpired = SelectNourishmentSlot(expired, 733, now);
    assert(reuseExpired.index == 1 && !reuseExpired.replaceExisting);

    std::array<NourishmentSlotEntry, 3> full{{
        {117, 2500}, {159, 1800}, {414, 3200}
    }};
    auto replaceSoonest = SelectNourishmentSlot(full, 733, now);
    assert(replaceSoonest.index == kNourishmentSlotCount);
}
