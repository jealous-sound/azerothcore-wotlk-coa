-- Bind the Tradesman's Scroll item script.
--
-- AzerothCore attaches an ItemScript by name through item_template.ScriptName,
-- so the C++ in mod-ascension-compat (AscensionTradesmanScroll.cpp) does nothing
-- until these rows point at it.
--
-- Three entries carry the scroll:
--   777998   Tradesman's Scroll            "Maxes out a Profession of your choice"
--   2200018  Soulbound Tradesman's Scroll  same, bind-on-pickup
--   1008036  Tradesman's Scroll (DEPRECATED) -- granted 300 skill points rather
--            than maxing. Bound too, so an existing one is not left inert; the
--            script raises to the rank cap either way, which is the closer
--            behaviour of the two.

UPDATE `item_template`
   SET `ScriptName` = 'ascension_tradesman_scroll'
 WHERE `entry` IN (777998, 1008036, 2200018);
