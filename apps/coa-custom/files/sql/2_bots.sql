-- CoA Custom 1.2: random bots of the races whose client model exists only as a male (Tuskarr, Taunka, Vrykul, Broken,
-- Fel Orc, Forest Troll, Ice Troll, Skeleton) become male; a female of these races has no body (invisible or broken).
-- New bots are created male by the server. Players' characters are not touched.
UPDATE acore_characters.characters c JOIN acore_auth.account a ON a.id = c.account
SET c.gender = 0
WHERE a.username LIKE 'RNDBOT%' AND c.gender = 1 AND c.race IN (15, 17, 18, 22, 23, 24, 25, 26);
