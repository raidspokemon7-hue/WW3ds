# Flicker then ARM11 on core 2

## Symptom

Gameplay briefly flickers, then Luma reports:

- Processor: Arm11 **(core 2)** — the render-owner thread
- Exception: data abort, Translation - Section
- FAR: `00000000`, Access type: **Write**

## Cause

citro3d's `C3D_FrameEnd` path called `GX_DisplayTransfer` with a destination from `gfxGetFramebuffer`. When a CPU-side `gfxSwapBuffers` / bottom UI paint raced that transfer (first-boot cover, bottom panel, map column), the framebuffer pointer could be null and the GPU write faulted at address 0.

## Fix

- Skip display transfers and clears when the colour buffer or `gfxGetFramebuffer` result is null.
- Wait for the GX queue (`C3Di_RenderQueueWaitDone`) before CPU painting or swapping the same screens.
- Refuse draws whose shader handle is missing instead of binding a null program.
- Refuse vertex packing when the destination buffer is missing.
