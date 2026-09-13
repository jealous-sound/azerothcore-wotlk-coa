-- Keep the Sentry's siege cannon model at a suitable size for a player summon.
UPDATE `creature_template_model` SET `DisplayScale` = 0.35 WHERE `CreatureID` = 50046 AND `CreatureDisplayID` = 28526;
