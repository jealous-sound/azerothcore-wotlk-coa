# Character creation: show each extra race's bonus ("signature") racial, the one borrowed from another race
# (server: playercreateinfo_spell_custom "racial combo", sql/racial_combos.sql). Keyed by race id, so the High Elf
# (which shares the Blood Elf files) gets its own line. The old name search of patchlua_races2.py never matched the
# client texts ("Berserk, ..." for Berserking) and is switched off here.
# Run from the scratchpad after the other CharacterCreate.lua patches: patches out\Interface\GlueXML\CharacterCreate.lua
p = 'out/Interface/GlueXML/CharacterCreate.lua'
src = open(p, 'rb').read().decode('utf-8')
KEY = '-- Local server: bonus racial of the extra races'
if KEY not in src:
    nl = '\r\n' if '\r\n' in src else '\n'
    code = KEY + ''' (race id = {source race, ability index, spell name})
CHAR_CREATE_EXTRA_RACE_SIGNATURE, CHAR_CREATE_EXTRA_RACE_SIGNATURE_HORDE = {}, {}
CHAR_CREATE_RACE_BONUS = {
	[9] = {"GNOME", 1, "Escape Artist"}, [12] = {"DRAENEI", 2, "Gift of the Naaru"}, [13] = {"TROLL", 1, "Berserking"},
	[14] = {"HUMAN", 5, "Every Man for Himself"}, [15] = {"TAUREN", 1, "War Stomp"}, [16] = {"DWARF", 1, "Stoneform"},
	[17] = {"DWARF", 3, "Frost Resistance"}, [18] = {"ORC", 1, "Blood Fury"}, [19] = {"GNOME", 1, "Escape Artist"},
	[20] = {"TAUREN", 1, "War Stomp"}, [21] = {"BLOODELF", 3, "Arcane Torrent"}, [22] = {"SCOURGE", 1, "Will of the Forsaken"},
	[23] = {"BLOODELF", 3, "Arcane Torrent"}, [24] = {"NIGHTELF", 1, "Shadowmeld"}, [25] = {"DWARF", 1, "Stoneform"},
	[26] = {"DWARF", 1, "Stoneform"}, [27] = {"TAUREN", 1, "War Stomp"}, [28] = {"ORC", 1, "Blood Fury"},
	[29] = {"DRAENEI", 2, "Gift of the Naaru"}, [30] = {"NIGHTELF", 1, "Shadowmeld"}, [31] = {"GNOME", 1, "Escape Artist"},
}
CHAR_CREATE_RACE_BONUS_FROM = {
	GNOME = "Gnome", DRAENEI = "Draenei", TROLL = "Troll", HUMAN = "Human", TAUREN = "Tauren", DWARF = "Dwarf",
	ORC = "Orc", BLOODELF = "Blood Elf", SCOURGE = "Undead", NIGHTELF = "Night Elf",
}

function CharacterCreate_RaceBonusText(raceID, withIcon)
	local bonus = raceID and CHAR_CREATE_RACE_BONUS[raceID]
	if not bonus then
		return nil
	end
	local source, index, name = bonus[1], bonus[2], bonus[3]
	local text = "|cff00ff96Bonus racial (" .. (CHAR_CREATE_RACE_BONUS_FROM[source] or source) .. "): " .. name .. "|r"
	local description = _G["ABILITY_INFO_" .. source .. index]
	if description then
		text = text .. "|n" .. description
	end
	local spell = CHAR_CREATE_PASSIVES and CHAR_CREATE_PASSIVES[source] and CHAR_CREATE_PASSIVES[source][index]
	if withIcon and spell and GetSpellData then
		local ok, _, _, icon = pcall(GetSpellData, spell, false, true)
		if ok and icon then
			text = "|T" .. icon .. ":24:24:0:0|t" .. text
		end
	end
	return text
end

do
	local setRace = CharCreateRaceButtonMixin.SetRace
	function CharCreateRaceButtonMixin:SetRace(...)
		setRace(self, ...)
		local text = CharacterCreate_RaceBonusText(self:GetID(), true)
		if text and self.tooltipText then
			tinsert(self.tooltipText, text .. "\\n")
		end
	end
	local setCharacterRace = SetCharacterRace
	function SetCharacterRace(id, ...)
		setCharacterRace(id, ...)
		local text = CharacterCreate_RaceBonusText(id or CharacterCreate.selectedRace)
		local bullets = CharCreateRaceInfoFrame and CharCreateRaceInfoFrame.scrollFrame.scrollChild.bulletText
		if text and bullets then
			bullets:SetText((bullets:GetText() or "") .. text .. "\\n\\n")
		end
	end
end
'''
    src = src.rstrip() + nl + nl + code.replace('\n', nl)
    open(p, 'wb').write(src.encode('utf-8'))
    print('racial bonus patch applied')
else:
    print('racial bonus patch already present')
