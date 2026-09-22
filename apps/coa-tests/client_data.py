import os
from pathlib import Path


SERVER_DBC_DIR = Path(__file__).resolve().parents[2] / 'env/dist/data/dbc'


def dbc_dir():
    directory = Path(os.environ.get('COA_DBC_DIR', SERVER_DBC_DIR))
    if not (directory / 'Spell.dbc').is_file():
        raise SystemExit(f'No CoA client DBCs in {directory}. Put them in the worldserver DataDir/dbc '
                         f'({SERVER_DBC_DIR} for the default install) or set COA_DBC_DIR.')
    return directory
