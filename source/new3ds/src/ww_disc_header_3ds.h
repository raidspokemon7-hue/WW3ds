#pragma once
#include <cstddef>
#include <cstring>

// Classify the first bytes of a Wind Waker disc image.
// 0 = not a usable USA Wind Waker image
// 1 = plain GZLE01 ISO
// 2 = CISO magic (Dolphin sparse image; boot via n3dsDiscPathKind)
inline int n3dsDiscHeaderKind(const unsigned char * h, size_t n)
{
    if (n >= 4 && std::memcmp(h, "CISO", 4) == 0) return 2;
    if (n >= 0x20
        && std::memcmp(h, "GZLE", 4) == 0
        && std::memcmp(h + 0x1C, "\xC2\x33\x9F\x3D", 4) == 0)
        return 1;
    return 0;
}
