# Catalog audit

`profile-audit.tsv` records the tested baseline classification of every food
and drink item considered by the generator:

- 300 included profiles;
- 195 deliberate exclusions.

The audit is tab-separated and suitable for review in a spreadsheet or with
standard command-line tools. It contains no account, character, or private
server data.

Do not edit the audit or baseline SQL by hand. Change the generator and
regenerate both artifacts together from the same supported world database and
enUS WoW 3.3.5a `Spell.dbc`.
