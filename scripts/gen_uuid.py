#!/usr/bin/env python3
"""
Generate UUIDs in the TAU_DECL_UUID literal format.

This mirrors what ComUUIDGenerator.exe (the C# tool) emits: an
RFC 4122 version 4 (random) UUID, split into two big-endian 64-bit
halves and printed as `0xXXXXXXXXXXXXXXXXull, 0xYYYYYYYYYYYYYYYYull`.

Usage:
    scripts/gen_uuid.py          # one UUID
    scripts/gen_uuid.py -n 3     # three UUIDs
    scripts/gen_uuid.py -d MyType
        # full TAU_DECL_UUID(MyType, ...); line
"""
from __future__ import annotations

import argparse
import sys
import uuid


def format_uuid(g: uuid.UUID) -> str:
    h = g.hex.upper()
    return f"0x{h[:16]}ull, 0x{h[16:]}ull"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("-n", "--count", type=int, default=1, help="number of UUIDs to generate (default: 1)")
    parser.add_argument("-d", "--decl", metavar="TYPE", help="emit a full TAU_DECL_UUID(TYPE, ...); line")
    args = parser.parse_args()

    if args.count < 1:
        parser.error("--count must be >= 1")

    for _ in range(args.count):
        literal = format_uuid(uuid.uuid4())
        if args.decl:
            print(f"TAU_DECL_UUID({args.decl}, {literal});")
        else:
            print(literal)

    return 0


if __name__ == "__main__":
    sys.exit(main())
