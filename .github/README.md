# CoA on AzerothCore

Private AzerothCore fork implementing local compatibility with the Ascension CoA client.
The game client is the standalone `ascension-live` directory; the official Launcher application is not part of this project.
This is an independent reconstruction, not the official backend or a claim of complete gameplay parity.

## Project repositories

- **This repository:** C++ core, vendored `modules/mod-ascension-compat`, SQL and server mechanics tests.
- **[coa-local-kit](https://github.com/jealous-sound/coa-local-kit):** client file Apply/Restore engine,
  generator/policy/test source snapshot and preparation of a portable client workflow.

Both repositories remain private. Game files, extracted assets, databases, credentials, profiles and installed
runtime files stay outside Git. The module remains in this fork because it depends on changes in the core.

## Current state

The September 9 class-followup release contains the current class and Manastorm work. Its build/startup fixes
and migration corrections are committed; [local-release-state.md](../modules/mod-ascension-compat/docs/local-release-state.md)
records the installed baseline and separate gameplay/UI acceptance limits.
Source presence and successful startup are not a full gameplay test. The client kit's prepared-file engine is tested
on synthetic files; generation of a complete clean-client package and portable per-process startup remain in its roadmap.

The core's existing developer setup is described by [AzerothCore installation documentation](https://www.azerothcore.org/wiki/installation).
CoA additionally needs matching client-derived server data and the compatibility module configuration.
A clean clone is source code, not a ready-to-run server repack. Never replace the required data with another user's DB dump.

## Development

Read [AGENTS.md](../AGENTS.md) and the matching task guides before changes. Applied SQL is immutable:
new corrections require new migrations, even when applied files remain under a `pending` directory.
Do not build/configure the server or change a running installation as an incidental repository-management step.

The active CI checks source boundaries and metadata without configuring or building the server.
Original upstream workflows and issue templates are preserved under `.github/upstream-*` as inactive references.
See [repository organization](../docs/repository-organization.md), [contributing](CONTRIBUTING.md)
and [security reporting](SECURITY.md).

## Attribution and licensing

Based on [AzerothCore](https://github.com/azerothcore/azerothcore-wotlk), with its existing
[LICENSE](../LICENSE) and [AUTHORS](../AUTHORS) preserved. Follow per-file third-party notices.
The compatibility module's MIT template and AGPL source headers require a provenance review before a future public release;
this repository organization does not relicense them. No public release is authorized at this stage.
