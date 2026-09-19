"""Verify the package manifest before installing or sharing it."""
from pathlib import Path
import hashlib
import json

root = Path(__file__).resolve().parents[1]
manifest = json.loads((root / "MANIFEST.json").read_text())
for name, expected in manifest.items():
    path = (root / name).resolve()
    if not path.is_relative_to(root.resolve()):
        raise ValueError("Invalid manifest path")
    if hashlib.sha256(path.read_bytes()).hexdigest() != expected:
        raise ValueError("File differs from manifest: " + name)
print(f"Verified {len(manifest)} files")
