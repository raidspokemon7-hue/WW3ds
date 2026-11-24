# Boot waits and clean exit

## Boot

Long waits during boot now keep pumping APT so the system watchdog stays fed:

- After a failed `NS_RebootToTitle` attempt, the process waits a few seconds with APT/vblank instead of sleeping forever.
- Disc scan, save import, GPU init, and game start each set a boot stage string.
- The existing ~120s diagnostic watchdog logs that stage in every tick (`boot "disc"`, `boot "gpu"`, …).
- Missing disc and low-memory failures re-enable the bottom console, show the message on screen, and wait for START (or HOME quit) before tearing down.

Plain GZLE01 ISO and Dolphin CISO are accepted. Paths checked:

- `sdmc:/ww3ds/game/GZLE01.iso` and `GZLE01.ciso`
- any `.iso` / `.ciso` under `sdmc:/ww3ds/game/`
- the same names in `sdmc:/ww3ds/`

## Exit

Leaving the title (START on an error screen, HOME, or process exit) uses one ordered path:

1. Mark prepare / stop console spam
2. Quiesce the memory card (if the game started)
3. Stop NDSP audio
4. Shut down Aurora / GPU once (`C3D_Fini` only if `C3D_Init` succeeded)
5. Idle one vblank, then `gfxExit`
6. `aptExit`, then return to the system or `svcExitProcess`

There is no 10-second `svcExitProcess` race thread, and `__libctru_exit` routes through the same path instead of jumping straight to the return address after a partial `gfxExit`.
