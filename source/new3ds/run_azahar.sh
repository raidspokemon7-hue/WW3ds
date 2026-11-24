#!/usr/bin/env bash
exec 9>/tmp/ww3ds-azahar.lock; echo "waiting for the Azahar lock..." >&2; flock 9
SD=~/.var/app/org.azahar_emu.Azahar/data/azahar-emu/sdmc
R=$SD/ww3ds
here=$(cd "$(dirname "$0")" && pwd)
OUT=${OUT:-$here/shots}
SECS=${SECS:-120}
killall9() { pkill -9 -f azahar-launcher; pkill -9 -x azahar; pkill -f 'zenity --warning'; sleep 1; }
killall9
rm -f $R/log.txt $R/shots.bin
if [ -n "${SHOTS:-}" ]; then echo "$SHOTS" > $R/shots.txt; else rm -f $R/shots.txt; fi
launch() { flatpak run org.azahar_emu.Azahar "$here/ww3ds.3dsx" >/dev/null 2>&1 & }
launch; for i in $(seq 1 9); do sleep 5; [ -e $R/log.txt ] && break; done
[ -e $R/log.txt ] || { echo "no start: relaunching Azahar"; killall9; launch; }
t=0; while [ $t -lt $SECS ]; do sleep 5; t=$((t+5)); pgrep -f 'zenity --warning' >/dev/null && { echo "AZAHAR CRASHED"; break; }; pgrep -x azahar >/dev/null || { echo "AZAHAR EXITED"; break; }; done
killall9
cat $R/log.txt 2>/dev/null | tail -${LINES_SHOWN:-60}
rm -rf "$OUT"; [ -s $R/shots.bin ] && python3 -I ~/Downloads/ww-replay3ds/frames2png.py $R/shots.bin "$OUT" >/dev/null && ls "$OUT"
