"""Installs the CoA chat emoji files into a game client folder, or removes them again.

    python apps/coa-emoji/apply_client.py C:\\ascension-live            install
    python apps/coa-emoji/apply_client.py C:\\ascension-live --revert   remove

The client prefers loose files under Interface\\ over its archives, so nothing in Data\\ is touched. The new files go
into Interface\\FrameXML and Interface\\CoAEmoji, and FrameXML.toc gets two lines after FloatingChatFrame.xml. For a
release, add the same Interface\\ tree to patch-B.MPQ and make the same two-line TOC edit there instead.
"""

import argparse
from pathlib import Path
import shutil
import sys

ROOT = Path(__file__).resolve().parent
SOURCE = ROOT / 'client' / 'Interface'
TOC = Path('Interface') / 'FrameXML' / 'FrameXML.toc'
MPQ = Path('Data') / 'patch-B.MPQ'
MPQ_TOC = 'Interface\\FrameXML\\FrameXML.toc'
ANCHOR = 'FloatingChatFrame.xml'
ADDED = ['ChatEmojiData.lua', 'ChatEmoji.lua']
BACKUP_SUFFIX = '.coa-emoji-backup'


def split_lines(text):
    newline = '\r\n' if '\r\n' in text else '\n'
    return text.split(newline), newline


def patch_toc(text):
    lines, newline = split_lines(text)
    names = [line.strip() for line in lines]
    if ADDED[-1] in names:
        return text
    anchors = [index for index, name in enumerate(names) if name.lower() == ANCHOR.lower()]
    if len(anchors) != 1:
        raise ValueError(f'FrameXML.toc must list {ANCHOR} exactly once, found {len(anchors)}')
    lines[anchors[0] + 1:anchors[0] + 1] = ADDED
    return newline.join(lines)


def unpatch_toc(text):
    lines, newline = split_lines(text)
    return newline.join(line for line in lines if line.strip() not in ADDED)


def read_toc_from_mpq(path):
    try:
        import mpyq
    except ImportError:
        raise SystemExit('Reading the TOC from patch-B.MPQ needs "pip install mpyq". '
                         'Or extract FrameXML.toc yourself and pass it with --toc.')
    data = mpyq.MPQArchive(str(path), listfile=False).read_file(MPQ_TOC)
    if data is None:
        raise SystemExit(f'{MPQ_TOC} was not found in {path}')
    return data.decode('utf-8')


def source_files():
    return sorted(path for path in SOURCE.rglob('*') if path.is_file())


def install(client, toc_text):
    for source in source_files():
        target = client / 'Interface' / source.relative_to(SOURCE)
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source, target)
    toc_path = client / TOC
    backup = toc_path.with_name(toc_path.name + BACKUP_SUFFIX)
    if not backup.exists():
        # An empty backup means there was no loose TOC before, so revert deletes ours instead of restoring one.
        backup.write_bytes(toc_path.read_bytes() if toc_path.exists() else b'')
    toc_path.write_text(patch_toc(toc_text), encoding='utf-8', newline='')


def revert(client):
    toc_path = client / TOC
    backup = toc_path.with_name(toc_path.name + BACKUP_SUFFIX)
    if backup.exists():
        original = backup.read_bytes()
        if original:
            toc_path.write_bytes(original)
        else:
            toc_path.unlink(missing_ok=True)
        backup.unlink()
    elif toc_path.exists():
        toc_path.write_text(unpatch_toc(toc_path.read_text(encoding='utf-8')), encoding='utf-8', newline='')
    directories = set()
    for source in source_files():
        target = client / 'Interface' / source.relative_to(SOURCE)
        target.unlink(missing_ok=True)
        directories.add(target.parent)
    for directory in sorted(directories, key=lambda path: len(path.parts), reverse=True):
        if directory.is_dir() and not any(directory.iterdir()):
            directory.rmdir()


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('client', type=Path, help='game client folder (the one holding Data and Interface)')
    parser.add_argument('--revert', action='store_true', help='remove the emoji files and restore FrameXML.toc')
    parser.add_argument('--toc', type=Path, help='FrameXML.toc to patch, instead of reading it from patch-B.MPQ')
    args = parser.parse_args(argv)
    if not (args.client / 'Data').is_dir():
        print(f'{args.client} has no Data folder; is this the game client?', file=sys.stderr)
        return 1
    if args.revert:
        revert(args.client)
        print('Removed the chat emoji files.')
        return 0
    loose = args.client / TOC
    if args.toc:
        toc_text = args.toc.read_text(encoding='utf-8')
    elif loose.is_file():
        toc_text = loose.read_text(encoding='utf-8')
    else:
        toc_text = read_toc_from_mpq(args.client / MPQ)
    install(args.client, toc_text)
    print(f'Installed {len(source_files())} files under {args.client / "Interface"}.')
    return 0


if __name__ == '__main__':
    sys.exit(main())
