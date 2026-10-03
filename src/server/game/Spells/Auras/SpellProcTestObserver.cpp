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

#include "SpellProcTestObserver.h"
#include <atomic>
#include <map>
#include <mutex>
#include <stdexcept>
#include <tuple>
#include <utility>

namespace SpellProcTestObserver
{
namespace
{
using Key = std::tuple<ObjectGuid, uint32, uint32>;

struct Entry
{
    uint64 generation;
    Observation observation;
};

struct State
{
    std::atomic<bool> enabled = false;
    std::mutex mutex;
    std::map<Key, Entry> entries;
    uint64 generation = 0;
};

State& Data()
{
    static State state;
    return state;
}
}

Subscription::Subscription(ObjectGuid owner, uint32 aura, uint32 trigger, uint64 generation)
    : _owner(owner), _aura(aura), _trigger(trigger), _generation(generation)
{
}

Subscription::Subscription(Subscription&& other) noexcept
    : _owner(other._owner), _aura(other._aura), _trigger(other._trigger),
      _generation(std::exchange(other._generation, 0))
{
}

Subscription& Subscription::operator=(Subscription&& other) noexcept
{
    if (this != &other)
    {
        Release();
        _owner = other._owner;
        _aura = other._aura;
        _trigger = other._trigger;
        _generation = std::exchange(other._generation, 0);
    }
    return *this;
}

Subscription::~Subscription()
{
    Release();
}

void Subscription::Release()
{
    if (!_generation)
        return;
    State& state = Data();
    std::lock_guard lock(state.mutex);
    auto itr = state.entries.find({ _owner, _aura, _trigger });
    if (itr != state.entries.end() && itr->second.generation == _generation)
        state.entries.erase(itr);
    state.enabled.store(!state.entries.empty(), std::memory_order_release);
    _generation = 0;
}

Observation Subscription::Read() const
{
    State& state = Data();
    std::lock_guard lock(state.mutex);
    auto const itr = state.entries.find({ _owner, _aura, _trigger });
    if (itr == state.entries.end() || itr->second.generation != _generation)
        return {};
    return itr->second.observation;
}

Subscription Subscribe(ObjectGuid owner, uint32 aura, uint32 trigger)
{
    State& state = Data();
    std::lock_guard lock(state.mutex);
    Key key{ owner, aura, trigger };
    if (!state.entries.contains(key) && state.entries.size() >= 128)
        throw std::runtime_error("Too many spell proc test subscriptions");
    uint64 const generation = ++state.generation;
    state.entries.insert_or_assign(key, Entry{ generation, {} });
    state.enabled.store(true, std::memory_order_release);
    return Subscription(owner, aura, trigger, generation);
}

void Record(ObjectGuid owner, uint32 aura, uint32 trigger, float chance)
{
    State& state = Data();
    if (!state.enabled.load(std::memory_order_acquire))
        return;
    std::lock_guard lock(state.mutex);
    auto itr = state.entries.find({ owner, aura, trigger });
    if (itr == state.entries.end())
        return;
    Observation& observation = itr->second.observation;
    if (!observation.attempts)
        observation.chance = chance;
    else if (observation.chance != chance)
        observation.chance = std::numeric_limits<float>::quiet_NaN();
    if (observation.attempts != std::numeric_limits<uint32>::max())
        ++observation.attempts;
}
}
