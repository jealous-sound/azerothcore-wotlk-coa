# Ascension / CoA compatibility module

This module is the local compatibility boundary between AzerothCore and the
copied Ascension 3.3.5a client. It includes the protocol adapters, account-wide
collections, character advancement, custom classes/resources and the targeted
Barbarian, Guardian and Ranger corrections documented by the workspace.
It is an independent reconstruction: source presence is not proof of complete
official gameplay parity. The canceled Retail/native-class experiment is absent.

The module is included directly in `azerothcore-wotlk-coa`; a normal clone has
its source without a submodule initialization step. Its previous Git history is
retained in the combined repository's ancestry. Keep its license and upstream
attribution. Runtime configuration is supplied by
`conf/mod_ascension_compat.conf.dist`, not by committing live local credentials.

Canonical client files, generated candidates, runtime databases and deployment
evidence remain in sibling workspace directories and are not part of this
server repository. Use the corresponding generators in the workspace `tools`
directory, preserve applied SQL hashes, and follow
`.agents/docs/systems/ascension-local-deployment.md` in the server checkout.
Do not run historical installers or clear client caches to compensate for a
protocol, data or process problem.
