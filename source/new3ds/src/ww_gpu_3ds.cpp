extern "C" 
{
    unsigned g_n3dsModelDirect = 0;
    unsigned g_n3dsMaterialIdentity = 0;
    unsigned g_n3dsModelRecords = 0;
    unsigned g_n3dsMaterialRecords = 0;
    
}
#include "ww_paint_budget_3ds.h"
#include <cstdint>
#include <cstdlib>
namespace t2p 
{
    extern uint64_t ( * g_compileClock)();
    extern uint64_t g_compilePhase[8];
    
}
extern void ( * g_r3dsTrace)(const char * ,
int);
extern int g_hwTest;
extern bool g_shaderNoDph;
extern int g_shaderOutTest;
#include <malloc.h>
#include <atomic>
static uint64_t g_recStateProf[8];
static std::atomic < int > g_foliageAb
{
     - 1
} ;
static std::atomic < int > g_abDrawReuse
{
     - 1
} ;
extern "C" unsigned n3dsFifoDrainStalls;
#include <3ds.h>
#include <citro3d.h>
#include <dirent.h>
#include <sys/iosupport.h>
#include <algorithm>
#include <atomic>
#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include "ww_n3ds.h"
#include "ww_renderer3ds.h"
#include "ww_packet_worker3ds.h"
#include "ww_texcache.h"
#include "ww_texconv.h"
#include "ww_render_snapshot_3ds.h"
#include "ww_record_frame_3ds.h"
#include "ww_material_program_pool.h"
#include "ww_prepared_material_store.h"
#include "first_boot_3ds.h"
static t2p::MaterialProgramPool sMaterialPrograms;
static t2p::MaterialVersions sMaterialVersions;
static t2p::PreparedMaterialStore sPreparedMaterials;
static bool sPreparedMaterialsEnabled = false;
extern "C" volatile unsigned g_n3dsShapeGen;
extern "C" void n3dsMaterialWriting(const void * p,
uint32_t n)
{
    sMaterialVersions.writing(p,
n);
    
}
extern "C" uint64_t n3dsMaterialRegisterVersioned(const uint8_t * p,
uint32_t n)
{
    static const bool audit = []
    {
        const char * a = std::getenv("N3DS_MODEL_ID_AUDIT");
        return a 
&& * a == '1';
        
    }
    ();
    const auto version = sMaterialVersions.sample(p,
n,
g_n3dsShapeGen);
    if(sPreparedMaterialsEnabled)return sMaterialPrograms.publishPrepared(p,
n,
version,
audit,
[](const uint8_t * bytes,
size_t size,
const std::shared_ptr < const t2p::PreparedMaterial > & previous,
std::vector < uint32_t > & args)
    {
        return sPreparedMaterials.resolve(bytes,
size,
previous,
args);
    }
    );
    return sMaterialPrograms.publishVersioned(p,
n,
version,
audit);
    
}
extern "C" uint64_t n3dsMaterialRegister(const uint8_t * p,
uint32_t n) 
{
    return sMaterialPrograms.publish(p,
n);
    
}
extern "C" const t2p::GxStateProgram * n3dsMaterialBorrow(uint64_t t) 
{
    return sMaterialPrograms.borrowed(t);
    
}
extern "C" void n3dsMaterialRelease(uint64_t t) 
{
    sMaterialPrograms.release(t);
    
}
namespace t2p 
{
    extern uint64_t g_replayProf[16];
    extern bool g_replayProfOn;
    
}
#include "ww_performance_3ds.h"
#include "ww_bottom_ui_3ds.h"
#include "ww_depth_visibility_3ds.h"
#include "ww_depth_probe_3ds.h"
#include "ww_core_probe_3ds.h"
extern "C" uint64_t g_n3dsDecodeCount[2];
extern "C" volatile unsigned g_n3dsShapeGen;
#include "ww_uniform_storage.h"
#include <unistd.h>
extern "C" char * fake_heap_end;
extern "C" 
{
    extern volatile int g_n3dsDlCensusOpen;
    void n3dsDlCensusWrite(const char * );
    
}
extern "C" void n3dsLockStatsLog(unsigned long frame);
extern "C" void n3dsAllocSitesLog(unsigned long frame);
void n3dsFastAllocEnable(bool on,
int maxSpans = 0);
void n3dsFastAllocLog();
void n3dsFastAllocBorrow(bool on);
static size_t brkRoom() 
{
    char * brk = (char * )sbrk(0);
    return brk 
&& brk != (char * ) - 1 
&& brk < fake_heap_end ? size_t(fake_heap_end - brk) : 0;
    
}
static uint64_t g_originTicks[2], g_originBytes[2], g_originDraws[2], g_originVerts[2], g_originCalls[2];
static size_t g_minBrkRoom = ~size_t(0);
static size_t g_heapCeiling = 85u << 20;
static std::atomic < bool > g_fenceRoomOnly
{
    false
} ;
extern "C" void n3dsPoolsTrim(void) 
{
    t2p::UniformBlockPool::get().trim();
    t2p::EventNodePool::get().trim();
    #if defined(N3DS_EVENT_INDIRECT) && N3DS_EVENT_INDIRECT
t2p::DirectLegacyStatePool::get().trim();
    #endif
}
namespace 
{
    void poolsFenceTrim();
    
}
static bool heapRoom(size_t * inUse) 
{
    const struct mallinfo mi = mallinfo();
    const size_t room = brkRoom();
     * inUse = size_t(mi.uordblks);
    g_minBrkRoom = std::min(g_minBrkRoom,
room);
    const bool ok = size_t(mi.uordblks) <= g_heapCeiling 
&& (g_fenceRoomOnly ? room >= (3u << 20) : room + size_t(mi.fordblks) >= (6u << 20));
    if (!ok) poolsFenceTrim();
    return ok;
    
}
static bool heapReady() 
{
    const struct mallinfo mi = mallinfo();
    const size_t room = brkRoom();
    g_minBrkRoom = std::min(g_minBrkRoom,
room);
    const bool ok = size_t(mi.uordblks) <= g_heapCeiling 
&& (g_fenceRoomOnly ? room >= (3u << 20) : true) 
&& size_t(mi.fordblks) + room >= (8u << 20);
    if (!ok) poolsFenceTrim();
    return ok;
    
}
extern "C" void C3Di_RenderQueueWaitDone(void);
extern "C" uint64_t g_n3dsIndirectDraws, g_n3ds2dDraws, g_n3ds2dHash;
namespace 
{
    constexpr int TW = 240, TH = 400;
    struct HostMem : t2p::GuestMem 
    {
        RenderSnapshot3ds snapshot;
        const RenderSnapshot3ds * view = nullptr;
        bool frozen = false, verifying = true;
        mutable uint32_t missing = 0, verifyMissing = 0;
        const uint8_t * read(uint32_t addr,
size_t size) const override 
        {
            if (!frozen) return addr ? reinterpret_cast < const uint8_t * > (uintptr_t(addr)) : nullptr;
            const RenderSnapshot3ds& sn = view ? * view : snapshot;
            const auto * p = sn.read(addr,
size);
            if (!p 
&& addr 
&& sn.isKnown(addr,
size)) return reinterpret_cast < const uint8_t * > (uintptr_t(addr));
            if (!p) 
            {
                if (verifying) ++verifyMissing;
                else ++missing;
                
            }
            return p;
            
        }
        bool hashHint(uint32_t addr,
size_t size,
uint64_t& hash) const override 
        {
            if (!frozen) return false;
            return (view ? * view : snapshot).hint(addr,
size,
hash);
            
        }
        
    } ;
    HostMem g_mem;
    extern "C" void n3dsAudioHealth(void);
    Performance3ds g_tuning;
    void poolsFenceTrim() 
    {
        const unsigned f = unsigned(g_tuning.floorPool);
        t2p::UniformBlockPool::get().trimTo(f / 4);
        t2p::EventNodePool::get().trimTo(f);
        #if defined(N3DS_EVENT_INDIRECT) && N3DS_EVENT_INDIRECT
t2p::DirectLegacyStatePool::get().trimTo(f / 4);
        #endif
}
    bool g_pipeline = false;
    t2p::CpuFrame3ds * g_cpuFrames[2] = 
    {
        
    } ;
    t2p::FrameRecorder3ds * g_recorder = nullptr;
    t2p::CpuFrame3ds * g_submitted = nullptr;
    unsigned g_recordIndex = 0;
    t2p::Renderer3ds * g_r = nullptr;
    C3D_RenderTarget * g_tgt = nullptr;
    DepthVisibility3ds g_depthVisibility;
    FILE * g_log = nullptr;
    LightLock g_logLock;
    bool g_consoleOn = true;
    BottomUi3ds g_bottomUi;
    std::atomic < bool > g_routeOwnsBottom
    {
        false
    } ;
    std::atomic < uint32_t > g_frames
    {
        0
    } ;
    extern "C" 
    {
        volatile unsigned g_n3dsMark[8] = 
        {
            
        } ;
        
    }
    static const char * volatile g_traceWhat = "-";
    static volatile int g_traceIdx = 0;
    static volatile unsigned g_traceCount = 0;
    std::atomic < uint32_t > g_inputFrames
    {
        0
    } ;
    size_t g_missesWritten = 0;
    static bool g_noBundleWrites = false;
    static std::atomic < uint32_t > g_paintEndedN
    {
        0
    }
    , g_paintHandedN
    {
        0
    } ;
    static uint64_t g_paintSlotRing[8] = 
    {
        
    } ;
    static uint64_t g_submittedSlot = 0;
    static uint64_t g_renderSlot = 0;
    static uint64_t n3dsPaintSlotOf(uint32_t n) 
    {
        return g_paintSlotRing[n & 7];
        
    }
    static uint32_t g_paintHandedCount() 
    {
        return g_paintHandedN.load(std::memory_order_acquire);
        
    }
    uint64_t g_processTicks = 0;
    std::vector < uint8_t > g_xfb;
    std::string g_errLast;
    std::vector < uint64_t > g_shotFrames;
    FILE * g_shots = nullptr;
    uint64_t g_pendingShot = 0;
    std::atomic < bool > g_captureRequest
    {
        false
    } ;
    uint64_t g_pendingCapture = 0;
    unsigned g_captureCount = 0;
    uint32_t g_errCount = 0;
    struct TextureRanges : t2p::GxSink 
    {
        uint32_t primitiveRunLimit() const override 
        {
            return g_r->primitiveRunLimit();
            
        }
        void collect(const t2p::GxRegs& r) 
        {
            if (!n3dsRenderThreadActive()) return;
            for (int m=0;m<8;++m) 
            {
                const int off = (m < 4 ? m : m - 4) + (m < 4 ? 0 : 0x20);
                if (!r.bpWritten[0x88 + off]) continue;
                const uint32_t image = r.bp[0x88 + off]&0xffffff, mode = r.bp[0x80 + off]&0xffffff;
                const uint32_t addr = r.texAddr[m] ? r.texAddr[m] : (r.bp[0x94 + off]&0x1fffff) << 5;
                if (!addr || g_r->cache().renderedAt(addr)) continue;
                t2p::GxTexBinding binding;
                binding.width = int(image&1023) + 1;
                binding.height = int((image >> 10)&1023) + 1;
                binding.fmt = int((image >> 20)&15);
                binding.mipMode = uint8_t((mode >> 5)&3);
                binding.maxLod = float((r.bp[0x84 + off] >> 8)&255) / 16.0f;
                size_t bytes = 0;
                int w = binding.width, h = binding.height;
                for (int level=0;level<binding.mipLevels();++level) 
                {
                    bytes += t2p::gxTextureBytes(binding.fmt,
w,
h);
                    w = std::max(1,
w / 2);
                    h = std::max(1,
h / 2);
                    
                }
                g_mem.snapshot.add(addr,
bytes <= UINT32_MAX ? uint32_t(bytes) : 0);
                
            }
            
        }
        void onDraw(const t2p::GxRegs& r,
const t2p::GxDraw& d) override 
        {
            collect(r);
            g_r->onDraw(r,
d);
            
        }
        t2p::GxDraw * appendTarget(const t2p::GxRegs& r,
const t2p::GxDraw& h) override 
        {
            collect(r);
            return g_r->appendTarget(r,
h);
            
        }
        void onAppended(const t2p::GxRegs& r,
t2p::GxDraw& d,
uint32_t first) override 
        {
            g_r->onAppended(r,
d,
first);
            
        }
        void onCopy(const t2p::GxRegs& r) override 
        {
            g_r->onCopy(r);
            
        }
        void onTlutLoad(const t2p::GxRegs& r) override 
        {
            g_r->onTlutLoad(r);
            
        }
        void onError(const std::string& e) override 
        {
            g_r->onError(e);
            
        }
        const uint8_t * resolveDisplayList(uint32_t a,
uint32_t n) override 
        {
            return g_r->resolveDisplayList(a,
n);
            
        }
        
    }
    g_ranges;
    uint32_t g_snapshotFallbacks = 0;
    uint64_t g_lastPresent = 0;
    double g_presentSum = 0, g_presentMin = 1e30, g_presentMax = 0;
    unsigned g_presentCount = 0;
    bool g_measurePresent = false;
    struct FrameSample 
    {
        uint32_t frame, drawn, gx, texMiss, texConv, texEvict, texKB;
        float process, render, build, plan, draw, present;
        u64 presentTick;
        
    } ;
    constexpr uint32_t kFrameLogMax = 3000;
    FrameSample g_frameLog[kFrameLogMax];
    uint32_t g_frameLogFrom = 0, g_frameLogTo = 0, g_frameLogCount = 0;
    bool g_frameLogDraws = false;
    t2p::TexCacheStats g_frameLogTex;
    bool g_textureDiag = false;
    static bool skipSnapshots(uint32_t addr,
uint32_t size,
const uint8_t * live,
uint64_t& hash) 
    {
        if (!g_tuning.skipSnapshots || !g_r) return false;
        if (g_tuning.copySnapshots 
&& t2p::texRenderedAt(addr)) 
        {
            hash = 0;
            return true;
            
        }
        hash = t2p::sampleHashOf(live,
size,
g_r->cache().sampleBytes());
        return t2p::texKnownContains(addr,
size,
hash);
        
    }
    extern "C" char * fake_heap_start;
    extern "C" char * fake_heap_end;
    const uint8_t * readableTexture(uint32_t addr,
uint32_t size) 
    {
        uint64_t p = addr, end = uint64_t(addr) + size;
        if (addr 
&& uintptr_t(fake_heap_start) <= addr 
&& end <= uint64_t(uintptr_t(fake_heap_end))) return reinterpret_cast < const uint8_t * > (uintptr_t(addr));
        while (p < end) 
        {
            MemInfo info;
            PageInfo page;
            if (R_FAILED(svcQueryMemory(&info,
&page,
uint32_t(p))) || !(info.perm&MEMPERM_READ) || 
            uint64_t(info.base_addr) + info.size <= p) return nullptr;
            p = uint64_t(info.base_addr) + info.size;
            
        }
        return reinterpret_cast < const uint8_t * > (uintptr_t(addr));
        
    }
    constexpr size_t kLogBufBytes = 192u << 10;
    char g_logBuf[2][kLogBufBytes];
    size_t g_logLen[2] = 
    {
        0, 0
    } ;
    int g_logCur = 0;
    size_t g_logDropped = 0;
    LightLock g_logFileLock;
    bool g_logAsync = false;
    void logFlushPendingLocked() 
    {
        LightLock_Lock(&g_logLock);
        const int idx = g_logCur;
        g_logCur ^= 1;
        g_logLen[g_logCur] = 0;
        const size_t n = g_logLen[idx];
        const size_t dropped = g_logDropped;
        g_logDropped = 0;
        FILE * f = g_log;
        LightLock_Unlock(&g_logLock);
        if (!f) return;
        if (n) std::fwrite(g_logBuf[idx],
1,
n,
f);
        if (dropped) std::fprintf(f,
"[log] %u bytes of log lines dropped (buffer full)\n",
unsigned(dropped));
        if (n || dropped) std::fflush(f);
        
    }
    void logWriterThread(void * ) 
    {
        for (;;) 
        {
            svcSleepThread(500000000LL);
            LightLock_Lock(&g_logFileLock);
            logFlushPendingLocked();
            LightLock_Unlock(&g_logFileLock);
            
        }
        
    }
    bool logUrgent(const char * p,
size_t n) 
    {
        if (n >= 6 
&& !std::memcmp(p,
"ww3ds ",
6)) 
        {
            p += 6;
            n -= 6;
            
        }
        static const char * const kUrgent[] = 
        {
            "[exit]", "[watchdog]", "[hang", "[crash", "[fatal", "[abort", "[hwtest]", "[oom"
        } ;
        for (const char* u : kUrgent) 
        {
            const size_t k = std::strlen(u);
            if (n >= k 
&& !std::memcmp(p,
u,
k)) return true;
            
        }
        return false;
        
    }
    void logWrite(const char * p,
size_t n) 
    {
        if (g_logAsync 
&& !logUrgent(p,
n)) 
        {
            LightLock_Lock(&g_logLock);
            size_t& len = g_logLen[g_logCur];
            if (len + n <= kLogBufBytes) 
            {
                std::memcpy(g_logBuf[g_logCur] + len,
p,
n);
                len += n;
                
            } else g_logDropped += n;
            svcOutputDebugString(p,
int(n));
            if (g_consoleOn) std::fwrite(p,
1,
n,
stdout);
            LightLock_Unlock(&g_logLock);
            return;
            
        }
        LightLock_Lock(&g_logFileLock);
        logFlushPendingLocked();
        LightLock_Lock(&g_logLock);
        if (g_log) 
        {
            std::fwrite(p,
1,
n,
g_log);
            std::fflush(g_log);
            
        }
        svcOutputDebugString(p,
int(n));
        if (g_consoleOn) std::fwrite(p,
1,
n,
stdout);
        LightLock_Unlock(&g_logLock);
        LightLock_Unlock(&g_logFileLock);
        
    }
    ssize_t errWrite(struct _reent * ,
void * ,
const char * p,
size_t n) 
    {
        logWrite(p,
n);
        return ssize_t(n);
        
    }
    const devoptab_t kErrDev = 
    {
        "n3dserr", 0, nullptr, nullptr, errWrite, nullptr
    } ;
    void push32(std::vector < uint8_t > & v,
uint32_t x) 
    {
        v.push_back(uint8_t(x >> 24));
        v.push_back(uint8_t(x >> 16));
        v.push_back(uint8_t(x >> 8));
        v.push_back(uint8_t(x));
        
    }
    
}
extern "C" void C3Dx_SetOutputRows(C3D_RenderTarget * target,
u16 rows);
static void routeOwnerChanged(bool owns)
{
    g_routeOwnsBottom.store(owns,
std::memory_order_release);
    n3dsUiSetGpuOwnsBottom(owns?1:0);
    static const bool fullRows = std::getenv("N3DS_ROUTE_FULL") != nullptr;
    if(owns 
&& g_r 
&& !fullRows)if(C3D_RenderTarget * t = g_r->uiRouteTarget())C3Dx_SetOutputRows(t,
n3ds_panel::kGpuRows);
    g_bottomUi.ownerChanged();
    n3dsLog("[ui-route] bottom screen %s\n",
owns?"taken by the map route":"given back");
    
}
extern "C" void n3dsUiToggleMapBottom(void)
{
    if(!g_r || !g_tuning.lowerScreen)return;
    g_tuning.mapOnLower = !g_tuning.mapOnLower;
    n3ds_panel::mapBottom.store(g_tuning.mapOnLower,
std::memory_order_release);
    g_r->setUiRoute(g_tuning.mapOnLower,
routeOwnerChanged);
    n3dsLog("[ui-route] map button %s\n",
g_tuning.mapOnLower?"on":"off");
    
}
#include "ww_settings_menu_3ds.h"
extern "C" 
{
    void n3dsLog(const char * fmt,
...) 
    {
        char buf[512];
        va_list ap;
        va_start(ap,
fmt);
        const int n = std::vsnprintf(buf,
sizeof buf,
fmt,
ap);
        va_end(ap);
        if (n > 0) 
        {
            char line[576];
            const int m = std::snprintf(line,
sizeof line,
"ww3ds %s",
buf);
            if (m > 0) logWrite(line,
size_t(m) < sizeof line ? size_t(m) : sizeof line - 1);
            
        }
        
    }
    static void n3dsPgoTick(unsigned frame);
    static const char * g_logPath = N3DS_ROOT "/log.txt";
    void n3dsSetLogPath(const char * path)
    {
        if(path)g_logPath = path;
        
    }
    static std::atomic < bool > g_processExiting
    {
        false
    } ;
    static std::atomic < const char * > g_bootStage
    {
        "init"
    } ;
    static u32 g_mainThreadId = 0;
    static bool g_c3dReady = false;
    static bool g_gpuShutdownDone = false;
    void n3dsSetBootStage(const char * stage)
    {
        if (stage) g_bootStage.store(stage,
std::memory_order_release);
        
    }
    const char * n3dsBootStage(void)
    {
        const char * s = g_bootStage.load(std::memory_order_acquire);
        return s ? s : "init";
        
    }
    void n3dsPumpApt(void)
    {
        aptMainLoop();
        
    }
    void n3dsShowBootError(const char * message)
    {
        LightLock_Lock(&g_logLock);
        g_consoleOn = true;
        LightLock_Unlock(&g_logLock);
        consoleInit(GFX_BOTTOM,
nullptr);
        if (message) 
        {
            std::printf("%s",
message);
            n3dsLog("%s",
message);
            
        }
        gfxFlushBuffers();
        gfxScreenSwapBuffers(GFX_BOTTOM,
false);
        
    }
    int n3dsWaitStartOrQuit(void)
    {
        while (aptMainLoop()) 
        {
            hidScanInput();
            if (hidKeysDown() & KEY_START) return 1;
            gfxFlushBuffers();
            gfxSwapBuffers();
            gspWaitForVBlank();
            
        }
        return 0;
        
    }
    int n3dsPrepareExit(void)
    {
        g_processExiting.store(true);
        n3dsSetBootStage("exit");
        if(g_mainThreadId)
        {
            LightLock_Lock(&g_logLock);
            g_consoleOn = false;
            LightLock_Unlock(&g_logLock);
            
        }
        u32 id = 0;
        svcGetThreadId(&id,
CUR_THREAD_HANDLE);
        return g_mainThreadId 
&& id == g_mainThreadId;
        
    }
    void n3dsCloseLog(void)
    {
        LightLock_Lock(&g_logFileLock);
        g_logAsync = false;
        logFlushPendingLocked();
        LightLock_Lock(&g_logLock);
        if(g_log)
        {
            std::fclose(g_log);
            g_log = nullptr;
            
        }
        LightLock_Unlock(&g_logLock);
        LightLock_Unlock(&g_logFileLock);
        
    }
    extern "C" void n3dsFlushLog(void)
    {
        LightLock_Lock(&g_logFileLock);
        logFlushPendingLocked();
        LightLock_Unlock(&g_logFileLock);
        
    }
    int n3dsInit(void) 
    {
        LightLock_Init(&g_logLock);
        LightLock_Init(&g_logFileLock);
        svcGetThreadId(&g_mainThreadId,
CUR_THREAD_HANDLE);
        osSetSpeedupEnable(true);
        gfxInitDefault();
        gfxSetWide(false);
        consoleInit(GFX_BOTTOM,
nullptr);
        if (!std::getenv("N3DS_DEBUG_CONSOLE")) 
        {
            g_consoleOn = false;
            g_bottomUi.update(0,
0);
            gfxFlushBuffers();
            gfxScreenSwapBuffers(GFX_BOTTOM,
false);
            
        }
        mkdir(N3DS_ROOT,
0777);
        int hwTest = - 1;
        bool hwTestRequested = false;
        if (FILE * enable = std::fopen(N3DS_ROOT "/hwtest.enable",
"r")) 
        {
            hwTestRequested = true;
            std::fclose(enable);
            
        }
        if (hwTestRequested)
        if (FILE * tf = std::fopen(N3DS_ROOT "/hwtest.txt",
"r")) 
        {
            if (std::fscanf(tf,
"%d",
&hwTest) != 1) hwTest = - 1;
            std::fclose(tf);
            
        }
        if (osGetMemRegionSize(MEMREGION_APPLICATION) < (90u << 20)) hwTest = - 1;
        if (hwTest >= 0) 
        {
            if (FILE * tf = std::fopen(N3DS_ROOT "/hwtest.txt",
"w")) 
            {
                std::fprintf(tf,
"%d\n",
hwTest + 1);
                std::fclose(tf);
                
            }
            char name[64];
            std::snprintf(name,
sizeof name,
N3DS_ROOT "/log_test%d.txt",
hwTest);
            g_log = std::fopen(name,
"w");
            g_hwTest = hwTest <= 5 ? hwTest : 0;
            g_shaderOutTest = g_hwTest >= 1 
&& g_hwTest <= 4 ? g_hwTest : 0;
            std::remove(N3DS_ROOT "/performance.ini");
            
        } else
        g_log = std::fopen(g_logPath,
"w");
        devoptab_list[STD_ERR] = &kErrDev;
        if (hwTest < 0 
&& !std::getenv("N3DS_LOG_SYNC")) 
        {
            Thread lt = threadCreate(logWriterThread,
nullptr,
0x2000,
0x3F,
1,
true);
            if (!lt) lt = threadCreate(logWriterThread,
nullptr,
0x2000,
0x3F,
0,
true);
            g_logAsync = lt != nullptr;
            
        }
        setvbuf(stderr,
nullptr,
_IONBF,
0);
        bool isNew = false;
        APT_CheckNew3DS(&isNew);
        extern u32 __ctru_heap_size, __ctru_linear_heap_size;
        if (hwTest >= 0) n3dsLog("[hwtest] launch %d, variant %d\n",
hwTest,
g_hwTest);
        n3dsLog("[3ds] Wind Waker 3DS port: New 3DS %d; application memory %lu KB (free %lu KB), heap %lu KB, linear heap %lu KB\n",
int(isNew),
        (unsigned long)(osGetMemRegionSize(MEMREGION_APPLICATION) >> 10),
(unsigned long)(osGetMemRegionFree(MEMREGION_APPLICATION) >> 10),
        (unsigned long)(__ctru_heap_size >> 10),
(unsigned long)(__ctru_linear_heap_size >> 10));
        g_r3dsTrace = [](const char * what,
int index) 
        {
            g_traceWhat = what;
            g_traceIdx = index;
            g_traceCount = g_traceCount + 1;
            
        } ;
        static const auto hwTestNext = [] 
        {
            u64 tid = 0;
            if (R_FAILED(APT_GetProgramID(&tid)) || R_FAILED(nsInit())) 
            {
                n3dsLog("[hwtest] cannot reboot by itself\n");
                return;
                
            }
            if (g_log) 
            {
                std::fflush(g_log);
                std::fclose(g_log);
                g_log = nullptr;
                
            }
            NS_RebootToTitle(MEDIATYPE_SD,
tid,
6);
            for (;;) svcSleepThread(100000000LL);
            
        } ;
        static volatile unsigned probeHigh = 0, probeLow = 0;
        #if defined(N3DS_SCHED_PROBES) && N3DS_SCHED_PROBES
threadCreate([](void * ) 
        {
            for (;;) 
            {
                svcSleepThread(10000000LL); probeHigh = probeHigh + 1; 
            }
            
        }
        ,
nullptr,
0x1000,
0x1A,
0,
true);
        threadCreate([](void * ) 
        {
            for (;;) 
            {
                svcSleepThread(10000000LL); probeLow = probeLow + 1; 
            }
            
        }
        ,
nullptr,
0x1000,
0x3F,
0,
true);
        #else
n3dsLog("[watchdog] scheduler probes disabled; two diagnostic thread slots freed\n");
        #endif
static volatile unsigned * const probes[2] = 
        {
            &probeHigh, &probeLow
        } ;
        static Thread watchdog = threadCreate([](void * ) 
        {
            extern unsigned g_n3dsAudioBlocks;
            extern void * g_n3dsGxQueue; extern unsigned * g_n3dsCmdListAddr[4]; extern unsigned g_n3dsCmdListSize[4]; extern unsigned g_n3dsCmdListCount;
            unsigned long seen[2] = 
            {
                ~0ul,
~0ul
            } ; bool dumped = false;
            for (int i = 1; i <= 60; i++) 
            {
                svcSleepThread(2000000000LL);
                if(g_processExiting.load())return;
                const unsigned long now = g_frames.load();
                static const bool dumpTest = std::getenv("N3DS_HANG_DUMP_TEST") != nullptr;
                if (!aptIsActive()) 
                {
                    seen[0] = seen[1] = ~0ul; continue; 
                }
                if (!dumped 
&& ((now > 0 
&& now == seen[0] 
&& now == seen[1]) || (dumpTest 
&& i == 1))) 
                {
                    dumped = true;
                    if (const gxCmdQueue_s * q = static_cast < const gxCmdQueue_s * > (g_n3dsGxQueue)) 
                    {
                        n3dsLog("[gpu-hang] GX queue: %u entries, next to submit %u, completed %u (capacity %u)\n",
q->numEntries,
q->curEntry,
q->lastEntry,
q->maxEntries);
                        for (unsigned e = 0; e < q->numEntries 
&& e < 16; e++) 
                        {
                            const u32 * d = q->entries[e].data;
                            n3dsLog("[gpu-hang]   entry %u: %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx\n",
e,
(unsigned long)d[0],
(unsigned long)d[1],
(unsigned long)d[2],
(unsigned long)d[3],
(unsigned long)d[4],
(unsigned long)d[5],
(unsigned long)d[6],
(unsigned long)d[7]);
                            
                        }
                        
                    } else n3dsLog("[gpu-hang] no GX queue bound\n");
                    for (unsigned k = 0; k < 1 
&& k < g_n3dsCmdListCount; k++) 
                    {
                        const unsigned slot = (g_n3dsCmdListCount - 1 - k) & 3;
                        const unsigned * a = g_n3dsCmdListAddr[slot]; const unsigned sz = g_n3dsCmdListSize[slot];
                        n3dsLog("[gpu-hang] command list -%u: address %p, %u bytes (list number %u)\n",
k,
(const void * )a,
sz,
g_n3dsCmdListCount - k);
                        char name[96]; std::snprintf(name,
sizeof name,
N3DS_ROOT "/hang_v%d_cmdlist_%u.bin",
g_hwTest,
k);
                        if (a 
&& sz 
&& sz <= (2u << 20)) if (FILE * f = std::fopen(name,
"wb")) 
                        {
                            std::fwrite(a,
1,
sz,
f); std::fclose(f); 
                        }
                        
                    }
                    n3dsLog("[gpu-hang] saved\n");
                    if (g_hwTest) 
                    {
                        n3dsLog("[hwtest] variant %d HANGS at frame %lu; rebooting into the next one\n",
g_hwTest,
now); hwTestNext(); 
                    }
                    
                }
                if (g_hwTest 
&& i == 8 
&& now > 60) 
                {
                    n3dsLog("[hwtest] variant %d RUNS (frame %lu after 16 s); rebooting into the next one\n",
g_hwTest,
now); hwTestNext(); 
                }
                seen[1] = seen[0]; seen[0] = now;
                n3dsLog("[watchdog] %d s: boot \"%s\", frame %lu, audio blocks %u, drains %u/%u, worker process %u/%u end-frame stage %u, renderer op %u stage %u, mail yields %u; renderer at \"%s\" %d (step %u); core-0 probes prio 0x1A %u, 0x3F %u\n",
i * 2,
n3dsBootStage(),
(unsigned long)g_frames.load(),
g_n3dsAudioBlocks,
g_n3dsMark[0],
g_n3dsMark[1],
g_n3dsMark[2],
g_n3dsMark[3],
g_n3dsMark[4],
g_n3dsMark[5],
g_n3dsMark[7],
g_n3dsMark[6],
g_traceWhat,
g_traceIdx,
g_traceCount,
* probes[0],
* probes[1]);
                
            }
            
        }
        ,
nullptr,
16u << 10,
0x19,
isNew ? 2 : 0,
true);
        (void)watchdog;
        return isNew ? 1 : 0;
        
    }
    namespace cachetest 
    {
        static void fill(u8 * p,
size_t n,
u32 seed) 
        {
            u32 x = seed * 2654435761u + 1;
            for (size_t i = 0; i < n; i += 4) 
            {
                x ^= x << 13;
                x ^= x >> 17;
                x ^= x << 5;
                std::memcpy(p + i,
&x,
4);
                
            }
            
        }
        struct Job 
        {
            u8 * p;
            size_t n;
            u32 seed;
            
        } ;
        static void fillThread(void * a) 
        {
            Job * j = static_cast < Job * > (a);
            fill(j->p,
j->n,
j->seed);
            
        }
        static unsigned compare(const u8 * b,
size_t n,
u32 seed) 
        {
            static u8 ref[32];
            unsigned bad = 0;
            u32 x = seed * 2654435761u + 1;
            for (size_t i = 0; i < n; i += 32) 
            {
                for (int k = 0; k < 32; k += 4) 
                {
                    x ^= x << 13;
                    x ^= x >> 17;
                    x ^= x << 5;
                    std::memcpy(ref + k,
&x,
4);
                    
                }
                if (std::memcmp(ref,
b + i,
32)) bad++;
                
            }
            return bad;
            
        }
        static bool dma(u8 * a,
u8 * b,
size_t n) 
        {
            C3D_SyncTextureCopy((u32 * )a,
GX_BUFFER_DIM(512 >> 4,
0),
(u32 * )b,
GX_BUFFER_DIM(512 >> 4,
0),
u32(n),
GX_TRANSFER_RAW_COPY(1));
            GSPGPU_InvalidateDataCache(b,
u32(n));
            static int logged = 0;
            if (logged++ < 1) 
            {
                u32 x, y;
                std::memcpy(&x,
a,
4);
                std::memcpy(&y,
b,
4);
                n3dsLog("[cache-test] first words: source %08lx copy %08lx\n",
(unsigned long)x,
(unsigned long)y);
                
            }
            return true;
            
        }
        static unsigned run(const char * tag) 
        {
            const size_t N = 512u << 10;
            u8 * a = static_cast < u8 * > (linearAlloc(N));
            u8 * b = static_cast < u8 * > (linearAlloc(N));
            if (!a || !b) 
            {
                if (a) linearFree(a);
                if (b) linearFree(b);
                n3dsLog("[cache-test] %s: no linear memory\n",
tag);
                return 0;
                
            }
            unsigned total = 0, r[4] = 
            {
                
            } ;
            std::memset(b,
0x5A,
N);
            GSPGPU_FlushDataCache(b,
N);
            fill(a,
N,
11);
            GSPGPU_FlushDataCache(a,
N);
            dma(a,
b,
N);
            r[0] = compare(b,
N,
11);
            fill(a,
N,
22);
            GSPGPU_FlushDataCache(a,
N);
            fill(a,
N,
23);
            GSPGPU_FlushDataCache(a,
N);
            dma(a,
b,
N);
            r[1] = compare(b,
N,
23);
            for (int core = 2; core <= 3; core++) 
            {
                Job j
                {
                    a, N, u32(30 + core)
                } ;
                Thread t = threadCreate(fillThread,
&j,
0x1000,
0x18,
core,
false);
                if (!t) 
                {
                    n3dsLog("[cache-test] %s: core %d refused\n",
tag,
core);
                    continue;
                    
                }
                threadJoin(t,
U64_MAX);
                threadFree(t);
                GSPGPU_FlushDataCache(a,
N);
                dma(a,
b,
N);
                r[core] = compare(b,
N,
u32(30 + core));
                
            }
            fill(a,
N,
44);
            for (size_t o = 0; o < N; o += 4096) GSPGPU_FlushDataCache(a + o,
4096);
            dma(a,
b,
N);
            const unsigned r4 = compare(b,
N,
44);
            total = r[0] + r[1] + r[2] + r[3] + r4;
            n3dsLog("[cache-test] %s: mismatched 32-byte lines of %u: same core %u, rewritten %u, core2 %u, core3 %u, 4KB flushes %u\n",
tag,
unsigned(N / 32),
r[0],
r[1],
r[2],
r[3],
r4);
            linearFree(a);
            linearFree(b);
            return total;
            
        }
        static void runAndFix() 
        {
            if (std::getenv("N3DS_NO_CACHE_TEST")) return;
            const unsigned bad = run("L2 as configured");
            bool isNew = false;
            if (!bad || R_FAILED(APT_CheckNew3DS(&isNew)) || !isNew) return;
            if (R_FAILED(ptmSysmInit())) return;
            const Result rc = PTMSYSM_ConfigureNew3DSCPU(1);
            n3dsLog("[cache-test] L2 cache off (configure %08lx)\n",
(unsigned long)rc);
            const unsigned bad2 = run("L2 off");
            if (bad2 >= bad) 
            {
                PTMSYSM_ConfigureNew3DSCPU(3);
                n3dsLog("[cache-test] L2 off did not help: L2 back on\n");
                
            }
            else n3dsLog("[cache-test] keeping L2 off\n");
            ptmSysmExit();
            
        }
        
    }
    int n3dsGpuInitLocal(void) 
    {
        n3dsSetBootStage("gpu");
        n3dsPumpApt();
        const char * presentFence = std::getenv("N3DS_PRESENT_FENCE");
        g_measurePresent = presentFence 
&& !std::strcmp(presentFence,
"1");
        const char * texDiag = std::getenv("N3DS_TEX_DIAG");
        g_textureDiag = texDiag 
&& !std::strcmp(texDiag,
"1");
        if (const char * fl = std::getenv("N3DS_FRAME_LOG")) 
        {
            unsigned a = 0, b = 0;
            if (std::sscanf(fl,
"%u-%u",
&a,
&b) == 2 
&& a >= 1 
&& b >= a) 
            {
                g_frameLogFrom = a;
                g_frameLogTo = std::min < uint32_t > (b,
a + kFrameLogMax - 1);
                const char * fd = std::getenv("N3DS_FRAME_LOG_DRAWS");
                g_frameLogDraws = fd 
&& !std::strcmp(fd,
"1");
                n3dsLog("[frame] per-frame log frames %u-%u draw timing %d\n",
g_frameLogFrom,
g_frameLogTo,
int(g_frameLogDraws));
                
            }
            
        }
        g_t2pProfOn = std::getenv("N3DS_PROF_DECODE") != nullptr;
        if (g_t2pProfOn) 
        {
            const unsigned long n = std::strtoul(std::getenv("N3DS_PROF_DECODE"),
nullptr,
10);
            g_t2pProfPeriod = n > 1 ? uint32_t(n) : 1u;
            n3dsLog("[prof] decode profile: every %lu-th GX draw timed\n",
(unsigned long)g_t2pProfPeriod);
            
        }
        if (!C3D_Init(0x200000)) 
        {
            n3dsLog("[3ds] C3D_Init failed\n");
            return 0;
            
        }
        g_c3dReady = true;
        g_gpuShutdownDone = false;
        n3dsPumpApt();
        cachetest::runAndFix();
        coreprobe::run("boot");
        if (std::getenv("N3DS_DEPTH_PROBE")) depthprobe::run();
        g_tgt = C3D_RenderTargetCreate(TW,
TH,
GPU_RB_RGBA8,
GPU_RB_DEPTH24_STENCIL8);
        if (!g_tgt) 
        {
            n3dsLog("[3ds] render target failed\n");
            return 0;
            
        }
        C3D_RenderTargetSetOutput(g_tgt,
GFX_TOP,
GFX_LEFT,
GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) |
        GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO));
        t2p::RendererOptions ro;
        ro.frame.efbW = 640;
        ro.frame.efbH = 480;
        ro.frame.targetW = 400;
        ro.frame.targetH = 240;
        ro.frame.fit = g_tuning.widePicture?t2p::FitMode::Stretch:t2p::FitMode::Pillarbox43;
        ro.frame.widePicture = g_tuning.widePicture;
        ro.frame.rotate = true;
        ro.materialCacheFastPath = g_tuning.cacheMaterials;
        ro.translate.fixedUniforms = g_tuning.fixedUniforms;
        ro.texCache.memoPalettes = g_tuning.cachePalettes;
        ro.workerPackets = g_tuning.workerPackets;
        ro.verifyPackets = g_tuning.verifyPackets;
        ro.keepPlanNotes = g_tuning.keepPlanNotes;
        ro.packDirect = g_tuning.packDirect;
        ro.batchMatrices = g_tuning.batchMatrices;
        ro.templatePlans = g_tuning.templatePlans;
        ro.packetNotes = false;
        ro.texCache.directEncode = g_tuning.textureDirect;
        ro.texCache.fastLossyError = g_tuning.errorTable;
        ro.texCache.budgetBytes = 8u << 20;
        t2p::g_packetWorkerCore = g_tuning.packetCore;
        n3dsFastAllocEnable(g_tuning.quickAlloc,
g_tuning.allocSpanCount);
        g_r = new t2p::Renderer3ds;
        if (!g_r->init(ro,
&g_mem,
g_tgt)) 
        {
            n3dsLog("[3ds] renderer init failed\n");
            return 0;
            
        }
        if(g_tuning.reuseDraws)g_r->setDrawReuse(true,
size_t(g_tuning.reuseDrawKb) << 10);
        n3dsLog("[perf] draw_reuse %d budget %d KB (linear free %lu KB)\n",
int(g_tuning.reuseDraws),
g_tuning.reuseDrawKb,
(unsigned long)(linearSpaceFree() >> 10));
        n3dsLog("[sizes] event %u state %u draw-state %u draw %u copy %u bytes\n",
unsigned(sizeof(t2p::RecordedEvent3ds)),
unsigned(sizeof(t2p::GxState)),
unsigned(sizeof(t2p::GxDrawState)),
unsigned(sizeof(t2p::GxDraw)),
unsigned(sizeof(t2p::GxCopy)));
        t2p::g_packPlanar = g_tuning.packPlanar;
        n3dsLog("[perf] planar_pack %d auto_ab %d\n",
int(g_tuning.packPlanar),
g_tuning.autoSplit);
        t2p::g_packVertexMajor = g_tuning.vertexMajorPack;
        n3dsLog("[perf] pack_vertex_major %d\n",
int(g_tuning.vertexMajorPack));
        
        {
            const char * k = std::getenv("N3DS_GPU_KICK");
            if(k)g_tuning.kickLimit = std::atoi(k);
            
        }
        g_r->backend().setKickDraws(g_tuning.kickLimit);
        n3dsLog("[perf] gpu_kick %d (GPU starts during the frame every split / %d draws; 0 = at frame end)\n",
g_tuning.kickLimit,
g_tuning.kickLimit);
        g_r->backend().setSplitStep(g_tuning.splitLimit / 100.0f);
        n3dsLog("[perf] gpu_split %d%% of the command buffer per GPU hand-off\n",
g_tuning.splitLimit);
        n3dsLog("[packet-worker] requested %d core %d active %d verify %d\n",
int(g_tuning.workerPackets),
g_tuning.packetCore,
int(g_r->packetWorkerActive()),
int(g_tuning.verifyPackets));
        g_r->setLogs([](const t2p::DrawReport& D) 
        {
            static std::vector < std::string > seen;
            if (g_textureDiag 
&& D.keepPlanNotes) 
            {
                static std::vector < std::string > notes;
                for (const auto& note : *D.keepPlanNotes) 
                {
                    if (notes.size() < 150 
&& std::find(notes.begin(),
notes.end(),
note) == notes.end()) 
                    {
                        notes.push_back(note);
                        n3dsLog("[tex-diag] frame %lu draw %d: %s\n",
(unsigned long)g_frames.load(),
D.index,
note.c_str());
                        
                    }
                    
                }
                
            }
            const double ms = D.buildMs + D.planMs + D.drawMs;
            bool newWhy = false;
            if (!D.drawn 
&& std::find(seen.begin(),
seen.end(),
D.why) == seen.end() 
&& seen.size() < 100) 
            {
                seen.push_back(D.why); newWhy = true; 
            }
            if (ms > 150 || newWhy)
            n3dsLog("[draw] #%d %s build %.0f plan %.0f draw %.0f ms verts %u%s%s\n",
D.index,
D.drawn ? "drawn" : "REFUSED",
D.buildMs,
D.planMs,
D.drawMs,
            unsigned(D.vertices),
D.drawn ? "" : ": ",
D.why.c_str());
            
        }
        ,
nullptr,
[](const std::string& e) 
        {
            g_errCount++;
            if (e != g_errLast 
&& g_errCount < 200) 
            {
                n3dsLog("[gx] %s\n",
e.c_str()); g_errLast = e; 
            }
            
        }
        );
        if (n3dsRenderThreadActive()) g_r->setCacheVerifiedHook([] 
        {
            g_mem.verifying = false; 
        }
        );
        if (FILE * sf = std::fopen(N3DS_ROOT "/shots.txt",
"r")) 
        {
            unsigned long long f;
            while (std::fscanf(sf,
"%llu",
&f) == 1) g_shotFrames.push_back(f);
            std::fclose(sf);
            if (!g_shotFrames.empty()) g_shots = std::fopen(N3DS_ROOT "/shots.bin",
"wb");
            
        }
        g_r->setFrameBeginHook([] 
        {
            if(g_tuning.lowerScreen 
&& g_routeOwnsBottom.load(std::memory_order_acquire))
            {
                g_bottomUi.paintColumn();g_bottomUi.captureRouted((unsigned long)g_frames.load());
                
            }
            static int idState = 0;
            if (idState == 1 
&& g_tgt) 
            {
                idState = 0;
                g_r->backend().beginDrawIdFrame(false);
                static FILE * xf = [] 
                {
                    FILE * f = std::fopen(N3DS_ROOT "/captures.bin",
"ab"); if (f) std::setvbuf(f,
nullptr,
_IOFBF,
64u << 10); return f; 
                }
                ();
                if (xf) 
                {
                    const u8 * cb = reinterpret_cast < const u8 * > (g_tgt->frameBuf.colorBuf);
                    static u8 copy[16384]; const size_t total = size_t(TW) * TH * 4;
                    char nm[32] = 
                    {
                        
                    } ; std::snprintf(nm,
sizeof nm,
"drawid %llu",
(unsigned long long)g_frames.load());
                    std::fwrite(nm,
1,
sizeof nm,
xf);
                    for (size_t o = 0; o < total; o += sizeof copy) 
                    {
                        const size_t n = std::min(sizeof copy,
total - o); std::memcpy(copy,
cb + o,
n); std::fwrite(copy,
1,
n,
xf); 
                    }
                    std::memset(copy,
0,
sizeof copy);
                    for (size_t o = 0; o < total; o += sizeof copy) 
                    {
                        const size_t n = std::min(sizeof copy,
total - o); std::fwrite(copy,
1,
n,
xf); 
                    }
                    std::fflush(xf);
                    n3dsLog("[drawid] capture saved (frame %llu)\n",
(unsigned long long)g_frames.load());
                    
                }
                
            }
            if (g_pendingCapture 
&& g_tgt) 
            {
                if (g_captureCount < 12) 
                {
                    idState = 1; g_r->backend().beginDrawIdFrame(true); n3dsLog("[drawid] frame after capture %llu drawn with draw numbers\n",
(unsigned long long)g_pendingCapture); 
                }
                static FILE * cf = [] 
                {
                    FILE * f = std::fopen(N3DS_ROOT "/captures.bin",
"ab"); if (f) std::setvbuf(f,
nullptr,
_IOFBF,
64u << 10); return f; 
                }
                ();
                if (cf 
&& g_captureCount < 40) 
                {
                    const u8 * cb = reinterpret_cast < const u8 * > (g_tgt->frameBuf.colorBuf); const u8 * db = reinterpret_cast < const u8 * > (g_tgt->frameBuf.depthBuf);
                    static u8 copy[16384]; const size_t total = size_t(TW) * TH * 4;
                    char nm[32] = 
                    {
                        
                    } ; std::snprintf(nm,
sizeof nm,
"capture %llu",
(unsigned long long)g_pendingCapture);
                    std::fwrite(nm,
1,
sizeof nm,
cf);
                    for (size_t o = 0; o < total; o += sizeof copy) 
                    {
                        const size_t n = std::min(sizeof copy,
total - o); std::memcpy(copy,
cb + o,
n); std::fwrite(copy,
1,
n,
cf); 
                    }
                    for (size_t o = 0; o < total; o += sizeof copy) 
                    {
                        const size_t n = std::min(sizeof copy,
total - o); if (db) std::memcpy(copy,
db + o,
n); else std::memset(copy,
0,
n); std::fwrite(copy,
1,
n,
cf); 
                    }
                    std::fflush(cf); g_captureCount++;
                    n3dsLog("[capture] saved frame %llu (%u this session, fog range %.0f)\n",
(unsigned long long)g_pendingCapture,
g_captureCount,
g_depthVisibility.fogRange);
                    if (g_captureCount <= 1) 
                    {
                        static FILE * tf = [] 
                        {
                            FILE * f = std::fopen(N3DS_ROOT "/texdump.bin",
"ab"); if (f) std::setvbuf(f,
nullptr,
_IOFBF,
64u << 10); return f; 
                        }
                        ();
                        const u64 t0 = svcGetSystemTick();
                        const size_t nt = g_r->dumpTextures(tf,
g_pendingCapture);
                        n3dsLog("[capture] texture dump: %lu textures in %.0f ms\n",
(unsigned long)nt,
double(svcGetSystemTick() - t0) * 1000.0 / SYSCLOCK_ARM11);
                        
                    }
                    
                }
                g_pendingCapture = 0;
                
            }
            if (!g_pendingShot || !g_shots) return;
            static const bool routeShot = std::getenv("N3DS_UI_ROUTE_SHOT") != nullptr;
            if (routeShot) if (C3D_RenderTarget * b = g_r->uiRouteTarget()) 
            {
                static FILE * rf = std::fopen(N3DS_ROOT "/ui_route_shots.bin",
"wb");
                if (rf) 
                {
                    u8 * bb = reinterpret_cast < u8 * > (b->frameBuf.colorBuf); GSPGPU_InvalidateDataCache(bb,
240 * 320 * 4);
                    char nm[32] = 
                    {
                        
                    } ; std::snprintf(nm,
sizeof nm,
"bottom %llu",
(unsigned long long)g_pendingShot);
                    std::fwrite(nm,
1,
sizeof nm,
rf); std::fwrite(bb,
1,
size_t(240) * 320 * 4,
rf); std::fflush(rf); 
                }
                
            }
            u8 * buf = reinterpret_cast < u8 * > (g_tgt->frameBuf.colorBuf);
            GSPGPU_InvalidateDataCache(buf,
TW * TH * 4);
            char name[32] = 
            {
                
            } ;
            std::snprintf(name,
sizeof name,
"frame %llu",
(unsigned long long)g_pendingShot);
            std::fwrite(name,
1,
sizeof name,
g_shots);
            std::fwrite(buf,
1,
size_t(TW) * TH * 4,
g_shots);
            std::fflush(g_shots);
            n3dsLog("[3ds] saved %s\n",
name);
            g_pendingShot = 0;
            
        }
        );
        g_r->fifo().setGuestArrayReader([](uint32_t addr,
uint32_t size) 
        {
            return g_mem.read(addr,
size); 
        }
        );
        t2p::g_picaDepthFold = g_tuning.foldDepth;
        n3dsLog("[perf] depth_fold %d\n",
int(g_tuning.foldDepth));
        t2p::g_picaDepthReverse = g_tuning.reverseDepth;
        n3dsLog("[perf] depth_reverse %d\n",
int(g_tuning.reverseDepth));
        t2p::g_picaForceZDepth = g_tuning.useDepthZ;
        n3dsLog("[perf] depth_z %d%s\n",
int(g_tuning.useDepthZ),
g_tuning.useDepthZ?" (EXPERIMENT: Z depth everywhere, fog off)":"");
        n3dsLog("[build-fix] console-return 2026-11-16: DMA boundary handoff, live map settings, matrix validity, draw-state capture\n");
        if(g_tuning.readyMaterials)
        {
            sPreparedMaterialsEnabled = sPreparedMaterials.open(N3DS_ROOT "/ww_materials.bin");
            if(sPreparedMaterialsEnabled)sMaterialPrograms.setBudget(2u << 20);
            
        }
        #if defined(N3DS_EVENT_INDIRECT) && N3DS_EVENT_INDIRECT
n3dsLog("[event-storage] indirect 1 event %lu state %lu bytes\n",
(unsigned long)sizeof(t2p::RecordedEvent3ds),
(unsigned long)sizeof(t2p::DeferredDrawState));
        #else
n3dsLog("[event-storage] indirect 0 event %lu state %lu bytes\n",
(unsigned long)sizeof(t2p::RecordedEvent3ds),
(unsigned long)sizeof(t2p::DeferredDrawState));
        #endif
n3dsLog("[material-pack] requested %d loaded %d programs %lu\n",
int(g_tuning.readyMaterials),
int(sPreparedMaterialsEnabled),
(unsigned long)sPreparedMaterials.programs());
        g_n3dsMaterialIdentity = g_tuning.materialIdsOnModels || sPreparedMaterialsEnabled;
        g_n3dsModelDirect = g_tuning.directModels;
        n3dsLog("[model] direct %d\n",
int(g_tuning.directModels));
        g_n3dsMaterialRecords = g_tuning.recordMaterials;
        n3dsLog("[model] material_records %d\n",
int(g_tuning.recordMaterials));
        g_n3dsModelRecords = g_tuning.recordModels;
        n3dsLog("[model] model_records %d\n",
int(g_tuning.recordModels));
        t2p::g_sharedMaterialVectors = g_tuning.shareMaterials;
        n3dsLog("[model] shared_materials %d\n",
int(g_tuning.shareMaterials));
        t2p::g_uniformSections = g_tuning.sectionUniforms;
        t2p::g_stateCpIncremental = g_tuning.incrementalCpState;
        n3dsLog("[state] cp incremental %d\n",
int(g_tuning.incrementalCpState));
        t2p::g_uniformPieces = g_tuning.pieceUniforms;
        n3dsLog("[uniform] pieces %d\n",
int(g_tuning.pieceUniforms));
        t2p::g_compactPoseStorage = g_tuning.compactPoses;
        n3dsLog("[model] compact_pose %d\n",
int(g_tuning.compactPoses));
        if (g_tuning.packedTextures) 
        {
            const bool verify = std::getenv("N3DS_TEXPACK_VERIFY") != nullptr;
            bool ok = g_r->loadTexturePack(g_tuning.splitTextures?N3DS_ROOT "/ww_texpack":N3DS_ROOT "/ww_texpack.bin",
verify,
g_tuning.splitTextures);
            if (!ok 
&& g_tuning.splitTextures) 
            {
                ok = g_r->loadTexturePack(N3DS_ROOT "/ww_texpack.bin",
verify,
false);
                if (ok) 
                {
                    g_tuning.splitTextures = false;
                    n3dsLog("[texpack] no split pack: using ww_texpack.bin\n");
                    
                }
                
            }
            n3dsLog("[texstore] split %d page 8 KB\n",
int(g_tuning.splitTextures));
            n3dsLog("[texpack] %s: %lu entries%s\n",
ok ? "loaded" : "not loaded (missing or built with other options): textures are converted here",
(unsigned long)g_r->texturePackEntries(),
verify ? ", VERIFY (every hit is converted too)" : "");
            
        } else n3dsLog("[texpack] off (texture_pack=0)\n");
        g_r->setDirectEvents(g_tuning.eventDirect || g_tuning.directModels,
g_tuning.directModels?512:g_tuning.directSlotCount);
        g_r->setDirectGroups(g_tuning.directModels);
        g_r->setDirectSnapshots(g_tuning.snapshotDirect);
        n3dsLog("[direct-snapshot] enabled %d\n",
int(g_tuning.snapshotDirect));
        n3dsLog("[direct] direct_events %d slots %d\n",
int(g_tuning.eventDirect),
g_tuning.directSlotCount);
        g_r->setModelPrograms(g_tuning.programsForModels);
        n3dsLog("[model] model_programs %d\n",
int(g_tuning.programsForModels));
        g_r->setUniformDelta(g_tuning.deltaUniforms);
        n3dsLog("[model] uniform_delta %d\n",
int(g_tuning.deltaUniforms));
        if(g_tuning.readyDraw)n3dsLog("[prepared] draw commands loaded %d\n",
int(g_r->backend().loadPreparedDraw(N3DS_ROOT "/ww_commands.cwcp")));
        g_r->setCommandBlocks(g_tuning.blockCommands);
        n3dsLog("[model] command_blocks %d\n",
int(g_tuning.blockCommands));
        g_r->setRigidClip(g_tuning.clipRigid);
        n3dsLog("[clip] rigid_clip %d\n",
int(g_tuning.clipRigid));
        g_r->backend().setCopyReuse(g_tuning.reuseCopies);
        n3dsLog("[copy-reuse] enabled %d\n",
int(g_tuning.reuseCopies));
        g_r->setPlanReuse(g_tuning.reusePlans);
        g_r->setPlanDelta(g_tuning.reusePlans 
&& g_tuning.deltaPlans,
std::getenv("N3DS_PLAN_DELTA_VERIFY") != nullptr);
        n3dsLog("[perf] plan_delta %d\n",
int(g_tuning.reusePlans 
&& g_tuning.deltaPlans));
        n3ds_ui::panelOn.store(g_tuning.lowerScreen,
std::memory_order_release);
        g_r->setUiRoute(g_tuning.lowerScreen 
&& g_tuning.mapOnLower,
routeOwnerChanged);
        n3dsLog("[perf] ui_map_bottom %d\n",
int(g_tuning.mapOnLower 
&& g_tuning.lowerScreen));
        g_r->setStateNotes(!g_tuning.slimState);
        g_r->setRetainGeometry(g_tuning.keepGeometry);
        if(g_tuning.keepGeometry 
&& g_tuning.stayGeometry)g_r->setResidentGeometry(true,
size_t(g_tuning.residentLimitKb) << 10);
        
        {
            const char * v = getenv("N3DS_DRAW_REPLAY");
            const bool on = v?( * v == '1'):g_tuning.replayDraws;
            if(g_tuning.keepGeometry 
&& g_tuning.stayGeometry 
&& on)g_r->setDrawReplay(true,
size_t(std::min(g_tuning.replayLimitKb,
1536)) << 10);
            n3dsLog("[replay] draw_replay %d\n",
int(g_r->replayDraws()));
            
        }
        
        {
            const char * v = getenv("N3DS_DRAW_RECORD");
            g_r->setDrawRecord(v?( * v == '1'):g_tuning.recordDraws);
            n3dsLog("[replay] draw_record %d\n",
int(g_r->recordDraws()));
            
        }
        
        {
            const char * v = getenv("N3DS_FLUSH_ALL");
            g_r->setFlushAllAtEnd(v?( * v == '1'):g_tuning.flushWholeFrame);
            n3dsLog("[flush] frame_flush_all %d\n",
int(g_r->flushAllAtEnd()));
            
        }
        n3dsLog("[perf] resident_geometry %d budget %d KB (linear free %lu KB)\n",
int(g_tuning.keepGeometry 
&& g_tuning.stayGeometry),
g_tuning.residentLimitKb,
(unsigned long)(linearSpaceFree() >> 10));
        g_r->setStateInPlace(g_tuning.inplaceState);
        g_r->setStatePool(g_tuning.poolState);
        g_r->setVertexPoolFifo(g_tuning.fifoVertexPool);
        g_r->setFogPreparation(g_tuning.prepareFog);
        g_r->fifo().setPrimitiveRuns(g_tuning.coalesceVertices);
        g_r->fifo().setGeometryCache(g_tuning.cacheGeometry 
&& !g_pipeline);
        g_r->fifo().setFastAttributeDecode(g_tuning.decodeVertices);
        g_r->fifo().setCachedVertexPlan(g_tuning.planVertices);
        g_r->fifo().setAuroraMode(true);
        g_r->fifo().setUncheckedArrays(true);
        g_r->fifo().setStopAtDrawDone(true);
        if (n3dsRenderThreadActive() 
&& !g_pipeline) 
        {
            if (!g_mem.snapshot.init(3u << 20)) n3dsLog("[render-thread] no snapshot memory; frames will stay synchronous\n");
            g_r->fifo().setSink(&g_ranges);
            g_r->setCacheVerifiedHook([] 
            {
                g_mem.verifying = false; 
            }
            );
            
        }
        
        {
            std::vector < std::string > names;
            if (DIR * d = opendir(N3DS_ROOT)) 
            {
                while (dirent * e = readdir(d)) 
                {
                    const std::string n = e->d_name;
                    if (n.size() >= 13 
&& n.compare(0,
9,
"ww_bundle") == 0 
&& n.compare(n.size() - 4,
4,
".bin") == 0) names.push_back(n);
                    
                }
                closedir(d);
                
            }
            if (const char * v = getenv("N3DS_NO_BUNDLES"); v 
&& * v == '1') 
            {
                names.clear();
                g_noBundleWrites = true;
                n3dsLog("[3ds] material bundles: off (N3DS_NO_BUNDLES)\n");
                
            }
            std::sort(names.begin(),
names.end(),
[](const std::string& a,
const std::string& b) 
            {
                const bool baseA = a == "ww_bundle.bin",
baseB = b == "ww_bundle.bin";
                if (baseA != baseB) return !baseA;
                return a < b;
                
            }
            );
            for (const std::string& n : names) 
            {
                std::vector < uint8_t > b;
                if (FILE * bf = std::fopen((std::string(N3DS_ROOT "/") + n).c_str(),
"rb")) 
                {
                    uint8_t tmp[1 << 14];
                    size_t k;
                    while ((k = std::fread(tmp,
1,
sizeof tmp,
bf)) > 0) b.insert(b.end(),
tmp,
tmp + k);
                    std::fclose(bf);
                    
                }
                std::string err;
                const bool ok = !b.empty() 
&& g_r->loadMaterialBundle(b.data(),
b.size(),
&err);
                if (ok 
&& n == "ww_bundle_misses.bin") g_r->loadMissBundle(b.data(),
b.size());
                n3dsLog("[3ds] material bundle %s: %s (%zu entries in all) %s\n",
n.c_str(),
ok ? "loaded" : b.empty() ? "empty" : "refused",
g_r->bundleEntries(),
err.c_str());
                
            }
            g_missesWritten = g_r->missEntries();
            g_r->recordBundleMisses(true);
            
        }
        n3ds_warm::begin( * g_r);
        return 1;
        
    }
    static unsigned g_processCalls = 0;
    uint32_t n3dsGpuProcessLocal(const uint8_t * data,
uint32_t size,
int * drawDone) 
    {
        const u64 t0 = svcGetSystemTick();
        g_processCalls++;
        size_t used = g_r->process(data,
size);
        g_processTicks += svcGetSystemTick() - t0;
         * drawDone = g_r->fifo().takeDrawDone() ? 1 : 0;
        if (used == 0 || (used < size 
&& ! * drawDone)) used = size;
        return uint32_t(used);
        
    }
    static LightEvent g_decodeWake;
    static std::atomic < bool > g_decodeAsleep
    {
        false
    } ;
    static std::atomic < bool > g_decodeStarted
    {
        false
    } ;
    static t2p::GxFifo * g_decodeFifo = nullptr;
    extern "C++" void JKRPcBeginHostAlloc();
    static void decodeHelperMain(void * ) 
    {
        JKRPcBeginHostAlloc();
        for (;;) 
        {
            t2p::GxFifo * fifo = g_decodeFifo;
            if (!fifo) 
            {
                LightEvent_Wait(&g_decodeWake);
                continue;
                
            }
            while (fifo->runDecodeJob()) 
            {
                
            }
            bool got = false;
            for (int spin = 0; spin < 4000 
&& !got; spin++) got = fifo->runDecodeJob();
            if (got) continue;
            g_decodeAsleep.store(true,
std::memory_order_release);
            if (fifo->runDecodeJob()) 
            {
                g_decodeAsleep.store(false,
std::memory_order_release);
                continue;
                
            }
            LightEvent_Wait(&g_decodeWake);
            g_decodeAsleep.store(false,
std::memory_order_release);
            
        }
        
    }
    static void decodeHelperWake() 
    {
        // Always signal. Checking asleep first can drop a wake between the
        // empty-queue recheck and Wait, which hangs finishDecodes forever.
        LightEvent_Signal(&g_decodeWake);
        
    }
    static void startDecodeHelper(t2p::GxFifo& fifo) 
    {
        if (!g_tuning.helperDecode) return;
        if (g_decodeStarted.load(std::memory_order_acquire)) return;
        if (!g_tuning.commandThread) 
        {
            n3dsLog("[threads] decode_helper needs fifo_thread=1: off\n");
            return;
            
        }
        LightEvent_Init(&g_decodeWake,
RESET_ONESHOT);
        g_decodeFifo = &fifo;
        fifo.setDeferredDecode(true,
decodeHelperWake);
        if (const char * m = std::getenv("N3DS_DEFER_MIN")) g_deferMin = uint32_t(std::atoi(m));
        s32 prio = 0x30;
        svcGetThreadPriority(&prio,
CUR_THREAD_HANDLE);
        int core = g_tuning.helperCore;
        if (const char * pv = std::getenv("N3DS_DECODE_PRIO")) prio = s32(std::strtol(pv,
nullptr,
0));
        // Stay off the fifo and render cores so a busy-wait drain cannot starve.
        if (core == g_tuning.commandCore || core == g_tuning.spareCore) 
        {
            n3dsLog("[threads] decode helper: core %d busy with fifo/render, using 0\n",
core);
            core = 0;
            
        }
        if (core == 1) 
        {
            const Result r = APT_SetAppCpuTimeLimit(80);
            if (R_FAILED(r)) 
            {
                n3dsLog("[threads] decode helper: APT core1 limit failed %08lx, using 0\n",
(unsigned long)r);
                core = 0;
                
            }
            
        }
        Thread t = threadCreate(decodeHelperMain,
nullptr,
32u << 10,
prio,
core,
false);
        if (!t 
&& core != 0) 
        {
            n3dsLog("[threads] decode helper: core %d refused, retrying on 0\n",
core);
            core = 0;
            t = threadCreate(decodeHelperMain,
nullptr,
32u << 10,
prio,
core,
false);
            
        }
        if (!t) 
        {
            fifo.setDeferredDecode(false);
            g_decodeFifo = nullptr;
            n3dsLog("[threads] decode helper: thread create refused: off\n");
            return;
            
        }
        g_decodeStarted.store(true,
std::memory_order_release);
        n3dsLog("[threads] decode helper prio 0x%lx on core %d\n",
(long)prio,
core);
        
    }
    static void finishRecording(t2p::GxFifo& fifo) 
    {
        fifo.finishDecodes();
        const t2p::GxRegs& R = fifo.regs();
        g_xfb.clear();
        auto bpw = [&](uint32_t id,
uint32_t val) 
        {
            g_xfb.push_back(0x61);
            push32(g_xfb,
(id << 24) | (val & 0xFFFFFF));
            
        } ;
        const uint32_t ids[] = 
        {
            0x40, 0x41, 0x49, 0x4A, 0x4B, 0x4D
        } ;
        uint32_t saved[6];
        bool had[6];
        for (int i = 0; i < 6; i++) 
        {
            saved[i] = R.bp[ids[i]];
            had[i] = R.bpWritten[ids[i]];
            
        }
        bpw(0x40,
0x1F);
        bpw(0x41,
0x18);
        bpw(0x49,
0);
        bpw(0x4A,
639u | (479u << 10));
        bpw(0x4B,
0x00400000u >> 5);
        bpw(0x4D,
640u * 2 / 32);
        bpw(0x52,
(1u << 14) | (1u << 11));
        for (int i = 0; i < 6; i++) if (had[i]) bpw(ids[i],
saved[i]);
        const bool stop = true;
        fifo.setStopAtDrawDone(false);
        fifo.process(g_xfb.data(),
g_xfb.size());
        fifo.setStopAtDrawDone(stop);
        
    }
    void n3dsGpuEndFrameLocal(void) 
    {
        g_n3dsMark[7] = 1;
        struct StageOut 
        {
            ~StageOut() 
            {
                g_n3dsMark[7] = 0;
                
            }
            
        }
        stageOut;
        t2p::CpuFrame3ds * frame = g_pipeline ? g_submitted : nullptr;
        g_renderSlot = g_pipeline ? g_submittedSlot : 0;
        DepthVisibility3ds depthVisibility;
        if(frame) for (const auto& e:frame->events) if(e.kind == t2p::RecordedEvent3ds::Draw 
&& e.viewState().draw.z.update)
        {
            const auto& v = e.viewState().viewport;
            const auto& p = e.viewState().proj;
            depthVisibility.add(
            {
                p.m[2][2],
p.m[2][3],
p.nearZ,
p.farZ,
v.nearZ,
v.farZ,
p.ortho
            }
            );
            
        }
        unsigned depthOrtho = 0, depthPersp = 0;
        const bool depthDiag = std::getenv("N3DS_DEPTH_DIAG") != nullptr;
        if(depthDiag 
&& frame) for (const auto& e:frame->events) if(e.kind == t2p::RecordedEvent3ds::Draw 
&& e.viewState().draw.z.compare 
&& e.viewState().draw.z.func != 7) 
        {
            if(e.viewState().proj.ortho)++depthOrtho;
            else ++depthPersp;
            
        }
        if(frame 
&& g_tuning.snapshotDirect 
&& (g_frames.load() + 1) % 60 == 0)n3dsLog("[direct-snapshot] frame %lu active %u\n",
(unsigned long)(g_frames.load() + 1),
frame->directDiag[14]);
        if(frame 
&& std::getenv("N3DS_DIRECT_DIAG") 
&& (g_frames.load() + 1) % 60 == 0)
        {
            const auto * d = frame->directDiag;
            n3dsLog("[direct-reasons] frame %lu total %u route %u palette %u no-geometry %u not-requested %u no-program %u matched %u flags %u colors %u texture-address %u BP %u XF %u captured %u\n",
(unsigned long)(g_frames.load() + 1),
d[0],
d[1],
d[2],
d[3],
d[4],
d[5],
d[6],
d[7],
d[8],
d[9],
d[10],
d[11],
d[15]);
            
        }
        if (frame) 
        {
            g_r->swapRecording( * frame);
            g_processTicks = frame->processTicks;
            g_processCalls = frame->processCalls;
            for (int i=0;i<16;++i) g_t2pProf[i] = frame->prof[i];
            
        } else finishRecording(g_r->fifo());
        char gameHeaps[256] = 
        {
            
        } ;
        if (frame) std::memcpy(gameHeaps,
frame->heaps,
sizeof gameHeaps);
        else if ((g_frames.load() + 1) % 60 == 0) n3dsGameHeapReport(gameHeaps,
sizeof gameHeaps);
        g_mem.verifying = true;
        g_mem.missing = 0;
        g_mem.verifyMissing = 0;
        if (g_r3dsTrace) g_r3dsTrace("end-frame: start",
0);
        const bool overlapped = frame ? frame->frozen : n3dsRenderThreadActive() 
&& g_mem.snapshot.freeze(readableTexture,
skipSnapshots);
        g_mem.view = frame ? &frame->snapshot : nullptr;
        const uint32_t snapshotBytes = frame ? frame->snapshot.bytes() : g_mem.snapshot.bytes();
        if(frame 
&& (g_frames.load() < 5 || (g_frames.load() + 1) % 60 == 0))
        {
            const double k = 1000.0 / SYSCLOCK_ARM11;
            const uint64_t work = frame->processTicks > frame->processStagingWaitTicks?frame->processTicks - frame->processStagingWaitTicks:0;
            n3dsLog("[cpu-cost] frame %lu process-work %.2f process-staging-wait %.2f all-staging-wait %.2f handoff-wait %.2f ms\n",
(unsigned long)(g_frames.load() + 1),
work * k,
frame->processStagingWaitTicks * k,
frame->stagingWaitTicks * k,
frame->handoffWaitTicks * k);
            
        }
        g_mem.frozen = overlapped;
        if (overlapped) n3dsRenderThreadRelease();
        else if (n3dsRenderThreadActive()) ++g_snapshotFallbacks;
        const u64 r0 = svcGetSystemTick();
        const uint32_t logFrame = g_frames.load() + 1;
        const bool frameLogged = g_frameLogFrom 
&& logFrame >= g_frameLogFrom 
&& logFrame <= g_frameLogTo;
        if (g_frameLogFrom 
&& logFrame == g_frameLogFrom) g_frameLogTex = g_r->cache().stats();
        const bool queueSample = logFrame <= 5 || (logFrame <= 300 
&& logFrame % 10 == 0) || logFrame % 60 == 0 || frameLogged;
        g_r->setFrameTiming(queueSample);
        g_r->setDrawTimingEnabled(g_tuning.timeDraws || g_frames.load() < 5 || (g_frames.load() + 1) % 60 == 0 || (frameLogged 
&& g_frameLogDraws));
        const int abSide = g_tuning.autoSplit?int((logFrame / 120)&1): - 1;
        constexpr int kKnobs = 8;
        constexpr uint32_t kKnobPhase = 120;
        static const bool knobAb = std::getenv("N3DS_AB_KNOBS") != nullptr;
        const uint32_t knobPhaseNo = logFrame / kKnobPhase;
        const int knob = !knobAb ? - 1 : (knobPhaseNo & 1) ? int((knobPhaseNo / 2) % (kKnobs - 1)) + 1 : 0;
        static const char * const kKnobNames[kKnobs] = 
        {
            "defaults", "borrow off", "pack_vertex_major", "decode_overwrite", "shared_materials", 
            "uniform_delta", "state_cp_incremental", "compact_pose"
        } ;
        if (knob >= 0) 
        {
            static int cur = - 1;
            if (knob != cur) 
            {
                cur = knob;
                n3dsFastAllocBorrow(knob != 1);
                t2p::g_packVertexMajor = knob == 2 || g_tuning.vertexMajorPack;
                t2p::g_decodedInitMode = (knob == 3 || g_tuning.overwriteDecode) ? 1 : 0;
                t2p::g_sharedMaterialVectors = knob == 4 || g_tuning.shareMaterials;
                g_r->setUniformDelta(knob == 5 || g_tuning.deltaUniforms);
                t2p::g_stateCpIncremental = knob == 6 || g_tuning.incrementalCpState;
                t2p::g_compactPoseStorage = knob == 7 || g_tuning.compactPoses;
                n3dsLog("[ab-knobs] frame %lu: config %d (%s)\n",
(unsigned long)logFrame,
knob,
kKnobNames[knob]);
                
            }
            
        }
        if (g_r3dsTrace) g_r3dsTrace("end-frame: before renderFrame",
0);
        
        {
            static int cur = - 1;
            const int want = g_abDrawReuse.load();
            if (want >= 0 
&& want != cur) 
            {
                cur = want;
                g_r->setDrawReuse(want != 0,
size_t(g_tuning.reuseDrawKb) << 10);
                
            }
            
        }
        const t2p::FrameReport F = g_r->renderFrame(g_r->framesReady() == 0);
        depthVisibility.fogRange = F.fogRange;
        g_depthVisibility = depthVisibility;
        if(depthDiag 
&& (g_frames.load() + 1) % 60 == 0) n3dsLog("[depth-order] frame %lu fog-range %.1f tested ortho %u perspective %u\n",
(unsigned long)(g_frames.load() + 1),
F.fogRange,
depthOrtho,
depthPersp);
        if (g_r3dsTrace) g_r3dsTrace("end-frame: after renderFrame",
0);
        const uint64_t finishTick = queueSample?svcGetSystemTick():0;
        if (g_measurePresent) C3Di_RenderQueueWaitDone();
        const uint64_t finishWaitTicks = queueSample?svcGetSystemTick() - finishTick:0;
        const double renderMs = double(svcGetSystemTick() - r0) * 1000.0 / SYSCLOCK_ARM11, processMs = double(g_processTicks) * 1000.0 / SYSCLOCK_ARM11;
        if(F.packetJobs 
&& (g_frames.load() < 5 || (g_frames.load() + 1) % 60 == 0))n3dsLog("[packet-worker] frame %lu jobs %u wait %.2f ms checks %u mismatches %u\n",
(unsigned long)(g_frames.load() + 1),
F.packetJobs,
F.packetWaitTicks * 1000.0 / SYSCLOCK_ARM11,
F.packetChecks,
F.packetMismatches);
        const u64 presented = svcGetSystemTick();
        const double presentMs = g_lastPresent ? double(presented - g_lastPresent) * 1000.0 / SYSCLOCK_ARM11 : 0;
        g_lastPresent = presented;
        if (g_r3dsTrace) g_r3dsTrace("end-frame: bottom ui",
0);
        // Skip the bottom panel during first-boot warm; the cover screens own
        // both displays and the panel only steals CPU from cache building.
        if(g_tuning.lowerScreen 
&& !n3ds_warm::covering()) g_bottomUi.update((unsigned long)(g_frames.load() + 1),
g_presentCount ? g_presentSum / g_presentCount : presentMs);
        n3ds_warm::afterFrame( * g_r,
presentMs,
F.draws);
        
        {
            static double abRender[2], abPresent[2], abProcess[2], abBuild[2], abPlan[2], abDraw[2];
            static uint32_t abFrames[2], abTimed[2];
            if(abSide >= 0 
&& F.draws >= 100 
&& logFrame % 120 >= 10 
&& presentMs > 0)
            {
                abRender[abSide] += renderMs;
                abPresent[abSide] += presentMs;
                abProcess[abSide] += processMs;
                ++abFrames[abSide];
                if(logFrame % 60 == 0)
                {
                    abBuild[abSide] += F.buildMs;
                    abPlan[abSide] += F.planMs;
                    abDraw[abSide] += F.drawMs;
                    ++abTimed[abSide];
                    
                }
                
            }
            if(abSide >= 0 
&& logFrame % 1200 == 0)
            {
                for (int s=0;s<2;s++)
                {
                    const double n = abFrames[s]?abFrames[s]:1, m = abTimed[s]?abTimed[s]:1;
                    n3dsLog("[ab] frame %lu side %d (%s): %u frames render %.2f present %.2f (%.2f fps) process %.2f ms; %u timed: build %.2f plan %.2f draw %.2f ms\n",
(unsigned long)logFrame,
s,
s?"block B":"block A",
abFrames[s],
abRender[s] / n,
abPresent[s] / n,
abPresent[s] > 0?1000.0 * n / abPresent[s]:0.0,
abProcess[s] / n,
abTimed[s],
abBuild[s] / m,
abPlan[s] / m,
abDraw[s] / m);
                    
                }
                
            }
            
        }
        if (knob >= 0) 
        {
            struct Acc 
            {
                double p = 0, r = 0, q = 0;
                uint32_t n = 0;
                
            } ;
            static Acc cur, prevDefaults;
            static uint32_t curPhase = ~0u;
            static int curKnob = - 1;
            static Acc candSide[kKnobs], baseSide[kKnobs];
            static uint32_t pairs[kKnobs];
            if (knobPhaseNo != curPhase) 
            {
                if (curKnob == 0) prevDefaults = cur;
                else if (curKnob > 0 
&& cur.n >= 20 
&& prevDefaults.n >= 20) 
                {
                    Acc& c = candSide[curKnob];
                    Acc& d = baseSide[curKnob];
                    c.p += cur.p / cur.n;
                    c.r += cur.r / cur.n;
                    c.q += cur.q / cur.n;
                    c.n += cur.n;
                    d.p += prevDefaults.p / prevDefaults.n;
                    d.r += prevDefaults.r / prevDefaults.n;
                    d.q += prevDefaults.q / prevDefaults.n;
                    d.n += prevDefaults.n;
                    ++pairs[curKnob];
                    
                }
                if (curKnob > 0) prevDefaults = Acc
                {
                    
                } ;
                cur = Acc
                {
                    
                } ;
                curPhase = knobPhaseNo;
                curKnob = knob;
                
            }
            if (F.draws >= 100 
&& logFrame % kKnobPhase >= 10 
&& presentMs > 0) 
            {
                cur.p += presentMs;
                cur.r += renderMs;
                cur.q += processMs;
                ++cur.n;
                
            }
            if (logFrame % (kKnobPhase * 2 * (kKnobs - 1)) == 0) for (int k = 1; k < kKnobs; k++) 
            {
                if (!pairs[k]) continue;
                const double m = pairs[k];
                n3dsLog("[ab-knobs] frame %lu %s vs defaults, %u pairs (%u/%u frames): present %.2f/%.2f ms (%+.1f%%) render %.2f/%.2f process %.2f/%.2f ms\n",
                (unsigned long)logFrame,
kKnobNames[k],
pairs[k],
candSide[k].n,
baseSide[k].n,
candSide[k].p / m,
baseSide[k].p / m,
                baseSide[k].p > 0 ? 100.0 * (candSide[k].p - baseSide[k].p) / baseSide[k].p : 0.0,
candSide[k].r / m,
baseSide[k].r / m,
candSide[k].q / m,
baseSide[k].q / m);
                
            }
            
        }
        if(presentMs > 0)
        {
            g_presentSum += presentMs;
            ++g_presentCount;
            g_presentMin = std::min(g_presentMin,
presentMs);
            g_presentMax = std::max(g_presentMax,
presentMs);
            
        }
        g_mem.frozen = false;
        if (g_r3dsTrace) g_r3dsTrace("end-frame: after bottom ui",
0);
        if (frame) 
        {
            g_r->swapRecording( * frame);
            frame->snapshot.reset();
            
        }
        if (g_r3dsTrace) g_r3dsTrace("end-frame: recording swapped",
0);
        else g_mem.snapshot.reset();
        g_mem.view = nullptr;
        if (g_mem.missing) n3dsLog("[render-thread] ERROR missing current texture snapshots %lu\n",
(unsigned long)g_mem.missing);
        g_processTicks = 0;
        const unsigned processCalls = g_processCalls;
        g_processCalls = 0;
        g_frames++;
        if(g_frames % 60 == 0 
&& g_presentCount)
        {
            n3dsLog("[present] frame %lu: %u intervals mean %.1f ms (%.2f fps) min %.1f max %.1f fence %d pipeline %d\n",
            (unsigned long)g_frames.load(),
g_presentCount,
g_presentSum / g_presentCount,
1000.0 * g_presentCount / g_presentSum,
g_presentMin,
g_presentMax,
int(g_measurePresent),
int(g_pipeline));
            g_presentSum = 0;
            g_presentCount = 0;
            g_presentMin = 1e30;
            g_presentMax = 0;
            
        }
        for (uint64_t f : g_shotFrames) if (f == g_frames 
&& F.rendered) g_pendingShot = f;
        
        {
            static const unsigned long capAt = [] 
            {
                const char * s = std::getenv("N3DS_CAPTURE_AT");
                return s ? std::strtoul(s,
nullptr,
10) : 0ul;
                
            }
            ();
            if (capAt 
&& g_frames == capAt) g_captureRequest.store(true);
            
        }
        if (F.rendered 
&& g_captureRequest.load(std::memory_order_relaxed)) 
        {
            g_captureRequest.store(false);
            g_pendingCapture = g_frames;
            
        }
        if (g_t2pProfOn 
&& g_frames % 60 == 0) 
        {
            const double k = 1000.0 * g_t2pProfPeriod / SYSCLOCK_ARM11;
            
            {
                n3dsLog("[prof] state decodes per frame: new batch %llu, matrix growth %llu\n",
(unsigned long long)g_n3dsDecodeCount[0],
(unsigned long long)g_n3dsDecodeCount[1]);
                g_n3dsDecodeCount[0] = g_n3dsDecodeCount[1] = 0;
                
            }
            n3dsLog("[prof-frontend] per frame: pre-append %.2f close+charge %.2f event-create %.2f attribute-setup %.2f retention-offer %.2f ms\n",
g_t2pProf[9] * k,
g_t2pProf[12] * k,
g_t2pProf[13] * k,
g_t2pProf[14] * k,
g_t2pProf[7] * k);
            n3dsLog("[prof] per frame (1/%lu sampled): vertex decode %.1f ms (%llu verts), sink %.1f ms (state decode %.1f, draw copy %.1f, append %.1f); process %.1f ms\n",
(unsigned long)g_t2pProfPeriod,
g_t2pProf[0] * k,
(unsigned long long)(g_t2pProf[2] * g_t2pProfPeriod),
            g_t2pProf[1] * k,
g_t2pProf[3] * k,
g_t2pProf[4] * k,
g_t2pProf[5] * k,
double(processMs));
            
        }
        if (g_t2pProfOn 
&& g_frames % 60 == 0) 
        {
            const double k = 1000.0 * g_t2pProfPeriod / SYSCLOCK_ARM11;
            n3dsLog("[prof-state] per frame: copy-prev %.1f tev %.1f vertex-pipe %.1f uniforms %.1f pixel+viewport %.1f textures %.1f ms\n",
g_recStateProf[5] * k,
g_recStateProf[0] * k,
g_recStateProf[1] * k,
g_recStateProf[2] * k,
g_recStateProf[3] * k,
g_recStateProf[4] * k);
            
        }
        if(g_t2pProfOn) for (uint64_t& v : g_t2pProf) v = 0;
        if (frame 
&& g_frames % 60 == 0)
        n3dsLog("[pipeline] frame %lu: estimated staging %lu KB fence %d heap checks %u peak in use %lu KB min sbrk room %ld KB compact %lu KB/%u\n",
(unsigned long)g_frames.load(),
(unsigned long)(frame->stagedBytes >> 10),
int(frame->stagingFence),
frame->heapChecks,
(unsigned long)(frame->heapPeakUse >> 10),
g_minBrkRoom == ~size_t(0)? - 1L:long(g_minBrkRoom >> 10),
(unsigned long)(frame->compactSavedBytes >> 10),
frame->compactBatches);
        if (g_decodeFifo) 
        {
            static unsigned long p[8];
            unsigned long d[8];
            for (int k=0;k<8;k++)
            {
                d[k] = g_deferWhy[k] - p[k];
                p[k] = g_deferWhy[k];
                
            }
            n3dsLog("[decode-why] per 60 frames: fast draws %lu, queued %lu (below minimum %lu), run by worker itself %lu; attribute-vertices all %lu queued %lu; min %lu\n",
d[0],
d[5],
d[3],
d[1],
d[6],
d[7],
(unsigned long)g_deferMin);
            
        }
        if (g_decodeFifo) 
        {
            static uint64_t pj = 0, ps = 0, pd = 0;
            const uint64_t j = g_decodeFifo->deferredJobs(), sf = g_decodeFifo->deferredSelfRun(), dr = g_decodeFifo->deferredDrains();
            n3dsLog("[decode-helper] since last: jobs %lu, run by the worker itself %lu, drains with work pending %lu\n",
(unsigned long)(j - pj),
(unsigned long)(sf - ps),
(unsigned long)(dr - pd));
            pj = j;
            ps = sf;
            pd = dr;
            
        }
        if (frame 
&& g_frames % 60 == 0) g_minBrkRoom = ~size_t(0);
        
        {
            static const bool dlCensus = std::getenv("N3DS_DL_CENSUS") != nullptr;
            if (dlCensus 
&& g_frames == 600) g_n3dsDlCensusOpen = 1;
            if (dlCensus 
&& g_frames == 700) 
            {
                g_n3dsDlCensusOpen = 0;
                n3dsDlCensusWrite(N3DS_ROOT "/dlcensus.txt");
                n3dsLog("[dl-census] written\n");
                
            }
            
        }
        if(g_frames % 120 == 0)n3dsAudioHealth();
        if(g_tuning.packedTextures 
&& g_tuning.splitTextures 
&& g_frames % 60 == 0)
        {
            const auto& s = g_r->textureStoreStats();
            n3dsLog("[texstore] reads %llu bytes %llu opens %llu hits %llu\n",
(unsigned long long)s.reads,
(unsigned long long)s.bytes,
(unsigned long long)s.opens,
(unsigned long long)s.hits);
            
        }
        if(g_tuning.clipRigid 
&& g_frames % 60 == 0)n3dsLog("[clip] frame %lu tests %u rejected %u\n",
(unsigned long)g_frames.load(),
F.clipTests,
F.clipCulled);
        if (g_frames % 600 == 0) 
        {
            const t2p::TexCacheStats ts = g_r->cache().stats();
            const double k = 1000.0 / SYSCLOCK_ARM11;
            n3dsLog("[texpack] frame %lu: pack hits %llu (read %.0f ms, %.1f MB), converted %llu (%.0f ms), read failures %llu, verified %llu bad %llu%s\n",
(unsigned long)g_frames.load(),
            (unsigned long long)ts.packHits,
ts.packReadTicks * k,
double(ts.packReadBytes) / 1e6,
(unsigned long long)(ts.misses - ts.packHits),
ts.convTicks * k,
(unsigned long long)ts.packReadFail,
            (unsigned long long)ts.packVerified,
(unsigned long long)ts.packVerifyBad,
ts.packSwitchedOff ? "; pack SWITCHED OFF (slower than converting)" : "");
            n3dsLog("[texpack-cost] cumulative: SD seek/read %.1f ms, output allocation %.1f ms\n",
ts.packIoTicks * k,
ts.packAllocTicks * k);
            
        }
        
        {
            static const bool texProf = [] 
            {
                const char * v = std::getenv("N3DS_PROF_TEX");
                return v 
&& v[0] == '1';
                
            }
            ();
            if (texProf 
&& g_frames % 60 == 0) 
            {
                const double k = 1000.0 / SYSCLOCK_ARM11;
                uint64_t * t = t2p::g_texProf;
                n3dsLog("[texprof] frame %lu: %llu conversions (%llu lossy) total %.1f ms: decode %.1f, exact pick %.1f, lossy trial %.1f; upload %.1f ms\n",
(unsigned long)g_frames.load(),
                (unsigned long long)t[5],
(unsigned long long)t[6],
t[0] * k,
t[1] * k,
t[2] * k,
t[3] * k,
t[4] * k);
                for (int i = 0; i < 8; i++) t[i] = 0;
                
            }
            
        }
        if (g_gxOriginProf 
&& g_frames % 60 == 0) 
        {
            const double k = 1000.0 / SYSCLOCK_ARM11 / 60.0;
            for (int o = 0; o < 2; o++) 
            {
                const uint64_t * c = g_gxOriginCount[o];
                n3dsLog("[origin] frame %lu %s: process %.1f ms/frame, %llu calls, %llu KB, draws %llu, verts %llu, BP %llu CP %llu XF %llu (%llu words) idxXF %llu per frame\n",
                (unsigned long)g_frames.load(),
o ? "displaylist" : "typed",
g_originTicks[o] * k,
(unsigned long long)(g_originCalls[o] / 60),
                (unsigned long long)(g_originBytes[o] / 60 >> 10),
(unsigned long long)(g_originDraws[o] / 60),
(unsigned long long)(g_originVerts[o] / 60),
                (unsigned long long)(c[0] / 60),
(unsigned long long)(c[1] / 60),
(unsigned long long)(c[2] / 60),
(unsigned long long)(c[3] / 60),
(unsigned long long)(c[4] / 60));
                g_originTicks[o] = g_originBytes[o] = g_originDraws[o] = g_originVerts[o] = g_originCalls[o] = 0;
                for (int i = 0; i < 5; i++) g_gxOriginCount[o][i] = 0;
                
            }
            
        }
        if (g_frames % 60 == 0) 
        {
            extern u32 __ctru_heap_size;
            const struct mallinfo mi = mallinfo();
            n3dsLog("[mem] host heap: in use %u KB, taken from the system %u KB of %lu KB (sbrk room %u KB); %s\n",
unsigned(mi.uordblks >> 10),
unsigned(mi.arena >> 10),
            (unsigned long)(__ctru_heap_size >> 10),
unsigned(brkRoom() >> 10),
gameHeaps);
            if (g_r 
&& g_r->replayDraws()) 
            {
                const size_t freeB = brkRoom() + size_t(mi.fordblks), used = g_r->drawReplayBytes(), keep = 7u << 20;
                const size_t want = std::max < size_t > (512u << 10,
std::min < size_t > (size_t(g_tuning.replayLimitKb) << 10,
freeB > keep ? used + (freeB - keep) : used / 2));
                g_r->setDrawReplayBudget(want);
                n3dsLog("[replay] budget %u KB (entries %u KB, heap free %u KB)\n",
unsigned(want >> 10),
unsigned(used >> 10),
unsigned(freeB >> 10));
                
            }
            n3dsBigAllocReport();
            if (g_r) n3dsLog("[copy] rendered-texture allocations failed so far: %u (copy lost, clear kept); VRAM free %u KB, render-target pool %u, rendered textures %u holding %u KB VRAM, reused %llu\n",
unsigned(g_r->backend().stats().copyAllocFails),
unsigned(vramSpaceFree() / 1024),
unsigned(g_r->backend().rtPoolSize()),
unsigned(g_r->backend().stats().rtLive),
unsigned(g_r->backend().stats().rtVramBytes / 1024),
(unsigned long long)g_r->backend().copyReuses());
            n3dsLog("[fifo] drain stalls %u\n",
n3dsFifoDrainStalls);
            
        }
        if (frameLogged 
&& g_frameLogCount < kFrameLogMax) 
        {
            const t2p::TexCacheStats ts = g_r->cache().stats();
            FrameSample& fs = g_frameLog[g_frameLogCount++];
            fs = FrameSample
            {
                logFrame, unsigned(F.drawn), unsigned(F.gxDraws), uint32_t(ts.misses - g_frameLogTex.misses), uint32_t(ts.conversions - g_frameLogTex.conversions), 
                uint32_t(ts.evicted - g_frameLogTex.evicted), uint32_t(ts.bytes >> 10), float(processMs), float(renderMs), float(F.buildMs), float(F.planMs), float(F.drawMs), 
                float(presentMs), presented
            } ;
            g_frameLogTex = ts;
            
        }
        if (g_frameLogFrom 
&& logFrame == g_frameLogTo) 
        {
            for (uint32_t i = 0; i < g_frameLogCount; i++) 
            {
                const FrameSample& fs = g_frameLog[i];
                n3dsLog("[frame] %lu draws %lu gx %lu process %.2f render %.2f build %.2f plan %.2f draw %.2f present %.2f tick %llu tex miss %lu conv %lu evict %lu KB %lu\n",
                (unsigned long)fs.frame,
(unsigned long)fs.drawn,
(unsigned long)fs.gx,
fs.process,
fs.render,
fs.build,
fs.plan,
fs.draw,
fs.present,
                (unsigned long long)fs.presentTick,
(unsigned long)fs.texMiss,
(unsigned long)fs.texConv,
(unsigned long)fs.texEvict,
(unsigned long)fs.texKB);
                
            }
            g_frameLogCount = 0;
            
        }
        if (g_frames <= 5 || (g_frames <= 300 
&& g_frames % 10 == 0) || g_frames % 60 == 0)
        n3dsLog("[3ds] frame %llu: %u/%u draws (gx %u) copies %u/%u  process %.0f ms (%u calls) render %.0f ms (build %.0f plan %.0f draw %.0f) present %.1f ms fence %d thread %d snapshot %lu KB fallback %lu verify-miss %lu\n",
(unsigned long long)g_frames.load(),
unsigned(F.drawn),
unsigned(F.draws),
        unsigned(F.gxDraws),
unsigned(F.copiesDone),
unsigned(F.copies),
processMs,
processCalls,
renderMs,
F.buildMs,
F.planMs,
F.drawMs,
presentMs,
int(g_measurePresent),
n3dsRenderThreadActive(),
(unsigned long)(snapshotBytes >> 10),
(unsigned long)g_snapshotFallbacks,
(unsigned long)g_mem.verifyMissing);
        static uint32_t lastKicks = 0, lastSplits = 0;
        const uint32_t kicksNow = g_r->backend().stats().kicks, splitsNow = g_r->backend().stats().splits;
        if(queueSample 
&& (g_tuning.materialIdsOnModels || sPreparedMaterialsEnabled))n3dsLog("[model-id] hits %llu misses %llu audit-failures %llu prepared %llu\n",
(unsigned long long)sMaterialPrograms.versionHits.load(std::memory_order_relaxed),
(unsigned long long)sMaterialPrograms.versionMisses.load(std::memory_order_relaxed),
(unsigned long long)sMaterialPrograms.versionAuditFailures.load(std::memory_order_relaxed),
(unsigned long long)sMaterialPrograms.preparedCalls.load(std::memory_order_relaxed));
        if(queueSample 
&& frame 
&& g_tuning.templateStates)n3dsLog("[state-template] hits %u misses %u\n",
frame->templateHits,
frame->templateMisses);
        if(queueSample)n3dsLog("[queue-cost] frame %llu begin-wait %.2f submit %.2f final-fence %.2f render-total %.2f process %.2f ms gpu-tail %.2f ms; since last: gpu kicks %u splits %u\n",
        (unsigned long long)g_frames.load(),
double(F.beginWaitTicks) * 1000.0 / SYSCLOCK_ARM11,
double(F.endSubmitTicks) * 1000.0 / SYSCLOCK_ARM11,
double(finishWaitTicks) * 1000.0 / SYSCLOCK_ARM11,
renderMs,
processMs,
double(C3D_GetDrawingTime()),
kicksNow - lastKicks,
splitsNow - lastSplits);
        if(queueSample)
        {
            lastKicks = kicksNow;
            lastSplits = splitsNow;
            
        }
        if(queueSample)
        {
            const double k = 1000.0 / SYSCLOCK_ARM11;
            n3dsLog("[render-split] frame %llu policy %.2f cache-begin %.2f loop %.2f (draws %.2f) copies %.2f end+recycle %.2f matrix-prep %.2f event-recycle %.2f ms\n",
(unsigned long long)g_frames.load(),
            F.splitTicks[0] * k,
F.splitTicks[1] * k,
F.splitTicks[2] * k,
F.buildMs + F.planMs + F.drawMs,
F.splitTicks[3] * k,
F.splitTicks[4] * k,
F.splitTicks[5] * k,
F.splitTicks[6] * k);
            static uint64_t s_dcMiss = 0, s_dcEvict = 0, s_dcLook = 0;
            const t2p::DrawCacheStats dc = g_r->drawCache().stats();
            n3dsLog("[render-split2] frame %llu matrix key %.2f append %.2f variant %.2f calib %.2f groups %llu members %llu; recycle pop %.2f ms; drawcache lookups %llu misses %llu evictions %llu entries %zu\n",
(unsigned long long)g_frames.load(),
            F.matrixSplit[0] * k,
F.matrixSplit[1] * k,
F.matrixSplit[2] * k,
F.matrixSplit[3] * k,
(unsigned long long)F.matrixSplit[4],
(unsigned long long)F.matrixSplit[5],
F.splitTicks[7] * k,
            (unsigned long long)(dc.lookups - s_dcLook),
(unsigned long long)(dc.misses - s_dcMiss),
(unsigned long long)(dc.evictions - s_dcEvict),
dc.entries);
            s_dcMiss = dc.misses;
            s_dcEvict = dc.evictions;
            s_dcLook = dc.lookups;
            
            {
                static uint64_t s_2d = 0;
                n3dsLog("[draws2d] frame %llu orthographic draws %llu hash %016llx\n",
(unsigned long long)g_frames.load(),
(unsigned long long)(g_n3ds2dDraws - s_2d),
(unsigned long long)g_n3ds2dHash);
                s_2d = g_n3ds2dDraws;
                g_n3ds2dHash = 0;
                
            }
            
            {
                static uint64_t s_ind = 0;
                if(g_n3dsIndirectDraws != s_ind)
                {
                    n3dsLog("[indirect] frame %llu draws with a dropped indirect warp: %llu\n",
(unsigned long long)g_frames.load(),
(unsigned long long)(g_n3dsIndirectDraws - s_ind));
                    s_ind = g_n3dsIndirectDraws;
                    
                }
                
            }
            if(t2p::g_compileClock 
&& dc.misses)
            {
                const uint64_t * P = t2p::g_compilePhase;
                n3dsLog("[compile-phase] total ms: vs1 %.1f vs2 %.1f parse %.1f tev %.1f fixed %.1f (fog %.1f) all %.1f (%llu misses)\n",
P[0] * k,
P[1] * k,
P[2] * k,
P[3] * k,
P[4] * k,
P[6] * k,
P[5] * k,
(unsigned long long)dc.misses);
                
            }
            
            {
                auto& up = t2p::UniformBlockPool::get();
                auto& ep = t2p::EventNodePool::get();
                n3dsLog("[pools] frame %llu uniforms hits %llu misses %llu kept %u (%lu KB); events hits %llu misses %llu kept %u (%lu KB)\n",
(unsigned long long)g_frames.load(),
up.hits,
up.misses,
up.count,
(unsigned long)((up.count * up.size) >> 10),
                ep.hits,
ep.misses,
ep.count,
(unsigned long)((ep.count * ep.size) >> 10));
                
            }
            n3dsLockStatsLog((unsigned long)g_frames.load());
            n3dsAllocSitesLog((unsigned long)g_frames.load());
            n3dsFastAllocLog();
            if(g_tuning.mapOnLower)n3dsLog("[ui-route] frame %llu markers %u routed %u refused %u copies %u switches %u owns %d\n",
(unsigned long long)g_frames.load(),
F.routeMarkers,
F.routedDraws,
F.routeRefused,
F.routeCopies,
F.routeSwitches,
int(g_routeOwnsBottom.load()));
            if(F.planDeltas || F.planDeltaFails || F.planDeltaChecks)n3dsLog("[plan-delta] frame %llu deltas %u fails %u checks %u mismatches %u\n",
(unsigned long long)g_frames.load(),
F.planDeltas,
F.planDeltaFails,
F.planDeltaChecks,
F.planDeltaMismatches);
            n3dsLog("[resident] frame %llu hits %u misses %u inserts %u full %u entries %lu (%lu KB)\n",
(unsigned long long)g_frames.load(),
F.residentHits,
F.residentMisses,
F.residentInserts,
F.residentFull,
(unsigned long)F.residentEntries,
(unsigned long)(F.residentBytes >> 10));
            if(g_r->replayDraws())
            {
                
                {
                    static const bool rp = []
                    {
                        const char * v = std::getenv("N3DS_REPLAY_PROF");
                        return v 
&& * v == '1';
                        
                    }
                    ();
                    t2p::g_replayProfOn = rp;
                    if(rp)
                    {
                        const uint64_t * q = t2p::g_replayProf;
                        const double h = q[5]?double(q[5]):1.0, us = 1e6 / SYSCLOCK_ARM11;
                        n3dsLog("[replay-prof] %.0f hits, us/hit: lookup range %.1f tail %.1f drawkey %.1f, remap %.1f textures %.1f repack %.1f backend %.1f [shader+attr %.1f uniforms %.1f texenv+tex %.1f effect %.1f updatectx %.1f drawcall %.1f]\n",
                        double(q[5]),
q[12] * us / h,
q[13] * us / h,
q[14] * us / h,
q[1] * us / h,
q[2] * us / h,
q[3] * us / h,
q[4] * us / h,
q[6] * us / h,
q[7] * us / h,
q[8] * us / h,
q[9] * us / h,
q[10] * us / h,
q[11] * us / h);
                        for (int i=0;i<16;i++)t2p::g_replayProf[i] = 0;
                        
                    }
                    
                }
                if(g_tuning.eventDirect || g_tuning.directModels)n3dsLog("[direct] draws %u fallback %u groups %u\n",
F.directDraws,
F.directFallbacks,
F.directGroups);
                n3dsLog("[replay] frame %llu kb %u hits %u (recorded %u, new projection %u) misses %u stale %u inserts %u groups %u/%u; misses so far: no-entry %llu tail %llu draw-state %llu 2d-uniforms %llu\n",
(unsigned long long)g_frames.load(),
F.replayLimitKb,
F.replayHits,
F.replayRecorded,
F.replayProj,
F.replayMisses,
F.replayStale,
F.replayInserts,
F.replayGroupHits,
F.replayGroupHits + F.replayGroupMisses,
(unsigned long long)t2p::g_replayWhy[0],
(unsigned long long)t2p::g_replayWhy[1],
(unsigned long long)t2p::g_replayWhy[2],
(unsigned long long)t2p::g_replayWhy[3]);
                
            }
            n3dsLog("[draw-reuse] frame %llu hits %u misses %u inserts %u full %u entries %lu (%lu KB) arena peak %lu KB linear free %lu KB textures %lu KB\n",
(unsigned long long)g_frames.load(),
            F.reuseHits,
F.reuseMisses,
F.reuseInserts,
F.reuseFull,
(unsigned long)F.reuseEntries,
(unsigned long)(F.reuseBytes >> 10),
(unsigned long)(g_r->backend().stats().arenaPeak >> 10),
            (unsigned long)(linearSpaceFree() >> 10),
(unsigned long)(g_r->backend().stats().textureBytes >> 10));
            
        }
        if (!n3ds_warm::covering() 
&& !g_noBundleWrites 
&& g_frames % 120 == 0 
&& g_r->missEntries() != g_missesWritten) 
        {
            g_missesWritten = g_r->missEntries();
            const std::vector < uint8_t > m = g_r->missBundle();
            if (FILE * f = std::fopen(N3DS_ROOT "/ww_bundle_misses.tmp",
"wb")) 
            {
                const bool ok = std::fwrite(m.data(),
1,
m.size(),
f) == m.size();
                std::fclose(f);
                if (ok) 
                {
                    std::remove(N3DS_ROOT "/ww_bundle_misses.bin");
                    std::rename(N3DS_ROOT "/ww_bundle_misses.tmp",
N3DS_ROOT "/ww_bundle_misses.bin");
                    
                }
                
            }
            
        }
        if(!n3ds_warm::covering() 
&& ((g_tuning.lowerScreen 
&& g_bottomUi.takeDirty()) || g_consoleOn) 
&& !g_routeOwnsBottom.load(std::memory_order_acquire)) 
        {
            // Serialize with citro3d's deferred swap so we never paint/swap
            // while a GX display transfer still targets the bottom buffer.
            C3Di_RenderQueueWaitDone();
            gfxFlushBuffers();
            gfxScreenSwapBuffers(GFX_BOTTOM,
false);
            
        }
        g_bottomUi.capture((unsigned long)g_frames.load());
        
    }
    int n3dsGpuInit(void) 
    {
        std::atexit(n3dsGpuShutdown);
        bool recoveredPrefs = false;
        FILE * prefs = std::fopen(N3DS_ROOT "/performance.ini",
"r");
        if(!prefs 
&& errno == ENOENT)
        {
            prefs = std::fopen(N3DS_ROOT "/performance.ini.ui.bak",
"r");
            if(prefs)
            {
                recoveredPrefs = true;
                n3dsLog("[settings] recovered preferences from backup\n");
                
            }
            
        }
        if(prefs)
        {
            g_tuning.read(prefs);
            std::fclose(prefs);
            if(recoveredPrefs)std::rename(N3DS_ROOT "/performance.ini.ui.bak",
N3DS_ROOT "/performance.ini");
            
        }
        else if(errno == ENOENT)
        {
            if(FILE * f = std::fopen(N3DS_ROOT "/performance.ini",
"w"))
            {
                std::fputs(Performance3ds::example(),
f);
                std::fclose(f);
                
            }
            
        }
        if(!g_tuning.widePicture)
        {
            g_tuning.upperHud = false;
            if(n3dsUiSetTopHudEnabled)n3dsUiSetTopHudEnabled(0);
            
        }
        ww_screen::fpsEnabled.store(g_tuning.showFrameRate,
std::memory_order_relaxed);
        
        {
            const char * o = std::getenv("N3DS_PROF_ORIGIN");
            g_gxOriginProf = o 
&& o[0] == '1';
            
        }
        t2p::g_decodedInitMode = g_tuning.overwriteDecode?1:0;
        const bool debugConsole = std::getenv("N3DS_DEBUG_CONSOLE") != nullptr;
        if(debugConsole)g_tuning.lowerScreen = false;
        if(!debugConsole)
        {
            g_consoleOn = false;
            g_bottomUi.init();
            if(!g_tuning.lowerScreen)
            {
                gfxFlushBuffers();
                gfxScreenSwapBuffers(GFX_BOTTOM,
false);
                g_bottomUi.init();
                gfxFlushBuffers();
                gfxScreenSwapBuffers(GFX_BOTTOM,
false);
                
            }
            static const devoptab_t toLog = 
            {
                "n3dslog", 0, nullptr, nullptr, 
                [](struct _reent * ,
void * ,
const char * p,
size_t n)->ssize_t
                {
                    char b[256];
                    size_t k = n < sizeof b - 1?n:sizeof b - 1;
                    std::memcpy(b,
p,
k);
                    b[k] = 0;
                    n3dsLog("%s",
b);
                    return ssize_t(n);
                    
                }
                , 
                nullptr, nullptr, nullptr
            } ;
            devoptab_list[STD_OUT] = &toLog;
            devoptab_list[STD_ERR] = &toLog;
            setvbuf(stdout,
nullptr,
_IONBF,
0);
            setvbuf(stderr,
nullptr,
_IONBF,
0);
            
        }
        if(g_tuning.spareCore >= 0)
        {
            setenv("N3DS_RENDER_THREAD",
g_tuning.spareCore?"1":"0",
1);
            setenv("N3DS_RENDER_PIPELINE",
g_tuning.spareCore?"1":"0",
1);
            
        }
        n3dsLog("[perf] fidelity-preserving settings: native 400x240 core2 %d material_cache %d geometry_cache %d vertex_runs %d packed_uniforms %d plan_reuse %d fog_prepare %d vertex_plan %d vertex_decode %d palette_cache %d bottom_ui %d draw_timing %d lean_state %d retain_geometry %d state_inplace %d pipeline_stage8 %d heap_fence %d fifo_thread %d fifo_core %d decode_helper %d ignored %u\n",
g_tuning.spareCore,
int(g_tuning.cacheMaterials),
int(g_tuning.cacheGeometry),
int(g_tuning.coalesceVertices),
int(g_tuning.fixedUniforms),
int(g_tuning.reusePlans),
int(g_tuning.prepareFog),
int(g_tuning.planVertices),
int(g_tuning.decodeVertices),
int(g_tuning.cachePalettes),
int(g_tuning.lowerScreen),
int(g_tuning.timeDraws),
int(g_tuning.slimState),
int(g_tuning.keepGeometry),
int(g_tuning.inplaceState),
int(g_tuning.stageEight),
int(g_tuning.fenceHeap),
int(g_tuning.commandThread),
g_tuning.commandCore,
int(g_tuning.helperDecode),
g_tuning.rejectedLines);
        n3dsLog("[perf] j3d_geometry %d ignored %u\n",
int(g_tuning.j3dMeshes),
g_tuning.rejectedLines);
        n3dsLog("[perf] j3d_programs %d uniform_sections %d foliage_far %d pool_floor %d batch_fx %d frame_skip %d paint_pacing %d paint_max_gap %d paint_cadence %d skip_draw %d audio_core1 %d real_shadows %d early_release %d ignored %u\n",
int(g_tuning.j3dShaders),
int(g_tuning.sectionUniforms),
g_tuning.farFoliage,
g_tuning.floorPool,
int(g_tuning.fxBatch),
g_tuning.skippedFrames,
g_tuning.pacePaint,
g_tuning.maxPaintGap,
g_tuning.cadencePaint,
int(g_tuning.omitDraw),
int(g_tuning.soundOnCore1),
int(g_tuning.shadowsOn),
int(g_tuning.releaseEarly),
g_tuning.rejectedLines);
        n3dsLog("[perf] extended settings: typed_gx %d direct_texture %d state_pool %d vertex_pool_fifo %d plan_notes %d direct_pack %d decode_overwrite %d matrix_batch %d plan_templates %d packet_worker %d packet_worker_verify %d state_incremental %d ignored %u\n",
int(g_tuning.typedCommands),
int(g_tuning.textureDirect),
int(g_tuning.poolState),
int(g_tuning.fifoVertexPool),
int(g_tuning.keepPlanNotes),
int(g_tuning.packDirect),
int(g_tuning.overwriteDecode),
int(g_tuning.batchMatrices),
int(g_tuning.templatePlans),
int(g_tuning.workerPackets),
int(g_tuning.verifyPackets),
int(g_tuning.incrementalState),
g_tuning.rejectedLines);
        const char * pipeline = std::getenv("N3DS_RENDER_PIPELINE");
        g_pipeline = pipeline 
&& !std::strcmp(pipeline,
"1");
        if (n3dsRenderThreadStart()) 
        {
            const int ok = n3dsRenderThreadInit();
            if (ok 
&& g_pipeline) 
            {
                g_cpuFrames[0] = new t2p::CpuFrame3ds;
                g_cpuFrames[1] = new t2p::CpuFrame3ds;
                for (auto* frame:g_cpuFrames) frame->snapshot.init(2u << 20);
                g_recorder = new t2p::FrameRecorder3ds;
                if(g_tuning.eventDirect || g_tuning.directModels)g_recorder->setDirectRegistry(g_r->directRegistry());
                g_recorder->setDirectSnapshots(g_tuning.snapshotDirect);
                g_recorder->setDirectDiagnostics(std::getenv("N3DS_DIRECT_DIAG") != nullptr);
                if(g_tuning.directModels)g_recorder->setGroupRegistry(g_r->groupRegistry());
                if(g_tuning.readyShapes)n3dsLog("[prepared] shape programs loaded %d\n",
int(g_recorder->fifo().loadPreparedShapes(N3DS_ROOT "/ww_shapes.bin")));
                g_recorder->fifo().setPrimitiveRuns(g_tuning.coalesceVertices);
                g_recorder->setStateNotes(!g_tuning.slimState);
                g_recorder->setStateInPlace(g_tuning.inplaceState);
                g_recorder->setStatePool(g_tuning.poolState);
                g_recorder->setStateIncremental(g_tuning.incrementalState);
                g_recorder->setStateTemplates(g_tuning.templateStates);
                n3dsLog("[state] templates %d\n",
int(g_tuning.templateStates));
                g_recorder->setRetainGeometry(g_tuning.keepGeometry,
size_t(g_tuning.retainLimitKb) << 10);
                n3dsLog("[perf] retain_geometry %d budget %d KB (recorder)\n",
int(g_tuning.keepGeometry),
g_tuning.retainLimitKb);
                g_recorder->setVertexPoolFifo(g_tuning.fifoVertexPool);
                g_recorder->setCompactDrawStorage(g_tuning.compactRecords);
                n3dsLog("[perf] record_compact %d tex_error_table %d\n",
int(g_tuning.compactRecords),
int(g_tuning.errorTable));
                g_recorder->setHeapProbe(g_tuning.fenceHeap?heapRoom:nullptr);
                if (const char * c = std::getenv("N3DS_HEAP_TEST_CEILING_MB")) 
                {
                    const int mb = atoi(c);
                    if (mb >= 16 
&& mb <= 85) g_heapCeiling = size_t(mb) << 20;
                    
                }
                g_recorder->setHeapReady(g_tuning.fenceHeap 
&& g_tuning.partialStaging ? heapReady : nullptr);
                n3dsLog("[perf] staging_partial %d heap ceiling %u MB\n",
int(g_tuning.partialStaging),
unsigned(g_heapCeiling >> 20));
                g_recorder->fifo().setGeometryCache(g_tuning.cacheGeometry);
                g_recorder->fifo().setFastAttributeDecode(g_tuning.decodeVertices);
                g_recorder->fifo().setCachedVertexPlan(g_tuning.planVertices);
                g_recorder->fifo().setAuroraMode(true);
                g_recorder->fifo().setUncheckedArrays(true);
                g_recorder->fifo().setStopAtDrawDone(true);
                startDecodeHelper(g_recorder->fifo());
                g_recorder->fifo().setGuestArrayReader([](uint32_t a,
uint32_t)
                {
                    return reinterpret_cast < const uint8_t * > (uintptr_t(a));
                }
                );
                g_recorder->select(g_cpuFrames[0]);
                n3dsLog("[render-thread] immutable frame pipeline enabled\n");
                
            }
            return ok;
            
        }
        g_pipeline = false;
        return n3dsGpuInitLocal();
        
    }
    extern "C" int n3dsOriginProfOn(void) 
    {
        return g_gxOriginProf ? 1 : 0;
        
    }
    extern "C" void n3dsSetGxOrigin(int o) 
    {
        g_gxOrigin = o ? 1 : 0;
        
    }
    uint32_t n3dsGpuProcess(const uint8_t * data,
uint32_t size,
int * done) 
    {
        if (g_pipeline) 
        {
            auto& frame = * g_recorder->current;
            const u64 t0 = svcGetSystemTick();
            ++frame.processCalls;
            const uint64_t d0 = g_recorder->fifo().drawCount(), v0 = g_t2pProf[2];
            g_n3dsMark[2] = g_n3dsMark[2] + 1;
            g_recorder->inProcess = true;
            size_t used = g_recorder->fifo().process(data,
size);
            g_recorder->inProcess = false;
            g_n3dsMark[3] = g_n3dsMark[3] + 1;
            frame.processTicks += svcGetSystemTick() - t0;
            if(g_gxOriginProf)
            {
                const int o = g_gxOrigin;
                g_originTicks[o] += svcGetSystemTick() - t0;
                g_originBytes[o] += used;
                g_originDraws[o] += g_recorder->fifo().drawCount() - d0;
                g_originVerts[o] += g_t2pProf[2] - v0;
                g_originCalls[o]++;
                
            }
             * done = g_recorder->fifo().takeDrawDone()?1:0;
            if(used == 0 || (used < size 
&& ! * done))used = size;
            return uint32_t(used);
            
        }
        if (n3dsRenderThreadActive()) return n3dsRenderThreadProcess(data,
size,
done);
        return n3dsGpuProcessLocal(data,
size,
done);
        
    }
    static int g_abFoliageFar = [] 
    {
        const char * v = std::getenv("N3DS_AB_FOLIAGE");
        return v ? std::atoi(v) : 2000;
        
    }
    ();
    extern "C" void n3dsEndFrameRelease(void);
    extern "C" void n3dsPacingNoteProcess(uint64_t ticks);
    #include "ui_play_3ds.h"
extern "C" bool C3Dx_InFrame(void);
    extern "C" void C3Dx_WaitGpu(void);
    extern "C" void n3dsDropSceneCachesIfIdle(void) 
    {
        if (!g_r || !g_r->ready()) return;
        if (C3Dx_InFrame() || (n3dsRenderThreadActive() 
&& n3dsRenderThreadBusy())) 
        {
            g_r->requestSceneDrop();
            return;
            
        }
        g_r->dropSceneCaches();
        n3dsLog("[scene] low memory: dropped caches so this scene can keep loading\n");
        
    }
    extern "C" void n3dsSetDialogueHold(int on) 
    {
        if (g_recorder 
&& g_recorder->current) g_recorder->current->keepPicture = on != 0;
        // Always latch on the renderer so skipped paints still depth-only clear.
        if (g_r) g_r->setKeepPicture(on != 0);
        
    }
    extern "C" void n3dsCutsceneHandoff(int entering) 
    {
        if (!g_r || !g_r->ready() || !n3dsUiInPlay()) return;
        if (C3Dx_InFrame() || (n3dsRenderThreadActive() 
&& n3dsRenderThreadBusy())) 
        {
            g_r->requestSceneDrop();
            return;
            
        }
        C3Dx_WaitGpu();
        g_r->dropSceneCaches();
        n3dsLog("[cutscene] %s: dropped scene caches after the GPU finished\n",
entering ? "start" : "end");
        
    }
    void n3dsGpuEndFrame(void) 
    {
        
        {
            static const bool ab = std::getenv("N3DS_AB_SHAPES") != nullptr;
            if (ab 
&& g_tuning.j3dMeshes != (((g_frames.load() / 300) & 1) == 0)) 
            {
                g_tuning.j3dMeshes = !g_tuning.j3dMeshes;
                n3dsLog("[ab] frame %llu: shape references %d\n",
(unsigned long long)g_frames.load(),
int(g_tuning.j3dMeshes));
                
            }
            
        }
        
        {
            static const bool cyc = std::getenv("N3DS_AB_CYCLE") != nullptr;
            static int lastPhase = - 1;
            if (cyc) 
            {
                const int ph = int((g_frames.load() / 300) % 2);
                if (ph != lastPhase) 
                {
                    lastPhase = ph;
                    g_foliageAb.store(ph == 1 ? g_abFoliageFar : 0);
                    n3dsLog("[ab] frame %llu: phase %d foliage_far=%d\n",
(unsigned long long)g_frames.load(),
ph,
ph ? g_abFoliageFar : 0);
                    
                }
                
            }
            
        }
        static const bool coreProbe = [] 
        {
            const char * v = std::getenv("N3DS_CORE_PROBE");
            return v 
&& * v == '1';
            
        }
        ();
        if (coreProbe 
&& (g_frames.load() == 600 || g_frames.load() == 1500)) coreprobe::run(g_frames.load() == 600 ? "frame600" : "frame1500");
        if(g_tuning.j3dMeshes 
&& g_recorder 
&& (g_frames.load() % 120) == 0)
        {
            static unsigned long long lastRefs = 0;
            static unsigned lastGen = 0;
            const unsigned long long refs = g_recorder->fifo().shapeRefs();
            const unsigned gen = g_n3dsShapeGen;
            n3dsLog("[j3d-shape] 120 frames: shape references %llu, model generation +%u\n",
refs - lastRefs,
gen - lastGen);
            lastRefs = refs;
            lastGen = gen;
            
        }
        if(g_tuning.readyShapes 
&& g_recorder 
&& (g_frames.load() % 120) == 0)n3dsLog("[prepared-shape] hits %llu misses %llu runs %llu bytes %u\n",
(unsigned long long)g_recorder->fifo().preparedShapeHits(),
(unsigned long long)g_recorder->fifo().preparedShapeMisses(),
(unsigned long long)g_recorder->fifo().preparedShapeRuns(),
unsigned(g_recorder->fifo().preparedShapeBytes()));
        if((g_tuning.recordModels || g_tuning.recordMaterials) 
&& g_recorder 
&& (g_frames.load() % 120) == 0)n3dsLog("[j3d-model] groups %llu materials %llu\n",
(unsigned long long)g_recorder->fifo().modelGroups(),
(unsigned long long)g_recorder->fifo().materialRefs());
        if(g_tuning.j3dShaders 
&& g_recorder 
&& (g_frames.load() % 120) == 0) n3dsLog("[j3d-program] hits %llu misses %llu\n",
(unsigned long long)g_recorder->fifo().stateProgramHits(),
(unsigned long long)g_recorder->fifo().stateProgramMisses());
        if (g_pipeline) 
        {
            g_n3dsMark[4] = 1;
            const uint64_t ej0 = svcGetSystemTick();
            finishRecording(g_recorder->fifo());
            const uint64_t ej1 = svcGetSystemTick();
            g_n3dsMark[4] = 2;
            if(g_tuning.keepGeometry 
&& (g_inputFrames.load() + 1) % 60 == 0)
            {
                const t2p::GxRetainStats rs = g_recorder->takeRetainStats();
                n3dsLog("[retain] 60 frames: draws %llu skipped %llu (%.0f%%), batches %llu retained %llu published %llu materialized %llu not-retainable %llu over-budget %llu; compared %llu KB arrays + %llu KB stream, snapshots %llu KB, held %zu KB; shape-reference matches %llu of %llu offered (new batch: no key %llu, failed %llu; mid-batch miss %llu)\n",
                (unsigned long long)rs.draws,
(unsigned long long)rs.skipped,
rs.draws?100.0 * double(rs.skipped) / double(rs.draws):0.0,
(unsigned long long)rs.batches,
(unsigned long long)rs.retainedBatches,
                (unsigned long long)rs.published,
(unsigned long long)rs.materialized,
(unsigned long long)rs.notRetainable,
(unsigned long long)rs.overBudget,
                (unsigned long long)(rs.verifyBytes >> 10),
(unsigned long long)(rs.streamBytes >> 10),
(unsigned long long)(rs.snapshotBytes >> 10),
rs.heldBytes >> 10,
(unsigned long long)rs.shapeMatches,
(unsigned long long)rs.shapeOffered,
(unsigned long long)rs.shapeNewNoPrev,
(unsigned long long)rs.shapeNewMiss,
(unsigned long long)rs.shapeMidMiss);
                n3dsLog("[retain-miss] 60 frames, batches/kverts decoded by reason: not-retainable %llu/%llu no-key %llu/%llu header %llu/%llu bindings %llu/%llu stream %llu/%llu array-bytes %llu/%llu shape %llu/%llu mid-batch %llu/%llu over-budget %llu/%llu\n",
                (unsigned long long)rs.missBatches[0],
(unsigned long long)(rs.missVerts[0] / 1000),
(unsigned long long)rs.missBatches[1],
(unsigned long long)(rs.missVerts[1] / 1000),
(unsigned long long)rs.missBatches[2],
(unsigned long long)(rs.missVerts[2] / 1000),
(unsigned long long)rs.missBatches[3],
(unsigned long long)(rs.missVerts[3] / 1000),
(unsigned long long)rs.missBatches[4],
(unsigned long long)(rs.missVerts[4] / 1000),
(unsigned long long)rs.missBatches[5],
(unsigned long long)(rs.missVerts[5] / 1000),
(unsigned long long)rs.missBatches[6],
(unsigned long long)(rs.missVerts[6] / 1000),
(unsigned long long)rs.missBatches[7],
(unsigned long long)(rs.missVerts[7] / 1000),
(unsigned long long)rs.missBatches[8],
(unsigned long long)(rs.missVerts[8] / 1000));
                
            }
            auto * frame = g_recorder->current;
            if ((g_inputFrames.load() + 1) % 60 == 0) n3dsGameHeapReport(frame->heaps,
sizeof frame->heaps);
            const uint64_t ej2 = svcGetSystemTick();
            frame->frozen = frame->snapshot.freeze(readableTexture,
skipSnapshots);
            
            {
                static int on = - 1, n = 0;
                if(on < 0)
                {
                    const char * v = getenv("N3DS_SNAP_LOG");
                    on = (v 
&& * v == '1')?1:0;
                    
                }
                if(on 
&& frame->frozen 
&& (++n % 120) == 0)
                {
                    std::string line;
                    char b[48];
                    int k = 0;
                    frame->snapshot.forEachCopied([&](uint32_t a,
uint32_t sz)
                    {
                        if(sz >= 2048 
&& k < 40)
                        {
                            snprintf(b,
sizeof b,
" %08lx+%lu",
(unsigned long)a,
(unsigned long)sz); line += b; k++; 
                        }
                        
                    }
                    );
                    n3dsLog("[snap] copied %u KB:%s\n",
unsigned(frame->snapshot.bytes() >> 10),
line.c_str());
                    
                }
                
            }
            n3dsEndFrameRelease();
            
            {
                static const bool rkOn = []
                {
                    const char * v = std::getenv("N3DS_REPLAY_KEYS");
                    return v 
&& * v == '1';
                    
                }
                ();
                if(rkOn 
&& g_r 
&& g_r->replayDraws()) for (auto& ev:frame->events) if(ev.kind == t2p::RecordedEvent3ds::Draw 
&& ev.geo) t2p::computeReplayKey(ev);
                
            }
            const uint64_t ej3 = svcGetSystemTick();
            for (int i=0;i<16;++i) 
            {
                frame->prof[i] = g_t2pProf[i];
                g_t2pProf[i] = 0;
                
            }
            for (int i=0;i<8;++i) 
            {
                g_recStateProf[i] = g_t2pStateProf[i];
                g_t2pStateProf[i] = 0;
                
            }
            const uint64_t handoffStart = svcGetSystemTick();
            n3dsGpuSync();
            frame->handoffWaitTicks = svcGetSystemTick() - handoffStart;
            g_n3dsMark[4] = 3;
            g_submitted = frame;
            n3dsPacingNoteProcess(frame->processTicks);
            g_submittedSlot = n3dsPaintSlotOf(g_paintHandedCount());
            n3dsRenderThreadFrame();
            
            {
                static uint64_t acc[5];
                static unsigned n = 0;
                const uint64_t ej4 = svcGetSystemTick();
                acc[0] += ej1 - ej0;
                acc[1] += ej2 - ej1;
                acc[2] += ej3 - ej2;
                acc[3] += handoffStart?0:0;
                acc[4] += ej4 - ej3;
                if(++n == 60)
                {
                    const double k = 1000.0 / SYSCLOCK_ARM11 / 60;
                    n3dsLog("[endjob] per frame: finish-recording %.2f stats+heaps %.2f snapshot-freeze %.2f (%u KB, skipped %u KB) handoff+ack %.2f ms\n",
acc[0] * k,
acc[1] * k,
acc[2] * k,
unsigned(frame->snapshot.bytes() >> 10),
unsigned(frame->snapshot.skippedBytes() >> 10),
acc[4] * k);
                    for (auto& a:acc)a = 0;
                    n = 0;
                    
                }
                
            }
            g_n3dsMark[4] = 0;
            g_recordIndex^= 1;
            auto * next = g_cpuFrames[g_recordIndex];
            next->processTicks = 0;
            next->processCalls = 0;
            next->heaps[0] = 0;
            g_recorder->select(next);
            
        }
        else if (n3dsRenderThreadActive()) n3dsRenderThreadFrame();
        else n3dsGpuEndFrameLocal();
        ++g_inputFrames;
        n3dsPgoTick(unsigned(g_inputFrames));
        
    }
    extern "C" 
    {
        int __llvm_profile_runtime = 0;
        extern uint64_t __llvm_profile_raw_version __attribute__((weak));
        extern char __start___llvm_prf_data[] __attribute__((weak)), __stop___llvm_prf_data[] __attribute__((weak));
        extern char __start___llvm_prf_cnts[] __attribute__((weak)), __stop___llvm_prf_cnts[] __attribute__((weak));
        extern char __start___llvm_prf_names[] __attribute__((weak)), __stop___llvm_prf_names[] __attribute__((weak));
        extern char __start___llvm_prf_bits[] __attribute__((weak)), __stop___llvm_prf_bits[] __attribute__((weak));
        
    }
    static void n3dsPgoTick(unsigned frame) 
    {
        static const unsigned dumpAt = [] 
        {
            const char * v = std::getenv("N3DS_PGO_DUMP");
            return v ? unsigned(std::strtoul(v,
nullptr,
10)) : 0u;
            
        }
        ();
        static bool done = false;
        if (!dumpAt || done || frame < dumpAt) return;
        done = true;
        if (!&__llvm_profile_raw_version || !__start___llvm_prf_data) 
        {
            n3dsLog("[pgo] not an instrumented build\n");
            return;
            
        }
        const uintptr_t db = uintptr_t(__start___llvm_prf_data), de = uintptr_t(__stop___llvm_prf_data);
        const uintptr_t cb = uintptr_t(__start___llvm_prf_cnts), ce = uintptr_t(__stop___llvm_prf_cnts);
        const uintptr_t nb = uintptr_t(__start___llvm_prf_names), ne = uintptr_t(__stop___llvm_prf_names);
        const uintptr_t bb = __start___llvm_prf_bits ? uintptr_t(__start___llvm_prf_bits) : 0, be = __start___llvm_prf_bits ? uintptr_t(__stop___llvm_prf_bits) : 0;
        constexpr size_t kDataSize = 48;
        auto pad8 = [](uint64_t n) 
        {
            return (8 - (n & 7)) & 7;
            
        } ;
        const uint64_t dataBytes = de - db, cntBytes = ce - cb, nameBytes = ne - nb, bitBytes = be - bb;
        const uint64_t magic = (uint64_t)255 << 56 | (uint64_t)'l' << 48 | (uint64_t)'p' << 40 | (uint64_t)'r' << 32 | (uint64_t)'o' << 24 | (uint64_t)'f' << 16 | (uint64_t)'R' << 8 | (uint64_t)129;
        const uint64_t hdr[16] = 
        {
            magic, __llvm_profile_raw_version, 0, dataBytes / kDataSize, pad8(dataBytes), cntBytes / 8, pad8(cntBytes), bitBytes, pad8(bitBytes), 
            nameBytes, uint64_t(cb - db), uint64_t(bb ? bb - db : 0 - db), uint64_t(nb), 0, 0, 2 
        } ;
        FILE * f = std::fopen(N3DS_ROOT "/ww.profraw",
"wb");
        if (!f) 
        {
            n3dsLog("[pgo] cannot write ww.profraw\n");
            return;
            
        }
        static const uint8_t zeros[8] = 
        {
            
        } ;
        std::fwrite(hdr,
sizeof hdr,
1,
f);
        std::fwrite(reinterpret_cast < const void * > (db),
1,
dataBytes,
f);
        std::fwrite(zeros,
1,
pad8(dataBytes),
f);
        std::fwrite(reinterpret_cast < const void * > (cb),
1,
cntBytes,
f);
        std::fwrite(zeros,
1,
pad8(cntBytes),
f);
        if (bitBytes) std::fwrite(reinterpret_cast < const void * > (bb),
1,
bitBytes,
f);
        std::fwrite(zeros,
1,
pad8(bitBytes),
f);
        std::fwrite(reinterpret_cast < const void * > (nb),
1,
nameBytes,
f);
        std::fwrite(zeros,
1,
pad8(nameBytes),
f);
        std::fclose(f);
        n3dsLog("[pgo] wrote ww.profraw at frame %u: %llu functions, %llu counters, names %llu bytes\n",
frame,
(unsigned long long)(dataBytes / kDataSize),
        (unsigned long long)(cntBytes / 8),
(unsigned long long)nameBytes);
        
    }
    void n3dsGpuShutdownLocal(void) 
    {
        if (g_gpuShutdownDone) return;
        g_gpuShutdownDone = true;
        if (g_c3dReady) C3Di_RenderQueueWaitDone();
        if(g_processExiting.load())
        {
            if (g_r) g_r->stopPacketWorker();
            if (g_c3dReady) 
            {
                C3D_Fini();
                g_c3dReady = false;
                
            }
            return;
            
        }
        if (g_r) 
        {
            delete g_r;
            g_r = nullptr;
            
        }
        if (g_tgt) 
        {
            C3D_RenderTargetDelete(g_tgt);
            g_tgt = nullptr;
            
        }
        if (g_shots) 
        {
            std::fclose(g_shots);
            g_shots = nullptr;
            
        }
        if (g_c3dReady) 
        {
            C3D_Fini();
            g_c3dReady = false;
            
        }
        delete g_recorder;
        g_recorder = nullptr;
        for (auto*& frame:g_cpuFrames)
        {
            delete frame;
            frame = nullptr;
            
        }
        
    }
    unsigned int n3dsCurrentThreadHandle(void) 
    {
        Handle h = 0;
        if (R_FAILED(svcDuplicateHandle(&h,
CUR_THREAD_HANDLE))) return 0;
        return h;
        
    }
    void n3dsSetThreadPriority(unsigned int handle,
int gcPriority) 
    {
        const s32 p = gcPriority < 0 ? 0x1B : 0x1C + (gcPriority > 31 ? 31 : gcPriority);
        const Result r = svcSetThreadPriority(handle ? Handle(handle) : CUR_THREAD_HANDLE,
p);
        static int logged = 0;
        if (logged < 40) 
        {
            logged++;
            n3dsLog("[3ds] thread priority gc %d -> 0x%lx (%s, result %08lx)\n",
gcPriority,
(long)p,
handle ? "other" : "self",
(unsigned long)r);
            
        }
        
    }
    struct __pthread_t;
    extern "C" int n3dsFifoThreadMode(void) 
    {
        static const int on = [] 
        {
            Performance3ds config;
            if (FILE * f = std::fopen(N3DS_ROOT "/performance.ini",
"r")) 
            {
                config.read(f);
                std::fclose(f);
                
            }
            n3dsLog("[threads] fifo_thread %d fifo_core %d\n",
int(config.commandThread),
config.commandCore);
            if (config.commandThread 
&& config.commandCore != 1) 
            {
                s32 prio = 0x30;
                svcGetThreadPriority(&prio,
CUR_THREAD_HANDLE);
                Thread probe = threadCreate([](void * ) 
                {
                    
                }
                ,
nullptr,
0x1000,
prio,
config.commandCore,
false);
                if (!probe) 
                {
                    n3dsLog("[threads] core %d refuses threads: FIFO inline on the game thread\n",
config.commandCore);
                    return 0;
                    
                }
                threadJoin(probe,
U64_MAX);
                threadFree(probe);
                
            }
            return config.commandThread ? 1 : 0;
            
        }
        ();
        return on;
        
    }
    static size_t g_nextThreadStack = 0;
    extern "C" void n3dsNextThreadStack(size_t bytes) 
    {
        g_nextThreadStack = bytes;
        
    }
    static int threadCore() 
    {
        static int core = [] 
        {
            Performance3ds config;
            if (FILE * f = std::fopen(N3DS_ROOT "/performance.ini",
"r")) 
            {
                config.read(f);
                std::fclose(f);
                
            }
            if (!config.systemCoreThreads) return 0;
            if (config.backgroundCore >= 2) 
            {
                n3dsLog("[threads] game threads on core %d (core 1 not borrowed)\n",
config.backgroundCore);
                return config.backgroundCore;
                
            }
            if (config.backgroundCore == 0) 
            {
                n3dsLog("[threads] game threads on core 0 (core 1 not borrowed)\n");
                return 0;
                
            }
            const Result r = APT_SetAppCpuTimeLimit(80);
            n3dsLog("[threads] core1_threads 1: APT_SetAppCpuTimeLimit(80) %08lx\n",
(unsigned long)r);
            return R_SUCCEEDED(r) ? 1 : 0;
            
        }
        ();
        return core;
        
    }
    static void threadFailureReport(s32 prio,
int core) 
    {
        static int reports = 0;
        if (reports >= 2) return;
        reports++;
        static u8 scratch[0x1000] __attribute__((aligned(8)));
        (void)core;
        for (int c : {0}) 
        {
            Handle h = 0;
            const Result rc = svcCreateThread(&h,
[](void * ) 
            {
                svcExitThread(); 
            }
            ,
0,
reinterpret_cast < u32 * > (scratch + sizeof scratch),
prio,
c);
            n3dsLog("[threads] diagnosis: svcCreateThread prio 0x%lx core %d -> %08lx\n",
(long)prio,
c,
(unsigned long)rc);
            if (R_SUCCEEDED(rc)) 
            {
                svcWaitSynchronization(h,
1000000000LL);
                svcCloseHandle(h);
                
            }
            if (c == 0) break;
            
        }
        Handle lim = 0;
        if (R_SUCCEEDED(svcGetResourceLimit(&lim,
CUR_PROCESS_HANDLE))) 
        {
            ResourceLimitType types[] = 
            {
                RESLIMIT_PRIORITY, RESLIMIT_COMMIT, RESLIMIT_THREAD, RESLIMIT_EVENT, RESLIMIT_MUTEX, RESLIMIT_SEMAPHORE, RESLIMIT_TIMER, RESLIMIT_SHAREDMEMORY, RESLIMIT_ADDRESSARBITER, RESLIMIT_CPUTIME
            } ;
            s64 cur[10] = 
            {
                
            }
            , max[10] = 
            {
                
            } ;
            svcGetResourceLimitCurrentValues(cur,
lim,
types,
10);
            svcGetResourceLimitLimitValues(max,
lim,
types,
10);
            svcCloseHandle(lim);
            static const char * names[] = 
            {
                "priority", "commit", "thread", "event", "mutex", "semaphore", "timer", "sharedmem", "arbiter", "cputime"
            } ;
            for (int i = 0; i < 10; i++) n3dsLog("[threads] diagnosis: limit %-9s %ld of %ld\n",
names[i],
(long)cur[i],
(long)max[i]);
            
        }
        n3dsLog("[threads] diagnosis: application region free %lu KB\n",
(unsigned long)(osGetMemRegionFree(MEMREGION_APPLICATION) >> 10));
        
    }
    static std::atomic < bool > g_core1Next
    {
        false
    } ;
    extern "C" void n3dsPreferCore1Next(void) 
    {
        static const bool on = [] 
        {
            Performance3ds c;
            if (FILE * f = std::fopen(N3DS_ROOT "/performance.ini",
"r")) 
            {
                c.read(f);
                std::fclose(f);
                
            }
            return c.soundOnCore1;
            
        }
        ();
        if (on) g_core1Next.store(true);
        
    }
    int __wrap___syscall_thread_create(struct __pthread_t * * thread,
void * ( * func)(void * ),
void * arg,
void * stackAddr,
size_t stackSize) 
    {
        if (stackAddr) return EINVAL;
        s32 prio = 0x30;
        svcGetThreadPriority(&prio,
CUR_THREAD_HANDLE);
        if (g_core1Next.exchange(false)) 
        {
            static Result limit = APT_SetAppCpuTimeLimit(80);
            Thread t = R_SUCCEEDED(limit) ? threadCreate(reinterpret_cast < ThreadFunc > (func),
arg,
stackSize ? stackSize : 0x8000,
prio < 0x30 ? 0x30 : prio,
1,
false) : nullptr;
            Result raised = 0;
            if (t 
&& prio < 0x30) raised = svcSetThreadPriority(threadGetHandle(t),
prio);
            n3dsLog("[threads] audio thread -> core 1: time limit %08lx, create %s, priority 0x%lx raise %08lx\n",
(unsigned long)limit,
t ? "ok" : "REFUSED (normal placement)",
(long)prio,
(unsigned long)raised);
            if (t) 
            {
                 * thread = reinterpret_cast < struct __pthread_t * > (t);
                return 0;
                
            }
            
        }
        int core = threadCore();
        if (g_nextThreadStack) 
        {
            stackSize = std::max(stackSize,
g_nextThreadStack);
            g_nextThreadStack = 0;
            Performance3ds config;
            if (FILE * f = std::fopen(N3DS_ROOT "/performance.ini",
"r")) 
            {
                config.read(f);
                std::fclose(f);
                
            }
            if (config.commandCore != 1) 
            {
                if (Thread t = threadCreate(reinterpret_cast < ThreadFunc > (func),
arg,
stackSize,
prio,
config.commandCore,
false)) 
                {
                    n3dsLog("[threads] FIFO worker prio 0x%lx stack %lu KB on core %d\n",
(long)prio,
(unsigned long)(stackSize >> 10),
config.commandCore);
                     * thread = reinterpret_cast < struct __pthread_t * > (t);
                    return 0;
                    
                }
                n3dsLog("[threads] FIFO worker: core %d refused, falling back\n",
config.commandCore);
                
            }
            
        }
        static bool core1Refused = [] 
        {
            const char * v = std::getenv("N3DS_TEST_BG_CORE");
            return v 
&& * v == '1';
            
        }
        ();
        static const int backgroundCore = [] 
        {
            Performance3ds c;
            if (FILE * f = std::fopen(N3DS_ROOT "/performance.ini",
"r")) 
            {
                c.read(f);
                std::fclose(f);
                
            }
            return c.backgroundCore;
            
        }
        ();
        if (core == 1 
&& core1Refused) core = backgroundCore >= 2 ? backgroundCore : 0;
        const s32 wantPrio = prio;
        const bool raiseAfter = core == 1 
&& prio < 0x30;
        Thread t = threadCreate(reinterpret_cast < ThreadFunc > (func),
arg,
stackSize ? stackSize : 0x8000,
raiseAfter ? 0x30 : prio,
core,
false);
        if (t 
&& raiseAfter) 
        {
            const Result rr = svcSetThreadPriority(threadGetHandle(t),
wantPrio);
            static int raised = 0;
            if (raised < 40) 
            {
                raised++;
                n3dsLog("[threads] core 1: created at 0x30, raised to 0x%lx: %08lx\n",
(long)wantPrio,
(unsigned long)rr);
                
            }
            
        }
        int used = core;
        if (!t 
&& core == 1) core1Refused = true;
        if (!t 
&& core == 1 
&& backgroundCore >= 2) 
        {
            t = threadCreate(reinterpret_cast < ThreadFunc > (func),
arg,
stackSize ? stackSize : 0x8000,
prio,
backgroundCore,
false);
            if (t) used = backgroundCore;
            
        }
        if (!t 
&& core != 0) 
        {
            t = threadCreate(reinterpret_cast < ThreadFunc > (func),
arg,
stackSize ? stackSize : 0x8000,
prio,
0,
false);
            used = 0;
            
        }
        
        {
            static int logged = 0;
            if (logged < 40) 
            {
                logged++;
                n3dsLog("[threads] pthread %d prio 0x%lx stack %lu KB on core %d%s\n",
logged,
(long)prio,
(unsigned long)(stackSize >> 10),
used,
t ? "" : " (failed)");
                
            }
            
        }
        if (!t || (used != core 
&& used < 2)) threadFailureReport(prio,
core);
        if (!t) return ENOMEM;
         * thread = reinterpret_cast < struct __pthread_t * > (t);
        return 0;
        
    }
    int n3dsPushCreatePriority(int gcPriority) 
    {
        s32 old = 0x30;
        svcGetThreadPriority(&old,
CUR_THREAD_HANDLE);
        n3dsSetThreadPriority(0,
gcPriority);
        return old;
        
    }
    void n3dsPopCreatePriority(int saved) 
    {
        svcSetThreadPriority(CUR_THREAD_HANDLE,
saved);
        
    }
    void n3dsYield(void) 
    {
        g_n3dsMark[6] = g_n3dsMark[6] + 1;
        svcSleepThread(50000);
        
    }
    int n3dsAptLoop(void) 
    {
        static const unsigned quitFrame = []()
        {
            const char * v = std::getenv("N3DS_TEST_QUIT_FRAME");
            return v?unsigned(std::strtoul(v,
nullptr,
10)):0u;
            
        }
        ();
        if(quitFrame 
&& g_frames.load() >= quitFrame)
        {
            n3dsLog("[exit] requested diagnostic quit at frame %u\n",
g_frames.load());
            return 0;
            
        }
        return aptMainLoop() ? 1 : 0;
        
    }
    namespace 
    {
        struct AutoPress 
        {
            uint64_t first, count;
            u32 keys;
            int sx, sy;
            
        } ;
        std::vector < AutoPress > g_auto;
        bool g_autoLoaded = false;
        void loadAutoPress() 
        {
            g_autoLoaded = true;
            FILE * f = std::fopen(N3DS_ROOT "/autopress.txt",
"r");
            if (!f) return;
            static const struct 
            {
                const char * n;
                u32 k;
                
            }
            names[] = 
            {
                
                {
                    "A", KEY_A
                }
                , 
                {
                    "B", KEY_B
                }
                , 
                {
                    "X", KEY_X
                }
                , 
                {
                    "Y", KEY_Y
                }
                , 
                {
                    "L", KEY_L
                }
                , 
                {
                    "R", KEY_R
                }
                , 
                {
                    "ZL", KEY_ZL
                }
                , 
                {
                    "ZR", KEY_ZR
                }
                , 
                
                {
                    "START", KEY_START
                }
                , 
                {
                    "SELECT", KEY_SELECT
                }
                , 
                {
                    "DUP", KEY_DUP
                }
                , 
                {
                    "DDOWN", KEY_DDOWN
                }
                , 
                {
                    "DLEFT", KEY_DLEFT
                }
                , 
                {
                    "DRIGHT", KEY_DRIGHT
                }
                , 
                {
                    "NONE", 0
                }
                
            } ;
            char line[256];
            while (std::fgets(line,
sizeof line,
f)) 
            {
                unsigned long long a = 0, n = 0;
                char keys[128] = 
                {
                    
                } ;
                int sx = 0, sy = 0;
                const int got = std::sscanf(line,
"%llu %llu %127s %d %d",
&a,
&n,
keys,
&sx,
&sy);
                if (got < 3) continue;
                AutoPress P
                {
                    a, n, 0, sx, sy
                } ;
                for (char* t = std::strtok(keys,
"+"); t; t = std::strtok(nullptr,
"+"))
                for (const auto& nm : names) if (std::strcmp(t,
nm.n) == 0) P.keys |= nm.k;
                g_auto.push_back(P);
                
            }
            std::fclose(f);
            n3dsLog("[3ds] autopress: %zu entries\n",
g_auto.size());
            
        }
        
    }
    void n3dsHidRead(N3dsPad * p) 
    {
        hidScanInput();
        u32 k = hidKeysHeld();
        static u32 menuHeld = 0;
        static bool menuTestDone = false;
        static const bool menuTestRequested = std::getenv("N3DS_SETTINGS_TEST") != nullptr;
        if((hidKeysDown()&KEY_TOUCH) 
&& g_tuning.lowerScreen 
&& !n3ds_ui::gpuOwnsBottom.load(std::memory_order_acquire))
        {
            touchPosition t
            {
                
            } ;
            hidTouchRead(&t);
            if(n3ds_panel::kFpsCap24.contains(t.px,
t.py))n3ds_panel::setFpsCap(24);
            else if(n3ds_panel::kFpsCap30.contains(t.px,
t.py))n3ds_panel::setFpsCap(30);
            else if(n3ds_panel::kFpsCap60.contains(t.px,
t.py))n3ds_panel::setFpsCap(60);
            
        }
        const bool menuTest = !menuTestDone 
&& menuTestRequested 
&& g_inputFrames.load() >= 620;
        if(menuTest)
        {
            menuTestDone = true;
            const bool hadAccel = n3ds_motion::enabled[1], hadGyro = n3ds_motion::enabled[2];
            n3dsMotionStop();
            try
            {
                ww_screen::runMenu(menuTest);
                
            } catch(...)
            {
                n3dsLog("[settings] unable to open or save (allocation failure)\n");
                
            }
            if(hadAccel)n3dsMotionEnable(1,
1);
            if(hadGyro)n3dsMotionEnable(2,
1);
            hidScanInput();
            menuHeld = hidKeysHeld();
            std::memset(p,
0,
sizeof * p);
            return;
            
        }
        menuHeld&= k;
        k&= ~menuHeld;
        if ((hidKeysDown() & KEY_SELECT) 
&& !(k & KEY_START)) g_captureRequest.store(true);
        if (!g_autoLoaded) loadAutoPress();
        int autoX = 0, autoY = 0;
        for (const AutoPress& a : g_auto)
        if (g_inputFrames.load() >= a.first 
&& g_inputFrames.load() < a.first + a.count) 
        {
            k |= a.keys;
            if (a.sx || a.sy) 
            {
                autoX = a.sx;
                autoY = a.sy;
                
            }
            
        }
        circlePosition c, cs;
        hidCircleRead(&c);
        hidCstickRead(&cs);
        std::memset(p,
0,
sizeof * p);
        p->keys = k;
        int sx = c.dx, sy = c.dy;
        constexpr int kDpadFull = 150;
        if ((k & KEY_DLEFT) 
&& !(k & KEY_DRIGHT)) sx = - kDpadFull;
        else if ((k & KEY_DRIGHT) 
&& !(k & KEY_DLEFT)) sx = kDpadFull;
        if ((k & KEY_DUP) 
&& !(k & KEY_DDOWN)) sy = kDpadFull;
        else if ((k & KEY_DDOWN) 
&& !(k & KEY_DUP)) sy = - kDpadFull;
        p->stickX = int16_t(sx);
        p->stickY = int16_t(sy);
        if (autoX || autoY) 
        {
            p->stickX = int16_t(autoX);
            p->stickY = int16_t(autoY);
            
        }
        p->cstickX = cs.dx;
        p->cstickY = cs.dy;
        
    }
    uint64_t n3dsTicks(void) 
    {
        return svcGetSystemTick();
        
    }
    double n3dsTicksPerSecond(void) 
    {
        return double(SYSCLOCK_ARM11);
        
    }
    
}
extern "C" int n3dsReadClearDepth(uint16_t gx,
uint16_t gy) 
{
    struct Query 
    {
        uint16_t x, y;
        int clear;
        
    }
    q
    {
        gx, gy, 0
    } ;
    if (gx >= 640 || gy >= 480 || !g_tgt) return 0;
    auto read = [](void * arg) 
    {
        auto& q = * static_cast < Query * > (arg);
        if (!g_tgt || !g_tgt->frameBuf.depthBuf || g_frames.load() == 0) return;
        C3Di_RenderQueueWaitDone();
        const float squeeze = 0.8f;
        const int lx = int(200.0f + (float(q.x) - 320.0f) * (400.0f / 640.0f) * squeeze);
        const int ly = int(float(q.y) * (240.0f / 480.0f));
        if (lx < 0 || lx >= 400 || ly < 0 || ly >= 240) return;
        const unsigned bx = 239 - ly, by = lx;
        unsigned morton = 0;
        for (unsigned b=0;b<3;b++)morton|= ((bx >> b)&1) << (2 * b)|((by >> b)&1) << (2 * b + 1);
        const size_t offset = ((by >> 3) * 30 + (bx >> 3)) * 64 + morton;
        const auto * depth = static_cast < const uint32_t * > (g_tgt->frameBuf.depthBuf);
        const uint32_t raw = depth[offset];
        q.clear = g_depthVisibility.isFar(raw);
        
        {
            static unsigned logged = 0;
            if(logged++ < 24)
            n3dsLog("[depth-peek] game %u,%u screen %d,%d raw %08lx clear %d\n",
q.x,
q.y,
lx,
ly,
(unsigned long)raw,
q.clear);
            
        }
        
    } ;
    if(n3dsRenderThreadActive()) n3dsRenderThreadJob(read,
&q);
    else read(&q);
    return q.clear;
    
}
extern "C" uint32_t n3dsRenderedFrames(void) 
{
    return g_frames.load();
    
}
extern "C" int n3dsReadCopyRGBA(uint32_t addr,
std::vector < uint8_t > * out,
int * outW,
int * outH) 
{
    struct Job 
    {
        uint32_t addr;
        std::vector < uint8_t > * out;
        int w, h, ok;
        
    }
    j
    {
        addr, out, 0, 0, 0
    } ;
    auto run = [](void * arg) 
    {
        Job& j = * static_cast < Job * > (arg);
        if (!g_r) return;
        C3Di_RenderQueueWaitDone();
        const t2p::TexEntry * e = g_r->cache().renderedAt(j.addr);
        if (!e || !e->user) return;
        const void * data = nullptr;
        uint32_t bytes = 0;
        if (!g_r->backend().textureImage( * e,
&data,
&bytes)) return;
        const int W = e->tex.width, H = e->tex.height, pw = e->portW, ph = e->portH, fmt = e->tex.fmt;
        const int bpp = fmt == t2p::PTF_RGBA8 ? 4 : fmt == t2p::PTF_RGB565 || fmt == t2p::PTF_RGBA4 ? 2 : 0;
        if (!bpp || pw <= 0 || ph <= 0 || pw > W || ph > H || uint32_t(W * H * bpp) > bytes) return;
        static int flip = - 1;
        if (flip < 0) 
        {
            const char * v = std::getenv("N3DS_RB_FLIP");
            flip = !(v 
&& * v == '1');
            
        }
        const uint8_t * src = static_cast < const uint8_t * > (data);
        j.out->assign(size_t(pw) * ph * 4,
0);
        for (int y = 0; y < ph; y++) 
        {
            const int v = flip ? y : H - 1 - y;
            for (int x = 0; x < pw; x++) 
            {
                unsigned morton = 0;
                for (unsigned b = 0; b < 3; b++) morton |= ((unsigned(x) >> b) & 1) << (2 * b) | ((unsigned(v) >> b) & 1) << (2 * b + 1);
                const size_t idx = (size_t(v >> 3) * (W >> 3) + (x >> 3)) * 64 + morton;
                uint8_t * o = j.out->data() + (size_t(y) * pw + x) * 4;
                if (bpp == 4) 
                {
                    const uint8_t * p = src + idx * 4;
                    o[0] = p[3];
                    o[1] = p[2];
                    o[2] = p[1];
                    o[3] = p[0];
                    
                }
                else 
                {
                    uint16_t t;
                    std::memcpy(&t,
src + idx * 2,
2);
                    if (fmt == t2p::PTF_RGB565) 
                    {
                        o[0] = uint8_t((t >> 11) * 255 / 31);
                        o[1] = uint8_t(((t >> 5) & 63) * 255 / 63);
                        o[2] = uint8_t((t & 31) * 255 / 31);
                        o[3] = 255;
                        
                    }
                    else 
                    {
                        o[0] = uint8_t((t >> 12) * 17);
                        o[1] = uint8_t(((t >> 8) & 15) * 17);
                        o[2] = uint8_t(((t >> 4) & 15) * 17);
                        o[3] = uint8_t((t & 15) * 17);
                        
                    }
                    
                }
                
            }
            
        }
        j.w = pw;
        j.h = ph;
        j.ok = 1;
        
    } ;
    if (n3dsRenderThreadActive()) n3dsRenderThreadJob(run,
&j);
    else run(&j);
    if (j.ok) 
    {
         * outW = j.w;
         * outH = j.h;
        n3dsLog("[readback] copy %08lx: %dx%d\n",
(unsigned long)addr,
j.w,
j.h);
        
    }
    else n3dsLog("[readback] copy %08lx: none\n",
(unsigned long)addr);
    return j.ok;
    
}
#include "ww_ui_route_3ds.h"
extern "C" void GXInsertDebugMarker(const char * label);
extern "C" int n3dsUiMapGetSourceRect(int * left,
int * top,
int * width,
int * height) __attribute__((weak));
static bool g_mapRouteOpen = false;
extern "C" int n3dsUiMapRoutedToBottom(void) 
{
    return g_tuning.mapOnLower 
&& g_tuning.lowerScreen 
&& g_r;
    
}
extern "C" bool n3dsUiMapBegin(int left,
int top,
int width,
int height) 
{
    if (!g_tuning.mapOnLower || !g_tuning.lowerScreen || !g_r || g_mapRouteOpen || !n3dsUiMapGetSourceRect) return false;
    if (width <= 0 || height <= 0 || left < 0 || top < 0 || left + width > 320 || top + height > 240) return false;
    int sl = 0, st = 0, sw = 0, sh = 0;
    if (!n3dsUiMapGetSourceRect(&sl,
&st,
&sw,
&sh) || sw <= 0 || sh <= 0 || sw > 4096 || sh > 4096 || sl < - 4096 || sl > 4096 || st < - 4096 || st > 4096) return false;
    char m[96];
    std::snprintf(m,
sizeof m,
"\x01WWROUTE1 B %d %d %d %d %d %d %d %d",
sl,
st,
sw,
sh,
left,
top,
width,
height);
    GXInsertDebugMarker(m);
    g_mapRouteOpen = true;
    return true;
    
}
extern "C" void n3dsUiMapEnd(void) 
{
    if (!g_mapRouteOpen) return;
    GXInsertDebugMarker("\x01WWROUTE1 E");
    g_mapRouteOpen = false;
    
}
extern "C" int n3dsBatchFx(void) 
{
    return g_tuning.fxBatch ? 1 : 0;
    
}
static bool g_paintSkipped = false;
static uint32_t g_logicFrames = 0, g_skippedFrames = 0, g_drawsSkipped = 0;
static uint64_t g_skipIdeal = 0;
static int g_skipRun = 0;
static bool g_skipDecided = false, g_skipNext = false;
static uint64_t g_drawStart = 0;
static int64_t g_drawEma = 0;
static bool g_actorDrawSkipped = false;
static bool skipForced()
{
    static const bool force = std::getenv("N3DS_SKIP_FORCE") != nullptr;
    return force;
    
}
static int64_t logicPeriod()
{
    return int64_t(SYSCLOCK_ARM11 / (n3ds_panel::fpsCap.load(std::memory_order_relaxed) == 60?60:30));
    
}
static int legacyBeginLogicFrame(void) 
{
    const uint64_t now = svcGetSystemTick();
    const int64_t period = logicPeriod();
    if(!g_skipIdeal)g_skipIdeal = now;
    int64_t lag = int64_t(now - g_skipIdeal);
    if(lag > 4 * period || lag < - period)
    {
        g_skipIdeal = now;
        lag = 0;
        
    }
    bool skip = skipForced() ? g_skipRun < 1 : g_tuning.skippedFrames > 0 
&& lag > period / 2 
&& g_skipRun < g_tuning.skippedFrames;
    if(g_skipDecided)
    {
        skip = g_skipNext;
        g_skipDecided = false;
        
    }
    g_skipRun = skip?g_skipRun + 1:0;
    g_skipIdeal += uint64_t(period);
    g_paintSkipped = skip;
    ++g_logicFrames;
    if(skip)++g_skippedFrames;
    if(g_logicFrames % 300 == 0)
    {
        n3dsLog("[frame-skip] logic frames %lu, not painted %lu of the last 300, draw passes left out %lu\n",
(unsigned long)g_logicFrames,
(unsigned long)g_skippedFrames,
(unsigned long)g_drawsSkipped);
        g_skippedFrames = 0;
        g_drawsSkipped = 0;
        
    }
    return skip?1:0;
    
}
static int legacySkipDrawPass(void) 
{
    g_drawStart = svcGetSystemTick();
    g_actorDrawSkipped = false;
    if(!g_tuning.omitDraw || (!skipForced() 
&& g_tuning.skippedFrames <= 0))return 0;
    const int64_t period = logicPeriod();
    const int64_t lag = int64_t(g_drawStart - g_skipIdeal) + g_drawEma;
    const bool skip = skipForced() ? g_skipRun < 1 : lag > period / 2 
&& lag <= 4 * period 
&& g_skipRun < g_tuning.skippedFrames;
    g_skipDecided = true;
    g_skipNext = skip;
    if(skip)++g_drawsSkipped;
    g_actorDrawSkipped = skip;
    return skip?1:0;
    
}
extern "C" void n3dsPaintEnded(void) 
{
    g_paintEndedN.fetch_add(1,
std::memory_order_relaxed);
    
}
extern "C" void n3dsPaintHandedOff(void) 
{
    g_paintHandedN.fetch_add(1,
std::memory_order_release);
    
}
extern "C" uint64_t n3dsRenderRemainingTicks(uint64_t now);
extern "C" uint64_t n3dsRenderFrameTicks(void);
static std::atomic < uint32_t > g_procSlow
{
    0
}
, g_procDev
{
    0
} ;
extern "C" void n3dsPacingNoteProcess(uint64_t ticks) 
{
    const uint32_t d = ticks > 0xFFFFFFFFull ? 0xFFFFFFFFu : uint32_t(ticks);
    const uint32_t a = g_procSlow.load(std::memory_order_relaxed);
    if (!a) 
    {
        g_procSlow.store(d,
std::memory_order_relaxed);
        return;
        
    }
    const uint32_t na = uint32_t((uint64_t(a) * 15 + d) / 16);
    g_procSlow.store(na,
std::memory_order_relaxed);
    const uint32_t up = d > na ? d - na : 0;
    g_procDev.store(uint32_t((uint64_t(g_procDev.load(std::memory_order_relaxed)) * 15 + up) / 16),
std::memory_order_relaxed);
    
}
extern "C" 
{
    uint64_t g_presentHeldTicks = 0;
    
}
extern "C" void n3dsPresentPace(void) 
{
    if (!g_tuning.pacePaint || !g_tuning.pacePresent || n3ds_panel::fpsCap.load(std::memory_order_relaxed) == 60) return;
    const uint64_t slot = g_renderSlot;
    if (!slot) return;
    static double ema = 0, dev = 0;
    static uint64_t slept = 0;
    static unsigned n = 0, held = 0;
    const uint64_t now = svcGetSystemTick();
    const double period = double(SYSCLOCK_ARM11) / 30.0;
    double lat = double(int64_t(now - slot));
    if (lat < 0) lat = 0;
    if (lat > 6 * period) 
    {
        ema = dev = 0;
        return;
        
    }
    if (ema > 0) 
    {
        const double target = ema + 2 * dev;
        if (lat < target) 
        {
            double w = target - lat;
            const double cap = double(SYSCLOCK_ARM11) * 0.030;
            if (w > cap) w = cap;
            svcSleepThread(int64_t(w * 1e9 / SYSCLOCK_ARM11));
            slept += uint64_t(w);
            ++held;
            g_presentHeldTicks = uint64_t(w);
            
        }
        ema = ema * 15.0 / 16.0 + lat / 16.0;
        const double up = lat > ema ? lat - ema : 0;
        dev = dev * 15.0 / 16.0 + up / 16.0;
        
    } else ema = lat;
    if (++n == 300) 
    {
        n3dsLog("[present-pace] 300 frames: render done %.1f ms after its tick (+%.1f dev), held %u frames, %.1f ms each\n",
ema * 1000.0 / SYSCLOCK_ARM11,
dev * 1000.0 / SYSCLOCK_ARM11,
held,
held ? double(slept) * 1000.0 / SYSCLOCK_ARM11 / held : 0.0);
        n = held = 0;
        slept = 0;
        
    }
    
}
extern "C" int n3dsOwnsPacing(void) 
{
    return g_tuning.pacePaint ? 1 : 0;
    
}
namespace pacing 
{
    n3ds::PaintBudget paintBudget;
    int since = 0;
    int k = 1;
    int late = 0, early = 0, paints = 0, firstReady = 0;
    uint64_t prevBegin = 0;
    uint64_t winStart = 0;
    uint32_t winTicks = 0, winPaints = 0, winForced = 0, winNotReady = 0, winKSum = 0;
    uint64_t sleptTicks = 0, writtenOff = 0, lagSum = 0, lagMax = 0;
    uint32_t gapHist[9] = 
    {
        
    } ;
    bool ready(uint64_t now,
int64_t horizon) 
    {
        const uint32_t inflight = g_paintEndedN.load(std::memory_order_relaxed) - g_paintHandedN.load(std::memory_order_acquire);
        if (inflight == 0) return true;
        const int64_t margin = int64_t(SYSCLOCK_ARM11 / 1000);
        return int64_t(n3dsRenderRemainingTicks(now)) + margin <= horizon;
        
    }
    void adapt(int gap) 
    {
        const double period = double(logicPeriod());
        const double need = double(std::max < uint64_t > (n3dsRenderFrameTicks(),
uint64_t(g_procSlow.load(std::memory_order_relaxed)) + g_procDev.load(std::memory_order_relaxed)));
        int want = 1;
        const int kMax = std::min(4,
g_tuning.maxPaintGap);
        while (want < kMax 
&& need > double(want) * period * 0.95) ++want;
        if (want > k) 
        {
            k = want;
            early = 0;
            late = 0;
            paints = 0;
            return;
            
        }
        if (gap > k) ++late;
        if (++paints >= 8) 
        {
            if (late >= 4 
&& k < kMax) 
            {
                ++k;
                early = 0;
                
            }
            paints = 0;
            late = 0;
            
        }
        if (k > 1 
&& need < double(k - 1) * period * 0.85) 
        {
            if (++early >= 30) 
            {
                --k;
                early = 0;
                paints = 0;
                late = 0;
                
            }
            
        }
        else early = 0;
        
    }
    bool decide(uint64_t now,
int64_t horizon,
int64_t lag) 
    {
        const int64_t period = logicPeriod();
        const int gap = since + 1;
        const bool r = ready(now,
horizon);
        if (r 
&& !firstReady) firstReady = gap;
        const int K = g_tuning.cadencePaint ? g_tuning.cadencePaint : k;
        const bool catchUp = lag > 2 * period;
        const int need = catchUp ? K + 1 : K;
        const bool forced = gap >= g_tuning.maxPaintGap;
        bool paint;
        if(g_tuning.fractionalPaint 
&& !g_tuning.cadencePaint)
        {
            const uint64_t work = std::max < uint64_t > (n3dsRenderFrameTicks(),
uint64_t(g_procSlow.load(std::memory_order_relaxed)) + g_procDev.load(std::memory_order_relaxed));
            const int64_t cost = std::min < int64_t > (period * g_tuning.maxPaintGap,
std::max < int64_t > (period,
int64_t(work + work / 20)));
            paint = paintBudget.decide(period,
cost,
r,
forced,
catchUp);
            
        } else paint = (r 
&& gap >= need) || forced;
        if (!paint) 
        {
            if (gap >= need) ++winNotReady;
            return false;
            
        }
        if (forced 
&& !r) ++winForced;
        ++gapHist[gap < 8 ? gap : 8];
        if (!g_tuning.cadencePaint) adapt(gap);
        firstReady = 0;
        return true;
        
    }
    
}
extern "C" long long OSGetTime(void);
extern "C" unsigned int VIGetRetraceCount(void);
extern "C" int n3dsBeginLogicFrame(void) 
{
    if (!g_tuning.pacePaint) return legacyBeginLogicFrame();
    
    {
        static bool logged = false;
        if(!logged)
        {
            logged = true;
            n3dsLog("[pacing] paint_fractional %d\n",
int(g_tuning.fractionalPaint));
            
        }
        
    }
    using namespace pacing;
    const int64_t period = logicPeriod();
    const bool doubleSpeed = n3ds_panel::fpsCap.load(std::memory_order_relaxed) == 60;
    uint64_t now = svcGetSystemTick();
    if (!g_skipIdeal) g_skipIdeal = now;
    int64_t lag = int64_t(now - g_skipIdeal);
    if (lag < 0) 
    {
        const int64_t wait = - lag < 2 * period ? - lag : 2 * period;
        svcSleepThread(int64_t(wait * 1000000000.0 / SYSCLOCK_ARM11));
        sleptTicks += uint64_t(wait);
        now = svcGetSystemTick();
        lag = int64_t(now - g_skipIdeal);
        if (lag < - period) 
        {
            g_skipIdeal = now;
            lag = 0;
            
        }
        
    }
    const int64_t slip = doubleSpeed ? 6 * period : 2 * period;
    if (lag > slip 
&& lag <= 8 * period) 
    {
        writtenOff += uint64_t(lag - period);
        g_skipIdeal = now - uint64_t(period);
        lag = period;
        
    }
    if (lag > 8 * period) 
    {
        n3dsLog("[pacing] stall: logic frame %lu began %.0f ms late (previous tick took %.0f ms, painted %d)\n",
(unsigned long)g_logicFrames,
double(lag) * 1000.0 / SYSCLOCK_ARM11,
        pacing::prevBegin ? double(now - pacing::prevBegin) * 1000.0 / SYSCLOCK_ARM11 : 0.0,
int(!g_paintSkipped));
        writtenOff += uint64_t(lag);
        g_skipIdeal = now;
        lag = 0;
        
    }
    if (lag < 0) lag = 0;
    prevBegin = now;
    bool skip;
    if (skipForced()) skip = since < 1 ? true : false;
    else if (g_skipDecided) skip = g_skipNext;
    else skip = !decide(now,
0,
lag);
    if (n3ds_panel::fpsCap.load(std::memory_order_relaxed) == 24)
    {
        static uint64_t lastCap = 0;
        static int phase = 0;
        const uint64_t dt = lastCap?now - lastCap:0;
        lastCap = now;
        if(dt > SYSCLOCK_ARM11 / 24) phase = 0;
        else 
        {
            if((phase % 5) == 4) skip = true;
            phase = (phase + 1) % 5;
            
        }
        
    }
    g_skipDecided = false;
    if (!skip) g_paintSlotRing[g_paintEndedN.load(std::memory_order_relaxed) & 7] = g_skipIdeal;
    since = skip ? since + 1 : 0;
    g_skipRun = skip ? g_skipRun + 1 : 0;
    g_skipIdeal += uint64_t(period);
    g_paintSkipped = skip;
    ++g_logicFrames;
    if (skip) ++g_skippedFrames;
    lagSum += uint64_t(lag);
    if (uint64_t(lag) > lagMax) lagMax = uint64_t(lag);
    if (!winStart) winStart = now;
    ++winTicks;
    if (!skip) ++winPaints;
    winKSum += uint32_t(g_tuning.cadencePaint ? g_tuning.cadencePaint : k);
    if (winTicks == 300) 
    {
        const double ms = 1000.0 / SYSCLOCK_ARM11;
        const double el = double(now - winStart) * ms;
        n3dsLog("[pacing] 300 ticks in %.0f ms: game speed %.0f%%, painted %u (%.1f fps), cadence K avg %.2f, paint gaps 1:%u 2:%u 3:%u 4:%u 5:%u 6+:%u, "
        "waited for renderer %u, renderer busy at cadence %u, slept %.0f ms, lag mean %.1f max %.1f ms, written off %.0f ms\n",
        el,
el > 0 ? 300.0 * 1000.0 / 30.0 / el * 100.0 : 0.0,
winPaints,
el > 0 ? winPaints * 1000.0 / el : 0.0,
winKSum / 300.0,
        gapHist[1],
gapHist[2],
gapHist[3],
gapHist[4],
gapHist[5],
gapHist[6] + gapHist[7] + gapHist[8],
        winForced,
winNotReady,
double(sleptTicks) * ms,
double(lagSum) * ms / 300,
double(lagMax) * ms,
double(writtenOff) * ms);
        if(g_tuning.fractionalPaint)n3dsLog("[pacing-budget] fractional on; credit %.2f ms (K above is the unused legacy estimate)\n",
double(paintBudget.credit) * ms);
        
        {
            static int64_t osPrev = 0;
            static uint64_t sysPrev = 0;
            const int64_t os = OSGetTime();
            if (osPrev) n3dsLog("[clock] OSGetTime advanced %.0f ms, 3DS system clock %.0f ms, retraces %u\n",
double(os - osPrev) / 40500.0,
            double(now - sysPrev) * 1000.0 / SYSCLOCK_ARM11,
(unsigned)VIGetRetraceCount());
            osPrev = os;
            sysPrev = now;
            
        }
        winStart = now;
        winTicks = winPaints = winForced = winNotReady = winKSum = 0;
        sleptTicks = writtenOff = lagSum = lagMax = 0;
        for (auto& g : gapHist) g = 0;
        
    }
    return skip ? 1 : 0;
    
}
extern "C" int n3dsSkipDrawPass(void) 
{
    if (!g_tuning.pacePaint) return legacySkipDrawPass();
    g_drawStart = svcGetSystemTick();
    g_actorDrawSkipped = false;
    if (!g_tuning.omitDraw) return 0;
    bool skip;
    if (skipForced()) skip = pacing::since == 0;
    else 
    {
        const int64_t untilSlot = int64_t(g_skipIdeal - g_drawStart);
        const int64_t horizon = g_drawEma > untilSlot ? g_drawEma : untilSlot;
        const int64_t lagNext = - untilSlot + g_drawEma;
        skip = !pacing::decide(g_drawStart,
horizon,
lagNext > 0 ? lagNext : 0);
        
    }
    g_skipDecided = true;
    g_skipNext = skip;
    if (skip) ++g_drawsSkipped;
    g_actorDrawSkipped = skip;
    return skip ? 1 : 0;
    
}
extern "C" int n3dsEarlyReleaseOn(void) 
{
    return g_tuning.releaseEarly ? 1 : 0;
    
}
extern "C" int n3dsRealShadowsOff(void) 
{
    return g_tuning.shadowsOn ? 0 : 1;
    
}
extern "C" int n3dsActorDrawSkipped(void) 
{
    return g_actorDrawSkipped ? 1 : 0;
    
}
extern "C" void n3dsDrawPassDone(void) 
{
    if(g_actorDrawSkipped)return;
    const int64_t t = int64_t(svcGetSystemTick() - g_drawStart);
    g_drawEma = g_drawEma?(g_drawEma * 7 + t) / 8:t;
    
}
extern "C" int n3dsPaintSkipped(void) 
{
    return g_paintSkipped ? 1 : 0;
    
}
extern "C" float n3dsFoliageFar(float original) 
{
    const int ab = g_foliageAb.load(std::memory_order_relaxed);
    int lim = ab >= 0 ? ab : g_tuning.farFoliage;
    // First-boot cover: cut tree/grass distance so cold frames stay nearer 10 FPS.
    if (n3ds_warm::covering() 
&& (lim <= 0 || lim > 900)) lim = 900;
    return lim > 0 
&& float(lim) < original ? float(lim) : original;
    
}
extern "C" int n3dsWarmCovering(void) 
{
    return n3ds_warm::coveringGate() ? 1 : 0;
    
}
extern "C" void n3dsOpeningPrepare(void) 
{
    if (!g_r || !g_r->ready()) return;
    if (C3Dx_InFrame() || (n3dsRenderThreadActive() 
&& n3dsRenderThreadBusy())) 
    {
        g_r->requestSceneDrop();
        n3dsLog("[opening] prepare: GPU busy, scene drop requested\n");
        return;
        
    }
    // Idle GPU + drop caches before Opening archive / Outset allocate into linear.
    C3Di_RenderQueueWaitDone();
    C3Dx_WaitGpu();
    g_r->dropSceneCaches();
    n3dsLog("[opening] prepare: GPU idle, scene caches dropped\n");
    
}
extern "C" int n3dsJ3dProgramsOn(void) 
{
    return g_tuning.j3dShaders ? 1 : 0;
    
}
extern "C" int n3dsJ3dGeometryOn(void) 
{
    return g_tuning.j3dMeshes ? 1 : 0;
    
}
extern "C" void n3dsSettingsRebase(uint64_t elapsed)
{
    if(g_skipIdeal)g_skipIdeal += elapsed;
    if(g_drawStart)g_drawStart += elapsed;
    if(pacing::prevBegin)pacing::prevBegin += elapsed;
    if(pacing::winStart)pacing::winStart += elapsed;
    for (auto& slot:g_paintSlotRing)if(slot)slot += elapsed;
    
}
