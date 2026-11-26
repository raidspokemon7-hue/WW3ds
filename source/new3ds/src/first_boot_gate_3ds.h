#pragma once
#include <atomic>
namespace n3ds_warm 
{
    inline std::atomic < bool > g_lockControls
    {
        false
    } ;
    // Covering the displays during first-boot warm (Preparing Wind Waker).
    inline std::atomic < bool > g_covering
    {
        false
    } ;
    inline bool blockingInput() 
    {
        return g_lockControls.load(std::memory_order_acquire);
        
    }
    inline bool coveringGate() 
    {
        return g_covering.load(std::memory_order_acquire);
        
    }
    
}
extern "C" int n3dsWarmCovering(void);
