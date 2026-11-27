#pragma once
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "ww_frame.h"
#include "ww_picaenc.h"
#include "ww_submit.h"
#include "ww_texcache.h"
namespace t2p 
{
    struct C3dShader 
    {
        uint64_t key = 0;
        std::string source;
        PicaAsmResult bin;
        struct Uni 
        {
            int id = 0;
            int size = 0;
            int packedSlot = - 1;
            const std::string * name = nullptr;
            
        } ;
        std::map < std::string, Uni > uniforms;
        std::vector < std::pair < std::string, Uni >> uniformsById;
        uint16_t inputMask = 0;
        std::string error;
        uintptr_t user = 0;
        uint64_t lastUseFrame = 0;
        
    } ;
    class C3dShaderCache 
    {
        public:
        C3dShader * get(const PicaDraw&);
        C3dShader * get(const std::string& source);
        C3dShader * get(uint64_t stateKey,
const std::string& source);
        void beginFrame(int assembleBudget);
        int assembleBudgetLeft() const 
        {
            return assembleLeft_;
            
        }
        size_t size() const 
        {
            return count_;
            
        }
        size_t deferredMisses() const 
        {
            return deferred_;
            
        }
        template < class F > void forEach(F f) 
        {
            for (auto& kv : byKey_) for (auto& s : kv.second) f( * s);
            
        }
        // Drop GPU program handles for cold entries; keep host SHBIN/source.
        size_t releaseColdGpuHandles(uint64_t olderThanFrame,
size_t maxRelease,
const std::function < void(C3dShader&) > & release);
        private:
        C3dShader * insertAssembled(uint64_t key,
std::string source,
PicaAsmResult bin);
        bool tryLoadShbin(uint64_t key,
PicaAsmResult& out) const;
        void trySaveShbin(uint64_t key,
const PicaAsmResult& bin) const;
        std::map < uint64_t, std::vector < std::unique_ptr < C3dShader >> > byKey_;
        size_t count_ = 0;
        size_t deferred_ = 0;
        int assembleLeft_ = 2;
        uint64_t id_ = nextId();
        static uint64_t nextId() 
        {
            static uint64_t n = 0;
            return ++n;
            
        }
        
    } ;
    namespace ctru 
    {
        enum : int 
        {
            BYTE = 0, UNSIGNED_BYTE = 1, SHORT = 2, FLOAT = 3 
        } ;
        enum : int 
        {
            CLAMP_TO_EDGE = 0, REPEAT = 2, MIRRORED_REPEAT = 3 
        } ;
        enum : int 
        {
            NEAREST = 0, LINEAR = 1 
        } ;
        enum : int 
        {
            CULL_NONE = 0, CULL_FRONT_CCW = 1, CULL_BACK_CCW = 2 
        } ;
        enum : int 
        {
            WRITE_RED = 1, WRITE_GREEN = 2, WRITE_BLUE = 4, WRITE_ALPHA = 8, WRITE_DEPTH = 0x10 
        } ;
        enum : int 
        {
            NO_FOG = 0, FOG = 5 
        } ;
        
    }
    struct C3dLoader 
    {
        int reg = 0, format = ctru::FLOAT, count = 0;
        
    } ;
    struct C3dAttrs 
    {
        C3dLoader loader[12];
        int count = 0;
        uint64_t permutation = 0;
        uint32_t stride = 0;
        bool planar = false;
        uint32_t offset[12] = 
        {
            
        }
        , size[12] = 
        {
            
        } ;
        
    } ;
    const std::string * internUniformName(const std::string& name);
    struct C3dUniform 
    {
        const std::string * name = nullptr;
        int id = 0;
        const PVec * rows = nullptr;
        int count = 0;
        
    } ;
    struct C3dTexEnv 
    {
        HwStage stage[6];
        int used = 0;
        uint32_t updateBuffer = 0;
        uint32_t bufferColor = 0;
        
    } ;
    struct C3dUnit 
    {
        bool enabled = false;
        TexEntry * entry = nullptr;
        bool projective = false;
        uint8_t wrapS = ctru::CLAMP_TO_EDGE, wrapT = ctru::CLAMP_TO_EDGE;
        uint8_t magFilter = ctru::NEAREST, minFilter = ctru::NEAREST, mipFilter = ctru::NEAREST;
        uint8_t minLevel = 0, maxLevel = 0;
        uint16_t lodBias = 0;
        
    } ;
    struct C3dEffect 
    {
        bool zBuffer = true;
        float zScale = 1, zOffset = 1;
        uint8_t cull = ctru::CULL_NONE;
        bool depthTest = true;
        uint8_t depthFunc = PF_LEQUAL;
        uint8_t writeMask = 0x1F;
        bool alphaTest = false;
        uint8_t alphaFunc = PF_ALWAYS;
        uint8_t alphaRef = 0;
        bool logicOp = false;
        uint8_t logic = 3;
        uint8_t eqRgb = 0, eqAlpha = 0, srcRgb = 1, dstRgb = 0, srcAlpha = 1, dstAlpha = 0;
        uint32_t blendColor = 0;
        bool fog = false;
        uint32_t fogColor = 0;
        
    } ;
    struct C3dPlan 
    {
        bool ok = false;
        std::string why;
        C3dShader * shader = nullptr;
        C3dAttrs attrs;
        std::vector < C3dUniform > uniforms;
        C3dTexEnv texenv;
        C3dUnit units[3];
        C3dEffect fx;
        uint32_t fogLut[128] = 
        {
            
        } ;
        PixelRect scissor;
        const uint8_t * vertices = nullptr;
        uint32_t vertexBytes = 0;
        const uint16_t * indices = nullptr;
        uint32_t indexCount = 0;
        uint64_t hashAttrs = 0, hashTexEnv = 0, hashEffect = 0;
        bool linearGeometry = false;
        std::vector < PVec > ownedRows;
        std::vector < std::string > notes;
        
    } ;
    struct C3dPlanTemplateStats 
    {
        uint64_t lookups = 0, hits = 0, misses = 0, inserts = 0, evictions = 0, expired = 0, rejected = 0, bypass = 0, shaderMismatch = 0;
        size_t entries = 0, bytes = 0, budget = 0;
        
    } ;
    class C3dPlanTemplateCache 
    {
        public:
        explicit C3dPlanTemplateCache(size_t budgetBytes = 256u << 10);
        ~C3dPlanTemplateCache();
        C3dPlanTemplateCache(const C3dPlanTemplateCache&) = delete;
        C3dPlanTemplateCache& operator = (const C3dPlanTemplateCache&) = delete;
        void clear();
        void setBudget(size_t budgetBytes);
        void purgeExpired();
        C3dPlanTemplateStats stats() const;
        struct Impl;
        Impl& impl() 
        {
            return * impl_;
            
        }
        private:
        std::unique_ptr < Impl > impl_;
        
    } ;
    C3dPlan buildPlan(const DrawPacket& P,
C3dShaderCache& shaders,
TextureCache& tex,
const GuestMem& mem,
TexEntry * scratchView = nullptr,
bool diagnostics = true,
    C3dPlanTemplateCache * templates = nullptr);
    C3dPlan& buildPlanInto(C3dPlan& out,
const DrawPacket&,
C3dShaderCache&,
TextureCache&,
const GuestMem&,
TexEntry * scratchView = nullptr,
bool diagnostics = true,
    C3dPlanTemplateCache * templates = nullptr);
    void c3dFinalize(C3dPlan& L);
    bool buildPlanDelta(C3dPlan& L,
const DrawPacket& P,
bool diagnostics);
    inline int c3dUniformId(int picassoPos) 
    {
        return picassoPos - 0x20;
        
    }
    uint16_t c3dLodBias(float bias);
    
}
