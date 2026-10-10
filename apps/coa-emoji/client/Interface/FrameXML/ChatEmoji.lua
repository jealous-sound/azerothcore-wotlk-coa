-- CoA chat emoji. Loaded from FrameXML.toc right after FloatingChatFrame.xml, so it is part of the base UI and
-- cannot be switched off in the addon list.
--
-- Players type :shortcodes: and the wire format stays plain text. Each client swaps them for inline textures when a
-- message is displayed, so the 255 byte chat limit is never at risk and a client without the textures just shows
-- the text. Needs ChatEmojiData.lua (generated) and the textures under Interface\CoAEmoji.

ChatEmoji = {}

local EMOJI_DIR = "Interface\\CoAEmoji\\"
local BUTTON_GLYPH = EMOJI_DIR .. "button.tga"
local NAME_CHARS = "[%w_%+%-]"
local MIN_QUERY_LENGTH = 2
local MAX_SUGGESTIONS = 6
local SUGGEST_ROW_HEIGHT = 20
local PICKER_COLUMNS = 8
local PICKER_ROWS = 6
local PICKER_CELL = 28
local PICKER_PADDING = 10
local PICKER_GRID_TOP = 38
local BUTTON_SIZE = 32
local BUTTON_GLYPH_SIZE = 16

local FILTERED_EVENTS = {
    "CHAT_MSG_SAY", "CHAT_MSG_YELL", "CHAT_MSG_EMOTE", "CHAT_MSG_WHISPER", "CHAT_MSG_WHISPER_INFORM",
    "CHAT_MSG_PARTY", "CHAT_MSG_PARTY_LEADER", "CHAT_MSG_RAID", "CHAT_MSG_RAID_LEADER", "CHAT_MSG_RAID_WARNING",
    "CHAT_MSG_GUILD", "CHAT_MSG_OFFICER", "CHAT_MSG_CHANNEL", "CHAT_MSG_BATTLEGROUND",
    "CHAT_MSG_BATTLEGROUND_LEADER", "CHAT_MSG_AFK", "CHAT_MSG_DND",
}

local BACKDROP = {
    bgFile = "Interface\\Tooltips\\UI-Tooltip-Background",
    edgeFile = "Interface\\Tooltips\\UI-Tooltip-Border",
    tile = true, tileSize = 16, edgeSize = 12,
    insets = { left = 3, right = 3, top = 3, bottom = 3 },
}

-- Data --------------------------------------------------------------------------------------------

local entries = {}
local byName = {}

for index, row in ipairs(CHAT_EMOJI_DATA) do
    local entry = { name = row[1], texture = EMOJI_DIR .. row[2] .. ".tga", names = { row[1] } }
    for column = 3, #row do
        entry.names[#entry.names + 1] = row[column]
    end
    for _, name in ipairs(entry.names) do
        byName[name] = entry
    end
    entries[index] = entry
end

ChatEmoji.entries = entries

function ChatEmoji.Find(name)
    return byName[name:lower()]
end

function ChatEmoji.Markup(entry)
    -- Height 0 scales the icon to the chat font, like the raid target icon tags.
    return "|T" .. entry.texture .. ":0|t"
end

-- Shortcodes in text ------------------------------------------------------------------------------

local function expandWord(word)
    local pieces, position = {}, 1
    while true do
        local colon = word:find(":", position, true)
        if not colon then
            break
        end
        local name, after = word:match("^(" .. NAME_CHARS .. "+):()", colon + 1)
        local entry = name and byName[name:lower()]
        if entry then
            pieces[#pieces + 1] = word:sub(position, colon - 1)
            pieces[#pieces + 1] = ChatEmoji.Markup(entry)
            position = after
        else
            -- Step one character so the closing colon can open the next candidate (":a:smile:").
            pieces[#pieces + 1] = word:sub(position, colon)
            position = colon + 1
        end
    end
    pieces[#pieces + 1] = word:sub(position)
    return table.concat(pieces)
end

local function expandPlainText(chunk)
    if not chunk:find(":", 1, true) then
        return chunk
    end
    return (chunk:gsub("%S+", function(word)
        -- "http://host:80/:smile:" is a link, not a message with an emoji in it.
        if word:find("://", 1, true) or word:find("^www%.") then
            return word
        end
        return expandWord(word)
    end))
end

-- Replaces :shortcode: with texture markup, leaving hyperlinks and existing textures alone. Their payloads are full
-- of colons, and an item id such as :100: would otherwise turn into an emoji.
function ChatEmoji.Render(text)
    if not text or not text:find(":", 1, true) then
        return text
    end
    local pieces, position = {}, 1
    while true do
        local start = text:find("|[HT]", position)
        if not start then
            pieces[#pieces + 1] = expandPlainText(text:sub(position))
            break
        end
        pieces[#pieces + 1] = expandPlainText(text:sub(position, start - 1))
        local _, finish
        if text:sub(start + 1, start + 1) == "H" then
            _, finish = text:find("|h.-|h", start)
        else
            _, finish = text:find("|t", start, true)
        end
        if not finish then
            pieces[#pieces + 1] = text:sub(start)
            break
        end
        pieces[#pieces + 1] = text:sub(start, finish)
        position = finish + 1
    end
    return table.concat(pieces)
end

local function renderChatMessage(self, event, message, ...)
    return false, ChatEmoji.Render(message), ...
end

for _, event in ipairs(FILTERED_EVENTS) do
    ChatFrame_AddMessageEventFilter(event, renderChatMessage)
end

-- Matching ----------------------------------------------------------------------------------------

local function normalizeQuery(query)
    query = query:lower():gsub(":", "")
    query = query:match("^%s*(.-)%s*$")
    return (query:gsub("%s+", "_"))
end

-- 1 exact, 2 prefix, 3 anywhere in the name; nil when the entry does not match.
local function matchRank(entry, query)
    local best
    for _, name in ipairs(entry.names) do
        local position = name:find(query, 1, true)
        if position then
            local rank = (name == query and 1) or (position == 1 and 2) or 3
            if not best or rank < best then
                best = rank
            end
        end
    end
    return best
end

-- Best matches first, picker order within a rank. An empty query returns everything.
function ChatEmoji.Search(query, limit)
    query = normalizeQuery(query)
    local buckets = { {}, {}, {} }
    for _, entry in ipairs(entries) do
        local rank = (query == "") and 3 or matchRank(entry, query)
        if rank then
            local bucket = buckets[rank]
            bucket[#bucket + 1] = entry
        end
    end
    local results = {}
    for _, bucket in ipairs(buckets) do
        for _, entry in ipairs(bucket) do
            if limit and #results >= limit then
                return results
            end
            results[#results + 1] = entry
        end
    end
    return results
end

-- The ":que" being typed at the end of the text, as its start position and the letters after the colon. A colon
-- glued to a word ("12:30", "http:") is not the start of a shortcode.
function ChatEmoji.TrailingToken(text)
    local start, query = text:match("():(" .. NAME_CHARS .. "*)$")
    if not start or #query < MIN_QUERY_LENGTH then
        return nil
    end
    if start > 1 and text:sub(start - 1, start - 1):find("%w") then
        return nil
    end
    return start, query
end

-- Typeahead ---------------------------------------------------------------------------------------

local function isChatEditBox(editBox)
    local name = editBox and editBox.GetName and editBox:GetName()
    return name ~= nil and name:find("^ChatFrame%d+EditBox$") ~= nil
end

-- The cursor may be counted in bytes or characters depending on the API, so accept either for "at the end".
local function cursorIsAtEnd(editBox, text)
    local cursor = editBox:GetCursorPosition()
    return cursor == #text or (strlenutf8 ~= nil and cursor == strlenutf8(text))
end

local suggest = CreateFrame("Frame", "ChatEmojiSuggest", UIParent)
suggest:SetFrameStrata("DIALOG")
suggest:SetWidth(210)
suggest:SetBackdrop(BACKDROP)
suggest:SetBackdropColor(0.05, 0.05, 0.08, 0.95)
suggest:SetBackdropBorderColor(0.6, 0.6, 0.6, 1)
suggest:EnableMouse(true)
suggest:Hide()
suggest.rows = {}
ChatEmoji.suggest = suggest

suggest.hint = suggest:CreateFontString(nil, "ARTWORK", "GameFontDisableSmall")
suggest.hint:SetPoint("BOTTOMLEFT", suggest, "BOTTOMLEFT", 8, 6)
suggest.hint:SetText("Tab: choose    Enter: insert")

local function acceptSuggestion(entry)
    local editBox = suggest.editBox
    if not editBox or not entry then
        return
    end
    local text = editBox:GetText()
    local start = ChatEmoji.TrailingToken(text)
    if not start then
        return
    end
    local newText = text:sub(1, start - 1) .. ":" .. entry.name .. ": "
    suggest:Hide()
    editBox:SetText(newText)
    editBox:SetCursorPosition(#newText)
end

local function setSuggestionSelected(index)
    suggest.selected = index
    for rowIndex, row in ipairs(suggest.rows) do
        if rowIndex == index then
            row:LockHighlight()
        else
            row:UnlockHighlight()
        end
    end
end

for index = 1, MAX_SUGGESTIONS do
    local row = CreateFrame("Button", nil, suggest)
    row:SetHeight(SUGGEST_ROW_HEIGHT)
    row:SetPoint("TOPLEFT", suggest, "TOPLEFT", 6, -6 - (index - 1) * SUGGEST_ROW_HEIGHT)
    row:SetPoint("TOPRIGHT", suggest, "TOPRIGHT", -6, -6 - (index - 1) * SUGGEST_ROW_HEIGHT)
    row:SetHighlightTexture("Interface\\QuestFrame\\UI-QuestTitleHighlight", "ADD")
    row.icon = row:CreateTexture(nil, "ARTWORK")
    row.icon:SetWidth(16)
    row.icon:SetHeight(16)
    row.icon:SetPoint("LEFT", row, "LEFT", 2, 0)
    row.label = row:CreateFontString(nil, "ARTWORK", "GameFontHighlightSmall")
    row.label:SetPoint("LEFT", row.icon, "RIGHT", 6, 0)
    row:SetScript("OnClick", function(self)
        acceptSuggestion(self.entry)
    end)
    suggest.rows[index] = row
end

local function showSuggestions(editBox, results)
    suggest.editBox = editBox
    suggest.results = results
    for index, row in ipairs(suggest.rows) do
        local entry = results[index]
        row.entry = entry
        if entry then
            row.icon:SetTexture(entry.texture)
            row.label:SetText(":" .. entry.name .. ":")
            row:Show()
        else
            row:Hide()
        end
    end
    setSuggestionSelected(nil)
    suggest:SetHeight(#results * SUGGEST_ROW_HEIGHT + 12 + 16)
    suggest:ClearAllPoints()
    suggest:SetPoint("BOTTOMLEFT", editBox, "TOPLEFT", 12, 2)
    suggest:SetFrameLevel(editBox:GetFrameLevel() + 10)
    suggest:Show()
end

local function hideSuggestions(editBox)
    if suggest:IsShown() and (not editBox or suggest.editBox == editBox) then
        suggest:Hide()
    end
end

local function updateSuggestions(editBox)
    local text = editBox:GetText()
    local _, query = ChatEmoji.TrailingToken(text)
    if query and cursorIsAtEnd(editBox, text) then
        local results = ChatEmoji.Search(query, MAX_SUGGESTIONS)
        if #results > 0 then
            showSuggestions(editBox, results)
            return
        end
    end
    hideSuggestions(editBox)
end

local function suggestionsOpenFor(editBox)
    return suggest:IsShown() and suggest.editBox == editBox
end

hooksecurefunc("ChatEdit_OnTextChanged", function(editBox, userInput)
    if not isChatEditBox(editBox) then
        return
    end
    if userInput then
        updateSuggestions(editBox)
    else
        hideSuggestions(editBox)
    end
end)
hooksecurefunc("ChatEdit_OnEditFocusLost", hideSuggestions)
hooksecurefunc("ChatEdit_OnHide", hideSuggestions)

-- The chat edit box asks AutoCompleteEditBox_On*Pressed first and stops if it returns true. That is the client's
-- own protocol for name completion, so the emoji list follows the same keys: Tab cycles, Enter inserts the chosen
-- row (Enter with nothing chosen still sends), Escape closes the list before it closes the edit box.
local nameCompleteTab = AutoCompleteEditBox_OnTabPressed
function AutoCompleteEditBox_OnTabPressed(editBox)
    if suggestionsOpenFor(editBox) then
        local count = #suggest.results
        local step = IsShiftKeyDown() and -1 or 1
        local selected = suggest.selected or (step == 1 and 0 or 1)
        setSuggestionSelected((selected - 1 + step) % count + 1)
        return true
    end
    return nameCompleteTab(editBox)
end

local nameCompleteEnter = AutoCompleteEditBox_OnEnterPressed
function AutoCompleteEditBox_OnEnterPressed(editBox)
    if suggestionsOpenFor(editBox) then
        local entry = suggest.selected and suggest.results[suggest.selected]
        if entry then
            acceptSuggestion(entry)
            return true
        end
        -- Close it before the message is sent: ChatEdit_OnEnterPressed ends by calling the Escape handler.
        suggest:Hide()
    end
    return nameCompleteEnter(editBox)
end

local nameCompleteEscape = AutoCompleteEditBox_OnEscapePressed
function AutoCompleteEditBox_OnEscapePressed(editBox)
    if suggestionsOpenFor(editBox) then
        suggest:Hide()
        return true
    end
    return nameCompleteEscape(editBox)
end

-- Picker ------------------------------------------------------------------------------------------

local picker = CreateFrame("Frame", "ChatEmojiPicker", UIParent)
local gridWidth = PICKER_COLUMNS * PICKER_CELL
local gridHeight = PICKER_ROWS * PICKER_CELL
picker:SetFrameStrata("DIALOG")
picker:SetWidth(PICKER_PADDING * 2 + gridWidth + 20)
picker:SetHeight(PICKER_GRID_TOP + gridHeight + 34)
picker:SetBackdrop(BACKDROP)
picker:SetBackdropColor(0.05, 0.05, 0.08, 0.95)
picker:SetBackdropBorderColor(0.6, 0.6, 0.6, 1)
picker:SetClampedToScreen(true)
picker:EnableMouse(true)
picker:EnableMouseWheel(true)
picker:Hide()
picker.cells = {}
picker.offset = 0
picker.list = entries
ChatEmoji.picker = picker
tinsert(UISpecialFrames, "ChatEmojiPicker")

picker.footer = picker:CreateFontString(nil, "ARTWORK", "GameFontHighlightSmall")
picker.footer:SetPoint("BOTTOMLEFT", picker, "BOTTOMLEFT", PICKER_PADDING, 10)

local close = CreateFrame("Button", nil, picker, "UIPanelCloseButton")
close:SetPoint("TOPRIGHT", picker, "TOPRIGHT", 2, 2)

local search = CreateFrame("EditBox", "ChatEmojiPickerSearch", picker)
search:SetHeight(20)
search:SetPoint("TOPLEFT", picker, "TOPLEFT", PICKER_PADDING, -10)
search:SetPoint("TOPRIGHT", picker, "TOPRIGHT", -28, -10)
search:SetAutoFocus(false)
search:SetFontObject(ChatFontNormal)
search:SetMaxLetters(32)
search:SetTextInsets(6, 6, 0, 0)
search:SetBackdrop(BACKDROP)
search:SetBackdropColor(0, 0, 0, 0.6)
search:SetBackdropBorderColor(0.5, 0.5, 0.5, 1)
search.placeholder = search:CreateFontString(nil, "ARTWORK", "GameFontDisableSmall")
search.placeholder:SetPoint("LEFT", search, "LEFT", 7, 0)
search.placeholder:SetText("Search emoji")
picker.search = search

local slider = CreateFrame("Slider", nil, picker)
slider:SetWidth(16)
slider:SetHeight(gridHeight)
slider:SetPoint("TOPRIGHT", picker, "TOPRIGHT", -PICKER_PADDING, -PICKER_GRID_TOP)
slider:SetOrientation("VERTICAL")
slider:SetThumbTexture("Interface\\Buttons\\UI-ScrollBar-Knob")
slider:GetThumbTexture():SetWidth(16)
slider:GetThumbTexture():SetHeight(24)
slider:SetMinMaxValues(0, 0)
slider:SetValueStep(1)
slider:SetValue(0)
picker.slider = slider

local function refreshPicker()
    local first = picker.offset * PICKER_COLUMNS
    for index, cell in ipairs(picker.cells) do
        local entry = picker.list[first + index]
        cell.entry = entry
        if entry then
            cell.icon:SetTexture(entry.texture)
            cell:Show()
        else
            cell:Hide()
        end
    end
end

local function maxPickerOffset()
    return math.max(0, math.ceil(#picker.list / PICKER_COLUMNS) - PICKER_ROWS)
end

local function setPickerOffset(offset)
    picker.offset = math.max(0, math.min(offset, maxPickerOffset()))
    slider:SetValue(picker.offset)
    refreshPicker()
end

local function filterPicker(query)
    picker.list = ChatEmoji.Search(query)
    local maxOffset = maxPickerOffset()
    slider:SetMinMaxValues(0, maxOffset)
    if maxOffset > 0 then
        slider:Show()
    else
        slider:Hide()
    end
    setPickerOffset(0)
end

local function insertEmoji(entry)
    local editBox = picker.editBox
    if not editBox or not entry then
        return
    end
    ChatEdit_ActivateChat(editBox)
    editBox:Insert(":" .. entry.name .. ": ")
    -- Shift-click keeps the picker open for several in a row.
    if not IsShiftKeyDown() then
        picker:Hide()
    end
end

for index = 1, PICKER_COLUMNS * PICKER_ROWS do
    local cell = CreateFrame("Button", nil, picker)
    cell:SetWidth(PICKER_CELL)
    cell:SetHeight(PICKER_CELL)
    cell:SetPoint("TOPLEFT", picker, "TOPLEFT",
        PICKER_PADDING + ((index - 1) % PICKER_COLUMNS) * PICKER_CELL,
        -PICKER_GRID_TOP - math.floor((index - 1) / PICKER_COLUMNS) * PICKER_CELL)
    cell:SetHighlightTexture("Interface\\Buttons\\ButtonHilight-Square", "ADD")
    cell.icon = cell:CreateTexture(nil, "ARTWORK")
    cell.icon:SetWidth(PICKER_CELL - 6)
    cell.icon:SetHeight(PICKER_CELL - 6)
    cell.icon:SetPoint("CENTER", cell, "CENTER", 0, 0)
    cell:SetScript("OnClick", function(self)
        insertEmoji(self.entry)
    end)
    cell:SetScript("OnEnter", function(self)
        picker.footer:SetText(":" .. self.entry.name .. ":")
    end)
    cell:SetScript("OnLeave", function()
        picker.footer:SetText("")
    end)
    picker.cells[index] = cell
end

picker:SetScript("OnMouseWheel", function(self, delta)
    setPickerOffset(self.offset - delta)
end)
slider:SetScript("OnValueChanged", function(self, value)
    local offset = math.floor(value + 0.5)
    if offset ~= picker.offset then
        setPickerOffset(offset)
    end
end)
search:SetScript("OnTextChanged", function(self)
    if self:GetText() == "" then
        self.placeholder:Show()
    else
        self.placeholder:Hide()
    end
    filterPicker(self:GetText())
end)
search:SetScript("OnEscapePressed", function()
    picker:Hide()
end)
search:SetScript("OnEnterPressed", function(self)
    self:ClearFocus()
end)
picker:SetScript("OnShow", function()
    picker.footer:SetText("")
    search:SetText("")
    filterPicker("")
end)

function ChatEmoji.TogglePicker(editBox)
    if picker:IsShown() and picker.editBox == editBox then
        picker:Hide()
        return
    end
    picker.editBox = editBox
    picker:ClearAllPoints()
    -- Opens over the world beside the chat, not over the messages being replied to.
    picker:SetPoint("BOTTOMLEFT", editBox.emojiButton or editBox, "TOPLEFT", 0, 6)
    picker:SetFrameLevel(editBox:GetFrameLevel() + 10)
    picker:Show()
end

-- Edit box button ---------------------------------------------------------------------------------

-- Built from the same parts as the client's own chat buttons (ChatFrameMenuButton): the stock square frame art, a gold
-- glyph, the stock mouse highlight and the same click sound. It hangs off the right end of the bar the way the client
-- hangs its input language button, so the typing area and its insets stay the client's.
-- The stock pushed textures sink their icon a pixel or two and darken it a little (about 8%). The glyph is a separate
-- texture, so it does the same itself.
local function pressGlyph(button)
    button.glyph:SetPoint("CENTER", button, "CENTER", 0, -1)
    button.glyph:SetVertexColor(0.92, 0.92, 0.92)
end

local function releaseGlyph(button)
    button.glyph:SetPoint("CENTER", button, "CENTER", 0, 0)
    button.glyph:SetVertexColor(1, 1, 1)
end

local function attachButton(editBox)
    if editBox.emojiButton or not isChatEditBox(editBox) then
        return
    end
    local button = CreateFrame("Button", editBox:GetName() .. "EmojiButton", editBox)
    button:SetWidth(BUTTON_SIZE)
    button:SetHeight(BUTTON_SIZE)
    button:SetPoint("LEFT", editBox, "RIGHT", 0, 0)
    -- Outside the bar it would go off screen for a chat window docked at the right edge.
    button:SetClampedToScreen(true)
    button:SetNormalTexture("Interface\\Buttons\\UI-SquareButton-Up")
    button:SetPushedTexture("Interface\\Buttons\\UI-SquareButton-Down")
    button:SetHighlightTexture("Interface\\Buttons\\UI-Common-MouseHilight", "ADD")
    -- OVERLAY draws above the frame art whatever order the textures were made in, and below the highlight.
    button.glyph = button:CreateTexture(nil, "OVERLAY")
    button.glyph:SetTexture(BUTTON_GLYPH)
    button.glyph:SetWidth(BUTTON_GLYPH_SIZE)
    button.glyph:SetHeight(BUTTON_GLYPH_SIZE)
    releaseGlyph(button)
    button:SetScript("OnMouseDown", pressGlyph)
    button:SetScript("OnMouseUp", releaseGlyph)
    button:SetScript("OnHide", releaseGlyph)
    button:SetScript("OnClick", function(self)
        PlaySound("igChatEmoteButton")
        ChatEmoji.TogglePicker(self:GetParent())
    end)
    button:SetScript("OnEnter", function(self)
        GameTooltip:SetOwner(self, "ANCHOR_TOP")
        GameTooltip:SetText("Emoji")
        GameTooltip:AddLine("Click to pick one, or type :name:", 1, 1, 1)
        GameTooltip:Show()
    end)
    button:SetScript("OnLeave", function()
        GameTooltip:Hide()
    end)
    editBox.emojiButton = button
end

-- Whisper tabs create their windows later, so anything that becomes active gets its button on first use.
hooksecurefunc("ChatEdit_ActivateChat", attachButton)

for index = 1, NUM_CHAT_WINDOWS do
    local editBox = _G["ChatFrame" .. index .. "EditBox"]
    if editBox then
        attachButton(editBox)
    end
end

-- Chat bubbles ------------------------------------------------------------------------------------

-- The engine builds the speech bubbles over characters' heads from the raw message, so the chat filters above never
-- see them. A bubble is an unnamed child of WorldFrame holding the ChatBubble background texture and one FontString,
-- which is also how ElvUI finds them. The engine reuses those frames, so after any chat event that can make a bubble
-- every known bubble is checked for new text, not just the frame that was created last.
local BUBBLE_BACKGROUND = "interface\\tooltips\\chatbubble-background"
-- Only players can type shortcodes, so NPC speech is not watched.
local BUBBLE_EVENTS = { "CHAT_MSG_SAY", "CHAT_MSG_YELL", "CHAT_MSG_EMOTE", "CHAT_MSG_PARTY", "CHAT_MSG_PARTY_LEADER" }
-- A bubble can show up a frame or so after its chat event, so every frame is checked for this long afterwards.
local BUBBLE_WATCH_SECONDS = 1

local bubbleTexts = {}
local checkedFrames = setmetatable({}, { __mode = "k" })
local worldChildCount = 0

local function findBubbleText(frame)
    local text, isBubble
    for index = 1, frame:GetNumRegions() do
        local region = select(index, frame:GetRegions())
        if region:IsObjectType("FontString") then
            text = region
        elseif region:IsObjectType("Texture") then
            local path = region:GetTexture()
            if type(path) == "string" and path:lower() == BUBBLE_BACKGROUND then
                isBubble = true
            end
        end
    end
    return isBubble and text or nil
end

-- WorldFrame also parents nameplates, so every child is looked at once and then remembered either way.
local function findNewBubbles()
    local count = WorldFrame:GetNumChildren()
    if count == worldChildCount then
        return
    end
    worldChildCount = count
    for _, frame in ipairs({ WorldFrame:GetChildren() }) do
        if not checkedFrames[frame] then
            checkedFrames[frame] = true
            local text = findBubbleText(frame)
            if text then
                bubbleTexts[#bubbleTexts + 1] = text
            end
        end
    end
end

-- Rendering twice changes nothing, so a bubble that already holds textures is not written again.
local function renderBubbleText(text)
    local current = text:GetText()
    local rendered = ChatEmoji.Render(current)
    if rendered ~= current then
        text:SetText(rendered)
    end
end

local function checkBubbles()
    findNewBubbles()
    for _, text in ipairs(bubbleTexts) do
        renderBubbleText(text)
    end
end

-- OnUpdate only runs while shown, so the watcher stays hidden and costs nothing until a chat event switches it on.
-- Events still reach a hidden frame.
local bubbleWatcher = CreateFrame("Frame")
bubbleWatcher:Hide()
bubbleWatcher:SetScript("OnEvent", function(self)
    checkBubbles()
    self.secondsLeft = BUBBLE_WATCH_SECONDS
    self:Show()
end)
bubbleWatcher:SetScript("OnUpdate", function(self, elapsed)
    checkBubbles()
    self.secondsLeft = self.secondsLeft - elapsed
    if self.secondsLeft <= 0 then
        self:Hide()
    end
end)
for _, event in ipairs(BUBBLE_EVENTS) do
    bubbleWatcher:RegisterEvent(event)
end
ChatEmoji.bubbleWatcher = bubbleWatcher
