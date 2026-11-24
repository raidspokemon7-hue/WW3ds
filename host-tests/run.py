#!/usr/bin/env python3
"""Run public host tests without any game assets, SDK or emulator."""
import argparse
import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import sys

ROOT=Path(__file__).resolve().parents[1]
def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--python-only',action='store_true',help='Skip C++ tests (requires no compiler)')
    parser.add_argument('--texture-tools',action='store_true',help='Build and test the native PC texture converter too')
    args=parser.parse_args()
    if args.texture_tools:
        if args.python_only:parser.error('--texture-tools requires a C++17 compiler; omit --python-only')
        subprocess.run([sys.executable,str(ROOT/'utilities/texture-converter/build.py')],check=True)
    for folder in ['host-tests','sd-card-installer','utilities/sd-export','utilities/console-logs','utilities/timing-overlay','utilities/release-prep','utilities/release-prep/runtime-assets','utilities/texture-converter','utilities/ciso-to-iso']:
        subprocess.run([sys.executable,'-m','unittest','discover','-v','-s',str(ROOT/folder)],check=True,cwd=ROOT)
    if not args.python_only:
        compiler=os.environ.get('CXX') or shutil.which('g++') or shutil.which('c++') or shutil.which('clang++')
        if not compiler:parser.error('A C++17 compiler is required, or use --python-only.')
        with tempfile.TemporaryDirectory(prefix='ww3ds-test-') as directory:
            binary=Path(directory)/('platform.exe' if os.name=='nt' else 'platform')
            subprocess.run([compiler,'-std=c++17','-O2','-Wall','-Wextra','-Werror','-I'+str(ROOT/'system/new3ds'),str(ROOT/'host-tests/platform.cpp'),'-o',str(binary)],check=True)
            subprocess.run([str(binary),str(Path(directory)/'qol.ini')],check=True)
if __name__=='__main__':main()
