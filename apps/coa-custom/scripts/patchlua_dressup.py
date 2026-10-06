# Character window / Wardrobe backgrounds are per-race atlases that only exist for the 10 stock races
# (DressUpBackground-<Race>, transmog-background-race-<race>). For the new races SetAtlas failed ("an interface
# error occurred" in the character window). The new races use the background of a base race.
# Run from the scratchpad (inputs extracted with mpqget.py into lua\): writes out\Interface\FrameXML\...
import os

FALLBACK = '''-- Custom races: backgrounds exist only for the stock races; the others use a base race's.
DRESSUP_RACE_FALLBACK = DRESSUP_RACE_FALLBACK or {
	Goblin = "Orc", ["Zandalari Troll"] = "Troll", Worgen = "Human", Tuskarr = "Dwarf", KulTiran = "Human",
	Taunka = "Tauren", Vrykul = "Human", Vulpera = "Orc", Pandaren = "Dwarf", Naga_ = "Troll", Broken = "Draenei",
	FelOrc = "Orc", ForestTroll = "Troll", IceTroll = "Troll", Skeleton = "Scourge", Npc_Earthen = "Dwarf",
	DrakkariTroll = "Troll", Troglodyte = "Gnome",
}
function GetDressUpRaceFileName(fileName)
	return fileName and DRESSUP_RACE_FALLBACK[fileName] or fileName
end
'''


def patch(src, dst, old, new):
    text = open(src, encoding='utf-8-sig', newline='').read()
    nl = '\r\n' if '\r\n' in text else '\n'
    old, new = old.replace('\n', nl), new.replace('\n', nl)
    assert text.count(old) == 1, (src, old[:60])
    text = text.replace(old, new)
    os.makedirs(os.path.dirname(dst), exist_ok=True)
    open(dst, 'w', encoding='utf-8', newline='').write(text)


patch('lua/interface/framexml/dressupframe.lua', 'out/Interface/FrameXML/DressUpFrame.lua',
      '''function DressUpTexturePath(fileName, atlas)
	if not fileName then
		fileName = select(2, UnitRace("player"))
	end
''', FALLBACK + '''
function DressUpTexturePath(fileName, atlas)
	if not fileName then
		fileName = select(2, UnitRace("player"))
	end
	fileName = GetDressUpRaceFileName(fileName)
''')

patch('lua/interface/framexml/util/appearanceutil.lua', 'out/Interface/FrameXML/Util/AppearanceUtil.lua',
      '''function AppearanceUtil.GetBackgroundAtlas()
    local _, race = UnitRace("player")
    return "transmog-background-race-" .. race:lower()''', FALLBACK.replace('\t', '    ') + '''
function AppearanceUtil.GetBackgroundAtlas()
    local _, race = UnitRace("player")
    return "transmog-background-race-" .. GetDressUpRaceFileName(race):lower()''')
print('dress-up backgrounds patched')

# Vanilla classes spend normal talent points: no "You have unspent Talent Essence" tip / pulsing button for them.
patch('lua/interface/framexml/mainmenubarmicrobuttons.lua', 'out/Interface/FrameXML/MainMenuBarMicroButtons.lua',
      '''function CheckUnspentEssences()
''', '''function CheckUnspentEssences()
	if C_Player:IsDefaultClass() then
		return
	end
''')
print('unspent essence tip disabled for vanilla classes')
