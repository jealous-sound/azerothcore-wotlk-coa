-- Sulfuron Harbinger's three named disciples (92031-92033), Mythic/Ascended only.
--
-- The 55-pull Mythic/Ascended log corpus for Sulfuron always shows the same four adds next to
-- him: one Corvus the Nimble (11662, the stock Flamewaker Priest, renamed on CoA by
-- rev_20260930_84) plus Cull the Destroyer (92031), Proxima the Opressor (92032) and Ebon the
-- Cruel (92033), one of each -- never a second Corvus alongside the three named ones, and never
-- more than four total. The base game spawns four Flamewaker Priests around Sulfuron; on
-- Mythic/Ascended three of those four spots are the disciples instead, not three extra adds on
-- top of all four Corvus. No Normal/Heroic Sulfuron pull exists in this corpus at all, so
-- Corvus's own four spawns are left exactly as they are on every difficulty -- this migration
-- adds nothing below Mythic and does not touch Corvus's creature_template row or spawns.
--
-- Each disciple's sub_name names the MC boss it apes (exiles-db export, 2026-09-13):
-- Cull the Destroyer -> "Disciple to Gehennas", Proxima the Opressor -> "Disciple to Shazzrah",
-- Ebon the Cruel -> "Disciple to Lucifron". Display id 12030 is the plain Flamewaker model (the
-- same one creature 11661 "Flamewaker" uses) -- the export contradicts the "Flamewaker Elite"
-- skin recollection; reported, not silently substituted.
--
-- Health: video reading is 6.5M at 23 players Ascended for all three, identical to Corvus's own
-- reading (hp/hp-pools.md) -- so this migration mirrors Corvus's own already-flexed coa_boss_flex
-- shape (rev_20260930_94) verbatim for all three. d0/d1 (Normal/Heroic) are never read at
-- runtime since these entries only spawn on Mythic/Ascended, but the table needs a value in
-- every column; carrying Corvus's own numbers there is the least invented choice, not a claim
-- that a Normal/Heroic reading exists.
--
-- Template columns otherwise copy Corvus/Flamewaker Priest's own row (creature_template.sql,
-- entry 11662) -- same archetype, same room -- except name/subname/spells/AIName/ScriptName.
-- HealthModifier is left at the neutral 1 used by every other flexed MC add (Shadow of
-- Lucifron, rev_20260930_92): actual health comes from coa_boss_flex, not this column.
-- rank stays 1 (Corvus's own), not 3: nothing here justifies a boss-tier CC-immunity change
-- for what is, mechanically, another Flamewaker Priest reskin.

INSERT INTO `creature_template`
  (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `name`,
     `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`,
     `speed_run`, `speed_swim`, `speed_flight`, `detection_range`, `rank`, `dmgschool`, `DamageModifier`,
     `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`,
     `dynamicflags`, `family`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `PetSpellDataId`,
     `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `HoverHeight`, `HealthModifier`, `ManaModifier`,
     `ArmorModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `CreatureImmunitiesId`,
     `flags_extra`, `ScriptName`)
VALUES
(92031, 0, 0, 0, 0, 0, 'Cull the Destroyer', 'Disciple to Gehennas', NULL, 0, 62, 62, 0, 54, 0, 1, 1.71429, 1, 1, 20,
 1, 0, 13, 2000, 2000, 1, 1, 8, 64, 2048, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, '', 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0,
 'npc_cull_the_destroyer_coa'),
(92032, 0, 0, 0, 0, 0, 'Proxima the Opressor', 'Disciple to Shazzrah', NULL, 0, 62, 62, 0, 54, 0, 1, 1.71429, 1, 1,
 20, 1, 0, 13, 2000, 2000, 1, 1, 8, 64, 2048, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, '', 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0,
 'npc_proxima_the_opressor_coa'),
(92033, 0, 0, 0, 0, 0, 'Ebon the Cruel', 'Disciple to Lucifron', NULL, 0, 62, 62, 0, 54, 0, 1, 1.71429, 1, 1, 20, 1,
 0, 13, 2000, 2000, 1, 1, 8, 64, 2048, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, '', 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0,
 'npc_ebon_the_cruel_coa')
ON DUPLICATE KEY UPDATE
  `difficulty_entry_1` = VALUES(`difficulty_entry_1`), `difficulty_entry_2` = VALUES(`difficulty_entry_2`),
  `difficulty_entry_3` = VALUES(`difficulty_entry_3`), `KillCredit1` = VALUES(`KillCredit1`),
  `KillCredit2` = VALUES(`KillCredit2`), `name` = VALUES(`name`), `subname` = VALUES(`subname`),
  `IconName` = VALUES(`IconName`), `gossip_menu_id` = VALUES(`gossip_menu_id`), `minlevel` = VALUES(`minlevel`),
  `maxlevel` = VALUES(`maxlevel`), `exp` = VALUES(`exp`), `faction` = VALUES(`faction`),
  `npcflag` = VALUES(`npcflag`), `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`),
  `speed_swim` = VALUES(`speed_swim`), `speed_flight` = VALUES(`speed_flight`),
  `detection_range` = VALUES(`detection_range`), `rank` = VALUES(`rank`), `dmgschool` = VALUES(`dmgschool`),
  `DamageModifier` = VALUES(`DamageModifier`), `BaseAttackTime` = VALUES(`BaseAttackTime`),
  `RangeAttackTime` = VALUES(`RangeAttackTime`), `BaseVariance` = VALUES(`BaseVariance`),
  `RangeVariance` = VALUES(`RangeVariance`), `unit_class` = VALUES(`unit_class`),
  `unit_flags` = VALUES(`unit_flags`), `unit_flags2` = VALUES(`unit_flags2`),
  `dynamicflags` = VALUES(`dynamicflags`), `family` = VALUES(`family`), `type` = VALUES(`type`),
  `type_flags` = VALUES(`type_flags`), `lootid` = VALUES(`lootid`), `pickpocketloot` = VALUES(`pickpocketloot`),
  `skinloot` = VALUES(`skinloot`), `PetSpellDataId` = VALUES(`PetSpellDataId`), `VehicleId` = VALUES(`VehicleId`),
  `mingold` = VALUES(`mingold`), `maxgold` = VALUES(`maxgold`), `AIName` = VALUES(`AIName`),
  `MovementType` = VALUES(`MovementType`), `HoverHeight` = VALUES(`HoverHeight`),
  `HealthModifier` = VALUES(`HealthModifier`), `ManaModifier` = VALUES(`ManaModifier`),
  `ArmorModifier` = VALUES(`ArmorModifier`), `ExperienceModifier` = VALUES(`ExperienceModifier`),
  `RacialLeader` = VALUES(`RacialLeader`), `movementId` = VALUES(`movementId`),
  `RegenHealth` = VALUES(`RegenHealth`), `CreatureImmunitiesId` = VALUES(`CreatureImmunitiesId`),
  `flags_extra` = VALUES(`flags_extra`), `ScriptName` = VALUES(`ScriptName`);

-- Model: export display_id 12030, plain Flamewaker (not Flamewaker Elite's 12002).
DELETE FROM `creature_template_model` WHERE `CreatureID` IN (92031, 92032, 92033);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) VALUES
(92031, 0, 12030, 1, 1),
(92032, 0, 12030, 1, 1),
(92033, 0, 12030, 1, 1);

-- Health: [measured] video reading identical to Corvus's own (6.5M/23 players, Ascended) --
-- mirrors Corvus's own coa_boss_flex row (rev_20260930_94) verbatim; see the header comment.
DELETE FROM `coa_boss_flex` WHERE `entry` IN (92031, 92032, 92033);
INSERT INTO `coa_boss_flex` (`entry`, `hp_d0`, `hp_d1`, `hp_d2`, `hp_d3`, `comment`) VALUES
(92031, 121814, 175412, 229010, 282609, 'Cull the Destroyer: CoA video Ascended (6.5M/23), same reading as Corvus; Mythic/Ascended only'),
(92032, 121814, 175412, 229010, 282609, 'Proxima the Opressor: CoA video Ascended (6.5M/23), same reading as Corvus; Mythic/Ascended only'),
(92033, 121814, 175412, 229010, 282609, 'Ebon the Cruel: CoA video Ascended (6.5M/23), same reading as Corvus; Mythic/Ascended only');

-- coa_boss_summon (rev_20260930_92) already carries idx/replace_entry/replace_radius/
-- min_difficulty in its final schema, so this revision only inserts Sulfuron's own rows.
--
-- Sulfuron (12098): the three disciples, Mythic/Ascended only (min_difficulty 2) -- no
-- Normal/Heroic Sulfuron pull exists in the corpus, so Normal/Heroic keep their four Corvus
-- spawns exactly as they are; only this row's own difficulty check changes anything. Each
-- disciple replaces the nearest still-alive Flamewaker Priest/Corvus the Nimble (11662) within
-- the priest cluster's own spread (creature.sql spawns run x594-613/y-1177..-1179 around
-- Sulfuron's own x=601/y=-1179, export creature_spawn) -- 25yd comfortably covers that without
-- reaching into neighbouring rooms. No buff spell: unlike Lucifron's Shadow, nothing in the kit
-- evidence names one. No delay evidence exists either; a small 500ms stagger avoids three
-- simultaneous despawn/summon pairs on the same tick.
DELETE FROM `coa_boss_summon` WHERE `entry` = 12098;
INSERT INTO `coa_boss_summon` (`entry`, `idx`, `summon_entry`, `summon_delay_ms`, `summon_buff_spell`,
    `replace_entry`, `replace_radius`, `min_difficulty`, `comment`) VALUES
(12098, 1, 92031, 500, 0, 11662, 25, 2, 'Sulfuron: Cull the Destroyer replaces a Flamewaker Priest'),
(12098, 2, 92032, 1000, 0, 11662, 25, 2, 'Sulfuron: Proxima the Opressor replaces a Flamewaker Priest'),
(12098, 3, 92033, 1500, 0, 11662, 25, 2, 'Sulfuron: Ebon the Cruel replaces a Flamewaker Priest');
