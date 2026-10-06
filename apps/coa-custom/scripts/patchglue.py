import pathlib
src=pathlib.Path('B/Interface/GlueXML/GlueParent.lua').read_bytes().decode('utf-8-sig')
nl='\r\n' if '\r\n' in src else '\n'; s=src.replace('\r\n','\n')
def rep(old,new):
    global s
    assert s.count(old)==1, old[:60]; s=s.replace(old,new)
rep('GlueAmbienceTracks["HORDEREGULAR"] = "GlueScreenOrcTroll";\n', '''GlueAmbienceTracks["HORDEREGULAR"] = "GlueScreenOrcTroll";

-- Local server: the extra playable races use the background of a close race.
EXTRA_RACE_BACKGROUNDS = {
	["GOBLIN"] = "ORC", ["ZANDALARI TROLL"] = "TROLL", ["WORGEN"] = "HUMAN", ["KULTIRAN"] = "HUMAN",
	["TUSKARR"] = "DWARF", ["TAUNKA"] = "TAUREN", ["VRYKUL"] = "DWARF", ["VULPERA"] = "ORC",
	["PANDAREN"] = "NIGHTELF", ["NAGA_"] = "ORC", ["BROKEN"] = "DRAENEI", ["FELORC"] = "ORC",
	["FORESTTROLL"] = "TROLL", ["ICETROLL"] = "TROLL", ["SKELETON"] = "SCOURGE", ["NPC_EARTHEN"] = "DWARF",
	["DRAKKARITROLL"] = "TROLL", ["TROGLODYTE"] = "GNOME",
}
for race, background in pairs(EXTRA_RACE_BACKGROUNDS) do
	GlueAmbienceTracks[race] = GlueAmbienceTracks[background]
end
''')
rep('''function SetBackgroundModel(model, name)
    local nameupper = strupper(name);
''', '''function SetBackgroundModel(model, name)
    local nameupper = strupper(name);
    local base, zoom = string.match(nameupper, "^(.-)(_ZOOM)$")
    local mapped = EXTRA_RACE_BACKGROUNDS[base or nameupper]
    if mapped then
        name = mapped..(zoom or "")
        nameupper = name
    end
''')
out=pathlib.Path('out/Interface/GlueXML/GlueParent.lua'); out.parent.mkdir(parents=True,exist_ok=True)
out.write_bytes(s.replace('\n',nl).encode('utf-8'))
print('GlueParent patched')
