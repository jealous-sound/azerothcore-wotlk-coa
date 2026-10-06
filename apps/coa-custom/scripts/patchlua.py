src=open('B/Interface/GlueXML/CharacterCreate.lua','r',encoding='utf-8-sig',newline='').read()
nl='\r\n' if '\r\n' in src else '\n'
def rep(old,new):
    global src
    assert src.count(old)==1, old
    src=src.replace(old,new)
override='''HERO_CLASS_ID = 10

-- Local server: let the classic (vanilla) classes be created next to the CoA classes.
local VANILLA_CLASS_IDS = {[1]=true,[2]=true,[3]=true,[4]=true,[5]=true,[6]=true,[7]=true,[8]=true,[9]=true,[11]=true}
function CharacterCreate_EnableVanillaClasses()
	if not C_CharacterCreate or C_CharacterCreate.vanillaClassesEnabled then
		return
	end
	local origCanCreateClass = C_CharacterCreate.CanCreateClass
	C_CharacterCreate.CanCreateClass = function(classID, ...)
		if VANILLA_CLASS_IDS[classID] then
			return true
		end
		return origCanCreateClass(classID, ...)
	end
	C_CharacterCreate.CanCreateWCR = function() return true end
	-- CharBaseInfo.dbc holds at most 30 classes per race in this client, so Death Knight
	-- is left out of it; accept it here, the server validates the real combination.
	local origIsRaceClassValid = IsRaceClassValid
	IsRaceClassValid = function(race, classID, ...)
		if classID == 6 then
			return true
		end
		return origIsRaceClassValid(race, classID, ...)
	end
	C_CharacterCreate.vanillaClassesEnabled = true
end
CharacterCreate_EnableVanillaClasses()'''
rep('HERO_CLASS_ID = 10', override)
rep('''function CharacterCreate_RefreshClassButtons(self)
	local isCoA''','''function CharacterCreate_RefreshClassButtons(self)
	CharacterCreate_EnableVanillaClasses()
	local isCoA''')
rep('CharCreateSwapClassesButton:SetShown(C_Realm.IsDevelopment() and isCoA and (isClassicClass or canCreateHero))',
    'CharCreateSwapClassesButton:SetShown(isCoA and isClassicClass)')
rep('''	if (CharacterCreate.classesVisible == "FREEPICK") then
		CharacterCreate_ShowRegularClasses(CharacterCreate)
		return
	elseif (CharacterCreate.classesVisible == "REGULAR") then
		CharacterCreate_ShowCoAClasses(CharacterCreate)
	elseif (CharacterCreate.classesVisible == "COA") then
		CharacterCreate_ShowFreepickClasses(CharacterCreate)
	end''','''	if (CharacterCreate.classesVisible == "REGULAR") then
		CharacterCreate_ShowCoAClasses(CharacterCreate)
	else
		CharacterCreate_ShowRegularClasses(CharacterCreate)
	end''')
# label the toggle button with what it switches to
rep('''	CharacterCreate.classesVisible = "REGULAR"
end''','''	CharacterCreate.classesVisible = "REGULAR"
	CharCreateSwapClassesButton:SetText("CoA Classes")
end''')
rep('''	self.classesVisible = "COA"
end

function CharacterCreate_ShowFreepickClasses''','''	self.classesVisible = "COA"
	CharCreateSwapClassesButton:SetText("Classic Classes")
end

function CharacterCreate_ShowFreepickClasses''')
src=src.replace('\r\n','\n').replace('\n',nl)
open('out/Interface/GlueXML/CharacterCreate.lua','w',encoding='utf-8',newline='').write(src)
print('ok')
