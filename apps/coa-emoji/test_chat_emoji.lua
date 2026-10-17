-- Run by test_emoji.py after wow_stub.lua, ChatEmojiData.lua and ChatEmoji.lua are loaded into a fresh Lua 5.1 state.

local TESTS = {}

local function eq(actual, expected, label)
    if actual ~= expected then
        error(string.format("%s: expected %q, got %q", label or "value", tostring(expected), tostring(actual)), 2)
    end
end

local function markup(name)
    return ChatEmoji.Markup(ChatEmoji.Find(name))
end

local function chatBox(index)
    return _G["ChatFrame" .. (index or 1) .. "EditBox"]
end

local function press(box, script)
    box:GetScript(script)(box)
end

local function indexOf(results, name)
    for index, entry in ipairs(results) do
        if entry.name == name then
            return index
        end
    end
end

-- Rendering ---------------------------------------------------------------------------------------

function TESTS.shortcode_becomes_texture()
    eq(markup("thumbsup"), "|TInterface\\CoAEmoji\\1f44d.tga:0|t", "markup")
    eq(ChatEmoji.Render("gg :thumbsup:"), "gg |TInterface\\CoAEmoji\\1f44d.tga:0|t", "rendered")
end

function TESTS.aliases_and_case()
    eq(ChatEmoji.Render(":+1:"), markup("thumbsup"), "alias")
    eq(ChatEmoji.Render(":JOY:"), markup("joy"), "upper case")
end

function TESTS.unknown_shortcodes_stay_text()
    eq(ChatEmoji.Render("a :notanemoji: b"), "a :notanemoji: b", "unknown")
    eq(ChatEmoji.Render("::"), "::", "bare colons")
    eq(ChatEmoji.Render(":joy"), ":joy", "unterminated")
end

function TESTS.several_in_one_message()
    local expected = markup("fire") .. markup("fire") .. " wp " .. markup("skull")
    eq(ChatEmoji.Render(":fire::fire: wp :skull:"), expected, "run")
end

function TESTS.closing_colon_can_open_the_next_shortcode()
    eq(ChatEmoji.Render(":a:smile:"), ":a" .. markup("smile"), "adjacent")
end

function TESTS.times_and_ports_are_not_shortcodes()
    eq(ChatEmoji.Render("meet at 12:30:45"), "meet at 12:30:45", "time")
    eq(ChatEmoji.Render("host:80 port:100:"), "host:80 port" .. markup("100"), "digits after a word still match")
end

function TESTS.hyperlinks_keep_their_payload()
    local link = "|cff0070dd|Hitem:100:0:0:0|h[Sword :joy:]|h|r"
    eq(ChatEmoji.Render(link .. " :100:"), link .. " " .. markup("100"), "link then emoji")
end

function TESTS.existing_textures_are_left_alone()
    local texture = "|TInterface\\Icons\\INV_Misc_Gem_01:16|t"
    eq(ChatEmoji.Render(texture .. " :joy:"), texture .. " " .. markup("joy"), "texture then emoji")
    eq(ChatEmoji.Render("|TInterface\\x:joy:|t"), "|TInterface\\x:joy:|t", "colons inside a texture")
end

function TESTS.urls_are_left_alone()
    eq(ChatEmoji.Render("http://example.com:80/:joy:"), "http://example.com:80/:joy:", "url")
    eq(ChatEmoji.Render("www.site.org/:fire: :fire:"), "www.site.org/:fire: " .. markup("fire"), "www and text")
end

function TESTS.malformed_markup_does_not_error()
    eq(ChatEmoji.Render("|Hitem:1:2 :joy:"), "|Hitem:1:2 :joy:", "unterminated link")
    eq(ChatEmoji.Render("|Tfoo :joy:"), "|Tfoo :joy:", "unterminated texture")
    eq(ChatEmoji.Render("a||Tb :joy:"), "a||Tb :joy:", "escaped pipe before T")
end

function TESTS.nil_and_empty()
    eq(ChatEmoji.Render(nil), nil, "nil")
    eq(ChatEmoji.Render(""), "", "empty")
end

function TESTS.markup_is_safe_inside_a_format_string()
    local rendered = ChatEmoji.Render("100% :joy:")
    eq(rendered, "100% " .. markup("joy"), "percent kept")
    eq(select(2, markup("joy"):gsub("%%", "")), 0, "markup has no percent sign")
end

-- Chat filter -------------------------------------------------------------------------------------

function TESTS.filter_is_registered_for_player_chat_only()
    local player = { "CHAT_MSG_SAY", "CHAT_MSG_YELL", "CHAT_MSG_EMOTE", "CHAT_MSG_WHISPER", "CHAT_MSG_WHISPER_INFORM",
        "CHAT_MSG_PARTY", "CHAT_MSG_PARTY_LEADER", "CHAT_MSG_RAID", "CHAT_MSG_RAID_LEADER", "CHAT_MSG_RAID_WARNING",
        "CHAT_MSG_GUILD", "CHAT_MSG_OFFICER", "CHAT_MSG_CHANNEL", "CHAT_MSG_BATTLEGROUND",
        "CHAT_MSG_BATTLEGROUND_LEADER", "CHAT_MSG_AFK", "CHAT_MSG_DND" }
    for _, event in ipairs(player) do
        eq(STUB.filterCount(event), 1, event)
    end
    for _, event in ipairs({ "CHAT_MSG_SYSTEM", "CHAT_MSG_LOOT", "CHAT_MSG_MONSTER_SAY", "CHAT_MSG_ADDON" }) do
        eq(STUB.filterCount(event), 0, event)
    end
end

function TESTS.filter_replaces_the_message_and_keeps_every_other_argument()
    local results = {
        STUB.runFilters("CHAT_MSG_SAY", "hi :joy:", "Bob", nil, "chan", nil, "", 0, 0, "", 0, 42, "guid"),
    }
    eq(results[1], false, "not filtered out")
    eq(results[2], "hi " .. markup("joy"), "message")
    eq(results[3], "Bob", "sender")
    eq(results[4], nil, "nil language stays nil")
    eq(results[5], "chan", "channel")
    eq(results[6], nil, "nil stays nil")
    eq(results[12], 42, "line id")
    eq(results[13], "guid", "guid")
end

function TESTS.rendering_twice_changes_nothing()
    local once = ChatEmoji.Render("a :joy: b :fire:")
    eq(ChatEmoji.Render(once), once, "idempotent")
end

-- Search and tokens -------------------------------------------------------------------------------

function TESTS.search_ranks_exact_then_prefix_then_contains()
    local results = ChatEmoji.Search("eyes")
    eq(results[1].name, "eyes", "exact first")
    assert(indexOf(results, "heart_eyes") > 1, "a name containing the query matches after the exact one")
    eq(ChatEmoji.Search("heart")[1].name, "heart", "exact beats heart_eyes")
    local prefixed = ChatEmoji.Search("hea")
    assert(indexOf(prefixed, "heart") < indexOf(prefixed, "sparkling_heart"), "prefix before contains")
end

function TESTS.search_uses_aliases()
    eq(ChatEmoji.Search("+1")[1].name, "thumbsup", "alias")
end

function TESTS.search_limit_and_empty_query()
    eq(#ChatEmoji.Search(""), #ChatEmoji.entries, "empty returns everything")
    eq(#ChatEmoji.Search("face", 5), 5, "limit")
end

function TESTS.search_normalizes_the_query()
    eq(ChatEmoji.Search(":Joy:")[1].name, "joy", "colons and case")
    eq(ChatEmoji.Search("  melting face ")[1].name, "melting_face", "spaces become underscores")
    eq(#ChatEmoji.Search("zzzzzz"), 0, "no match")
end

function TESTS.trailing_token()
    local function token(text)
        local start, query = ChatEmoji.TrailingToken(text)
        return start and (start .. ":" .. query) or "none"
    end
    eq(token("hello :jo"), "7:jo", "plain")
    eq(token(":+1"), "1:+1", "plus alias at the start")
    eq(token("x (:jo"), "4:jo", "after punctuation")
    eq(token("hello :j"), "none", "one letter is too short")
    eq(token("12:30"), "none", "time")
    eq(token("a:joy"), "none", "glued to a word")
    eq(token(":joy:"), "none", "already complete")
    eq(token("http://x"), "none", "url")
    eq(token("a :b :cd"), "6:cd", "last colon wins")
end

-- Typeahead ---------------------------------------------------------------------------------------

function TESTS.typing_a_shortcode_lists_matches_above_the_edit_box()
    local box = chatBox()
    box:_type("gg :fir")
    local suggest = ChatEmoji.suggest
    eq(suggest:IsShown(), true, "shown")
    eq(suggest.editBox, box, "attached to the box")
    eq(suggest.points.BOTTOMLEFT.relativeTo, box, "anchored above the box")
    eq(suggest.rows[1].label:GetText(), ":" .. ChatEmoji.Search("fir", 1)[1].name .. ":", "best match first")
    eq(suggest.rows[1].icon.texture, ChatEmoji.Search("fir", 1)[1].texture, "icon")
end

function TESTS.typeahead_is_quiet_when_it_should_be()
    local box = chatBox()
    local suggest = ChatEmoji.suggest
    box:_type("gg :f")
    eq(suggest:IsShown(), false, "one letter")
    box:_type("gg :zzzzzz")
    eq(suggest:IsShown(), false, "nothing matches")
    box:_type("gg :fir", 3)
    eq(suggest:IsShown(), false, "cursor not at the end")
    box:_type("gg :fir")
    eq(suggest:IsShown(), true, "shown again")
    box:SetText("recalled from history")
    eq(suggest:IsShown(), false, "programmatic text change closes it")
end

function TESTS.tab_walks_the_list_and_wraps()
    local box = chatBox()
    local suggest = ChatEmoji.suggest
    box:_type("gg :face")
    local count = 0
    for _, row in ipairs(suggest.rows) do
        if row:IsShown() then count = count + 1 end
    end
    press(box, "OnTabPressed")
    eq(suggest.selected, 1, "first tab picks the first row")
    eq(suggest.rows[1].highlightLocked, true, "highlight")
    press(box, "OnTabPressed")
    eq(suggest.selected, 2, "second tab")
    for _ = 1, count - 2 do press(box, "OnTabPressed") end
    eq(suggest.selected, count, "last row")
    press(box, "OnTabPressed")
    eq(suggest.selected, 1, "wraps to the first")
    STUB.shift = true
    press(box, "OnTabPressed")
    eq(suggest.selected, count, "shift-tab wraps backwards")
    eq(STUB.tabFellThrough, 0, "the normal tab handling never ran")
end

function TESTS.first_shift_tab_picks_the_last_row()
    local box = chatBox()
    box:_type("gg :face")
    local suggest = ChatEmoji.suggest
    local count = 0
    for _, row in ipairs(suggest.rows) do
        if row:IsShown() then count = count + 1 end
    end
    STUB.shift = true
    press(box, "OnTabPressed")
    eq(suggest.selected, count, "last")
end

function TESTS.tab_is_normal_without_a_list()
    local box = chatBox()
    box:_type("hello")
    press(box, "OnTabPressed")
    eq(STUB.tabFellThrough, 1, "default tab handling ran")
end

function TESTS.enter_inserts_the_chosen_emoji_instead_of_sending()
    local box = chatBox()
    box:_type("gg :fir")
    local first = ChatEmoji.suggest.rows[1].entry.name
    press(box, "OnTabPressed")
    press(box, "OnEnterPressed")
    eq(box:GetText(), "gg :" .. first .. ": ", "text")
    eq(box:GetCursorPosition(), #box:GetText(), "cursor at the end")
    eq(#STUB.sent, 0, "nothing sent")
    eq(ChatEmoji.suggest:IsShown(), false, "list closed")
    eq(box:IsShown(), true, "edit box stays open")
end

function TESTS.enter_with_nothing_chosen_still_sends()
    local box = chatBox()
    box:_type("gg :fir")
    press(box, "OnEnterPressed")
    eq(STUB.sent[1], "gg :fir", "sent as typed")
    eq(ChatEmoji.suggest:IsShown(), false, "list closed")
    eq(box:IsShown(), false, "edit box closed after sending")
end

function TESTS.escape_closes_the_list_before_the_edit_box()
    local box = chatBox()
    box:_type("gg :fir")
    press(box, "OnEscapePressed")
    eq(ChatEmoji.suggest:IsShown(), false, "list closed")
    eq(box:IsShown(), true, "edit box still open")
    eq(box:GetText(), "gg :fir", "text kept")
    press(box, "OnEscapePressed")
    eq(box:IsShown(), false, "second escape closes the edit box")
end

function TESTS.clicking_a_row_inserts_it()
    local box = chatBox()
    box:_type("hi :skul")
    local row = ChatEmoji.suggest.rows[1]
    row:Click()
    eq(box:GetText(), "hi :" .. row.entry.name .. ": ", "text")
end

function TESTS.list_closes_when_focus_or_the_box_goes_away()
    local box = chatBox()
    local suggest = ChatEmoji.suggest
    box:_type("gg :fir")
    box:SetFocus()
    box:ClearFocus()
    eq(suggest:IsShown(), false, "focus lost")
    box:_type("gg :fir")
    box:Hide()
    eq(suggest:IsShown(), false, "box hidden")
end

function TESTS.other_edit_boxes_are_untouched()
    local mail = CreateFrame("EditBox", "SendMailNameEditBox", UIParent)
    local chat = chatBox()
    chat:_type("gg :fir")
    eq(AutoCompleteEditBox_OnTabPressed(mail), false, "tab in another box while the chat list is open")
    eq(AutoCompleteEditBox_OnEscapePressed(mail), false, "escape in another box")
    eq(ChatEmoji.suggest:IsShown(), true, "chat list unaffected")
    mail:_type(":fir")
    ChatEdit_OnTextChanged(mail, true)
    eq(ChatEmoji.suggest.editBox, chat, "still attached to the chat box")
end

-- Picker ------------------------------------------------------------------------------------------

function TESTS.every_chat_edit_box_gets_a_button_built_like_the_clients_chat_buttons()
    for index = 1, 2 do
        local box = chatBox(index)
        local button = box.emojiButton
        eq(button:GetParent(), box, "button parent")
        eq(button:GetWidth(), 32, "same size as ChatFrameMenuButton")
        eq(button:GetHeight(), 32, "height")
        eq(button.normalTexture, "Interface\\Buttons\\UI-SquareButton-Up", "stock frame")
        eq(button.pushedTexture, "Interface\\Buttons\\UI-SquareButton-Down", "stock pushed frame")
        eq(button.highlightTexture, "Interface\\Buttons\\UI-Common-MouseHilight", "stock highlight")
        eq(button.highlightMode, "ADD", "additive highlight")
        eq(button.glyph.texture, "Interface\\CoAEmoji\\button.tga", "glyph")
        eq(button.glyph:GetWidth(), 16, "glyph width")
        eq(button.glyph:GetHeight(), 16, "glyph height")
        eq(button.glyph.points.CENTER.relativeTo, button, "glyph centred on the button")
    end
end

function TESTS.button_hangs_off_the_right_end_of_the_bar_and_leaves_the_text_area_alone()
    local box = chatBox()
    local anchor = box.emojiButton.points.LEFT
    eq(anchor.relativeTo, box, "anchored to the edit box")
    eq(anchor.relativePoint, "RIGHT", "at its right end")
    eq(anchor.x, 0, "x")
    eq(anchor.y, 0, "y")
    eq(box.emojiButton.clamped, true, "stays on screen beside a chat window at the right edge")
    ChatEdit_UpdateHeader(box)
    local left, right = box:GetTextInsets()
    eq(left, 15 + 40, "left inset is the client's")
    eq(right, 13, "right inset is the client's, also after a header update")
end

function TESTS.pressing_the_button_sinks_and_darkens_the_glyph_and_releasing_restores_it()
    local button = chatBox().emojiButton
    eq(button.glyph.vertexColor[1], 1, "starts at full brightness")
    button:GetScript("OnMouseDown")(button, "LeftButton")
    eq(button.glyph.points.CENTER.x, 0, "pushed x")
    eq(button.glyph.points.CENTER.y, -1, "pushed y")
    eq(button.glyph.vertexColor[1], 0.92, "pushed shade")
    button:GetScript("OnMouseUp")(button, "LeftButton")
    eq(button.glyph.points.CENTER.x, 0, "released x")
    eq(button.glyph.points.CENTER.y, 0, "released y")
    eq(button.glyph.vertexColor[1], 1, "released shade")
end

function TESTS.hiding_the_button_while_pressed_does_not_leave_the_glyph_pressed()
    local button = chatBox().emojiButton
    button:GetScript("OnMouseDown")(button, "LeftButton")
    button:Hide()
    eq(button.glyph.points.CENTER.y, 0, "y")
    eq(button.glyph.vertexColor[1], 1, "shade")
end

function TESTS.clicking_the_button_plays_the_chat_button_sound()
    chatBox().emojiButton:Click()
    eq(STUB.sounds[1], "igChatEmoteButton", "same sound as ChatFrameMenuButton")
end

function TESTS.windows_created_later_get_a_button_when_first_used()
    local late = STUB.newChatEditBox(11)
    eq(late.emojiButton, nil, "no button yet")
    ChatEdit_ActivateChat(late)
    assert(late.emojiButton, "button attached on first use")
    local other = CreateFrame("EditBox", "MacroEditBox", UIParent)
    ChatEdit_ActivateChat(other)
    eq(other.emojiButton, nil, "non chat edit boxes are skipped")
end

function TESTS.button_opens_and_closes_the_picker_beside_the_chat()
    local box = chatBox()
    local picker = ChatEmoji.picker
    box.emojiButton:Click()
    eq(picker:IsShown(), true, "open")
    eq(picker.editBox, box, "target box")
    eq(picker.points.BOTTOMLEFT.relativeTo, box.emojiButton, "anchored to the button")
    eq(picker.points.BOTTOMLEFT.relativePoint, "TOPLEFT", "above the button, growing away from the chat")
    box.emojiButton:Click()
    eq(picker:IsShown(), false, "closed by a second click")
end

function TESTS.picker_shows_every_emoji_and_scrolls_by_row()
    local box = chatBox()
    box.emojiButton:Click()
    local picker = ChatEmoji.picker
    eq(#picker.list, #ChatEmoji.entries, "all of them")
    eq(picker.cells[1].entry, ChatEmoji.entries[1], "first cell")
    eq(picker.cells[48]:IsShown(), true, "grid is full")
    local maxOffset = math.ceil(#ChatEmoji.entries / 8) - 6
    eq(select(2, picker.slider:GetMinMaxValues()), maxOffset, "slider range")
    picker:GetScript("OnMouseWheel")(picker, -1)
    eq(picker.cells[1].entry, ChatEmoji.entries[9], "one row down")
    eq(picker.slider:GetValue(), 1, "slider follows the wheel")
    for _ = 1, maxOffset + 5 do picker:GetScript("OnMouseWheel")(picker, -1) end
    eq(picker.offset, maxOffset, "clamped at the bottom")
    picker.slider:SetValue(0)
    eq(picker.cells[1].entry, ChatEmoji.entries[1], "slider moves the grid")
    for _ = 1, 3 do picker:GetScript("OnMouseWheel")(picker, 1) end
    eq(picker.offset, 0, "clamped at the top")
end

function TESTS.picker_search_filters_and_hides_the_scroll_bar_when_short()
    chatBox().emojiButton:Click()
    local picker = ChatEmoji.picker
    picker.search:SetText("fire")
    eq(picker.list[1].name, "fire", "best match first")
    assert(#picker.list < #ChatEmoji.entries, "filtered")
    eq(picker.slider:IsShown(), false, "no scroll bar for a short list")
    eq(picker.search.placeholder:IsShown(), false, "placeholder hidden while typing")
    picker.search:SetText("")
    eq(#picker.list, #ChatEmoji.entries, "cleared")
    eq(picker.slider:IsShown(), true, "scroll bar back")
    eq(picker.search.placeholder:IsShown(), true, "placeholder back")
end

function TESTS.picker_reopens_clean()
    local box = chatBox()
    box.emojiButton:Click()
    ChatEmoji.picker.search:SetText("fire")
    ChatEmoji.picker:Hide()
    box.emojiButton:Click()
    eq(ChatEmoji.picker.search:GetText(), "", "search cleared")
    eq(#ChatEmoji.picker.list, #ChatEmoji.entries, "full list")
end

function TESTS.picking_inserts_at_the_cursor_and_closes()
    local box = chatBox()
    box:_type("hi ")
    box.emojiButton:Click()
    local picker = ChatEmoji.picker
    picker.search:SetText("fire")
    picker.cells[1]:Click()
    eq(box:GetText(), "hi :fire: ", "inserted")
    eq(box:HasFocus(), true, "edit box focused")
    eq(picker:IsShown(), false, "picker closed")
end

function TESTS.shift_click_keeps_the_picker_open()
    local box = chatBox()
    box.emojiButton:Click()
    local picker = ChatEmoji.picker
    STUB.shift = true
    picker.cells[1]:Click()
    picker.cells[2]:Click()
    eq(box:GetText(), ":" .. ChatEmoji.entries[1].name .. ": :" .. ChatEmoji.entries[2].name .. ": ", "both inserted")
    eq(picker:IsShown(), true, "still open")
end

function TESTS.hovering_a_cell_names_it_and_escape_is_registered()
    chatBox().emojiButton:Click()
    local picker = ChatEmoji.picker
    picker.cells[3]:GetScript("OnEnter")(picker.cells[3])
    eq(picker.footer:GetText(), ":" .. ChatEmoji.entries[3].name .. ":", "footer")
    picker.cells[3]:GetScript("OnLeave")(picker.cells[3])
    eq(picker.footer:GetText(), "", "footer cleared")
    local registered = false
    for _, name in ipairs(UISpecialFrames) do
        if name == "ChatEmojiPicker" then registered = true end
    end
    eq(registered, true, "escape closes the picker")
end

function TESTS.button_tooltip()
    local button = chatBox().emojiButton
    button:GetScript("OnEnter")(button)
    eq(STUB.tooltip.title, "Emoji", "title")
    eq(STUB.tooltip.shown, true, "shown")
    button:GetScript("OnLeave")(button)
    eq(STUB.tooltip.shown, false, "hidden")
end

-- Chat bubbles ------------------------------------------------------------------------------------

local function nextFrame(seconds)
    STUB.nextFrame(seconds or 0.016)
end

function TESTS.a_bubble_that_exists_when_the_chat_event_arrives_is_rendered_at_once()
    local _, text = STUB.newBubble("gg :fire:")
    STUB.fireEvent("CHAT_MSG_SAY", "gg :fire:", "Bob")
    eq(text:GetText(), "gg " .. markup("fire"), "bubble text")
end

function TESTS.a_bubble_created_after_the_event_is_rendered_on_the_next_frame()
    STUB.fireEvent("CHAT_MSG_YELL", "look :eyes:", "Bob")
    local _, text = STUB.newBubble("look :eyes:")
    eq(text:GetText(), "look :eyes:", "not yet")
    nextFrame()
    eq(text:GetText(), "look " .. markup("eyes"), "rendered")
end

function TESTS.a_reused_bubble_is_rendered_again_for_its_new_message()
    local _, text = STUB.newBubble("one :joy:")
    STUB.fireEvent("CHAT_MSG_SAY", "one :joy:", "Bob")
    eq(text:GetText(), "one " .. markup("joy"), "first message")
    text:SetText("two :fire:")
    nextFrame()
    eq(text:GetText(), "two " .. markup("fire"), "second message, found by the watch")
    text:SetText("three :joy:")
    STUB.fireEvent("CHAT_MSG_PARTY", "three :joy:", "Bob")
    eq(text:GetText(), "three " .. markup("joy"), "third message, found by the event")
end

function TESTS.every_bubble_is_handled_not_only_the_newest()
    local _, older = STUB.newBubble("a :joy:")
    local _, newer = STUB.newBubble("b :fire:")
    STUB.fireEvent("CHAT_MSG_SAY", "b :fire:", "Bob")
    eq(older:GetText(), "a " .. markup("joy"), "older bubble")
    eq(newer:GetText(), "b " .. markup("fire"), "newer bubble")
end

function TESTS.the_order_of_the_regions_inside_a_bubble_does_not_matter()
    local _, text = STUB.newBubble(":joy:", true)
    STUB.fireEvent("CHAT_MSG_SAY", ":joy:", "Bob")
    eq(text:GetText(), markup("joy"), "font string before the textures")
end

function TESTS.other_world_frames_are_not_touched()
    local nameplate = CreateFrame("Frame", nil, WorldFrame)
    local label = nameplate:CreateFontString(nil, "OVERLAY")
    label:SetText("Bob :fire:")
    nameplate:CreateTexture(nil, "BORDER"):SetTexture("Interface\\Tooltips\\Nameplate-Border")
    local textOnly = CreateFrame("Frame", nil, WorldFrame)
    local caption = textOnly:CreateFontString(nil, "OVERLAY")
    caption:SetText(":joy:")
    CreateFrame("Frame", nil, WorldFrame)
    local _, bubbleText = STUB.newBubble(":joy:")
    STUB.fireEvent("CHAT_MSG_SAY", ":joy:", "Bob")
    eq(label:GetText(), "Bob :fire:", "nameplate text")
    eq(caption:GetText(), ":joy:", "text without the bubble texture")
    eq(bubbleText:GetText(), markup("joy"), "the bubble in the same crowd")
end

function TESTS.text_without_shortcodes_is_never_rewritten()
    local _, text = STUB.newBubble("hello there")
    STUB.fireEvent("CHAT_MSG_SAY", "hello there", "Bob")
    nextFrame()
    eq(text.setCount, 1, "only the engine's own SetText")
end

function TESTS.a_rendered_bubble_is_left_alone_on_later_frames()
    local _, text = STUB.newBubble("a :joy:")
    STUB.fireEvent("CHAT_MSG_SAY", "a :joy:", "Bob")
    nextFrame()
    nextFrame()
    eq(text.setCount, 2, "the engine's SetText and one render")
end

function TESTS.nothing_runs_between_chat_events()
    local _, text = STUB.newBubble(":joy:")
    nextFrame()
    eq(text:GetText(), ":joy:", "no event, no work")
    eq(ChatEmoji.bubbleWatcher:IsShown(), false, "watcher idle")
end

function TESTS.the_watch_stops_after_a_second()
    STUB.fireEvent("CHAT_MSG_SAY", "hi", "Bob")
    eq(ChatEmoji.bubbleWatcher:IsShown(), true, "watching")
    nextFrame(0.6)
    eq(ChatEmoji.bubbleWatcher:IsShown(), true, "still watching")
    nextFrame(0.6)
    eq(ChatEmoji.bubbleWatcher:IsShown(), false, "done")
    local _, text = STUB.newBubble(":joy:")
    nextFrame()
    eq(text:GetText(), ":joy:", "no longer looking")
end

function TESTS.bubble_events_are_player_speech_only()
    for _, event in ipairs({ "CHAT_MSG_SAY", "CHAT_MSG_YELL", "CHAT_MSG_EMOTE", "CHAT_MSG_PARTY",
        "CHAT_MSG_PARTY_LEADER" }) do
        eq(ChatEmoji.bubbleWatcher.events[event], true, event)
    end
    for _, event in ipairs({ "CHAT_MSG_MONSTER_SAY", "CHAT_MSG_GUILD", "CHAT_MSG_WHISPER" }) do
        eq(ChatEmoji.bubbleWatcher.events[event], nil, event)
    end
end

-- Data --------------------------------------------------------------------------------------------

function TESTS.every_name_and_alias_finds_its_entry()
    for _, entry in ipairs(ChatEmoji.entries) do
        for _, name in ipairs(entry.names) do
            eq(ChatEmoji.Find(name), entry, name)
        end
    end
end

return TESTS
