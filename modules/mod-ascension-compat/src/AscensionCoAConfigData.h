/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */
// The realm-wide configuration set the client's Extensions.dll keeps from SMSG_COA_CONFIG.
#ifndef ASCENSION_COA_CONFIG_DATA_H
#define ASCENSION_COA_CONFIG_DATA_H

#include <cstdint>
#include <vector>

namespace AscensionCompatData
{
// One scalar SMSG_COA_CONFIG entry. Values are the live realm's, from the captured login burst;
// the wire writes them as u32, u8 and f32 per section.
struct CoAConfigScalar
{
    char const* Key;
    std::uint32_t Value;
};

struct CoAConfigFloat
{
    char const* Key;
    float Value;
};

struct CoAConfigIntVector
{
    char const* Key;
    std::vector<std::uint32_t> Values;
};

struct CoAConfigFloatVector
{
    char const* Key;
    std::vector<float> Values;
};

// Wire order: the u32, u8 and f32 configs, the rates, then the int- and float-vector tables.
extern std::vector<CoAConfigScalar> CoAConfigInts;
extern std::vector<CoAConfigScalar> CoAConfigBools;
extern std::vector<CoAConfigFloat> CoAConfigFloats;
extern std::vector<CoAConfigFloat> CoAConfigRates;
extern std::vector<CoAConfigIntVector> CoAConfigIntVectors;
extern std::vector<CoAConfigFloatVector> CoAConfigFloatVectors;
}

#endif
