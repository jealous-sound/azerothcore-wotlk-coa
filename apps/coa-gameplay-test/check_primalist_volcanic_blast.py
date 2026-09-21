#!/usr/bin/env python3
CLI_DESCRIPTION = """Verify both Volcanic Blast ranks copy resolved damage onto nearby targets."""

import argparse
import json
from pathlib import Path


def check(directory):
    summary = json.loads((directory / 'summary.json').read_text(encoding='utf-8'))
    result = json.loads((directory / 'result.json').read_text(encoding='utf-8'))
    if summary['status'] != 'passed' or result['status'] != 'passed':
        raise ValueError('A completed native scenario pass is required')
    values = {step['label']: int(float(step['actual'])) for step in result['steps'] if 'actual' in step}
    for phase, percent in [('rank1', 20), ('rank2', 40)]:
        expected = values[phase + '_source'] * percent // 100
        for target in ('target', 'near'):
            actual = values[phase + '_' + target]
            if actual != expected:
                raise ValueError(f'{phase}, {target}: {actual}, expected {expected}')
        print(f'{phase}: {expected} on each nearby target, {percent}% of resolved critical damage')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=CLI_DESCRIPTION)
    parser.add_argument('result_directory', type=Path)
    check(parser.parse_args().result_directory)
