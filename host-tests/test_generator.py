#!/usr/bin/env python3
"""Smoke-check the typed GX generator has no missing imports."""
import ast
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
GEN = ROOT / "source" / "new3ds" / "ww_gen_typed_gx.py"


class GeneratorImportTest(unittest.TestCase):
    def test_no_spread_layout_import(self):
        source = GEN.read_text(encoding="utf-8")
        tree = ast.parse(source, filename=str(GEN))
        imported = []
        for node in ast.walk(tree):
            if isinstance(node, ast.Import):
                imported.extend(alias.name for alias in node.names)
            elif isinstance(node, ast.ImportFrom):
                imported.append(node.module or "")
        self.assertNotIn("spread_layout", imported)
        self.assertIn("pathlib", imported)


if __name__ == "__main__":
    unittest.main()
