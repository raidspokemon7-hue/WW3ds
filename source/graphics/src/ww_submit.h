#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "ww_draw.h"
#include "ww_frame.h"
#include "ww_gxfifo.h"
#include "ww_gxstate.h"
namespace t2p 
{
    extern bool g_packPlanar;
    extern bool g_packVertexMajor;
    class DrawCompileCache;
    namespace matrixbatch 
    {
        class Group;
        
    }
    class PackedGeometryCache 
    {
        public:
        struct Entry 
        {
            uint64_t geoId = 0;
            uint32_t stride = 0;
            std::vector < uint32_t > sig;
            std::vector < uint8_t > vertices;
            std::vector < uint16_t > indices;
            bool missingNrm = false, missingClr = false, indicesChecked = false;
            uint64_t frame = 0;
            
        } ;
        Entry * find(uint64_t key,
uint64_t geoId,
uint32_t stride,
const std::vector < uint32_t > & sig);
        Entry * insert(uint64_t key);
        uint64_t frame() const 
        {
            return frame_;
            
        }
        void endFrame();
        void clear() 
        {
            map_.clear();
            bytes_ = 0;
            
        }
        void setBudget(size_t b) 
        {
            budget_ = b;
            
        }
        size_t bytes() const 
        {
            return bytes_;
            
        }
        size_t entries() const 
        {
            return map_.size();
            
        }
        private:
        std::unordered_map < uint64_t, Entry > map_;
        uint64_t frame_ = 1;
        size_t budget_ = 6u << 20, bytes_ = 0;
        
    } ;
    struct SubmitAttr 
    {
        DrawAttr::Kind kind = DrawAttr::Pos;
        int index = 0;
        int reg = 0;
        int components = 3;
        bool ubyte = false;
        uint32_t offset = 0;
        
    } ;
    struct SubmitUnit 
    {
        int texmap = - 1;
        int texcoord = - 1;
        bool scratch = false;
        bool needAlpha = false;
        GxTexBinding tex;
        float scaleS = 1, scaleT = 1;
        
    } ;
    struct ResidentGeometryKey 
    {
        std::vector < uint64_t > ids;
        std::vector < int8_t > slots;
        std::vector < uint32_t > sig;
        uint32_t stride = 0, vertexCount = 0, indexCount = 0;
        bool planar = false;
        bool operator == (const ResidentGeometryKey& o) const 
        {
            return stride == o.stride 
&& vertexCount == o.vertexCount 
&& indexCount == o.indexCount 
&& planar == o.planar 
&& ids == o.ids 
&& slots == o.slots 
&& sig == o.sig;
            
        }
        
    } ;
    struct ResidentGeometryHit 
    {
        uint8_t * vertices = nullptr;
        uint16_t * indices = nullptr;
        bool missingNrm = false, missingClr = false, indicesChecked = false;
        
    } ;
    struct ResidentGeometryLookup 
    {
        const uint64_t * ids = nullptr;
        size_t idCount = 0;
        void * context = nullptr;
        bool ( * find)(void * ,
uint64_t hash,
const ResidentGeometryKey&,
ResidentGeometryHit&) = nullptr;
        ResidentGeometryKey key;
        uint64_t hash = 0;
        bool hit = false;
        
    } ;
    struct PacketGeometryOutput 
    {
        void * context = nullptr;
        bool ( * allocate)(void * ,
size_t,
size_t,
uint8_t * &,
uint16_t * &) = nullptr;
        ResidentGeometryLookup * resident = nullptr;
        
    } ;
    struct DrawPacket 
    {
        bool ok = false;
        const PicaDraw& pica() const;
        std::weak_ptr < const PicaDraw > picaOwner() const 
        {
            return pica_;
            
        }
        PicaDraw& picaMut();
        void setPica(std::shared_ptr < const PicaDraw > p) 
        {
            pica_ = std::move(p);
            
        }
        std::shared_ptr < PackedUniforms > fixedUniforms;
        bool usesPackedUniforms = false;
        UniformMap uniforms;
        std::vector < SubmitAttr > layout;
        uint32_t stride = 0;
        uint32_t vertexCount = 0;
        bool planar = false;
        std::vector < uint8_t > vertices;
        std::vector < uint16_t > indices;
        uint8_t * externalVertices = nullptr;
        uint16_t * externalIndices = nullptr;
        size_t externalIndexCount = 0;
        const uint8_t * vertexData() const 
        {
            return externalVertices ? externalVertices : vertices.data();
            
        }
        size_t vertexBytes() const 
        {
            return externalVertices ? size_t(stride) * vertexCount : vertices.size();
            
        }
        const uint16_t * indexData() const 
        {
            return externalIndices ? externalIndices : indices.data();
            
        }
        size_t indexCount() const 
        {
            return externalIndices ? externalIndexCount : indices.size();
            
        }
        std::vector < HwStage > texenv;
        uint32_t texenvUpdateBuffer = 0;
        SubmitUnit units[3];
        GxViewport viewport;
        GxScissor scissor;
        uint8_t cullMode = 2;
        PicaFrame frame;
        bool colorUpdate = true, alphaUpdate = true, dither = false;
        bool dstAlphaEnable = false;
        uint8_t dstAlpha = 0;
        uint8_t pixelFmt = 0;
        bool zCompBeforeTex = false;
        float lodFrameBias = 0;
        std::vector < std::string > notes;
        PackedGeometryCache::Entry * packedFrom = nullptr;
        uint64_t packKey = 0, packGeoId = 0;
        std::vector < uint32_t > packSig;
        bool packMissingNrm = false, packMissingClr = false;
        bool indicesChecked = false;
        std::shared_ptr < DrawPacket > second;
        private:
        std::shared_ptr < const PicaDraw > pica_;
        
    } ;
    DrawPacket buildDraw(const GxState&,
const GxDraw&,
const Options& = {}
    ,
const FrameConfig& = {}
    ,
DrawCompileCache * cache = nullptr);
    void buildDrawInto(DrawPacket& P,
const GxState&,
const GxDraw&,
const Options& = {}
    ,
const FrameConfig& = {}
    ,
DrawCompileCache * cache = nullptr,
bool notes = true,
    PackedGeometryCache * packCache = nullptr,
uint64_t geoId = 0,
const PacketGeometryOutput * output = nullptr,
const matrixbatch::Group * group = nullptr,
std::shared_ptr < const PicaDraw > compiled = {}
    ,
std::shared_ptr < PackedUniforms > preparedUniforms = {}
    );
    void releasePacked(DrawPacket& P,
PackedGeometryCache& cache,
bool planOk);
    
}
