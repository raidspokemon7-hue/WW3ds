#!/usr/bin/env bash
# Prepare Azahar's virtual SD for WW3ds and optionally install an ISO / 3dsx.
set -euo pipefail
ROOT=$(cd "$(dirname "$0")/.." && pwd)
SD_DEFAULT="$HOME/.var/app/org.azahar_emu.Azahar/data/azahar-emu/sdmc"
SD=${AZAHAR_SDMC:-$SD_DEFAULT}
ISO=${1:-}
APP=${2:-}

mkdir -p "$SD/ww3ds/game" "$SD/ww3ds/saves"
echo "Azahar SDMC: $SD"

if [ -n "$ISO" ]; then
  case "$ISO" in
    *.ciso|*.CISO)
      echo "installing CISO (WW3ds reads Dolphin CISO directly)..."
      cp -f "$ISO" "$SD/ww3ds/game/GZLE01.ciso"
      echo "installed CISO → $SD/ww3ds/game/GZLE01.ciso"
      ;;
    *)
      cp -f "$ISO" "$SD/ww3ds/game/GZLE01.iso"
      echo "installed ISO → $SD/ww3ds/game/GZLE01.iso"
      ;;
  esac
fi

if [ -n "$APP" ]; then
  cp -f "$APP" "$SD/ww3ds/ww3ds.3dsx"
  echo "installed app → $SD/ww3ds/ww3ds.3dsx"
  echo "launch with: flatpak run org.azahar_emu.Azahar \"$SD/ww3ds/ww3ds.3dsx\""
else
  echo "no .3dsx given; pass one as the second argument, or open it from Azahar after unzipping WW3ds-sd.zip onto $SD"
fi

echo "done. disc path expected by the port: sdmc:/ww3ds/game/GZLE01.iso"
