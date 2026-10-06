import os
S=chr(92)
def rd(p): return open(p,'rb').read().decode('utf-8-sig').replace('\r\n','\n')
def wr(p,s):
    os.makedirs(os.path.dirname(p),exist_ok=True); open(p,'wb').write(s.replace('\n','\r\n').encode('utf-8'))
def rep(s,old,new):
    assert s.count(old)==1, old; return s.replace(old,new)
T='Interface/AddOns/Blizzard_TalentUI/'
stock=rd('STOCK/'+T+'Blizzard_TalentUI.lua'); asc=rd('BB/'+T+'Blizzard_TalentUI.lua')
lua=('-- Local server: vanilla (default) classes get the full Blizzard talent window,\n'
     '-- CoA classes keep Ascension\'s pet-only version.\n'
     'PlayerTalentFrame_VanillaMode = IsDefaultClass("player") and true or false\n'
     'if PlayerTalentFrame_VanillaMode then\n'
     'MAX_TALENT_TABS = 3\nDEFAULT_TALENT_SPEC = "spec1"\n'
     + stock + '\nelse\n' + asc + '\nend\n')
wr('out/'+T+'Blizzard_TalentUI.lua', lua)
x=rd('STOCK/'+T+'Blizzard_TalentUI.xml')
x=rep(x,'PlayerSpecTab_Load(self, "spec1");','PlayerSpecTab_Load(self, PlayerTalentFrame_VanillaMode and "spec1" or "petspec1");')
x=rep(x,'PlayerSpecTab_Load(self, "spec2");','if PlayerTalentFrame_VanillaMode then PlayerSpecTab_Load(self, "spec2"); else self:Hide(); end')
x=rep(x,'<OnLoad function="PlayerTalentFrame_OnLoad"/>','''<OnLoad>
				PlayerTalentFrame_OnLoad(self);
				if not PlayerTalentFrame_VanillaMode then
					PlayerTalentFrameTab2:Hide();
					PlayerTalentFrameTab3:Hide();
					PlayerTalentFrameTab4:ClearAllPoints();
					PlayerTalentFrameTab4:SetPoint("LEFT", PlayerTalentFrameTab1, "RIGHT", -15, 0);
				else
					-- preview mode: left click adds a point, right click removes it, Learn commits
					SetCVar("previewTalents", "1");
					local vanity = CreateFrame("Button", "PlayerTalentFrameVanityButton", self, "UIPanelButtonTemplate");
					vanity:SetSize(80, 22);
					vanity:SetPoint("TOPRIGHT", self, "TOPRIGHT", -42, -42);
					vanity:SetText("Vanity");
					vanity:SetScript("OnClick", function()
						HideUIPanel(PlayerTalentFrame);
						Collections:GoToTab(Collections.Tabs.Vanity);
					end);
				end
			</OnLoad>''')
wr('out/'+T+'Blizzard_TalentUI.xml', x)
u=rd('BB/Interface/FrameXML/UIParent.lua')
u=rep(u,'''	if Collections:IsShown() then
		HideUIPanel(Collections)
	elseif (IsCustomClass()''','''	if Collections:IsShown() then
		HideUIPanel(Collections)
	elseif IsDefaultClass("player") and not IsShiftKeyDown() then
		-- Local server: vanilla classes open the normal talent window (Shift opens Collections).
		TalentFrame_LoadUI()
		if PlayerTalentFrame_Toggle then
			PlayerTalentFrame_Toggle(false, GetActiveTalentGroup())
		end
	elseif (IsCustomClass()''')
wr('out/Interface/FrameXML/UIParent.lua', u)
print('ok')
