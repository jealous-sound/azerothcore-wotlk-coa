"""Prepare optional second-name UI members from the canonical client source; never install them."""

import argparse
import difflib
from pathlib import Path


SHARED = 'Interface/SharedXML/Util/CharacterNames.lua'


def replace(text, before, after, count=1):
    if text.count(before) != count:
        raise ValueError(f'Unsupported UI source: expected {count} occurrences of {before!r}')
    return text.replace(before, after)


def transform(path, text):
    if path.endswith(('GlueXML.toc', 'FrameXML.toc')):
        return replace(text, '..\\SharedXML\\Util\\NewCharacterSetupUtil.lua',
                       '..\\SharedXML\\Util\\CharacterNames.lua\n..\\SharedXML\\Util\\NewCharacterSetupUtil.lua')
    if path.endswith('CharacterCreate.xml'):
        text = replace(text, 'inherits="GlueFontHighlight" text="NAME"',
                       'name="CharacterCreateFirstNameLabel" inherits="GlueFontNormalSmall" text="COA_FIRST_NAME"')
        return replace(text, 'CreateCharacter(CharacterCreateNameEdit:GetText(),',
                       'CoACharacterNames.CreateCharacter(', 2)
    if path.endswith('CharacterCreate.lua'):
        border = 'CharacterCreateNameEdit:SetBackdropBorderColor(backdropColor[1], backdropColor[2], backdropColor[3]);'
        text = replace(text, border, border + '\n\tCoACharacterNames.InitCreation()')
        color = 'CharacterCreateNameEdit:SetBackdropColor(backdropColor[4], backdropColor[5], backdropColor[6]);'
        text = replace(text, color, color + '\n\tCharacterCreateSecondNameEdit:SetBackdropColor('
                       'backdropColor[4], backdropColor[5], backdropColor[6]);', 2)
        text = replace(text, 'CreateCharacter(CharacterCreateNameEdit:GetText());',
                       'CoACharacterNames.CreateCharacter();')
        text = replace(text, 'CharacterCreateNameEdit:SetText( PaidChange_GetName() );',
                       'CoACharacterNames.SetCreationName(PaidChange_GetName());')
        return replace(text, 'CharacterCreateNameEdit:SetText("");', 'CoACharacterNames.SetCreationName("");')
    if path.endswith('GlueDialog.lua'):
        return replace(text, 'CreateCharacter(CharacterCreateNameEdit:GetText());',
                       'CoACharacterNames.CreateCharacter();')
    if path.endswith('CharacterSelect.xml'):
        text = replace(text, 'name="CharacterRenameEditBox" letters="12"',
                       'name="CharacterRenameEditBox" letters="25"')
        # Restrict the width replacement to this edit box, preserving other dialogs.
        start = text.index('<EditBox name="CharacterRenameEditBox"')
        end = text.index('</EditBox>', start)
        block = replace(text[start:end], 'x="130" y="32"', 'x="220" y="32"')
        block = replace(block, '<Scripts>',
                        '<Scripts>\n                    <OnLoad>self:SetMaxBytes(CoACharacterNames.maxBytes);</OnLoad>')
        return text[:start] + block + text[end:]
    if path.endswith('NewCharacterSetupUtil.lua'):
        return replace(text, 'pendingCharacterName:firstUpper()', 'CoACharacterNames.Normalize(pendingCharacterName)')
    if path.endswith('GlobalOverwrites.lua'):
        return replace(text, 'player = player:firstUpper()', 'player = CoACharacterNames.Normalize(player)', 2)
    if path.endswith('ItemRef.lua'):
        return replace(text, 'SendWho(WHO_TAG_NAME..name);',
                       'SendWho(WHO_TAG_NAME..CoACharacterNames.Quote(name));')
    if path.endswith('ChatFrame.lua'):
        start = text.index('function ChatFrame_SendTell(name, chatFrame)')
        end = text.index('\nfunction ChatFrame_ReplyTell(', start)
        text = text[:start] + '''function ChatFrame_SendTell(name, chatFrame)
  local editBox = ChatEdit_ChooseBoxForSend(chatFrame)
  if not editBox then return end
  if editBox ~= ChatEdit_GetActiveWindow() then ChatFrame_OpenChat("", chatFrame) end
  editBox:SetAttribute("chatType", "WHISPER")
  editBox:SetAttribute("tellTarget", name)
  editBox:SetText("")
  ChatEdit_UpdateHeader(editBox)
end
''' + text[end:]
        text = replace(text, 'text = text.." "..editBox:GetAttribute("tellTarget");',
                       'text = text.." "..CoACharacterNames.Quote(editBox:GetAttribute("tellTarget"));')
        text = replace(text, 'local player, note = strmatch(msg, "%s*([^%s]+)%s*(.*)");',
                       'local player, note = CoACharacterNames.ExtractRecipient(msg);\n\tif not player then return end')
        text = replace(text, 'function ChatEdit_ExtractTellTarget(editBox, msg)', '''function ChatEdit_ExtractTellTarget(editBox, msg)
  if msg:match('^%s*"') then
    local name, message = CoACharacterNames.ExtractRecipient(msg:gsub("^%s+", ""))
    if not name or not msg:match('"%s') then return false end
    editBox:SetAttribute("tellTarget", name)
    editBox:SetAttribute("chatType", "WHISPER")
    editBox:SetText(message)
    ChatEdit_UpdateHeader(editBox)
    return true
  end''')
        return replace(text, 'MAX_CHARACTER_NAME_BYTES = 48;', 'MAX_CHARACTER_NAME_BYTES = CoACharacterNames.maxBytes;')
    if path.endswith('MailFrame.xml'):
        text = replace(text, 'name="SendMailNameEditBox" letters="12"',
                       'name="SendMailNameEditBox" letters="25"')
        start = text.index('<EditBox name="SendMailNameEditBox"')
        end = text.index('</EditBox>', start)
        block = replace(text[start:end], 'x="109" y="20"', 'x="180" y="20"')
        block = replace(block, 'x="105" y="-46"', 'x="60" y="-46"')
        block = replace(block, 'self.autoCompleteParams = AUTOCOMPLETE_LIST.MAIL;',
                        'self:SetMaxBytes(CoACharacterNames.maxBytes);\n'
                        '                                self.autoCompleteParams = AUTOCOMPLETE_LIST.MAIL;')
        return text[:start] + block + text[end:]
    if path.endswith('StaticPopup.lua'):
        for dialog in ('ADD_MUTE', 'ADD_TEAMMEMBER', 'ADD_GUILDMEMBER', 'ADD_RAIDMEMBER'):
            start = text.index(f'StaticPopupDialogs["{dialog}"] = {{')
            end = text.index('\n};', start)
            block = replace(text[start:end], 'maxLetters = 12,',
                            'maxLetters = CoACharacterNames.maxLetters,\n\tmaxBytes = CoACharacterNames.maxBytes,')
            text = text[:start] + block + text[end:]
        return text
    raise ValueError(f'Unknown member {path}')


MEMBERS = (
    'Interface/GlueXML/GlueXML.toc', 'Interface/FrameXML/FrameXML.toc',
    'Interface/GlueXML/CharacterCreate.xml', 'Interface/GlueXML/CharacterCreate.lua',
    'Interface/GlueXML/CharacterSelect.xml', 'Interface/GlueXML/GlueDialog.lua',
    'Interface/SharedXML/Util/NewCharacterSetupUtil.lua', 'Interface/FrameXML/Util/GlobalOverwrites.lua',
    'Interface/FrameXML/ChatFrame.lua', 'Interface/FrameXML/ItemRef.lua',
    'Interface/FrameXML/MailFrame.xml', 'Interface/FrameXML/StaticPopup.lua',
)


def generate(source, output):
    if output.exists():
        raise FileExistsError('Choose a new output directory')
    if source.resolve() == output.resolve() or source.resolve() in output.resolve().parents:
        raise ValueError('Output must be outside the canonical source tree')
    members = {}
    diff = []
    for member in MEMBERS:
        original = (source / member).read_text(encoding='utf-8-sig')
        changed = transform(member, original)
        members[member] = changed
        diff.extend(difflib.unified_diff(original.splitlines(True), changed.splitlines(True),
                                         fromfile='a/' + member, tofile='b/' + member))
    members[SHARED] = (Path(__file__).parent / 'character-names/CharacterNames.lua').read_text(encoding='utf-8')
    for member, content in members.items():
        destination = output / member
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_text(content, encoding='utf-8', newline='\n')
    (output / 'interface.diff').write_text(''.join(diff), encoding='utf-8', newline='\n')
    return members


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    try:
        members = generate(args.source, args.output)
    except (OSError, ValueError) as error:
        parser.error(str(error))
    print(f'Prepared {len(members)} UI members in {args.output}. No client files installed.')
