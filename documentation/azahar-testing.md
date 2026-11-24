# Test WW3ds in Azahar

WW3ds is a native New 3DS homebrew title. Azahar can load the `.3dsx` against a virtual SD card. It is not a GameCube emulator.

## Disc image

Put a USA Wind Waker **ISO or Dolphin CISO** on the virtual SD:

```text
sdmc/ww3ds/game/GZLE01.iso
```

or `GZLE01.ciso` in that folder. NKIT / RVZ / PSP CSO still will not boot.

## One-time Azahar SD layout

Flatpak Azahar (Linux) uses:

```text
~/.var/app/org.azahar_emu.Azahar/data/azahar-emu/sdmc/ww3ds/
```

Other builds: open Azahar → Emulation → Configure → Open SDMC Directory, then create the same `ww3ds/...` tree inside it.

```bash
SD=~/.var/app/org.azahar_emu.Azahar/data/azahar-emu/sdmc   # or your SDMC path
mkdir -p "$SD/ww3ds/game" "$SD/ww3ds/saves"
cp /path/to/GZLE01.iso "$SD/ww3ds/game/GZLE01.iso"
```

## App binary

You need `ww3ds.3dsx` (or install the CIA into Azahar’s NAND).

- Release zip: [WW3ds-sd.zip](https://github.com/raidspokemon7-hue/WW3ds/releases/download/v1/WW3ds-sd.zip) — unzip onto the SD root.
- Local build (needs SwitchWaker + the container build): `source/new3ds/ww_build.sh`, then copy `ww3ds.3dsx`.

To exercise the boot/exit/flicker fixes from this branch, you must build from this tree; the public v1 zip does not include them yet.

## Launch

```bash
flatpak run org.azahar_emu.Azahar /path/to/ww3ds.3dsx
```

Or File → Open `ww3ds.3dsx` in Azahar.

First boot can sit on **Preparing Wind Waker** for up to ~90 seconds. Watch:

```text
sdmc/ww3ds/log.txt
```

## Expectation

Azahar is useful for boot, ISO detection, menus, and crash logs. Frame rate and GPU behaviour will not match a New 3DS.
