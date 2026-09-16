-- Quest 287 "Frostmane Hold" grants its "Fully explore Frostmane Hold" objective through area trigger 169
-- (`areatrigger_involvedrelation` maps 169 -> 287, and `quest_template_addon`.`SpecialFlags` = 2).
-- The CoA client ships a widened trigger: AreaTrigger.dbc in Data/patch-M.MPQ gives entry 169 a radius of
-- 100, while the four stock Blizzard copies of that file and this world's `areatrigger` row both still say 8.
-- The client sends CMSG_AREATRIGGER once, when it crosses into its own 100 yard sphere, and
-- `Player::IsInAreaTriggerRadius` rejects that packet against the 8 yard server radius before
-- `HandleAreaTriggerOpcode` can reach `AreaExploredOrEventHappens`. The client does not resend while the
-- player stays inside the sphere, so walking all the way to the cave never grants credit. Match the server
-- volume to the shipped client. Centre (-5586.46, 679.298, 384.955) is unchanged.
UPDATE `areatrigger` SET `radius` = 100 WHERE `entry` = 169;
