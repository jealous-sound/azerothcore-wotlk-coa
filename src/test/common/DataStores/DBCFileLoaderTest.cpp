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

#include "DBCFileLoader.h"
#include "gtest/gtest.h"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace
{
// AutoProduceData packs fields without padding, like the structures in DBCStructure.h.
#pragma pack(push, 1)
struct TestEntry
{
    uint32 ID;
    char const* Name;
    char const* Subtext;
};
#pragma pack(pop)

std::string WriteDbc(uint32 fields, std::vector<uint32> const& records, std::string const& strings)
{
    std::vector<uint32> const header = { 0x43424457, uint32(records.size() / fields), fields, fields * 4,
        uint32(strings.size()) };

    testing::TestInfo const* test = testing::UnitTest::GetInstance()->current_test_info();
    std::filesystem::path const path = std::filesystem::temp_directory_path() / (std::string(test->name()) + ".dbc");
    std::ofstream stream(path.string(), std::ios::binary);
    stream.write(reinterpret_cast<char const*>(header.data()), header.size() * sizeof(uint32));
    stream.write(reinterpret_cast<char const*>(records.data()), records.size() * sizeof(uint32));
    stream.write(strings.data(), strings.size());
    return path.string();
}

// Two records of "nss": (1, "Alpha", <offset past the block>) and (2, "", "Beta").
std::string WriteTestDbc()
{
    return WriteDbc(3, { 1, 1, 0xFF00FE, 2, 0, 7 }, std::string("\0Alpha\0Beta\0", 12));
}
}

TEST(DBCFileLoaderTest, StringOutsideTheBlockLoadsAsEmpty)
{
    std::string const path = WriteTestDbc();
    char const* format = "nss";

    DBCFileLoader dbc;
    ASSERT_TRUE(dbc.Load(path.c_str(), format));

    uint32 records = 0;
    char** indexTable = nullptr;
    char* data = dbc.AutoProduceData(format, records, indexTable);
    char* strings = dbc.AutoProduceStrings(format, data);
    ASSERT_NE(strings, nullptr);

    TestEntry const* first = reinterpret_cast<TestEntry const*>(indexTable[1]);
    TestEntry const* second = reinterpret_cast<TestEntry const*>(indexTable[2]);
    EXPECT_STREQ(first->Name, "Alpha");
    EXPECT_STREQ(first->Subtext, "");
    EXPECT_STREQ(second->Name, "");
    EXPECT_STREQ(second->Subtext, "Beta");
    EXPECT_EQ(dbc.GetInvalidStringCount(), 1u);

    delete[] strings;
    delete[] data;
    delete[] indexTable;
    std::filesystem::remove(path);
}

TEST(DBCFileLoaderTest, IndexMinusOneIsLeftOutOfTheIndexTable)
{
    std::string const path = WriteDbc(2, { 5, 50, 0xFFFFFFFF, 60, 2, 20 }, std::string("\0", 1));
    char const* format = "ni";

    DBCFileLoader dbc;
    ASSERT_TRUE(dbc.Load(path.c_str(), format));

    uint32 records = 0;
    char** indexTable = nullptr;
    char* data = dbc.AutoProduceData(format, records, indexTable);
    ASSERT_NE(data, nullptr);
    EXPECT_EQ(records, 6u);
    ASSERT_NE(indexTable[5], nullptr);
    ASSERT_NE(indexTable[2], nullptr);
    EXPECT_EQ(*reinterpret_cast<uint32 const*>(indexTable[5] + sizeof(uint32)), 50u);
    EXPECT_EQ(*reinterpret_cast<uint32 const*>(indexTable[2] + sizeof(uint32)), 20u);
    for (uint32 id : { 0u, 1u, 3u, 4u })
        EXPECT_EQ(indexTable[id], nullptr);

    delete[] data;
    delete[] indexTable;
    std::filesystem::remove(path);
}
