# Character creation: the Murloc is always shown undressed. The creation screen paints armor onto the body with the
# human layout, which smears it over the murloc model (its skin colour is painted the same way and must stay clean).
# Run from the scratchpad after the other CharacterCreate.lua patches: patches out\Interface\GlueXML\CharacterCreate.lua
p = 'out/Interface/GlueXML/CharacterCreate.lua'
src = open(p, 'rb').read().decode('utf-8')
KEY = '-- Local server: the Murloc is always shown undressed'
if KEY not in src:
    nl = '\r\n' if '\r\n' in src else '\n'
    code = KEY + ''' (armor is painted with the human body layout).
local function CharacterCreate_IsMurlocSelected()
	local _, fileString = GetNameForRace()
	return fileString == "Troglodyte"
end
do
	local dress, previewClassOutfit = C_CharacterCreate.Dress, C_CharacterCreate.PreviewClassOutfit
	C_CharacterCreate.Dress = function(...)
		if CharacterCreate_IsMurlocSelected() then return C_CharacterCreate.Undress() end
		return dress(...)
	end
	C_CharacterCreate.PreviewClassOutfit = function(...)
		if CharacterCreate_IsMurlocSelected() then return C_CharacterCreate.Undress() end
		return previewClassOutfit(...)
	end
	local raceOnClick = CharacterRace_OnClick
	function CharacterRace_OnClick(...)
		raceOnClick(...)
		if CharacterCreate_IsMurlocSelected() then C_CharacterCreate.Undress() end
	end
end
'''
    src = src.rstrip() + nl + nl + code.replace('\n', nl)
    open(p, 'wb').write(src.encode('utf-8'))
    print('murloc undress patch applied')
else:
    print('murloc undress patch already present')

# Male (Whim) murloc: the race's hair style option is the armor (ChrRaces hair customization "MURLOCARMOR");
# the female (classic) murloc has no armor, so the option is hidden for it.
src = open(p, 'rb').read().decode('utf-8')
KEY2 = '-- Local server: Murloc armor option'
if KEY2 not in src:
    nl = '\r\n' if '\r\n' in src else '\n'
    code = KEY2 + '''
HAIR_MURLOCARMOR_STYLE = "Armor"
do
	local updateHair = CharacterCreate_UpdateHairCustomization
	function CharacterCreate_UpdateHairCustomization(...)
		updateHair(...)
		if CharacterCreate_IsMurlocSelected() and GetSelectedSex() ~= SEX_MALE then
			CharCreateCustomizationButton3:Hide()
		end
	end
end
'''
    src = src.rstrip() + nl + nl + code.replace('\n', nl)
    open(p, 'wb').write(src.encode('utf-8'))
    print('murloc armor option patch applied')
