-- Bookworm (1660000): Flags was 0 (should be 8), QuestDescription (Details
-- text) was NULL, QuestCompletionLog had stray leftover text (should be
-- empty per source data).
UPDATE `quest_template` SET
  `Flags`=8,
  `QuestDescription`='<A woman greets you, visibly agitated. A sharp mix of anger and worry flashes in her eyes.>$b$bSorry, I didn\'t mean to make a scene, but... that blasted brother of mine, always buried in his books! I\'ve been out here forever, yelling my lungs out, and nothing. The guards have already warned me about "disturbing the abbey\'s peace" one more time.$b$bWould you mind going in there and dragging him out by the ears if you have to? Our mother\'s on her deathbed, and I\'ve traveled a long way to fetch him so he can say goodbye. The ungrateful wretch.',
  `QuestCompletionLog`=''
WHERE `ID`=1660000;
