#pragma once
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

// Dolphin/Wiimm GameCube CISO: sparse ISO, not PSP zlib CSO.
// Header is 0x8000 bytes: "CISO", u32 LE block_size, then 0x7FF8 map bytes
// (1 = present). Present blocks are concatenated from 0x8000.
inline constexpr uint32_t kN3dsCisoHeaderSize = 0x8000;
inline constexpr uint32_t kN3dsCisoMapSize = kN3dsCisoHeaderSize - 8;
inline constexpr uint16_t kN3dsCisoAbsent = 0xFFFF;

struct N3dsCisoMap
{
    bool active = false;
    uint32_t blockSize = 0;
    std::vector<uint16_t> fileIndex;

    bool load(const unsigned char * header, size_t n)
    {
        active = false;
        fileIndex.clear();
        blockSize = 0;
        if (n < kN3dsCisoHeaderSize || std::memcmp(header, "CISO", 4) != 0) return false;
        uint32_t size = 0;
        std::memcpy(&size, header + 4, 4);
        // PSP CSO stores header_size 0x18 here.
        if (size == 0x18 || size == 0 || (size & (size - 1))) return false;
        blockSize = size;
        fileIndex.assign(kN3dsCisoMapSize, kN3dsCisoAbsent);
        uint16_t present = 0;
        const unsigned char * map = header + 8;
        for (uint32_t i = 0; i < kN3dsCisoMapSize; i++)
        {
            if (map[i] == 1)
            {
                fileIndex[i] = present;
                ++present;
            }
        }
        active = true;
        return true;
    }

    // Translate one virtual ISO range into either zeros or a file offset.
    // Returns bytes handled in this step (up to one CISO block).
    size_t step(uint64_t virt, size_t want, uint64_t * fileOff, bool * zero) const
    {
        if (!active || !blockSize || !want)
        {
            if (fileOff) *fileOff = virt;
            if (zero) *zero = false;
            return want;
        }
        const uint64_t block = virt / blockSize;
        const uint32_t inner = uint32_t(virt % blockSize);
        const size_t chunk = want < (blockSize - inner) ? want : size_t(blockSize - inner);
        if (block >= fileIndex.size() || fileIndex[size_t(block)] == kN3dsCisoAbsent)
        {
            if (zero) *zero = true;
            if (fileOff) *fileOff = 0;
            return chunk;
        }
        if (zero) *zero = false;
        if (fileOff)
            *fileOff = uint64_t(kN3dsCisoHeaderSize) + uint64_t(fileIndex[size_t(block)]) * blockSize + inner;
        return chunk;
    }
};

inline bool n3dsCisoReadFile(FILE * f, const N3dsCisoMap * map, uint64_t off, void * dst, size_t n)
{
    auto * out = static_cast<unsigned char *>(dst);
    while (n)
    {
        uint64_t fileOff = off;
        bool zero = false;
        const size_t chunk = map && map->active ? map->step(off, n, &fileOff, &zero) : n;
        if (zero) std::memset(out, 0, chunk);
        else
        {
            if (std::fseek(f, long(fileOff), SEEK_SET) != 0) return false;
            if (std::fread(out, 1, chunk, f) != chunk) return false;
        }
        out += chunk;
        off += chunk;
        n -= chunk;
    }
    return true;
}

// Classify a disc path: 0 = unusable, 1 = plain GZLE ISO, 2 = GZLE CISO.
inline int n3dsDiscPathKind(const char * path)
{
    if (!path) return 0;
    FILE * f = std::fopen(path, "rb");
    if (!f) return 0;
    unsigned char h[kN3dsCisoHeaderSize];
    const size_t n = std::fread(h, 1, sizeof h, f);
    int kind = 0;
    if (n >= 4 && std::memcmp(h, "CISO", 4) == 0)
    {
        N3dsCisoMap map;
        unsigned char iso[0x20];
        if (map.load(h, n) && n3dsCisoReadFile(f, &map, 0, iso, sizeof iso)
            && std::memcmp(iso, "GZLE", 4) == 0
            && std::memcmp(iso + 0x1C, "\xC2\x33\x9F\x3D", 4) == 0)
            kind = 2;
    }
    else if (n >= 0x20 && std::memcmp(h, "GZLE", 4) == 0
             && std::memcmp(h + 0x1C, "\xC2\x33\x9F\x3D", 4) == 0)
        kind = 1;
    std::fclose(f);
    return kind;
}
