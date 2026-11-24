<p align="center">
  <img src="media/ww3ds-header.jpg" alt="WW3ds" width="100%">
</p>

<p align="center">
  A native port of Wind Waker to the Nintendo 3DS.
</p>

## Download

[WW3ds-sd.zip](https://github.com/raidspokemon7-hue/WW3ds/releases/download/v1.1/WW3ds-sd.zip)

Unzip onto the SD card. Install `cia/ww3ds.cia` with FBI. Put your own GZLE01.iso in `3ds/ww3ds/game/`.

This pack ships `performance.ini` with `foliage_far=2000` and `real_shadows=0` for better frame rate. Edit that file and restart to tune it.

## What you need

- A New 3DS, New 3DS XL or New 2DS XL with custom firmware and [FBI](https://github.com/Steveice10/FBI).
- Your USA copy of Wind Waker for GameCube, GZLE01, as a plain ISO or a Dolphin/Wiimm CISO. NKIT, RVZ, and PSP CSO will not boot.

- Azahar (PC) testing: see [documentation/azahar-testing.md](documentation/azahar-testing.md).

## Install

1. Install the `ww3ds` CIA with FBI.
2. On the SD card, put the disc here:

   ```text
   3ds/ww3ds/game/GZLE01.iso
   ```

   A Dolphin CISO is fine as `GZLE01.ciso` in the same folder. (CISO needs a newer CIA build from `main`.)

3. Start **ww3ds** from the HOME Menu.

If the disc is missing, the console tells you and START leaves the game.

## Saves

Saves are memory-card files at:

```text
3ds/ww3ds/saves/USA/Card A
```

To bring a GameCube save across, put a `.gci` or a Dolphin `.raw` in `3ds/ww3ds/saves/`. On the next boot it is imported into Card A, and the file you dropped is moved to `3ds/ww3ds/saves/imported/`. The same Card A files can go back to a GameCube or to Dolphin.

The starting island is the hardest place to run. Bringing a save that has already left Outset is the better way to start.

## First boot

The first time you launch the game, both screens show **Preparing Wind Waker** and the buttons do nothing. The game is building the caches it needs while the title screen runs offscreen. When that settles, the title appears. If it has not settled after 90 seconds, the title opens anyway.

Later launches skip that screen. The first visit to a new area can still hitch while new graphics are prepared. It gets smoother as you keep playing.

## How it runs

The aim is a steady 30 frames per second. Most places land around 15–25. Heavy places drop under 10. Cutscenes can pause. These are console speeds, not emulator numbers.

The bottom screen shows the X, Y and Z items, a Map button, a compass, and frame-rate caps of 24, 30 and 60. An empty item slot stays blank. START opens the game's pause menu. There is no Menu button on the touch screen.

## Credits

- [zeldaret/tww](https://github.com/zeldaret/tww)

Anyone modding or forking this with an AI agent should start from [AGENTS.md](AGENTS.md). [Report an issue](https://github.com/raidspokemon7-hue/WW3ds/issues).

---

The Legend of Zelda and The Wind Waker belong to Nintendo. WW3ds is an independent project and is not affiliated with or endorsed by Nintendo. Original project code is MIT-licensed. That licence does not cover Nintendo's game.
