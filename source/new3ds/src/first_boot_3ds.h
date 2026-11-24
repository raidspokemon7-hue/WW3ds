#pragma once
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include "first_boot_gate_3ds.h"
#include "bottom_intro_3ds.h"
#include "ww_settings_font_3ds.h"
#include "ww_n3ds.h"
#include "ww_renderer3ds.h"
extern "C" void C3Di_RenderQueueWaitDone(void);
namespace n3ds_warm 
{
    inline constexpr const char * kMarker = N3DS_ROOT "/first-run.done";
    inline constexpr int kWindow = 60;
    inline constexpr double kOkAverageMs = 75.0;
    inline constexpr double kOkWorstMs = 100.0;
    inline constexpr uint32_t kMinDraws = 20;
    inline constexpr uint64_t kGiveUpSeconds = 90;
    struct State 
    {
        bool covering = false;
        t2p::Renderer3ds * drawTarget = nullptr;
        uint64_t openedTick = 0;
        double recentMs[kWindow] = 
        {
            
        } ;
        int sampleCount = 0;
        int sampleIndex = 0;
        
    } ;
    inline State& state() 
    {
        static State s;
        return s;
        
    }
    inline bool covering() 
    {
        return state().covering;
        
    }
    inline int textWidth(const char * s) 
    {
        int n = 0;
        for (; *s; ++s) 
        {
            const unsigned c = unsigned(uint8_t( * s));
            if (c >= 32 
&& c < 127) n += kSettingsAdvance[c - 32];
            
        }
        return n;
        
    }
    inline void glyph(uint8_t * fb,
int stride,
int bpp,
bool bgr,
int wide,
int x,
int y,
const char * s,
unsigned colour) 
    {
        const int end = x + wide;
        for (; *s; ++s) 
        {
            const unsigned c = unsigned(uint8_t( * s));
            if (c < 32 || c >= 127) continue;
            const unsigned g = c - 32;
            const int advance = kSettingsAdvance[g];
            if (x + advance > end) break;
            for (int j = 0; j < 18; ++j) for (int i = 0; i < 14; ++i) 
            {
                const unsigned alpha = kSettingsGlyph[g * 14 * 18 + j * 14 + i];
                if (!alpha) continue;
                const int px = x + i, py = y + j;
                if (px < 0 || py < 0 || px >= wide || py >= stride) continue;
                uint8_t * p = fb + (size_t(px) * stride + (stride - 1 - py)) * bpp;
                if (bpp == 2) 
                {
                    const uint16_t old = uint16_t(p[0] | (p[1] << 8));
                    const unsigned r = ((old >> 11) * 255 + 15) / 31, gch = (((old >> 5) & 63) * 255 + 31) / 63, b = ((old & 31) * 255 + 15) / 31;
                    const unsigned nr = (((colour >> 16) & 255) * alpha + r * (255 - alpha) + 127) / 255;
                    const unsigned ng = (((colour >> 8) & 255) * alpha + gch * (255 - alpha) + 127) / 255;
                    const unsigned nb = ((colour & 255) * alpha + b * (255 - alpha) + 127) / 255;
                    const uint16_t out = uint16_t(((nr >> 3) << 11) | ((ng >> 2) << 5) | (nb >> 3));
                    p[0] = uint8_t(out);
                    p[1] = uint8_t(out >> 8);
                    
                } else 
                {
                    const int cr = bgr ? 2 : 0, cb = bgr ? 0 : 2;
                    auto mix = [&](uint8_t dst,
unsigned src) 
                    {
                        return uint8_t((src * alpha + dst * (255 - alpha) + 127) / 255);
                        
                    } ;
                    p[cr] = mix(p[cr],
(colour >> 16) & 255);
                    p[1] = mix(p[1],
(colour >> 8) & 255);
                    p[cb] = mix(p[cb],
colour & 255);
                    
                }
                
            }
            x += advance;
            
        }
        
    }
    inline void fill(uint8_t * fb,
int stride,
int wide,
int bpp,
bool bgr,
unsigned colour) 
    {
        const size_t n = size_t(stride) * wide;
        if (bpp == 2) 
        {
            const uint16_t px = uint16_t((((colour >> 19) & 31) << 11) | (((colour >> 10) & 63) << 5) | ((colour >> 3) & 31));
            auto * p = reinterpret_cast < uint16_t * > (fb);
            for (size_t i = 0; i < n; ++i) p[i] = px;
            
        } else 
        {
            const uint8_t bytes[4] = 
            {
                uint8_t(bgr ? colour & 255 : (colour >> 16) & 255), uint8_t((colour >> 8) & 255), uint8_t(bgr ? (colour >> 16) & 255 : colour & 255), 255
            } ;
            for (size_t i = 0; i < n; ++i) std::memcpy(fb + i * bpp,
bytes,
bpp);
            
        }
        
    }
    inline void paint(gfxScreen_t screen,
const char * title,
const char * line,
int settled) 
    {
        u16 stride = 0, wide = 0;
        auto * fb = static_cast < uint8_t * > (gfxGetFramebuffer(screen,
GFX_LEFT,
&stride,
&wide));
        if (!fb || !stride || !wide) return;
        const auto fmt = gfxGetScreenFormat(screen);
        const bool bgr = fmt == GSP_BGR8_OES;
        const int bpp = fmt == GSP_RGBA8_OES ? 4 : fmt == GSP_BGR8_OES ? 3 : 2;
        if (fmt != GSP_RGB565_OES 
&& fmt != GSP_BGR8_OES 
&& fmt != GSP_RGBA8_OES) return;
        fill(fb,
stride,
wide,
bpp,
bgr,
0x1f6b32);
        const int y0 = stride > 200 ? 70 : 62;
        glyph(fb,
stride,
bpp,
bgr,
wide,
(wide - textWidth(title)) / 2,
y0,
title,
0xf4ffe8);
        glyph(fb,
stride,
bpp,
bgr,
wide,
(wide - textWidth(line)) / 2,
y0 + 28,
line,
0xd4f0c4);
        const char * once = "This only happens the first time you play";
        glyph(fb,
stride,
bpp,
bgr,
wide,
(wide - textWidth(once)) / 2,
y0 + 52,
once,
0xe8ffe0);
        const int barW = wide > 280 ? 220 : 160;
        const int barX = (wide - barW) / 2, barY = y0 + 84;
        for (int y = barY; y < barY + 8 
&& y < stride; ++y)
        for (int x = barX; x < barX + barW 
&& x < wide; ++x) 
        {
            const bool on = (x - barX) * kWindow < settled * barW;
            uint8_t * p = fb + (size_t(x) * stride + (stride - 1 - y)) * bpp;
            const unsigned colour = on ? 0xc6ff6a : 0x145022;
            if (bpp == 2) 
            {
                const uint16_t px = uint16_t((((colour >> 19) & 31) << 11) | (((colour >> 10) & 63) << 5) | ((colour >> 3) & 31));
                p[0] = uint8_t(px);
                p[1] = uint8_t(px >> 8);
                
            } else 
            {
                p[bgr ? 2 : 0] = uint8_t((colour >> 16) & 255);
                p[1] = uint8_t((colour >> 8) & 255);
                p[bgr ? 0 : 2] = uint8_t(colour & 255);
                
            }
            
        }
        const char * thanks = "thx for playing";
        glyph(fb,
stride,
bpp,
bgr,
wide,
(wide - textWidth(thanks)) / 2,
barY + 14,
thanks,
0xf4ffe8);
        GSPGPU_FlushDataCache(fb,
u32(stride) * wide * bpp);
        
    }
    inline void present(int settled) 
    {
        // Wait out any pending GX display transfer before CPU-painting the
        // same framebuffers. Racing C3D's FrameEnd transfer caused flicker
        // and ARM11 null-destination writes on the render core.
        C3Di_RenderQueueWaitDone();
        char line[64];
        const size_t shaders = state().drawTarget ? state().drawTarget->shaders().size() : 0;
        std::snprintf(line,
sizeof line,
"Loading graphics  %zu",
shaders);
        paint(GFX_TOP,
"Preparing Wind Waker",
line,
settled);
        paint(GFX_BOTTOM,
"Preparing Wind Waker",
line,
settled);
        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
        
    }
    inline int settledCount() 
    {
        State& s = state();
        const int count = s.sampleCount < kWindow ? s.sampleCount : kWindow;
        if (!count) return 0;
        int ok = 0;
        double sum = 0, worst = 0;
        for (int i = 0; i < count; ++i) 
        {
            const double ms = s.recentMs[i];
            sum += ms;
            if (ms > worst) worst = ms;
            if (ms <= kOkWorstMs) ++ok;
            
        }
        if (count >= kWindow 
&& worst <= kOkWorstMs 
&& sum / count <= kOkAverageMs) return kWindow;
        return ok;
        
    }
    inline void finish(bool timedOut) 
    {
        State& s = state();
        if (FILE * f = std::fopen(kMarker,
"w")) 
        {
            std::fputs(timedOut ? "timeout\n" : "ok\n",
f);
            std::fclose(f);
            
        }
        if (s.drawTarget) s.drawTarget->setHideOutput(false);
        n3ds_intro::dismiss();
        g_lockControls.store(false,
std::memory_order_release);
        s.covering = false;
        n3dsSetBootStage("title");
        n3dsLog("[warm] first-run loading %s\n",
timedOut ? "gave up after 90s and opened the menu" : "settled, opening the menu");
        
    }
    inline void begin(t2p::Renderer3ds& output) 
    {
        if (FILE * hw = std::fopen(N3DS_ROOT "/hwtest.enable",
"r")) 
        {
            std::fclose(hw);
            return;
            
        }
        if (FILE * done = std::fopen(kMarker,
"r")) 
        {
            std::fclose(done);
            n3dsLog("[warm] first-run already done\n");
            return;
            
        }
        State& s = state();
        s.drawTarget = &output;
        s.covering = true;
        s.openedTick = svcGetSystemTick();
        g_lockControls.store(true,
std::memory_order_release);
        output.setHideOutput(true);
        present(0);
        n3dsSetBootStage("warm");
        n3dsLog("[warm] first launch, covering the menu until it is running smoothly\n");
        
    }
    inline void afterFrame(t2p::Renderer3ds& output,
double presentMs,
uint32_t draws) 
    {
        State& s = state();
        if (!s.covering) return;
        s.drawTarget = &output;
        if (draws >= kMinDraws 
&& presentMs > 0.0 
&& presentMs < 5000.0) 
        {
            s.recentMs[s.sampleIndex] = presentMs;
            s.sampleIndex = (s.sampleIndex + 1) % kWindow;
            if (s.sampleCount < kWindow) ++s.sampleCount;
            
        }
        const int settled = settledCount();
        const bool smooth = s.sampleCount >= kWindow 
&& settled == kWindow;
        const bool giveUp = s.openedTick 
&& svcGetSystemTick() - s.openedTick > kGiveUpSeconds * SYSCLOCK_ARM11;
        if (smooth || giveUp) finish(giveUp 
&& !smooth);
        else present(settled);
        
    }
    
}
