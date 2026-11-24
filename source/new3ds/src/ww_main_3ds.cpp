#include <cstdint>
namespace t2p 
{
    extern void ( * g_drawCacheMissLog)(const char * );
    extern uint64_t ( * g_compileClock)();
    
}
#include <cstddef>
extern void ( * g_texConvLog)(uint32_t,
int,
int,
int,
size_t,
bool,
int,
uint64_t);
#include <3ds.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <malloc.h>
#include <sys/stat.h>
#include <dirent.h>
#include <vector>
#include "ww_n3ds.h"
#include "ww_core2_replay_test_3ds.h"
#include "ww_disc_header_3ds.h"
#include "ww_ciso_3ds.h"
#include "ww_exit_order_3ds.h"
extern "C" int aurora_main(int argc,
char * argv[]);
extern "C" void aurora_shutdown(void);
extern "C" bool aurora_card_raw_list(const char * ,
const char * ,
const char * ,
void ( * )(const char * ,
void * ),
void * );
extern "C" size_t aurora_card_raw_extract(const char * ,
const char * ,
const char * ,
const char * ,
void * ,
size_t);
namespace 
{
    bool s_gameStarted = false;
    N3dsExitState s_exit;
    bool s_gfxLive = false;
    
}
extern "C" void __appExit(void);
extern "C" void userAppExit(void)
{
    if(!n3dsPrepareExit())return;
    if(!s_exit.mark(N3DS_EXIT_PREPARE))return;
    n3dsLog("[exit] cleanup begin game %d\n",
int(s_gameStarted));
    if(s_gameStarted)
    {
        if(s_exit.mark(N3DS_EXIT_CARD))
        {
            const int cardIdle = n3dsMemcardQuiesce(5000);
            n3dsLog("[exit] card idle %d\n",
cardIdle);
            
        }
        if(s_exit.mark(N3DS_EXIT_AUDIO))
        {
            n3dsAudioShutdown();
            n3dsLog("[exit] NDSP stopped\n");
            
        }
        if(s_exit.mark(N3DS_EXIT_GPU))
        {
            aurora_shutdown();
            n3dsLog("[exit] FIFO/render owner/GPU stopped\n");
            
        }
        
    }
    else if(s_exit.mark(N3DS_EXIT_GPU))
    {
        n3dsGpuShutdown();
        
    }
    if(s_gfxLive 
&& s_exit.mark(N3DS_EXIT_GFX))
    {
        gspWaitForVBlank();
        gfxExit();
        s_gfxLive = false;
        n3dsLog("[exit] gfx stopped; services next, heaps stay mapped\n");
        
    }
    
}
static void exitThreadReport(void)
{
    u32 ids[48];
    s32 n = 0;
    if(R_FAILED(svcGetThreadList(&n,
ids,
48,
CUR_PROCESS_HANDLE)) || n < 0 || n > 48)
    {
        n3dsLog("[exit] thread list unavailable\n");
        return;
        
    }
    u32 self = 0;
    svcGetThreadId(&self,
CUR_THREAD_HANDLE);
    n3dsLog("[exit] %ld threads alive (this one %lu):",
(long)n,
(unsigned long)self);
    for (s32 i=0;i<n
&&i<48;i++)
    {
        Handle h = 0;
        s32 core = - 9, prio = - 1;
        if(R_SUCCEEDED(svcOpenThread(&h,
CUR_PROCESS_HANDLE,
ids[i])))
        {
            svcGetThreadIdealProcessor(&core,
h);
            svcGetThreadPriority(&prio,
h);
            svcCloseHandle(h);
            
        }
        n3dsLog(" %lu/c%ld/p%lx",
(unsigned long)ids[i],
(long)core,
(long)prio);
        
    }
    n3dsLog("\n");
    
}
extern "C" void __appExit(void)
{
    static volatile int entered = 0;
    u32 caller = 0;
    svcGetThreadId(&caller,
CUR_THREAD_HANDLE);
    if(__atomic_exchange_n(&entered,
1,
__ATOMIC_ACQ_REL))
    {
        n3dsLog("[exit] second exit request from thread %lu ignored\n",
(unsigned long)caller);
        for (;;)svcSleepThread(1000000000LL);
        
    }
    n3dsLog("[exit] exit path on thread %lu\n",
(unsigned long)caller);
    userAppExit();
    exitThreadReport();
    if(s_exit.mark(N3DS_EXIT_APT))
    {
        n3dsLog("[exit] aptExit next\n");
        aptExit();
        
    }
    extern void ( * __system_retAddr)(void);
    n3dsLog("[exit] apt prepared; APT_CloseApplication %s, then the process ends (kernel closes hid, card, srv)\n",
__system_retAddr?"next":"not requested");
    n3dsCloseLog();
    if(__system_retAddr)__system_retAddr();
    svcExitProcess();
    
}
extern "C" [[noreturn]] void n3dsCleanExit(void)
{
    __appExit();
    __builtin_unreachable();
    
}
extern "C" [[noreturn]] void __libctru_exit(int)
{
    __appExit();
    __builtin_unreachable();
    
}
extern "C" size_t g_bigAllocLimit;
extern "C" void n3dsAllocSitesEnable();
namespace 
{
    extern bool g_bigLog;
    
}
u32 __stacksize__ = 0x200000;
u32 __ctru_linear_heap_size = 18u << 20;
extern "C" 
{
    bool __ctru_speedup = true;
    extern char * fake_heap_start;
    extern char * fake_heap_end;
    extern u32 __ctru_heap, __ctru_linear_heap, __ctru_heap_size;
    void __system_allocateHeaps(void) 
    {
        Handle reslimit = 0;
        if (R_FAILED(svcGetResourceLimit(&reslimit,
CUR_PROCESS_HANDLE))) svcBreak(USERBREAK_PANIC);
        s64 maxCommit = 0, currentCommit = 0;
        ResourceLimitType type = RESLIMIT_COMMIT;
        svcGetResourceLimitLimitValues(&maxCommit,
reslimit,
&type,
1);
        svcGetResourceLimitCurrentValues(&currentCommit,
reslimit,
&type,
1);
        svcCloseHandle(reslimit);
        const u32 remaining = u32(maxCommit - currentCommit) & ~0xFFFu;
        const u32 slack = 1u << 20;
        if (remaining < __ctru_linear_heap_size + slack + (16u << 20)) svcBreak(USERBREAK_PANIC);
        __ctru_heap_size = remaining - __ctru_linear_heap_size - slack;
        if (R_FAILED(svcControlMemory(&__ctru_heap,
OS_HEAP_AREA_BEGIN,
0x0,
__ctru_heap_size,
MEMOP_ALLOC,
MemPerm(MEMPERM_READ | MEMPERM_WRITE)))) svcBreak(USERBREAK_PANIC);
        if (R_FAILED(svcControlMemory(&__ctru_linear_heap,
0x0,
0x0,
__ctru_linear_heap_size,
MEMOP_ALLOC_LINEAR,
MemPerm(MEMPERM_READ | MEMPERM_WRITE)))) svcBreak(USERBREAK_PANIC);
        mappableInit(OS_MAP_AREA_BEGIN,
OS_MAP_AREA_END);
        fake_heap_start = reinterpret_cast < char * > (__ctru_heap);
        fake_heap_end = fake_heap_start + __ctru_heap_size;
        
    }
    
}
int main(int,
char * * ) 
{
    n3dsInit();
    s_gfxLive = true;
    n3dsSetBootStage("boot");
    if(core2test::requested()) return core2test::run();
    #ifdef N3DS_RENDER_OPT
n3dsLog("[build] renderer compiler O%d LTO %d, strict floating point\n",
N3DS_RENDER_OPT,
N3DS_RENDER_LTO);
    #endif
bool cpuIsNew = false;
    const Result cpuModelResult = APT_CheckNew3DS(&cpuIsNew);
    if(R_SUCCEEDED(cpuModelResult) 
&& cpuIsNew)
    {
        const Result cpuServiceResult = ptmSysmInit();
        const Result cpuConfigureResult = R_SUCCEEDED(cpuServiceResult)?PTMSYSM_ConfigureNew3DSCPU(3):cpuServiceResult;
        if(R_SUCCEEDED(cpuServiceResult))ptmSysmExit();
        n3dsLog("[cpu-mode] New3DS speedup 1 requested clock/L2 3 service %08lx configure %08lx\n",
(unsigned long)cpuServiceResult,
(unsigned long)cpuConfigureResult);
        
    } else n3dsLog("[cpu-mode] New3DS check %08lx result %d\n",
(unsigned long)cpuModelResult,
int(cpuIsNew));
    
    {
        static const char * marker = N3DS_ROOT "/relaunch.tmp";
        const u32 appMem = osGetMemRegionSize(MEMREGION_APPLICATION);
        struct stat ms;
        const bool tried = stat(marker,
&ms) == 0;
        if (appMem >= (90u << 20)) 
        {
            if (tried) remove(marker);
            
        }
        else 
        {
            u64 tid = 0;
            const Result idr = APT_GetProgramID(&tid);
            if (!tried 
&& R_SUCCEEDED(idr) 
&& (tid >> 32) == 0x00040000) 
            {
                if (FILE * f = std::fopen(marker,
"w")) std::fclose(f);
                n3dsLog("[3ds] only %lu KB of application memory: relaunching title %016llx once for the 124 MB mode\n",
(unsigned long)(appMem >> 10),
(unsigned long long)tid);
                if (R_SUCCEEDED(nsInit())) 
                {
                    const Result rr = NS_RebootToTitle(MEDIATYPE_SD,
tid,
6);
                    n3dsLog("[3ds] NS_RebootToTitle %08lx\n",
(unsigned long)rr);
                    if (R_SUCCEEDED(rr)) 
                    {
                        // Keep APT alive while the reboot takes over. If we are
                        // still here after a few seconds, fall through to the
                        // on-screen message instead of sleeping forever.
                        for (int i = 0; i < 50; i++) 
                        {
                            n3dsPumpApt();
                            if (!aptMainLoop()) break;
                            gspWaitForVBlank();
                            svcSleepThread(100000000LL);
                            
                        }
                        n3dsLog("[3ds] still running after NS_RebootToTitle; showing memory help\n");
                        
                    }
                    nsExit();
                    
                } else n3dsLog("[3ds] ns:s not available\n");
                aptSetChainloader(tid,
MEDIATYPE_SD);
                
            }
            remove(marker);
            {
                char msg[512];
                std::snprintf(msg,
sizeof msg,
"\nNot enough memory: the console gave this app %lu MB,\n"
"it needs the New 3DS 124 MB mode.\n\n"
"Start it from its HOME Menu icon (not from FBI or\n"
"another launcher). If this was the HOME Menu icon,\n"
"turn the console fully off and on and try again.\n\n"
"Press START to exit.\n",
(unsigned long)(appMem >> 20));
                n3dsShowBootError(msg);
                
            }
            n3dsWaitStartOrQuit();
            n3dsCleanExit();
            
        }
        
    }
    if (FILE * f = std::fopen(N3DS_ROOT "/env.txt",
"r")) 
    {
        char line[512];
        while (std::fgets(line,
sizeof line,
f)) 
        {
            char * eq = std::strchr(line,
'=');
            if (!eq || line[0] == '#') continue;
             * eq = 0;
            char * v = eq + 1;
            v[std::strcspn(v, "\r\n")] = 0;
            setenv(line,
v,
1);
            n3dsLog("[3ds] env %s=%s\n",
line,
v);
            
        }
        std::fclose(f);
        
    }
    if (const char * v = std::getenv("N3DS_BIGALLOC_KB")) g_bigAllocLimit = size_t(std::atoi(v)) << 10;
    if (const char * v = std::getenv("N3DS_ALLOC_SITES")) if ( * v == '1') n3dsAllocSitesEnable();
    if (const char * v = std::getenv("N3DS_DC_DIAG")) if ( * v == '1') 
    {
        t2p::g_drawCacheMissLog = [](const char * m) 
        {
            n3dsLog("%s\n",
m);
            
        } ;
        t2p::g_compileClock = []() ->uint64_t 
        {
            return svcGetSystemTick();
            
        } ;
        g_texConvLog = [](uint32_t a,
int f,
int w,
int h,
size_t n,
bool pack,
int stale,
uint64_t t) 
        {
            n3dsLog("[tex-conv] addr %08lx fmt %d %dx%d %u B pack %d why %d %.2f ms\n",
(unsigned long)a,
f,
w,
h,
unsigned(n),
int(pack),
int(stale),
double(t) * 1000.0 / SYSCLOCK_ARM11);
            
        } ;
        
    }
    if (const char * v = std::getenv("N3DS_BIGALLOC_LOG")) g_bigLog = std::atoi(v) != 0;
    romfsInit();
    n3dsSetBootStage("disc");
    n3dsPumpApt();
    static char discPath[512];
    auto headerKind = [](const char * path) ->int 
    {
        return n3dsDiscPathKind(path);
        
    } ;
    mkdir(N3DS_ROOT "/game",
0777);
    const char * disc = nullptr;
    auto takeDisc = [&](const char * path,
int kind) 
    {
        if (disc || (kind != 1 
&& kind != 2)) return;
        std::snprintf(discPath,
sizeof discPath,
"%s",
path);
        disc = discPath;
        
    } ;
    auto scanDisc = [&](const char * dirPath) 
    {
        if (disc) return;
        n3dsPumpApt();
        char exact[512];
        std::snprintf(exact,
sizeof exact,
"%s/GZLE01.iso",
dirPath);
        takeDisc(exact,
headerKind(exact));
        if (disc) return;
        std::snprintf(exact,
sizeof exact,
"%s/GZLE01.ciso",
dirPath);
        takeDisc(exact,
headerKind(exact));
        if (disc) return;
        if (DIR * dir = opendir(dirPath)) 
        {
            while (const dirent * ent = readdir(dir)) 
            {
                if (ent->d_name[0] == '.') continue;
                std::snprintf(discPath,
sizeof discPath,
"%s/%s",
dirPath,
ent->d_name);
                const int kind = headerKind(discPath);
                if (kind == 1 || kind == 2) 
                {
                    disc = discPath;
                    break;
                    
                }
                
            }
            closedir(dir);
            
        }
        
    } ;
    scanDisc(N3DS_ROOT "/game");
    scanDisc(N3DS_ROOT);
    if (!disc 
&& headerKind("romfs:/GZLE01.iso") == 1) 
    {
        std::snprintf(discPath,
sizeof discPath,
"%s",
"romfs:/GZLE01.iso");
        disc = discPath;
        
    }
    if (disc) setenv("COS_DISC",
disc,
0);
    setenv("COS_PERF_LOG",
"0",
0);
    setenv("COS_FPS60",
"0",
0);
    n3dsSetBootStage("saves");
    n3dsPumpApt();
    mkdir(N3DS_ROOT "/saves",
0777);
    mkdir(N3DS_ROOT "/saves/USA",
0777);
    mkdir(N3DS_ROOT "/saves/USA/Card A",
0777);
    mkdir(N3DS_ROOT "/saves/imported",
0777);
    auto copyFile = [](const char * from,
const char * to) 
    {
        FILE * in = std::fopen(from,
"rb");
        if (!in) return false;
        FILE * out = std::fopen(to,
"wb");
        if (!out) 
        {
            std::fclose(in);
            return false;
            
        }
        char buf[64 * 1024];
        size_t n;
        bool ok = true;
        while ((n = std::fread(buf,
1,
sizeof buf,
in)) > 0) if (std::fwrite(buf,
1,
n,
out) != n) ok = false;
        std::fclose(out);
        std::fclose(in);
        return ok;
        
    } ;
    auto newer = [](const char * a,
const char * b) 
    {
        struct stat sa, sb;
        if (stat(a,
&sa) != 0) return false;
        if (stat(b,
&sb) != 0) return true;
        return sa.st_mtime >= sb.st_mtime;
        
    } ;
    auto ends = [](const char * name,
const char * ext) 
    {
        const size_t n = std::strlen(name), e = std::strlen(ext);
        if (n < e) return false;
        for (size_t i = 0; i < e; ++i) 
        {
            char a = name[n - e + i], b = ext[i];
            if (a >= 'A' 
&& a <= 'Z') a = char(a + 32);
            if (b >= 'A' 
&& b <= 'Z') b = char(b + 32);
            if (a != b) return false;
            
        }
        return true;
        
    } ;
    auto adoptGci = [&](const char * from) 
    {
        FILE * f = std::fopen(from,
"rb");
        unsigned char h[0x28]
        {
            
        } ;
        if (!f || std::fread(h,
1,
sizeof h,
f) != sizeof h) 
        {
            if (f) std::fclose(f);
            return;
            
        }
        std::fclose(f);
        char file[33]
        {
            
        } ;
        std::memcpy(file,
h + 8,
32);
        for (char* p = file; *p; ++p) if ((unsigned char) * p < 32 || std::strchr("<>:\"/\\|?*",
* p)) * p = '_';
        if (!file[0]) return;
        char name[96];
        std::snprintf(name,
sizeof name,
"%.2s-%.4s-%s.gci",
h + 4,
h,
file);
        char dest[512];
        std::snprintf(dest,
sizeof dest,
N3DS_ROOT "/saves/USA/Card A/%s",
name);
        if (newer(from,
dest)) copyFile(from,
dest);
        
    } ;
    const char * previous[] = 
    {
        N3DS_ROOT "/user/USA/Card A", N3DS_ROOT "/saves/user/USA/Card A"
    } ;
    for (const char* dirPath : previous) 
    {
        if (DIR * dir = opendir(dirPath)) 
        {
            while (const dirent * ent = readdir(dir)) 
            {
                if (!ends(ent->d_name,
".gci")) continue;
                char from[512];
                std::snprintf(from,
sizeof from,
"%s/%s",
dirPath,
ent->d_name);
                char dest[512];
                std::snprintf(dest,
sizeof dest,
N3DS_ROOT "/saves/USA/Card A/%s",
ent->d_name);
                struct stat st;
                if (stat(dest,
&st) != 0) copyFile(from,
dest);
                
            }
            closedir(dir);
            
        }
        
    }
    if (DIR * dir = opendir(N3DS_ROOT "/saves")) 
    {
        char names[32][256];
        int count = 0;
        while (const dirent * ent = readdir(dir)) 
        {
            if (ent->d_name[0] == '.' || count == 32) continue;
            if (ends(ent->d_name,
".gci") || ends(ent->d_name,
".raw")) std::snprintf(names[count++],
256,
"%s",
ent->d_name);
            
        }
        closedir(dir);
        for (int i = 0; i < count; ++i) 
        {
            char from[512];
            std::snprintf(from,
sizeof from,
N3DS_ROOT "/saves/%s",
names[i]);
            if (ends(names[i],
".gci")) 
            {
                adoptGci(from);
                char imported[512];
                std::snprintf(imported,
sizeof imported,
N3DS_ROOT "/saves/imported/%s",
names[i]);
                std::rename(from,
imported);
                
            } else 
            {
                struct Found 
                {
                    char name[16][33];
                    int n;
                    
                }
                found
                {
                    
                } ;
                if (!aurora_card_raw_list(from,
"GZLE",
"01",
[](const char * fileName,
void * user) 
                {
                    auto * f = static_cast < Found * > (user);
                    if (f->n < 16) std::snprintf(f->name[f->n++],
33,
"%s",
fileName);
                    
                }
                ,
&found)) continue;
                std::vector < uint8_t > gci(256 * 1024);
                int wrote = 0;
                for (int g = 0; g < found.n; ++g) 
                {
                    size_t need = aurora_card_raw_extract(from,
"GZLE",
"01",
found.name[g],
nullptr,
0);
                    if (!need) continue;
                    if (need > gci.size()) gci.resize(need);
                    need = aurora_card_raw_extract(from,
"GZLE",
"01",
found.name[g],
gci.data(),
gci.size());
                    if (!need || need > gci.size()) continue;
                    char dropped[512];
                    std::snprintf(dropped,
sizeof dropped,
N3DS_ROOT "/saves/imported/%s.gci",
found.name[g]);
                    if (FILE * out = std::fopen(dropped,
"wb")) 
                    {
                        std::fwrite(gci.data(),
1,
need,
out);
                        std::fclose(out);
                        adoptGci(dropped);
                        std::remove(dropped);
                        ++wrote;
                        
                    }
                    
                }
                if (wrote) 
                {
                    char imported[512];
                    std::snprintf(imported,
sizeof imported,
N3DS_ROOT "/saves/imported/%s",
names[i]);
                    std::rename(from,
imported);
                    
                }
                
            }
            
        }
        
    }
    if (!disc) 
    {
        char msg[512];
        std::snprintf(msg,
sizeof msg,
"\nPut your Wind Waker (USA) ISO or CISO in\n  %s/game\n"
"as GZLE01.iso or GZLE01.ciso\n\nPress START to exit.\n",
N3DS_ROOT);
        n3dsShowBootError(msg);
        n3dsWaitStartOrQuit();
        n3dsCleanExit();
        
    }
    static char arg0[] = N3DS_ROOT "/ww3ds.3dsx";
    char * argv[] = 
    {
        arg0, nullptr
    } ;
    s_gameStarted = true;
    n3dsSetBootStage("game");
    n3dsPumpApt();
    return aurora_main(1,
argv);
    
}
#include <cstdlib>
extern "C" 
{
    void * __real_malloc(size_t);
    void * __real_calloc(size_t,
size_t);
    void * __real_realloc(void * ,
size_t);
    void * __real_aligned_alloc(size_t,
size_t);
    void __real_free(void * );
    size_t g_bigAllocLimit = 0;
    
}
namespace 
{
    bool g_bigLog = false;
    struct Live 
    {
        void * p;
        size_t n;
        void * caller;
        
    } ;
    constexpr unsigned kSlots = 1u << 15;
    Live g_live[kSlots];
    LightLock g_liveLock;
    bool g_liveLockInit = false;
    unsigned slotOf(void * p) 
    {
        return (uintptr_t(p) >> 3) * 2654435761u >> 17;
        
    }
    struct Guard 
    {
        Guard() 
        {
            if (!g_liveLockInit) 
            {
                LightLock_Init(&g_liveLock);
                g_liveLockInit = true;
                
            }
            LightLock_Lock(&g_liveLock);
            
        }
        ~Guard() 
        {
            LightLock_Unlock(&g_liveLock);
            
        }
        
    } ;
    void liveAdd(void * p,
size_t n,
void * caller) 
    {
        Guard g;
        for (unsigned i = slotOf(p),
k = 0; k < kSlots; k++,
i = (i + 1) & (kSlots - 1))
        if (g_live[i].p == nullptr || g_live[i].p == (void * )1) 
        {
            g_live[i] = Live
            {
                p, n, caller
            } ;
            return;
            
        }
        
    }
    void liveDel(void * p) 
    {
        Guard g;
        for (unsigned i = slotOf(p),
k = 0; k < kSlots; k++,
i = (i + 1) & (kSlots - 1)) 
        {
            if (g_live[i].p == nullptr) return;
            if (g_live[i].p == p) 
            {
                g_live[i].p = (void * )1;
                return;
                
            }
            
        }
        
    }
    void * noteBig(void * p,
size_t n,
void * caller,
const char * what) 
    {
        if (p 
&& g_bigAllocLimit 
&& n >= g_bigAllocLimit) 
        {
            liveAdd(p,
n,
caller);
            if (g_bigLog) n3dsLog("[mem] %s %u KB -> %p from %p\n",
what,
unsigned(n >> 10),
p,
caller);
            
        }
        return p;
        
    }
    
}
extern "C" 
{
    void n3dsNoteCaller(void * p,
void * caller) 
    {
        if (!p || !g_bigAllocLimit) return;
        if (g_bigLog) n3dsLog("[mem] new-caller %p\n",
caller);
        Guard g;
        for (unsigned i = slotOf(p),
k = 0; k < kSlots; k++,
i = (i + 1) & (kSlots - 1)) 
        {
            if (g_live[i].p == nullptr) return;
            if (g_live[i].p == p) 
            {
                g_live[i].caller = caller;
                return;
                
            }
            
        }
        
    }
    void n3dsBigAllocReport(void) 
    {
        if (!g_bigAllocLimit) return;
        struct Site 
        {
            void * caller;
            size_t bytes;
            unsigned count;
            
        } ;
        static Site sites[512];
        unsigned ns = 0;
        size_t total = 0;
        
        {
            Guard g;
            for (const Live& l : g_live) 
            {
                if (l.p == nullptr || l.p == (void * )1) continue;
                total += l.n;
                unsigned j = 0;
                while (j < ns 
&& sites[j].caller != l.caller) j++;
                if (j == ns) 
                {
                    if (ns == 512) continue;
                    sites[ns++] = Site
                    {
                        l.caller, 0, 0
                    } ;
                    
                }
                sites[j].bytes += l.n;
                sites[j].count++;
                
            }
            
        }
        std::sort(sites,
sites + ns,
[](const Site& a,
const Site& b) 
        {
            return a.bytes > b.bytes; 
        }
        );
        n3dsLog("[mem] live tracked allocations: %u KB at %u call sites; largest:\n",
unsigned(total >> 10),
ns);
        for (unsigned j = 0; j < ns 
&& j < 16; j++) n3dsLog("[mem]   %p %u KB in %u\n",
sites[j].caller,
unsigned(sites[j].bytes >> 10),
sites[j].count);
        
    }
    
}
#include <sys/lock.h>
#include <atomic>
struct N3dsLockStat 
{
    std::atomic < unsigned > calls
    {
        0
    }
    , contended
    {
        0
    } ;
    std::atomic < unsigned long long > waitTicks
    {
        0
    } ;
    int core = - 1;
    
} ;
N3dsLockStat g_lockStats[12];
static std::atomic < int > g_lockSlots
{
    0
} ;
static thread_local int t_lockSlot = - 1;
extern "C" 
{
    void __real___libc_lock_acquire_recursive(_LOCK_RECURSIVE_T * l);
    void __wrap___libc_lock_acquire_recursive(_LOCK_RECURSIVE_T * l) 
    {
        if (t_lockSlot < 0) 
        {
            const int s = g_lockSlots.fetch_add(1);
            t_lockSlot = s < 12 ? s : 11;
            if (s < 12) g_lockStats[s].core = svcGetProcessorID();
            
        }
        N3dsLockStat& st = g_lockStats[t_lockSlot];
        st.calls.fetch_add(1,
std::memory_order_relaxed);
        if (__libc_lock_try_acquire_recursive(l) == 0) return;
        const u64 t0 = svcGetSystemTick();
        __real___libc_lock_acquire_recursive(l);
        st.contended.fetch_add(1,
std::memory_order_relaxed);
        st.waitTicks.fetch_add(svcGetSystemTick() - t0,
std::memory_order_relaxed);
        
    }
    void n3dsLog(const char * fmt,
...);
    void n3dsLockStatsLog(unsigned long frame) 
    {
        char line[480];
        int at = std::snprintf(line,
sizeof line,
"[locks] frame %lu (thread core calls contended wait-ms):",
frame);
        const int n = std::min(g_lockSlots.load(),
12);
        for (int i = 0; i < n 
&& at < int(sizeof line) - 48; i++) 
        {
            N3dsLockStat& st = g_lockStats[i];
            const unsigned c = st.calls.exchange(0), k = st.contended.exchange(0);
            const unsigned long long w = st.waitTicks.exchange(0);
            if (!c) continue;
            at += std::snprintf(line + at,
sizeof line - at,
" t%d c%d %u %u %.2f",
i,
st.core,
c,
k,
double(w) * 1000.0 / SYSCLOCK_ARM11);
            
        }
        n3dsLog("%s\n",
line);
        
    }
    
}
namespace asite 
{
    struct Site 
    {
        void * caller;
        unsigned count;
        unsigned long long bytes;
        
    } ;
    constexpr int kSites = 48;
    static Site g_sites[4][kSites];
    static bool g_on = false;
    static inline void note(void * caller,
size_t n) 
    {
        if (!g_on) return;
        const int s = t_lockSlot;
        if (s < 0 || s >= 4) return;
        Site * t = g_sites[s];
        for (int i = 0; i < kSites; i++) 
        {
            if (t[i].caller == caller) 
            {
                t[i].count++;
                t[i].bytes += n;
                return;
                
            }
            if (!t[i].caller) 
            {
                t[i].caller = caller;
                t[i].count = 1;
                t[i].bytes = n;
                return;
                
            }
            
        }
        
    }
    
}
extern "C" void n3dsAllocSitesLog(unsigned long frame) 
{
    if (!asite::g_on) return;
    for (int s = 0; s < 4; s++) 
    {
        asite::Site * t = asite::g_sites[s];
        std::sort(t,
t + asite::kSites,
[](const asite::Site& a,
const asite::Site& b) 
        {
            return a.count > b.count; 
        }
        );
        for (int i = 0; i < 12 
&& t[i].caller; i++)
        n3dsLog("[alloc-sites] frame %lu slot %d core %d %p calls %u bytes %llu\n",
frame,
s,
g_lockStats[s].core,
t[i].caller,
t[i].count,
t[i].bytes);
        std::memset(t,
0,
sizeof asite::g_sites[s]);
        
    }
    
}
extern "C" void n3dsAllocSitesEnable() 
{
    asite::g_on = true;
    
}
extern "C" char * fake_heap_end;
namespace fa 
{
    constexpr size_t kSpan = 64u << 10;
    constexpr int kClasses = 12;
    static unsigned kFaMaxSpans = 64;
    static const uint16_t kSize[kClasses] = 
    {
        16, 32, 48, 64, 96, 128, 192, 256, 384, 512, 768, 1024
    } ;
    static uint8_t kClassOf[65];
    struct Block 
    {
        Block * next;
        
    } ;
    struct Central 
    {
        LightLock lock;
        Block * head = nullptr;
        unsigned count = 0;
        uint8_t * bump = nullptr;
        uint8_t * end = nullptr;
        
    } ;
    static Central g_central[kClasses];
    static uint8_t * g_base = nullptr;
    static uint8_t * g_end = nullptr;
    static uint8_t g_spanClass[2048];
    static std::atomic < unsigned > g_spans
    {
        0
    } ;
    static std::atomic < bool > g_on
    {
        false
    } ;
    std::atomic < unsigned long > g_fallbacks
    {
        0
    } ;
    static std::atomic < unsigned long > g_borrowed
    {
        0
    }
    , g_classFallbacks[kClasses];
    std::atomic < bool > g_borrowOn
    {
        true
    } ;
    struct Cache 
    {
        Block * head[kClasses] = 
        {
            
        } ;
        uint16_t n[kClasses] = 
        {
            
        } ;
        
    } ;
    static thread_local Cache t_cache;
    static inline int spanClassOf(const void * p) 
    {
        const uint8_t * q = static_cast < const uint8_t * > (p);
        if (q < g_base || q >= g_end) return - 1;
        return int(g_spanClass[size_t(q - g_base) >> 16]) - 1;
        
    }
    static bool refill(int c) 
    {
        Central& C = g_central[c];
        Cache& t = t_cache;
        unsigned moved = 0;
        while (C.head 
&& moved < 32) 
        {
            Block * b = C.head;
            C.head = b->next;
            C.count--;
            b->next = t.head[c];
            t.head[c] = b;
            moved++;
            
        }
        while (moved < 32) 
        {
            if (C.bump + kSize[c] > C.end) 
            {
                if (g_spans.load() >= kFaMaxSpans) 
                {
                    if (g_spans.load() >= 160) break;
                    char * brk = static_cast < char * > (sbrk(0));
                    if (brk == reinterpret_cast < char * > ( - 1) || fake_heap_end - brk < (3 << 20)) break;
                    
                }
                uint8_t * s = static_cast < uint8_t * > (memalign(kSpan,
kSpan));
                if (!s) break;
                if (s < g_base || s + kSpan > g_end) 
                {
                    free(s);
                    break;
                    
                }
                g_spanClass[size_t(s - g_base) >> 16] = uint8_t(c + 1);
                g_spans.fetch_add(1);
                C.bump = s;
                C.end = s + kSpan;
                
            }
            Block * b = reinterpret_cast < Block * > (C.bump);
            C.bump += kSize[c];
            b->next = t.head[c];
            t.head[c] = b;
            moved++;
            
        }
        t.n[c] = uint16_t(t.n[c] + moved);
        return moved > 0;
        
    }
    static void pregrow(int c,
int spans) 
    {
        Central& C = g_central[c];
        for (int n = 0; n < spans 
&& g_spans.load() < kFaMaxSpans; n++) 
        {
            uint8_t * s = static_cast < uint8_t * > (memalign(kSpan,
kSpan));
            if (!s) return;
            if (s < g_base || s + kSpan > g_end) 
            {
                free(s);
                return;
                
            }
            g_spanClass[size_t(s - g_base) >> 16] = uint8_t(c + 1);
            g_spans.fetch_add(1);
            for (uint8_t* p = s; p + kSize[c] <= s + kSpan; p += kSize[c]) 
            {
                Block * b = reinterpret_cast < Block * > (p);
                b->next = C.head;
                C.head = b;
                C.count++;
                
            }
            
        }
        
    }
    static inline void * alloc(size_t n) 
    {
        if (!g_on.load(std::memory_order_relaxed) || n > 1024) return nullptr;
        const int c = kClassOf[(n + 15) >> 4];
        Cache& t = t_cache;
        if (!t.head[c]) 
        {
            Central& C = g_central[c];
            LightLock_Lock(&C.lock);
            const bool ok = refill(c);
            LightLock_Unlock(&C.lock);
            if (!ok) 
            {
                if (g_borrowOn.load(std::memory_order_relaxed)) for (int c2 = c + 1; c2 < kClasses 
&& kSize[c2] <= 4 * kSize[c]; c2++) 
                {
                    if (!t.head[c2]) 
                    {
                        Central& C2 = g_central[c2];
                        if (!C2.head 
&& C2.bump + kSize[c2] > C2.end) continue;
                        LightLock_Lock(&C2.lock);
                        const bool ok2 = refill(c2);
                        LightLock_Unlock(&C2.lock);
                        if (!ok2) continue;
                        
                    }
                    Block * b = t.head[c2];
                    t.head[c2] = b->next;
                    t.n[c2]--;
                    g_borrowed.fetch_add(1,
std::memory_order_relaxed);
                    return b;
                    
                }
                g_fallbacks.fetch_add(1,
std::memory_order_relaxed);
                g_classFallbacks[c].fetch_add(1,
std::memory_order_relaxed);
                return nullptr;
                
            }
            
        }
        Block * b = t.head[c];
        t.head[c] = b->next;
        t.n[c]--;
        return b;
        
    }
    static inline void release(void * p,
int c) 
    {
        Cache& t = t_cache;
        Block * b = static_cast < Block * > (p);
        b->next = t.head[c];
        t.head[c] = b;
        if (++t.n[c] > 128) 
        {
            Block * first = t.head[c];
            Block * last = first;
            for (int i = 1; i < 64; i++) last = last->next;
            t.head[c] = last->next;
            t.n[c] = uint16_t(t.n[c] - 64);
            Central& C = g_central[c];
            LightLock_Lock(&C.lock);
            last->next = C.head;
            C.head = first;
            C.count += 64;
            LightLock_Unlock(&C.lock);
            
        }
        
    }
    
}
extern "C" char * fake_heap_start;
extern "C" char * fake_heap_end;
void n3dsFastAllocEnable(bool on,
int maxSpans) 
{
    if (!on || fa::g_on.load()) return;
    if (maxSpans >= 32 
&& maxSpans <= 160) fa::kFaMaxSpans = unsigned(maxSpans);
    for (int i = 0,
c = 0; i <= 64; i++) 
    {
        while (c < fa::kClasses - 1 
&& size_t(i) * 16 > fa::kSize[c]) c++;
        fa::kClassOf[i] = uint8_t(c);
        
    }
    for (auto& C : fa::g_central) LightLock_Init(&C.lock);
    fa::g_base = reinterpret_cast < uint8_t * > (uintptr_t(fake_heap_start) & ~uintptr_t(fa::kSpan - 1));
    fa::g_end = fa::g_base + sizeof fa::g_spanClass * fa::kSpan;
    if (reinterpret_cast < uint8_t * > (fake_heap_end) > fa::g_end) 
    {
        n3dsLog("[fast-alloc] heap larger than the span table: off\n");
        return;
        
    }
    fa::g_on.store(true);
    for (int c = 0; c < fa::kClasses; c++) 
    {
        LightLock_Lock(&fa::g_central[c].lock);
        fa::pregrow(c,
fa::kSize[c] <= 512 ? 4 : 2);
        LightLock_Unlock(&fa::g_central[c].lock);
        
    }
    n3dsLog("[fast-alloc] on: classes <= 1024 B, at most %u spans of 64 KB, %u ready\n",
fa::kFaMaxSpans,
fa::g_spans.load());
    
}
void n3dsFastAllocBorrow(bool on) 
{
    fa::g_borrowOn.store(on,
std::memory_order_relaxed);
    
}
void n3dsFastAllocLog() 
{
    unsigned perClass[fa::kClasses] = 
    {
        
    } ;
    for (uint8_t v : fa::g_spanClass) if (v) perClass[v - 1]++;
    char cls[256];
    int at = 0;
    for (int c = 0; c < fa::kClasses; c++) at += std::snprintf(cls + at,
sizeof cls - at,
" %u:%u/%lu",
unsigned(fa::kSize[c]),
perClass[c],
fa::g_classFallbacks[c].load());
    n3dsLog("[fast-alloc] spans %u (%u KB), fallbacks %lu, borrowed %lu; size:spans/fallbacks%s\n",
fa::g_spans.load(),
unsigned(fa::g_spans.load() * 64),
fa::g_fallbacks.load(),
fa::g_borrowed.load(),
cls);
    
}
extern "C" 
{
    void n3dsPoolsTrim(void);
    static void * retryAfterTrim(void * p,
bool wanted,
void * ( * again)(void * ),
void * arg) 
    {
        if (p || !wanted) return p;
        n3dsPoolsTrim();
        return again(arg);
        
    }
    void * __wrap_malloc(size_t n) 
    {
        if (void * p = fa::alloc(n)) return p;
        asite::note(__builtin_return_address(0),
n);
        void * p = retryAfterTrim(__real_malloc(n),
n != 0,
[](void * a) 
        {
            return __real_malloc( * static_cast < size_t * > (a)); 
        }
        ,
&n);
        return noteBig(p,
n,
__builtin_return_address(0),
"malloc");
        
    }
    void * __wrap_calloc(size_t a,
size_t b) 
    {
        if (b 
&& a <= 1024 / b) if (void * p = fa::alloc(a * b)) 
        {
            std::memset(p,
0,
a * b);
            return p;
            
        }
        size_t ab[2] = 
        {
            a, b
        } ;
        void * p = retryAfterTrim(__real_calloc(a,
b),
a 
&& b,
[](void * x) 
        {
            const size_t * v = static_cast < size_t * > (x); return __real_calloc(v[0],
v[1]); 
        }
        ,
ab);
        return noteBig(p,
a * b,
__builtin_return_address(0),
"calloc");
        
    }
    void * __wrap_realloc(void * p,
size_t n) 
    {
        if (p) if (const int c = fa::spanClassOf(p); c >= 0) 
        {
            if (n 
&& n <= fa::kSize[c]) return p;
            void * q = n ? __wrap_malloc(n) : nullptr;
            if (n 
&& !q) return nullptr;
            if (q) std::memcpy(q,
p,
std::min < size_t > (n,
fa::kSize[c]));
            fa::release(p,
c);
            return q;
            
        }
        if (p 
&& g_bigAllocLimit) liveDel(p);
        asite::note(__builtin_return_address(0),
n);
        return noteBig(__real_realloc(p,
n),
n,
__builtin_return_address(0),
"realloc");
        
    }
    void * __wrap_aligned_alloc(size_t a,
size_t n) 
    {
        if (a <= 16) if (void * p = fa::alloc(n)) return p;
        size_t an[2] = 
        {
            a, n
        } ;
        void * p = retryAfterTrim(__real_aligned_alloc(a,
n),
n != 0,
[](void * x) 
        {
            const size_t * v = static_cast < size_t * > (x); return __real_aligned_alloc(v[0],
v[1]); 
        }
        ,
an);
        return noteBig(p,
n,
__builtin_return_address(0),
"aligned_alloc");
        
    }
    void __wrap_free(void * p) 
    {
        if (p) if (const int c = fa::spanClassOf(p); c >= 0) 
        {
            fa::release(p,
c);
            return;
            
        }
        if (p 
&& g_bigAllocLimit) liveDel(p);
        if (p) asite::note(__builtin_return_address(0),
0);
        __real_free(p);
        
    }
    
}
