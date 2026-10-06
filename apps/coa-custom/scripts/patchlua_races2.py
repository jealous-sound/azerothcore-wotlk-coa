import pathlib
p=pathlib.Path('out/Interface/GlueXML/CharacterCreate.lua')
src=p.read_bytes().decode('utf-8'); nl='\r\n' if '\r\n' in src else '\n'; s=src.replace('\r\n','\n')
def rep(old,new):
    global s
    assert s.count(old)==1, old[:70]; s=s.replace(old,new)
rep('''CHAR_CREATE_EXTRA_RACE_ICONS = {''', '''-- racials shown for the extra races: those of the base race they borrow (same rule as the server)
CHAR_CREATE_EXTRA_RACE_RACIALS = {
	["WORGEN"] = "NIGHTELF", ["GOBLIN"] = "ORC", ["TUSKARR"] = "DWARF", ["KULTIRAN"] = "HUMAN", ["TAUNKA"] = "TAUREN",
	["VRYKUL"] = "HUMAN", ["VULPERA"] = "TROLL", ["NAGA_"] = "SCOURGE", ["BROKEN"] = "DRAENEI", ["FELORC"] = "ORC",
	["FORESTTROLL"] = "TROLL", ["ICETROLL"] = "TROLL", ["SKELETON"] = "SCOURGE", ["NPC_EARTHEN"] = "DWARF",
	["DRAKKARITROLL"] = "TROLL", ["ZANDALARI TROLL"] = "TROLL", ["PANDAREN"] = "DRAENEI", ["TROGLODYTE"] = "SCOURGE",
}
CHAR_CREATE_EXTRA_RACE_RACIALS_HORDE = { ["PANDAREN"] = "TAUREN", ["TROGLODYTE"] = "SCOURGE" }
CHAR_CREATE_EXTRA_RACE_INFO = {
	["WORGEN"] = "Cursed Gilneans who learned to master the beast within. Worgen fight with the speed and stealth of the Night Elves.",
	["GOBLIN"] = "Cunning merchants and inventors from Kezan, as tough as any Orc when the profit is right.",
	["TUSKARR"] = "Hardy fisher-folk of Northrend's coasts, as stubborn and cold-resistant as the Dwarves.",
	["KULTIRAN"] = "Proud sailors from the island kingdom of Kul Tiras, sharing the human spirit of their cousins.",
	["TAUNKA"] = "Northern kin of the Tauren, forged by the harsh winters of the Borean Tundra.",
	["VRYKUL"] = "Towering warriors of Northrend, distant ancestors of humanity.",
	["VULPERA"] = "Nimble desert scavengers from Vol'dun who survive with troll-like resilience.",
	["PANDAREN"] = "Wandering monks and brewers of Pandaria. Alliance Pandaren share the gifts of the Draenei, Horde Pandaren those of the Tauren.",
	["NAGA_"] = "Serpentine children of the deep, as unyielding as the Forsaken.",
	["BROKEN"] = "Draenei twisted by the fel, who still carry the Light of the Naaru.",
	["FELORC"] = "Orcs corrupted by demon blood, stronger and angrier than their brothers.",
	["FORESTTROLL"] = "Trolls of the Hinterlands and Eversong, regenerating and ferocious.",
	["ICETROLL"] = "Trolls of the frozen north, hardened by ice and hunger.",
	["SKELETON"] = "Restless dead risen once more, bound by the will of the Forsaken.",
	["NPC_EARTHEN"] = "Titan-forged guardians of stone, kin to the Dwarves.",
	["DRAKKARITROLL"] = "Trolls of the fallen Drakkari empire of Zul'Drak.",
	["ZANDALARI TROLL"] = "Proud trolls of Zandalar, eldest and wisest of their kind.",
	["TROGLODYTE"] = "Murlocs! Mrglglgl! Creatures of the shallows who breathe underwater, sharing the gifts of the Forsaken.",
}

-- signature racial borrowed from another race (server: playercreateinfo_spell_custom "racial combo")
CHAR_CREATE_EXTRA_RACE_SIGNATURE = {
	["WORGEN"] = {"TROLL", "Berserking"}, ["GOBLIN"] = {"GNOME", "Escape Artist"}, ["TUSKARR"] = {"TAUREN", "War Stomp"},
	["KULTIRAN"] = {"DWARF", "Stoneform"}, ["TAUNKA"] = {"DWARF", "Frost Resistance"}, ["VRYKUL"] = {"ORC", "Blood Fury"},
	["VULPERA"] = {"GNOME", "Escape Artist"}, ["NAGA_"] = {"BLOODELF", "Arcane Torrent"}, ["BROKEN"] = {"SCOURGE", "Will of the Forsaken"},
	["FELORC"] = {"BLOODELF", "Arcane Torrent"}, ["FORESTTROLL"] = {"NIGHTELF", "Shadowmeld"}, ["ICETROLL"] = {"DWARF", "Stoneform"},
	["SKELETON"] = {"DWARF", "Stoneform"}, ["NPC_EARTHEN"] = {"TAUREN", "War Stomp"}, ["DRAKKARITROLL"] = {"ORC", "Blood Fury"},
	["ZANDALARI TROLL"] = {"DRAENEI", "Gift of the Naaru"}, ["PANDAREN"] = {"TAUREN", "War Stomp"}, ["TROGLODYTE"] = {"NIGHTELF", "Shadowmeld"},
}
CHAR_CREATE_EXTRA_RACE_SIGNATURE_HORDE = { ["PANDAREN"] = {"DRAENEI", "Gift of the Naaru"}, ["TROGLODYTE"] = {"GNOME", "Escape Artist"} }

function CharacterCreate_AliasExtraRacials(faction)
	for race, base in pairs(CHAR_CREATE_EXTRA_RACE_RACIALS) do
		if faction == "Horde" and CHAR_CREATE_EXTRA_RACE_RACIALS_HORDE[race] then
			base = CHAR_CREATE_EXTRA_RACE_RACIALS_HORDE[race]
		end
		for i = 1, 10 do
			_G["ABILITY_INFO_"..race..i] = _G["ABILITY_INFO_"..base..i]
		end
		local passives = CHAR_CREATE_PASSIVES and CHAR_CREATE_PASSIVES[base]
		local count = 0
		while _G["ABILITY_INFO_"..race..(count + 1)] do
			count = count + 1
		end
		local signature = (faction == "Horde" and CHAR_CREATE_EXTRA_RACE_SIGNATURE_HORDE[race]) or CHAR_CREATE_EXTRA_RACE_SIGNATURE[race]
		local signaturePassive
		if signature then
			for i = 1, 10 do
				local text = _G["ABILITY_INFO_"..signature[1]..i]
				if text and string.find(text, signature[2], 1, true) then
					count = count + 1
					_G["ABILITY_INFO_"..race..count] = text
					signaturePassive = CHAR_CREATE_PASSIVES and CHAR_CREATE_PASSIVES[signature[1]] and CHAR_CREATE_PASSIVES[signature[1]][i]
					break
				end
			end
			for i = count + 1, 10 do
				_G["ABILITY_INFO_"..race..i] = nil
			end
		end
		if CHAR_CREATE_PASSIVES then
			local list = {}
			for i, spell in ipairs(passives or {}) do
				list[i] = spell
			end
			if signaturePassive then
				list[count] = signaturePassive
			end
			CHAR_CREATE_PASSIVES[race] = list
		end
		if not _G["RACE_INFO_"..race] then
			_G["RACE_INFO_"..race] = CHAR_CREATE_EXTRA_RACE_INFO[race]
		end
	end
end

CHAR_CREATE_EXTRA_RACE_ICONS = {''')
# per button (tooltip) and for the selected race (info panel)
rep('''		raceEnglishName = raceEnglishName:upper()
		button = _G["CharCreateRaceButton"..index]
''', '''		raceEnglishName = raceEnglishName:upper()
		button = _G["CharCreateRaceButton"..index]
		local okFaction, _, buttonFaction = pcall(GetFactionForRace, index)
		CharacterCreate_AliasExtraRacials(okFaction and buttonFaction or "Alliance")
''')
rep('''	local name, faction = GetFactionForRace(CharacterCreate.selectedRace);

	if faction == nil then
		faction = "Alliance";
	end
''', '''	local name, faction = GetFactionForRace(CharacterCreate.selectedRace);

	if faction == nil then
		faction = "Alliance";
	end
	CharacterCreate_AliasExtraRacials(faction)
''')
p.write_bytes(s.replace('\n',nl).encode('utf-8'))
print('racial texts patched')
