# Homebrew Nourishment client addon

Homebrew Nourishment by PMZFX is the companion addon for the
`mod-homebrew-nourishment` AzerothCore server module.

The addon is optional for mechanics but required for accurate dynamic values.
The server uses stock WoW 3.3.5a Well Fed aura records for client compatibility
and selects their values at runtime. This addon replaces the active aura's
fixed stock description with authoritative server values and adds full
nourishment details to eligible item tooltips.

Install the `HomebrewNourishment` directory under `Interface/AddOns`, enable it
at character selection, and use `/nourishment` or `/mealbuff` for current
status. The addon has no third-party dependencies.

Server module and installation instructions:
https://github.com/PMZFX/mod-homebrew-nourishment
