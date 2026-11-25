#pragma once
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <atomic>
#include <cmath>
#include "compass_la_3ds.h"
#include <cstdlib>
#include "ww_settings_canvas_3ds.h"
#include "item_icons_3ds.h"
#include "ww_ui_shortcuts_3ds.h"
#include "ww_ui_lifecycle_3ds.h"
#include "ww_ui_fade_3ds.h"
namespace n3ds_panel 
{
    inline constexpr int kGpuRows = 216;
    inline constexpr n3ds_ui::Rect kFpsCap24
    {
        8, 186, 64, 24
    } ;
    inline constexpr n3ds_ui::Rect kFpsCap30
    {
        76, 186, 64, 24
    } ;
    inline constexpr n3ds_ui::Rect kFpsCap60
    {
        144, 186, 64, 24
    } ;
    inline std::atomic < uint8_t > fpsCap
    {
        30
    } ;
    inline std::atomic < bool > mapBottom
    {
        false
    } ;
    inline void setFpsCap(int v)
    {
        fpsCap.store(uint8_t(v == 24?24:v == 60?60:30),
std::memory_order_relaxed);
        
    }
    inline const char * itemName(uint8_t id)
    {
        switch(id)
        {
            case 0x20:return "Telescope";
            case 0x21:return "Tingle Tuner";
            case 0x22:return "Wind Waker";
            case 0x23:return "Picto Box";
            case 0x24:return "Spoils Bag";
            case 0x25:return "Grappling Hook";
            case 0x26:return "Deluxe Picto Box";
            case 0x27:return "Bow";
            case 0x28:return "Power Bracelets";
            case 0x29:return "Iron Boots";
            case 0x2A:return "Magic Armor";
            case 0x2C:return "Bait Bag";
            case 0x2D:return "Boomerang";
            case 0x2F:return "Hookshot";
            case 0x30:return "Delivery Bag";
            case 0x31:return "Bombs";
            case 0x33:return "Skull Hammer";
            case 0x34:return "Deku Leaf";
            case 0x35:return "Fire & Ice Arrows";
            case 0x36:return "Light Arrows";
            case 0x50:return "Empty Bottle";
            case 0x51:return "Red Potion";
            case 0x52:return "Green Potion";
            case 0x53:return "Blue Potion";
            case 0x54:return "Half Soup";
            case 0x55:return "Elixir Soup";
            case 0x56:return "Water";
            case 0x57:return "Fairy";
            case 0x58:return "Forest Firefly";
            case 0x59:return "Forest Water";
            case 0x82:case 0x84:case 0x85:case 0x86:case 0x87:case 0x88:return "All-Purpose Bait";
            case 0x83:return "Hyoi Pear";
            case 0x45:return "Skull Necklace";
            case 0x46:return "Boko Baba Seed";
            case 0x47:return "Golden Feather";
            case 0x48:return "Knight's Crest";
            case 0x49:return "Red Jelly";
            case 0x4A:return "Green Jelly";
            case 0x4B:return "Blue Jelly";
            case 0x8C:return "Town Flower";
            case 0x8D:return "Sea Flower";
            case 0x8E:return "Exotic Flower";
            case 0x8F:return "Hero's Flag";
            case 0x90:return "Big Catch Flag";
            case 0x91:return "Big Sale Flag";
            case 0x92:return "Pinwheel";
            case 0x93:return "Sickle Moon Flag";
            case 0x94:return "Skull Tower Idol";
            case 0x95:return "Fountain Idol";
            case 0x96:return "Postman Statue";
            case 0x97:return "Shop Guru Statue";
            case 0x98:return "Father's Letter";
            case 0x99:return "Note to Mom";
            case 0x9A:return "Maggie's Letter";
            case 0x9B:return "Moblin's Letter";
            case 0x9C:return "Cabana Deed";
            case 0x9D:return "Complimentary ID";
            case 0x9E:return "Fill-Up Coupon";
            default:return "Item";
            
        }
        
    }
    struct State 
    {
        uint32_t presentation = 0;
        uint32_t items = 0x00ffffff;
        int pressedSlot = - 1;
        bool fps = false;
        char fpsText[16] = 
        {
            
        } ;
        bool mapOwned = false, mapBottom = false;
        bool settingsPressed = false;
        uint8_t heading = 0;
        uint8_t fpsCap = 30;
        bool inPlay = false;
        bool operator == (const State& o)const
        {
            return presentation == o.presentation 
&& items == o.items 
&& pressedSlot == o.pressedSlot 
&& fps == o.fps 
&& 
            std::strcmp(fpsText,
o.fpsText) == 0 
&& mapOwned == o.mapOwned 
&& mapBottom == o.mapBottom 
&& settingsPressed == o.settingsPressed 
&& heading == o.heading 
&& fpsCap == o.fpsCap 
&& inPlay == o.inPlay;
            
        }
        bool operator != (const State& o)const
        {
            return !( * this == o);
            
        }
        
    } ;
    inline unsigned mix(unsigned a,
unsigned b,
unsigned t)
    {
        auto ch = [&](int s)
        {
            const unsigned x = (a >> s)&255, y = (b >> s)&255;
            return ((x * (255 - t) + y * t + 127) / 255) << s;
            
        } ;
        return ch(16)|ch(8)|ch(0);
        
    }
    inline constexpr int kCapMid = 7;
    inline int inkWidth(const ww_screen::Canvas& c,
const char * s)
    {
        const int w = c.width(s);
        return w > 0?w - 1:0;
        
    }
    inline void textMid(ww_screen::Canvas& c,
int x,
int cy,
const char * s,
unsigned col,
int maxw = 320)
    {
        c.text(x,
cy - kCapMid,
s,
col,
maxw);
        
    }
    inline void centerMid(ww_screen::Canvas& c,
int x,
int cy,
int w,
const char * s,
unsigned col)
    {
        c.text(x + (w - inkWidth(c,
s)) / 2,
cy - kCapMid,
s,
col,
w);
        
    }
    inline unsigned readPx(const ww_screen::Canvas& c,
int x,
int y)
    {
        const uint16_t p = c.pixels[x * 240 + 239 - y];
        return ((((p >> 11) * 255 + 15) / 31) << 16)|(((((p >> 5)&63) * 255 + 31) / 63) << 8)|(((p&31) * 255 + 15) / 31);
        
    }
    inline void blendPx(ww_screen::Canvas& c,
int x,
int y,
unsigned col,
unsigned a)
    {
        if(x < 0 || x >= 320 || y < 0 || y >= 240 || !a)return;
        c.put(x,
y,
a >= 255?col:mix(readPx(c,
x,
y),
col,
a));
        
    }
    inline void aaDisk(ww_screen::Canvas& c,
float cx,
float cy,
float r,
unsigned fill,
unsigned edge)
    {
        const int x0 = int(cx - r - 2), x1 = int(cx + r + 2), y0 = int(cy - r - 2), y1 = int(cy + r + 2);
        for (int y=y0;y<=y1;++y)for (int x=x0;x<=x1;++x)
        {
            const float dx = x + 0.5f - cx, dy = y + 0.5f - cy;
            float d = dx * dx + dy * dy;
            float q = d > 0?d:0;
            float sq = q > 0?q * 0.5f + 0.5f:0;
            if(q > 0)
            {
                sq = 0.5f * (sq + q / sq);
                sq = 0.5f * (sq + q / sq);
                sq = 0.5f * (sq + q / sq);
                sq = 0.5f * (sq + q / sq);
                
            }
            const float outside = sq - r;
            const float cover = outside <= - 0.5f?1.f:outside >= 0.5f?0.f:0.5f - outside;
            if(cover <= 0)continue;
            const float ringIn = sq - (r - 1.5f);
            const float ringCover = ringIn <= - 0.5f?0.f:ringIn >= 0.5f?1.f:ringIn + 0.5f;
            const unsigned col = mix(fill,
edge,
unsigned(ringCover * 255.f + 0.5f));
            blendPx(c,
x,
y,
col,
unsigned(cover * 255.f + 0.5f));
            
        }
        
    }
    inline void paintCompass(ww_screen::Canvas& c,
int cx,
int cy,
uint8_t step)
    {
        constexpr int n = kCompassN;
        const float ang = step * (6.28318530718f / 256.f);
        const float co = std::cos(ang), si = std::sin(ang), half = (n - 1) * 0.5f;
        for (int y=0;y<n;++y)for (int x=0;x<n;++x)
        {
            const float dx = x - half, dy = y - half;
            const int sx = int(dx * co + dy * si + half + 0.5f), sy = int( - dx * si + dy * co + half + 0.5f);
            if(sx < 0 || sy < 0 || sx >= n || sy >= n)continue;
            const int i = sy * n + sx;
            const unsigned a = kCompassA[i];
            if(!a)continue;
            const uint16_t p = kCompassRgb[i];
            const unsigned col = ((((p >> 11) * 255) / 31) << 16)|(((((p >> 5)&63) * 255) / 63) << 8)|(((p&31) * 255) / 31);
            blendPx(c,
cx - n / 2 + x,
cy - n / 2 + y,
col,
a);
            
        }
        
    }
    inline void faceButton(ww_screen::Canvas& c,
int cx,
int cy,
char key,
bool enabled)
    {
        aaDisk(c,
float(cx) + 0.5f,
float(cy) + 0.5f,
8.f,
enabled?0x2b2d2f:0x45484b,
enabled?0x5a5e61:0x3a3d40);
        unsigned col = 0x8d9296;
        if(enabled)col = key == 'X'?0x5a91d0:key == 'Y'?0x4ea878:0xd5dbe0;
        static const uint8_t glyph[3][7] = 
        {
            
            {
                0b1000001, 0b0100010, 0b0010100, 0b0001000, 0b0010100, 0b0100010, 0b1000001
            }
            , 
            
            {
                0b1000001, 0b0100010, 0b0010100, 0b0001000, 0b0001000, 0b0001000, 0b0001000
            }
            , 
            
            {
                0b1111111, 0b0000010, 0b0000100, 0b0001000, 0b0010000, 0b0100000, 0b1111111
            }
            , 
            
        } ;
        const uint8_t * g = glyph[key == 'Y'?1:key == 'Z'?2:0];
        for (int j=0;j<7;++j)for (int i=0;i<7;++i)if(g[j]&(1 << (6 - i)))c.put(cx - 3 + i,
cy - 3 + j,
col);
        
    }
    inline void roundRect(ww_screen::Canvas& c,
int x,
int y,
int w,
int h,
int r,
unsigned fill,
unsigned edge)
    {
        for (int j=0;j<h;++j)for (int i=0;i<w;++i)
        {
            const int cxp = i < r?r:i >= w - r?w - r - 1:i, cyp = j < r?r:j >= h - r?h - r - 1:j;
            float a = 1.f, inner = 1.f;
            if((i < r || i >= w - r) 
&& (j < r || j >= h - r))
            {
                const float dx = float(i - cxp), dy = float(j - cyp);
                const float d2 = dx * dx + dy * dy;
                float q = d2, sq = q > 0?q * 0.5f + 0.5f:0;
                if(q > 0)for (int k=0;k<4;++k)sq = 0.5f * (sq + q / sq);
                const float o = sq - (r - 0.5f);
                a = o <= - 0.5f?1.f:o >= 0.5f?0.f:0.5f - o;
                const float oi = sq - (r - 1.5f);
                inner = oi <= - 0.5f?1.f:oi >= 0.5f?0.f:0.5f - oi;
                
            } else if(i == 0 || j == 0 || i == w - 1 || j == h - 1)inner = 0.f;
            if(a <= 0)continue;
            blendPx(c,
x + i,
y + j,
mix(edge,
fill,
unsigned(inner * 255.f + 0.5f)),
unsigned(a * 255.f + 0.5f));
            
        }
        
    }
    inline int wrap2(const ww_screen::Canvas& c,
const char * s,
int width,
char out[2][40])
    {
        out[0][0] = out[1][0] = 0;
        int line = 0;
        for (const char* p=s;*p
&&line<2;)
        {
            while( * p == ' ')++p;
            const char * e = p;
            while( * e 
&& * e != ' ')++e;
            char next[40];
            std::snprintf(next,
sizeof next,
"%s%s%.*s",
out[line],
out[line][0]?" ":"",
int(e - p),
p);
            if(inkWidth(c,
next) <= width || !out[line][0])
            {
                std::snprintf(out[line],
40,
"%s",
next);
                p = e;
                
            }
            else ++line;
            
        }
        return out[1][0]?2:out[0][0]?1:0;
        
    }
    inline void line(ww_screen::Canvas& c,
int x0,
int y0,
int x1,
int y1,
unsigned col)
    {
        int dx = std::abs(x1 - x0), sx = x0 < x1?1: - 1, dy = - std::abs(y1 - y0), sy = y0 < y1?1: - 1, err = dx + dy;
        for (;;)
        {
            c.put(x0,
y0,
col);
            if(x0 == x1 
&& y0 == y1)break;
            const int e2 = 2 * err;
            if(e2 >= dy)
            {
                err += dy;
                x0 += sx;
                
            }
            if(e2 <= dx)
            {
                err += dx;
                y0 += sy;
                
            }
            
        }
        
    }
    inline void button(ww_screen::Canvas& c,
n3ds_ui::Rect r,
bool enabled,
bool pressed)
    {
        const unsigned fill = pressed?0xc9893a:enabled?0xf4e2b0:0x6e6248, edge = pressed?0xfff1c4:enabled?0x8a5a22:0x3e3428;
        roundRect(c,
r.x,
r.y,
r.w,
r.h,
5,
fill,
edge);
        if(!pressed)c.rect(r.x + 5,
r.y + r.h - 3,
r.w - 10,
1,
mix(fill,
0x3a2712,
50));
        
    }
    inline void glyph(ww_screen::Canvas& c,
int x,
int y,
uint8_t id,
unsigned ink)
    {
        const unsigned sea = 0x1f6f86, leaf = 0x3d8f45, gold = 0xe6b14a, red = 0xc4483a;
        auto bottle = [&](unsigned liquid)
        {
            c.rect(x + 6,
y + 2,
4,
3,
ink);
            c.rect(x + 4,
y + 5,
8,
10,
ink);
            c.rect(x + 5,
y + 8,
6,
6,
liquid);
            
        } ;
        switch(id)
        {
            case 0x20:c.disk(x + 8,
y + 8,
6,
sea);
            c.disk(x + 8,
y + 8,
3,
0xd7eef2);
            c.rect(x + 12,
y + 12,
4,
2,
ink);
            break;
            case 0x21:c.disk(x + 8,
y + 5,
3,
0xf2d27a);
            c.rect(x + 6,
y + 9,
4,
6,
0x4aa35a);
            break;
            case 0x22:c.rect(x + 7,
y + 2,
2,
12,
ink);
            c.rect(x + 4,
y + 3,
7,
4,
gold);
            break;
            case 0x23:case 0x26:c.rect(x + 2,
y + 4,
12,
8,
ink);
            c.disk(x + 8,
y + 8,
3,
sea);
            c.rect(x + 5,
y + 2,
4,
2,
ink);
            break;
            case 0x24:case 0x2C:case 0x30:c.rect(x + 3,
y + 6,
10,
8,
0x8a5a22);
            c.rect(x + 5,
y + 3,
6,
3,
gold);
            break;
            case 0x25:case 0x2F:line(c,
x + 3,
y + 13,
x + 12,
y + 3,
ink);
            c.rect(x + 11,
y + 2,
3,
3,
gold);
            break;
            case 0x27:case 0x35:case 0x36:line(c,
x + 4,
y + 13,
x + 12,
y + 3,
ink);
            line(c,
x + 4,
y + 13,
x + 3,
y + 4,
ink);
            line(c,
x + 3,
y + 4,
x + 10,
y + 5,
ink);
            break;
            case 0x28:c.disk(x + 5,
y + 8,
3,
gold);
            c.disk(x + 11,
y + 8,
3,
gold);
            break;
            case 0x29:c.rect(x + 2,
y + 8,
4,
6,
ink);
            c.rect(x + 9,
y + 8,
4,
6,
ink);
            break;
            case 0x2A:c.rect(x + 3,
y + 4,
10,
9,
0x3d6ea5);
            c.rect(x + 6,
y + 2,
4,
3,
gold);
            break;
            case 0x2D:line(c,
x + 3,
y + 5,
x + 12,
y + 8,
ink);
            line(c,
x + 12,
y + 8,
x + 3,
y + 12,
ink);
            break;
            case 0x31:c.disk(x + 8,
y + 9,
5,
0x222222);
            c.rect(x + 7,
y + 2,
2,
4,
gold);
            break;
            case 0x33:c.rect(x + 3,
y + 3,
10,
4,
0x8d8d96);
            c.rect(x + 7,
y + 7,
2,
8,
0x8a5a22);
            break;
            case 0x34:c.disk(x + 8,
y + 7,
5,
leaf);
            c.rect(x + 7,
y + 11,
2,
4,
0x6b4a28);
            break;
            case 0x50:bottle(0xd7eef2);
            break;
            case 0x51:bottle(red);
            break;
            case 0x52:bottle(leaf);
            break;
            case 0x53:bottle(0x3d6ea5);
            break;
            case 0x54:case 0x55:bottle(0xe6b14a);
            break;
            case 0x56:case 0x59:bottle(0x7ec8e0);
            break;
            case 0x57:bottle(0xf2d27a);
            c.disk(x + 8,
y + 10,
2,
0xfff4cf);
            break;
            case 0x58:bottle(0xc8e07a);
            break;
            case 0x83:c.disk(x + 8,
y + 8,
5,
0xc6d84a);
            break;
            case 0x82:case 0x84:case 0x85:case 0x86:case 0x87:case 0x88:c.disk(x + 8,
y + 8,
4,
0xc9893a);
            break;
            default:c.rect(x + 3,
y + 4,
10,
9,
0xf7f1df);
            c.frame(x + 3,
y + 4,
10,
9,
ink);
            break;
            
        }
        
    }
    inline void composeColumn(ww_screen::Canvas& c,
const State& s)
    {
        using namespace n3ds_ui;
        const Presentation pr
        {
            s.presentation
        } ;
        c.rect(kGpuRows,
0,
320 - kGpuRows,
240,
0x5c371c);
        c.rect(kGpuRows + 3,
0,
320 - kGpuRows - 3,
240,
0x123f4c);
        c.rect(kGpuRows,
0,
3,
240,
0x8a5a22);
        static const char * keys[3] = 
        {
            "X", "Y", "Z"
        } ;
        for (int i=0;i<5;++i)
        {
            if(!targets[i].action)continue;
            const Rect r = targets[i].rect;
            uint8_t id = 255;
            if(i < 3)id = uint8_t(s.items >> (8 * i));
            const bool held = i < 3 
&& id != 255;
            const bool canUse = i >= 3?s.inPlay:(held 
&& s.inPlay 
&& pr.blocked() == 0);
            const bool pressed = canUse 
&& (s.pressedSlot == i || (i == 4 
&& s.mapBottom));
            button(c,
r,
canUse,
pressed);
            const int dy = pressed?1:0, cy = r.y + r.h / 2 + dy;
            const unsigned ink = canUse?0x3a2712:0x7a6a52;
            if(i < 3)
            {
                const int box = 24, ix = r.x + 4, iy = r.y + 4 + dy;
                const bool photo = held 
&& n3ds_icons::paint(c,
ix,
iy,
box,
id,
canUse);
                int tx, tw;
                if(photo)
                {
                    faceButton(c,
ix + box / 2,
iy + box + 9,
keys[i][0],
canUse);
                    tx = r.x + box + 6;
                    tw = r.x + r.w - 2 - tx;
                    
                } else if(!held)
                {
                    faceButton(c,
r.x + 16,
cy,
keys[i][0],
false);
                    continue;
                    
                } else
                {
                    c.rect(r.x + 4,
r.y + 6 + dy,
16,
16,
canUse?0xf7f1df:0xcbb992);
                    glyph(c,
r.x + 4,
r.y + 6 + dy,
id,
ink);
                    faceButton(c,
r.x + 12,
r.y + r.h - 12 + dy,
keys[i][0],
canUse);
                    tx = r.x + 24;
                    tw = r.x + r.w - 4 - tx;
                    
                }
                char lines[2][40];
                const int n = wrap2(c,
itemName(id),
tw,
lines);
                if(n == 1)textMid(c,
tx,
cy - 1,
lines[0],
ink,
tw);
                else 
                {
                    textMid(c,
tx,
cy - 8,
lines[0],
ink,
tw);
                    textMid(c,
tx,
cy + 7,
lines[1],
ink,
tw);
                    
                }
                
            } else
            {
                centerMid(c,
r.x,
cy - 1,
r.w,
i == 3?"Menu":"Map",
ink);
                
            }
            
        }
        
    }
    inline void paintFps(ww_screen::Canvas& c,
const State& s)
    {
        const char * t = s.fpsText[0]?s.fpsText:"0.0 FPS";
        const int w = inkWidth(c,
t);
        c.rect(0,
0,
w + 8,
18,
0x123f4c);
        c.text(4,
0,
t,
0xffee00,
w + 2);
        
    }
    inline void composeLeft(ww_screen::Canvas& c,
const uint16_t * ,
const State& s)
    {
        for (int y=0;y<240;++y)c.rect(0,
y,
kGpuRows,
1,
0x1b72ad);
        c.rect(6,
22,
kGpuRows - 12,
194,
0xf4e2b0);
        c.rect(48,
34,
kGpuRows - 96,
2,
0xc9893a);
        const int cx = kGpuRows / 2, cy = 96;
        paintCompass(c,
cx,
cy,
s.heading);
        centerMid(c,
cx - 10,
cy - 40,
20,
"N",
0xb42318);
        centerMid(c,
cx + 36,
cy,
16,
"E",
0x3a2712);
        centerMid(c,
cx - 10,
cy + 40,
20,
"S",
0x3a2712);
        centerMid(c,
cx - 52,
cy,
16,
"W",
0x3a2712);
        c.rect(28,
170,
kGpuRows - 56,
1,
0xc9893a);
        centerMid(c,
10,
172,
kGpuRows - 20,
"FPS Cap",
0x3a2712);
        const bool cap24 = s.fpsCap == 24, cap60 = s.fpsCap == 60;
        button(c,
kFpsCap24,
true,
cap24);
        button(c,
kFpsCap30,
true,
!cap24 
&& !cap60);
        button(c,
kFpsCap60,
true,
cap60);
        centerMid(c,
kFpsCap24.x,
kFpsCap24.y + kFpsCap24.h / 2 - 1 + (cap24?1:0),
kFpsCap24.w,
"24",
0x3a2712);
        centerMid(c,
kFpsCap30.x,
kFpsCap30.y + kFpsCap30.h / 2 - 1 + (!cap24 
&& !cap60?1:0),
kFpsCap30.w,
"30",
0x3a2712);
        centerMid(c,
kFpsCap60.x,
kFpsCap60.y + kFpsCap60.h / 2 - 1 + (cap60?1:0),
kFpsCap60.w,
"60",
0x3a2712);
        
    }
    inline void compose(uint16_t * out,
const uint16_t * cloth,
const State& s)
    {
        ww_screen::Canvas c
        {
            out
        } ;
        if(!s.mapOwned)
        {
            static uint16_t cache[kGpuRows * 240];
            static int key = - 1;
            const int k = (s.fpsCap << 9)|(int(s.heading) << 1);
            if(key != k)
            {
                ww_screen::Canvas cc
                {
                    cache
                } ;
                composeLeft(cc,
cloth,
s);
                key = k;
                
            }
            std::memcpy(out,
cache,
sizeof cache);
            
        }
        composeColumn(c,
s);
        const uint8_t alpha = n3ds_ui::Presentation
        {
            s.presentation
        }
        .alpha();
        if(alpha != 255 
&& alpha != 0)n3ds_ui::fade565(out,
out,
320 * 240,
uint8_t(64 + alpha * 191 / 255));
        paintFps(c,
s);
        
    }
    
}
