#!/usr/bin/env python3
import struct
import tempfile
import unittest
from pathlib import Path

from ciso_to_iso import CISO_HEADER_SIZE, CISO_MAP_SIZE, convert


class CisoConvertTests(unittest.TestCase):
    def test_sparse_blocks_roundtrip(self):
        block = 0x800
        raw0 = b"A" * block
        raw2 = b"C" * block
        block_map = bytearray(CISO_MAP_SIZE)
        block_map[0] = 1
        block_map[1] = 0
        block_map[2] = 1
        header = b"CISO" + struct.pack("<I", block) + bytes(block_map)
        self.assertEqual(len(header), CISO_HEADER_SIZE)
        payload = raw0 + raw2
        with tempfile.TemporaryDirectory() as d:
            src = Path(d) / "t.ciso"
            dst = Path(d) / "t.iso"
            src.write_bytes(header + payload)
            convert(src, dst)
            out = dst.read_bytes()
            self.assertEqual(out[:block], raw0)
            self.assertEqual(out[block : 2 * block], b"\0" * block)
            self.assertEqual(out[2 * block : 3 * block], raw2)
            # Stops after the last present block (not the full 0x7FF8 map).
            self.assertEqual(len(out), 3 * block)

    def test_rejects_plain_iso(self):
        with tempfile.TemporaryDirectory() as d:
            src = Path(d) / "plain.iso"
            src.write_bytes(b"GZLE" + b"\0" * 64)
            with self.assertRaises(SystemExit):
                convert(src, Path(d) / "out.iso")

    def test_rejects_psp_cso_header(self):
        with tempfile.TemporaryDirectory() as d:
            src = Path(d) / "psp.ciso"
            # PSP CSO: magic + header_size 0x18
            src.write_bytes(b"CISO" + struct.pack("<I", 0x18) + b"\0" * (CISO_HEADER_SIZE - 8))
            with self.assertRaises(SystemExit) as ctx:
                convert(src, Path(d) / "out.iso")
            self.assertIn("PSP", str(ctx.exception))


if __name__ == "__main__":
    unittest.main()
