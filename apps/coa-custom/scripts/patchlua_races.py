import pathlib
p=pathlib.Path('out/Interface/GlueXML/CharacterCreate.lua')
src=p.read_bytes().decode('utf-8'); nl='\r\n' if '\r\n' in src else '\n'; s=src.replace('\r\n','\n')
def rep(old,new):
    global s
    assert s.count(old)==1, old[:70]; s=s.replace(old,new)
rep('MAX_RACES = 11;','''MAX_RACES = 32;

-- Local server: extra playable races. Buttons beyond the 11 of the XML are created on demand
-- and laid out in columns, Alliance on the left and Horde on the right.
function CharacterCreate_EnsureRaceButtons()
	for i = 1, MAX_RACES do
		if not _G["CharCreateRaceButton"..i] then
			local button = CreateFrame("CheckButton", "CharCreateRaceButton"..i, CharCreateRaceButtonsFrame, "CharCreateRaceButtonTemplate")
			button:SetID(i)
			button:Hide()
		end
	end
end

function CharacterCreate_LayoutRaceButtons(numRaces)
	local perColumn, columnWidth, rowHeight = 6, 190, 82
	local count = { Alliance = 0, Horde = 0 }
	for i = 1, numRaces do
		local button = _G["CharCreateRaceButton"..i]
		local ok, _, faction = pcall(GetFactionForRace, i)
		if not ok or faction ~= "Horde" then
			faction = "Alliance"
		end
		local k = count[faction]
		count[faction] = k + 1
		local column, row = math.floor(k / perColumn), k % perColumn
		button:ClearAllPoints()
		if faction == "Alliance" then
			button:SetPoint("TOPLEFT", CharCreateRaceButtonsFrame, "TOPLEFT", 32 + column * columnWidth, -75 - row * rowHeight)
		else
			button:SetPoint("TOPRIGHT", CharCreateRaceButtonsFrame, "TOPRIGHT", -32 - column * columnWidth, -75 - row * rowHeight)
		end
		button:SetFaction(faction)
		button:Show()
	end
end''')
rep('''function CharacterCreate_OnShow()
	NewCharacterSetupUtil.ClearPendingData()
''','''function CharacterCreate_OnShow()
	NewCharacterSetupUtil.ClearPendingData()
	CharacterCreate_EnsureRaceButtons()
''')
rep('''	for i = CharacterCreate.numRaces + 1, MAX_RACES, 1 do
		_G["CharCreateRaceButton"..i]:Hide()
	end''','''	for i = CharacterCreate.numRaces + 1, MAX_RACES, 1 do
		_G["CharCreateRaceButton"..i]:Hide()
	end
	CharacterCreate_LayoutRaceButtons(CharacterCreate.numRaces)''')
rep('''	local coords = RACE_ICON_TCOORDS[raceEnglishName.."_"..gender]
''','''	local coords = RACE_ICON_TCOORDS[raceEnglishName.."_"..gender] or RACE_ICON_TCOORDS["HUMAN_"..gender] or {0, 1, 0, 1}
''')
rep('''function CharCreateRaceButtonMixin:OnLoad()
	self:GetNormalTexture():SetDrawLayer("BACKGROUND")
	self:GetPushedTexture():SetDrawLayer("BACKGROUND")

	local _, faction = GetFactionForRace(self:GetID())
''','''function CharCreateRaceButtonMixin:OnLoad()
	self:GetNormalTexture():SetDrawLayer("BACKGROUND")
	self:GetPushedTexture():SetDrawLayer("BACKGROUND")

	local faction
	if self:GetID() > 0 then
		local ok, _, f = pcall(GetFactionForRace, self:GetID())
		faction = ok and f or nil
	end
''')
rep('GetFlavorText("RACE_INFO_"..raceEnglishName, gender)..', '(GetFlavorText("RACE_INFO_"..raceEnglishName, gender) or raceName)..')
# icons of the extra races (keyed by display name: High Elf shares the Blood Elf file name)
rep('''	local coords = RACE_ICON_TCOORDS[raceEnglishName.."_"..gender] or RACE_ICON_TCOORDS["HUMAN_"..gender] or {0, 1, 0, 1}

	self:GetNormalTexture():SetTexCoord(unpack(coords))
	self:GetPushedTexture():SetTexCoord(unpack(coords))
''', '''	local coords = RACE_ICON_TCOORDS[raceEnglishName.."_"..gender] or RACE_ICON_TCOORDS["HUMAN_"..gender] or {0, 1, 0, 1}

	local iconFile = CHAR_CREATE_EXTRA_RACE_ICONS[raceName]
	if iconFile then
		if SetPortraitToTexture then
			-- crop the square icon to a circle like the built-in race icons
			SetPortraitToTexture(self:GetNormalTexture(), iconFile)
			SetPortraitToTexture(self:GetPushedTexture(), iconFile)
			coords = {0, 1, 0, 1}
		else
			self:SetNormalTexture(iconFile)
			self:SetPushedTexture(iconFile)
			self:GetNormalTexture():SetSize(40, 40)
			self:GetPushedTexture():SetSize(40, 40)
			coords = {0.1, 0.9, 0.1, 0.9}
		end
	else
		self:SetNormalTexture("Interface\\\\Glues\\\\CharacterCreate\\\\UI-CHARACTERCREATE-RACES_Round")
		self:SetPushedTexture("Interface\\\\Glues\\\\CharacterCreate\\\\UI-CHARACTERCREATE-RACES_Round")
	end
	self:GetNormalTexture():SetTexCoord(unpack(coords))
	self:GetPushedTexture():SetTexCoord(unpack(coords))
''')
rep('MAX_RACES = 32;', '''MAX_RACES = 32;

CHAR_CREATE_EXTRA_RACE_ICONS = {
	["Worgen"] = "Interface\\\\Icons\\\\_BeastTaming_Worgen",
	["Goblin"] = "Interface\\\\Icons\\\\INV_Gizmo_GoblinBoomBox_01",
	["High Elf"] = "Interface\\\\Icons\\\\_HighElf_01",
	["Tuskarr"] = "Interface\\\\Icons\\\\INV_Misc_Head_Tuskarr",
	["Kul Tiran"] = "Interface\\\\Icons\\\\achievement_alliedrace_kultiranhuman",
	["Taunka"] = "Interface\\\\Icons\\\\INV_Misc_Head_Tauren_02",
	["Vrykul"] = "Interface\\\\Icons\\\\INV_Misc_Head_Vrykul",
	["Vulpera"] = "Interface\\\\Icons\\\\achievement_alliedrace_vulpera",
	["Pandaren"] = "Interface\\\\Icons\\\\achievement_character_pandaren_female",
	["Naga"] = "Interface\\\\Icons\\\\achievement_boss_elitenagamale",
	["Broken"] = "Interface\\\\Icons\\\\Achievement_Character_Draenei_Male",
	["Fel Orc"] = "Interface\\\\Icons\\\\achievement_character_orc_male_brn",
	["Forest Troll"] = "Interface\\\\Icons\\\\INV_Misc_Head_Troll_01",
	["Ice Troll"] = "Interface\\\\Icons\\\\INV_Misc_Head_Troll_02",
	["Skeleton"] = "Interface\\\\Icons\\\\SkeletonPowerful",
	["Earthen"] = "Interface\\\\Icons\\\\ability_earthen_titanwroughtframe",
	["Drakkari Troll"] = "Interface\\\\Icons\\\\Achievement_Character_Troll_Male",
	["Zandalari Troll"] = "Interface\\\\Icons\\\\achievement_alliedrace_zandalaritroll",
	["Murloc"] = "Interface\\\\Icons\\\\INV_Misc_Head_Murloc_01",
}''')

# races whose client data only has a male model
rep('''function SetCharacterRace(id)

	CharacterCreate.selectedRace = id;
	for i=1, CharacterCreate.numRaces, 1 do
		local button = _G["CharCreateRaceButton"..i];
		if ( i == id ) then
			button:SetChecked(1);
		else
			button:SetChecked(0);
		end
	end
''', '''CHAR_CREATE_MALE_ONLY_RACES = {
	["Tuskarr"] = true, ["Taunka"] = true, ["Vrykul"] = true, ["Forest Troll"] = true, ["Ice Troll"] = true,
	["Broken"] = true, ["Fel Orc"] = true, ["Skeleton"] = true, -- Murloc: male = Whim murloc, female = classic murloc
}

function CharacterCreate_IsMaleOnlyRace(id)
	local button = _G["CharCreateRaceButton"..(id or CharacterCreate.selectedRace or 0)]
	local name = button and button.Text and button.Text:GetText()
	return name and CHAR_CREATE_MALE_ONLY_RACES[name] or false
end

function SetCharacterRace(id)

	CharacterCreate.selectedRace = id;
	for i=1, CharacterCreate.numRaces, 1 do
		local button = _G["CharCreateRaceButton"..i];
		if ( i == id ) then
			button:SetChecked(1);
		else
			button:SetChecked(0);
		end
	end

	if CharacterCreate_IsMaleOnlyRace(id) then
		CharCreateFemaleButton:Hide()
		if GetSelectedSex() ~= SEX_MALE then
			SetCharacterGender(SEX_MALE)
			return
		end
	elseif CharCreateMaleButton:IsShown() then
		CharCreateFemaleButton:Show()
	end
''')
rep('''		CharCreateMaleButton:Show()
		CharCreateFemaleButton:Show()
		CharCreateOkayButton:SetText(CUSTOMIZE);''', '''		CharCreateMaleButton:Show()
		CharCreateFemaleButton:SetShown(not CharacterCreate_IsMaleOnlyRace())
		CharCreateOkayButton:SetText(CUSTOMIZE);''')

p.write_bytes(s.replace('\n',nl).encode('utf-8'))
print('race buttons patched')
