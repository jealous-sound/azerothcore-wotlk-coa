-- Widen quest reward quantity columns for harvested Ascension quest data.
--
-- Stock AzerothCore stores reward quantities as smallint unsigned, capping them
-- at 65,535. Of the 18,550 quests harvested out of the client's questcache, one
-- carries a RewardAmount of 100,000, so the live realm's schema is wider than
-- the stock one. Clamping it would silently record the wrong reward.
--
-- Only one row overflows today, but RewardAmount2-4 and the RewardChoiceItem
-- quantities hold exactly the same kind of value, so they are widened together
-- rather than left to fail on the next harvest.
--
-- Safe against the core: ObjectMgr::LoadQuests reads these via
-- questRecord[N].Get<uint32>() into uint32 members (RewardItemIdCount and
-- RewardChoiceItemCount are uint32 arrays in QuestDef.h), so the column was
-- already narrower than the field it feeds.

ALTER TABLE `quest_template`
    MODIFY COLUMN `RewardAmount1` INT UNSIGNED NOT NULL DEFAULT 0,
    MODIFY COLUMN `RewardAmount2` INT UNSIGNED NOT NULL DEFAULT 0,
    MODIFY COLUMN `RewardAmount3` INT UNSIGNED NOT NULL DEFAULT 0,
    MODIFY COLUMN `RewardAmount4` INT UNSIGNED NOT NULL DEFAULT 0,
    MODIFY COLUMN `RewardChoiceItemQuantity1` INT UNSIGNED NOT NULL DEFAULT 0,
    MODIFY COLUMN `RewardChoiceItemQuantity2` INT UNSIGNED NOT NULL DEFAULT 0,
    MODIFY COLUMN `RewardChoiceItemQuantity3` INT UNSIGNED NOT NULL DEFAULT 0,
    MODIFY COLUMN `RewardChoiceItemQuantity4` INT UNSIGNED NOT NULL DEFAULT 0,
    MODIFY COLUMN `RewardChoiceItemQuantity5` INT UNSIGNED NOT NULL DEFAULT 0,
    MODIFY COLUMN `RewardChoiceItemQuantity6` INT UNSIGNED NOT NULL DEFAULT 0;
