# Contributing

Bug reports and focused pull requests are welcome.

Before submitting a change:

1. Run `./tests/test-release.sh`.
2. Keep server and addon protocol changes synchronized.
3. Do not edit the generated baseline SQL or catalog audit by hand. Change the
   generator, regenerate both artifacts from a supported database and enUS
   3.3.5a `Spell.dbc`, and describe the catalog change.
4. Do not add private server paths, credentials, deployment service names, or
   player data.
5. Confirm the module still builds on every server stack affected by the change.
