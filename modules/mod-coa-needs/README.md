# CoA needs

An isolated port of the hunger, hydration, and vigor mechanics from
`C:\Azerothcore-Fresh\source\modules\mod-survival`. Human characters participate by default.
The complete Survival realm layer is not installed.

- Hunger and hydration start at 100. Default drain is 4.5 and 6 points per minute while alive,
  outside battlegrounds and arenas. Offline characters do not drain.
- Consuming food restores 45 hunger plus up to 5 from Cooking. Consuming drinks restores
  55/65/75 hydration according to item tier. Existing Nourishment buffs remain independent.
- Vigor starts at 100; maximum capacity falls with hunger and hydration, down to 25 when both
  are empty. Recovery is 8 per second after a 1.5-second spending delay, 25% in combat,
  and doubled when seated outside combat. Engineering improves critical strike chance with trained skill.
- Entering combat costs 8 vigor with a 10-second grace period. Dealing damage costs 3 vigor
  at most once per second. Moving in water costs 4 vigor per second.
- Hunger at or below 35 reduces damage by 15%; hydration at or below 30 reduces it by 10%.
  Each also multiplies vigor recovery by 0.65. Empty vigor reduces damage by 50%.
- Hunger at or below 5 reduces maximum health by 30%. Hydration at or below 10 causes
  2% maximum-health environmental damage every 10 seconds outside rest areas and battlegrounds.
- Sprint unlocks at level 1: +40% speed, 10 vigor to start and 10 per moving second.
  Sprint adds 2 hunger and 3 hydration drain per minute. It is unavailable while mounted,
  in flight, controlled, starving, exhausted, seated, or in battlegrounds/arenas.
- Sprint ends at 15 vigor, causing 15 seconds of Exhaustion: attributes -25%, maximum health
  -15%, and outgoing damage blocked for the first 5 seconds. Entering water clears Exhaustion.
- Dodge unlocks at level 30: 25 vigor, +35% dodge for 3 seconds, 20-second cooldown.
- Second Wind unlocks at level 60: restores 25 vigor, 2.5x recovery for 6 seconds,
  120-second cooldown.

CoA Survivalist challenges retain their authoritative hunger/thirst counters, restoration,
and failure rules. The bridge supplies their remaining food and water to the vigor system;
this module does not drain or penalize their hunger/hydration a second time.

State and expiry timestamps use existing `character_settings`, source `core.coa.needs`,
through the core's prepared statements and character save transaction. No new schema is needed.
Sprint never persists across logout. Cooldowns, Exhaustion, needs and vigor do persist.

Client/server Spell.dbc must contain 996009-996017 and 996100-996109. Startup fails closed for this module if
any of these spells are absent. The CoA-only HomebrewNourishment payload loads `CoANeeds.lua`;
it supplies needs bars, three food/drink buff icons with remaining time and effect tooltips,
and secure native ability buttons. `/needs` toggles the panel outside combat.
The server sends status using `HXN`; addon messages cannot activate abilities.

Configuration is in `CoANeeds.conf`. Disable with `CoANeeds.Enable = 0`; the next player tick
removes active needs auras. Bots remain excluded unless `CoANeeds.IncludeBots = 1` is set.
The CoA challenge bridge header is a build dependency of `mod-coa-challenges`.

The addon and DBC patch are installed by the existing CoA launcher profile without changing
launcher validation rules or the Fresh/Survival payload inventories.

## Camps

Campfire unlocks at Survivalist skill 1 and costs 2 Simple Wood. Shelter unlocks at skill 75 and also costs
4 Light Leather; Hearthstead unlocks at skill 150 and also costs 4 Heavy Leather. The existing CoA
item 190120 is Branding Rod, so the Survival realm's Tinder reagent cannot be reused.
Native placement and pack-up abilities appear as buttons in the needs panel and in a dedicated
Survivalist spellbook section (SkillLine 9200, category 14), together with the vigor abilities.
The skill mappings allow all CoA races and classes. Placement requires
standing still on dry land outside combat, instances, battlegrounds, flight, and mounted movement.
One beneficial camp per owner is registered, with a configurable one-hour lifetime and a
persisted 60-second placement cooldown. Replacement, packing, and owner logout remove it.
Supplies are consumed only after all required objects spawn successfully. Materials are not refunded.
Objects left in a different map expire naturally and provide no registered camp benefits.

Campfire/Shelter/Hearthstead radii are 15/20/25 yards. Visible nearby camps serve human characters;
camp recovery requires sitting outside combat. Native health and mana regeneration increase by 200%,
and vigor recovery receives an additional 1.5/2/2.5 multiplier. Rested XP accumulates at
the full configured rested-XP cap per five minutes of active camp rest for every camp tier.
CoANeeds.CampRestFillSeconds defaults to 300. The rate follows the core's Rate.Rest.MaxBonus
and next-level XP; existing rested XP shortens the time to full. Standing or combat pauses
camp accrual, and maximum-level characters receive no rested XP.
Global hunger/hydration drain and severe dehydration damage pause while near a live camp.
Survivalist challenge hunger/thirst rules continue to control their own counters.

Sitting continuously for 30 seconds grants Campfire/Shelter/Hearthstead Fellowship:
+3/+4/+5 to all primary attributes for one hour. Only one Fellowship tier can be active;
a stronger camp can upgrade it, and equal/weaker camps cannot continually refresh its duration.
Camp rest auras stop when standing, entering combat, dying, or leaving the camp radius.

## Weather and injuries

Weather exposure uses native zone weather through the global ALE hook. Rain and snow build
Soaked after 5 minutes; cold biomes/snow build Freezing after 7 minutes; hot biomes without
rain/snow build Heat Strain after 3 minutes. Exposure decays twice as fast when conditions
end. Indoor areas, rest areas, camps (even while standing), death, instances and battlegrounds
clear weather exposure. Northrend except Sholazar and the original Survival cold/hot zone
sets are retained. Weather is transient across logout; offline exposure does not accrue.

Soaked and Freezing each slow movement 5% and vigor recovery 10%. Cold plus wet deals 1%
maximum health damage per minute. Heat adds 10% hunger and 25% hydration drain and slows
vigor recovery 10%. Storms add 10% hydration drain, slow recovery 5% and spend 5 vigor per
minute. Clear temperate skies grant +1 primary attributes and +5% recovery. Delegated
challenge hunger/hydration remain authoritative; extra drain multipliers are not applied twice.

Creature-applied bleed/poison effects cause Lingering Wound (movement -3%) or Venom
(hydration drain +15%). A nonfatal hostile creature hit of at least 30% maximum health
causes Broken Leg (health -10%, movement -20%); 20% to below 30% causes Broken Arm
(health -5%, attack power -10%). Player-controlled creatures and battlegrounds are excluded.
Four injury flags persist in character_settings index 10 until treated; no migration is needed.
Dead characters and battlegrounds suppress injury auras while retaining their flags.

A consumed bandage must reach natural aura expiry to treat a wound; interrupted channels
do not count. A consumed healing potion or poison dispel treats venom. Field Splint is a
native First Aid ability learned at skill 1: outside combat, standing still and unmounted,
consume 4 existing Simple Wood and 2 Linen Cloth to treat one fracture, leg first.
Successful fracture treatment can improve First Aid through skill 100.
The native CoA Woodcutting and Woodworking professions are retained.
Custom spells 996110-996119 cover weather, injuries and Field Splint. The DBC builder
normalizes effect dice and strips unintended secondary effects from inherited templates.

The CoA dashboard follows the original Survival layout with gold trim, numbered meal
slots, three bars, weather/exposure and injury rows, hover help, dragging and width resizing.
Abilities remain in the spellbook and an optional expandable tray. /needs reset restores
default placement, width and scale; /needs toggles visibility outside combat. Meal textures
are cached while their item stays unchanged and the hidden panel skips timer painting.

## Survival tools and professions

Survivalist skill 9200 starts at 1 and progresses to 300 through crafting. Essential recipes are available
at skill 1: flask, rain bucket, field repair kit, fishing float and fishing pole. Personal Raft unlocks at
50, Barber Scissors at 75, War Drum at 100 and Travel Drum at 150. Camp placement also awards diminishing
Survivalist skill; Shelter unlocks at 75 and Hearthstead at 150. Recipe access is checked when casting,
including spells retained from the earlier character-level unlocks. Existing regular recipes and skill
progress remain intact.

Barber Scissors consume 2 Copper Bars, 2 Forestwood Planks and 2 Coarse Thread when crafted. The reusable
item places a standard barber chair outdoors outside combat and instances for one minute. Other tool
recipes retain their existing CoA materials and behavior. All custom field items are usable at character
level 1. Field Splint belongs to First Aid and consumes 4 Simple Wood and 2 Linen Cloth only after
successfully treating a fracture, leg first.

With CoANeeds.Professions.AllAtStart enabled, human characters receive missing professions from the
Book of Artisans rank data on login. Rank upgrades are granted when normal trainer skill, level and
expansion requirements are met. The primary profession limit becomes 32, enough for every native trade.
Bushcraft is available to every race/class through the paired server/client DBC update. Survivalist is
granted independently at 1/300. Bots retain their existing profession policy.

Missing starter tools are granted once; overflow is mailed and persistent character settings prevent
repeated grants. Tools include the mining pick, skinning knife, fishing pole, lumber axe, blacksmith
hammer, jeweler's kit, inking set and runed copper rod.

CoANeeds.Professions.UnrestrictedGathering allows trained human Mining, Herbalism, Skinning and
Woodcutting to harvest every eligible tier at any skill. Original difficulty values remain in place for
skill gains; lockpicking, corpse eligibility and tool requirements are unchanged. Refinement and crafting
retain their skill requirements.

Profession bonuses scale with trained skill / 450. Full benefits: Mining health/armor +4%; Blacksmithing
AP +3%; Skinning physical/spell crit +2 percentage points; Inscription haste +2%; Tailoring spell/healing
+20; Jewelcrafting attributes +2%; Enchanting mana +10/5sec; Leatherworking beast damage +5%; Herbalism
vigor recovery +5%; Alchemy healing-potion recovery +15%; First Aid bandage recovery +20%; Fishing swim
vigor cost -50%; Engineering physical/spell crit +1 percentage point. Engineering no longer improves
vigor recovery. Alchemy and First Aid bonuses apply before absorption only to their item spell families,
not ordinary healing spells. Cooking retains its nourishment benefit.

The client patch includes the matching spells, items and profession mappings. Configure tools and custom
bonuses with CoANeeds.Tools.Enable and CoANeeds.Professions.Enable.
