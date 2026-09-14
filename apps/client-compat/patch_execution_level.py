"""Remove the pinned native client's elevation request without changing executable code."""

import argparse
import hashlib
from pathlib import Path


BASELINE_SHA256 = "970cbf7e6ee5d422cec1b6a7579c70fb2b34d39b154fa8637e2a164bbdd0b201"
PATCHED_SHA256 = "f4b9f6fce448194638c5b1c751483090a48c597c6272237f31b3d151b50d3114"
MANIFEST_OFFSET = 0x7566F4  # RT_MANIFEST resource 1, language 1033.
MANIFEST_SIZE = 327
OLD_LEVEL = b'level="requireAdministrator"'
NEW_LEVEL = b'level="asInvoker"'.ljust(len(OLD_LEVEL), b" ")


def generate(source, destination):
    if source.resolve() == destination.resolve():
        raise ValueError("Input and output must be different files; in-place patching is not supported.")
    if destination.exists() or destination.is_symlink():
        raise FileExistsError("Output already exists; choose a new candidate path.")

    data = bytearray(source.read_bytes())
    checksum = hashlib.sha256(data).hexdigest()
    if checksum == BASELINE_SHA256:
        end = MANIFEST_OFFSET + MANIFEST_SIZE
        manifest = data[MANIFEST_OFFSET:end]
        if manifest.count(OLD_LEVEL) != 1:
            raise ValueError("The expected administrator execution level is missing.")
        # XML whitespace preserves the resource size and all following file offsets.
        # The accepted unsigned baseline has a zero PE checksum; leave its header unchanged.
        data[MANIFEST_OFFSET:end] = manifest.replace(OLD_LEVEL, NEW_LEVEL)
    elif checksum != PATCHED_SHA256:
        raise ValueError("Unsupported Ascension.exe; expected the pinned native v4 or standard-user EXE.")

    if hashlib.sha256(data).hexdigest() != PATCHED_SHA256:
        raise ValueError("Patched EXE does not match the verified output.")
    with destination.open("xb") as output:
        output.write(data)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", required=True, type=Path, help="Pinned native v4 Ascension.exe")
    parser.add_argument("--output", required=True, type=Path, help="New candidate file; never overwritten")
    args = parser.parse_args()
    try:
        generate(args.input, args.output)
    except (OSError, ValueError) as error:
        parser.error(str(error))
    print(f"Generated {args.output}; SHA256 {PATCHED_SHA256}. No client was installed or launched.")


if __name__ == "__main__":
    main()
