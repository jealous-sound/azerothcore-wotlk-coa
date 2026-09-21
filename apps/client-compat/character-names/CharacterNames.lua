-- Loaded in both GlueXML and FrameXML. The server remains the naming authority.
CoACharacterNames = {}
local Names = CoACharacterNames
Names.maxLetters = 25
Names.maxBytes = 47 -- The native client's name buffer includes a terminating zero.

local function Encode(code)
  if code < 128 then return string.char(code) end
  if code < 2048 then return string.char(192 + math.floor(code / 64), 128 + code % 64) end
  return string.char(224 + math.floor(code / 4096), 128 + math.floor(code / 64) % 64, 128 + code % 64)
end

-- Match the server's supported Latin/Cyrillic case mapping without splitting UTF-8 bytes.
local function Lower(code)
  if (code >= 65 and code <= 90) or (code >= 192 and code <= 214)
    or (code >= 216 and code <= 222) or (code >= 1040 and code <= 1071) then
    return code + 32
  end
  if code >= 256 and code <= 302 and code % 2 == 0 then return code + 1 end
  if code == 7838 then return 223 end
  if code == 1025 then return 1105 end
  return code
end

local function Upper(code)
  if (code >= 97 and code <= 122) or (code >= 224 and code <= 246)
    or (code >= 248 and code <= 254) or (code >= 1072 and code <= 1103) then
    return code - 32
  end
  if code >= 257 and code <= 303 and code % 2 == 1 then return code - 1 end
  if code == 223 then return 7838 end
  if code == 1105 then return 1025 end
  return code
end

function Names.Normalize(name)
  local first = true
  return (name:gsub("[%z\1-\127\194-\244][\128-\191]*", function(char)
    if char == " " then first = true; return char end
    local a, b, c = char:byte(1, 3)
    local code = a
    if #char == 2 then code = (a - 192) * 64 + b - 128 end
    if #char == 3 then code = (a - 224) * 4096 + (b - 128) * 64 + c - 128 end
    if #char > 3 then first = false; return char end
    code = Lower(code)
    if first then code = Upper(code) end
    first = false
    return Encode(code)
  end))
end

function Names.Quote(name)
  if name:find(" ", 1, true) then return '"' .. name .. '"' end
  return name
end

-- A quoted full name has an explicit boundary before a message or friend note.
function Names.ExtractRecipient(text)
  if text:sub(1, 1) == '"' then
    local last = text:find('"', 2, true)
    if not last or last == 2 then return nil end
    local tail = text:sub(last + 1)
    if tail ~= "" and not tail:match("^%s") then return nil end
    return text:sub(2, last - 1), (tail:gsub("^%s+", ""))
  end
  return text:match("^%s*([^%s]+)%s*(.*)")
end

function Names.CreationName()
  local first = CharacterCreateNameEdit:GetText():match("^%s*(.-)%s*$")
  local second = CharacterCreateSecondNameEdit:GetText():match("^%s*(.-)%s*$")
  return Names.Normalize(first .. (second ~= "" and " " .. second or ""))
end

function Names.CreateCharacter(...)
  local name = Names.CreationName()
  if #name > Names.maxBytes then
    GlueDialog_Show("OKAY", CHAR_NAME_TOO_LONG)
    return
  end
  return CreateCharacter(name, ...)
end

function Names.SetCreationName(name)
  local first, second = name:match("^([^ ]*) ?(.*)$")
  CharacterCreateNameEdit:SetText(first or "")
  CharacterCreateSecondNameEdit:SetText(second or "")
end

function Names.InitCreation()
  if CharacterCreateSecondNameEdit then return end
  local first = CharacterCreateNameEdit
  first:ClearAllPoints()
  first:SetPoint("BOTTOM", first:GetParent(), "BOTTOM", -106, 22)
  first:SetMaxBytes(Names.maxBytes)
  CharacterCreateFirstNameLabel:ClearAllPoints()
  CharacterCreateFirstNameLabel:SetPoint("BOTTOM", first, "TOP", 0, 1)
  CharacterCreateNameEditHelpText:ClearAllPoints()
  CharacterCreateNameEditHelpText:SetPoint("BOTTOM", first, "TOP", 106, 20)
  CharacterCreateNameEditHelpText:SetSize(408, 48)
  local second = CreateFrame("EditBox", "CharacterCreateSecondNameEdit", first)
  second:SetSize(196, 42)
  second:SetPoint("LEFT", first, "RIGHT", 16, 0)
  second:SetAutoFocus(false)
  second:SetMaxLetters(12)
  second:SetMaxBytes(Names.maxBytes)
  second:SetFontObject("GlueEditBoxFont")
  second:SetTextInsets(15, 10, 0, 0)
  second:SetBackdrop(first:GetBackdrop())
  second:SetBackdropColor(first:GetBackdropColor())
  second:SetBackdropBorderColor(first:GetBackdropBorderColor())
  local label = second:CreateFontString(nil, "ARTWORK", "GlueFontNormalSmall")
  label:SetPoint("BOTTOM", second, "TOP", 0, 1)
  label:SetText(GetLocale() == "ruRU" and "Второе имя (необязательно)" or "Second name (optional)")
  first:SetScript("OnTabPressed", function() second:SetFocus() end)
  second:SetScript("OnTabPressed", function() first:SetFocus() end)
  second:SetScript("OnEscapePressed", function() CharacterCreate_Back() end)
  second:SetScript("OnEnterPressed", function() CharacterCreate_Forward() end)
  CharacterCreateRandomName:ClearAllPoints()
  CharacterCreateRandomName:SetPoint("LEFT", second, "RIGHT", 4, 13)
end

COA_FIRST_NAME = GetLocale() == "ruRU" and "Имя" or "First name"
