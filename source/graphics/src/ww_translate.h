#pragma once
#include <string>
#include <vector>
#include "ww_model.h"
namespace t2p 
{
    enum class Fidelity 
    {
        Exact = 2, Approx = 1, Unsupported = 0 
    } ;
    const char * fidelityName(Fidelity);
    struct StageNote 
    {
        int gcStage = 0;
        Fidelity color = Fidelity::Exact, alpha = Fidelity::Exact;
        int picaStages = 0;
        double colorErr255 = 0, alphaErr255 = 0;
        std::string text;
        
    } ;
    struct VerifyResult 
    {
        double logicMaxErr255 = 0;
        double maxErr255 = 0, meanErr255 = 0;
        int samples = 0;
        
    } ;
    class FogCompileCache;
    struct Options 
    {
        FogCompileCache * fogCompileCache = nullptr;
        bool fixedUniforms = false;
        double approxLimit = 0.20;
        int verifySamples = 4000;
        uint64_t seed = 0x5eed1234abcdULL;
        float frameFogRange = 0.0f;
        bool flipTexT = true;
        bool multipass = true;
        int approxEffort = 10;
        bool approximate = true;
        
    } ;
    struct Translation 
    {
        Fidelity overall = Fidelity::Unsupported;
        PicaProgram prog;
        std::vector < StageNote > notes;
        VerifyResult verify;
        
    } ;
    Translation translate(const TevMaterial&,
const Options& = {}
    );
    VerifyResult verify(const TevMaterial&,
const PicaProgram&,
uint64_t seed,
int n);
    constexpr double kLogicTol255 = 0.05;
    std::string describeStage(const PStage&);
    std::string formatReport(const std::string& name,
const TevMaterial&,
const Translation&);
    struct Rng 
    {
        uint64_t s;
        explicit Rng(uint64_t seed) : s(seed ? seed : 1) 
        {
            
        }
        uint64_t next() 
        {
            s ^= s >> 12;
            s ^= s << 25;
            s ^= s >> 27;
            return s * 2685821657736338717ULL;
            
        }
        float f() 
        {
            return float(next() >> 40) / 16777216.0f;
            
        }
        float v8();
        Rgba rgba();
        Inputs inputs();
        
    } ;
    
}
