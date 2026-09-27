--
CREATE TABLE IF NOT EXISTS `patch_spell` (
    `spell_id` INT UNSIGNED NOT NULL,
    `enabled` TINYINT UNSIGNED NOT NULL DEFAULT 1,
    `name` TEXT NULL,
    `rank` TEXT NULL,
    `description` TEXT NULL,
    `aura_description` TEXT NULL,
    `comment` VARCHAR(255) NOT NULL DEFAULT '',
    PRIMARY KEY (`spell_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

DELETE FROM `patch_spell` WHERE `spell_id` IN (560384, 560394);
INSERT INTO `patch_spell` (`spell_id`, `aura_description`, `comment`) VALUES
(560384, 'Healing for ${$w1} every 0.5 sec while Ripple is channeled.', 'Rippling Renewal: show healing per pulse'),
(560394, 'Absorbing up to ${$w1} damage while Ripple is channeled.', 'Rippling Protection: show remaining absorb');
