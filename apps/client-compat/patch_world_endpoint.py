"""Generate the verified native world-address fix without modifying the input DLL."""

import argparse
import hashlib
from pathlib import Path


BASELINE_SHA256 = "f7b713095aab17a1e376f487290d4b7c4c18931635e4d91136d76db2592be8fa"
PATCHED_SHA256 = "9791801053f828d1ccdab1a4c17e64852d3ebe0fa708b91fa3674d0805d15bc8"
PATCH_OFFSET = 0xA3BF4D


def generate(source, destination):
    if source.resolve() == destination.resolve():
        raise ValueError("Input and output must be different files; in-place patching is not supported.")
    if destination.exists() or destination.is_symlink():
        raise FileExistsError("Output already exists; choose a new candidate path.")

    data = bytearray(source.read_bytes())
    checksum = hashlib.sha256(data).hexdigest()
    if checksum == BASELINE_SHA256:
        # DLL RVA 0xA3CB4D: JE -> JMP to 0xA3CB99, preserving the displacement.
        # The skipped failure block corrupts the active EXE hook at 0x403340.
        if data[PATCH_OFFSET:PATCH_OFFSET + 2] != bytes.fromhex("744a"):
            raise ValueError("The expected native world-address branch is missing.")
        data[PATCH_OFFSET] = 0xEB
    elif checksum != PATCHED_SHA256:
        raise ValueError("Unsupported Extensions.dll; expected the pinned native v4 baseline or patched DLL.")

    if hashlib.sha256(data).hexdigest() != PATCHED_SHA256:
        raise ValueError("Patched DLL does not match the verified output.")
    # Exclusive creation also protects an output created after the initial check.
    with destination.open("xb") as output:
        output.write(data)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", required=True, type=Path, help="Pinned native v4 Extensions.dll")
    parser.add_argument("--output", required=True, type=Path, help="New candidate file; never overwritten")
    args = parser.parse_args()
    try:
        generate(args.input, args.output)
    except (OSError, ValueError) as error:
        parser.error(str(error))
    print(f"Generated {args.output}; SHA256 {PATCHED_SHA256}. No client was installed or launched.")


if __name__ == "__main__":
    main()
