#!/usr/bin/env python3
"""Expand a Dolphin/Wiimm GameCube CISO into a plain ISO for WW3ds / Azahar.

Dolphin CISO is a sparse image (not the PSP zlib .ciso):
  0x0000  "CISO"
  0x0004  u32 LE block_size
  0x0008  0x7FF8-byte map (1 = block present, else zero-fill)
  0x8000  present blocks concatenated in map order

Output is truncated to the standard GameCube image size (or the last
present block, whichever is smaller) so empty high map slots do not
create a multi‑GB file of zeros.
"""
from __future__ import annotations

import argparse
import struct
from pathlib import Path

CISO_HEADER_SIZE = 0x8000
CISO_MAP_SIZE = CISO_HEADER_SIZE - 8
# Standard single-layer GameCube disc image size (Wind Waker USA).
GC_ISO_SIZE = 1_459_978_240


def convert(src: Path, dst: Path) -> None:
    with src.open("rb") as f:
        header = f.read(CISO_HEADER_SIZE)
        if len(header) < CISO_HEADER_SIZE:
            raise SystemExit(f"{src}: too small to be a Dolphin CISO")
        if header[:4] != b"CISO":
            raise SystemExit(f"{src}: not a CISO (missing CISO magic)")
        (block_size,) = struct.unpack_from("<I", header, 4)
        # PSP CSO puts header_size=0x18 here; Dolphin puts a large power-of-two block size.
        if block_size == 0x18:
            raise SystemExit(
                f"{src}: looks like a PSP CSO, not a GameCube/Wii CISO.\n"
                "Convert with Dolphin: right-click → Convert → ISO."
            )
        if block_size == 0 or (block_size & (block_size - 1)):
            raise SystemExit(f"{src}: bad CISO block size {block_size}")
        block_map = header[8:]
        last_present = -1
        present = 0
        for i, status in enumerate(block_map):
            if status == 1:
                present += 1
                last_present = i
        if last_present < 0:
            raise SystemExit(f"{src}: CISO map has no present blocks")
        logical = (last_present + 1) * block_size
        out_size = min(logical, GC_ISO_SIZE)
        blocks_to_write = (out_size + block_size - 1) // block_size

        expected = present * block_size
        payload = f.read()
        if len(payload) < expected:
            raise SystemExit(
                f"{src}: truncated CISO payload ({len(payload)} bytes for {present} blocks)"
            )

        dst.parent.mkdir(parents=True, exist_ok=True)
        written = 0
        pos = 0
        with dst.open("wb") as out:
            for i, status in enumerate(block_map):
                if i >= blocks_to_write:
                    break
                if status == 1:
                    chunk = payload[pos : pos + block_size]
                    pos += block_size
                else:
                    chunk = b"\0" * block_size
                remain = out_size - written
                if remain <= 0:
                    break
                if len(chunk) > remain:
                    chunk = chunk[:remain]
                out.write(chunk)
                written += len(chunk)

    print(
        f"wrote {dst} ({written} bytes; block {block_size}, "
        f"{present}/{len(block_map)} blocks present, "
        f"capped at {out_size})"
    )


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("ciso", type=Path, help="input .ciso")
    p.add_argument(
        "-o",
        "--output",
        type=Path,
        help="output .iso (default: same stem with .iso)",
    )
    args = p.parse_args()
    src = args.ciso.expanduser().resolve()
    if not src.is_file():
        raise SystemExit(f"missing file: {src}")
    dst = args.output.expanduser().resolve() if args.output else src.with_suffix(".iso")
    if dst == src:
        raise SystemExit("output path must differ from the CISO")
    convert(src, dst)


if __name__ == "__main__":
    main()
