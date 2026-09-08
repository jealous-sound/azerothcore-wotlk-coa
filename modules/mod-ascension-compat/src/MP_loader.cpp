/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "AscensionReaperDirge.h"
#include "AscensionTinkerOverload.h"
#include "AscensionRunemasterGlyphs.h"
#include "AscensionManastorm.h"
#include "AscensionRunemasterBrand.h"
#include "AscensionRunemasterScaling.h"
#include "AscensionTinkerRockadier.h"
#include "AscensionRunemasterZenith.h"
#include "AscensionRunemasterEchoes.h"
#include "AscensionVenomancerVenoms.h"
#include "AscensionTinkerCombatSymbiosis.h"

void AddAscensionCompatScripts();
void AddAscensionBarbarianCompletionScripts();
void AddAscensionBarbarianEventScripts();
void AddAscensionBarbarianAbilityScripts();
void AddAscensionGuardianStandardScripts();
void AddAscensionGuardianFavorScripts();
void AddAscensionGuardianEventScripts();
void AddAscensionGuardianCompletionScripts();
void AddAscensionGuardianAbilityScripts();
void AddAscensionGuardianTalentScripts();
void AddAscensionGuardianDrumScripts();
void AddAscensionRangerDamageScripts();
void AddAscensionRangerScalingScripts();
void AddAscensionRangerAssaultScripts();
void AddAscensionWitchHunterTonicScripts();
void AddAscensionWitchHunterTonicTalentScripts();
void AddAscensionWitchHunterTorchScripts();
void AddAscensionWitchHunterFlameScripts();
void AddAscensionWitchHunterTorchlightScripts();
void AddAscensionWitchHunterStakeScripts();
void AddAscensionWitchHunterCompletionScripts();
void AddAscensionWitchHunterEventScripts();
void AddAscensionWitchHunterAbilityScripts();
void AddAscensionWitchHunterDefenseScripts();
void AddAscensionWitchHunterSummonScripts();
void AddAscensionWitchDoctorCompletionScripts();
void AddAscensionWitchDoctorEventScripts();
void AddAscensionWitchDoctorAbilityScripts();
void AddAscensionWitchDoctorBrewingScripts();
void AddAscensionWitchDoctorAuraScripts();
void AddAscensionWitchDoctorSummonScripts();
void AddAscensionTinkerAugmentationScripts();
void AddAscensionTinkerAugmentationTalentScripts();
void AddAscensionTinkerHackingScripts();
void AddSC_AscensionPrimalistEarthshaping();
void AddSC_AscensionVenomancerCatalyst();
void AddSC_AscensionPrimalistSpiritBeast();

// Add all
// cf. the naming convention https://github.com/azerothcore/azerothcore-wotlk/blob/master/doc/changelog/master.md#how-to-upgrade-4
// additionally replace all '-' in the module folder name with '_' here
void Addmod_ascension_compatScripts()
{
    AddAscensionManastormScripts();
    AddSC_AscensionReaperDirge();
    AddAscensionTinkerOverloadScripts();
    AddAscensionCompatScripts();
    AddAscensionBarbarianCompletionScripts();
    AddAscensionBarbarianEventScripts();
    AddAscensionBarbarianAbilityScripts();
    AddAscensionGuardianStandardScripts();
    AddAscensionGuardianFavorScripts();
    AddAscensionGuardianEventScripts();
    AddAscensionGuardianCompletionScripts();
    AddAscensionGuardianAbilityScripts();
    AddAscensionGuardianTalentScripts();
    AddAscensionGuardianDrumScripts();
    AddAscensionRangerDamageScripts();
    AddAscensionRangerScalingScripts();
    AddAscensionRangerAssaultScripts();
    AddAscensionWitchHunterTonicScripts();
    AddAscensionWitchHunterTonicTalentScripts();
    AddAscensionWitchHunterTorchScripts();
    AddAscensionWitchHunterFlameScripts();
    AddAscensionWitchHunterTorchlightScripts();
    AddAscensionWitchHunterStakeScripts();
    AddAscensionWitchHunterCompletionScripts();
    AddAscensionWitchHunterEventScripts();
    AddAscensionWitchHunterAbilityScripts();
    AddAscensionWitchHunterDefenseScripts();
    AddAscensionWitchHunterSummonScripts();
    AddAscensionWitchDoctorCompletionScripts();
    AddAscensionWitchDoctorEventScripts();
    AddAscensionWitchDoctorAbilityScripts();
    AddAscensionWitchDoctorBrewingScripts();
    AddAscensionWitchDoctorAuraScripts();
    AddAscensionWitchDoctorSummonScripts();
    AddAscensionTinkerAugmentationScripts();
    AddAscensionTinkerAugmentationTalentScripts();
    AddAscensionTinkerHackingScripts();
    AddSC_AscensionPrimalistEarthshaping();
    AddAscensionRunemasterGlyphScripts();
    AddSC_AscensionVenomancerCatalyst();
    AddAscensionRunemasterBrandScripts();
    AddAscensionRunemasterScalingScripts();
    AddAscensionTinkerRockadierScripts();
    AddSC_AscensionPrimalistSpiritBeast();
    AddAscensionRunemasterZenithScripts();
    AddAscensionRunemasterEchoesScripts();
    AddAscensionVenomancerVenomScripts();
    AddAscensionTinkerCombatSymbiosisScripts();
}
