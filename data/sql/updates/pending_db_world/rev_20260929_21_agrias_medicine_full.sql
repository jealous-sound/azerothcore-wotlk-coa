-- Agria's Medicine (1660056) was missing most of its data (only
-- RequiredItemId1 was set, no start item, no reward choices, no full
-- text). Full user-provided quest data applied: buy 5 items at the
-- Goldshire market (Elgris Blossom Petals/Dun Kazad Liquor Concentrate/
-- Pumpkin Juice/Murloc Eyeball/the quest's own start item 558960), reward
-- choice between two potion items, turned in to Aldia Crayon (162802) at
-- the Spada manor -- not Aldia (162815), who remains the giver.
UPDATE `quest_template` SET
  `RewardMoney`=110, `RewardMoneyDifficulty`=120, `Flags`=8, `StartItem`=558960,
  `RewardChoiceItemID1`=2302015, `RewardChoiceItemQuantity1`=1,
  `RewardChoiceItemID2`=2302020, `RewardChoiceItemQuantity2`=1,
  `RewardFactionID1`=72, `RewardFactionValue1`=5,
  `LogDescription`='Buy the potion\'s ingredients at the Goldshire market: Elgris Blossom Petals, Dun Kazad Liquor Concentrate, Pumpkin Juice, and a Murloc Eyeball.',
  `QuestDescription`='Lady Agria Spada is a woman besieged by age. Of late she\'s borne a litany of ailments that keep her to her bed.$b$bOnly one thing eases her: the draught brewed by her master alchemist.$b$bI\'ve gathered a few ingredients, but the list is long and fussy. We still need Elgris Blossom Petals, Dun Kazad Liquor Concentrate, Pumpkin Juice, and a Murloc Eyeball.$b$bTake a turn through the market. Once you\'ve got the lot, report to my lady\'s manor. I don\'t dare the roads alone, but you... you\'re made of sterner stuff, aren\'t you?',
  `QuestCompletionLog`='Speak with Aldia Crayon, butler to the Spadas, at the family manor.',
  `RequiredItemId1`=558956, `RequiredItemCount1`=1,
  `RequiredItemId2`=558957, `RequiredItemCount2`=1,
  `RequiredItemId3`=558958, `RequiredItemCount3`=1,
  `RequiredItemId4`=558959, `RequiredItemCount4`=1,
  `RequiredItemId5`=558960, `RequiredItemCount5`=1
WHERE `ID`=1660056;

DELETE FROM `creature_questender` WHERE `quest`=1660056;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES (162802, 1660056);
