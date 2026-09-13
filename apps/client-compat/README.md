# Native client world-address compatibility

The native v4 client can authenticate to a remote authserver, then crash when entering
the world. `Extensions.dll` checks the world endpoint against its own address allowlist.
On rejection it overwrites three bytes at EXE address `0x403340`, corrupting an active hook.

`patch_world_endpoint.py` reproduces the verified fix from the accepted native v4 DLL.
It changes one conditional branch byte to skip that destructive failure block. The client
uses the world address supplied by the authserver, with no per-IP exception. Native address
parsing, SRP authentication and session proofs remain unchanged. The EXE is unchanged.

## Generate a candidate

Python 3.11 or newer is sufficient; no game files are included in this repository.

```sh
python apps/client-compat/patch_world_endpoint.py \
  --input /path/to/native-v4/Extensions.dll \
  --output /path/to/new-candidate/Extensions.dll
```

The output parent directory must exist. The tool refuses unknown input hashes, in-place
changes and existing outputs. An already patched input can produce another identical
candidate. It never installs files, opens the client or changes accounts, profiles or caches.

| File | SHA256 |
| --- | --- |
| Accepted baseline DLL | `f7b713095aab17a1e376f487290d4b7c4c18931635e4d91136d76db2592be8fa` |
| Patched DLL | `9791801053f828d1ccdab1a4c17e64852d3ebe0fa708b91fa3674d0805d15bc8` |
| Unchanged companion EXE | `970cbf7e6ee5d422cec1b6a7579c70fb2b34d39b154fa8637e2a164bbdd0b201` |

The DLL delta is file offset `0xA3BF4D`: `74 4A` becomes `EB 4A`. Both branches target
DLL RVA `0xA3CB99`; the fix changes when the safe exit is taken, without adding native code.

For an authorized installation, close the affected client, retain its prior DLL and update
the native-pair manifest through the existing deployment guards. Keep the native realmlist
and RealmData companion fixes. Set `Data/<locale>/realmlist.wtf` to the authserver and set
the server's realm address to its reachable world endpoint. Enable the module's remote
client compatibility settings as described in its [README](../../modules/mod-ascension-compat/README.md).
