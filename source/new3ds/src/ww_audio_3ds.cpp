extern "C" 
{
    unsigned g_n3dsAudioBlocks = 0;
    
}
#include <3ds.h>
#include <cstdlib>
#include <cstring>
#include "ww_n3ds.h"
namespace 
{
    constexpr int kBuffers = 24;
    constexpr int kFrames = 512;
    constexpr int kBytes = kFrames * 4;
    LightLock mutex;
    bool initialized = false, opened = false, settingsPaused = false;
    unsigned emptyChecks = 0, rejectedPushes = 0, acceptedFrames = 0;
    int rate = 0, partial = 0;
    u8 * samples = nullptr;
    u8 staging[kBytes];
    ndspWaveBuf waves[kBuffers];
    struct Guard 
    {
        Guard() 
        {
            LightLock_Lock(&mutex);
            
        }
        ~Guard() 
        {
            LightLock_Unlock(&mutex);
            
        }
        
    } ;
    bool busy(const ndspWaveBuf& w) 
    {
        return w.status == NDSP_WBUF_QUEUED || w.status == NDSP_WBUF_PLAYING;
        
    }
    void shutdown() 
    {
        Guard guard;
        if (!initialized) return;
        opened = false;
        ndspChnWaveBufClear(0);
        ndspExit();
        linearFree(samples);
        samples = nullptr;
        initialized = false;
        
    }
    
}
extern "C" void n3dsAudioShutdown(void)
{
    shutdown();
    
}
extern "C" int n3dsAudioInit(void) 
{
    Guard guard;
    if (initialized) return 1;
    Result rc = ndspInit();
    if (R_FAILED(rc)) 
    {
        n3dsLog("[audio] ndspInit failed: 0x%08lx; check sdmc:/3ds/dspfirm.cdc\n",
(unsigned long)rc);
        return 0;
        
    }
    samples = static_cast < u8 * > (linearAlloc(kBuffers * kBytes));
    if (!samples) 
    {
        ndspExit();
        n3dsLog("[audio] no linear memory for NDSP\n");
        return 0;
        
    }
    ndspSetOutputMode(NDSP_OUTPUT_STEREO);
    initialized = true;
    std::atexit(shutdown);
    return 1;
    
}
extern "C" int n3dsAudioOpen(int hz) 
{
    if (hz != 32000 
&& hz != 48000) return 0;
    if (!n3dsAudioInit()) return 0;
    Guard guard;
    if (opened) return 0;
    ndspChnReset(0);
    ndspChnSetPaused(0,
true);
    ndspChnSetFormat(0,
NDSP_FORMAT_STEREO_PCM16);
    ndspChnSetInterp(0,
NDSP_INTERP_POLYPHASE);
    ndspChnSetRate(0,
float(hz));
    float mix[12] = 
    {
        1.0f, 1.0f
    } ;
    ndspChnSetMix(0,
mix);
    std::memset(waves,
0,
sizeof waves);
    partial = 0;
    rate = hz;
    opened = true;
    n3dsLog("[audio-open] NDSP S16 stereo %d Hz, %d x %d frames (paused until resume)\n",
hz,
kBuffers,
kFrames);
    return 1;
    
}
extern "C" int n3dsAudioResume(void) 
{
    Guard guard;
    if (!opened) return 0;
    ndspChnSetPaused(0,
false);
    n3dsLog("[audio-open] NDSP channel resumed\n");
    return 1;
    
}
extern "C" int n3dsAudioSetRate(int hz) 
{
    Guard guard;
    if (!opened || (hz != 32000 
&& hz != 48000)) return 0;
    if (rate != hz) 
    {
        ndspChnWaveBufClear(0);
        std::memset(waves,
0,
sizeof waves);
        partial = 0;
        ndspChnSetRate(0,
float(hz));
        rate = hz;
        
    }
    return 1;
    
}
extern "C" int n3dsAudioQueued(void) 
{
    Guard guard;
    if (!opened) return - 1;
    int bytes = partial;
    for (const auto& w : waves) if (busy(w)) bytes += kBytes;
    if(bytes == partial 
&& !settingsPaused)++emptyChecks;
    return bytes;
    
}
extern "C" int n3dsAudioPut(const void * data,
int bytes) 
{
    Guard guard;
    if (!opened || bytes < 0 || bytes % 4 || (!data 
&& bytes)) return 0;
    if(settingsPaused)return 1;
    int available = 0;
    for (const auto& w : waves) if (!busy(w)) available += kBytes;
    if (bytes > available + kBytes - 4 - partial)
    {
        ++rejectedPushes;
        return 0;
        
    }
    acceptedFrames += unsigned(bytes / 4);
    auto * src = static_cast < const u8 * > (data);
    while (bytes) 
    {
        int take = bytes < kBytes - partial ? bytes : kBytes - partial;
        std::memcpy(staging + partial,
src,
take);
        partial += take;
        src += take;
        bytes -= take;
        if (partial != kBytes) continue;
        for (int i = 0; i < kBuffers; ++i) 
        {
            auto& w = waves[i];
            if (busy(w)) continue;
            std::memcpy(samples + i * kBytes,
staging,
kBytes);
            DSP_FlushDataCache(samples + i * kBytes,
kBytes);
            std::memset(&w,
0,
sizeof w);
            w.data_vaddr = samples + i * kBytes;
            w.nsamples = kFrames;
            ndspChnWaveBufAdd(0,
&w);
            g_n3dsAudioBlocks++;
            partial = 0;
            break;
            
        }
        
    }
    return 1;
    
}
extern "C" void n3dsAudioSettingsPause(int pause)
{
    Guard guard;
    settingsPaused = pause != 0;
    if(!opened)return;
    ndspChnSetPaused(0,
true);
    ndspChnWaveBufClear(0);
    std::memset(waves,
0,
sizeof waves);
    partial = 0;
    if(!settingsPaused)ndspChnSetPaused(0,
false);
    
}
extern "C" void n3dsAudioStreamDrop(void)
{
    Guard guard;
    if(!opened)return;
    // Opening-stream force-drop can free JAI buffers while NDSP still queues
    // garbage DirectPCM frames; mute and flush before the heap goes away.
    ndspChnSetPaused(0,
true);
    ndspChnWaveBufClear(0);
    std::memset(waves,
0,
sizeof waves);
    partial = 0;
    // Always leave the channel running for the next stream/BGM unless settings
    // explicitly hold audio paused.
    if(!settingsPaused)
    {
        ndspChnSetPaused(0,
false);
        n3dsLog("[audio-open] stream drop flushed; NDSP channel resumed\n");
        
    }
    else n3dsLog("[audio-open] stream drop flushed; NDSP stays paused (settings)\n");
    
}
extern "C" void n3dsAudioHealth(void)
{
    unsigned empty, rejected, frames;
    int queued = 0;
    
    {
        Guard guard;
        if(!opened)return;
        empty = emptyChecks;
        rejected = rejectedPushes;
        frames = acceptedFrames;
        for (const auto& w:waves)if(busy(w))queued += kFrames;
        
    }
    n3dsLog("[audio-health] submitted %u frames queued %d frames empty-checks %u rejected-pushes %u\n",
frames,
queued,
empty,
rejected);
    
}
