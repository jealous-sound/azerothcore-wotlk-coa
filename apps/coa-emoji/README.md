# CoA chat emoji

Type `:thumbsup:` in chat and everyone with the client files sees 👍 as an inline icon, in the chat windows and in
the speech bubbles over characters' heads. There is a list of matches as you type and a picker button by the chat box.

- **Shortcodes.** `:melting_face:`, `:joy:`, `:+1:`, `:skull:` and 200-odd more (`emoji.json`). Case does not matter,
  and aliases work (`:+1:` is `:thumbsup:`).
- **Typeahead.** After `:` and two letters a list opens above the chat box. `Tab` walks it (`Shift+Tab` goes back),
  `Enter` inserts the highlighted one, `Esc` closes it, or click a row. `Enter` with nothing highlighted still sends
  the message, so a stray `:ok` is never swallowed. It only appears at the end of the text, never inside a word
  (`12:30`) or a link.
- **Picker.** A gold smiley button just right of every chat box opens a searchable grid. The button is made from the
  client's own chat button parts (square frame, pushed frame, mouse highlight, click sound), so it looks like the
  buttons next to it. It belongs to the chat box, so it is dim while the box is idle and bright while you type. Click
  inserts at the cursor, `Shift+Click` keeps it open for several in a row, mouse wheel or the bar scrolls, `Esc`
  closes.
- **Chat bubbles.** Bubbles over heads show the icons too, as long as chat bubbles are switched on in the game options.

## How it works

Everything is client side, and the worldserver is not involved. The message on the wire stays plain text
(`gg :fire:`), and each client swaps shortcodes for `|T...|t` texture markup when it displays the message, in the
same chat filter path the client already uses for `{skull}` raid icons.

That is deliberate. Expanding on the server would put 34 bytes of markup on the wire for every emoji, which hits the
255 byte chat limit after seven, and a client without the textures would show green squares. As plain text a message
costs what the player typed, and an old client just shows `:fire:`.

The code is FrameXML, not an addon: `ChatEmoji.lua` is loaded from `FrameXML.toc`, so it cannot be disabled in the
addon list. It does not edit `ChatFrame.lua`. It registers `ChatFrame_AddMessageEventFilter` filters, post-hooks
`ChatEdit_OnTextChanged` and `ChatEdit_ActivateChat`, and wraps `AutoCompleteEditBox_OnTabPressed`, `_OnEnterPressed`
and `_OnEscapePressed`. Those three are the client's own name completion protocol (`ChatEdit_On*Pressed` calls them
first and stops when they return true), so the emoji list behaves like name completion does.

### Chat bubbles

The engine builds bubbles from the raw message, so the chat filters never see them. A bubble is an unnamed child of
`WorldFrame` holding the `ChatBubble-Background` texture and one font string, which is how ElvUI finds them on this
client too. A hidden frame listens for say, yell, emote and party chat; when one arrives it looks at every known bubble
on each frame for a second (the bubble can appear a frame after the event, and the engine reuses bubble frames for new
messages) and runs the same `ChatEmoji.Render` over the bubble's text. Between chat events it does nothing.

This is written against how ElvUI reads and rewrites bubbles, and it has not been watched running in the game. If a
client build lays bubbles out differently, a bubble keeps the raw `:shortcode:` and nothing else is affected. It does
not resize bubbles. Whether the engine re-fits a bubble to new text is one to check in game; the icon is narrower than
its shortcode, so the worst case is a bubble a little wider than its text.

## Files

| Path | What |
| --- | --- |
| `emoji.json` | The emoji set: primary name, aliases, and the Twemoji code point of the art. Order is picker order. |
| `build.py` | Regenerates `ChatEmojiData.lua` and downloads and converts the art to TGA. |
| `client/Interface/FrameXML/ChatEmoji.lua` | Rendering, typeahead, picker and chat bubbles. |
| `client/Interface/FrameXML/ChatEmojiData.lua` | Generated from `emoji.json`. |
| `client/Interface/CoAEmoji/*.tga` | 64x64 32-bit textures, named by code point. |
| `client/Interface/CoAEmoji/button.tga` | The 32x32 smiley on the picker button. Drawn by `build.py`, not Twemoji. |
| `apply_client.py` | Installs or removes the above in a client folder. |
| `test_emoji.py`, `test_chat_emoji.lua`, `wow_stub.lua` | Tests. |
| `NOTICE.md` | Art and name licences. |

## Shipping it

The client files are the contents of `client/Interface/`, plus two lines in `FrameXML.toc`, right after
`FloatingChatFrame.xml`:

```
ChatEmojiData.lua
ChatEmoji.lua
```

For a client release, add that `Interface\` tree to `Data\patch-B.MPQ` with the rest of the client patch (the repository
does not hold `.mpq` files) and make the TOC edit there. To try it on a loose client instead:

```
python apps/coa-emoji/apply_client.py C:\ascension-live
python apps/coa-emoji/apply_client.py C:\ascension-live --revert
```

The client prefers loose files under `Interface\` over its archives, so `Data\` is not touched. The install reads the
TOC from `patch-B.MPQ` with `pip install mpyq`; `--toc FILE` patches an extracted `FrameXML.toc` when `mpyq` is not
installed. Revert restores any loose TOC that was there before.

The loose `FrameXML.toc` is a copy of `patch-B.MPQ`'s at install time and then hides it. After a client patch that
changes `patch-B.MPQ`, run `--revert` and install again, or the new patch's UI file list is ignored.

## Changing the set

Edit `emoji.json` and run `python apps/coa-emoji/build.py` (needs Pillow and network for new art; it also draws
`button.tga` when that file is missing, so delete it to redraw it). Names are lowercase
letters, digits, `_`, `+` and `-`, and each must be unique, aliases included. `codepoints` is the lowercase hex
Twemoji file name, without `fe0f`, for example `1f44d`. Multi-codepoint sequences joined with zero width joiners are
not supported. `python apps/coa-emoji/build.py --check` fails when the generated Lua or the textures disagree with
`emoji.json`.

## Tests

```
python apps/coa-emoji/test_emoji.py
```

The data, texture, TOC and installer checks use only the standard library. The Lua checks run `ChatEmoji.lua` under
Lua 5.1 against `wow_stub.lua`, a stripped down client with the same call order as the chat edit box, and need
`pip install lupa`; without it they are skipped. The stub has only the widget methods the 3.3.5 client has, so a
retail-only call fails the test.

## Not covered

- A chat window replaced by an addon that does not use `ChatFrame_MessageEventHandler` will not convert.
- The edit box shows the shortcode until the message is sent; edit boxes cannot draw inline textures.
- With an input method (IME) language the client also shows its own language button at the right end of the chat box,
  where the emoji button sits. That button is off for other clients.
- Anything that displays chat text without going through the chat frames (mail, guild MOTD) is unchanged.
