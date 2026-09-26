/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#ifndef ASCENSION_PATCH_ACHIEVEMENT_DATA_H
#define ASCENSION_PATCH_ACHIEVEMENT_DATA_H

#include "Define.h"
#include <array>
#include <string_view>

namespace AscensionPatchAchievementData
{
inline constexpr std::string_view AchievementSourceSha256 =
    "0a03ebc05bdc8dabdd134a04efb64c8bc909ea6e5f62860288b2aa5400f552af";

inline constexpr std::string_view CriteriaSourceSha256 =
    "4ddcc8300a008b96a6b0625e79a99c9efb86486ec9026877591acd7677f989e2";

inline constexpr std::string_view CategorySourceSha256 =
    "3b8f666e58ebb5c5f584a1060efcb5854d8ce14ec10845ac471b873a97284985";

template <std::size_t FieldCount, std::size_t StringCount>
struct Record
{
    std::array<uint32, FieldCount> Fields;
    std::array<std::string_view, StringCount> Strings;
};

inline constexpr std::array<Record<14, 3>, 336> Achievements = {{
    {
        {{
            5000u, 4294967295u, 4294967295u, 0u, 0u, 0u, 15105u, 5u,
            1u, 0u, 4467u, 0u, 0u, 0u
        }},
        {{
            "Ascension Season 1 Level 20",
            "",
            ""
        }}
    },
    {
        {{
            5001u, 4294967295u, 4294967295u, 0u, 0u, 0u, 15105u, 5u,
            2u, 0u, 4468u, 0u, 0u, 0u
        }},
        {{
            "Ascension Season 1 Level 30",
            "",
            ""
        }}
    },
    {
        {{
            5002u, 4294967295u, 4294967295u, 0u, 0u, 0u, 15105u, 5u,
            3u, 0u, 4469u, 0u, 0u, 0u
        }},
        {{
            "Ascension Season 1 Level 40",
            "",
            ""
        }}
    },
    {
        {{
            5003u, 4294967295u, 4294967295u, 0u, 0u, 0u, 15105u, 5u,
            4u, 0u, 4470u, 0u, 0u, 0u
        }},
        {{
            "Ascension Season 1 Level 50",
            "",
            ""
        }}
    },
    {
        {{
            5004u, 4294967295u, 4294967295u, 0u, 0u, 0u, 15105u, 5u,
            5u, 0u, 4471u, 0u, 0u, 0u
        }},
        {{
            "Ascension Season 1 Level 58",
            "",
            ""
        }}
    },
    {
        {{
            5005u, 4294967295u, 4294967295u, 0u, 0u, 0u, 15105u, 5u,
            6u, 0u, 4472u, 0u, 0u, 0u
        }},
        {{
            "Ascension Season 1 Level 60",
            "",
            ""
        }}
    },
    {
        {{
            5301u, 4294967295u, 4294967295u, 0u, 0u, 0u, 15105u, 25u,
            2u, 0u, 4474u, 0u, 0u, 0u
        }},
        {{
            "Season I Supporter Chest",
            "Supported Ascension Season 1",
            ""
        }}
    },
    {
        {{
            5302u, 4294967295u, 4294967295u, 0u, 0u, 0u, 15105u, 25u,
            3u, 0u, 4474u, 0u, 0u, 0u
        }},
        {{
            "Season I Warchest",
            "Supported Ascension Season 1",
            ""
        }}
    },
    {
        {{
            5861u, 4294967295u, 4294967295u, 0u, 0u, 0u, 15105u, 5u,
            2u, 0u, 4474u, 0u, 1u, 0u
        }},
        {{
            "Season II Supporter",
            "Supported Ascension Season 2",
            ""
        }}
    },
    {
        {{
            6312u, 4294967295u, 4294967295u, 0u, 0u, 0u, 81u, 0u,
            10u, 256u, 4402u, 0u, 0u, 0u
        }},
        {{
            "Realm First! Learned The Book of Ascension",
            "First player to learn The Book Of Ascension",
            ""
        }}
    },
    {
        {{
            6660u, 4294967295u, 4294967295u, 0u, 0u, 0u, 15105u, 5u,
            2u, 0u, 4474u, 0u, 1u, 0u
        }},
        {{
            "Season III Supporter",
            "Supported Ascension Season 3",
            ""
        }}
    },
    {
        {{
            8660u, 4294967295u, 4294967295u, 0u, 0u, 0u, 15105u, 5u,
            2u, 0u, 4474u, 0u, 1u, 0u
        }},
        {{
            "Season III Supporter",
            "Supported Ascension Season 3",
            ""
        }}
    },
    {
        {{
            10312u, 4294967295u, 4294967295u, 0u, 0u, 0u, 81u, 0u,
            10u, 256u, 4402u, 0u, 0u, 0u
        }},
        {{
            "Realm First! Learned The Book of Ascension",
            "First player to learn The Book Of Ascension",
            ""
        }}
    },
    {
        {{
            10660u, 4294967295u, 4294967295u, 0u, 0u, 0u, 15105u, 5u,
            2u, 0u, 4474u, 0u, 1u, 0u
        }},
        {{
            "Season V Supporter",
            "Supported Ascension Season 5",
            ""
        }}
    },
    {
        {{
            15312u, 4294967295u, 4294967295u, 0u, 0u, 0u, 81u, 0u,
            10u, 256u, 4402u, 0u, 1u, 0u
        }},
        {{
            "Realm First! Nightmare Mode: Learned The Book of Ascension",
            "First player to learn The Book Of Ascension",
            ""
        }}
    },
    {
        {{
            16719u, 4294967295u, 4294967295u, 16718u, 0u, 0u, 16134u, 5u,
            4u, 0u, 4544u, 0u, 2u, 0u
        }},
        {{
            "Seasonal: Rank II",
            "Earned 100 Seasonal Essences & 1000 Marks of Ascension",
            ""
        }}
    },
    {
        {{
            16720u, 4294967295u, 4294967295u, 16719u, 0u, 0u, 16134u, 5u,
            4u, 0u, 4545u, 0u, 2u, 0u
        }},
        {{
            "Seasonal: Rank III",
            "Earned 300 Seasonal Essences & 5000 Marks of Ascension",
            ""
        }}
    },
    {
        {{
            16721u, 4294967295u, 4294967295u, 16720u, 0u, 0u, 16134u, 5u,
            4u, 0u, 4546u, 0u, 2u, 0u
        }},
        {{
            "Seasonal: Rank IV",
            "Earned 600 Seasonal Essences & 10000 Marks of Ascension",
            ""
        }}
    },
    {
        {{
            16722u, 4294967295u, 4294967295u, 16721u, 0u, 0u, 16134u, 5u,
            4u, 0u, 4547u, 0u, 2u, 0u
        }},
        {{
            "Seasonal: Rank V",
            "Earned 1500 Seasonal Essences & 25000 Marks of Ascension",
            ""
        }}
    },
    {
        {{
            16723u, 4294967295u, 4294967295u, 16722u, 0u, 0u, 16134u, 5u,
            4u, 0u, 4548u, 0u, 2u, 0u
        }},
        {{
            "Seasonal: Rank VI",
            "Earned 3000 Seasonal Essences & 50000 Marks of Ascension",
            ""
        }}
    },
    {
        {{
            16724u, 4294967295u, 4294967295u, 16723u, 0u, 0u, 16134u, 5u,
            4u, 0u, 4549u, 0u, 2u, 0u
        }},
        {{
            "Seasonal: Rank VII",
            "Earned 5000 Seasonal Essences & 100000 Marks of Ascension",
            ""
        }}
    },
    {
        {{
            17036u, 4294967295u, 4294967295u, 0u, 0u, 0u, 16134u, 5u,
            2u, 0u, 4474u, 0u, 1u, 0u
        }},
        {{
            "Seasonal: Supporter",
            "Supported Ascension Seasonal",
            ""
        }}
    },
    {
        {{
            41067u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            1u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (100)",
            "Obtain 100 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41068u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            2u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (500)",
            "Obtain 500 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41069u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            3u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (750)",
            "Obtain 750 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41070u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            4u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (1000)",
            "Obtain 1000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41071u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            5u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (2500)",
            "Obtain 2500 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41072u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            6u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (5000)",
            "Obtain 5000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41073u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            7u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (7500)",
            "Obtain 7500 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41074u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            8u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (10000)",
            "Obtain 10000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41075u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            9u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (12500)",
            "Obtain 12500 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41076u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            10u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (15000)",
            "Obtain 15000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41077u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            11u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (17500)",
            "Obtain 17500 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41078u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            12u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (20000)",
            "Obtain 20000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41079u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            13u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (22500)",
            "Obtain 22500 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41080u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            14u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (25000)",
            "Obtain 25000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41081u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            15u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (27500)",
            "Obtain 27500 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41082u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            16u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (30000)",
            "Obtain 30000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41083u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            17u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (32500)",
            "Obtain 32500 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41084u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            18u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (35000)",
            "Obtain 35000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41085u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            19u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (37500)",
            "Obtain 37500 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41086u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            20u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (40000)",
            "Obtain 40000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41087u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            21u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (42500)",
            "Obtain 42500 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41088u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            22u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (45000)",
            "Obtain 45000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41089u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            23u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (50000)",
            "Obtain 50000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41090u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            24u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (55000)",
            "Obtain 55000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41091u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            25u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (60000)",
            "Obtain 60000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41092u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            26u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (65000)",
            "Obtain 65000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41093u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            27u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (70000)",
            "Obtain 70000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41094u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            28u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (75000)",
            "Obtain 75000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41095u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            29u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (80000)",
            "Obtain 80000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41096u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            30u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (85000)",
            "Obtain 85000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41097u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            31u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (90000)",
            "Obtain 90000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41098u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            32u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (95000)",
            "Obtain 95000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41099u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            33u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (100000) ",
            "Obtain 100000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41100u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            34u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (110000)",
            "Obtain 110000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41101u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            35u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (120000)",
            "Obtain 120000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41102u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            36u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (130000)",
            "Obtain 130000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41103u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            37u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (140000)",
            "Obtain 140000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41104u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            38u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (150000)",
            "Obtain 150000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41105u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            39u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (160000)",
            "Obtain 160000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41106u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            40u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (170000)",
            "Obtain 170000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41107u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            41u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (180000)",
            "Obtain 180000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41108u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            42u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (190000)",
            "Obtain 190000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41109u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            43u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (200000)",
            "Obtain 200000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41110u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            44u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (210000)",
            "Obtain 210000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41111u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            45u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (235000)",
            "Obtain 235000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41112u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            46u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (260000)",
            "Obtain 260000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41113u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            47u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (285000)",
            "Obtain 285000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41114u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            48u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (310000)",
            "Obtain 310000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41115u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            49u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (335000)",
            "Obtain 335000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41116u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            50u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (360000)",
            "Obtain 360000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41117u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            51u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (385000)",
            "Obtain 385000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41118u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            52u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (410000)",
            "Obtain 410000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41119u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            53u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (435000)",
            "Obtain 435000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41120u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            54u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (460000)",
            "Obtain 460000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41121u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            55u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (485000)",
            "Obtain 485000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41122u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            56u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (500000)",
            "Obtain 500000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41123u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            57u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (550000)",
            "Obtain 550000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41124u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            58u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (600000)",
            "Obtain 600000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41125u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            59u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (650000)",
            "Obtain 650000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41126u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            60u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (700000)",
            "Obtain 700000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41127u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            61u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (750000)",
            "Obtain 750000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41128u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            62u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (800000)",
            "Obtain 800000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41129u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            63u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (850000)",
            "Obtain 850000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41130u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            64u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (900000)",
            "Obtain 900000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41131u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            65u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (950000)",
            "Obtain 950000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41132u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (1000000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41141u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (1000000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41142u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (1100000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41143u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (1200000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41144u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (1300000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41145u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (1400000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41146u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (1500000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41147u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (1600000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41148u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (1700000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41149u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (1800000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41150u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (1900000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41151u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (2000000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41152u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (2100000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41153u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (2200000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41154u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (2300000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41155u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (2400000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41156u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (2500000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41157u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (2600000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41158u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (2700000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41159u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (2800000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41160u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (2900000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41161u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (3000000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41162u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (3100000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41163u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (3200000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41164u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (3300000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41165u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (3400000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41166u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (3500000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41167u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (3600000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41168u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (3700000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41169u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (3800000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41170u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (3900000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41171u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (4000000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41172u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (4100000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41173u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (4200000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41174u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (4300000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41175u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (4400000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41176u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (4500000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41177u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (4600000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41178u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (4700000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41179u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (4800000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41180u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (4900000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41181u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (5000000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41182u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (5500000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41183u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (6000000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41184u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (6500000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41185u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (7000000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41186u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (7500000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41187u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (8000000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41188u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (8500000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41189u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (9000000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41190u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (9500000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            41191u, 4294967295u, 4294967295u, 0u, 0u, 0u, 21000u, 100u,
            66u, 4u, 1833u, 0u, 0u, 0u
        }},
        {{
            "Runes of Ascension (10000000)",
            "Obtain 1000000 Marks of Ascension.",
            ""
        }}
    },
    {
        {{
            86676u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 776u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Visit an Innkeeper",
            "Visit an innkeeper. You can use the map lens to locate innkeepers in towns and cities.",
            ""
        }}
    },
    {
        {{
            86679u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 28347u, 0u, 1u, 0u
        }},
        {{
            "Path to Ascension: Call Boards",
            "Interact with the Call Board in your Capital City. Ask a Guard for directions!",
            ""
        }}
    },
    {
        {{
            86687u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 4462u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Fel Commutation Portal",
            "Discover the location of the Fel Commutation Portal in the Cleft of Shadows within "
            "Ogrimmar and speak with Belpix.",
            ""
        }}
    },
    {
        {{
            86688u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 4462u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Fel Commutation Portal",
            "Discover the location of the Fel Commutation Portal in the basement of the Slaughtered "
            "Lamb Inn in Stormwind's Mage District and speak with Belpix.",
            ""
        }}
    },
    {
        {{
            86710u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 1176u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Novice Rider",
            "Summon your new mount.",
            ""
        }}
    },
    {
        {{
            86711u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 956u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Novice Rider",
            "Summon your new mount.",
            ""
        }}
    },
    {
        {{
            86712u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 1177u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Novice Rider",
            "Summon your new mount.",
            ""
        }}
    },
    {
        {{
            86713u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 836u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Novice Rider",
            "Summon your new mount.",
            ""
        }}
    },
    {
        {{
            86714u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 1241u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Novice Rider",
            "Summon your new mount.",
            ""
        }}
    },
    {
        {{
            86715u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 1682u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Novice Rider",
            "Summon your new mount.",
            ""
        }}
    },
    {
        {{
            86716u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 1240u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Novice Rider",
            "Summon your new mount.",
            ""
        }}
    },
    {
        {{
            86717u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 1180u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Novice Rider",
            "Summon your new mount.",
            ""
        }}
    },
    {
        {{
            86718u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 2398u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Novice Rider",
            "Summon your new mount.",
            ""
        }}
    },
    {
        {{
            86719u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 2404u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Novice Rider",
            "Summon your new mount.",
            ""
        }}
    },
    {
        {{
            87200u, 4294967295u, 4294967295u, 0u, 0u, 0u, 12010u, 10u,
            10u, 1024u, 3609u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Choose Your Risk",
            "Choose your risk mode in the PvP Ruleset UI (Hotkey H)",
            ""
        }}
    },
    {
        {{
            87201u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 6455u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Interesting Ethereals",
            "Talk to Tiraxis and Ameer Greatluck",
            ""
        }}
    },
    {
        {{
            87202u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 4650u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mystic Extracts",
            "Obtain an Untarnished Mystic Scroll",
            ""
        }}
    },
    {
        {{
            87203u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 4648u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mystic Enchant Collection",
            "Save a Mystic Enchant Scroll to your Enchant Collection",
            ""
        }}
    },
    {
        {{
            87204u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 64206u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Vanity Auctions",
            "Buy a Vanity Weapon from the Auction House",
            ""
        }}
    },
    {
        {{
            87205u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 1575u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Transmogrify Your Appearance",
            "Transmogrify any item at Marazz or in your Wardrobe",
            ""
        }}
    },
    {
        {{
            87206u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 17141u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: End Game Builds",
            "Load a featured end-game build",
            ""
        }}
    },
    {
        {{
            87207u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 21992u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: PvE Power",
            "Reach 460 PVE Power by equipping soulbound gear",
            ""
        }}
    },
    {
        {{
            87208u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 1648u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: PvP Power",
            "Equip PvP or Bloodforged gear in each gear slot to reach 250 PVP Power.",
            ""
        }}
    },
    {
        {{
            87209u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 7712u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Gear Up!",
            "Equip more powerful gear to reach 40 item level.",
            ""
        }}
    },
    {
        {{
            87210u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 19133u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Epic Mystic Enchant Collecting",
            "Save 3 Epic Enchants to your Enchant Collection",
            ""
        }}
    },
    {
        {{
            87211u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 19133u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Legendary Mystic Enchant Collecting",
            "Save a Legendary Enchant to your Enchant Collection",
            ""
        }}
    },
    {
        {{
            87212u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 27816u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Dungeon Diving",
            "Defeat the final boss in any end game Normal Dungeon",
            ""
        }}
    },
    {
        {{
            87213u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 27819u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Heroic Dungeons",
            "Complete any Heroic Dungeon",
            ""
        }}
    },
    {
        {{
            87214u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3649u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Normal Raiding",
            "Defeat the final boss in any Normal Raid",
            ""
        }}
    },
    {
        {{
            87215u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 27817u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mythic Dungeons",
            "Defeat the final boss in any Mythic Dungeon",
            ""
        }}
    },
    {
        {{
            87216u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 27818u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mythic+ Dungeons",
            "Complete any Mythic+ Dungeon",
            ""
        }}
    },
    {
        {{
            87217u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3819u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Heroic Raiding",
            "Defeat the final boss in any Heroic Raid",
            ""
        }}
    },
    {
        {{
            87218u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 4075u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Ascended Raiding",
            "Defeat the final boss in any Ascended Raid",
            ""
        }}
    },
    {
        {{
            87219u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 17922u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Bloodforging",
            "Bloodforge a piece of gear",
            ""
        }}
    },
    {
        {{
            87220u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 1648u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Bloodforged Auctions",
            "Buy Bloodforged Gear from the Auction House.",
            ""
        }}
    },
    {
        {{
            87221u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3387u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Battlegrounds!",
            "Complete any Battleground!",
            ""
        }}
    },
    {
        {{
            87222u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3100u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: 2v2 Solo Que",
            "Join a 2v2 Arena Game using the Solo Queue function.",
            ""
        }}
    },
    {
        {{
            87223u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 4616u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Talk to an Arena Vendor.",
            "Visit and View any Arena Vendor's Wares.",
            ""
        }}
    },
    {
        {{
            87224u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 2873u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Less Running, More Ressing",
            "Resurrect in Closest Town or Capital",
            ""
        }}
    },
    {
        {{
            87225u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 4650u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mystic Enchant Collector",
            "Save Mystic Enchants to your Enchanting Collection",
            ""
        }}
    },
    {
        {{
            87226u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 1644u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Epic Riding",
            "Learn Epic Riding.",
            ""
        }}
    },
    {
        {{
            87227u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 17141u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Specialization II",
            "Learn a Second Specialization.",
            ""
        }}
    },
    {
        {{
            87228u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 87u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Bazaar Token",
            "Obtain 1 Bazaar Token",
            ""
        }}
    },
    {
        {{
            87229u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 6455u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Ethereal Bazaar",
            "Buy any item from the Ethereal Bazaar",
            ""
        }}
    },
    {
        {{
            87230u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 26245u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: First Class Experimental Teleporter",
            "Use the First Class Experimental Teleporter.",
            ""
        }}
    },
    {
        {{
            87231u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3390u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Wargames",
            "Complete any Wargame.",
            ""
        }}
    },
    {
        {{
            87232u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 19121u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Titan Scrolls",
            "Use any Titan Scroll.",
            ""
        }}
    },
    {
        {{
            87233u, 4294967295u, 4294967295u, 0u, 0u, 0u, 12010u, 10u,
            10u, 1024u, 28558u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Speak with a Mentor Recruiter.",
            "Seek out Riko in any capital city.",
            ""
        }}
    },
    {
        {{
            87234u, 4294967295u, 4294967295u, 0u, 0u, 0u, 12010u, 10u,
            10u, 1024u, 3660u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Take Down a Faction Leader",
            "Slay any Faction Leader.",
            ""
        }}
    },
    {
        {{
            87235u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 1700u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Take Down a World Boss",
            "Slay any World Boss.",
            ""
        }}
    },
    {
        {{
            87236u, 4294967295u, 4294967295u, 0u, 0u, 0u, 12010u, 10u,
            10u, 1024u, 3400u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Blood Bowl",
            "Slay 1 player inside the Blood Bowl.",
            ""
        }}
    },
    {
        {{
            87237u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 31029u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Complete an Invasion",
            "Defeat an Invasion Boss.",
            ""
        }}
    },
    {
        {{
            87238u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 4648u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Apply a Mystic Enchantment",
            "Activate a Mystic Enchant from your Collections.",
            ""
        }}
    },
    {
        {{
            87239u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 27391u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Prestige",
            "Obtain a Prestigious Pamphet from Chromie and Prestige, resetting to level 1.",
            ""
        }}
    },
    {
        {{
            87240u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 307u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Choose Your Path",
            "Hit N, then Choose your Path in the upper right section of Character Advancement.",
            ""
        }}
    },
    {
        {{
            87241u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 21548u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Collection and Capitals",
            "Hit N then select Vanity Tab in your Character Advancement menu.",
            ""
        }}
    },
    {
        {{
            87242u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 4448u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Seeking Power",
            "Apply a Mystic Enchant Scroll at the Mystic Enchanting Altar!",
            ""
        }}
    },
    {
        {{
            87243u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 2365u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Fel Commute Your Gear",
            "Interact with the Fel Commutation Portal and protect your gear by clicking gear slots, "
            "then Commute Slot.",
            ""
        }}
    },
    {
        {{
            87245u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 17141u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Building Your Class Fantasy",
            "Purchase and use a Temporal Contract from Chromie.",
            ""
        }}
    },
    {
        {{
            87246u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 4402u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Rank Up Your Spells",
            "Rank Up your Spells at a Beginner's Book of Ascension.",
            ""
        }}
    },
    {
        {{
            87247u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 72997u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Reach Level 10!",
            "Earn enough experience to reach level 10.",
            ""
        }}
    },
    {
        {{
            87248u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 17141u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Building Your Class Fantasy",
            "Hit N, then click the Hero Architect Tab.",
            ""
        }}
    },
    {
        {{
            87250u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 2418u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Power of Professions",
            "Speak to the Book of Artisans and learn a profession.",
            ""
        }}
    },
    {
        {{
            87251u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 72997u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Reach Level 15!",
            "Reach level 15",
            ""
        }}
    },
    {
        {{
            87252u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 2055u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Report to the Flightmaster",
            "Speak to Doras the Flightmaster at top of the tower next to the Ogrimmar Bank.",
            ""
        }}
    },
    {
        {{
            87253u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3842u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Report to the Flightmaster",
            "Head to the East Corner of the Trade District, then follow the ramp up, speak to Dungar "
            "Longdrink.",
            ""
        }}
    },
    {
        {{
            87254u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 72997u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Reach Level 20!",
            "Reach level 20",
            ""
        }}
    },
    {
        {{
            87255u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3524u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Hybrid Risk",
            "Speak to Blood Keeper Rozok by the Orgrimmar Call Board.",
            ""
        }}
    },
    {
        {{
            87256u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3524u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Hybrid Risk",
            "Speak to Battlemaster Tressa by the Stormwind Call Board. Select No-Risk PvP, No-Risk PvE "
            "or High-Risk.",
            ""
        }}
    },
    {
        {{
            87257u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 27816u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Dungeon Finder",
            "Open the Dungeon Finder by hitting I or click the Green Eye icon in your options menu.",
            ""
        }}
    },
    {
        {{
            87258u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 956u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Apprentice Riding!",
            "Learn Apprentice Riding Training. Speak to Kildar in the Valley of Honor within Ogrimmar "
            "and select 'I seek to training to ride a steed.'",
            ""
        }}
    },
    {
        {{
            87259u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 1176u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Apprentice Riding!",
            "Learn Apprentice Riding Training. Speak to Randal Hunter at the Eastvale Logging Camp "
            "outside Stormwind in Elwynn Forest and select 'I seek to training to ride a steed.'",
            ""
        }}
    },
    {
        {{
            87260u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 956u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mount Up!",
            "Open your bags and right click your new Mount.",
            ""
        }}
    },
    {
        {{
            87261u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 1176u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mount Up!",
            "Open your bags and right click your new Mount.",
            ""
        }}
    },
    {
        {{
            87262u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 72997u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Reach Level 60!",
            "Reach level 60",
            ""
        }}
    },
    {
        {{
            87263u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 72997u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Reach Level 70!",
            "Reach level 70",
            ""
        }}
    },
    {
        {{
            87264u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 455u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Pet Talents",
            "Click Pet Talents by right clicking your Pet's portrait.",
            ""
        }}
    },
    {
        {{
            87265u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 80293u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Fully Mystic Enchanted",
            "Fill all 17 Active Enchant slots.",
            ""
        }}
    },
    {
        {{
            87266u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 4648u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Activate a Legendary Mystic Enchant",
            "Activate a Legendary Mystic Enchant.",
            ""
        }}
    },
    {
        {{
            87267u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 80292u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Epic Mystic Enchants",
            "Have 3 Epic Mystic Enchants Active!",
            ""
        }}
    },
    {
        {{
            87268u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 4651u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mystic Enchant Specializations",
            "Unlock a new Mystic Enchant Specializations.",
            ""
        }}
    },
    {
        {{
            87269u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 19133u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Worldforged Enchants",
            "Acquire a Worldforged Mystic Enchant Scroll and save it to your collection.",
            ""
        }}
    },
    {
        {{
            87270u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 80293u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Fully Mystic Enchanted",
            "Fill all 17 Active Enchant slots with Rare and Uncommon Enchants.",
            ""
        }}
    },
    {
        {{
            87271u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 4648u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mystic Enchant Collector",
            "Save 10 Mystic Enchants to your collection.",
            ""
        }}
    },
    {
        {{
            87272u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 63839u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension Wildcard: Reroll Spells",
            "Try to Reroll your least used spells and abilities that are not increasing your build's "
            "potential.",
            ""
        }}
    },
    {
        {{
            87273u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 63839u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension Wildcard: Reroll Talents",
            "Reroll your talents using Scrolls of Fortune.",
            ""
        }}
    },
    {
        {{
            87274u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 63839u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension Wildcard: Locking Spells and Talents",
            "Lock your Spells and Talents.",
            ""
        }}
    },
    {
        {{
            87275u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 63839u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension Wildcard: Skill Card Packs",
            "Open a Sealed Card Pack.",
            ""
        }}
    },
    {
        {{
            87276u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 63839u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension Wildcard: Golden Skill Cards",
            "Use a Golden Skill Card.",
            ""
        }}
    },
    {
        {{
            87277u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 63839u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension Wildcard: Darkmoon Tickets",
            "Use a Darkmoon ticket.",
            ""
        }}
    },
    {
        {{
            87278u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 63839u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension Wildcard: Activate a Skill Card",
            "Activate a Skill Card at level 1-9.",
            ""
        }}
    },
    {
        {{
            87293u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3649u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Normal Zul'Gurub",
            "Defeat Hakkar in Zul'Gurub on Normal Difficulty.",
            ""
        }}
    },
    {
        {{
            87294u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3819u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Normal Molten Core",
            "Defeat Ragnaros in Molten Core on Normal Difficulty.",
            ""
        }}
    },
    {
        {{
            87295u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3820u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Normal Onyxia's Lair",
            "Defeat Onyxia in her lair on Normal Difficulty.",
            ""
        }}
    },
    {
        {{
            87296u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3817u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Normal Blackwing Lair",
            "Defeat Nefarian in Blackwing Lair on Normal Difficulty.",
            ""
        }}
    },
    {
        {{
            87297u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3681u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Normal Ruins of Ahn'Qiraj",
            "Defeat Ossirian the Unscarred in Ruins of Ahn'Qiraj on Normal Difficulty.",
            ""
        }}
    },
    {
        {{
            87298u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3677u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Normal Temple of Ahn'Qiraj",
            "Defeat C'thun in Temple of Ahn'Qiraj on Normal difficulty.",
            ""
        }}
    },
    {
        {{
            87299u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 4075u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Normal Naxxramas",
            "Defeat Kel'thuzad in Naxxramas on Normal Difficulty.",
            ""
        }}
    },
    {
        {{
            87300u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3649u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Heroic Zul'Gurub",
            "Defeat Hakkar in Zul'Gurub on Heroic Difficulty.",
            ""
        }}
    },
    {
        {{
            87301u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3819u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Heroic Molten Core",
            "Defeat Ragnaros in Molten Core on Heroic Difficulty.",
            ""
        }}
    },
    {
        {{
            87302u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3820u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Heroic Onyxia's Lair",
            "Defeat Onyxia in her lair on Heroic Difficulty.",
            ""
        }}
    },
    {
        {{
            87303u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3817u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Heroic Blackwing Lair",
            "Defeat Nefarian in Blackwing Lair on Heroic Difficulty.",
            ""
        }}
    },
    {
        {{
            87304u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3681u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Heroic Ruins of Ahn'Qiraj",
            "Defeat Ossirian the Unscarred in Ruins of Ahn'Qiraj on Heroic Difficulty.",
            ""
        }}
    },
    {
        {{
            87305u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3677u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Heroic Temple of Ahn'Qiraj",
            "Defeat C'thun in Temple of Ahn'Qiraj on Heroic difficulty.",
            ""
        }}
    },
    {
        {{
            87306u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 4075u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Heroic Naxxramas",
            "Defeat Kel'thuzad in Naxxramas on Heroic Difficulty.",
            ""
        }}
    },
    {
        {{
            87307u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3649u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mythic Zul'Gurub",
            "Defeat Hakkar in Zul'Gurub on Mythic Difficulty.",
            ""
        }}
    },
    {
        {{
            87308u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3819u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mythicl Molten Core",
            "Defeat Ragnaros in Molten Core on Mythic Difficulty.",
            ""
        }}
    },
    {
        {{
            87309u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3820u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mythic Onyxia's Lair",
            "Defeat Onyxia in her lair on Mythic Difficulty.",
            ""
        }}
    },
    {
        {{
            87310u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3817u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mythic Blackwing Lair",
            "Defeat Nefarian in Blackwing Lair on Mythic Difficulty.",
            ""
        }}
    },
    {
        {{
            87311u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3681u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mythic Ruins of Ahn'Qiraj",
            "Defeat Ossirian the Unscarred in Ruins of Ahn'Qiraj on Mythic Difficulty.",
            ""
        }}
    },
    {
        {{
            87312u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3677u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mythic Temple of Ahn'Qiraj",
            "Defeat C'thun in Temple of Ahn'Qiraj on Heroic difficulty.",
            ""
        }}
    },
    {
        {{
            87313u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 4075u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mythic Naxxramas",
            "Defeat Kel'thuzad in Naxxramas on Mythic Difficulty.",
            ""
        }}
    },
    {
        {{
            87314u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3649u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Ascended Zul'Gurub",
            "Defeat Hakkar in Zul'Gurub on Ascended Difficulty.",
            ""
        }}
    },
    {
        {{
            87315u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3819u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Ascended Molten Core",
            "Defeat Ragnaros in Molten Core on Ascended Difficulty.",
            ""
        }}
    },
    {
        {{
            87316u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3820u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Ascended Onyxia's Lair",
            "Defeat Onyxia in her lair on Ascended Difficulty.",
            ""
        }}
    },
    {
        {{
            87317u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3817u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Ascended Blackwing Lair",
            "Defeat Nefarian in Blackwing Lair on Ascended Difficulty.",
            ""
        }}
    },
    {
        {{
            87318u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3681u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Ascended Ruins of Ahn'Qiraj",
            "Defeat Ossirian the Unscarred in Ruins of Ahn'Qiraj on Ascended Difficulty.",
            ""
        }}
    },
    {
        {{
            87319u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3677u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Ascended Temple of Ahn'Qiraj",
            "Defeat C'thun in Temple of Ahn'Qiraj on Heroic difficulty.",
            ""
        }}
    },
    {
        {{
            87320u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 4075u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Ascended Naxxramas",
            "Defeat Kel'thuzad in Naxxramas on Ascended Difficulty.",
            ""
        }}
    },
    {
        {{
            87321u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 64757u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Normal Karazhan",
            "Defeat Prince Malchezaar in Karazhan on Normal difficult.",
            ""
        }}
    },
    {
        {{
            87322u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 28520u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Normal Gruul's Lair",
            "Defeat Gruul the Dragonslayer in Gruul's Lair on Normal difficulty.",
            ""
        }}
    },
    {
        {{
            87323u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3640u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Normal Magtheridon's Lair",
            "Defeat Magtheridon in Magtheridon's Lair on Normal difficulty.",
            ""
        }}
    },
    {
        {{
            87324u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3658u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Normal Serpentshrine Cavern",
            "Defeat Lady Vashj in Serpentshrine Cavern on Normal difficulty.",
            ""
        }}
    },
    {
        {{
            87325u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3664u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Normal Tempest Keep",
            "Defeat Kael'thas Sunstrider in Tempest Keep at Normal difficulty.",
            ""
        }}
    },
    {
        {{
            87326u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3630u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Normal Zul'Aman",
            "Defeat Zul'jin in Zul'Aman on Normal difficulty.",
            ""
        }}
    },
    {
        {{
            87327u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 55125u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Normal Battle for Mount Hyjal",
            "Defeat Archimonde in Battle for Mount Hyjal on Normal difficulty.",
            ""
        }}
    },
    {
        {{
            87328u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3663u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Normal Black Temple",
            "Defeat Illidan Stormrage in Black Temple on Normal difficulty.",
            ""
        }}
    },
    {
        {{
            87329u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3592u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Normal Sunwell Plateau",
            "Defeat Kil'jaeden in Sunwell Plateau on Normal difficulty.",
            ""
        }}
    },
    {
        {{
            87330u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 64757u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Heroic Karazhan",
            "Defeat Prince Malchezaar in Karazhan on Heroic difficult.",
            ""
        }}
    },
    {
        {{
            87331u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 28520u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Heroic Gruul's Lair",
            "Defeat Gruul the Dragonslayer in Gruul's Lair on Heroic difficulty.",
            ""
        }}
    },
    {
        {{
            87332u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3640u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Heroic Magtheridon's Lair",
            "Defeat Magtheridon in Magtheridon's Lair on Heroic difficulty.",
            ""
        }}
    },
    {
        {{
            87333u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3658u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Heroic Serpentshrine Cavern",
            "Defeat Lady Vashj in Serpentshrine Cavern on Heroic difficulty.",
            ""
        }}
    },
    {
        {{
            87334u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3664u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Heroic Tempest Keep",
            "Defeat Kael'thas Sunstrider in Tempest Keep at Heroic difficulty.",
            ""
        }}
    },
    {
        {{
            87335u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3630u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Heroic Zul'Aman",
            "Defeat Zul'jin in Zul'Aman on Heroic difficulty.",
            ""
        }}
    },
    {
        {{
            87336u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 55125u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Heroic Battle for Mount Hyjal",
            "Defeat Archimonde in Battle for Mount Hyjal on Heroic difficulty.",
            ""
        }}
    },
    {
        {{
            87337u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3663u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Heroic Black Temple",
            "Defeat Illidan Stormrage in Black Temple on Heroic difficulty.",
            ""
        }}
    },
    {
        {{
            87338u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3592u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Heroic Sunwell Plateau",
            "Defeat Kil'jaeden in Sunwell Plateau on Heroic difficulty.",
            ""
        }}
    },
    {
        {{
            87339u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 64757u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mythic Karazhan",
            "Defeat Prince Malchezaar in Karazhan on Mythic difficult.",
            ""
        }}
    },
    {
        {{
            87340u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 28520u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mythic Gruul's Lair",
            "Defeat Gruul the Dragonslayer in Gruul's Lair on Mythic difficulty.",
            ""
        }}
    },
    {
        {{
            87341u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3640u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mythic Magtheridon's Lair",
            "Defeat Magtheridon in Magtheridon's Lair on Mythic difficulty.",
            ""
        }}
    },
    {
        {{
            87342u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3658u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mythic Serpentshrine Cavern",
            "Defeat Lady Vashj in Serpentshrine Cavern on Mythic difficulty.",
            ""
        }}
    },
    {
        {{
            87343u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3664u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mythic Tempest Keep",
            "Defeat Kael'thas Sunstrider in Tempest Keep at Mythic difficulty.",
            ""
        }}
    },
    {
        {{
            87344u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3630u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mythic Zul'Aman",
            "Defeat Zul'jin in Zul'Aman on Mythic difficulty.",
            ""
        }}
    },
    {
        {{
            87345u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 55125u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mythic Battle for Mount Hyjal",
            "Defeat Archimonde in Battle for Mount Hyjal on Mythic difficulty.",
            ""
        }}
    },
    {
        {{
            87346u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3663u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mythic Black Temple",
            "Defeat Illidan Stormrage in Black Temple on Mythic difficulty.",
            ""
        }}
    },
    {
        {{
            87347u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3592u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mythic Sunwell Plateau",
            "Defeat Kil'jaeden in Sunwell Plateau on Mythic difficulty.",
            ""
        }}
    },
    {
        {{
            87348u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 64757u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Ascended Karazhan",
            "Defeat Prince Malchezaar in Karazhan on Ascended difficult.",
            ""
        }}
    },
    {
        {{
            87349u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 28520u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Ascended Gruul's Lair",
            "Defeat Gruul the Dragonslayer in Gruul's Lair on Ascended difficulty.",
            ""
        }}
    },
    {
        {{
            87350u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3640u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Ascended Magtheridon's Lair",
            "Defeat Magtheridon in Magtheridon's Lair on Ascended difficulty.",
            ""
        }}
    },
    {
        {{
            87351u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3658u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Ascended Serpentshrine Cavern",
            "Defeat Lady Vashj in Serpentshrine Cavern on Ascended difficulty.",
            ""
        }}
    },
    {
        {{
            87352u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3664u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Ascended Tempest Keep",
            "Defeat Kael'thas Sunstrider in Tempest Keep at Ascended difficulty.",
            ""
        }}
    },
    {
        {{
            87353u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3630u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Ascended Zul'Aman",
            "Defeat Zul'jin in Zul'Aman on Ascended difficulty.",
            ""
        }}
    },
    {
        {{
            87354u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 55125u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Ascended Battle for Mount Hyjal",
            "Defeat Archimonde in Battle for Mount Hyjal on Ascended difficulty.",
            ""
        }}
    },
    {
        {{
            87355u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3663u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Ascended Black Temple",
            "Defeat Illidan Stormrage in Black Temple on Ascended difficulty.",
            ""
        }}
    },
    {
        {{
            87356u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3592u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Ascended Sunwell Plateau",
            "Defeat Kil'jaeden in Sunwell Plateau on Ascended difficulty.",
            ""
        }}
    },
    {
        {{
            87357u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3649u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Gear Up for Zul'Gurub",
            "Reach an average item level of 45.",
            ""
        }}
    },
    {
        {{
            87358u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 1949u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Gear Up for Mythic Dungeons",
            "Reach an average item level of 52.",
            ""
        }}
    },
    {
        {{
            87359u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 4521u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Gear Up for Molten Core",
            "Reach an average item level of 58.",
            ""
        }}
    },
    {
        {{
            87360u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3820u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Gear Up for Onyxia's Lair",
            "Reach an average item level of 60.",
            ""
        }}
    },
    {
        {{
            87361u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3817u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Gear Up for Blackwing Lair",
            "Reach an average item level of 62.",
            ""
        }}
    },
    {
        {{
            87362u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3681u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Gear Up for Ruins of Ahn'Qiraj",
            "Reach an average item level of 64.",
            ""
        }}
    },
    {
        {{
            87363u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3677u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Gear Up for Temple of Ahn'Qiraj",
            "Reach an average item level of 68.",
            ""
        }}
    },
    {
        {{
            87364u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3259u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Gear Up for Naxxramas",
            "Reach an average item level of 70.",
            ""
        }}
    },
    {
        {{
            87365u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 1949u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Gear Up for Heroic Dungeons (TBC)",
            "Reach an average item level of 76.",
            ""
        }}
    },
    {
        {{
            87366u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 64757u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Gear Up for Karazhan",
            "Reach an average item level of 78.",
            ""
        }}
    },
    {
        {{
            87367u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 28520u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Gear Up for Gruul's Lair",
            "Reach an average item level of 92.",
            ""
        }}
    },
    {
        {{
            87368u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3640u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Gear Up for Magtheridon's Lair",
            "Reach an average item level of 95.",
            ""
        }}
    },
    {
        {{
            87369u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 1948u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Gear Up for Mythic Dungeons (TBC)",
            "Reach an average item level of 100.",
            ""
        }}
    },
    {
        {{
            87370u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3658u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Gear Up for Serpentshrine Caverns",
            "Reach an average item level of 108.",
            ""
        }}
    },
    {
        {{
            87371u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3664u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Gear Up for Tempest Keep",
            "Reach an average item level of 114.",
            ""
        }}
    },
    {
        {{
            87372u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 55125u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Gear Up for Hyjal Summit",
            "Reach an average item level of 116.",
            ""
        }}
    },
    {
        {{
            87373u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3663u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Gear Up for Black Temple",
            "Reach an average item level of 124.",
            ""
        }}
    },
    {
        {{
            87374u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3630u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Gear Up for Zul'Aman",
            "Reach an average item level of 134.",
            ""
        }}
    },
    {
        {{
            87375u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 3592u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Gear Up for Sunwell Plateau",
            "Reach an average item level of 145.",
            ""
        }}
    },
    {
        {{
            87376u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 21548u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Call Board Daily Quests",
            "Complete any Call Board Daily Quest.",
            ""
        }}
    },
    {
        {{
            87377u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 21548u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: PvP Progression Vendor",
            "Visit the PvP Progression Vendor.",
            ""
        }}
    },
    {
        {{
            87378u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 21548u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Purchase Honor Gear",
            "Purchase honor gear from the PvP Progression Vendor.",
            ""
        }}
    },
    {
        {{
            87379u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 21548u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Purchase Honor Gear",
            "Purchase honor gear from the PvP Progression Vendor.",
            ""
        }}
    },
    {
        {{
            87380u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 21548u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: PvP Progression Vendor",
            "Visit the PvP Progression Vendor.",
            ""
        }}
    },
    {
        {{
            87381u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 72997u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Roll Your First Spell",
            "Reach level 20",
            ""
        }}
    },
    {
        {{
            87382u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 72997u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Reach Level 60 in Wildcard!",
            "Reach level 60",
            ""
        }}
    },
    {
        {{
            87383u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 72997u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Reach Level 70 in Wildcard!",
            "Reach level 70",
            ""
        }}
    },
    {
        {{
            87384u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 27391u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Wildcard Prestige",
            "Obtain a Prestigious Pamphet from Chromie and Prestige, resetting to level 1.",
            ""
        }}
    },
    {
        {{
            87385u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 27391u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Featured Builds",
            "Obtain a Prestigious Pamphet from Chromie and Prestige, resetting to level 1.",
            ""
        }}
    },
    {
        {{
            87386u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 27391u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Rapid Rerolling",
            "Obtain a Prestigious Pamphet from Chromie and Prestige, resetting to level 1.",
            ""
        }}
    },
    {
        {{
            87387u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 27391u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Buy Back Scrolls",
            "Obtain a Prestigious Pamphet from Chromie and Prestige, resetting to level 1.",
            ""
        }}
    },
    {
        {{
            87388u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 27391u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Scroll of Fortune",
            "Obtain a Prestigious Pamphet from Chromie and Prestige, resetting to level 1.",
            ""
        }}
    },
    {
        {{
            87389u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22000u, 10u,
            10u, 1024u, 27391u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Deliver Any Store Item",
            "Obtain a Prestigious Pamphet from Chromie and Prestige, resetting to level 1.",
            ""
        }}
    },
    {
        {{
            122009u, 0u, 4294967295u, 0u, 0u, 0u, 15105u, 30u,
            47u, 1024u, 1703u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Capital of the Horde!",
            "Discovered Orgrimmar!",
            ""
        }}
    },
    {
        {{
            122010u, 1u, 4294967295u, 0u, 0u, 0u, 22000u, 30u,
            47u, 1024u, 1704u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Capital of the Alliance!",
            "Discovered Stormwind!",
            ""
        }}
    },
    {
        {{
            212828u, 4294967295u, 4294967295u, 0u, 0u, 0u, 92u, 10u,
            10u, 1024u, 27816u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mythic+ Vendor",
            "Locate and speak with Edrim Skysong",
            ""
        }}
    },
    {
        {{
            212829u, 4294967295u, 4294967295u, 0u, 0u, 0u, 92u, 10u,
            10u, 1024u, 27816u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Into Outland",
            "Arrive at the steps of The Dark Portal.",
            ""
        }}
    },
    {
        {{
            212830u, 4294967295u, 4294967295u, 0u, 0u, 0u, 92u, 10u,
            10u, 1024u, 27816u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Trials & Challenges",
            "Discover what conquering Azeroth really means.",
            ""
        }}
    },
    {
        {{
            212831u, 4294967295u, 4294967295u, 0u, 0u, 0u, 92u, 10u,
            10u, 1024u, 27816u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Take to the skys!",
            "Learn how to fly through Outland.",
            ""
        }}
    },
    {
        {{
            212832u, 4294967295u, 4294967295u, 0u, 0u, 0u, 92u, 10u,
            10u, 1024u, 27816u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Take to the skys!",
            "Learn how to fly through Outland.",
            ""
        }}
    },
    {
        {{
            212833u, 4294967295u, 4294967295u, 0u, 0u, 0u, 92u, 10u,
            10u, 1024u, 27816u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Mystic Enchant Quality",
            "Collect an enchant of each basic quality; rare, epic, legendary & Artifact.",
            ""
        }}
    },
    {
        {{
            212834u, 4294967295u, 4294967295u, 0u, 0u, 0u, 92u, 10u,
            10u, 1024u, 27816u, 0u, 0u, 0u
        }},
        {{
            "Path to Ascension: Fully Loaded",
            "Apply an enchant to every open active enchant slot.",
            ""
        }}
    },
}};

inline constexpr std::array<Record<14, 16>, 189> Criteria = {{
    {
        {{
            15313u, 6312u, 34u, 979602u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 1u
        }},
        {{
            "First book of Ascension",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            20451u, 16719u, 36u, 375250u, 1000u, 0u, 0u, 0u,
            0u, 1u, 0u, 0u, 0u, 1u
        }},
        {{
            "Acquired 1000 Runes of Ascension",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            20452u, 16720u, 36u, 375250u, 5000u, 0u, 0u, 0u,
            0u, 1u, 0u, 0u, 0u, 1u
        }},
        {{
            "Acquired 5000 Runes of Ascension",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            20453u, 16721u, 36u, 375250u, 10000u, 0u, 0u, 0u,
            0u, 1u, 0u, 0u, 0u, 1u
        }},
        {{
            "Acquired 10000 Runes of Ascension",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            20454u, 16722u, 36u, 375250u, 25000u, 0u, 0u, 0u,
            0u, 1u, 0u, 0u, 0u, 1u
        }},
        {{
            "Acquired 25000 Runes of Ascension",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            20455u, 16723u, 36u, 375250u, 50000u, 0u, 0u, 0u,
            0u, 1u, 0u, 0u, 0u, 1u
        }},
        {{
            "Acquired 50000 Runes of Ascension",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            20456u, 16724u, 36u, 375250u, 100000u, 0u, 0u, 0u,
            0u, 1u, 0u, 0u, 0u, 1u
        }},
        {{
            "Acquired 100000 Runes of Ascension",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            29313u, 15312u, 34u, 979602u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 1u
        }},
        {{
            "First book of Ascension in Nightmare Mode.",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41067u, 41067u, 36u, 375250u, 100u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (100)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41068u, 41068u, 36u, 375250u, 500u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (500)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41069u, 41069u, 36u, 375250u, 750u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (750)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41070u, 41070u, 36u, 375250u, 1000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (1000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41071u, 41071u, 36u, 375250u, 2500u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (2500)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41072u, 41072u, 36u, 375250u, 5000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (5000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41073u, 41073u, 36u, 375250u, 7500u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (7500)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41074u, 41074u, 36u, 375250u, 10000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (10000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41075u, 41075u, 36u, 375250u, 12500u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (12500)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41076u, 41076u, 36u, 375250u, 15000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (15000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41077u, 41077u, 36u, 375250u, 17500u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (17500)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41078u, 41078u, 36u, 375250u, 20000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (20000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41079u, 41079u, 36u, 375250u, 22500u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (22500)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41080u, 41080u, 36u, 375250u, 25000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (25000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41081u, 41081u, 36u, 375250u, 27500u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (27500)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41082u, 41082u, 36u, 375250u, 30000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (30000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41083u, 41083u, 36u, 375250u, 32500u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (32500)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41084u, 41084u, 36u, 375250u, 35000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (35000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41085u, 41085u, 36u, 375250u, 37500u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (37500)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41086u, 41086u, 36u, 375250u, 40000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (40000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41087u, 41087u, 36u, 375250u, 42500u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (42500)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41088u, 41088u, 36u, 375250u, 45000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (45000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41089u, 41089u, 36u, 375250u, 50000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (50000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41090u, 41090u, 36u, 375250u, 55000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (55000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41091u, 41091u, 36u, 375250u, 60000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (60000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41092u, 41092u, 36u, 375250u, 65000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (65000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41093u, 41093u, 36u, 375250u, 70000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (70000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41094u, 41094u, 36u, 375250u, 75000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (75000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41095u, 41095u, 36u, 375250u, 80000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (80000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41096u, 41096u, 36u, 375250u, 85000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (85000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41097u, 41097u, 36u, 375250u, 90000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (90000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41098u, 41098u, 36u, 375250u, 95000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (95000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41099u, 41099u, 36u, 375250u, 100000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (100000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41100u, 41100u, 36u, 375250u, 110000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (110000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41101u, 41101u, 36u, 375250u, 120000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (120000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41102u, 41102u, 36u, 375250u, 130000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (130000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41103u, 41103u, 36u, 375250u, 140000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (140000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41104u, 41104u, 36u, 375250u, 150000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (150000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41105u, 41105u, 36u, 375250u, 160000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (160000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41106u, 41106u, 36u, 375250u, 170000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (170000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41107u, 41107u, 36u, 375250u, 180000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (180000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41108u, 41108u, 36u, 375250u, 190000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (190000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41109u, 41109u, 36u, 375250u, 200000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (200000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41110u, 41110u, 36u, 375250u, 210000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (210000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41111u, 41111u, 36u, 375250u, 235000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (235000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41112u, 41112u, 36u, 375250u, 260000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (260000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41113u, 41113u, 36u, 375250u, 285000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (285000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41114u, 41114u, 36u, 375250u, 310000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (310000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41115u, 41115u, 36u, 375250u, 335000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (335000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41116u, 41116u, 36u, 375250u, 360000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (360000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41117u, 41117u, 36u, 375250u, 385000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (385000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41118u, 41118u, 36u, 375250u, 410000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (410000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41119u, 41119u, 36u, 375250u, 435000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (435000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41120u, 41120u, 36u, 375250u, 460000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (460000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41121u, 41121u, 36u, 375250u, 485000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (485000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41122u, 41122u, 36u, 375250u, 500000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (500000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41123u, 41123u, 36u, 375250u, 550000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (550000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41124u, 41124u, 36u, 375250u, 600000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (600000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41125u, 41125u, 36u, 375250u, 650000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (650000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41126u, 41126u, 36u, 375250u, 700000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (700000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41127u, 41127u, 36u, 375250u, 750000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (750000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41128u, 41128u, 36u, 375250u, 800000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (800000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41129u, 41129u, 36u, 375250u, 850000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (850000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41130u, 41130u, 36u, 375250u, 900000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (900000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41131u, 41131u, 36u, 375250u, 950000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (950000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41132u, 41132u, 36u, 375250u, 1000000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (1000000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41140u, 41141u, 36u, 375250u, 1000000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (1000000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41141u, 41142u, 36u, 375250u, 1100000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (1100000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41142u, 41143u, 36u, 375250u, 1200000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (1200000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41143u, 41144u, 36u, 375250u, 1300000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (1300000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41144u, 41145u, 36u, 375250u, 1400000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (1400000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41145u, 41146u, 36u, 375250u, 1500000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (1500000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41146u, 41147u, 36u, 375250u, 1600000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (1600000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41147u, 41148u, 36u, 375250u, 1700000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (1700000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41148u, 41149u, 36u, 375250u, 1800000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (1800000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41149u, 41150u, 36u, 375250u, 1900000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (1900000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41150u, 41151u, 36u, 375250u, 2000000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (2000000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41151u, 41152u, 36u, 375250u, 2100000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (2100000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41152u, 41153u, 36u, 375250u, 2200000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (2200000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41153u, 41154u, 36u, 375250u, 2300000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (2300000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41154u, 41155u, 36u, 375250u, 2400000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (2400000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41155u, 41156u, 36u, 375250u, 2500000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (2500000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41156u, 41157u, 36u, 375250u, 2600000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (2600000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41157u, 41158u, 36u, 375250u, 2700000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (2700000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41158u, 41159u, 36u, 375250u, 2800000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (2800000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41159u, 41160u, 36u, 375250u, 2900000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (2900000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41160u, 41161u, 36u, 375250u, 3000000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (3000000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41161u, 41162u, 36u, 375250u, 3100000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (3100000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41162u, 41163u, 36u, 375250u, 3200000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (3200000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41163u, 41164u, 36u, 375250u, 3300000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (3300000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41164u, 41165u, 36u, 375250u, 3400000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (3400000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41165u, 41166u, 36u, 375250u, 3500000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (3500000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41166u, 41167u, 36u, 375250u, 3600000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (3600000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41167u, 41168u, 36u, 375250u, 3700000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (3700000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41168u, 41169u, 36u, 375250u, 3800000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (3800000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41169u, 41170u, 36u, 375250u, 3900000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (3900000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41170u, 41171u, 36u, 375250u, 4000000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (4000000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41171u, 41172u, 36u, 375250u, 4100000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (4100000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41172u, 41173u, 36u, 375250u, 4200000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (4200000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41173u, 41174u, 36u, 375250u, 4300000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (4300000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41174u, 41175u, 36u, 375250u, 4400000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (4400000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41175u, 41176u, 36u, 375250u, 4500000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (4500000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41176u, 41177u, 36u, 375250u, 4600000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (4600000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41177u, 41178u, 36u, 375250u, 4700000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (4700000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41178u, 41179u, 36u, 375250u, 4800000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (4800000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41179u, 41180u, 36u, 375250u, 4900000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (4900000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41180u, 41181u, 36u, 375250u, 5000000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (5000000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41181u, 41182u, 36u, 375250u, 5500000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (5500000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41182u, 41183u, 36u, 375250u, 6000000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (6000000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41183u, 41184u, 36u, 375250u, 6500000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (6500000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41184u, 41185u, 36u, 375250u, 7000000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (7000000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41185u, 41186u, 36u, 375250u, 7500000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (7500000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41186u, 41187u, 36u, 375250u, 8000000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (8000000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41187u, 41188u, 36u, 375250u, 8500000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (8500000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41188u, 41189u, 36u, 375250u, 9000000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (9000000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41189u, 41190u, 36u, 375250u, 9500000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (9500000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            41190u, 41191u, 36u, 375250u, 10000000u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 1u
        }},
        {{
            "Runes of Ascension (1000000)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87293u, 87293u, 0u, 14834u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Normal: Hakkar",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87294u, 87294u, 0u, 11502u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Normal: Ragnaros",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87295u, 87295u, 0u, 10184u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Normal: Onyxia (Vanilla)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87296u, 87296u, 0u, 11583u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Normal: Nefarian",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87297u, 87297u, 0u, 15339u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Normal: Ossirian the Unscarred",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87298u, 87298u, 0u, 15727u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Normal: C'thun",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87299u, 87299u, 0u, 15990u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "PTA Path to Ascension: Normal: Kel'thuzad",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87300u, 87300u, 0u, 14834u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Heroic: Hakkar",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87301u, 87301u, 0u, 11502u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Heroic: Ragnaros",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87302u, 87302u, 0u, 10184u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Heroic: Onyxia (Vanilla)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87303u, 87303u, 0u, 11583u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Heroic: Nefarian",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87304u, 87304u, 0u, 15339u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Heroic: Ossirian the Unscarred",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87305u, 87305u, 0u, 15727u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Heroic: C'thun",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87306u, 87306u, 0u, 15990u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "PTA Path to Ascension: Heroic: Kel'thuzad",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87307u, 87307u, 0u, 14834u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Mythic: Hakkar",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87308u, 87308u, 0u, 11502u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Mythic: Ragnaros",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87309u, 87309u, 0u, 10184u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Mythic: Onyxia (Vanilla)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87310u, 87310u, 0u, 11583u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Mythic: Nefarian",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87311u, 87311u, 0u, 15339u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Mythic: Ossirian the Unscarred",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87312u, 87312u, 0u, 15727u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Mythic: C'thun",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87313u, 87313u, 0u, 15990u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "PTA Path to Ascension: Mythic: Kel'thuzad",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87314u, 87314u, 0u, 14834u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Ascended: Hakkar",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87315u, 87315u, 0u, 11502u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Ascended: Ragnaros",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87316u, 87316u, 0u, 10184u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Ascended: Onyxia (Vanilla)",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87317u, 87317u, 0u, 11583u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Ascended: Nefarian",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87318u, 87318u, 0u, 15339u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Ascended: Ossirian the Unscarred",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87319u, 87319u, 0u, 15727u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Ascended: C'thun",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87320u, 87320u, 0u, 15990u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "PTA Path to Ascension: Ascended: Kel'thuzad",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87321u, 87321u, 0u, 15690u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Normal: Prince Malchezaar",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87322u, 87322u, 0u, 19044u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Normal: Gruul the Dragonkiller",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87323u, 87323u, 0u, 17257u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Normal: Magtheridon",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87324u, 87324u, 0u, 21212u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Normal: Lady Vashj",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87325u, 87325u, 0u, 19622u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Normal: Kael'thas Sunstrider",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87326u, 87326u, 0u, 23863u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Normal: Zul'Jin",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87327u, 87327u, 0u, 17968u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Normal: Archimonde",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87328u, 87328u, 0u, 22917u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Normal: Illidan Stormrage",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87329u, 87329u, 0u, 25315u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Normal: Kil'Jaeden",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87330u, 87330u, 0u, 15690u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Heroic: Prince Malchezaar",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87331u, 87331u, 0u, 19044u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Heroic: Gruul the Dragonkiller",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87332u, 87332u, 0u, 17257u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Heroic: Magtheridon",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87333u, 87333u, 0u, 21212u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Heroic: Lady Vashj",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87334u, 87334u, 0u, 19622u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Heroic: Kael'thas Sunstrider",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87335u, 87335u, 0u, 23863u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Heroic: Zul'Jin",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87336u, 87336u, 0u, 17968u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Heroic: Archimonde",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87337u, 87337u, 0u, 22917u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Heroic: Illidan Stormrage",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87338u, 87338u, 0u, 25315u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Heroic: Kil'Jaeden",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87339u, 87339u, 0u, 15690u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Mythic: Prince Malchezaar",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87340u, 87340u, 0u, 19044u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Mythic: Gruul the Dragonkiller",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87341u, 87341u, 0u, 17257u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Mythic: Magtheridon",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87342u, 87342u, 0u, 21212u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Mythic: Lady Vashj",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87343u, 87343u, 0u, 19622u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Mythic: Kael'thas Sunstrider",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87344u, 87344u, 0u, 23863u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Mythic: Zul'Jin",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87345u, 87345u, 0u, 17968u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Mythic: Archimonde",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87346u, 87346u, 0u, 22917u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Mythic: Illidan Stormrage",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87347u, 87347u, 0u, 25315u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Mythic: Kil'Jaeden",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87348u, 87348u, 0u, 15690u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Ascended: Prince Malchezaar",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87349u, 87349u, 0u, 19044u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Ascended: Gruul the Dragonkiller",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87350u, 87350u, 0u, 17257u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Ascended: Magtheridon",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87351u, 87351u, 0u, 21212u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Ascended: Lady Vashj",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87352u, 87352u, 0u, 19622u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Ascended: Kael'thas Sunstrider",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87353u, 87353u, 0u, 23863u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Ascended: Zul'Jin",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87354u, 87354u, 0u, 17968u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Ascended: Archimonde",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87355u, 87355u, 0u, 22917u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Ascended: Illidan Stormrage",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            87356u, 87356u, 0u, 25315u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 6u
        }},
        {{
            "Path to Ascension: Ascended: Kil'Jaeden",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
}};

inline constexpr std::array<Record<4, 16>, 16> Categories = {{
    {
        {{
            2000u, 168u, 16712190u, 5u
        }},
        {{
            "Ascension Dungeons",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            15103u, 128u, 16712190u, 4u
        }},
        {{
            "Ascension Season 1",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            15114u, 128u, 16712190u, 2u
        }},
        {{
            "Ascension Season 2",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            15124u, 128u, 16712190u, 4u
        }},
        {{
            "Ascension Season 3",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            15129u, 122u, 16712190u, 5u
        }},
        {{
            "Ascension Season 3",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            15130u, 21u, 16712190u, 4u
        }},
        {{
            "Ascension Season 3",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            15132u, 1u, 16712190u, 14u
        }},
        {{
            "Ascension",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            15134u, 128u, 16712190u, 4u
        }},
        {{
            "Ascension Season 4",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            15139u, 122u, 16712190u, 5u
        }},
        {{
            "Ascension Season 4",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            15140u, 21u, 16712190u, 4u
        }},
        {{
            "Ascension Season 4",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            16124u, 128u, 16712190u, 4u
        }},
        {{
            "Ascension Season 5",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            16129u, 122u, 16712190u, 5u
        }},
        {{
            "Ascension Season 5",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            16130u, 21u, 16712190u, 4u
        }},
        {{
            "Ascension Season 5",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            16135u, 4294967295u, 16712190u, 8u
        }},
        {{
            "Ascension Seasonal",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            21000u, 4294967295u, 16712190u, 30u
        }},
        {{
            "Runes of Ascension",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
    {
        {{
            22000u, 4294967295u, 16712190u, 30u
        }},
        {{
            "Path to Ascension",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }}
    },
}};

}

#endif
