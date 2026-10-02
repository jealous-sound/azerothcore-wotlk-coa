-- Just enough of the 3.3.5 client to run ChatEmoji.lua outside the game. Widgets only have methods the real client has,
-- so a retail-only call fails here the way it would in game. The chat edit box functions follow the client's
-- ChatFrame.lua and AutoComplete.lua call order: the Enter, Tab and Escape handlers ask AutoCompleteEditBox_On*Pressed
-- first and stop when it returns true.

STUB = {
    sent = {}, tabFellThrough = 0, shift = false, tooltip = {}, sounds = {}, frames = {}, listeners = {},
}

local function fire(frame, name, ...)
    local script = frame.scripts[name]
    if script then
        script(frame, ...)
    end
    for _, hook in ipairs(frame.hooks[name] or {}) do
        hook(frame, ...)
    end
end

local Region = {}
Region.__index = Region
Region.objectTypes = { Region = true }
function Region:IsObjectType(kind) return self.objectTypes[kind] == true end
function Region:SetWidth(width) self.width = width end
function Region:SetHeight(height) self.height = height end
function Region:GetWidth() return self.width or 0 end
function Region:GetHeight() return self.height or 0 end
function Region:ClearAllPoints() self.points = {} end
function Region:SetPoint(point, relativeTo, relativePoint, x, y)
    self.points[point] = {
        relativeTo = relativeTo or self.parent, relativePoint = relativePoint or point, x = x or 0, y = y or 0,
    }
end
function Region:Show() self.shown = true end
function Region:Hide() self.shown = false end
function Region:IsShown() return self.shown end

local Texture = setmetatable({}, { __index = Region })
Texture.__index = Texture
Texture.objectTypes = { Texture = true, Region = true }
function Texture:SetTexture(path) self.texture = path end
function Texture:SetVertexColor(r, g, b, a) self.vertexColor = { r, g, b, a } end
function Texture:GetTexture() return self.texture end

local FontString = setmetatable({}, { __index = Region })
FontString.__index = FontString
FontString.objectTypes = { FontString = true, Region = true }
function FontString:SetText(text)
    self.text = text
    self.setCount = (self.setCount or 0) + 1
end
function FontString:GetText() return self.text or "" end

local function newRegion(class, parent)
    return setmetatable({ parent = parent, points = {}, shown = true }, class)
end

local Frame = setmetatable({}, { __index = Region })
Frame.__index = Frame
Frame.objectTypes = { Frame = true, Region = true }
function Frame:GetName() return self.name end
function Frame:GetParent() return self.parent end
function Frame:SetFrameStrata(strata) self.strata = strata end
function Frame:SetFrameLevel(level) self.level = level end
function Frame:GetFrameLevel() return self.level end
function Frame:SetAlpha(alpha) self.alpha = alpha end
function Frame:GetAlpha() return self.alpha end
function Frame:EnableMouse(enabled) self.mouse = enabled end
function Frame:EnableMouseWheel(enabled) self.wheel = enabled end
function Frame:SetClampedToScreen(clamped) self.clamped = clamped end
function Frame:SetBackdrop(info)
    assert(type(info) == "table" or info == nil, "SetBackdrop needs a table")
    self.backdrop = info
end
function Frame:SetBackdropColor(r, g, b, a) self.backdropColor = { r, g, b, a } end
function Frame:SetBackdropBorderColor(r, g, b, a) self.backdropBorderColor = { r, g, b, a } end
local LAYERS = { BACKGROUND = true, BORDER = true, ARTWORK = true, OVERLAY = true, HIGHLIGHT = true }
local function addRegion(frame, class, layer)
    assert(layer == nil or LAYERS[layer], "unknown draw layer " .. tostring(layer))
    local region = newRegion(class, frame)
    table.insert(frame.regions, region)
    return region
end
function Frame:CreateTexture(name, layer) return addRegion(self, Texture, layer) end
function Frame:CreateFontString(name, layer, inherits)
    assert(inherits == nil or _G[inherits] ~= nil, "unknown font object " .. tostring(inherits))
    return addRegion(self, FontString, layer)
end
function Frame:GetNumRegions() return #self.regions end
function Frame:GetRegions() return unpack(self.regions) end
function Frame:GetNumChildren() return #self.children end
function Frame:GetChildren() return unpack(self.children) end
function Frame:IsVisible() return self.shown and (self.parent == nil or self.parent:IsVisible()) end
function Frame:RegisterEvent(event)
    assert(type(event) == "string", "RegisterEvent needs an event name")
    if not self.events[event] then
        self.events[event] = true
        STUB.listeners[event] = STUB.listeners[event] or {}
        table.insert(STUB.listeners[event], self)
    end
end
function Frame:SetScript(name, script) self.scripts[name] = script end
function Frame:GetScript(name) return self.scripts[name] end
function Frame:HookScript(name, hook)
    self.hooks[name] = self.hooks[name] or {}
    table.insert(self.hooks[name], hook)
end
function Frame:Show()
    if not self.shown then
        self.shown = true
        fire(self, "OnShow")
    end
end
function Frame:Hide()
    if self.shown then
        self.shown = false
        fire(self, "OnHide")
    end
end

local Button = setmetatable({}, { __index = Frame })
Button.__index = Button
Button.objectTypes = { Button = true, Frame = true, Region = true }
function Button:SetNormalTexture(path) self.normalTexture = path end
function Button:SetPushedTexture(path) self.pushedTexture = path end
function Button:SetHighlightTexture(path, mode)
    self.highlightTexture = path
    self.highlightMode = mode
end
function Button:LockHighlight() self.highlightLocked = true end
function Button:UnlockHighlight() self.highlightLocked = false end
function Button:Click() fire(self, "OnClick") end

local EditBox = setmetatable({}, { __index = Frame })
EditBox.__index = EditBox
EditBox.objectTypes = { EditBox = true, Frame = true, Region = true }
function EditBox:SetAutoFocus(enabled) self.autoFocus = enabled end
function EditBox:SetFontObject(font) assert(font ~= nil, "SetFontObject needs a font object") self.font = font end
function EditBox:SetMaxLetters(count) self.maxLetters = count end
function EditBox:SetTextInsets(l, r, t, b) self.insets = { l, r, t, b } end
function EditBox:GetTextInsets() return unpack(self.insets) end
function EditBox:GetText() return self.text end
function EditBox:GetCursorPosition() return self.cursor end
function EditBox:SetCursorPosition(position) self.cursor = math.max(0, math.min(position, #self.text)) end
function EditBox:SetText(text)
    self.text = text
    self.cursor = #text
    fire(self, "OnTextChanged", false)
end
function EditBox:Insert(text)
    self.text = self.text:sub(1, self.cursor) .. text .. self.text:sub(self.cursor + 1)
    self.cursor = self.cursor + #text
    fire(self, "OnTextChanged", false)
end
function EditBox:HasFocus() return self.focused end
function EditBox:SetFocus()
    if not self.focused then
        self.focused = true
        fire(self, "OnEditFocusGained")
    end
end
function EditBox:ClearFocus()
    if self.focused then
        self.focused = false
        fire(self, "OnEditFocusLost")
    end
end
-- Test helper, not client API: what the player does by typing.
function EditBox:_type(text, cursor)
    self.text = text
    self.cursor = cursor or #text
    fire(self, "OnTextChanged", true)
end

local Slider = setmetatable({}, { __index = Frame })
Slider.__index = Slider
Slider.objectTypes = { Slider = true, Frame = true, Region = true }
function Slider:SetOrientation(orientation) self.orientation = orientation end
function Slider:SetValueStep(step) self.step = step end
function Slider:SetThumbTexture(path) self.thumb = newRegion(Texture, self) self.thumb.texture = path end
function Slider:GetThumbTexture() return self.thumb end
function Slider:GetMinMaxValues() return self.min, self.max end
function Slider:GetValue() return self.value end
function Slider:SetValue(value)
    value = math.max(self.min, math.min(value, self.max))
    if value ~= self.value then
        self.value = value
        fire(self, "OnValueChanged", value)
    end
end
function Slider:SetMinMaxValues(min, max)
    self.min, self.max = min, max
    self:SetValue(self.value)
end

local CLASSES = { Frame = Frame, Button = Button, EditBox = EditBox, Slider = Slider }
local KNOWN_TEMPLATES = { UIPanelCloseButton = true }

function CreateFrame(kind, name, parent, template)
    local class = assert(CLASSES[kind], "unsupported frame type " .. tostring(kind))
    assert(template == nil or KNOWN_TEMPLATES[template], "unknown template " .. tostring(template))
    local frame = setmetatable({
        name = name, parent = parent, points = {}, scripts = {}, hooks = {}, shown = true, level = 1,
        text = "", cursor = 0, insets = { 0, 0, 0, 0 }, min = 0, max = 100, value = 0,
        regions = {}, children = {}, events = {},
    }, class)
    if name then
        _G[name] = frame
    end
    if parent then
        table.insert(parent.children, frame)
    end
    table.insert(STUB.frames, frame)
    return frame
end

-- The client runs OnUpdate for frames that are visible, which is a shown frame under shown parents.
function STUB.nextFrame(elapsed)
    for _, frame in ipairs(STUB.frames) do
        if frame:IsVisible() and frame.scripts.OnUpdate then
            fire(frame, "OnUpdate", elapsed)
        end
    end
end

-- Events reach every frame that registered for them, shown or not.
function STUB.fireEvent(event, ...)
    for _, frame in ipairs(STUB.listeners[event] or {}) do
        fire(frame, "OnEvent", event, ...)
    end
end

UIParent = CreateFrame("Frame", "UIParent")
WorldFrame = CreateFrame("Frame", "WorldFrame")

-- A speech bubble the way the engine makes one: an unnamed WorldFrame child with the ChatBubble textures and one
-- FontString holding the raw message. The order of the regions is not something the code may depend on.
function STUB.newBubble(message, fontStringFirst)
    local bubble = CreateFrame("Frame", nil, WorldFrame)
    local text
    local function addText()
        text = bubble:CreateFontString(nil, "OVERLAY")
        text:SetText(message)
    end
    if fontStringFirst then
        addText()
    end
    bubble:CreateTexture(nil, "BORDER"):SetTexture("Interface\\Tooltips\\ChatBubble-Backdrop")
    bubble:CreateTexture(nil, "BACKGROUND"):SetTexture("Interface\\Tooltips\\ChatBubble-Background")
    bubble:CreateTexture(nil, "ARTWORK"):SetTexture("Interface\\Tooltips\\ChatBubble-Tail")
    if not fontStringFirst then
        addText()
    end
    return bubble, text
end
UISpecialFrames = {}
ChatFontNormal = {}
GameFontHighlightSmall = {}
GameFontDisableSmall = {}
tinsert = table.insert
function strlenutf8(text) return select(2, text:gsub("[^\128-\191]", "")) end
function IsShiftKeyDown() return STUB.shift end
function PlaySound(name)
    assert(type(name) == "string", "PlaySound needs a sound name")
    table.insert(STUB.sounds, name)
end

GameTooltip = {}
function GameTooltip:SetOwner(owner, anchor) STUB.tooltip = { owner = owner, lines = {} } end
function GameTooltip:SetText(text) STUB.tooltip.title = text end
function GameTooltip:AddLine(text) table.insert(STUB.tooltip.lines, text) end
function GameTooltip:Show() STUB.tooltip.shown = true end
function GameTooltip:Hide() STUB.tooltip.shown = false end

function hooksecurefunc(name, hook)
    local original = assert(_G[name], "Attempt to hook a nonexistent function " .. name)
    _G[name] = function(...)
        local results = { original(...) }
        hook(...)
        return unpack(results)
    end
end

-- The filter loop of ChatFrame_MessageEventHandler: each filter gets all twelve args and may replace all of them.
local chatFilters = {}
function ChatFrame_AddMessageEventFilter(event, filter)
    assert(event and filter)
    chatFilters[event] = chatFilters[event] or {}
    for _, existing in ipairs(chatFilters[event]) do
        if existing == filter then
            return
        end
    end
    table.insert(chatFilters[event], filter)
end

function STUB.runFilters(event, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12)
    local filter
    local new1, new2, new3, new4, new5, new6, new7, new8, new9, new10, new11, new12
    for _, filterFunc in next, chatFilters[event] or {} do
        filter, new1, new2, new3, new4, new5, new6, new7, new8, new9, new10, new11, new12 =
            filterFunc(nil, event, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12)
        if filter then
            return true
        elseif new1 then
            arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12 =
                new1, new2, new3, new4, new5, new6, new7, new8, new9, new10, new11, new12
        end
    end
    return false, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12
end

function STUB.filterCount(event) return #(chatFilters[event] or {}) end

-- Name completion and chat edit box handlers, in the order the client calls them.
function AutoCompleteEditBox_OnTabPressed(editBox) return false end
function AutoCompleteEditBox_OnEnterPressed(editBox) return false end
function AutoCompleteEditBox_OnEscapePressed(editBox) return false end

function ChatEdit_OnTabPressed(self)
    if not AutoCompleteEditBox_OnTabPressed(self) then
        STUB.tabFellThrough = STUB.tabFellThrough + 1
    end
end

function ChatEdit_OnEscapePressed(editBox)
    if not AutoCompleteEditBox_OnEscapePressed(editBox) then
        editBox:SetText("")
        editBox:Hide()
    end
end

function ChatEdit_OnEnterPressed(self)
    if AutoCompleteEditBox_OnEnterPressed(self) then
        return
    end
    table.insert(STUB.sent, self:GetText())
    ChatEdit_OnEscapePressed(self)
end

function ChatEdit_OnTextChanged(self, userInput) end
function ChatEdit_OnEditFocusLost(self) end
function ChatEdit_OnHide(self) end

function ChatEdit_UpdateHeader(editBox)
    editBox:SetTextInsets(15 + 40, 13, 0, 0)
end

function ChatEdit_ActivateChat(editBox)
    editBox:Show()
    editBox:SetFocus()
    ChatEdit_UpdateHeader(editBox)
end

-- The XML scripts of ChatFrameEditBoxTemplate call these globals by name when the event happens.
function STUB.newChatEditBox(index)
    local editBox = CreateFrame("EditBox", "ChatFrame" .. index .. "EditBox", UIParent)
    editBox:SetScript("OnTextChanged", function(self, userInput) ChatEdit_OnTextChanged(self, userInput) end)
    editBox:SetScript("OnEditFocusLost", function(self) ChatEdit_OnEditFocusLost(self) end)
    editBox:SetScript("OnHide", function(self) ChatEdit_OnHide(self) end)
    editBox:SetScript("OnTabPressed", function(self) ChatEdit_OnTabPressed(self) end)
    editBox:SetScript("OnEnterPressed", function(self) ChatEdit_OnEnterPressed(self) end)
    editBox:SetScript("OnEscapePressed", function(self) ChatEdit_OnEscapePressed(self) end)
    ChatEdit_UpdateHeader(editBox)
    return editBox
end

NUM_CHAT_WINDOWS = 2
STUB.newChatEditBox(1)
STUB.newChatEditBox(2)
