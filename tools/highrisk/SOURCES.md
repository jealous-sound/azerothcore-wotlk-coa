# Sources and scope

Ruleset spell IDs, aura markers, native UI behavior and PvP Power constants were
checked against preserved Ascension client data. The client uses
`C_Player:SetRuleset` to cast 84422, 84420 or 84421, then reads aura markers
9931032, 1004119 and 1004019. The native General spellbook remains available.

The reviewed item subset comes from public community CoA item-cache captures.
It retains original item IDs, names, required levels and stats. It is not a dump
of Ascension's private server database and does not establish original drop rates.

Public references:

- https://ascension.gg/en/news/conquest-of-azeroth-alpha/332
- https://ascension.gg/en/news/the-future-of-world-pvp/513
- https://ascension.gg/en/features/pvp/high-risk
- https://github.com/azerothcore/azerothcore-wotlk

The general High Risk articles describe principles shared across some realms;
they do not prove a rule or numerical value belongs to a particular CoA season.
The drop percentages, loss probabilities and catalogue selection window in this
package are explicit realm tuning. The chest uses entry 994300, display 259,
lock 43 and Opening 22810; it is not claimed to be Ascension's original chest ID.

The package contains only this feature's source, SQL templates, referenced server
DBC data and tests. It has no original realm accounts, characters, escrow deposits,
access keys, deployment scripts, IP addresses or bundled client archives.
