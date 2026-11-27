#pragma once
#include <algorithm>
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
    // Warm targets: percent only reaches 100 after these are met (or timeout).
    inline constexpr int kNeededGoodFrames = 36;
    inline constexpr int kNeededActiveFrames = 24;
    inline constexpr size_t kMinShaders = 8;
    // 10 FPS floor: a present interval of 100 ms counts as a good frame.
    inline constexpr double kOkFrameMs = 100.0;
    inline constexpr uint64_t kGiveUpSeconds = 90;
    // Do not repaint the cover on every single percent tick.
    inline constexpr int kPresentPercentStep = 2;
    inline constexpr uint64_t kPresentMinTicks = SYSCLOCK_ARM11 / 2;
    struct State 
    {
        bool covering = false;
        t2p::Renderer3ds * drawTarget = nullptr;
        uint64_t openedTick = 0;
        uint64_t lastPresentTick = 0;
        int percent = 0;
        int shownPercent = -1;
        int goodFrames = 0;
        int activeFrames = 0;
        int frameIndex = 0;
        size_t peakShaders = 0;
        size_t peakTextures = 0;
        
    } ;
    inline State& state() 
    {
        static State s;
        return s;
        
    }
    inline bool covering() 
    {
        return state().covering || coveringGate();
        
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
                auto mix = [&](uint8_t dst,
unsigned src) 
                {
                    return uint8_t((src * alpha + dst * (255 - alpha) + 127) / 255);
                    
                } ;
                if (bpp == 2) 
                {
                    const uint16_t old = uint16_t(p[0] | (p[1] << 8));
                    const unsigned or_ = ((old >> 11) & 31) * 255 / 31, og = ((old >> 5) & 63) * 255 / 63, ob = (old & 31) * 255 / 31;
                    const unsigned nr = mix(uint8_t(or_),
(colour >> 16) & 255), ng = mix(uint8_t(og),
(colour >> 8) & 255), nb = mix(uint8_t(ob),
colour & 255);
                    const uint16_t px16 = uint16_t(((nr >> 3) << 11) | ((ng >> 2) << 5) | (nb >> 3));
                    p[0] = uint8_t(px16);
                    p[1] = uint8_t(px16 >> 8);
                    
                } else 
                {
                    const int cr = bgr ? 2 : 0, cb = bgr ? 0 : 2;
                    auto mix8 = [&](uint8_t dst,
unsigned src) 
                    {
                        return uint8_t((src * alpha + dst * (255 - alpha) + 127) / 255);
                        
                    } ;
                    p[cr] = mix8(p[cr],
(colour >> 16) & 255);
                    p[1] = mix8(p[1],
(colour >> 8) & 255);
                    p[cb] = mix8(p[cb],
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
int percent) 
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
        const int clamped = percent < 0 ? 0 : percent > 100 ? 100 : percent;
        for (int y = barY; y < barY + 8 
&& y < stride; ++y)
        for (int x = barX; x < barX + barW 
&& x < wide; ++x) 
        {
            const bool on = (x - barX) * 100 < clamped * barW;
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
        char pct[16];
        std::snprintf(pct,
sizeof pct,
"%d%%",
clamped);
        glyph(fb,
stride,
bpp,
bgr,
wide,
(wide - textWidth(pct)) / 2,
barY + 14,
pct,
0xf4ffe8);
        const char * thanks = "thx for playing";
        glyph(fb,
stride,
bpp,
bgr,
wide,
(wide - textWidth(thanks)) / 2,
barY + 34,
thanks,
0xf4ffe8);
        GSPGPU_FlushDataCache(fb,
u32(stride) * wide * bpp);
        
    }
    // Cover paint must not WaitDone/swap every frame: that serializes core-2
    // rendering (sub-1 FPS) and races GX display transfers (FAR 0 writes).
    inline bool present(int percent,
bool includeTop,
bool forceGpuIdle) 
    {
        if (!forceGpuIdle 
&& n3dsRenderThreadBusy()) return false;
        if (forceGpuIdle) C3Di_RenderQueueWaitDone();
        char line[64];
        std::snprintf(line,
sizeof line,
"Building graphics cache");
        if (includeTop) 
        {
            paint(GFX_TOP,
"Preparing Wind Waker",
line,
percent);
            gfxFlushBuffers();
            gfxScreenSwapBuffers(GFX_TOP,
false);
            
        }
        paint(GFX_BOTTOM,
"Preparing Wind Waker",
line,
percent);
        gfxFlushBuffers();
        gfxScreenSwapBuffers(GFX_BOTTOM,
false);
        return true;
        
    }
    // Raw completion score from warm work. Never used to lower the shown %.
    inline int computePercent(const State& s) 
    {
        // Shader/texture discovery drives most of the bar while cold (presentMs may be huge).
        const int shaderPart = int(std::min < size_t > (s.peakShaders,
kMinShaders) * 45 / kMinShaders);
        const int discoverPart = std::min(s.goodFrames,
kNeededGoodFrames) * 30 / kNeededGoodFrames;
        const int activePart = std::min(s.activeFrames,
kNeededActiveFrames) * 20 / kNeededActiveFrames;
        int timePart = 0;
        if (s.openedTick) 
        {
            const uint64_t elapsed = svcGetSystemTick() - s.openedTick;
            const uint64_t soft = 50ull * SYSCLOCK_ARM11;
            timePart = int(std::min(elapsed,
soft) * 5 / soft);
            
        }
        int total = shaderPart + discoverPart + activePart + timePart;
        if (total > 99) total = 99;
        // Cap at 99 until discovery targets are met so 100% means ready.
        if (s.goodFrames >= kNeededGoodFrames 
&& s.activeFrames >= kNeededActiveFrames 
&& s.peakShaders >= kMinShaders) total = 100;
        return total;
        
    }
    inline void finish(bool timedOut) 
    {
        State& s = state();
        s.percent = 100;
        present(100,
true,
true);
        s.shownPercent = 100;
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
        g_covering.store(false,
std::memory_order_release);
        n3dsSetBootStage("title");
        n3dsLog("[warm] first-run loading %s (%d%% shaders=%zu good=%d active=%d)\n",
timedOut ? "gave up after 90s and opened the menu" : "ready, opening the menu",
s.percent,
s.peakShaders,
s.goodFrames,
s.activeFrames);
        
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
        g_covering.store(true,
std::memory_order_release);
        s.openedTick = svcGetSystemTick();
        s.lastPresentTick = s.openedTick;
        s.percent = 0;
        s.shownPercent = -1;
        s.goodFrames = 0;
        s.activeFrames = 0;
        s.frameIndex = 0;
        s.peakShaders = 0;
        s.peakTextures = 0;
        g_lockControls.store(true,
std::memory_order_release);
        output.setHideOutput(true);
        // First cover only: GPU is idle here. Later updates skip WaitDone.
        present(0,
true,
true);
        s.shownPercent = 0;
        n3dsSetBootStage("warm");
        n3dsLog("[warm] first launch, covering until graphics cache is ready\n");
        
    }
    inline void afterFrame(t2p::Renderer3ds& output,
double presentMs,
uint32_t draws) 
    {
        State& s = state();
        if (!s.covering) return;
        s.drawTarget = &output;
        // Keep the offscreen RT detached so FrameEnd never transfers into a
        // framebuffer we are also CPU-painting for the % bar.
        output.setHideOutput(true);
        ++s.frameIndex;
        const size_t shaders = output.shaders().size();
        const size_t textures = output.cache().stats().entries;
        const bool grewShaders = shaders > s.peakShaders;
        const bool grewTextures = textures > s.peakTextures;
        if (grewShaders) s.peakShaders = shaders;
        if (grewTextures) s.peakTextures = textures;
        if (draws >= 20) ++s.activeFrames;
        // Discovery of shaders/textures advances the bar even when presentMs is huge.
        // Also keep the classic ≤100 ms good-frame credit when frames are already fast.
        if (grewShaders || grewTextures || (draws >= 20 
&& presentMs > 0.0 
&& presentMs <= kOkFrameMs)) ++s.goodFrames;
        const int next = computePercent(s);
        if (next > s.percent) s.percent = next;
        const uint64_t now = svcGetSystemTick();
        const bool giveUp = s.openedTick 
&& now - s.openedTick > kGiveUpSeconds * SYSCLOCK_ARM11;
        if (s.percent >= 100 || giveUp) 
        {
            finish(giveUp 
&& s.percent < 100);
            return;
            
        }
        // Pace cover redraws: every few percent or ~0.5 s. Never WaitDone mid-warm.
        const bool stepped = s.percent >= s.shownPercent + kPresentPercentStep;
        const bool timed = s.lastPresentTick == 0 
|| now - s.lastPresentTick >= kPresentMinTicks;
        if (s.percent != s.shownPercent 
&& (stepped || timed) 
&& present(s.percent,
false,
false)) 
        {
            s.shownPercent = s.percent;
            s.lastPresentTick = now;
            
        }
        
    }
    
}
