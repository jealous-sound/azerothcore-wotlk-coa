-- Underwalk (800797) "Sneak around in the spirit world": the spell itself only carries the stealth
-- shapeshift, the stealth and the movement slow, so the Reaper stealthed without ever entering the
-- spirit world. The look is authored in the hidden companion 414054 "Reaper Stealth Ghost Visual",
-- a single SPELL_AURA_SCREEN_EFFECT with MiscValue 1. ScreenEffect 1 is the ghost presentation: the
-- desaturated screen, the spirit world sky, and its ambience and music. Nothing applied it.
--
-- The core has nothing to do for SPELL_AURA_SCREEN_EFFECT beyond making the player hold the aura; the
-- client resolves the MiscValue against its own ScreenEffect.dbc. 414054 is already in the server's
-- Spell.dbc and the client's, it draws no buff icon, and it is not passive, so it is sent to the client.
--
-- Ride it on Underwalk with SPELL_LINK_AURA (type 2) so it applies and drops with the stealth on every
-- path: the cast, any other spell that applies Underwalk, an aura restored at login, and removal by
-- cancel, combat or death. 414054 keeps its authored 20 minute duration.
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 800797 AND `spell_effect` = 414054 AND `type` = 2;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(800797, 414054, 2, 'CoA Underwalk - spirit world screen effect while stealthed');
