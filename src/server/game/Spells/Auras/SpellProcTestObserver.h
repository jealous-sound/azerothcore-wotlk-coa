/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef AC_SPELL_PROC_TEST_OBSERVER_H
#define AC_SPELL_PROC_TEST_OBSERVER_H

#include "Define.h"
#include "ObjectGuid.h"
#include <limits>

namespace SpellProcTestObserver
{
struct Observation
{
    uint32 attempts = 0;
    float chance = std::numeric_limits<float>::quiet_NaN();
};

class Subscription
{
public:
    Subscription() = default;
    Subscription(Subscription const&) = delete;
    Subscription& operator=(Subscription const&) = delete;
    Subscription(Subscription&& other) noexcept;
    Subscription& operator=(Subscription&& other) noexcept;
    ~Subscription();

    Observation Read() const;

private:
    friend Subscription Subscribe(ObjectGuid owner, uint32 aura, uint32 trigger);
    Subscription(ObjectGuid owner, uint32 aura, uint32 trigger, uint64 generation);
    void Release();

    ObjectGuid _owner;
    uint32 _aura = 0;
    uint32 _trigger = 0;
    uint64 _generation = 0;
};

Subscription Subscribe(ObjectGuid owner, uint32 aura, uint32 trigger);
void Record(ObjectGuid owner, uint32 aura, uint32 trigger, float chance);
}

#endif
