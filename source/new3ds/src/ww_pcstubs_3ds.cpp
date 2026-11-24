#include "pc/pc_internal.h"
#include "pc/pc_hd_textures.h"
#include "pc/pc_update.h"
#include "pc/game_hooks.h"
#include <dolphin/pad.h>
#include <dolphin/gx.h>
#include <dolphin/os.h>
#include "m_Do/m_Do_graphic.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <vector>
namespace pc 
{
    bool captureFrame(unsigned int,
FrameSink,
void * ) 
    {
        return false;
        
    }
    void saveFramePng(unsigned int,
const std::vector < uint8_t > &,
uint32_t,
uint32_t) 
    {
        
    }
    void captureFrameEnd();
    const CaptureStats& captureStats() 
    {
        static CaptureStats s;
        return s;
        
    }
    bool menuOpen() 
    {
        return false;
        
    }
    void menuSmokeState(int * tab,
int * row,
int * captureAction) 
    {
        if (tab) * tab = 0;
        if (row) * row = 0;
        if (captureAction) * captureAction = 0;
        
    }
    void overlayFrame(uint64_t,
bool) 
    {
        
    }
    bool menuSmokeWantsRunCard() 
    {
        return false;
        
    }
    void menuInit() 
    {
        
    }
    void menuFrameEnd(unsigned int) 
    {
        
    }
    void menuFrame() 
    {
        
    }
    bool menuBuiltinSetting(const char * ) 
    {
        return false;
        
    }
    bool loadShots() 
    {
        return false;
        
    }
    void shotSplitEnd(unsigned int) 
    {
        
    }
    void shotFrameEnd(unsigned int) 
    {
        
    }
    void shotProbe(unsigned int,
std::function < void(const std::vector < uint8_t > &,
uint32_t,
uint32_t) > ) 
    {
        
    }
    void precompileOverlay() 
    {
        
    }
    void precompileLoadingScreen() 
    {
        
    }
    void precompileInit(const char * ) 
    {
        
    }
    void precompileFrame(unsigned int) 
    {
        
    }
    
}
extern "C" uint32_t n3dsRenderedFrames(void);
extern "C" int n3dsReadCopyRGBA(uint32_t addr,
std::vector < uint8_t > * out,
int * outW,
int * outH);
extern "C" void n3dsLog(const char * fmt,
...);
namespace 
{
    enum class RbState 
    {
        Idle, Requested, Done 
    } ;
    RbState sCapState = RbState::Idle;
    void * sCapDest = nullptr;
    u32 sCapW = 0, sCapH = 0, sCapReq = 0;
    int sCapFmt = 0;
    alignas(32) u8 sPeekKey[32];
    void ( * sPeekDone)() = nullptr;
    u16 sPeekLeft = 0, sPeekTop = 0, sPeekWidth = 0, sPeekHeight = 0;
    u32 sPeekReq = 0;
    bool sPeekValid = false;
    std::vector < uint8_t > sPeek;
    int sPeekSrcW = 0, sPeekSrcH = 0;
    void writeCapture(const std::vector < uint8_t > * rgba,
int srcW,
int srcH) 
    {
        uint8_t * dest = static_cast < uint8_t * > (sCapDest);
        for (u32 y = 0; y < sCapH; y++) 
        {
            for (u32 x = 0; x < sCapW; x++) 
            {
                uint32_t rgb[3] = 
                {
                    128, 128, 128
                } ;
                if (rgba) 
                {
                    const uint32_t x0 = x * srcW / sCapW, x1 = std::max < uint32_t > (x0 + 1,
(x + 1) * srcW / sCapW);
                    const uint32_t y0 = y * srcH / sCapH, y1 = std::max < uint32_t > (y0 + 1,
(y + 1) * srcH / sCapH);
                    uint32_t sum[3] = 
                    {
                        
                    } ;
                    for (uint32_t sy = y0; sy < y1; sy++)
                    for (uint32_t sx = x0; sx < x1; sx++) 
                    {
                        const uint8_t * p = rgba->data() + (size_t(sy) * srcW + sx) * 4;
                        sum[0] += p[0];
                        sum[1] += p[1];
                        sum[2] += p[2];
                        
                    }
                    const uint32_t n = (x1 - x0) * (y1 - y0);
                    for (int c = 0; c < 3; c++) rgb[c] = (sum[c] + n / 2) / n;
                    
                }
                if (sCapFmt == GX_TF_I8) 
                {
                    const size_t off = (size_t(y / 4) * (sCapW / 8) + x / 8) * 32 + (y % 4) * 8 + x % 8;
                    dest[off] = uint8_t(std::min < uint32_t > (235,
std::max < uint32_t > (16,
rgb[0])));
                    
                } else 
                {
                    const size_t off = (size_t(y / 4) * (sCapW / 4) + x / 4) * 32 + (y % 4) * 8 + (x % 4) * 2;
                    const u16 t = u16((rgb[0] & 0xF8) << 8 | (rgb[1] & 0xFC) << 3 | rgb[2] >> 3);
                    std::memcpy(dest + off,
&t,
2);
                    
                }
                
            }
            
        }
        
    }
    
}
void pc_efb_peek_request(u16 left,
u16 top,
u16 width,
u16 height,
void ( * done)()) 
{
    if (sPeekDone) 
    {
        n3dsLog("[readback] efb-peek: a request is already pending; dropped\n");
        return;
        
    }
    sPeekLeft = left;
    sPeekTop = top;
    sPeekWidth = width;
    sPeekHeight = height;
    sPeekDone = done;
    sPeekReq = n3dsRenderedFrames();
    GXSetTexCopySrc(left,
top,
width,
height);
    GXSetTexCopyDst(width,
height,
GX_TF_RGBA8,
GX_FALSE);
    GXCopyTex(sPeekKey,
GX_FALSE);
    GXPixModeSync();
    
}
u32 pc_efb_peek_argb(u16 x,
u16 y) 
{
    if (!sPeekValid || x < sPeekLeft || y < sPeekTop || x >= sPeekLeft + sPeekWidth || y >= sPeekTop + sPeekHeight) return 0xFFFFFFFFu;
    const uint32_t sx = (uint32_t(x - sPeekLeft) * 2 + 1) * sPeekSrcW / (2u * sPeekWidth);
    const uint32_t sy = (uint32_t(y - sPeekTop) * 2 + 1) * sPeekSrcH / (2u * sPeekHeight);
    const uint8_t * p = sPeek.data() + (size_t(sy) * sPeekSrcW + sx) * 4;
    return u32(p[3]) << 24 | u32(p[0]) << 16 | u32(p[1]) << 8 | p[2];
    
}
bool pc_gph_capture_ready(void * dest,
u32 width,
u32 height,
int format) 
{
    if (sCapState == RbState::Done 
&& sCapDest == dest) 
    {
        sCapState = RbState::Idle;
        return true;
        
    }
    if (format != GX_TF_I8 
&& format != GX_TF_RGB565) 
    {
        n3dsLog("[readback] capture: format %d is not read back\n",
format);
        return true;
        
    }
    if (sCapState == RbState::Requested 
&& sCapDest == dest) return false;
    sCapState = RbState::Requested;
    sCapDest = dest;
    sCapW = width;
    sCapH = height;
    sCapFmt = format;
    sCapReq = n3dsRenderedFrames();
    return false;
    
}
namespace pc 
{
    void captureFrameEnd() 
    {
        const uint32_t now = n3dsRenderedFrames();
        if (sCapState == RbState::Requested 
&& now >= sCapReq + 3) 
        {
            std::vector < uint8_t > rgba;
            int w = 0, h = 0;
            if (n3dsReadCopyRGBA(uint32_t(uintptr_t(sCapDest)),
&rgba,
&w,
&h)) writeCapture(&rgba,
w,
h);
            else 
            {
                n3dsLog("[readback] capture: no copy, the photo is grey\n");
                writeCapture(nullptr,
0,
0);
                
            }
            sCapState = RbState::Done;
            
        }
        if (sPeekDone 
&& now >= sPeekReq + 3) 
        {
            void ( * done)() = sPeekDone;
            sPeekDone = nullptr;
            sPeekValid = n3dsReadCopyRGBA(uint32_t(uintptr_t(sPeekKey)),
&sPeek,
&sPeekSrcW,
&sPeekSrcH) != 0;
            done();
            sPeekValid = false;
            
        }
        
        {
            static int testAt = - 2;
            static int n = 0;
            if (testAt == - 2) 
            {
                const char * v = getenv("N3DS_RB_TEST");
                testAt = v ? atoi(v) : - 1;
                
            }
            if (testAt > 0 
&& ++n == testAt) 
            {
                std::vector < uint8_t > rgba;
                int w = 0, h = 0;
                const char * a = getenv("N3DS_RB_ADDR");
                const uint32_t addr = a ? uint32_t(strtoul(a,
nullptr,
16)) : uint32_t(uintptr_t(mDoGph_gInf_c::getFrameBufferTex()));
                if (n3dsReadCopyRGBA(addr,
&rgba,
&w,
&h)) 
                {
                    if (FILE * f = fopen("sdmc:/ww3ds/rbtest.rgba",
"wb")) 
                    {
                        uint32_t hd[2] = 
                        {
                            uint32_t(w), uint32_t(h)
                        } ;
                        fwrite(hd,
4,
2,
f);
                        fwrite(rgba.data(),
1,
rgba.size(),
f);
                        fclose(f);
                        
                    }
                    
                }
                
            }
            
        }
        
    }
    
}
void pc_snap_judged(int,
const dSnap_RegistObjElm * ,
int) 
{
    
}
extern "C" 
{
    void pc_hd_textures_init(const char * ) 
    {
        
    }
    void pc_hd_textures_frame_end(unsigned int) 
    {
        
    }
    void pc_update_boot(void) 
    {
        
    }
    void pcWwrpCaptureInit(void) 
    {
        
    }
    void aurora_draw_census_request(const char * ) 
    {
        
    }
    int pc_debug_give(const char * const * ,
int,
char * done,
size_t size) 
    {
        if (done 
&& size) done[0] = 0;
        return 0;
        
    }
    PADButtonMapping * PADGetButtonMappings(u32,
u32 * count) 
    {
        if (count) * count = 0;
        return nullptr;
        
    }
    PADAxisMapping * PADGetAxisMappings(u32,
u32 * count) 
    {
        if (count) * count = 0;
        return nullptr;
        
    }
    void PADSetButtonMapping(u32,
PADButtonMapping) 
    {
        
    }
    void PADSetAxisMapping(u32,
PADAxisMapping) 
    {
        
    }
    void PADRestoreDefaultMapping(u32) 
    {
        
    }
    struct SDL_Gamepad * PADGetSDLGamepadForIndex(u32) 
    {
        return nullptr;
        
    }
    
}
#include "JSystem/JKernel/JKRExpHeap.h"
#include "m_Do/m_Do_ext.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRAramHeap.h"
extern "C" void n3dsGameHeapReport(char * out,
unsigned size) 
{
    auto kb = [](JKRHeap * h,
bool total) ->long 
    {
        return h ? long((total ? h->getTotalFreeSize() : h->getFreeSize()) / 1024) : - 1;
        
    } ;
    auto sz = [](JKRHeap * h) ->long 
    {
        return h ? long(h->getHeapSize() / 1024) : - 1;
        
    } ;
    snprintf(out,
size,
"root free %ld/%ld KB, game %ld/%ld, zelda %ld/%ld, archive %ld/%ld, command %ld/%ld",
    kb(JKRHeap::getRootHeap(),
true),
sz(JKRHeap::getRootHeap()),
kb(mDoExt_getGameHeap(),
true),
sz(mDoExt_getGameHeap()),
    kb(mDoExt_getZeldaHeap(),
true),
sz(mDoExt_getZeldaHeap()),
kb(mDoExt_getArchiveHeap(),
true),
sz(mDoExt_getArchiveHeap()),
    kb(mDoExt_getCommandHeap(),
true),
sz(mDoExt_getCommandHeap()));
    if (JKRAram::getManager() 
&& JKRAram::getAramHeap()) 
    {
        const size_t n = strlen(out);
        snprintf(out + n,
size - n,
", aram free %ld KB (largest %ld)",
long(JKRAram::getAramHeap()->getTotalFreeSize() / 1024),
long(JKRAram::getAramHeap()->getFreeSize() / 1024));
        
    }
    
}
#include "m_Do/m_Do_MemCard.h"
#include <dolphin/os.h>
#include <thread>
#include <chrono>
extern "C" int n3dsMemcardQuiesce(unsigned timeoutMs)
{
    if(!OSGetCurrentThread())return 1;
    const OSTime start = OSGetTime();
    for (;;)
    {
        if(OSTryLockMutex(&g_mDoMemCd_control.mMutex))
        {
            if(g_mDoMemCd_control.mCommand == mDoMemCd_Ctrl_c::CARD_NO_COMMAND)return 1;
            OSUnlockMutex(&g_mDoMemCd_control.mMutex);
            
        }
        if(OSGetTime() - start >= OSMillisecondsToTicks(timeoutMs))return 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        
    }
    
}
