# Local compatibility checkpoint — 2026-09-08

This checkpoint collects the implemented class contracts, the first Manastorm server implementation,
and the subsequent Guardian source package. It does not claim complete class or Manastorm parity.

The deployed class-88/Manastorm release uses base 4e4e12678430eda4076ab219c37d6f9966fb0923 plus
the recorded overlays. Its 38 class world migrations are now included here, byte-for-byte, together
with the Manastorm character migration. Five compile compatibility corrections and the matching
class identity header have been integrated from that successfully linked release into this checkout.
The newer Guardian package and its two world migrations were source-tested separately and have not
yet been installed. A future deployment must account for those migrations explicitly.

Manastorm currently implements isolated solo instances, depths 1–15, three RFC scenes, protected
entry, three affixes, clear persistence and atomic first-clear mail. Its known startup and pre-map
login crashes are fixed. The next implementation phase replaces the remaining prototype limits,
reward policy and unsupported feature paths. Disabled/unimplemented paths are not gameplay parity.

The local client is maintained outside this source repository. It includes Ranger rank-aware
Advantage visibility, suppression of its unused secondary mana bar, the trusted FrameXML registration
of scenario saved positions, and concise Manastorm labels. Those files/MPQs are not bundled here.

Recorded local evidence is under C:/Ascension/runtime/releases/manastorm-class88-20260908 and
C:/Ascension/runtime/validation/guardian-fixes-20260908. Character data, server installations,
client executables, credentials, generated build output and private runtime databases are not part
of this source checkpoint. User-controlled Malarass login passed after the login repair; broader
Manastorm scene/combat and Guardian acceptance remain separate from compilation/source tests.
