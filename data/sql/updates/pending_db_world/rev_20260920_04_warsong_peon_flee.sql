-- CoA/Ascension: fix Warsong Peon (11656) instant flee on aggro.
-- Issue #4364: The SAI entry uses SMART_EVENT_AGGRO (4) which fires immediately on
-- combat enter, causing the peon to flee the moment it is attacked. On CoA the peon
-- should only flee at low health. Changed to SMART_EVENT_HEALTH_PCT (2) at 0-20%.
-- Source: smart_scripts base data analysis, issue #4364

UPDATE `smart_scripts` SET `event_type` = 2, `event_param1` = 0, `event_param2` = 20, `event_param3` = 0, `event_param4` = 0, `comment` = 'Warsong Peon - On Low Health (20%) - Flee For Assist' WHERE `entryorguid` = 11656 AND `source_type` = 0 AND `id` = 0;
