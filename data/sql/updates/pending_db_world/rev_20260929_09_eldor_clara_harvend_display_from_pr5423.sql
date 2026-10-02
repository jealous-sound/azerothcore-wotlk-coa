-- Eldor Hammer (162801), Clara the Mad (162805), and Harvend Thorm (162807)
-- were on known-safe stock fallback displays (1292/120/1923, see
-- coa_codebase_gotchas). Matched to PR #5423's displays for each (their
-- only model each, no alternates).
-- Quest links for 1660058 (Worm-Eaten Apple, giver/ender Clara the Mad) and
-- 1660059 (Goldshire's Generosity, ender Eldor Hammer, giver Harvend Thorm)
-- already matched PR #5423 exactly on our side -- no quest-side change needed.
UPDATE `creature_template_model` SET `CreatureDisplayID`=3553 WHERE `CreatureID`=162801;
UPDATE `creature_template_model` SET `CreatureDisplayID`=18618 WHERE `CreatureID`=162805;
UPDATE `creature_template_model` SET `CreatureDisplayID`=15766 WHERE `CreatureID`=162807;
