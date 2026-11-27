#pragma once
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "ww_gxstate.h"
#include "ww_texmap.h"
namespace t2p 
{
    class TexPackReader;
    struct TexPackRecord;
    class GuestMem 
    {
        public:
        virtual ~GuestMem() = default;
        virtual const uint8_t * read(uint32_t addr,
size_t size) const = 0;
        virtual bool hashHint(uint32_t addr,
size_t size,
uint64_t& hash) const 
        {
            (void)addr;
            (void)size;
            (void)hash;
            return false;
            
        }
        
    } ;
    uint64_t sampleHashOf(const uint8_t * p,
size_t n,
size_t sampleBytes);
    bool texKnownContains(uint32_t addr,
uint32_t bytes,
uint64_t hash);
    bool texRenderedAt(uint32_t addr);
    class TlutMemory 
    {
        public:
        static constexpr uint32_t kUnitBytes = 512;
        static constexpr uint32_t kUnits = 1024;
        static constexpr uint32_t kLineBytes = 32;
        TlutMemory();
        TlutMemory(const TlutMemory&) = default;
        TlutMemory& operator = (const TlutMemory& other) 
        {
            if(this != &other)
            {
                ++generation_;
                bytes_ = other.bytes_;
                lineLoaded_ = other.lineLoaded_;
                loads_ = other.loads_;
                
            }
            return * this;
            
        }
        uint64_t generation() const 
        {
            return generation_;
            
        }
        size_t load(const GxTlutLoad& L,
const GuestMem& mem);
        GxTlut resolve(int fmt,
uint32_t offset,
int entries,
uint64_t& hash,
int& missing) const;
        uint64_t hashOf(int fmt,
uint32_t offset,
int entries,
int& missing) const;
        uint32_t loads() const 
        {
            return loads_;
            
        }
        const uint8_t * rawAt(uint32_t offset,
size_t& avail) const 
        {
            const size_t off = size_t(offset) * kUnitBytes;
            if (off >= bytes_.size()) 
            {
                avail = 0;
                return nullptr;
                
            }
            avail = (bytes_.size() - off) / 2;
            return &bytes_[off];
            
        }
        private:
        std::vector < uint8_t > bytes_;
        std::vector < uint8_t > lineLoaded_;
        uint32_t loads_ = 0;
        uint64_t generation_ = 0;
        
    } ;
    struct TexCacheOptions 
    {
        bool directEncode = false;
        size_t budgetBytes = 24u << 20;
        int protectFrames = 2;
        size_t verifyBytesPerFrame = 64u << 10;
        size_t sampleBytes = 512;
        bool fastLossyError = false;
        bool tryEtc1 = true;
        bool memoPalettes = true;
        bool sampleOncePerFrame = false;
        
    } ;
    struct TexCacheStats 
    {
        uint64_t lookups = 0, hits = 0, misses = 0;
        uint64_t conversions = 0;
        uint64_t staleSample = 0, staleVerify = 0, invalidated = 0, evicted = 0;
        uint64_t retiredHits = 0;
        uint64_t hintHits = 0, hintMisses = 0;
        uint64_t packHits = 0, packMisses = 0, packReadFail = 0, packVerified = 0, packVerifyBad = 0, packReadTicks = 0, convTicks = 0, packReadBytes = 0, packSwitchedOff = 0;
        uint64_t packAllocTicks = 0, packIoTicks = 0;
        size_t entries = 0, bytes = 0;
        
    } ;
    struct RenderedTexture 
    {
        uint32_t addr = 0;
        int gxFmt = 0, gxW = 0, gxH = 0;
        uint32_t guestBytes = 0;
        int pfmt = 0;
        int allocW = 0, allocH = 0;
        int portW = 0, portH = 0;
        size_t cost = 0;
        std::vector < std::string > notes;
        
    } ;
    struct TexEntry 
    {
        uint32_t id = 0;
        MappedTexture tex;
        size_t cost = 0;
        uintptr_t user = 0;
        uint32_t addr = 0;
        size_t srcBytes = 0;
        uint64_t sampleHash = 0, fullHash = 0;
        uint64_t lastUseFrame = 0;
        uint64_t sampledFrame = ~uint64_t(0);
        bool rendered = false;
        int gxFmt = 0, gxW = 0, gxH = 0, portW = 0, portH = 0;
        std::vector < std::string > renderedNotes;
        
    } ;
    struct TexLookup 
    {
        const TexEntry * entry = nullptr;
        PicaSamplerDesc sampler;
        bool created = false;
        std::string error;
        std::vector < std::string > notes;
        TexEntry * mutableEntry() const 
        {
            return const_cast < TexEntry * > (entry);
            
        }
        
    } ;
    class TextureCache 
    {
        public:
        explicit TextureCache(const TexCacheOptions& o = 
        {
            
        }
        ) : opt_(o) 
        {
            
        }
        TlutMemory& tlut() 
        {
            return tlut_;
            
        }
        using OutputAllocator = std::function < uint8_t * (TexEntry&,
int,
int,
int,
int,
const std::vector < size_t > &,
size_t) > ;
        void setOutputAllocator(OutputAllocator f,
std::function < void(TexEntry&) > finish)
        {
            onOutput_ = std::move(f);
            onOutputReady_ = std::move(finish);
            
        }
        void setEvictCallback(std::function < void(const TexEntry&) > f) 
        {
            onEvict_ = std::move(f);
            
        }
        TexLookup get(const GxTexBinding& b,
const GuestMem& mem,
const TexMapOptions& mo = 
        {
            
        }
        );
        bool validThisFrame(const TexEntry * e) const 
        {
            const uint32_t i = quickSlot(e);
            for (uint32_t k = 0; k < kQuickProbe; k++) 
            {
                const Quick& q = quick_[(i + k) & (kQuickSlots - 1)];
                if (q.frame != quickGen_) return false;
                if (q.e == e) return true;
                
            }
            return false;
            
        }
        void invalidateRange(uint32_t addr,
uint32_t size);
        void invalidateAll();
        void dropRendered();
        TexEntry * addRendered(const RenderedTexture& r,
bool reuse = false);
        const TexEntry * renderedAt(uint32_t addr) const;
        uint8_t scaledRenderedTexmaps(const GxTexBinding bindings[8]) const;
        void beginFrame(const GuestMem& mem);
        // Evict unprotected LRU entries until under budget or cand exhausted.
        bool pressureEvict();
        void publishKnown() const;
        size_t sampleBytes() const 
        {
            return opt_.sampleBytes;
            
        }
        TexCacheStats stats() const;
        void setPack(const TexPackReader * p,
bool verify) 
        {
            pack_ = p;
            packVerify_ = verify;
            
        }
        uint64_t frame() const 
        {
            return frame_;
            
        }
        struct EntryInfo 
        {
            uint32_t addr;
            int width, height, fmt, levels, wrapS, wrapT, tlutFmt;
            bool needAlpha;
            uint64_t tlutHash;
            
        } ;
        void forEachEntry(const std::function < void(const EntryInfo&,
const TexEntry&) > & f) const 
        {
            for (const auto& kv : entries_) 
            {
                const Key& k = kv.second.first;
                f(EntryInfo
                {
                    k.addr,
k.width,
k.height,
k.fmt,
k.levels,
k.wrapS,
k.wrapT,
k.tlutFmt,
k.needAlpha,
k.tlutHash
                }
                ,
* kv.second.second);
                
            }
            
        }
        private:
        struct Key 
        {
            uint32_t addr;
            uint16_t width, height;
            uint8_t fmt, levels, wrapS, wrapT;
            bool needAlpha;
            uint8_t tlutFmt;
            uint64_t tlutHash;
            bool operator < (const Key& o) const;
            
        } ;
        void erase(uint32_t id);
        void retire(uint32_t id);
        void dropRetired(size_t keep);
        struct Retired 
        {
            struct Key key;
            std::unique_ptr < TexEntry > e;
            
        } ;
        std::vector < Retired > retired_;
        size_t retiredBytes_ = 0;
        static constexpr size_t kRetiredMax = 24, kRetiredBytes = 1u << 20;
        struct PaletteMemo 
        {
            bool valid = false;
            int fmt = 0, entries = 0, missing = 0;
            uint32_t offset = 0;
            uint64_t generation = 0, hash = 0;
            
        } ;
        PaletteMemo paletteMemo_[16];
        TexCacheOptions opt_;
        const TexPackReader * pack_ = nullptr;
        bool packOff_ = false;
        bool packVerify_ = false;
        bool loadFromPack(const GxTexBinding& b,
const TexPackRecord& r,
TexEntry& e);
        TlutMemory tlut_;
        OutputAllocator onOutput_;
        std::function < void(TexEntry&) > onOutputReady_;
        std::function < void(const TexEntry&) > onEvict_;
        std::map < Key, uint32_t > byKey_;
        std::map < uint32_t, uint32_t > rendered_;
        std::map < uint32_t, std::pair < Key, std::unique_ptr < TexEntry >> > entries_;
        uint32_t nextId_ = 1;
        uint64_t frame_ = 0;
        static constexpr uint32_t kQuickSlots = 2048, kQuickProbe = 8;
        struct Quick 
        {
            const TexEntry * e = nullptr;
            uint64_t frame = ~uint64_t(0);
            
        } ;
        Quick quick_[kQuickSlots];
        uint64_t quickGen_ = 1;
        static uint32_t quickSlot(const TexEntry * e) 
        {
            return uint32_t((uintptr_t(e) >> 3) * 2654435761u) >> 21;
            
        }
        void markValid(const TexEntry * e) 
        {
            const uint32_t i = quickSlot(e);
            for (uint32_t k = 0; k < kQuickProbe; k++) 
            {
                Quick& q = quick_[(i + k) & (kQuickSlots - 1)];
                if (q.frame != quickGen_) 
                {
                    q.e = e;
                    q.frame = quickGen_;
                    return;
                    
                }
                if (q.e == e) return;
                
            }
            
        }
        TexLookup getImpl(const GxTexBinding& b,
const GuestMem& mem,
const TexMapOptions& mo);
        uint32_t verifyCursor_ = 0;
        size_t bytes_ = 0;
        TexCacheStats st_;
        
    } ;
    uint64_t hashBytes(const uint8_t * p,
size_t n,
uint64_t seed = 0);
    
}
