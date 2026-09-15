-- Handle the voucher once instead of casting both charge-consuming template spells.
UPDATE `item_template` SET `ScriptName` = 'item_ascension_bank_voucher' WHERE `entry` = 102130;
