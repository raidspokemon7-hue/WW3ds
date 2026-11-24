#pragma once
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <vector>
#include "ww_n3ds.h"
#include "ww_ciso_3ds.h"
#include "ww_settings_canvas_3ds.h"
namespace n3ds_icons 
{
    inline uint32_t be32(const uint8_t * p)
    {
        return uint32_t(p[0]) << 24|uint32_t(p[1]) << 16|uint32_t(p[2]) << 8|p[3];
        
    }
    inline uint16_t be16(const uint8_t * p)
    {
        return uint16_t((uint16_t(p[0]) << 8)|p[1]);
        
    }
    inline uint8_t ex4(uint32_t n)
    {
        return uint8_t((n << 4)|n);
        
    }
    inline uint8_t ex5(uint32_t n)
    {
        return uint8_t((n << 3)|(n >> 2));
        
    }
    inline uint8_t ex3(uint32_t n)
    {
        return uint8_t((n << 5)|(n << 2)|(n >> 1));
        
    }
    inline uint8_t ex6(uint32_t n)
    {
        return uint8_t((n << 2)|(n >> 4));
        
    }
    inline const char * fileFor(uint8_t id)
    {
        static const char * const names[256] = 
        {
            "bottle_00.bti", "get_rupy.bti", "get_rupy.bti", "get_rupy.bti", 
            "get_rupy.bti", "get_rupy.bti", "get_rupy.bti", "heart_up_02.bti", 
            "heart_up_01.bti", "bottle_00.bti", "bottle_00.bti", "bomb_00.bti", 
            "bomb_00.bti", "bomb_00.bti", "bomb_00.bti", "get_rupy.bti", 
            "bow_01.bti", "bow_01.bti", "bow_01.bti", nullptr, 
            nullptr, "get_key.bti", nullptr, "heart_up_02.bti", 
            "heart_up_02.bti", "heart_up_02.bti", "get_rupy.bti", nullptr, 
            nullptr, nullptr, "bottle_00.bti", "beast_08.bti", 
            "telescope.bti", "whistle.bti", "baton.bti", "camera.bti", 
            "coverofbeast.bti", "rope.bti", "camera_2.bti", "bow_01.bti", 
            "gloves_00.bti", "boots_00.bti", "shield_02.bti", "boots_01.bti", 
            "coverofbait.bti", "boomerang.bti", "gloves_00.bti", "hookshot.bti", 
            "delivery.bti", "bomb_00.bti", "clothes.bti", "hammer_01.bti", 
            "fan.bti", "arrow_power_01.bti", "arrow_power_02.bti", "clothes.bti", 
            "sword_00.bti", "sword_01.bti", "sword_02.bti", "shield_00.bti", 
            "shield_01.bti", "sword_00.bti", "sword_03.bti", "heart_up_02.bti", 
            nullptr, nullptr, "amulet_00.bti", "amulet_01.bti", 
            "bottle_00.bti", "beast_01.bti", "beast_02.bti", "beast_03.bti", 
            "beast_04.bti", "beast_05.bti", "beast_06.bti", "beast_07.bti", 
            "dungeon_map.bti", "compass.bti", "boss_key.bti", "bottle_00.bti", 
            "bottle_00.bti", "bottle_01.bti", "bottle_02.bti", "bottle_03.bti", 
            "bottle_09.bti", "bottle_04.bti", "bottle_05.bti", "bottle_06.bti", 
            "bottle_07.bti", "bottle_08.bti", nullptr, nullptr, 
            nullptr, nullptr, nullptr, nullptr, 
            nullptr, "triforce_00.bti", "triforce_01.bti", "triforce_02.bti", 
            "triforce_03.bti", "triforce_04.bti", "triforce_05.bti", "triforce_06.bti", 
            "triforce_07.bti", "god_symbol_02.bti", "god_symbol_00.bti", "god_symbol_01.bti", 
            "amulet_00.bti", "baton.bti", "baton.bti", "baton.bti", 
            "baton.bti", "baton.bti", "baton.bti", nullptr, 
            nullptr, nullptr, nullptr, nullptr, 
            "sail_00.bti", "cmap_tri2.bti", "cmap_tri2.bti", "cmap_tri2.bti", 
            "cmap_tri2.bti", "cmap_tri2.bti", "cmap_tri2.bti", "cmap_tri2.bti", 
            "cmap_tri2.bti", nullptr, "bait_01.bti", "bait_02.bti", 
            nullptr, nullptr, nullptr, nullptr, 
            nullptr, nullptr, nullptr, nullptr, 
            "delivery_01.bti", "delivery_02.bti", "delivery_03.bti", "delivery_04.bti", 
            "delivery_05.bti", "delivery_06.bti", "delivery_07.bti", "delivery_08.bti", 
            "delivery_09.bti", "delivery_10.bti", "delivery_11.bti", "delivery_12.bti", 
            "delivery_13.bti", "delivery_14.bti", "delivery_15.bti", "delivery_16.bti", 
            "delivery_17.bti", "delivery_18.bti", "delivery_19.bti", "camera.bti", 
            "amulet_00.bti", "amulet_00.bti", "amulet_00.bti", "tingle_figure.bti", 
            "tingle_figure.bti", "tingle_figure.bti", "tingle_figure.bti", "tingle_figure.bti", 
            nullptr, nullptr, nullptr, "big_purse.bti", 
            "max_purse.bti", "bombpouch_1.bti", "bombpouch_2.bti", "arrowcase_1.bti", 
            "arrowcase_2.bti", nullptr, nullptr, "get_rupy.bti", 
            "get_rupy.bti", "get_rupy.bti", "get_rupy.bti", "get_rupy.bti", 
            "get_rupy.bti", "get_rupy.bti", "get_rupy.bti", "get_rupy.bti", 
            "get_rupy.bti", "get_rupy.bti", "get_rupy.bti", "get_rupy.bti", 
            "get_rupy.bti", "get_rupy.bti", "cmap_hint2.bti", "cmap_hint2.bti", 
            "cmap_hint2.bti", "cmap_hint2.bti", "cmap_hint2.bti", "cmap_hint2.bti", 
            "cmap_hint2.bti", "cmap_hint2.bti", "cmap_hint2.bti", "cmap_tingle2.bti", 
            "cmap_treasure2.bti", "cmap_treasure2.bti", "cmap_treasure2.bti", "cmap_treasure2.bti", 
            "cmap_treasure2.bti", "cmap_treasure2.bti", "cmap_treasure2.bti", "cmap_treasure2.bti", 
            "cmap_treasure2.bti", "cmap_treasure2.bti", "cmap_treasure2.bti", "cmap_treasure2.bti", 
            "cmap_treasure2.bti", "cmap_treasure2.bti", "cmap_treasure2.bti", "cmap_phantomship2.bti", 
            "cmap_tingle2.bti", "cmap_treasure2.bti", "cmap_treasure2.bti", "cmap_treasure2.bti", 
            "cmap_treasure2.bti", "cmap_treasure2.bti", "cmap_treasure2.bti", "cmap_treasure2.bti", 
            "cmap_treasure2.bti", "cmap_treasure2.bti", "cmap_treasure2.bti", "cmap_treasure2.bti", 
            "cmap_treasure2.bti", "cmap_treasure2.bti", "cmap_treasure2.bti", "cmap_treasure2.bti", 
            "cmap_treasure2.bti", "cmap_treasure2.bti", "cmap_treasure2.bti", "cmap_treasure2.bti", 
            "cmap_treasure2.bti", "cmap_treasure2.bti", "cmap_treasure2.bti", "cmap_treasure2.bti", 
            "cmap_treasure2.bti", "cmap_treasure2.bti", "cmap_treasure2.bti", "cmap_tri2.bti", 
            "cmap_tri2.bti", "cmap_tri2.bti", "cmap_tri2.bti", "cmap_tri2.bti", 
            "cmap_tri2.bti", "cmap_tri2.bti", "cmap_tri2.bti", nullptr, 
            
        } ;
        return names[id];
        
    }
    inline N3dsCisoMap& discMap()
    {
        static N3dsCisoMap m;
        return m;
        
    }
    inline bool readAt(FILE * f,
uint32_t off,
void * dst,
size_t n)
    {
        return n3dsCisoReadFile(f,
&discMap(),
off,
dst,
n);
        
    }
    inline bool yaz0(const uint8_t * b,
size_t n,
std::vector < uint8_t > & out)
    {
        if(n < 16 || std::memcmp(b,
"Yaz0",
4)) return false;
        const uint32_t dec = be32(b + 4);
        if(dec > 256 * 1024) return false;
        out.clear();
        out.reserve(dec);
        size_t i = 16;
        while(out.size() < dec)
        {
            if(i >= n) return false;
            const uint8_t code = b[i++];
            for (int bit=7; bit>=0 
&& out.size()<dec; --bit)
            {
                if(code&(1u << bit))
                {
                    if(i >= n) return false;
                    out.push_back(b[i++]);
                    
                } else
                {
                    if(i + 1 >= n) return false;
                    const uint8_t b1 = b[i++], b2 = b[i++];
                    const uint32_t dist = ((b1&0xF) << 8)|b2;
                    uint32_t copy = b1 >> 4;
                    if(!copy)
                    {
                        if(i >= n) return false;
                        copy = uint32_t(b[i++]) + 0x12;
                        
                    }
                    else copy += 2;
                    if(dist + 1 > out.size()) return false;
                    const size_t pos0 = out.size() - (dist + 1);
                    for (uint32_t k=0;k<copy;++k) out.push_back(out[pos0 + k]);
                    
                }
                
            }
            
        }
        return true;
        
    }
    inline bool findPath(const uint8_t * fst,
size_t fstSize,
const char * const * parts,
int nparts,
uint32_t& off,
uint32_t& size)
    {
        if(fstSize < 12) return false;
        const uint32_t count = be32(fst + 8);
        if(uint64_t(count) * 12 > fstSize) return false;
        const char * names = reinterpret_cast < const char * > (fst) + count * 12;
        const size_t namesLen = fstSize - size_t(count) * 12;
        uint32_t dir = 0;
        for (int p=0;p<nparts;++p)
        {
            const uint32_t end = be32(fst + dir * 12 + 8);
            bool found = false;
            for (uint32_t i=dir+1;i<end;)
            {
                const uint8_t * e = fst + i * 12;
                const uint32_t no = be32(e)&0xffffff;
                const char * nm = no < namesLen?names + no:"";
                const bool isDir = e[0] != 0;
                if(std::strcmp(nm,
parts[p]) == 0)
                {
                    if(p + 1 == nparts)
                    {
                        if(isDir) return false;
                        off = be32(e + 4);
                        size = be32(e + 8);
                        return size 
&& size < 4u * 1024u * 1024u;
                        
                    }
                    if(!isDir) return false;
                    dir = i;
                    found = true;
                    break;
                    
                }
                i = isDir?be32(e + 8):i + 1;
                
            }
            if(!found) return false;
            
        }
        return false;
        
    }
    struct Image 
    {
        int w = 0, h = 0;
        std::vector < uint8_t > rgba;
        
    } ;
    inline void putPx(uint8_t * o,
uint8_t r,
uint8_t g,
uint8_t b,
uint8_t a)
    {
        o[0] = r;
        o[1] = g;
        o[2] = b;
        o[3] = a;
        
    }
    inline void palColor(int fmt,
uint16_t v,
uint8_t * o)
    {
        if(fmt == 1)
        {
            putPx(o,
ex5((v >> 11)&31),
ex6((v >> 5)&63),
ex5(v&31),
255);
            return;
            
        }
        if(fmt == 0)
        {
            const uint8_t i = uint8_t(v&255);
            putPx(o,
i,
i,
i,
uint8_t(v >> 8));
            return;
            
        }
        if(v&0x8000) putPx(o,
ex5((v >> 10)&31),
ex5((v >> 5)&31),
ex5(v&31),
255);
        else putPx(o,
ex4((v >> 8)&15),
ex4((v >> 4)&15),
ex4(v&15),
ex3((v >> 12)&7));
        
    }
    inline bool decodeBti(const uint8_t * b,
size_t n,
Image& img)
    {
        if(n < 0x20) return false;
        const int fmt = b[0];
        const int w = be16(b + 2), h = be16(b + 4);
        if(w < 1 || h < 1 || w > 128 || h > 128) return false;
        int bw = 8, bh = 4, bbytes = 32;
        if(fmt == 8)
        {
            bw = 8;
            bh = 8;
            
        }
        else if(fmt == 9 || fmt == 2)
        {
            bw = 8;
            bh = 4;
            
        }
        else return false;
        const uint32_t imgOff = be32(b + 0x1C);
        const size_t blocksX = (size_t(w) + bw - 1) / bw, blocksY = (size_t(h) + bh - 1) / bh, need = blocksX * blocksY * size_t(bbytes);
        if(uint64_t(imgOff) + need > n) return false;
        const int tlutFmt = b[9];
        const int ncol = be16(b + 0xA);
        const uint32_t palOff = be32(b + 0xC);
        if((fmt == 8 || fmt == 9) 
&& (ncol < 1 || uint64_t(palOff) + size_t(ncol) * 2 > n)) return false;
        img.w = w;
        img.h = h;
        img.rgba.assign(size_t(w) * size_t(h) * 4,
0);
        const uint8_t * in = b + imgOff;
        for (size_t by=0; by<blocksY; ++by)
        for (size_t bx=0; bx<blocksX; ++bx,
in+=bbytes)
        for (int y=0;y<bh;++y) for (int x=0;x<bw;++x)
        {
            const int px = int(bx) * bw + x, py = int(by) * bh + y;
            if(px >= w || py >= h) continue;
            uint8_t * o = &img.rgba[(size_t(py) * size_t(w) + px) * 4];
            if(fmt == 2)
            {
                const uint8_t v = in[y * 8 + x];
                const uint8_t i = ex4(v&15);
                putPx(o,
i,
i,
i,
ex4(v >> 4));
                
            } else
            {
                unsigned idx;
                if(fmt == 8)
                {
                    const uint8_t v = in[(y * 8 + x) / 2];
                    idx = (x&1)?(v&15):(v >> 4);
                    
                }
                else idx = in[y * 8 + x];
                if(int(idx) >= ncol) putPx(o,
0,
0,
0,
0);
                else palColor(tlutFmt,
be16(b + palOff + idx * 2),
o);
                
            }
            
        }
        return true;
        
    }
    inline const std::vector < uint8_t > * archive()
    {
        static std::vector < uint8_t > arc;
        static int state = 0;
        if(state) return state > 0?&arc:nullptr;
        const char * disc = std::getenv("COS_DISC");
        if(!disc || ! * disc) disc = N3DS_ROOT "/game/GZLE01.iso";
        FILE * f = std::fopen(disc,
"rb");
        if(!f) return nullptr;
        state = - 1;
        unsigned char cisoHead[kN3dsCisoHeaderSize];
        std::fseek(f,
0,
SEEK_SET);
        const size_t got = std::fread(cisoHead,
1,
sizeof cisoHead,
f);
        discMap() = N3dsCisoMap();
        if (got >= 4 
&& std::memcmp(cisoHead,
"CISO",
4) == 0) discMap().load(cisoHead,
got);
        uint8_t hdr[0x440];
        if(!readAt(f,
0,
hdr,
sizeof hdr) || std::memcmp(hdr,
"GZLE",
4) || std::memcmp(hdr + 0x1C,
"\xC2\x33\x9F\x3D",
4))
        {
            std::fclose(f);
            return nullptr;
            
        }
        const uint32_t fstOff = be32(hdr + 0x424), fstSize = be32(hdr + 0x428);
        if(fstSize < 12 || fstSize > 1024 * 1024)
        {
            std::fclose(f);
            return nullptr;
            
        }
        std::vector < uint8_t > fst(fstSize);
        if(!readAt(f,
fstOff,
fst.data(),
fstSize))
        {
            std::fclose(f);
            return nullptr;
            
        }
        const char * parts[] = 
        {
            "res", "Msg", "itemicon.arc"
        } ;
        uint32_t off = 0, size = 0;
        if(!findPath(fst.data(),
fst.size(),
parts,
3,
off,
size))
        {
            std::fclose(f);
            return nullptr;
            
        }
        arc.resize(size);
        const bool ok = readAt(f,
off,
arc.data(),
size) 
&& std::memcmp(arc.data(),
"RARC",
4) == 0;
        std::fclose(f);
        if(!ok)
        {
            arc.clear();
            return nullptr;
            
        }
        state = 1;
        return &arc;
        
    }
    inline bool rarcFile(const std::vector < uint8_t > & arc,
const char * name,
const uint8_t * & data,
uint32_t& size)
    {
        if(arc.size() < 0x40) return false;
        const uint32_t num = be32(&arc[0x28]);
        const uint32_t entry = 0x20 + be32(&arc[0x2C]);
        const uint32_t strs = 0x20 + be32(&arc[0x34]);
        const uint32_t base = 0x20 + be32(&arc[0xC]);
        for (uint32_t i=0;i<num;++i)
        {
            const size_t o = entry + size_t(i) * 0x14;
            if(o + 0x10 > arc.size()) return false;
            const uint32_t tf = be32(&arc[o + 4]);
            if((tf >> 24)&2) continue;
            const uint32_t no = tf&0xffffff;
            if(size_t(strs) + no >= arc.size()) continue;
            if(std::strcmp(reinterpret_cast < const char * > (&arc[size_t(strs) + no]),
name)) continue;
            const uint32_t start = base + be32(&arc[o + 8]);
            size = be32(&arc[o + 0xC]);
            if(uint64_t(start) + size > arc.size()) return false;
            data = &arc[start];
            return true;
            
        }
        return false;
        
    }
    inline Image * cached(uint8_t id)
    {
        struct Slot 
        {
            uint8_t id = 0;
            bool used = false;
            Image img;
            
        } ;
        static Slot slots[8];
        static int next = 0;
        for (auto& s:slots) if(s.used 
&& s.id == id) return &s.img;
        const char * name = fileFor(id);
        const auto * arc = name?archive():nullptr;
        if(!arc) return nullptr;
        const uint8_t * data = nullptr;
        uint32_t size = 0;
        if(!rarcFile( * arc,
name,
data,
size)) return nullptr;
        std::vector < uint8_t > plain;
        const uint8_t * b = data;
        size_t n = size;
        if(n >= 4 
&& std::memcmp(data,
"Yaz0",
4) == 0)
        {
            if(!yaz0(data,
size,
plain)) return nullptr;
            b = plain.data();
            n = plain.size();
            
        }
        Slot& s = slots[next++&7];
        s.used = false;
        if(!decodeBti(b,
n,
s.img)) return nullptr;
        s.id = id;
        s.used = true;
        return &s.img;
        
    }
    inline bool paint(ww_screen::Canvas& c,
int x,
int y,
int box,
uint8_t id,
bool enabled)
    {
        Image * img = cached(id);
        if(!img || img->w < 1 || img->h < 1) return false;
        const int dw = img->w >= img->h?box:std::max(1,
box * img->w / img->h);
        const int dh = img->h >= img->w?box:std::max(1,
box * img->h / img->w);
        const int ox = x + (box - dw) / 2, oy = y + (box - dh) / 2;
        for (int j=0;j<dh;++j) for (int i=0;i<dw;++i)
        {
            const int sx = i * img->w / dw, sy = j * img->h / dh;
            const uint8_t * p = &img->rgba[(size_t(sy) * img->w + size_t(sx)) * 4];
            if(p[3] < 16) continue;
            const int dx = ox + i, dy = oy + j;
            if(dx < 0 || dy < 0 || dx >= 320 || dy >= 240) continue;
            unsigned nr = p[0], ng = p[1], nb = p[2];
            const unsigned a = enabled?p[3]:p[3] * 2 / 3;
            if(a < 255)
            {
                const uint16_t old = c.pixels[dx * 240 + 239 - dy];
                const unsigned r = ((old >> 11) * 255 + 15) / 31, g = (((old >> 5)&63) * 255 + 31) / 63, b = ((old&31) * 255 + 15) / 31;
                nr = (nr * a + r * (255 - a) + 127) / 255;
                ng = (ng * a + g * (255 - a) + 127) / 255;
                nb = (nb * a + b * (255 - a) + 127) / 255;
                
            }
            if(!enabled)
            {
                nr >>= 1;
                ng >>= 1;
                nb >>= 1;
                
            }
            c.put(dx,
dy,
(nr << 16)|(ng << 8)|nb);
            
        }
        return true;
        
    }
    
}
