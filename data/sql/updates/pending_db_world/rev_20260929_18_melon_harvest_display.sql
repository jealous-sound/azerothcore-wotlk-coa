-- Melon harvest node (999013, used by Goldshire's Generosity 1660059, 10
-- spawns) had displayId 334, the same model as "Barrel of Melon Juice"
-- (entry 3659) -- a barrel, not an actual melon. Fixed to 332, the real
-- standalone melon-fruit model (matches entry 2300548's "Melon" in the
-- archive/2300xxx family).
UPDATE `gameobject_template` SET `displayId`=332 WHERE `entry`=999013;
