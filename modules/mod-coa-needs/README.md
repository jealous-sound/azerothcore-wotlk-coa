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
  and doubled when seated outside combat. Engineering grants up to 4 extra recovery per second.
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

Client/server Spell.dbc must contain 996009-996017. Startup fails closed for this module if
any of these spells are absent. The CoA-only HomebrewNourishment payload loads `CoANeeds.lua`;
it supplies bars and secure native spell buttons. `/needs` toggles the panel outside combat.
The server sends status using `HXN`; addon messages cannot activate abilities.

Configuration is in `CoANeeds.conf`. Disable with `CoANeeds.Enable = 0`; the next player tick
removes active needs auras. Bots remain excluded unless `CoANeeds.IncludeBots = 1` is set.
The CoA challenge bridge header is a build dependency of `mod-coa-challenges`.

The addon and DBC patch are installed by the existing CoA launcher profile without changing
launcher validation rules or the Fresh/Survival payload inventories.
