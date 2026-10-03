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
#include "gtest/gtest.h"
#include <atomic>
#include <cmath>
#include <stdexcept>
#include <thread>
#include <vector>

using namespace SpellProcTestObserver;

TEST(SpellProcTestObserver, CountsOnlySubscribedOwnerAuraAndTrigger)
{
    ObjectGuid const owner = ObjectGuid::Create<HighGuid::Player>(81001);
    auto subscription = Subscribe(owner, 805743, 653272);
    EXPECT_EQ(subscription.Read().attempts, 0u);
    EXPECT_TRUE(std::isnan(subscription.Read().chance));
    Record(ObjectGuid::Create<HighGuid::Player>(81002), 805743, 653272, 5);
    Record(owner, 805744, 653272, 5);
    Record(owner, 805743, 653273, 5);
    EXPECT_EQ(subscription.Read().attempts, 0u);
    Record(owner, 805743, 653272, 5);
    EXPECT_EQ(subscription.Read().attempts, 1u);
    EXPECT_FLOAT_EQ(subscription.Read().chance, 5);
    Record(owner, 805743, 653272, 6);
    Record(owner, 805743, 653272, 5);
    EXPECT_EQ(subscription.Read().attempts, 3u);
    EXPECT_TRUE(std::isnan(subscription.Read().chance));
}

TEST(SpellProcTestObserver, LateOldUnsubscribePreservesNewGeneration)
{
    ObjectGuid const owner = ObjectGuid::Create<HighGuid::Player>(81003);
    auto old = Subscribe(owner, 805743, 653272);
    Record(owner, 805743, 653272, 5);
    auto replacement = Subscribe(owner, 805743, 653272);
    EXPECT_EQ(old.Read().attempts, 0u);
    EXPECT_EQ(replacement.Read().attempts, 0u);
    old = {};
    Record(owner, 805743, 653272, 5);
    EXPECT_EQ(replacement.Read().attempts, 1u);
}

TEST(SpellProcTestObserver, ConcurrentWritersAndReaderRetainAllAttempts)
{
    ObjectGuid const owner = ObjectGuid::Create<HighGuid::Player>(81004);
    auto subscription = Subscribe(owner, 805743, 653272);
    std::atomic<uint32> finished = 0;
    std::vector<std::thread> writers;
    for (uint32 thread = 0; thread < 4; ++thread)
        writers.emplace_back([&]
        {
            for (uint32 attempt = 0; attempt < 2000; ++attempt)
                Record(owner, 805743, 653272, 5);
            ++finished;
        });
    while (finished.load() != 4)
    {
        auto const observation = subscription.Read();
        EXPECT_LE(observation.attempts, 8000u);
        if (observation.attempts)
            EXPECT_FLOAT_EQ(observation.chance, 5);
        std::this_thread::yield();
    }
    for (auto& writer : writers)
        writer.join();
    EXPECT_EQ(subscription.Read().attempts, 8000u);
    EXPECT_FLOAT_EQ(subscription.Read().chance, 5);
}

TEST(SpellProcTestObserver, BoundedSubscriptionsReleaseDuringExceptionUnwind)
{
    ObjectGuid const owner = ObjectGuid::Create<HighGuid::Player>(81005);
    EXPECT_THROW(([&]
    {
        std::vector<Subscription> subscriptions;
        for (uint32 aura = 1; aura <= 128; ++aura)
            subscriptions.push_back(Subscribe(owner, aura, 653272));
        EXPECT_THROW(Subscribe(owner, 129, 653272), std::runtime_error);
        throw std::runtime_error("Unwind subscribed scenario");
    })(), std::runtime_error);
    std::vector<Subscription> subscriptions;
    for (uint32 aura = 1; aura <= 128; ++aura)
        subscriptions.push_back(Subscribe(owner, aura, 653272));
    EXPECT_EQ(subscriptions.size(), 128u);
}
