-- The Character Advancement tree is rebuilt from the client's own spellbook:
-- Ascension_Collections/CharacterAdvancementStateCompat.lua derives each node's
-- rank from IsSpellKnown on the spell that node grants.
--
-- Two kinds of talent spell never reach that spellbook:
--   * hidden ones (SPELL_ATTR0_DO_NOT_DISPLAY), which is most passives;
--   * the base rank of a ranked spell, which the server deactivates as soon as a
--     higher rank is learned, and inactive spells are not sent at all.
--
-- Measured on a level 60 Knight of Xoroth: of 37 talents owned, the client could
-- see 4. Everything else drew as unspent after a relog, which is why builds
-- looked lost even though the server still had every spell.
--
-- The server knows the truth and now pushes it at login, unprompted: a series of
-- CoATalentSync lines carrying the spell ids this character actually owns.
-- IsSpellKnown is wrapped to include them, and everything downstream -- ranks,
-- spent points, gates -- derives from that one call, so nothing else is touched.

local ownedFromServer = {}
local activeSpecFromServer
local syncComplete = false

local function Announce(message)
    if DEFAULT_CHAT_FRAME then
        DEFAULT_CHAT_FRAME:AddMessage("|cff5ee2ffCoA Talents|r: " .. message)
    end
end

-- CharacterAdvancementStateCompat resolves a node's rank like this:
--
--     if IsSpellKnown then known = SafeCall(IsSpellKnown, spellID) end
--     if not known and IsPlayerSpell then known = SafeCall(IsPlayerSpell, spellID) end
--
-- Overriding IsSpellKnown works but taints every execution path that reaches it,
-- and the default UI calls it constantly -- that is how this addon ended up
-- blocking WorldMapBlobFrame:Hide(), a secure function.
--
-- This client has no IsPlayerSpell at all: the diagnostic returned nil for it,
-- not false. So define it. The compat layer already asks for it by name and gets
-- the answer it needs, while nothing in the stock UI has ever heard of it, which
-- means no taint to spread.
local nativeIsPlayerSpell = IsPlayerSpell

IsPlayerSpell = function(spellID, ...)
    if spellID and ownedFromServer[spellID] then
        return true
    end
    if nativeIsPlayerSpell then
        return nativeIsPlayerSpell(spellID, ...)
    end
    return false
end

local function OnSystemMessage(_, _, message, ...)
    local payload = message and message:match("^CoATalentSync:(.+)$")
    if not payload then
        return false
    end

    if payload == "end" then
        if not syncComplete then
            syncComplete = true
            local count = 0
            for _ in pairs(ownedFromServer) do
                count = count + 1
            end
            Announce(count .. " owned talent spells restored, specialization " ..
                tostring(activeSpecFromServer or ASCENSION_LOCAL_ACTIVE_SPEC_ID or "unknown") .. ".")
        end
        return true
    end

    local spec = payload:match("^spec=(%d+)$")
    if spec then
        activeSpecFromServer = tonumber(spec)
        return true
    end

    for id in payload:gmatch("%d+") do
        ownedFromServer[tonumber(id)] = true
    end
    return true                     -- consumed; keep the raw ids out of chat
end

if ChatFrame_AddMessageEventFilter then
    ChatFrame_AddMessageEventFilter("CHAT_MSG_SYSTEM", OnSystemMessage)
end

-- The login push normally arrives on its own. Only ask if it did not, which
-- covers the addon loading late or a session that reconnected without a fresh
-- login. The compat layer caches a node's rank the first time it is queried, so
-- this must not sit and wait.
local frame = CreateFrame("Frame")
frame:RegisterEvent("PLAYER_ENTERING_WORLD")
frame:SetScript("OnEvent", function(self)
    self:UnregisterEvent("PLAYER_ENTERING_WORLD")

    local waited = 0
    self:SetScript("OnUpdate", function(_, elapsed)
        if syncComplete then
            self:SetScript("OnUpdate", nil)
            return
        end

        waited = waited + elapsed
        if waited < 2 then
            return
        end
        self:SetScript("OnUpdate", nil)

        if SendChatMessage then
            SendChatMessage(".localtalentsync")
        end
    end)
end)


-- C_CharacterAdvancement.GetActiveChrSpec returns nil on this server: the
-- advancement service it belongs to does not exist, so the client never learns
-- which specialization it is on. GetMappedClassAndSpec then gives up, the picker
-- shows nothing as chosen and automatic nodes evaluate against no spec at all.
--
-- The value is known twice over -- the server sends it with the talent sync, and
-- Ascension_Collections keeps its own copy -- so answer with that when the native
-- call has nothing.
local api = C_CharacterAdvancement
if api and api.GetActiveChrSpec then
    local nativeGetActiveChrSpec = api.GetActiveChrSpec
    api.GetActiveChrSpec = function(...)
        local ok, spec = pcall(nativeGetActiveChrSpec, ...)
        if ok and spec then
            return spec
        end
        return activeSpecFromServer or ASCENSION_LOCAL_ACTIVE_SPEC_ID
    end
end

-- /coabuffs -- lists the auras currently on the player and flags the ones that
-- appear more than once. Buffs are reported as stacking when they should not,
-- but the DBC says otherwise and server and client agree on it, so the next step
-- needs the actual spell ids rather than another guess.
SLASH_COABUFFS1 = "/coabuffs"
SlashCmdList = SlashCmdList or {}
SlashCmdList["COABUFFS"] = function()
    local seen, order = {}, {}
    for i = 1, 40 do
        local name, _, _, count, _, duration, _, _, _, _, spellID = UnitAura("player", i, "HELPFUL")
        if not name then
            break
        end
        local key = tostring(spellID or name)
        if not seen[key] then
            seen[key] = { name = name, id = spellID, slots = 0, count = count or 0, duration = duration or 0 }
            table.insert(order, key)
        end
        seen[key].slots = seen[key].slots + 1
    end

    Announce("buffs on you:")
    local duplicates = 0
    for _, key in ipairs(order) do
        local aura = seen[key]
        local flag = ""
        if aura.slots > 1 then
            flag = "  <-- " .. aura.slots .. " SEPARATE ICONS"
            duplicates = duplicates + 1
        end
        DEFAULT_CHAT_FRAME:AddMessage(string.format("  %s (%s) stacks=%d dur=%ds%s",
            aura.name, tostring(aura.id), aura.count, math.floor(aura.duration), flag))
    end
    Announce(#order .. " distinct, " .. duplicates .. " duplicated.")
end

-- Shift-clicking an item in AtlasLoot produces two problems for a dot command.
--
-- First the link ends in "|r|r"; the second terminator closes nothing and the
-- client refuses the whole message with "Invalid escape code in chat message".
--
-- Then the link itself is the short form, "|Hitem:50363|h[Name]|h|r" with none
-- of the trailing item fields. The server's hyperlink parser requires the full
-- set and answers "Provided link has invalid link data".
--
-- Commands take a bare item id just as happily, so reduce any item link in a dot
-- command to its number and leave ordinary chat untouched.
local nativeSendChatMessage = SendChatMessage
if nativeSendChatMessage then
    SendChatMessage = function(message, ...)
        if type(message) == "string" then
            while message:find("|r|r", 1, true) do
                message = message:gsub("|r|r", "|r")
            end

            if message:sub(1, 1) == "." then
                message = message:gsub("|c%x+|Hitem:(%d+)[^|]*|h%[.-%]|h|r", "%1")
                message = message:gsub("|Hitem:(%d+)[^|]*|h%[.-%]|h", "%1")
            end
        end
        return nativeSendChatMessage(message, ...)
    end
end
