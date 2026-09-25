-- Native ruleset spells for an Ascension client and AzerothCore with ALE.
-- Inventory losses and chest transactions are handled by mod-highrisk.
HighRisk = HighRisk or {}
local Rulesets = {}
HighRisk.Rulesets = Rulesets

local modes = {[84422] = 1, [84420] = 2, [84421] = 3}
local names = {[1] = "PvE Mode", [2] = "War Mode", [3] = "High-Risk"}
local auras = {1004019, 1004119, 9931032}
local states, timers = {}, {}
local PLAYER_FLAGS = 0x96
local RESTING = 0x20
local MINIMUM_LEVEL = 15
local LEVEL_CAP = 60
local KILL_XP_BONUS = 20
local OPENING_SPELL = 22810

local function isOpenWorld(player)
    local map = player:GetMap()
    return not (map:IsDungeon() or map:IsRaid() or map:IsBattleground() or map:IsArena())
end

local function isResting(player)
    -- Banked rested XP does not mean the player is currently in a rest area.
    return player:HasFlag(PLAYER_FLAGS, RESTING)
end

function Rulesets.Experience(player, amount, victim, source)
    local state = states[player:GetGUIDLow()]
    if source ~= 0 or not victim or amount <= 0 or not state or state.mode ~= 3 then
        return amount
    end
    if player:GetLevel() >= LEVEL_CAP or not player:IsAlive()
        or not player:HasAura(1004019) or not isOpenWorld(player) then
        return amount
    end
    return amount * (1 + KILL_XP_BONUS / 100)
end

function Rulesets.Eligible(player)
    if player:GetLevel() < MINIMUM_LEVEL then
        return false, "Risk modes become available at level 15."
    end
    if player:IsDead() or player:IsInCombat() or player:IsFlying() then
        return false, "You cannot change risk modes while dead, flying, or in combat."
    end
    if not isOpenWorld(player) or not isResting(player) then
        return false, "You can change risk modes in a capital city or another rested area."
    end
    return true
end

function Rulesets.Learn(player)
    if player:GetLevel() < MINIMUM_LEVEL then return end
    for spell in pairs(modes) do
        if not player:HasSpell(spell) then player:LearnSpell(spell) end
    end
    if not player:HasSpell(OPENING_SPELL) then player:LearnSpell(OPENING_SPELL) end
end

function Rulesets.Refresh(player)
    local state = states[player:GetGUIDLow()]
    if not state then return end
    local wanted = {
        [1004019] = state.mode == 3,
        [1004119] = state.mode == 1 or state.mode == 2,
        [9931032] = state.mode == 1,
    }
    for _, aura in ipairs(auras) do
        if wanted[aura] then
            if player:IsAlive() and not player:HasAura(aura) then player:AddAura(aura, player) end
        elseif player:HasAura(aura) then
            player:RemoveAura(aura)
        end
    end
    if isOpenWorld(player) then
        local ffa = state.mode == 3 and not isResting(player)
        if player:IsFFAPvP() ~= ffa then player:SetFFA(ffa) end
        if player:IsPvPFlagged() ~= (state.mode ~= 1) then player:SetPvP(state.mode ~= 1) end
        state.outdoors = true
    elseif state.outdoors then
        player:SetFFA(player:GetMap():IsArena())
        state.outdoors = false
    end
end

function Rulesets.Cast(_, player, spell)
    local mode = modes[spell:GetEntry()]
    if not mode or not player:HasSpell(spell:GetEntry()) then return end
    local eligible, reason = Rulesets.Eligible(player)
    if not eligible then
        spell:Cancel()
        player:SendBroadcastMessage(reason)
        return
    end
    local guid = player:GetGUIDLow()
    if states[guid] and states[guid].mode == mode then
        Rulesets.Refresh(player)
        return
    end
    CharDBExecute(string.format(
        "INSERT INTO highrisk_ruleset (guid, mode) VALUES (%u,%u) ON DUPLICATE KEY UPDATE mode=VALUES(mode)",
        guid, mode))
    states[guid] = {mode = mode}
    Rulesets.Refresh(player)
    player:SendBroadcastMessage(names[mode] .. " enabled.")
    if mode == 3 then
        player:SendBroadcastMessage("When losses are enabled, any open-world death can leave tradeable gear, resources and gold in a chest.")
        player:SendBroadcastMessage("All High-Risk players can recover it, including you. Fel Commutation is not implemented.")
    end
end

function Rulesets.Login(_, player)
    if player:IsBot() then return end
    Rulesets.Learn(player)
    local guid = player:GetGUIDLow()
    local result = CharDBQuery(string.format("SELECT mode FROM highrisk_ruleset WHERE guid=%u", guid))
    if result then
        local mode = result:GetUInt32(0)
        if names[mode] then
            states[guid] = {mode = mode}
            Rulesets.Refresh(player)
        end
    end
    if timers[guid] then player:RemoveEventById(timers[guid]) end
    timers[guid] = player:RegisterEvent(function(_, _, _, owner)
        Rulesets.Refresh(owner)
    end, 1000, 0)
end

-- Register this callback only once. Integrate it into any existing XP pipeline.
RegisterPlayerEvent(12, function(_, player, amount, victim, source)
    if player:IsBot() then return end
    return math.min(4294967295, math.floor(Rulesets.Experience(player, amount, victim, source)))
end)
RegisterPlayerEvent(3, Rulesets.Login)
RegisterPlayerEvent(4, function(_, player)
    local guid = player:GetGUIDLow()
    if timers[guid] then player:RemoveEventById(timers[guid]) end
    states[guid], timers[guid] = nil, nil
end)
RegisterPlayerEvent(5, Rulesets.Cast)
RegisterPlayerEvent(13, function(_, player) Rulesets.Learn(player) end)
RegisterPlayerEvent(27, function(_, player) Rulesets.Refresh(player) end)
RegisterPlayerEvent(28, function(_, player) Rulesets.Refresh(player) end)
RegisterPlayerEvent(36, function(_, player) Rulesets.Refresh(player) end)
print("[High Risk] Native rulesets ready; open-world kill XP bonus: " .. KILL_XP_BONUS .. "%")
