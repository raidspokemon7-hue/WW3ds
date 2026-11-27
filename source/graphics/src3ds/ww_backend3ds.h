#include "ww_texture_binding_key.h"
#pragma once
#include <3ds.h>
#include <citro3d.h>
#include <functional>
#include <string>
#include <utility>
#include <unordered_map>
#include <memory>
#include <vector>
#include "ww_c3dplan.h"
#include "ww_prepared_draw.h"
#include "ww_efbcopy.h"
#include "ww_multiplan.h"
namespace t2p 
{
    class Backend3ds 
    {
        public:
        void setKickDraws(int n)
        {
            kickDraws_ = n > 0 
&& n < 4096?n:0;
            
        }
        int kickDraws() const 
        {
            return kickDraws_;
            
        }
        void setSplitStep(float f)
        {
            splitStep_ = f > 0.005f 
&& f <= 0.5f?f:0.25f;
            
        }
        void setCopyReuse(bool on)
        {
            copyReuse_ = on;
            
        }
        uint64_t copyReuses()const
        {
            return copyReuses_;
            
        }
        void setTextureBindingMemo(bool on)
        {
            textureBindingMemo_ = on;
            
        }
        const void * textureLevel0(const TexEntry& e,
size_t& bytes,
int& width,
int& height) const;
        using Log = std::function < void(const std::string&) > ;
        struct Stats 
        {
            uint32_t draws = 0, copyAllocFails = 0, rtLive = 0, rtVramBytes = 0, refused = 0, noArena = 0, noCmdBuf = 0, splits = 0, kicks = 0;
            uint32_t shadersLoaded = 0, texturesUploaded = 0;
            uint64_t textureBytes = 0, arenaPeak = 0;
            uint32_t stateSkips = 0;
            uint32_t copies = 0, clears = 0;
            uint32_t twoPass = 0;
            uint32_t viewRepacks = 0;
            
        } ;
        Backend3ds() = default;
        ~Backend3ds() 
        {
            shutdown();
            
        }
        Backend3ds(const Backend3ds&) = delete;
        Backend3ds& operator = (const Backend3ds&) = delete;
        void setLog(Log l) 
        {
            log_ = std::move(l);
            
        }
        bool init(size_t arenaBytes = 6u << 20);
        void shutdown();
        void beginFrame();
        size_t rtPoolSize() const 
        {
            return rtPool_.size();
            
        }
        void trimRenderPool(size_t keep = 4);
        void flushGarbage();
        void beginTarget(C3D_RenderTarget * t);
        uint8_t * allocateTextureOutput(TexEntry&,
int,
int,
int,
int,
const std::vector < size_t > &,
size_t);
        void finishTextureOutput(TexEntry&);
        size_t geometryMark() const 
        {
            return arenaUsed_;
            
        }
        bool reserveGeometry(size_t vertexBytes,
size_t indexCount,
uint8_t * &,
uint16_t * &);
        void rewindGeometry(size_t mark);
        bool draw(C3dPlan& plan);
        struct RecordedMaterial 
        {
            uint64_t hashAttrs = 0, hashTexEnv = 0, hashEffect = 0;
            std::vector < uint32_t > words;
            uint16_t at[5] = 
            {
                
            } ;
            std::vector < uint8_t > state;
            
        } ;
        struct UniformLayout 
        {
            struct Row 
            {
                uint8_t id, uniform, row;
                
            } ;
            std::vector < Row > rows;
            
        } ;
        struct RecordedDraw 
        {
            bool prepared = false;
            bool ok = false;
            std::shared_ptr < const UniformLayout > layout;
            uint64_t serial = 0;
            std::shared_ptr < const RecordedMaterial > mat;
            std::vector < uint32_t > words;
            uint16_t at[5] = 
            {
                
            } ;
            std::vector < uint8_t > state;
            const void * program = nullptr;
            uint8_t unitKey[3] = 
            {
                
            } ;
            TextureBindingKey sampler[3];
            uint64_t gen[3] = 
            {
                
            } ;
            PixelRect scissor;
            size_t bytes() const 
            {
                return words.capacity() * 4 + state.capacity();
                
            }
            
        } ;
        bool loadPreparedDraw(const char * path,
bool verify = false);
        uint64_t preparedBuilds()const
        {
            return preparedBuilds_;
            
        }
        uint64_t preparedSubmits()const
        {
            return preparedSubmits_;
            
        }
        uint64_t preparedMismatches()const
        {
            return preparedMismatches_;
            
        }
        size_t materialBytes() const 
        {
            return matBytes_;
            
        }
        bool recordDraw(const C3dPlan& L,
RecordedDraw& R);
        bool drawRecorded(C3dPlan& L,
RecordedDraw& R);
        uint32_t recordedDraws() const 
        {
            return recordedDraws_;
            
        }
        struct RecordedItem 
        {
            C3dPlan * plan = nullptr;
            RecordedDraw * record = nullptr;
            
        } ;
        void setCommandBlocks(bool on)
        {
            commandBlockEnable_ = on;
            
        }
        int drawRecordedBlock(const RecordedItem * items,
size_t count);
        uint32_t blockCommands()const
        {
            return commandBlocks_;
            
        }
        uint32_t commandBlockDraws()const
        {
            return commandBlockDraws_;
            
        }
        struct UniformRowWords 
        {
            uint8_t reg = 0;
            uint32_t xyzw[4] = 
            {
                
            } ;
            
        } ;
        static bool makeUniformCommands(const UniformRowWords * rows,
size_t n,
std::vector < uint32_t > & out);
        bool setMainView(TexEntry& view,
C3D_RenderTarget * mainTarget);
        bool copy(EfbCopyPlan& plan,
TextureCache& cache,
C3D_RenderTarget * mainTarget);
        bool allocRenderedMips(TexEntry& e,
int levels);
        bool drawIntoLevel(TexEntry& e,
int level,
C3dPlan& L,
C3D_RenderTarget * mainTarget);
        bool drawTwoPass(C3dMultiPlan& plan,
C3D_RenderTarget * scratch,
C3D_RenderTarget * mainTarget);
        bool textureImage(const TexEntry& e,
const void * * data,
uint32_t * bytes) const;
        void release(const TexEntry& e);
        void releaseShader(C3dShader& s);
        const std::string& lastError() const 
        {
            return err_;
            
        }
        static float cmdUsage();
        const Stats& stats() const 
        {
            return st_;
            
        }
        private:
        struct ShaderHandle;
        struct GpuTex;
        bool fail(const std::string& s);
        bool allocRendered(TexEntry& e);
        void refreshViews(C3dPlan& L);
        void drawIdOverride(C3dPlan& L);
        public:
        void beginDrawIdFrame(bool on);
        private:
        void freeGpuTex(GpuTex * g);
        bool loadShader(C3dShader& s);
        bool uploadTexture(TexEntry& e);
        void * arenaAlloc(size_t bytes,
size_t align);
        void invalidateCaches();
        public:
        void flushArena();
        private:
        Log log_;
        std::string err_;
        Stats st_;
        u8 * arenas_[2] = 
        {
            nullptr, nullptr
        } ;
        u8 * arena_ = nullptr;
        int arenaActive_ = 0;
        size_t arenaSize_ = 0, arenaUsed_ = 0, arenaFlushed_ = 0;
        float splitAt_ = 0.25f;
        int kickDraws_ = 0, sinceKick_ = 0;
        float splitStep_ = 0.25f;
        public:
        std::function < bool() > linearPressure;
        private:
        C3D_Tex dummy_;
        C3D_Tex slot_[3];
        C3D_FogLut fogLut_;
        bool dummyOk_ = false;
        uint64_t frameNo_ = 0;
        std::vector < GpuTex * > rtPool_;
        std::vector < std::pair < GpuTex * , uint64_t >> garbage_;
        const ShaderHandle * program_ = nullptr;
        uint64_t attrHash_ = 0, texenvHash_ = 0, effectHash_ = 0;
        bool attrValid_ = false, texenvValid_ = false, effectValid_ = false;
        float uniformShadow_[96][4];
        bool uniformValid_[96];
        C3D_Tex * bound_[3] = 
        {
            nullptr, nullptr, nullptr
        } ;
        uint8_t boundKey_[3] = 
        {
            0, 0, 0
        } ;
        TextureBindingKey boundSampler_[3];
        bool textureBindingMemo_ = true;
        bool copyReuse_ = false;
        uint64_t copyReuses_ = 0;
        PixelRect scissor_;
        bool scissorValid_ = false;
        uint64_t lastHashes_[3] = 
        {
            
        } ;
        const void * lastIndices_ = nullptr;
        uint32_t lastIndexCount_ = 0;
        uint32_t recordedDraws_ = 0;
        struct CommandBlock 
        {
            std::vector < uint64_t > serials;
            std::vector < uint32_t > words;
            std::vector < uint16_t > patches;
            std::vector < uint16_t > inputRows;
            uint32_t masks[7][3] = 
            {
                
            } ;
            
        } ;
        std::unique_ptr < PreparedCommands > preparedDraw_;
        bool preparedVerify_ = false;
        uint64_t preparedBuilds_ = 0, preparedSubmits_ = 0, preparedMismatches_ = 0;
        bool commandBlockEnable_ = false;
        std::vector < std::shared_ptr < const UniformLayout >> uniformLayouts_;
        std::vector < CommandBlock > commandBlocksCache_;
        size_t commandBlocksBytes_ = 0;
        uint64_t nextRecordedSerial_ = 1;
        uint32_t commandBlocks_ = 0, commandBlockDraws_ = 0;
        std::unordered_map < uint64_t, std::shared_ptr < RecordedMaterial >> mats_;
        size_t matBytes_ = 0;
        
    } ;
    
}
