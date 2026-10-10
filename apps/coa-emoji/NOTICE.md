# Third-party material in apps/coa-emoji

## Emoji graphics

`client/Interface/CoAEmoji/*.tga` are derived from [Twemoji](https://github.com/jdecked/twemoji) v17.0.3.

Copyright 2019 Twitter, Inc and other contributors. The graphics are licensed under
[CC-BY 4.0](https://creativecommons.org/licenses/by/4.0/). They were resized to 64x64 pixels and converted from PNG to
uncompressed TGA; nothing else was changed.

`client/Interface/CoAEmoji/button.tga` is the exception. It is not Twemoji: `build.py` draws it, and it is part of this
project. The picker button also uses the game client's own `Interface\Buttons\UI-SquareButton-*` and
`UI-Common-MouseHilight` textures, which are referenced by path and not copied here.

## Shortcode names

The names and aliases in `emoji.json` follow [gemoji](https://github.com/github/gemoji) (MIT, Copyright GitHub, Inc).
