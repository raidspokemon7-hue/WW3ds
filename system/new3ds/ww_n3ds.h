#pragma once
#include <stdint.h>
#define N3DS_ROOT "sdmc:/ww3ds"
#ifdef __cplusplus
extern "C" 
{
    #endif
typedef struct 
    {
        uint32_t keys;
        int16_t stickX, stickY;
        int16_t cstickX, cstickY;
        
    }
    N3dsPad;
    enum 
    {
        N3DS_KEY_A = 1u << 0, N3DS_KEY_B = 1u << 1, N3DS_KEY_SELECT = 1u << 2, N3DS_KEY_START = 1u << 3, 
        N3DS_KEY_DRIGHT = 1u << 4, N3DS_KEY_DLEFT = 1u << 5, N3DS_KEY_DUP = 1u << 6, N3DS_KEY_DDOWN = 1u << 7, 
        N3DS_KEY_R = 1u << 8, N3DS_KEY_L = 1u << 9, N3DS_KEY_X = 1u << 10, N3DS_KEY_Y = 1u << 11, 
        N3DS_KEY_ZL = 1u << 14, N3DS_KEY_ZR = 1u << 15, N3DS_KEY_TOUCH = 1u << 20, 
        
    } ;
    int n3dsAudioInit(void);
    int n3dsAudioOpen(int hz);
    int n3dsAudioPut(const void * data,
int bytes);
    int n3dsAudioQueued(void);
    int n3dsAudioResume(void);
    int n3dsAudioSetRate(int hz);
    int n3dsPrepareExit(void);
    void n3dsCloseLog(void);
    void n3dsAudioShutdown(void);
    int n3dsMemcardQuiesce(unsigned timeoutMs);
    void n3dsSetLogPath(const char * path);
    void n3dsSetBootStage(const char * stage);
    const char * n3dsBootStage(void);
    void n3dsPumpApt(void);
    void n3dsShowBootError(const char * message);
    int n3dsWaitStartOrQuit(void);
    [[noreturn]] void n3dsCleanExit(void);
    int n3dsInit(void);
    void n3dsLog(const char * fmt,
...);
    int n3dsRenderThreadStart(void);
    int n3dsRenderThreadActive(void);
    void n3dsRenderThreadJob(void ( * job)(void * ),
void * context);
    uint32_t n3dsRenderStagingBudget(void);
    float n3dsCameraCullAspect(float aspect,
float viewportWidth);
    int n3dsRenderThreadInit(void);
    uint32_t n3dsRenderThreadProcess(const uint8_t * ,
uint32_t,
int * );
    void n3dsRenderThreadFrame(void);
    void n3dsRenderThreadRelease(void);
    void n3dsGpuSync(void);
    int n3dsRenderThreadBusy(void);
    void n3dsGpuShutdown(void);
    int n3dsGpuInitLocal(void);
    uint32_t n3dsGpuProcessLocal(const uint8_t * ,
uint32_t,
int * );
    void n3dsGpuEndFrameLocal(void);
    void n3dsGpuShutdownLocal(void);
    int n3dsGpuInit(void);
    uint32_t n3dsGpuProcess(const uint8_t * data,
uint32_t size,
int * drawDone);
    void n3dsGpuEndFrame(void);
    int n3dsAptLoop(void);
    void n3dsHidRead(N3dsPad * out);
    uint64_t n3dsTicks(void);
    void n3dsYield(void);
    unsigned int n3dsCurrentThreadHandle(void);
    void n3dsSetThreadPriority(unsigned int handle,
int gcPriority);
    int n3dsPushCreatePriority(int gcPriority);
    void n3dsPopCreatePriority(int saved);
    double n3dsTicksPerSecond(void);
    void n3dsGameHeapReport(char * out,
unsigned size);
    void n3dsBigAllocReport(void);
    #ifdef __cplusplus
}
#endif
