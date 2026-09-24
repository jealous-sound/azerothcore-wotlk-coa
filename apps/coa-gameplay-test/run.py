#!/usr/bin/env python3
CLI_DESCRIPTION = """Run gameplay scenarios in a dedicated worldserver with disposable local databases."""

import argparse
import configparser
from dataclasses import dataclass, field
import hashlib
import json
import math
import os
from pathlib import Path
import re
import secrets
import shutil
import signal
import socket
import subprocess
import sys
import tempfile
import time

from world_cache import WorldCache, input_fingerprint

ROOT = Path(__file__).resolve().parents[2]
CREATE_FLAGS = subprocess.CREATE_NO_WINDOW if os.name == 'nt' else 0
IDENTIFIER = re.compile(r'[A-Za-z_][A-Za-z0-9_]*\Z')
ACTOR_ID = re.compile(r'[a-z][a-z0-9_]{0,31}\Z')
LOCAL_HOSTS = {'127.0.0.1', 'localhost', '::1'}
METRICS = {
    'moving', 'water_walk', 'forced_forward', 'distance_2d', 'cast_remaining_ms', 'cast_pushback_ms',
    'melee_damage_count',
    'pet_power', 'pet_max_power', 'spell_energize_count', 'spell_energize_total',
    'xp', 'next_level_xp', 'skill_value',
    'view_level', 'sent_level', 'sent_max_health', 'quest_level', 'quest_xp',
    'health', 'health_pct', 'max_health', 'power', 'max_power', 'alive', 'combat', 'casting', 'level',
    'aura', 'aura_stacks', 'aura_charges', 'aura_duration_ms', 'aura_amount', 'aura_positive',
    'knows_spell', 'has_talent', 'talent_points', 'cooldown_ms', 'global_cooldown_ms', 'spell_charges',
    'action_button', 'item_count', 'carried_item_count', 'carried_pool_item_count', 'carried_variant_item_count',
    'pool_variant_count', 'pool_retired_item_count', 'pool_row_count', 'pool_item_present',
    'cache_token_count', 'cache_token_stage', 'cache_token_present',
    'free_inventory_slots', 'mail_count', 'mail_item_count', 'mail_has_item',
    'mail_pool_item_count', 'notifications', 'notification_contains',
    'bank_bag_slots', 'bank_shows',
    'system_messages',
    'system_message_contains', 'challenge_start_responses', 'challenge_start_code',
    'owned_creature_scale', 'unit_scale', 'token_count', 'item_sell_price', 'creature_model_scale', 'creature_model_display',
    'taxi_node', 'pet_entry', 'pet_aura_stacks', 'pet_aura_duration_ms', 'pet_is_banker', 'pet_display',
    'pet_scale', 'owned_creature_count',
    'charm_entry', 'charm_aura_stacks', 'controls_self', 'private_instance',
    'dynamic_object', 'dynamic_object_duration_ms', 'gossip_options',
    'owned_gameobject_count', 'gameobject_remaining_ms', 'at_homebind',
    'spellbook_rows', 'spellbook_offers_spell', 'spellbook_covers_spell', 'spellbook_learned_alerts',
    'spellbook_buy_succeeded', 'spellbook_buy_failed',
    'spellbook_buys_granted', 'spellbook_unannounced_buys', 'spellbook_misannounced_buys',
    'spellbook_notify_rows', 'spellbook_notified_spells', 'spellbook_unnotified_buys',
    'trainer_list_packets', 'trainer_window_rows', 'trainer_window_state', 'trainer_window_ability',
    'spellbook_superseded_packets', 'spellbook_superseded_for',
    'spellbook_cues_in_last_buy', 'spellbook_last_buy_cued',
    'spellbook_silent_buys', 'spellbook_multi_announced_buys',
    'cast_speed_multiplier', 'spell_crit_chance', 'spell_power_cost', 'spell_damage_done', 'melee_damage_done',
    'who_count', 'who_class', 'player_name', 'name_lookup', 'loot_count', 'loot_entry', 'loot_received',
    'loot_gold', 'loot_bloodforged', 'nearby_gameobject_count', 'nearby_creature_count', 'carried_money',
    'quest_rewarded', 'spell_damage_taken', 'melee_damage_taken', 'spell_healing_taken',
    'spell_hit_bonus_taken', 'rooted', 'stunned', 'spell_cast_count', 'spell_go_count', 'cast_failure',
    'stealth_detection', 'can_detect',
    'quest_status', 'quest_takeable', 'quest_objective_count', 'dialog_status',
    'ball_offer_count', 'ball_offers_quest',
    'ball_carried_count', 'ball_carried_quest', 'ball_turn_in_count', 'ball_turn_in_quest',
    'gossip_text',
    'stat', 'attack_power', 'ranged_attack_power', 'armor', 'weapon_damage_min', 'resistance',
    'attack_time_ms', 'run_speed_rate', 'display_id',
    'aura_amplitude_ms', 'melee_crit_chance', 'dodge_chance', 'parry_chance', 'expertise', 'combat_rating',
    'spell_modifier', 'spell_cast_time_ms', 'spell_max_range', 'spell_max_stacks', 'spell_healing_done',
    'aura_crit_chance', 'aura_script_value', 'melee_hit_chance', 'spell_hit_chance', 'spell_power',
    'spell_done_crit_chance', 'spell_done_crit_chance_scripted', 'melee_spell_damage_done', 'script_melee_damage_taken',
    'script_spell_damage_taken', 'script_periodic_damage_taken', 'script_heal_received', 'spell_effect_value',
    'block_chance', 'block_value', 'critical_block_chance', 'spell_critical_damage', 'armor_reduced_damage',
    'aoe_damage_taken', 'reputation_gain', 'spell_immune', 'spell_effect_immune', 'melee_attack_count',
    'spell_damage_count', 'spell_damage_total', 'spell_uses_armor',
    'spell_heal_count', 'spell_heal_total', 'spell_effective_heal_total',
    'pet_aura_amount', 'pet_aura_amplitude_ms', 'pet_max_health', 'pet_attack_power', 'pet_run_speed_rate',
    'distance', 'spell_proc_count', 'temporary_spell_replacement',
}
PLAYER_STAT_METRICS = {
    'spell_go_count',
    'global_cooldown_ms',
    'melee_damage_count',
    'pet_power', 'pet_max_power', 'spell_energize_count', 'spell_energize_total',
    'melee_crit_chance', 'dodge_chance', 'parry_chance', 'expertise', 'combat_rating',
    'spell_modifier', 'spell_cast_time_ms', 'spell_max_range', 'spell_max_stacks', 'spell_healing_done',
    'melee_hit_chance', 'spell_hit_chance', 'spell_power', 'spell_done_crit_chance', 'spell_done_crit_chance_scripted', 'melee_spell_damage_done',
    'script_melee_damage_taken', 'script_spell_damage_taken', 'script_periodic_damage_taken',
    'script_heal_received', 'spell_effect_value',
    'block_chance', 'block_value', 'critical_block_chance', 'spell_critical_damage', 'armor_reduced_damage',
    'aoe_damage_taken', 'reputation_gain', 'spell_immune', 'spell_effect_immune', 'melee_attack_count',
    'spell_damage_count', 'spell_damage_total', 'spell_uses_armor',
    'spell_heal_count', 'spell_heal_total', 'spell_effective_heal_total',
    'pet_aura_amount', 'pet_aura_amplitude_ms', 'pet_aura_duration_ms', 'pet_max_health', 'pet_attack_power',
    'pet_run_speed_rate',
}
METRIC_FIELDS = {'actor', 'metric', 'spell', 'power', 'caster', 'effect', 'item', 'entry', 'button',
                 'relative_to', 'ratio_to', 'target', 'quest', 'id', 'stat', 'school', 'hand', 'rating', 'op',
                 'base', 'key', 'index', 'pet', 'critical', 'target_pet', 'periodic', 'name', 'text',
                 'min_distance', 'owner_display', 'skill', 'cache', 'table', 'exclude'}
ACTIONS = {
    'stop_attack': ({'actor'}, {'actor'}),
    'set_moving': ({'actor', 'enabled'}, {'actor', 'enabled'}),
    'level_scaling_packet': ({'actor', 'value'}, {'actor', 'value'}),
    'console': ({'command'}, {'command'}),
    'command': ({'actor', 'command'}, {'actor', 'command'}),
    'wait': ({'ms'}, {'ms'}),
    'snapshot': ({'actor', 'metric', 'save_as'}, METRIC_FIELDS | {'save_as'}),
    'assert': ({'actor', 'metric'}, METRIC_FIELDS | {'equals', 'min', 'max', 'within_ms'}),
    'learn': ({'actor', 'spell'}, {'actor', 'spell'}),
    'set_action_button': ({'actor', 'spell', 'button'}, {'actor', 'spell', 'button'}),
    'grant_resource': ({'actor', 'spell'}, {'actor', 'spell', 'amount'}),
    'unlearn': ({'actor', 'spell'}, {'actor', 'spell', 'all_specs'}),
    'money': ({'actor', 'copper'}, {'actor', 'copper'}),
    'set_aura': ({'actor', 'spell', 'stacks'}, {'actor', 'spell', 'stacks', 'pet'}),
    'cancel_aura': ({'actor', 'spell'}, {'actor', 'spell'}),
    'cast': ({'actor', 'spell'}, {'actor', 'spell', 'target', 'destination'}),
    'attack': ({'actor', 'target'}, {'actor', 'target', 'pet'}),
    'pvp': ({'actor', 'enabled'}, {'actor', 'enabled'}),
    'group': ({'actor', 'target'}, {'actor', 'target'}),
    'cast_charm': ({'actor', 'spell'}, {'actor', 'spell', 'target'}),
    'gossip_hello': ({'actor'}, {'actor', 'target'}),
    'banker_activate': ({'actor'}, {'actor', 'target', 'owner', 'entry'}),
    'start_challenge': ({'actor', 'challenge', 'level'}, {'actor', 'challenge', 'level'}),
    'area_trigger': ({'actor', 'id'}, {'actor', 'id'}),
    'trainer_buy': ({'actor', 'spell'}, {'actor', 'spell', 'target'}),
    'gossip_select': ({'actor', 'option'}, {'actor', 'option'}),
    'who': ({'actor'}, {'actor', 'target', 'race_mask', 'class_mask'}),
    'open_item': ({'actor', 'item'}, {'actor', 'item'}),
    'collect_loot': ({'actor'}, {'actor'}),
    'close_loot': ({'actor'}, {'actor'}),
    'set_money': ({'actor', 'value'}, {'actor', 'value'}),
    'set_phase': ({'actor'}, {'actor', 'value'}),
    'use_nearby_gameobject': ({'actor', 'entry'}, {'actor', 'entry'}),
    'attack_nearby': ({'actor', 'entry'}, {'actor', 'entry', 'kill'}),
    'loot_nearby': ({'actor', 'entry'}, {'actor', 'entry'}),
    'loot_creature': ({'actor', 'target'}, {'actor', 'target'}),
    'loot_slot': ({'actor'}, {'actor', 'slot'}),
    'loot_money': ({'actor'}, {'actor'}),
    'prepare_quest': ({'actor', 'quest'}, {'actor', 'quest', 'complete'}),
    'reward_quest': ({'actor', 'quest'}, {'actor', 'quest', 'choice'}),
    'restore_quest_spells': ({'actor'}, {'actor'}),
    'login_hooks': ({'actor'}, {'actor'}),
    'talent': ({'actor', 'talent', 'rank'}, {'actor', 'talent', 'rank'}),
    'reset_talents': ({'actor'}, {'actor'}),
    'add_item': ({'actor', 'item'}, {'actor', 'item', 'count'}),
    'fill_bags': ({'actor'}, {'actor', 'slots'}),
    'equip': ({'actor', 'item', 'slot'}, {'actor', 'item', 'slot'}),
    'use_item': ({'actor', 'item', 'spell'}, {'actor', 'item', 'spell', 'target', 'destination'}),
    'use_gameobject': ({'actor', 'entry'}, {'actor', 'entry'}),
    'set_skill': ({'actor', 'skill', 'value', 'maximum'}, {'actor', 'skill', 'value', 'maximum'}),
    'gather_skill': ({'actor', 'skill', 'required'}, {'actor', 'skill', 'required'}),
    'set_xp_enabled': ({'actor', 'enabled'}, {'actor', 'enabled'}),
    'set_level': ({'actor', 'value'}, {'actor', 'value'}),
    'set_health': ({'actor', 'value'}, {'actor', 'value', 'pet', 'maximum'}),
    'reset_cooldown': ({'actor', 'spell'}, {'actor', 'spell'}),
    'restore_charges': ({'actor', 'spell'}, {'actor', 'spell'}),
    'set_power': ({'actor', 'value'}, {'actor', 'value', 'power', 'pet'}),
    'teleport': ({'actor', 'map', 'x', 'y', 'z'}, {'actor', 'map', 'x', 'y', 'z', 'o'}),
    'quest_accept': ({'actor', 'quest', 'entry'}, {'actor', 'quest', 'entry'}),
    'quest_open': ({'actor', 'quest', 'entry'}, {'actor', 'quest', 'entry'}),
    'quest_click': ({'actor', 'quest', 'entry'}, {'actor', 'quest', 'entry'}),
    'quest_complete': ({'actor', 'quest'}, {'actor', 'quest'}),
    'quest_turn_in': ({'actor', 'quest', 'entry'}, {'actor', 'quest', 'entry', 'reward'}),
}


def require(condition, message):
    if not condition:
        raise ValueError(message)


def keys(value, required, allowed, where):
    require(isinstance(value, dict), f'{where}: expected an object')
    require(required <= value.keys(), f'{where}: missing {sorted(required - value.keys())}')
    require(value.keys() <= allowed, f'{where}: unknown fields {sorted(value.keys() - allowed)}')


def number(value, where, minimum=None, maximum=None, integer=False):
    require(type(value) in (int, float) and math.isfinite(value), f'{where}: expected a finite number')
    require(not integer or type(value) is int, f'{where}: expected an integer')
    require(minimum is None or value >= minimum, f'{where}: value is too small')
    require(maximum is None or value <= maximum, f'{where}: value is too large')


def unique_object(pairs):
    result = {}
    for key, value in pairs:
        require(key not in result, f'Duplicate JSON key: {key}')
        result[key] = value
    return result


def read_json(path):
    return json.loads(Path(path).read_text(encoding='utf-8-sig'), object_pairs_hook=unique_object)


def validate(scenario):
    keys(scenario, {'schema', 'name', 'players', 'steps'},
         {'schema', 'name', 'players', 'creatures', 'steps', 'timeout_ms', 'location', 'contract'}, 'scenario')
    require(type(scenario['schema']) is int and scenario['schema'] == 1, 'Unsupported scenario schema')
    require(isinstance(scenario['name'], str) and scenario['name'].strip(), 'Scenario needs a name')
    number(scenario.get('timeout_ms', 90000), 'timeout_ms', 1, 600000, True)
    players = scenario['players']
    creatures = scenario.get('creatures', [])
    require(isinstance(players, list) and 1 <= len(players) <= 8, 'Expected 1..8 players')
    require(isinstance(creatures, list) and len(creatures) <= 8, 'Expected at most eight creatures')
    player_ids = set()
    actor_ids = set()
    for player in players:
        keys(player, {'id', 'race', 'class'},
             {'id', 'race', 'class', 'level', 'bot', 'spell_hit_rating', 'spell_crit_rating',
              'melee_crit_rating', 'ranged_hit_rating', 'melee_hit_rating', 'expertise_rating',
              'allow_regeneration', 'name'}, 'player')
        identity = player['id']
        require(isinstance(identity, str) and ACTOR_ID.fullmatch(identity), 'Invalid player id')
        require(identity not in actor_ids, 'Duplicate actor id')
        actor_ids.add(identity)
        player_ids.add(identity)
        if 'name' in player:
            require(isinstance(player['name'], str) and 1 <= len(player['name']) <= 25
                    and len(player['name'].encode('utf-8')) <= 47, 'Invalid fixture character name')
        for key in ('race', 'class'):
            number(player[key], key, 1, 255, True)
        number(player.get('level', 80), 'level', 1, 255, True)
        require(type(player.get('bot', False)) is bool, 'bot must be boolean')
        number(player.get('spell_hit_rating', 0), 'spell_hit_rating', 0, 100000, True)
        number(player.get('spell_crit_rating', 0), 'spell_crit_rating', 0, 100000, True)
        number(player.get('melee_crit_rating', 0), 'melee_crit_rating', 0, 100000, True)
        number(player.get('ranged_hit_rating', 0), 'ranged_hit_rating', 0, 100000, True)
        number(player.get('melee_hit_rating', 0), 'melee_hit_rating', 0, 100000, True)
        number(player.get('expertise_rating', 0), 'expertise_rating', 0, 100000, True)
        require(type(player.get('allow_regeneration', True)) is bool, 'allow_regeneration must be boolean')
    for creature in creatures:
        keys(creature, {'id', 'owner', 'entry'},
             {'id', 'owner', 'entry', 'distance', 'faction', 'level', 'health', 'level_scaling'}, 'creature')
        identity = creature['id']
        require(isinstance(identity, str) and ACTOR_ID.fullmatch(identity), 'Invalid creature id')
        require(identity not in actor_ids, 'Duplicate actor id')
        actor_ids.add(identity)
        require(creature['owner'] in player_ids, 'Creature owner must be a player')
        for key, default in (('entry', None), ('faction', 14), ('health', 100000)):
            number(creature.get(key, default), key, 1, 2**31 - 1, True)
        number(creature.get('level', 80), 'creature level', 1, 255, True)
        number(creature.get('distance', 3), 'distance', 0, 100)
        require(isinstance(creature.get('level_scaling', False), bool), 'level_scaling must be a boolean')
    if 'location' in scenario:
        location = scenario['location']
        keys(location, {'map', 'x', 'y', 'z'}, {'map', 'x', 'y', 'z', 'o', 'ignore_access'}, 'location')
        require(type(location.get('ignore_access', False)) is bool, 'ignore_access must be boolean')
        number(location['map'], 'map', 0, 2**32 - 1, True)
        for key in ('x', 'y', 'z'):
            number(location[key], key, -17000, 17000)
        number(location.get('o', 0), 'orientation', 0, 2 * math.pi)
    steps = scenario['steps']
    require(isinstance(steps, list) and 1 <= len(steps) <= 10000, 'Expected 1..10000 steps')
    snapshots = {}
    assertions = 0
    for index, step in enumerate(steps):
        where = f'step {index}'
        require(isinstance(step, dict) and step.get('action') in ACTIONS, f'{where}: unknown action')
        action = step['action']
        required, allowed = ACTIONS[action]
        keys(step, required | {'action'}, allowed | {'action', 'label'}, where)
        if action in {'console', 'command'}:
            require(isinstance(step['command'], str) and step['command'].strip()
                    and '\n' not in step['command'] and '\r' not in step['command'],
                    f'{where}: expected one command')
            if action == 'command':
                require(step['command'].startswith('.') and len(step['command']) > 1,
                        f'{where}: player command must start with a dot')
        if 'actor' in step:
            require(step['actor'] in actor_ids, f'{where}: unknown actor')
            require(action in {'snapshot', 'assert', 'set_health'} or step['actor'] in player_ids,
                    f'{where}: action needs a player')
        for key in ('target', 'caster'):
            if key in step:
                require(step[key] in actor_ids, f'{where}: unknown {key}')
        if 'destination' in step:
            destination = step['destination']
            keys(destination, {'x', 'y', 'z'}, {'x', 'y', 'z'}, f'{where}.destination')
            for key in ('x', 'y', 'z'):
                number(destination[key], f'{where}.destination.{key}', -17000, 17000)
        if action == 'teleport':
            number(step['map'], f'{where}.map', 0, 2**31 - 1, True)
            for key in ('x', 'y', 'z', 'o'):
                if key in step:
                    number(step[key], f'{where}.{key}', -17000, 17000)
        for key in ('spell', 'item', 'talent', 'count', 'entry', 'quest', 'id', 'challenge', 'level'):
            if key in step:
                number(step[key], f'{where}.{key}', 1, 2**31 - 1, True)
        for key, maximum in (('rank', 4), ('effect', 2), ('slot', 18), ('power', 6), ('choice', 5),
                             ('reward', 5), ('option', 2**32 - 1)):
            if key in step:
                number(step[key], f'{where}.{key}', 0, maximum, True)
        if 'stacks' in step:
            number(step['stacks'], f'{where}.stacks', 0, 255, True)
        if action in ('set_aura', 'attack', 'set_health', 'set_power') and 'pet' in step:
            require(type(step['pet']) is bool, f'{where}: pet must be boolean')
            require(step['actor'] in player_ids, f'{where}: pet fixture needs a player')
        if action in {'pvp', 'set_moving'}:
            require(type(step['enabled']) is bool, f'{where}: enabled must be boolean')
        if 'all_specs' in step:
            require(type(step['all_specs']) is bool, f'{where}: all_specs must be boolean')
        for key in ('race_mask', 'class_mask'):
            if key in step:
                number(step[key], f'{where}.{key}', 0, 2**32 - 1, True)
        if action == 'who' and 'target' in step:
            require(step['target'] in player_ids, f'{where}: Who name filter needs a player')
        if action == 'group':
            require(step['target'] in player_ids and step['target'] != step['actor'],
                    f'{where}: group needs another player')
        for key in ('ms', 'within_ms'):
            if key in step:
                number(step[key], f'{where}.{key}', 0, scenario.get('timeout_ms', 90000), True)
        if action in {'set_skill', 'gather_skill'}:
            number(step['skill'], f'{where}.skill', 1, 65535, True)
        if action == 'set_skill':
            number(step['maximum'], f'{where}.maximum', 1, 450, True)
            number(step['value'], f'{where}.value', 0, step['maximum'], True)
        if action == 'gather_skill':
            require(step['skill'] in {182, 186, 393, 633, 773, 755}, f'{where}: unsupported gathering skill')
            number(step['required'], f'{where}.required', 0, 450, True)
        if action == 'set_xp_enabled':
            require(type(step['enabled']) is bool, f'{where}: enabled must be boolean')
        if action == 'money':
            number(step['copper'], f'{where}.copper', 1, 2**31 - 1, True)
        if action == 'set_level':
            number(step['value'], f'{where}.value', 1, 80, True)
        if action == 'level_scaling_packet':
            number(step['value'], f'{where}.value', 0, 1, True)
        if 'value' in step:
            number(step['value'], f'{where}.value', 1 if action == 'set_health' else 0, 2**31 - 1, True)
        if action == 'set_health' and 'maximum' in step:
            require(step['actor'] in player_ids, f'{where}: maximum health fixture needs a player or their pet')
            number(step['maximum'], f'{where}.maximum', 1, 2**31 - 1, True)
            require(step['value'] <= step['maximum'], f'{where}: health exceeds fixture maximum')
        if action in {'snapshot', 'assert'}:
            metric = step['metric']
            if 'periodic' in step:
                require(metric in {'spell_damage_done', 'spell_healing_done', 'spell_healing_taken'}
                        and type(step['periodic']) is bool,
                        f'{where}: periodic requires a damage/healing calculation and a boolean')
            require(metric in METRICS, f'{where}: unknown metric')
            if metric in {'xp', 'next_level_xp', 'skill_value'}:
                require(step['actor'] in player_ids, f'{where}: XP/skill metric needs a player')
                if metric == 'skill_value':
                    number(step.get('skill'), f'{where}.skill', 1, 65535, True)
            if metric in {'player_name', 'name_lookup'}:
                require(step['actor'] in player_ids and isinstance(step.get('name'), str)
                        and bool(step['name']), f'{where}: name metric needs a player and name')
            elif 'name' in step:
                require(False, f'{where}: name only applies to name metrics')
            if metric in {'view_level', 'sent_level', 'sent_max_health'}:
                require(step['actor'] in player_ids and 'target' in step,
                        f'{where}: view metric needs a player and target')
            if metric in {'quest_level', 'quest_xp'}:
                require(step['actor'] in player_ids and 'quest' in step,
                        f'{where}: quest metric needs a player and quest')
            if metric.startswith('aura') or metric in {
                    'knows_spell', 'cooldown_ms', 'global_cooldown_ms', 'spell_charges', 'cast_remaining_ms', 'has_talent',
                    'pet_aura_stacks', 'pet_aura_duration_ms', 'charm_aura_stacks',
                    'dynamic_object', 'dynamic_object_duration_ms', 'spell_power_cost',
                    'spell_damage_done', 'spell_damage_taken', 'spell_healing_taken', 'spell_hit_bonus_taken',
                    'spell_cast_count', 'spell_go_count', 'spell_modifier', 'spell_cast_time_ms',
                    'spell_max_range', 'spell_max_stacks', 'spell_healing_done', 'spell_done_crit_chance',
                    'spell_done_crit_chance_scripted',
                    'melee_spell_damage_done', 'script_spell_damage_taken', 'script_periodic_damage_taken',
                    'script_heal_received', 'spell_effect_value', 'spell_critical_damage', 'armor_reduced_damage',
                    'spell_immune', 'spell_effect_immune', 'spell_damage_count', 'spell_damage_total',
                    'spell_uses_armor', 'pet_aura_amount', 'pet_aura_amplitude_ms', 'spell_heal_count', 'spell_heal_total',
                    'spell_effective_heal_total', 'spell_energize_count', 'spell_energize_total',
                    'spell_proc_count', 'temporary_spell_replacement', 'cast_failure',
                    'trainer_window_state', 'trainer_window_ability', 'spellbook_superseded_for'}:
                require('spell' in step, f'{where}: metric needs spell')
            for key in ('pet', 'critical'):
                if key in step:
                    require(metric in {'spell_damage_count', 'spell_damage_total', 'spell_heal_count',
                                       'spell_heal_total', 'spell_effective_heal_total'}
                            or (key == 'pet' and metric in {'spell_go_count', 'spell_energize_count',
                                                           'spell_energize_total',
                                                           'armor_reduced_damage', 'spell_effect_value',
                                                           'spell_damage_done'}),
                            f'{where}: {key} only filters supported spell combat events')
                    require(type(step[key]) is bool, f'{where}: {key} must be boolean')
            if 'target_pet' in step:
                require(metric in {'spell_heal_count', 'spell_heal_total', 'spell_effective_heal_total',
                                   'spell_energize_count', 'spell_energize_total'}
                        and step.get('target') in player_ids, f'{where}: target_pet needs a healing or energize target player')
                require(type(step['target_pet']) is bool, f'{where}: target_pet must be boolean')
            if metric in {'spell_damage_done', 'melee_damage_done', 'spell_damage_taken', 'melee_damage_taken',
                          'spell_healing_done', 'spell_healing_taken', 'spell_done_crit_chance', 'spell_done_crit_chance_scripted', 'melee_spell_damage_done',
                          'spell_critical_damage', 'armor_reduced_damage', 'spell_immune', 'spell_effect_immune',
                          'distance_2d', 'can_detect'} \
                    or metric.startswith('script_'):
                require('target' in step, f'{where}: damage metric needs target')
            if metric == 'distance':
                require('target' in step, f'{where}: {metric} metric needs target')
            if metric == 'stat':
                number(step.get('stat'), f'{where}.stat', 0, 4, True)
            if metric == 'aura_script_value':
                number(step.get('key'), f'{where}.key', 0, 2**32 - 1, True)
            if metric in {'resistance', 'spell_power'}:
                number(step.get('school'), f'{where}.school', 1, 6, True)
            if metric == 'combat_rating':
                number(step.get('rating'), f'{where}.rating', 0, 24, True)
            if metric == 'aoe_damage_taken':
                number(step.get('school'), f'{where}.school', 0, 6, True)
            if metric == 'reputation_gain':
                require('id' in step, f'{where}: reputation metric needs faction id')
            if metric == 'spell_modifier':
                number(step.get('op'), f'{where}.op', 0, 31, True)
                number(step.get('base'), f'{where}.base')
            if 'hand' in step:
                maximum = 1 if metric in {'melee_attack_count', 'melee_damage_count'} else 2
                number(step['hand'], f'{where}.hand', 0, maximum, True)
            if 'school' in step and metric == 'spell_crit_chance':
                number(step['school'], f'{where}.school', 0, 6, True)
            if metric == 'item_count':
                require('item' in step, f'{where}: metric needs item')
            if metric == 'carried_pool_item_count':
                require('cache' in step, f'{where}: metric needs the cache item it checks against')
            if metric == 'pool_variant_count':
                require('cache' in step, f'{where}: metric needs the cache item it checks against')
            if metric == 'pool_retired_item_count':
                require('cache' in step, f'{where}: metric needs the cache item it checks against')
            if metric == 'pool_row_count':
                require('cache' in step, f'{where}: metric needs the cache item it checks against')
            if metric == 'pool_item_present':
                require('cache' in step, f'{where}: metric needs the cache item it checks against')
                require('item' in step, f'{where}: metric needs the item to look for')
            if metric in ('cache_token_count', 'cache_token_stage', 'cache_token_present'):
                require('cache' in step, f'{where}: metric needs the cache item it checks against')
            if metric == 'cache_token_present':
                require('item' in step, f'{where}: metric needs the token to look for')
            if metric in ('mail_has_item',):
                require('item' in step, f'{where}: metric needs the item to look for')
            if metric == 'mail_pool_item_count':
                require('cache' in step, f'{where}: metric needs the cache item it checks against')
            if metric == 'notification_contains':
                require(isinstance(step.get('text'), str) and step['text'].strip(),
                        f'{where}: metric needs the text to look for')
            if metric == 'quest_rewarded':
                require('quest' in step, f'{where}: metric needs quest')
            if metric == 'who_class':
                require(step.get('target') in player_ids, f'{where}: Who class metric needs a target player')
            if metric == 'owned_creature_count':
                require('entry' in step, f'{where}: metric needs creature entry')
                require('caster' not in step or 'spell' in step, f'{where}: aura caster filter needs spell')
            if metric == 'owned_creature_scale':
                require('entry' in step, f'{where}: metric needs creature entry')
            if metric == 'system_message_contains':
                require(isinstance(step.get('text'), str) and step['text'].strip(),
                        f'{where}: metric needs the text to look for')
            if metric in {'spellbook_offers_spell', 'spellbook_learned_alerts',
                          'spellbook_buy_succeeded', 'spellbook_buy_failed', 'cast_failure'}:
                require('spell' in step, f'{where}: metric needs spell')
            if metric in {'owned_gameobject_count', 'gameobject_remaining_ms'}:
                require('entry' in step, f'{where}: metric needs gameobject entry')
            if metric in {'quest_status', 'quest_takeable', 'quest_objective_count'}:
                require('quest' in step, f'{where}: metric needs quest')
            if metric == 'dialog_status':
                require('entry' in step, f'{where}: metric needs creature entry')
            if metric == 'taxi_node':
                number(step.get('entry'), f'{where}.entry', 1, 2**31 - 1, True)
            if metric == 'ball_offers_quest':
                require('quest' in step, f'{where}: metric needs quest')
            if metric in {'ball_carried_quest', 'ball_turn_in_quest'}:
                require('quest' in step, f'{where}: metric needs quest')
            if metric == 'gossip_text':
                require('id' in step, f'{where}: metric needs text id')
            if metric in {'knows_spell', 'has_talent', 'talent_points', 'cooldown_ms', 'spell_charges', 'action_button', 'item_count',
                          'carried_item_count', 'carried_pool_item_count', 'carried_variant_item_count',
                          'bank_bag_slots', 'taxi_node',
                          'cast_pushback_ms',
                          'bank_shows', 'system_messages', 'system_message_contains',
                          'challenge_start_responses', 'challenge_start_code', 'owned_creature_scale', 'cast_failure',
                          'pet_entry', 'pet_aura_stacks', 'pet_is_banker', 'pet_display', 'pet_scale',
                          'owned_creature_count', 'charm_entry',
                          'charm_aura_stacks', 'controls_self', 'private_instance',
                          'dynamic_object', 'dynamic_object_duration_ms', 'gossip_options',
                          'owned_gameobject_count', 'gameobject_remaining_ms', 'at_homebind',
                          'spellbook_rows', 'spellbook_offers_spell', 'spellbook_covers_spell',
                          'spellbook_learned_alerts', 'spellbook_buy_succeeded', 'spellbook_buy_failed',
                          'spellbook_buys_granted', 'spellbook_unannounced_buys',
                          'spellbook_misannounced_buys', 'spellbook_silent_buys',
                          'spellbook_multi_announced_buys',
                          'spellbook_notify_rows', 'spellbook_notified_spells',
                          'spellbook_unnotified_buys',
                          'trainer_list_packets', 'trainer_window_rows', 'trainer_window_state',
                          'trainer_window_ability', 'spellbook_superseded_packets',
                          'spellbook_superseded_for',
                          'spellbook_cues_in_last_buy', 'spellbook_last_buy_cued',
                          'cast_speed_multiplier', 'spell_crit_chance', 'spell_power_cost',
                          'spell_damage_done', 'melee_damage_done',
                          'who_count', 'who_class',
                          'loot_count', 'loot_entry', 'loot_received', 'quest_rewarded',
                          'quest_status', 'quest_takeable', 'quest_objective_count', 'dialog_status',
                          'ball_offer_count', 'ball_offers_quest',
                          'ball_carried_count', 'ball_carried_quest',
                          'ball_turn_in_count', 'ball_turn_in_quest',
                          'temporary_spell_replacement'} | PLAYER_STAT_METRICS:
                require(step['actor'] in player_ids, f'{where}: metric needs a player')
            shape = (metric, step.get('exclude'))
            if 'relative_to' in step:
                require(snapshots.get(step['relative_to']) == shape, f'{where}: missing or incompatible snapshot')
            if 'ratio_to' in step:
                require(step['ratio_to'] in snapshots, f'{where}: missing ratio snapshot')
            if action == 'snapshot':
                name = step['save_as']
                require(isinstance(name, str) and ACTOR_ID.fullmatch(name), f'{where}: invalid snapshot name')
                require(name not in snapshots, f'{where}: duplicate snapshot')
                snapshots[name] = shape
            else:
                assertions += 1
                require(any(key in step for key in ('equals', 'min', 'max')), f'{where}: no expected value')
                for key in ('equals', 'min', 'max'):
                    if key in step:
                        number(step[key], f'{where}.{key}')
                require(step.get('min', -math.inf) <= step.get('max', math.inf), f'{where}: reversed range')
                if 'equals' in step:
                    require(step.get('min', -math.inf) <= step['equals'] <= step.get('max', math.inf),
                            f'{where}: contradictory assertion')
    require(assertions > 0, 'Scenario must contain assertions')
    return scenario


def read_config(path):
    values = {}
    for line in Path(path).read_text(encoding='utf-8-sig').splitlines():
        match = re.match(r'^\s*([A-Za-z0-9_.]+)\s*=\s*(.*)$', line)
        if not match:
            continue
        key, value = match.groups()
        if value.startswith('"'):
            require('"' in value[1:], f'Unterminated config value: {key}')
            value = value[1:value.index('"', 1)]
        else:
            value = value.split('#', 1)[0].strip()
        values[key] = value
    return values


def env_var_name(key):
    result = []
    for index, char in enumerate(key):
        if char in ' .-':
            result.append('_')
            continue
        if index + 1 < len(key):
            following = key[index + 1]
            if ((not char.isupper() and following.isupper())
                    or ('0' <= char <= '9') != ('0' <= following <= '9')):
                result.append(char.upper() + '_')
                continue
        result.append(char.upper())
    return 'AC_' + ''.join(result)


def server_environment(overrides, environment=None):
    blocked = {env_var_name(key) for key in overrides}
    source = os.environ if environment is None else environment
    return {name: value for name, value in source.items() if name not in blocked}


def source_setting(config, key, default=None, environment=None):
    source = os.environ if environment is None else environment
    value = source.get(env_var_name(key), config.get(key, default))
    require(value is not None, f'Missing source setting: {key}')
    return value


@dataclass(frozen=True)
class Connection:
    host: str
    port: int
    user: str
    password: str = field(repr=False)
    database: str

    @classmethod
    def parse(cls, value):
        parts = value.split(';')
        require(len(parts) == 5, 'Expected five database connection fields')
        host, port, user, password, database = parts
        require(host in LOCAL_HOSTS, 'Only local database sources are supported')
        require(IDENTIFIER.fullmatch(database), 'Invalid source database name')
        require(port.isdecimal() and 0 < int(port) <= 65535, 'Invalid database port')
        return cls(host, int(port), user, password, database)

    def with_database(self, name):
        return ';'.join((self.host, str(self.port), self.user, self.password, name))


def cnf_quote(value):
    return '"' + value.replace('\\', '\\\\').replace('"', '\\"').replace('\n', '\\n').replace('\r', '\\r') + '"'


def database_credentials(connections, path):
    parser = configparser.ConfigParser(interpolation=None)
    try:
        parser.read_string(path.read_text(encoding='utf-8-sig'))
    except configparser.Error:
        raise ValueError('Invalid database client config') from None
    require('client' in parser, 'Database client config needs a [client] section')
    settings = parser['client']

    def value(key):
        require(key in settings, f'Database client config needs {key}')
        text = settings[key].strip()
        if text.startswith(('"', "'")):
            require(len(text) >= 2 and text[-1] == text[0], f'Invalid client config quoting for {key}')
            text = text[1:-1]
        escapes = {'b': '\b', 't': '\t', 'n': '\n', 'r': '\r', 's': ' ', '\\': '\\', '"': '"', "'": "'"}
        return re.sub(r'\\(.)', lambda match: escapes.get(match[1], match[0]), text)

    host, port, user, password = (value(key) for key in ('host', 'port', 'user', 'password'))
    require(host in LOCAL_HOSTS and port.isdecimal(), 'Client config must select a local MySQL endpoint')
    require(user and not any(char in user + password for char in ';\r\n"'),
            'Client credentials cannot be represented in a worldserver connection string')
    result = {}
    for role, source in connections.items():
        require(host == source.host and int(port) == source.port,
                'Client config endpoint must match every source database connection')
        result[role] = Connection(source.host, source.port, user, password, source.database)
    return result


class Databases:
    def __init__(self, mysql, dump, directory, connections, run_id):
        require(re.fullmatch(r'[0-9a-f]{12}', run_id), 'Invalid run id')
        self.mysql = str(mysql)
        self.dump = str(dump)
        self.directory = directory
        self.connections = connections
        self.names = {role: f'coa_test_{run_id}_{role}' for role in connections}
        self.created = []
        self.option_files = {}
        try:
            for role, connection in connections.items():
                require(self.names[role] != connection.database, 'Source and test database must differ')
                path = directory / f'{role}-client.cnf'
                self.option_files[role] = path
                content = '[client]\n' + '\n'.join(f'{key}={cnf_quote(str(value))}' for key, value in {
                    'host': connection.host, 'port': connection.port, 'user': connection.user,
                    'password': connection.password, 'protocol': 'TCP', 'default-character-set': 'utf8mb4',
                }.items()) + '\n'
                path.write_text(content, encoding='utf-8')
                path.chmod(0o600)
        except BaseException:
            self.remove_credentials()
            raise

    def redact(self, detail):
        for connection in self.connections.values():
            if connection.password:
                detail = detail.replace(connection.password, '[redacted]')
        return detail.strip()[-4000:]

    def sql(self, role, statement, timeout=60):
        result = subprocess.run([self.mysql, f'--defaults-extra-file={self.option_files[role]}',
                                 '--batch', '--skip-column-names'], input=statement, text=True,
                                capture_output=True, timeout=timeout, creationflags=CREATE_FLAGS)
        if result.returncode:
            detail = self.redact(result.stderr)
            raise ValueError(f'MySQL operation failed for {role}: {detail or result.returncode}')
        return result.stdout.strip()

    def copy(self, role, schema_only=False, tables=()):
        connection = self.connections[role]
        arguments = [self.dump, f'--defaults-extra-file={self.option_files[role]}',
                     '--single-transaction', '--skip-lock-tables', '--no-tablespaces', '--skip-add-locks',
                     '--set-gtid-purged=OFF', '--column-statistics=0', '--skip-triggers']
        if schema_only:
            arguments.append('--no-data')
        elif tables:
            arguments.append('--no-create-info')
        arguments.extend([connection.database, *tables])
        with tempfile.TemporaryFile() as errors:
            source = subprocess.Popen(arguments, stdout=subprocess.PIPE, stderr=errors, creationflags=CREATE_FLAGS)
            target = None
            try:
                target = subprocess.Popen([self.mysql, f'--defaults-extra-file={self.option_files[role]}',
                                           self.names[role]], stdin=source.stdout, stdout=subprocess.DEVNULL,
                                          stderr=errors, creationflags=CREATE_FLAGS)
                source.stdout.close()
                target.wait(timeout=1800)
                source.wait(timeout=60)
                if source.returncode or target.returncode:
                    errors.seek(0)
                    detail = self.redact(errors.read().decode('utf-8', errors='replace'))
                    raise ValueError(f'Database copy failed for {role}: {detail}')
            finally:
                for process in (target, source):
                    if process is not None and process.poll() is None:
                        process.kill()
                        process.wait()

    def prepare(self, roles=('auth', 'characters', 'world')):
        for role in roles:
            name = self.names[role]
            self.sql(role, f'CREATE DATABASE `{name}` CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;')
            self.created.append(role)
            print(f'Preparing isolated {role} database...', flush=True)
            self.copy(role, schema_only=(role != 'world'))
            if role != 'world':
                tables = ['updates', 'updates_include']
                if role == 'auth':
                    tables += ['rbac_permissions', 'rbac_linked_permissions', 'rbac_default_permissions', 'realmlist']
                else:
                    tables += ['active_arena_season']
                self.copy(role, tables=tables)

    def cleanup(self):
        failures = []
        for role in reversed(self.created):
            try:
                self.sql(role, f'DROP DATABASE `{self.names[role]}`;')
            except (ValueError, subprocess.SubprocessError):
                failures.append(self.names[role])
        return failures

    def remove_credentials(self):
        for path in self.option_files.values():
            path.unlink(missing_ok=True)


def unused_port():
    with socket.socket() as listener:
        listener.bind(('127.0.0.1', 0))
        return listener.getsockname()[1]


def write_config(source, destination, overrides):
    lines = []
    for line in source.read_text(encoding='utf-8-sig').splitlines():
        match = re.match(r'^\s*([A-Za-z0-9_.]+)\s*=', line)
        if not match or match.group(1) not in overrides:
            lines.append(line)
    for key, value in overrides.items():
        require('"' not in str(value) and '\n' not in str(value) and '\r' not in str(value),
                f'Unsupported config characters in {key}')
        lines.append(f'{key} = "{value}"')
    destination.write_text('\n'.join(lines) + '\n', encoding='utf-8')
    destination.chmod(0o600)


def check_no_reserved_overrides(path, reserved):
    settings = read_config(path)
    require(not settings.keys() & reserved
            and not any(key.startswith('CoAGameplayTest.') for key in settings),
            f'Module config overrides harness controls: {path.name}')


def stage_modules(source, destination, reserved):
    if destination.exists():
        for path in sorted(destination.glob('*.conf')):
            check_no_reserved_overrides(path, reserved)
            require((source / path.name).is_file(),
                    f'Unexpected server module config: {path.name}; use an empty module config directory')
    staged = []
    try:
        for path in sorted(source.glob('*.conf')):
            check_no_reserved_overrides(path, reserved)
            target = destination / path.name
            target.parent.mkdir(parents=True, exist_ok=True)
            with target.open('xb') as staged_file:
                staged.append(target)
                staged_file.write(path.read_bytes())
            target.chmod(0o600)
        return staged
    except BaseException:
        for path in staged:
            path.unlink(missing_ok=True)
        raise


class ServerStillRunning(RuntimeError):
    def __init__(self, pid):
        super().__init__(f'Test process {pid} could not be stopped; databases were retained')
        self.pid = pid


def check_report(report, run_id, scenario, returncode):
    require(int(report.get('schema', 0)) == 1, 'Unsupported result schema')
    require(report.get('run_id') == run_id, 'Result belongs to a different run')
    require(report.get('scenario') == scenario['name'], 'Result belongs to a different scenario')
    require(report.get('execution') == 'socketless-session-handlers', 'Unexpected execution mode')
    require(report.get('status') == 'passed', report.get('message', 'Scenario failed'))
    require(returncode == 0, f'Worldserver exited with code {returncode}')
    expected = sum(step['action'] == 'assert' for step in scenario['steps'])
    require(int(report.get('assertions', 0)) == expected, 'Not all assertions ran')
    require(int(report.get('completed_steps', 0)) == len(scenario['steps']), 'Scenario did not complete')
    records = report.get('steps', [])
    require(isinstance(records, list) and len(records) == len(scenario['steps']), 'Missing step evidence')
    for index, (record, step) in enumerate(zip(records, scenario['steps'])):
        require(int(record.get('index', -1)) == index and record.get('action') == step['action'],
                'Step evidence does not match the scenario')
        if step['action'] == 'assert':
            require(record.get('status') == 'passed', 'A recorded assertion failed')
            actual = float(record['actual'])
            require(math.isfinite(actual), 'Non-finite assertion result')
            require('equals' not in step or actual == step['equals'], 'Equality assertion failed')
            require('min' not in step or actual >= step['min'], 'Minimum assertion failed')
            require('max' not in step or actual <= step['max'], 'Maximum assertion failed')
        else:
            require(record.get('status') == 'completed', 'An action did not complete')


def run_process(command, directory, ready_path, result_path, run_id, startup_timeout, timeout,
                on_ready=None, environment=None):
    with (directory / 'worldserver.log').open('wb') as log:
        process = subprocess.Popen(command, cwd=directory, stdin=subprocess.PIPE, stdout=log, stderr=log,
                                   env=environment, creationflags=CREATE_FLAGS)
        start = time.monotonic()
        ready_at = None
        try:
            while process.poll() is None:
                now = time.monotonic()
                if ready_at is None and ready_path.exists():
                    ready = read_json(ready_path)
                    require(ready.get('run_id') == run_id and ready.get('status') == 'ready',
                            'Invalid readiness record')
                    if on_ready:
                        on_ready(ready)
                    ready_at = time.monotonic()
                    print('Worldserver harness is ready; executing scenario...', flush=True)
                if ready_at is None:
                    require(now - start < startup_timeout, 'Worldserver/harness readiness timed out')
                else:
                    require(now - ready_at < timeout, 'Gameplay scenario/shutdown timed out')
                time.sleep(0.1)
            require(result_path.exists(), f'Worldserver exited with code {process.returncode} without a result; '
                    'check the build and server log')
            report = read_json(result_path)
            if report.get('status') == 'passed':
                require(on_ready is None or ready_at is not None, 'Startup barrier was not observed')
                require(ready_path.exists(), 'Successful result is missing harness readiness')
                ready = read_json(ready_path)
                require(ready.get('run_id') == run_id and ready.get('status') == 'ready', 'Invalid readiness record')
            return report, process.returncode
        finally:
            if process.poll() is None:
                try:
                    process.stdin.write(b'server shutdown 0\n')
                    process.stdin.flush()
                    process.wait(timeout=15)
                except (OSError, subprocess.TimeoutExpired):
                    try:
                        process.kill()
                        process.wait(timeout=15)
                    except (OSError, subprocess.TimeoutExpired) as error:
                        if process.poll() is None:
                            raise ServerStillRunning(process.pid) from error
            process.stdin.close()


def sha256(path):
    with path.open('rb') as source:
        return hashlib.file_digest(source, 'sha256').hexdigest()


def execute(args, scenario):
    started = time.monotonic()
    binary = args.worldserver.resolve(strict=True)
    source_config = args.config.resolve(strict=True)
    mysql = args.mysql.resolve(strict=True)
    dump = args.mysqldump.resolve(strict=True)
    config = read_config(source_config)
    connections = {role: Connection.parse(source_setting(config, key)) for role, key in {
        'auth': 'LoginDatabaseInfo', 'characters': 'CharacterDatabaseInfo', 'world': 'WorldDatabaseInfo',
    }.items()}
    if args.database_client_config:
        connections = database_credentials(connections, args.database_client_config)
    run_id = secrets.token_hex(6)
    output = (args.output or ROOT / '.cache' / 'coa-gameplay-tests' / run_id).resolve()
    output.mkdir(parents=True, exist_ok=False)
    args.result_directory = output
    result_path = output / 'result.json'
    ready_path = output / 'ready.json'
    scenario_path = output / 'scenario.json'
    scenario_path.write_text(json.dumps(scenario, indent=2) + '\n', encoding='utf-8')
    print(f'Run {run_id}: {output}', flush=True)
    credentials_dir = Path(tempfile.mkdtemp(prefix='coa-gameplay-test-'))
    generated_config = credentials_dir / 'worldserver.conf'
    module_configs = []
    cache = None
    retain_databases = False
    summary = {'schema': 1, 'run_id': run_id, 'scenario': scenario['name'], 'status': 'failed',
               'binary_sha256': sha256(binary), 'scenario_sha256': sha256(scenario_path),
               'binary': str(binary)}
    database = Databases(mysql, dump, credentials_dir, connections, run_id)
    summary['databases'] = database.names
    try:
        module_source = args.modules_config_dir or source_config.parent / 'modules'
        if not args.fresh_databases:
            cache = WorldCache(database, args.world_cache_dir.resolve(),
                               input_fingerprint(ROOT, source_config, module_source), result_directory=output)
            summary['world_cache'] = cache.info
            cache.prepare(refresh=args.refresh_world)
        else:
            summary['world_cache'] = {'mode': 'fresh', 'retained': False}
        database.prepare(roles=('auth', 'characters') if cache else ('auth', 'characters', 'world'))
        summary['database_prepare_seconds'] = round(time.monotonic() - started, 3)
        data_dir = Path(source_setting(config, 'DataDir', '.'))
        if not data_dir.is_absolute():
            data_dir = binary.parent / data_dir
        overrides = {
            'LoginDatabaseInfo': connections['auth'].with_database(database.names['auth']),
            'CharacterDatabaseInfo': connections['characters'].with_database(database.names['characters']),
            'WorldDatabaseInfo': connections['world'].with_database(database.names['world']),
            'DataDir': data_dir.resolve().as_posix(), 'SourceDirectory': ROOT.as_posix(),
            'LogsDir': output.as_posix(), 'BindIP': '127.0.0.1', 'WorldServerPort': unused_port(),
            'Console.Enable': 1, 'Ra.Enable': 0, 'SOAP.Enabled': 0, 'MapUpdate.Threads': 0,
            'Warden.Enabled': 0, 'Network.UseSocketActivation': 0,
            'LoginDatabase.WorkerThreads': 1, 'CharacterDatabase.WorkerThreads': 1,
            'Updates.EnableDatabases': 7, 'CoAGameplayTest.Enable': 1, 'CoAGameplayTest.RunId': run_id,
            'CoAGameplayTest.WorldDatabaseId': cache.metadata['world_id'] if cache else run_id,
            'CoAGameplayTest.StartFile': (output / 'start.json').as_posix() if cache else '',
            'CoAGameplayTest.ScenarioFile': scenario_path.as_posix(),
            'CoAGameplayTest.ReadyFile': ready_path.as_posix(),
            'CoAGameplayTest.ResultFile': result_path.as_posix(),
        }
        module_target = args.server_modules_dir or output / 'configs' / 'modules'
        module_configs = stage_modules(module_source, module_target, set(overrides))
        summary['module_config_sha256'] = {path.name: sha256(path) for path in module_configs}
        write_config(source_config, generated_config, overrides)
        server_started = time.monotonic()
        on_ready = (lambda record: cache.ready(record, output / 'start.json', run_id)) if cache else None
        report, returncode = run_process([str(binary), '-c', str(generated_config)], output, ready_path,
                                         result_path, run_id, args.startup_timeout,
                                         scenario.get('timeout_ms', 90000) / 1000 + 30,
                                         on_ready=on_ready, environment=server_environment(overrides))
        summary['server_seconds'] = round(time.monotonic() - server_started, 3)
        check_report(report, run_id, scenario, returncode)
        summary.update(status='passed', assertions=int(report['assertions']))
    except ServerStillRunning as error:
        retain_databases = True
        summary.update(message=str(error), process_id=error.pid)
    except (ValueError, OSError, subprocess.SubprocessError, KeyError) as error:
        summary['message'] = str(error)
    finally:
        failures = list(database.names.values()) if retain_databases else database.cleanup()
        if cache:
            try:
                cache.finish(server_still_running=retain_databases)
            except (ValueError, OSError, subprocess.SubprocessError, KeyError) as error:
                summary['cache_cleanup_error'] = str(error)
                failures.append(database.names['world'])
        if failures:
            summary.update(status='failed', cleanup_failed=failures)
        generated_config.unlink(missing_ok=True)
        for path in module_configs:
            path.unlink(missing_ok=True)
        database.remove_credentials()
        shutil.rmtree(credentials_dir, ignore_errors=True)
        summary['total_seconds'] = round(time.monotonic() - started, 3)
        (output / 'summary.json').write_text(json.dumps(summary, indent=2) + '\n', encoding='utf-8')
    stage = 'COMPLETE' if summary['status'] == 'passed' else 'FAILED'
    print(f"NATIVE STAGE {stage}: {scenario['name']}\nResults: {output}")
    if 'message' in summary:
        print(summary['message'])
    return 0 if summary['status'] == 'passed' else 1


def _raise_keyboard_interrupt(signum, frame):
    raise KeyboardInterrupt


def add_run_arguments(run):
    run.add_argument('--worldserver', type=Path, required=True)
    run.add_argument('--config', type=Path, required=True, help='Source worldserver config; never changed')
    run.add_argument('--mysql', type=Path, required=True)
    run.add_argument('--mysqldump', type=Path, required=True)
    run.add_argument('--database-client-config', type=Path,
                     help='Optional MySQL [client] file with credentials allowed to create/drop test schemas')
    run.add_argument('--modules-config-dir', type=Path, help='Defaults to the source config directory/modules')
    run.add_argument('--server-modules-dir', type=Path,
                     help='Directory the worldserver reads module configs from; required outside Windows')
    run.add_argument('--output', type=Path, help='New directory for logs and results')
    run.add_argument('--native-only', action='store_true',
                     help='Exploratory execution only; explicitly skip combined verification')
    run.add_argument('--startup-timeout', type=float, default=600)
    mode = run.add_mutually_exclusive_group()
    mode.add_argument('--fresh-databases', action='store_true', help='Use disposable copies without the world cache')
    mode.add_argument('--refresh-world', action='store_true', help='Replace the owned world cache before this run')
    run.add_argument('--world-cache-dir', type=Path, default=ROOT / '.cache/coa-gameplay-tests/world-cache',
                     help='Local cache metadata/lease directory; database ownership is verified separately')


def main(argv=None):
    parser = argparse.ArgumentParser(description=CLI_DESCRIPTION)
    subparsers = parser.add_subparsers(dest='command', required=True)
    check = subparsers.add_parser('validate', help='Validate a scenario without starting a server')
    check.add_argument('scenario', type=Path)
    run = subparsers.add_parser('run', help='Prepare isolated test DBs, run a scenario, and clean up')
    run.add_argument('scenario', type=Path)
    add_run_arguments(run)
    args = parser.parse_args(argv)
    previous_sigterm_handler = None
    sigterm_installed = False
    try:
        scenario = validate(read_json(args.scenario))
        if args.command == 'validate':
            print(f"Valid scenario: {scenario['name']} ({len(scenario['steps'])} steps)")
            return 0
        require(math.isfinite(args.startup_timeout) and args.startup_timeout > 0, 'Invalid startup timeout')
        require(args.server_modules_dir or os.name == 'nt',
                '--server-modules-dir is required outside Windows (the worldserver reads CONF_DIR/modules)')
        if hasattr(signal, 'SIGTERM'):
            previous_sigterm_handler = signal.signal(signal.SIGTERM, _raise_keyboard_interrupt)
            sigterm_installed = True
        from workflow import run_registered
        return run_registered(args, scenario, execute)
    except (ValueError, OSError, KeyError, TypeError) as error:
        print(f'ERROR: {error}', file=sys.stderr)
        return 1
    finally:
        if sigterm_installed:
            signal.signal(signal.SIGTERM, previous_sigterm_handler)


if __name__ == '__main__':
    sys.exit(main())
