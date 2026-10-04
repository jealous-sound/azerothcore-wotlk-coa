-- SPDX-License-Identifier: GPL-2.0-or-later

HomebrewNourishment = HomebrewNourishment or {}
local HBN = HomebrewNourishment

local PREFIX = "HBN"
local PROTOCOL = "3"
local SEPARATOR = "~"

HBN.connected = false
HBN.lastHello = 0
HBN.profiles = HBN.profiles or {}
HBN.pending = HBN.pending or {}
HBN.activeSlots = HBN.activeSlots or {}
HBN.completionSeconds = 10

local function chat(message)
    if DEFAULT_CHAT_FRAME then
        DEFAULT_CHAT_FRAME:AddMessage("|cff55dd88Nourishment:|r " .. tostring(message or ""))
    end
end

local function trim(value)
    return (string.gsub(string.gsub(value or "", "^%s+", ""), "%s+$", ""))
end

local function decode(value)
    return (tostring(value or ""):gsub("%%(%x%x)", function(hex)
        return string.char(tonumber(hex, 16) or 0)
    end))
end

local function split(value)
    local fields = {}
    local start = 1
    value = value or ""
    while true do
        local position = string.find(value, SEPARATOR, start, true)
        if not position then
            table.insert(fields, string.sub(value, start))
            return fields
        end
        table.insert(fields, string.sub(value, start, position - 1))
        start = position + 1
    end
end

local function itemIdFromLink(link)
    return tonumber(string.match(link or "", "item:(%d+):"))
end

local function gradeName(grade)
    return tonumber(grade or 0) == 1 and "Premium" or "Ordinary"
end

function HBN:Send(opcode, payload)
    if type(SendAddonMessage) ~= "function" then return false end
    local player = UnitName("player")
    if not player or player == "" then return false end
    local message = trim(opcode)
    if payload and payload ~= "" then message = message .. SEPARATOR .. payload end
    SendAddonMessage(PREFIX, message, "WHISPER", player)
    return true
end

function HBN:Start()
    if type(RegisterAddonMessagePrefix) == "function" then
        RegisterAddonMessagePrefix(PREFIX)
    end
    self.connected = false
    self.lastHello = GetTime() or 0
    self:Send("HELLO", PROTOCOL)
end

function HBN:RequestProfile(itemId)
    itemId = tonumber(itemId)
    if not itemId or self.profiles[itemId] ~= nil or self.pending[itemId] then return end
    self.pending[itemId] = true
    self:Send("QUERY", tostring(itemId))
end

local function parseProfile(fields)
    return {
        itemId = tonumber(fields[1] or "0") or 0,
        itemName = decode(fields[2]),
        family = fields[3] or "",
        label = decode(fields[4]),
        tier = tonumber(fields[5] or "0") or 0,
        grade = tonumber(fields[6] or "0") or 0,
        markerSpell = tonumber(fields[7] or "0") or 0,
        amount1 = tonumber(fields[8] or "0") or 0,
        amount2 = tonumber(fields[9] or "0") or 0,
        amount3 = tonumber(fields[10] or "0") or 0,
        duration = tonumber(fields[11] or "0") or 0,
        effect = decode(fields[12]),
        cookedBonusPercent = tonumber(fields[13] or "0") or 0,
    }
end

local function parseSlot(fields)
    local slot = tonumber(fields[1] or "0") or 0
    if slot < 1 or slot > 3 or tonumber(fields[2] or "0") == 0 then return slot, nil end
    return {
        slot = slot,
        itemId = tonumber(fields[2] or "0") or 0,
        itemName = decode(fields[3]),
        family = fields[4] or "",
        label = decode(fields[5]),
        tier = tonumber(fields[6] or "0") or 0,
        grade = tonumber(fields[7] or "0") or 0,
        markerSpell = tonumber(fields[8] or "0") or 0,
        amount1 = tonumber(fields[9] or "0") or 0,
        amount2 = tonumber(fields[10] or "0") or 0,
        amount3 = tonumber(fields[11] or "0") or 0,
        expiresAt = tonumber(fields[12] or "0") or 0,
        effect = decode(fields[13]),
        cookedBonusPercent = tonumber(fields[14] or "0") or 0,
    }, slot
end

function HBN:AddItemTooltip(tooltip, profile)
    if not tooltip or not profile or tooltip.__hbnItemId == profile.itemId then return end
    tooltip.__hbnItemId = profile.itemId
    tooltip:AddLine(" ")
    tooltip:AddLine("Homebrew Nourishment", 0.33, 0.95, 0.55)
    tooltip:AddLine(profile.label .. " - Tier " .. profile.tier .. " " .. gradeName(profile.grade), 1.0, 0.82, 0.25)
    tooltip:AddLine(profile.effect, 0.9, 0.9, 0.9, true)
    if profile.cookedBonusPercent > 0 then
        tooltip:AddLine("Cooked recipe: +" .. profile.cookedBonusPercent .. "% Nourishment stats", 0.6, 1.0, 0.6, true)
    end
    local minutes = math.floor((profile.duration or 0) / 60)
    tooltip:AddLine("Eat or drink uninterrupted for " .. tostring(HBN.completionSeconds) .. " seconds. Lasts " .. minutes .. " minutes.", 0.65, 0.75, 0.9, true)
    tooltip:AddLine("Finish eating or drinking for 10 seconds to earn a lasting effect. Three different effects fit; active effects cannot be refreshed or replaced.", 0.65, 0.75, 0.9, true)
    tooltip:Show()
end

function HBN:OnItemTooltip(tooltip)
    local _, link = tooltip:GetItem()
    local itemId = itemIdFromLink(link)
    if not itemId then return end
    local profile = self.profiles[itemId]
    if profile == false then return end
    if profile then
        self:AddItemTooltip(tooltip, profile)
    else
        self:RequestProfile(itemId)
    end
end

function HBN:OnAuraTooltip(tooltip, unit, index, filter)
    if tooltip.__hbnReplacingAura then return end
    local name, _, _, count, _, duration, expirationTime, _, _, _, spellId = UnitBuff(unit, index, filter)
    local active
    for slot = 1, 3 do
        local candidate = self.activeSlots[slot]
        if candidate and tonumber(spellId or 0) == tonumber(candidate.markerSpell or 0) then
            active = candidate
            break
        end
    end
    if not active then return end

    -- These client-known marker spells have a baked-in 3.3.5a description
    -- (often +40) that does not follow server-selected base points. Rebuild
    -- only our active marker tooltip from authoritative server state.
    tooltip.__hbnReplacingAura = true
    tooltip:ClearLines()
    tooltip:AddLine(name or "Well Fed", 1.0, 1.0, 1.0)
    tooltip:AddLine(active.label .. " - Tier " .. active.tier .. " " .. gradeName(active.grade), 1.0, 0.82, 0.25)
    tooltip:AddLine(active.effect, 1.0, 1.0, 1.0, true)
    if active.cookedBonusPercent > 0 then
        tooltip:AddLine("Cooked recipe: +" .. active.cookedBonusPercent .. "% Nourishment stats", 0.6, 1.0, 0.6, true)
    end
    if tonumber(count or 0) > 1 then
        tooltip:AddLine("Stacks: " .. tostring(count), 0.75, 0.75, 0.75)
    end
    local remaining = math.max(0, math.floor((tonumber(expirationTime or 0) - (GetTime() or 0)) + 0.5))
    if tonumber(duration or 0) > 0 and remaining > 0 then
        local minutes = math.floor(remaining / 60)
        -- WoW 3.3.5a's embedded Lua does not expose math.mod.
        local seconds = remaining - (minutes * 60)
        if minutes > 0 then
            tooltip:AddLine(string.format("Time remaining: %d:%02d", minutes, seconds), 0.75, 0.85, 1.0)
        else
            tooltip:AddLine("Time remaining: " .. seconds .. " sec", 0.75, 0.85, 1.0)
        end
    end
    tooltip:AddLine("Meal slot " .. tostring(active.slot) .. " of 3. Different meals and drinks remain active together.", 0.65, 0.75, 0.9, true)
    tooltip:Show()
    tooltip.__hbnReplacingAura = nil
end

function HBN:OnMessage(prefix, message)
    if prefix ~= PREFIX then return false end
    local fields = split(message or "")
    local opcode = string.upper(trim(table.remove(fields, 1) or ""))
    if opcode == "HELLO_ACK" then
        if tostring(fields[1] or "") ~= PROTOCOL then
            self.connected = false
            chat("Server protocol " .. tostring(fields[1] or "unknown") .. " is not compatible with addon protocol " .. PROTOCOL .. ".")
            return true
        end
        self.completionSeconds = tonumber(fields[2] or "10") or 10
        self.connected = true
        self:Send("STATUS")
        return true
    elseif opcode == "STATE" then
        self.activeSlots = {}
        return true
    elseif opcode == "SLOT" then
        local active, slot = parseSlot(fields)
        if slot >= 1 and slot <= 3 then self.activeSlots[slot] = active end
        return true
    elseif opcode == "PROFILE" then
        local itemId = tonumber(fields[1] or "0") or 0
        self.pending[itemId] = nil
        if fields[2] == "0" then
            self.profiles[itemId] = false
            return true
        end
        local profile = parseProfile(fields)
        self.profiles[profile.itemId] = profile
        if GameTooltip and GameTooltip:IsShown() then
            local _, link = GameTooltip:GetItem()
            if itemIdFromLink(link) == profile.itemId then self:AddItemTooltip(GameTooltip, profile) end
        end
        return true
    end
    return false
end

local events = CreateFrame("Frame")
events:RegisterEvent("PLAYER_LOGIN")
events:RegisterEvent("PLAYER_ENTERING_WORLD")
events:RegisterEvent("CHAT_MSG_ADDON")
events:SetScript("OnEvent", function(_, event, ...)
    if event == "PLAYER_LOGIN" then
        HBN:Start()
    elseif event == "PLAYER_ENTERING_WORLD" then
        HBN:Send("STATUS")
    elseif event == "CHAT_MSG_ADDON" then
        local prefix, message = ...
        HBN:OnMessage(prefix, message)
    end
end)
events:SetScript("OnUpdate", function()
    if HBN.connected then return end
    local now = GetTime() or 0
    if now - (HBN.lastHello or 0) >= 5 then
        HBN.lastHello = now
        HBN:Send("HELLO", PROTOCOL)
    end
end)

if GameTooltip then
    GameTooltip:HookScript("OnTooltipSetItem", function(tooltip) HBN:OnItemTooltip(tooltip) end)
    GameTooltip:HookScript("OnTooltipCleared", function(tooltip) tooltip.__hbnItemId = nil end)
    if type(hooksecurefunc) == "function" then
        -- The stock 3.3.5a player BuffFrame uses SetUnitAura. Unit-frame and
        -- addon paths may use SetUnitBuff instead, so cover both entry points.
        if type(GameTooltip.SetUnitAura) == "function" then
            hooksecurefunc(GameTooltip, "SetUnitAura", function(tooltip, unit, index, filter)
                HBN:OnAuraTooltip(tooltip, unit, index, filter)
            end)
        end
        hooksecurefunc(GameTooltip, "SetUnitBuff", function(tooltip, unit, index, filter)
            HBN:OnAuraTooltip(tooltip, unit, index, filter)
        end)
    end
end

SLASH_HOMEBREWNOURISHMENT1 = "/nourishment"
SLASH_HOMEBREWNOURISHMENT2 = "/mealbuff"
SlashCmdList["HOMEBREWNOURISHMENT"] = function()
    HBN:Send("STATUS")
    local any = false
    for slot = 1, 3 do
        local active = HBN.activeSlots[slot]
        if active then
            any = true
            chat("Slot " .. slot .. ": " .. active.label .. " tier " .. active.tier .. " " .. gradeName(active.grade) .. ": " .. active.effect)
        end
    end
    if not any then
        chat("No lasting meal effect is active.")
    end
end
