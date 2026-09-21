"""Validate cross-metric Primalist contracts from native gameplay observations."""
import json
import sys
from pathlib import Path


folder = Path(sys.argv[1])
mode = sys.argv[2]
result = json.loads((folder / 'result.json').read_text(encoding='utf-8'))
summary = json.loads((folder / 'summary.json').read_text(encoding='utf-8'))
scenario = json.loads((folder / 'scenario.json').read_text(encoding='utf-8'))
assert result['status'] == summary['status'] == 'passed'
values = {scenario['steps'][int(step['index'])]['save_as']: float(step['actual'])
          for step in result['steps'] if step['action'] == 'snapshot'}

if mode in ['douse', 'sacred-grove']:
    coefficient = .08 if mode == 'douse' else 1
    for phase in ['base', 'ap', 'sp']:
        expected = int(1000 + values[phase + 'ap'] * coefficient)
        assert abs(values[phase + 'query'] - expected) <= 1, (phase, values, expected)
    assert values['apap'] > values['baseap']
    assert values['apquery'] > values['basequery']
    assert values['spquery'] == values['apquery']
    if mode == 'sacred-grove' and 'ally_max_mana' in values:
        assert values['mana_recovery'] == int((values['ally_max_mana'] - 1000) * .03)
    print(mode, 'native AP coefficient and zero additional SP coefficient verified')
elif mode == 'wildheart':
    expected = int((values['max_mana'] - 1000 + values['wildheart_cost']) * .05)
    assert values['mana_recovery'] == expected, (values, expected)
    print('Wildheart: five percent of missing Mana after the ordinary cast cost')
elif mode == 'ancient-war':
    for phase, percent in [('baseline', 20), ('active', 26), ('expired', 20)]:
        hit = values[phase + 'hit']
        assert hit > 0
        expected = int(hit) * percent // 100
        assert values[phase + 'heal'] == values[phase + 'copy'] == expected, (phase, values, expected)
        print(phase, hit, 'damage;', expected, 'per Hammer recipient')
elif mode == 'neptulon-wrath':
    expected = int(values['caster_ap'] * .35)
    assert expected > 0 and values['caster_ap'] != values['ally_ap']
    for actor in ['primalist', 'ally']:
        assert values[actor + '_amount'] == expected, (actor, values, expected)
        assert values[actor + '_hit'] in [expected, expected * 1.5, int(expected * 1.5)], (actor, values)
    print('Neptulon Wrath:', expected, 'snapshotted damage per direct hit for both players')
elif mode == 'sacred-grove-cap':
    recipients = [value for key, value in values.items() if key.startswith('recipient_')]
    assert len(recipients) == 13 and all(value in [0, 1] for value in recipients)
    assert sum(recipients) == 12, values
    print('Sacred Grove: exactly twelve of thirteen eligible recipients')
else:
    raise ValueError(mode)
