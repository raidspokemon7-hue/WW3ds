#pragma once
#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include "ww_pipeline.h"
#include "ww_translate.h"
#include "ww_vtx.h"
namespace t2p 
{
    constexpr int kScratchTexmap = 8;
    struct GxDrawState 
    {
        TevMaterial tev;
        bool indirect = false;
        uint32_t indRaw[4] = 
        {
            
        } ;
        std::vector < std::string > importBlockers;
        VtxSetup vtx;
        int numPosMtx = 1;
        GxAlphaCompare alpha;
        GxBlend blend;
        GxZMode z;
        GxFog fog;
        float projFar = 100000.0f;
        uint8_t texmapScaled = 0;
        bool projOrtho = false;
        
    } ;
    struct DrawAttr 
    {
        enum Kind 
        {
            Pos, Nrm, Clr0, MtxIdx, Tex 
        }
        kind = Pos;
        int index = 0;
        int reg = 0;
        
    } ;
    enum class PassRole : uint8_t 
    {
        Single, ScratchWrite, ScratchRead 
    } ;
    struct PicaDraw 
    {
        Fidelity overall = Fidelity::Unsupported;
        std::vector < std::string > blockers;
        std::vector < std::string > notes;
        std::vector < std::string > info;
        Translation tev;
        VtxSetup vtx;
        VtxShader vs;
        std::vector < DrawAttr > attrs;
        int numUnits = 0;
        int unitTexmap[3] = 
        {
             - 1,  - 1,  - 1
        } ;
        int unitTexcoord[3] = 
        {
             - 1,  - 1,  - 1
        } ;
        PicaAlphaTest alpha;
        PicaBlend blend;
        PicaDepth depth;
        PicaDepthMap depthMap;
        PicaFog fog;
        bool unitNeedAlpha[3] = 
        {
            false, false, false
        } ;
        PassRole role = PassRole::Single;
        std::shared_ptr < const PicaDraw > pass2;
        int splitAt = 0;
        bool scratchDepth = false;
        bool scratchDepthFar = true;
        uint64_t vsKey = 0;
        int vsInstructions = 0, vsOpdescs = 0, vsUniformRegs = 0;
        struct ShaderMemo 
        {
            uint64_t cacheId = 0;
            void * shader = nullptr;
            ShaderMemo() = default;
            ShaderMemo(const ShaderMemo&) 
            {
                
            }
            ShaderMemo& operator = (const ShaderMemo&) 
            {
                cacheId = 0;
                shader = nullptr;
                return * this;
                
            }
            
        } ;
        mutable ShaderMemo shaderMemo;
        struct TexEnvMemo 
        {
            bool valid = false;
            float in[7][4] = 
            {
                
            } ;
            std::vector < HwStage > words;
            uint32_t updateBuffer = 0;
            TexEnvMemo() = default;
            TexEnvMemo(const TexEnvMemo&) 
            {
                
            }
            TexEnvMemo& operator = (const TexEnvMemo&) 
            {
                valid = false;
                return * this;
                
            }
            
        } ;
        mutable TexEnvMemo texenvMemo;
        
    } ;
    PicaDraw compileDraw(const GxDrawState&,
const Options& = {}
    );
    PicaDraw compileDrawSingle(const GxDrawState&,
const Options& = {}
    );
    PicaDraw compileDrawExact(const GxDrawState&,
const Options& = {}
    );
    float frameFogRange(const GxDrawState * draws,
size_t n);
    class FrameFogRange 
    {
        public:
        void add(const GxDrawState&);
        float value() const 
        {
            return range_ > 0 ? std::max(range_,
far_) : 0.0f;
            
        }
        private:
        struct Seen 
        {
            GxFog f;
            float projFar;
            
        } ;
        std::vector < Seen > seen_;
        float range_ = 0, far_ = 0;
        
    } ;
    
}
