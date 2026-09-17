local root=arg[1] or '.'
local events,stored={},{}
function RegisterPlayerEvent(id,fn) events[id]=fn end
function CharDBExecute(q)
 local id,mode=q:match('VALUES %((%d+),(%d+)%)');assert(id and mode);stored[tonumber(id)]=tonumber(mode)
end
function CharDBQuery(q)
 local mode=stored[tonumber(q:match('guid=(%d+)'))]
 if mode then return {GetUInt32=function() return mode end} end
end
HighRisk={}
dofile(root..'/lua_scripts/highrisk_rulesets.lua')
local function player()
 local p={level=60,spells={},auras={},messages={},guid=1,rest=true,dead=false,combat=false,fly=false,instance=false,arena=false,pvp=false,ffa=false}
 function p:IsBot() return self.bot or false end
 function p:GetLevel() return self.level end
 function p:HasSpell(id) return self.spells[id] end
 function p:LearnSpell(id) self.spells[id]=true end
 function p:HasAura(id) return self.auras[id] end
 function p:AddAura(id) self.auras[id]=true end
 function p:RemoveAura(id) self.auras[id]=nil end
 function p:IsAlive() return not self.dead end
 function p:IsDead() return self.dead end
 function p:IsInCombat() return self.combat end
 function p:IsFlying() return self.fly end
 function p:GetGUIDLow() return self.guid end
 function p:HasFlag(field,flag) assert(field==0x96 and flag==32);return self.rest end
 function p:GetMap() return {IsDungeon=function()return self.instance end,IsRaid=function()return self.raid end,IsBattleground=function()return self.bg end,IsArena=function()return self.arena end} end
 function p:IsFFAPvP() return self.ffa end
 function p:SetFFA(v) self.ffa=v end
 function p:IsPvPFlagged() return self.pvp end
 function p:SetPvP(v) self.pvp=v end
 function p:SendBroadcastMessage(m) self.messages[#self.messages+1]=m end
 function p:RegisterEvent(fn) self.timer=fn;return 1 end
 function p:RemoveEventById() self.timer=nil end
 return p
end
local checks=0
local function check(v) checks=checks+1;assert(v,'Check '..checks..' failed') end
local function cast(p,id,triggered)
 local s={GetEntry=function()return id end};function s:Cancel()self.cancelled=true end
 events[5](5,p,s,triggered);return s
end
local p=player();events[3](3,p)
check(p.spells[84420] and p.spells[84421] and p.spells[84422]);check(not next(p.auras));check(not p.pvp)
cast(p,84421,false);check(stored[1]==3);check(p.auras[1004019] and not p.auras[1004119]);check(p.pvp and not p.ffa)
p.rest=false;p.timer(nil,nil,nil,p);check(p.ffa)
check(cast(p,84422,false).cancelled);check(stored[1]==3)
p.rest=true;p.combat=true;check(cast(p,84422,false).cancelled);p.combat=false
p.fly=true;check(cast(p,84422,false).cancelled);p.fly=false
p.dead=true;check(cast(p,84422,false).cancelled);p.dead=false
p.instance=true;check(cast(p,84422,false).cancelled);events[28](28,p);check(not p.ffa)
p.instance=false;p.arena=true;check(cast(p,84422,false).cancelled);p.arena=false
cast(p,84422,false);check(stored[1]==1);check(p.auras[1004119] and p.auras[9931032] and not p.auras[1004019]);check(not p.ffa and not p.pvp)
cast(p,84420,false);check(stored[1]==2);check(p.auras[1004119] and not p.auras[9931032]);check(p.pvp and not p.ffa)
cast(p,84421,true);check(stored[1]==3)
cast(p,84420,true);check(stored[1]==2)
local unknown=player();unknown.guid=3;cast(unknown,84421,true);check(not stored[3])
events[4](4,p);p=player();events[3](3,p);check(p.auras[1004119] and p.pvp)
p.auras={};events[36](36,p);check(p.auras[1004119])
p.level=14;check(cast(p,84421,false).cancelled)
local low=player();low.guid=2;low.level=14;events[3](3,low);check(not next(low.spells));low.level=15;events[13](13,low);check(low.spells[84421])
p=player();p.guid=4;p.level=30;events[3](3,p);cast(p,84421,true)
local R=HighRisk.Rulesets;local victim={}
check(R.Experience(p,100,victim,0)==120)
for source=1,4 do check(R.Experience(p,100,victim,source)==100) end
check(R.Experience(p,100,nil,0)==100);check(R.Experience(p,0,victim,0)==0)
for _,kind in ipairs({'instance','raid','bg','arena'}) do
 p[kind]=true;check(R.Experience(p,100,victim,0)==100);p[kind]=false
end
p.auras[1004019]=nil;check(R.Experience(p,100,victim,0)==100);R.Refresh(p)
p.dead=true;check(R.Experience(p,100,victim,0)==100);p.dead=false
p.level=60;check(R.Experience(p,100,victim,0)==100);p.level=59
check(R.Experience(p,100,victim,0)==120)
cast(p,84420,true);check(R.Experience(p,100,victim,0)==100)
cast(p,84422,true);check(R.Experience(p,100,victim,0)==100)
cast(p,84421,true);events[4](4,p);events[3](3,p);check(R.Experience(p,100,victim,0)==120)

check(events[12](12,p,100,victim,0)==120)
check(events[12](12,p,101,victim,0)==121)
check(events[12](12,p,100,nil,1)==100)
check(events[12](12,p,4294967295,victim,0)==4294967295)
p.bot=true;check(events[12](12,p,100,victim,0)==nil)
print(checks..' ruleset checks passed')
