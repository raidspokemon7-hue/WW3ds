#pragma once
#include <stdint.h>

// Tracks which 3DS teardown steps have already run so exit is idempotent
// and never skips apt after gfx, or runs C3D_Fini twice.
enum N3dsExitStep : uint32_t
{
    N3DS_EXIT_PREPARE = 1u << 0,
    N3DS_EXIT_CARD = 1u << 1,
    N3DS_EXIT_AUDIO = 1u << 2,
    N3DS_EXIT_GPU = 1u << 3,
    N3DS_EXIT_GFX = 1u << 4,
    N3DS_EXIT_APT = 1u << 5
};

struct N3dsExitState
{
    uint32_t done = 0;

    // Returns true the first time this step is recorded.
    bool mark(uint32_t step)
    {
        if (done & step) return false;
        done |= step;
        return true;
    }

    bool has(uint32_t step) const
    {
        return (done & step) != 0;
    }

    // Safe to return to the HOME Menu only after gfx and apt are down.
    bool readyForHome() const
    {
        return has(N3DS_EXIT_GFX) && has(N3DS_EXIT_APT);
    }
};
