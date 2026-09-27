/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
#ifndef ASCENSION_PATCH_SPELL_DATA_H
#define ASCENSION_PATCH_SPELL_DATA_H

#include "Define.h"
#include <array>
#include <string_view>

namespace AscensionPatchSpellData
{
inline constexpr std::string_view SourceSha256 =
    "6a305f94f459332273aa4e0d684b0d71a13543f99ee61ddb941adb9922493921";

struct Record
{
    std::array<uint32, 170> Fields;
    std::array<std::string_view, 4> Strings;
};

inline constexpr std::array<Record, 2> Records = {{
    {
        {{
            560384u, 0u, 0u, 0u, 256u, 1024u, 4u, 0u,
            128u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 1u, 0u, 0u, 0u,
            0u, 0u, 0u, 101u, 0u, 0u, 0u, 0u,
            28u, 0u, 0u, 0u, 0u, 0u, 1u, 0u,
            0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 4294967295u, 0u, 0u, 6u,
            0u, 0u, 1u, 1u, 1u, 0u, 0u, 0u,
            4294967295u, 4294967295u, 4294967295u, 0u, 0u, 0u, 1u, 0u,
            0u, 0u, 0u, 0u, 0u, 0u, 0u, 23u,
            0u, 0u, 500u, 0u, 0u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 560385u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 24832u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
            28u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
            1065353216u, 1065353216u, 1065353216u, 0u, 0u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 0u, 0u, 0u,
            0u, 0u,
        }},
        {{
            "Rippling Renewal",
            "",
            "",
            "",
        }}
    },
    {
        {{
            560394u, 0u, 0u, 19u, 0u, 1024u, 4u, 268435712u,
            0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 1u, 0u, 0u, 1u,
            0u, 0u, 0u, 101u, 0u, 0u, 0u, 0u,
            28u, 0u, 0u, 0u, 0u, 0u, 1u, 0u,
            0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 4294967295u, 0u, 0u, 6u,
            0u, 0u, 1u, 1u, 1u, 1083179008u, 0u, 0u,
            149u, 4294967295u, 4294967295u, 0u, 0u, 0u, 22u, 0u,
            0u, 30u, 0u, 0u, 18u, 0u, 0u, 69u,
            0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 0u, 127u, 0u,
            0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 5650u, 0u, 0u,
            0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
            28u, 0u, 0u, 0u, 20u, 1u, 0u, 0u,
            1065353216u, 1065353216u, 1065353216u, 0u, 0u, 0u, 0u, 0u,
            0u, 2u, 0u, 0u, 0u, 0u, 0u, 0u,
            0u, 0u,
        }},
        {{
            "Rippling Protection",
            "",
            "",
            "Absorbing damage.",
        }}
    },
}};
}

#endif
