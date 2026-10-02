-- Father Harnos (161844) was missing its creature_questender link for
-- Oracular Idol (1660036), so the quest could be started (via item 559159's
-- startquest) but never turned in.
DELETE FROM `creature_questender` WHERE `id`=161844 AND `quest`=1660036;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES (161844, 1660036);
