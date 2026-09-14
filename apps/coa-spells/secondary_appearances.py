"""Prepare the captured appearance dependencies for issues #51/#82 in separate DBC copies.

Does not modify runtime data or MPQs. The matching pending SQL supplies Creature
entries 377942 (Conduit), 421460 (Natural Disguise), 462071 (Hemostasis) and
346852 (Ancient of Lore), plus Animated Blood's summons. Both server and client
need these records when deploying; SQL DBC overrides alone cannot fix client models.
"""
import argparse
from pathlib import Path

from runemaster_travel import merge_rows


ROWS = {
    'CreatureDisplayInfo': [
        [93307, 10899, 0, 0, 4.0, 255, 'bloodelemental', '', '', '', 0, 0, 0, 0, 0, 0],
        [236827, 110722, 0, 0, 0.20000000298023224, 255, 'bloodticklarva', '', '', '', 0, 0, 0, 0, 0, 0],
        [94074, 8498, 6696, 0, 1.0, 255, '', '', '', '', 0, 0, 0, 0, 0, 0],
        [111213, 187112, 0, 0, 0.699999988079071, 255, 'AncientofLoreSkin', '', '', '', 4, 0, 94, 0, 0, 0],
        [421460, 421460, 0, 0, 1.0, 255, '', '', '', '', 0, 0, 0, 0, 0, 0],
        [462071, 462071, 0, 0, 1.0, 255, '', '', '', '', 0, 0, 0, 0, 0, 0],
    ],
    'CreatureModelData': [
        [10899, 3, r'creature\bloodelemental\bloodelemental.mdx', 1, 1.0, 1, 0, 18.0, 12.0, 1.0,
         0, 0, 6118, 0, 2.031280040740967, 1.0, 0.0, -1.1306400299072266, -1.4304399490356445,
         -0.5066109895706177, 1.6768100261688232, 1.6711900234222412, 3.9143900871276855, 1.0, 0.0, 1.0, 0.0, 0.0],
        [110722, 0, r'Creature\bloodticklarva\bloodticklarva.mdx', 0, 1.0, 1, 5, 10.0, 6.0, 1.0,
         0, 0, 0, 3294, 0.0, 2.0, 0.0, -5.0, -2.0, 0.0, 3.0, 2.0, 4.0, 1.0, 1.0, 0.0, 0.0, 0.0],
        [8498, 4097, r'creature\lightningelemental\lightningelemental.mdx', 1, 1.0, 18, 4,
         18.0, 12.0, 1.0, 0, 0, 5535, 0, 2.031280040740967, 1.0, 0.0, -0.9996770024299622,
         -1.507949948310852, 0.9417759776115417, 0.9307060241699219, 1.5024399757385254,
         4.271399974822998, 1.0, 0.0, 1.0, 0.0, 0.0],
        [187112, 0, r'Creature\AncientOfLore\AncientofLore.mdx', 4, 1.0, -1, 1,
         18.0, 12.0, 1.0, 0, 0, 0, 140, 0.6944000124931335, 2.0829999446868896, 0.0,
         -2.7309000492095947, -3.889509916305542, -0.1058100014925003, 3.599950075149536,
         3.886080026626587, 9.049429893493652, 1.0, 1.0, 0.0, 0.0, 0.0],
        [421460, 0, r'world\lordaeron\aeriepeaks\passivedoodads\bushes\aeriepeaksbush02.mdx', 1, 1.0,
         0, 0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0],
        [462071, 0, r'SPELLS\bleedingaltar_orb_state.m2', 1, 1.0, 0, 0, 0.0, 0.0, 0.0, 0, 0, 0, 0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0],
    ],
}


def transform(raw, table):
    return merge_rows(raw, ROWS[table], table)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--table', choices=ROWS, required=True)
    parser.add_argument('--input', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    if args.input.resolve() == args.output.resolve():
        parser.error('Use a separate output path')
    result = transform(args.input.read_bytes(), args.table)
    if args.output.exists():
        if args.output.read_bytes() != result:
            parser.error('Existing candidate differs; preserve it and select another output path')
    else:
        with args.output.open('xb') as output:
            output.write(result)


if __name__ == '__main__':
    main()
