#include "ww_uniform_recipe.h"
#pragma once
#include <cstdio>
#include "ww_texpack.h"
#include "ww_texture_store.h"
#include "ww_drawreuse3ds.h"
#include <unordered_map>
#include <unordered_set>
#include <3ds.h>
#include <citro3d.h>
#include <deque>
#include "ww_state_vector_pool3ds.h"
#include <functional>
#include <memory>
#include <atomic>
#include <string>
#include <vector>
#include "ww_backend3ds.h"
#include "ww_bundle.h"
#include "ww_c3dplan.h"
#include "ww_drawcache.h"
#include "ww_efbcopy.h"
#include "ww_frame.h"
#include "ww_gxbatch.h"
#include "ww_matrixbatch.h"
#include "ww_gxfifo.h"
#include "ww_direct_draw_binding.h"
#include "ww_gxretain.h"
#include "ww_gxstate.h"
#include "ww_submit.h"
#include "ww_packet_worker3ds.h"
#include "ww_texcache.h"
#include "ww_translate.h"
namespace t2p 
{
    struct RendererOptions 
    {
        FrameConfig frame;
        Options translate = [] 
        {
            Options o;
            o.verifySamples = 0;
            o.approxEffort = 0;
            return o;
            
        }
        ();
        bool materialCacheFastPath = true;
        size_t drawCacheEntries = 4096;
        TexCacheOptions texCache = [] 
        {
            TexCacheOptions t;
            t.tryEtc1 = false;
            t.sampleOncePerFrame = true;
            return t;
            
        }
        ();
        size_t arenaBytes = 6u << 20;
        bool workerPackets = false, verifyPackets = false;
        bool frameTiming = false;
        bool templatePlans = false;
        bool matrixBatchProfile = false;
        bool batchMatrices = false;
        bool packDirect = false;
        bool keepPlanNotes = true;
        bool packetNotes = true;
        uint32_t batchMaxVertices = kBatchMaxVertices;
        uint8_t initialClear[4] = 
        {
            0, 0, 0, 255
        } ;
        uint8_t frameFlags = 0;
        
    } ;
    struct DrawReport 
    {
        int index = 0;
        bool packetOk = false, planOk = false, drawn = false;
        bool twoPass = false;
        uint32_t vertices = 0, triangles = 0;
        std::string why;
        const std::vector < std::string > * packetNotes = nullptr;
        const std::vector < std::string > * keepPlanNotes = nullptr;
        double buildMs = 0, planMs = 0, drawMs = 0;
        
    } ;
    struct CopyReport 
    {
        int index = 0;
        bool toXfbClear = false;
        const GxCopy * copy = nullptr;
        const EfbCopyPlan * plan = nullptr;
        bool done = false;
        std::string why;
        double planMs = 0, drawMs = 0;
        
    } ;
    extern uint64_t g_replayWhy[4];
    struct FrameReport 
    {
        uint32_t packetJobs = 0, packetChecks = 0, packetMismatches = 0;
        uint64_t packetWaitTicks = 0;
        bool rendered = false;
        bool skipped = false;
        uint32_t draws = 0, drawn = 0, refused = 0, copies = 0, copiesDone = 0, tlutLoads = 0;
        uint32_t gxDraws = 0;
        uint32_t clipTests = 0, clipCulled = 0;
        uint32_t twoPass = 0;
        uint64_t beginWaitTicks = 0, endSubmitTicks = 0;
        uint64_t matrixSplit[6] = 
        {
            
        } ;
        uint64_t splitTicks[8] = 
        {
            
        } ;
        C3dPlanTemplateStats templateStats;
        uint32_t reuseHits = 0, reuseMisses = 0, reuseInserts = 0, reuseEvicted = 0, reuseFull = 0;
        size_t reuseBytes = 0, reuseEntries = 0;
        uint32_t planDeltas = 0, planDeltaFails = 0, planDeltaChecks = 0, planDeltaMismatches = 0;
        uint32_t residentHits = 0, residentMisses = 0, residentInserts = 0, residentFull = 0;
        size_t residentBytes = 0, residentEntries = 0;
        uint32_t modelProgramHits = 0, modelProgramDraws = 0, modelProgramReject[6] = 
        {
            
        } ;
        uint32_t modelGroupPrograms = 0, modelGroupMembers = 0;
        uint32_t uniformDeltaHits = 0;
        uint32_t replayHits = 0, replayMisses = 0, replayInserts = 0, replayStale = 0, replayEntries = 0, replayProj = 0, replayLimitKb = 0, replayRecorded = 0;
        uint32_t directDraws = 0, directFallbacks = 0, directGroups = 0;
        uint32_t replayGroupHits = 0, replayGroupMisses = 0;
        uint32_t routeMarkers = 0, routedDraws = 0, routeRefused = 0, routeCopies = 0, routeSwitches = 0;
        uint64_t matrixTicks[3] = 
        {
            
        } ;
        uint32_t matrixSamples = 0, matrixAttempts = 0;
        float fogRange = 0;
        double buildMs = 0, planMs = 0, drawMs = 0;
        
    } ;
    struct RecordedEvent3ds 
    {
        enum Kind 
        {
            Draw, TexCopy, Xfb, Tlut, Route, DirectDraw 
        }
        kind = Draw;
        DeferredDrawState stateStorage;
        RecordedEvent3ds()
        {
            
        }
        GxState& state()
        {
            return stateStorage.state();
            
        }
        const GxState& state()const
        {
            return stateStorage.state();
            
        }
        const GxState& viewState()const
        {
            return stateStorage.view();
            
        }
        bool isDraw()const
        {
            return kind == Draw || kind == DirectDraw;
            
        }
        void setDirect(std::shared_ptr < const DirectDrawBinding > owner,
uint32_t row,
uint32_t count)
        {
            stateStorage.setDirect(std::move(owner),
row,
count);
            kind = DirectDraw;
            
        }
        void legacy()
        {
            if(kind == DirectDraw)
            {
                stateStorage.reset();
                kind = Draw;
                
            }
            
        }
        std::shared_ptr < const DrawRegisterSignature > rawSignature;
        GxDraw d
        {
            
        } ;
        std::shared_ptr < const RetainedGeometry > geo;
        uint32_t merged = 1;
        uint32_t dirtyIn = 0xFFFFFFFFu;
        uint64_t rk[2] = 
        {
            0, 0
        } ;
        int maxPm = - 1;
        GxCopy c
        {
            
        } ;
        GxTlutLoad tl
        {
            
        } ;
        std::vector < uint8_t > tlutBytes;
        GxUiRoute route
        {
            
        } ;
        
    } ;
    void computeReplayKey(RecordedEvent3ds& e);
    struct EventNodePool 
    {
        static FixedBlockPool& get() 
        {
            static FixedBlockPool * p = new FixedBlockPool(768,
sizeof(RecordedEvent3ds));
            return * p;
            
        }
        
    } ;
    using EventDeque3ds = std::deque < RecordedEvent3ds, PoolAlloc < RecordedEvent3ds, EventNodePool >> ;
    struct RecordedFrame3ds 
    {
        std::vector < PVec > directRows;
        uint32_t directCaptured = 0, directRejected = 0, directChecks = 0, directMismatches = 0;
        uint32_t directDiag[16] = 
        {
            
        } ;
        EventDeque3ds events;
        StateVectorPool3ds poolState;
        std::vector < GxDraw > drawPool;
        size_t drawPoolRead = 0;
        size_t drawPoolBytes = 0, framesReady = 0;
        bool keepPicture = false;
        
    } ;
    class Renderer3ds : public GxSink 
    {
        public:
        using DrawLog = std::function < void(const DrawReport&) > ;
        using CopyLog = std::function < void(const CopyReport&) > ;
        using ErrorLog = std::function < void(const std::string&) > ;
        using DisplayList = std::function < const uint8_t * (uint32_t addr,
uint32_t size) > ;
        Renderer3ds() : fifo_(this) 
        {
            
        }
        ~Renderer3ds() 
        {
            shutdown();
            
        }
        Renderer3ds(const Renderer3ds&) = delete;
        Renderer3ds& operator = (const Renderer3ds&) = delete;
        bool init(const RendererOptions& o,
GuestMem * mem,
C3D_RenderTarget * target);
        void shutdown();
        bool ready() const 
        {
            return ready_;
            
        }
        size_t process(const uint8_t * data,
size_t size) 
        {
            return fifo_.process(data,
size);
            
        }
        uint32_t primitiveRunLimit() const override 
        {
            if(!ready_ || !opt_.batchMaxVertices)return 0;
            if(!evs_.empty() 
&& evs_.back().kind == Ev::Draw 
&& lastDrawGen_ == fifo_.stateGeneration() 
&& gxTriCount(evs_.back().d) != 0)
            return opt_.batchMaxVertices - std::min < uint32_t > (opt_.batchMaxVertices,
evs_.back().d.v.count);
            return opt_.batchMaxVertices;
            
        }
        GxFifo& fifo() 
        {
            return fifo_;
            
        }
        void setFogPreparation(bool on) 
        {
            fogPreparation_ = on;
            
        }
        void setPlanReuse(bool on) 
        {
            planReuse_ = on;
            
        }
        void setPlanDelta(bool on,
bool verify) 
        {
            planDelta_ = on;
            planDeltaVerify_ = verify;
            planPrevOwner_.reset();
            
        }
        void setFrameTiming(bool on)
        {
            opt_.frameTiming = on;
            
        }
        void setPlanTemplates(bool on)
        {
            opt_.templatePlans = on;
            if(on 
&& !planTemplates_)planTemplates_ = std::make_unique < C3dPlanTemplateCache > (256u << 10);
            if(!on)planTemplates_.reset();
            
        }
        void setPacketWorker(bool on);
        void setPacketWorkerVerify(bool on)
        {
            opt_.verifyPackets = on;
            
        }
        void stopPacketWorker();
        bool packetWorkerActive() const
        {
            return packetWorker_ 
&& packetWorker_->active();
            
        }
        void setDirectPack(bool on) 
        {
            opt_.packDirect = on;
            
        }
        void setStateNotes(bool on) 
        {
            stateNotes_ = on;
            
        }
        void setRetainGeometry(bool on);
        void setStateDecodeRepeat(int n) 
        {
            stateRepeat_ = n;
            
        }
        void setPlanNotes(bool on) 
        {
            opt_.keepPlanNotes = on;
            
        }
        void setVertexPoolFifo(bool on) 
        {
            vertexPoolFifo_ = on;
            
        }
        void setStatePool(bool on) 
        {
            statePoolEnabled_ = on;
            
        }
        void setStateInPlace(bool on) 
        {
            stateInPlace_ = on;
            
        }
        bool keepGeometry() const 
        {
            return retainOn_;
            
        }
        GxRetainStats takeRetainStats() 
        {
            return retainer_.takeStats();
            
        }
        const PackedGeometryCache& packCache() const 
        {
            return packCache_;
            
        }
        void setPackedUniforms(bool on) 
        {
            opt_.translate.fixedUniforms = on;
            
        }
        void setDrawTimingEnabled(bool on) 
        {
            drawTimingEnabled_ = on;
            
        }
        void setDisplayListResolver(DisplayList f) 
        {
            dl_ = std::move(f);
            
        }
        void invalidateTextures(uint32_t addr,
uint32_t size) 
        {
            cache_.invalidateRange(addr,
size);
            
        }
        void invalidateAllTextures() 
        {
            cache_.invalidateAll();
            
        }
        void dropSceneCaches();
        void requestSceneDrop() 
        {
            sceneDrop_.store(true,
std::memory_order_release);
            
        }
        size_t framesReady() const 
        {
            return framesReady_;
            
        }
        bool loadMaterialBundle(const uint8_t * data,
size_t size,
std::string * err = nullptr);
        void recordBundleMisses(bool on);
        std::vector < uint8_t > missBundle() const 
        {
            return misses_.serialize(sourceFingerprint());
            
        }
        size_t missEntries() const 
        {
            return misses_.translations() + misses_.approximations();
            
        }
        bool loadMissBundle(const uint8_t * data,
size_t size,
std::string * err = nullptr) 
        {
            return misses_.load(data,
size,
sourceFingerprint(),
err);
            
        }
        size_t bundleEntries() const 
        {
            return bundle_.translations();
            
        }
        FrameReport renderFrame(bool force = false);
        void setSkipNextFrame(bool on = true) 
        {
            skipNext_ = on;
            
        }
        void setKeepPicture(bool on) 
        {
            keepPicture_ = on;
            dialogueHoldFrames_ = on ? 8 : 0;
            
        }
        void setHideOutput(bool on) 
        {
            if (on == hideOutput_) return;
            hideOutput_ = on;
            if (!target_) return;
            if (on) 
            {
                if (target_->linked) C3D_RenderTargetDetachOutput(target_);
                
            }
            else if (!target_->linked) C3D_RenderTargetSetOutput(target_,
target_->screen,
target_->side,
target_->transferFlags);
            
        }
        void swapRecording(RecordedFrame3ds& frame);
        void setLogs(DrawLog d,
CopyLog c,
ErrorLog e) 
        {
            drawLog_ = std::move(d);
            copyLog_ = std::move(c);
            errLog_ = std::move(e);
            
        }
        void setFrameBeginHook(std::function < void() > f) 
        {
            beginHook_ = std::move(f);
            
        }
        size_t dumpTextures(std::FILE * f,
uint64_t captureFrame);
        bool loadTexturePack(const char * path,
bool verify,
bool splitStore = false);
        const TextureStore::Stats& textureStoreStats()const
        {
            return texStore_.stats();
            
        }
        TexCacheStats texPackStats() const 
        {
            return cache_.stats();
            
        }
        size_t texturePackEntries() const 
        {
            return texPack_.count();
            
        }
        void setCacheVerifiedHook(std::function < void() > f) 
        {
            cacheVerifiedHook_ = std::move(f);
            
        }
        void setDrawFilter(std::function < bool(int) > f) 
        {
            skipDraw_ = std::move(f);
            
        }
        void setPlanTweak(std::function < void(C3dPlan&,
int) > f) 
        {
            tweak_ = std::move(f);
            
        }
        void setMatrixBatchProfile(bool on)
        {
            opt_.matrixBatchProfile = on;
            
        }
        void setMatrixBatch(bool on) 
        {
            opt_.batchMatrices = on;
            
        }
        void setDrawReuse(bool on,
size_t budgetBytes = 2560u << 10);
        void setDrawReplay(bool on,
size_t budgetBytes = 2048u << 10) 
        {
            replayOn_ = on;
            replayBudget_ = budgetBytes;
            if (!on) 
            {
                replayPrev_.clear();
                replayCur_.clear();
                
            }
            
        }
        size_t drawReplayBytes() const 
        {
            return replayBytes_;
            
        }
        void setDrawReplayBudget(size_t b) 
        {
            replayBudget_ = b;
            
        }
        bool replayDraws() const 
        {
            return replayOn_;
            
        }
        void computeReplayKeys() 
        {
            for (auto& e : evs_) computeReplayKey(e);
            
        }
        void setDrawRecord(bool on) 
        {
            recordOn_ = on;
            
        }
        bool recordDraws() const 
        {
            return recordOn_;
            
        }
        std::shared_ptr < DirectDrawRegistry > directRegistry()const
        {
            return directRegistry_;
            
        }
        bool directSnapshots_ = false;
        public:
        void setDirectSnapshots(bool on)
        {
            directSnapshots_ = on;
            
        }
        void setDirectGroups(bool on,
bool cold = false)
        {
            directGroups_ = on;
            directGroupCold_ = cold;
            
        }
        std::shared_ptr < DirectGroupRegistry > groupRegistry()const
        {
            return directGroupRegistry_;
            
        }
        void setDirectEvents(bool on,
size_t slots = 128)
        {
            directRegistry_->configure(slots);
            directEvents_ = on;
            
        }
        void setModelPrograms(bool on)
        {
            modelPrograms_ = on;
            be_.setCommandBlocks(on || commandBlocks_);
            
        }
        void setUniformDelta(bool on)
        {
            uniformDelta_ = on;
            
        }
        void setCommandBlocks(bool on)
        {
            commandBlocks_ = on;
            be_.setCommandBlocks(on || modelPrograms_);
            
        }
        void setRigidClip(bool on)
        {
            rigidClip_ = on;
            
        }
        void setFlushAllAtEnd(bool on) 
        {
            flushAllAtEnd_ = on;
            
        }
        bool flushAllAtEnd() const 
        {
            return flushAllAtEnd_;
            
        }
        void setResidentGeometry(bool on,
size_t budgetBytes = 2048u << 10);
        void setBatchMaxVertices(uint32_t n) 
        {
            opt_.batchMaxVertices = n;
            
        }
        void setDrawEdit(std::function < void(GxState&,
GxDraw&,
int) > f) 
        {
            edit_ = std::move(f);
            
        }
        Backend3ds& backend() 
        {
            return be_;
            
        }
        TextureCache& cache() 
        {
            return cache_;
            
        }
        C3dShaderCache& shaders() 
        {
            return shaders_;
            
        }
        DrawCompileCache& drawCache() 
        {
            return drawCache_;
            
        }
        const FrameConfig& frameConfig() const 
        {
            return opt_.frame;
            
        }
        uint64_t framesRendered() const 
        {
            return frames_;
            
        }
        void onDraw(const GxRegs& r,
const GxDraw& d) override;
        GxDraw * appendTarget(const GxRegs& r,
const GxDraw& header) override;
        void onAppended(const GxRegs& r,
GxDraw& into,
uint32_t firstVertex) override;
        bool retainOffer(const GxDrawInputs& in,
GxDraw& D) override;
        void retainDecoded(const GxDrawInputs& in,
const GxDraw& D,
uint32_t firstVertex,
size_t firstTri,
bool arrayError) override;
        void onCopy(const GxRegs& r) override;
        void onTlutLoad(const GxRegs& r) override;
        void onUiRoute(const GxRegs& r,
const GxUiRoute& route) override;
        void setUiRoute(bool on,
std::function < void(bool) > owner = 
        {
            
        }
        ) 
        {
            if (owner 
&& !routeOwner_) routeOwner_ = std::move(owner);
            routeOn_.store(on,
std::memory_order_release);
            
        }
        bool uiRouteOwnsBottom() const 
        {
            return routeLinked_;
            
        }
        C3D_RenderTarget * uiRouteTarget() const 
        {
            return routeLinked_ ? routeTarget_ : nullptr;
            
        }
        void onError(const std::string& e) override 
        {
            if (errLog_) errLog_(e);
            
        }
        const uint8_t * resolveDisplayList(uint32_t addr,
uint32_t size) override 
        {
            return dl_ ? dl_(addr,
size) : nullptr;
            
        }
        private:
        using Ev = RecordedEvent3ds;
        struct SnapMem : GuestMem 
        {
            uint32_t addr = 0;
            const std::vector < uint8_t > * bytes = nullptr;
            const uint8_t * read(uint32_t a,
size_t n) const override 
            {
                if (!bytes || a < addr || uint64_t(a) + n > uint64_t(addr) + bytes->size()) return nullptr;
                return bytes->data() + (a - addr);
                
            }
            
        } ;
        bool routeTarget();
        bool routeFrame(const GxState& st,
FrameConfig& cfg) const;
        void drawOne(Ev& x,
const Options& opt,
int index,
FrameReport& R,
const matrixbatch::Group * group = nullptr,
const DrawPacket * prepared = nullptr,
std::shared_ptr < const PicaDraw > known = 
        {
            
        }
        ,
std::shared_ptr < PackedUniforms > rows = 
        {
            
        }
        );
        void closeRetained();
        void recycleEvent(Ev& e);
        void recycleEvents(size_t n,
bool xfb);
        int stateMaxPm(int maxPm) const 
        {
            return retainOn_ ? maxPm : kScanPosMtx;
            
        }
        bool ensureScratch();
        void copyOne(const GxCopy& c,
const GxState& st,
const EfbCopyOptions& co,
int index,
bool xfbClear,
FrameReport& R);
        bool warpOne(const GxCopy& c);
        RendererOptions opt_;
        static constexpr uint32_t kRouteIdleFrames = 30;
        std::atomic < bool > routeOn_
        {
            false
        } ;
        bool routeFailed_ = false, routeLinked_ = false, routeActive_ = false;
        std::function < void(bool) > routeOwner_;
        GxUiRoute routeCur_;
        C3D_RenderTarget * routeTarget_ = nullptr;
        C3D_RenderTarget * curTarget_ = nullptr;
        uint32_t routeIdle_ = 0, routeRetry_ = 0;
        const FrameConfig * drawCfg_ = nullptr;
        bool drawRouted_ = false;
        MaterialBundle bundle_, misses_;
        GuestMem * mem_ = nullptr;
        C3D_RenderTarget * target_ = nullptr;
        bool ready_ = false;
        Backend3ds be_;
        TextureCache cache_;
        TexPackReader texPack_;
        std::FILE * texPackFile_ = nullptr;
        TextureStore texStore_;
        C3dShaderCache shaders_;
        DrawCompileCache drawCache_;
        C3dPlan planScratch_;
        std::unique_ptr < C3dPlanTemplateCache > planTemplates_;
        bool planReuse_ = false;
        bool planDelta_ = false, planDeltaVerify_ = false, planPrevRouted_ = false;
        size_t curEvAt_ = ~size_t(0), planPrevAt_ = ~size_t(0);
        uint64_t planPrevFrame_ = ~0ull;
        std::shared_ptr < const PicaDraw > planPrevOwner_;
        bool stateNotes_ = true;
        bool fogPreparation_ = false;
        FogCompileCache fogCompileCache_;
        bool drawTimingEnabled_ = true;
        bool useDrawCache_ = true;
        TexEntry view_;
        C3D_RenderTarget * scratch_ = nullptr;
        TexEntry scratchView_;
        bool scratchFailed_ = false;
        GxFifo fifo_;
        DisplayList dl_;
        DrawLog drawLog_;
        CopyLog copyLog_;
        ErrorLog errLog_;
        std::function < void() > beginHook_;
        std::function < void() > cacheVerifiedHook_;
        std::function < bool(int) > skipDraw_;
        std::function < void(C3dPlan&,
int) > tweak_;
        std::function < void(GxState&,
GxDraw&,
int) > edit_;
        EventDeque3ds evs_;
        std::vector < PVec > directRows_;
        std::unique_ptr < PacketWorker3ds > packetWorker_;
        DrawPacket packet_;
        size_t drawPoolRead_ = 0;
        bool vertexPoolFifo_ = false;
        uint32_t matrixSamplePhase_ = 0;
        size_t drawPoolBytes_ = 0;
        std::vector < GxDraw > drawPool_;
        bool appendNew_ = false;
        bool retainOn_ = false;
        int stateRepeat_ = 0;
        StateVectorPool3ds statePool_;
        bool statePoolEnabled_ = false;
        bool stateInPlace_ = false;
        bool skipNext_ = false;
        bool keepPicture_ = false;
        int dialogueHoldFrames_ = 0;
        bool hideOutput_ = false;
        GxRetainer retainer_;
        PackedGeometryCache packCache_;
        bool skipped_ = false;
        int skipMaxPm_ = - 1;
        uint64_t lastDrawGen_ = ~0ull;
        size_t framesReady_ = 0;
        std::unique_ptr < Ev > pendingClear_;
        uint64_t frames_ = 0;
        bool reuseOn_ = false;
        size_t reuseBudget_ = 0, reuseBytes_ = 0;
        std::unordered_map < uint64_t, std::unique_ptr < reuse::Entry >> reuse_;
        reuse::Key reuseKey_;
        std::unordered_set < uint64_t > reuseSeenPrev_, reuseSeenCur_;
        void reuseEvict(bool all);
        bool reuseEvictUnused();
        uint64_t reusePressureEvicts_ = 0;
        struct ResidentEntry 
        {
            ResidentGeometryKey key;
            void * mem = nullptr;
            size_t bytes = 0;
            uint64_t lastFrame = 0;
            ResidentGeometryHit hit;
            
        } ;
        struct ReplayEntry;
        using ReplayMap = std::unordered_multimap < uint64_t, std::shared_ptr < ReplayEntry >> ;
        struct ReplayEntry 
        {
            ReplayMap::iterator location;
            uint64_t availableFrame = ~0ull, drawnFrame = ~0ull;
            std::shared_ptr < const UniformRecipe > recipe;
            uint64_t geoId = 0;
            GxDrawState draw;
            std::vector < uint8_t > tail;
            uint64_t rk[2] = 
            {
                0, 0
            } ;
            uint64_t tailHash = 0;
            std::shared_ptr < const VtxUniforms > uniformBlock;
            std::shared_ptr < const CompactUniforms > sectionUniforms;
            DrawPacket packet;
            C3dPlan plan;
            uint64_t residentHash = 0;
            void * residentMem = nullptr;
            ResidentGeometryKey residentKey;
            std::weak_ptr < ResidentEntry > residentLease;
            ResidentEntry * residentPtr = nullptr;
            uint64_t residentGen = ~0ull;
            bool directGroupPublished = false;
            uint32_t groupCount = 0, groupPalette = 0;
            std::vector < uint64_t > groupIds;
            std::vector < std::array < int8_t, 10 >> groupSlots;
            struct RowMap 
            {
                uint8_t uniform, array;
                uint16_t first, count, at;
                
            } ;
            std::unique_ptr < PVec[] > rows;
            std::vector < RowMap > rowMap;
            Backend3ds::RecordedDraw rec;
            size_t bytes = 0;
            size_t * live = nullptr;
            ~ReplayEntry() 
            {
                if (live) * live -= bytes;
                
            }
            
        } ;
        size_t replayBytes_ = 0;
        std::atomic < size_t > replayBudget_
        {
            2048u << 10
        } ;
        std::unique_ptr < PackedUniforms > replayScratch_;
        bool replayOn_ = false;
        std::atomic < bool > sceneDrop_
        {
            false
        } ;
        bool flushAllAtEnd_ = false;
        bool recordOn_ = true;
        bool directGroupCold_ = false;
        bool directGroups_ = false;
        std::shared_ptr < DirectGroupRegistry > directGroupRegistry_ = std::make_shared < DirectGroupRegistry > ();
        void learnDirectGroup(const std::shared_ptr < ReplayEntry > &,
const Options&);
        bool drawDirectGroup(const Options&,
int,
FrameReport&,
size_t&);
        bool directEvents_ = false;
        std::shared_ptr < DirectDrawRegistry > directRegistry_ = std::make_shared < DirectDrawRegistry > ();
        void learnDirect(Ev&,
const std::shared_ptr < ReplayEntry > &);
        void drawDirect(Ev&,
const Options&,
int,
FrameReport&);
        bool commandBlocks_ = false, uniformDelta_ = false, modelPrograms_ = false;
        bool rigidClip_ = false;
        RigidClipPlanes rigidClipPlanes_;
        bool outsideRigidClip(const Ev& x);
        ReplayMap replayPrev_, replayCur_;
        size_t replayCursor_ = 0;
        bool replayNoInsert_ = false;
        bool replayGroups_ = true;
        struct ReplayGroupKey 
        {
            const uint64_t * ids = nullptr;
            const std::array < int8_t, 10 > * slots = nullptr;
            size_t count = 0, palette = 0;
            
        } ;
        bool replayTry(Ev& use,
FrameReport& R,
DrawReport& D,
const ReplayGroupKey * group = nullptr,
Backend3ds::RecordedItem * prepared = nullptr,
std::shared_ptr < ReplayEntry > * resolved = nullptr);
        struct ReplayProgram 
        {
            std::array < std::weak_ptr < ReplayEntry > , 8 > entries;
            uint8_t count = 0, segmentEnd[8] = 
            {
                
            } ;
            
        } ;
        struct GroupPrograms 
        {
            std::array < std::weak_ptr < ReplayEntry > , 4 > entries;
            uint8_t next = 0;
            
        } ;
        std::unordered_map < uint64_t, GroupPrograms > modelGroupPrograms_;
        bool executeGroupProgram(int index,
FrameReport&,
size_t& consumed);
        std::unordered_map < uint64_t, ReplayProgram > modelProgramsCache_;
        std::vector < std::shared_ptr < const UniformRecipe >> uniformRecipes_;
        bool executeModelProgram(int index,
FrameReport&,
size_t& consumed);
        void learnModelProgram(const std::shared_ptr < ReplayEntry > * ,
size_t count);
        bool replayEvent(Ev& x,
int index,
FrameReport& R,
size_t * consumed = nullptr);
        bool replayGroupEvent(Ev& x,
const matrixbatch::Group& group,
int index,
FrameReport& R,
const Options&);
        bool replayInsert(const GxState& st,
uint64_t geoId,
const uint64_t * rk,
const ReplayGroupKey * group = nullptr,
std::shared_ptr < ReplayEntry > * inserted = nullptr);
        bool residentOn_ = false;
        size_t residentBudget_ = 0, residentBytes_ = 0;
        std::unordered_map < uint64_t, std::shared_ptr < ResidentEntry >> resident_;
        std::unordered_set < uint64_t > residentSeenPrev_, residentSeenCur_;
        uint64_t residentGen_ = 0;
        ResidentGeometryLookup residentLookup_;
        static bool residentFind(void * self,
uint64_t hash,
const ResidentGeometryKey& key,
ResidentGeometryHit& out);
        void residentEvict(bool all);
        bool residentEvictUnused();
        void updateLinearPressure();
        
    } ;
    
}
