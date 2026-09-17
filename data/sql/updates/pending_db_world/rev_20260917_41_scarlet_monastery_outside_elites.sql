-- Scarlet Scout (4281), Scarlet Preserver (4280) and Scarlet Sentry (4283) are elite on Ascension
-- (db.exil.es export of 2026-09-13: rank 1; AzerothCore data: rank 0).
UPDATE `creature_template` SET `rank` = 1 WHERE `entry` IN (4280, 4281, 4283);
