-- Seven Years of Bad Luck (1660057) was missing full reward/text data.
-- RequiredNpcOrGo1 (-162920, Mirror Shard) and giver/ender (162802, Aldia
-- Crayon) were already correct; the user's source data listed it as a
-- positive 162920 but that's inconsistent with it being a gameobject in
-- our DB (negative = GO, per the established convention) -- kept as-is.
UPDATE `quest_template` SET
  `RewardMoney`=350, `RewardMoneyDifficulty`=382, `Flags`=8,
  `RewardChoiceItemID1`=2302030, `RewardChoiceItemQuantity1`=1,
  `RewardChoiceItemID2`=2302035, `RewardChoiceItemQuantity2`=1,
  `RewardChoiceItemID3`=2302040, `RewardChoiceItemQuantity3`=1,
  `RewardFactionID1`=72, `RewardFactionValue1`=5,
  `LogDescription`='Inspect the broken mirror shards that Aldia Crayon blames for the curse afflicting Lady Agria Spada.',
  `QuestDescription`='Hm...$b$b<The majordomo gives you a long, measuring look.>$b$bBefore you go... I need your help with one more matter.$b$bI\'ve had the sense for some time that my lady\'s ailments aren\'t physical, but magical. A curse.$b$bIt started with a broken mirror. And you know what they say. However thorough I\'ve been, shards keep turning up; bits of glass tucked around the manor and the grounds.$b$bWould you kindly deal with them?',
  `QuestCompletionLog`='Return to the butler.',
  `ObjectiveText1`='Mirror Shard Inspected'
WHERE `ID`=1660057;
