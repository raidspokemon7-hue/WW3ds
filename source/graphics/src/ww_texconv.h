#pragma once
#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>
namespace t2p 
{
    extern uint64_t g_texProf[8];
    uint64_t texProfTicks();
    enum GxTexFmt : int 
    {
        GXF_I4 = 0, GXF_I8 = 1, GXF_IA4 = 2, GXF_IA8 = 3, GXF_RGB565 = 4, GXF_RGB5A3 = 5, GXF_RGBA8 = 6, GXF_C4 = 8, GXF_C8 = 9, GXF_C14X2 = 10, GXF_CMPR = 14 
    } ;
    enum GxTlutFmt : int 
    {
        GXT_IA8 = 0, GXT_RGB565 = 1, GXT_RGB5A3 = 2 
    } ;
    struct GxTlut 
    {
        std::vector < uint8_t > rgba;
        
    } ;
    GxTlut makeGxTlut(int tlutFmt,
const uint8_t * bigEndianData,
int entries);
    size_t gxTextureBytes(int fmt,
int w,
int h);
    bool gxFormatHasPalette(int fmt);
    bool decodeGxTexture(int fmt,
int w,
int h,
const uint8_t * data,
size_t size,
const GxTlut * tlut,
std::vector < uint8_t > & rgba,
std::string& err);
    enum PicaTexFmt : int 
    {
        PTF_RGBA8 = 0, PTF_RGB8 = 1, PTF_RGB5A1 = 2, PTF_RGB565 = 3, PTF_RGBA4 = 4, PTF_IA8 = 5, PTF_HILO8 = 6, PTF_I8 = 7, PTF_A8 = 8, 
        PTF_IA4 = 9, PTF_I4 = 10, PTF_A4 = 11, PTF_ETC1 = 12, PTF_ETC1A4 = 13 
    } ;
    const char * picaTexFmtName(int fmt);
    size_t picaTextureBytes(int fmt,
int w,
int h);
    void picaSampleTexel(int fmt,
const uint8_t * data,
int w,
int x,
int y,
uint8_t rgba[4]);
    std::vector < uint8_t > picaDecodeImage(int fmt,
const uint8_t * data,
int w,
int h);
    bool picaEncode(int fmt,
const std::vector < uint8_t > & rgba,
int w,
int h,
std::vector < uint8_t > & out,
bool compareAlpha = true,
bool verify = true);
    void etc1EncodeBlock(const uint8_t rgb[16][3],
uint64_t& out,
int quality);
    void picaEncodeEtc1(const std::vector < uint8_t > & rgba,
int w,
int h,
bool withAlpha,
int quality,
std::vector < uint8_t > & out);
    using TextureOutput = std::function < uint8_t * (int,
int,
int,
int,
const std::vector < size_t > &,
size_t) > ;
    struct TexConvOptions 
    {
        TextureOutput output;
        bool needAlpha = true;
        bool allowLossy = true;
        int lossyMinTexels = 1024;
        int etc1Quality = 1;
        bool fastLossyError = false;
        bool tryEtc1 = true;
        double lossyMeanErrLimit = 2.0;
        double lossyMaxErrLimit = 32.0;
        
    } ;
    struct PicaTexture 
    {
        bool externalOutput = false;
        size_t encodedBytes = 0;
        int fmt = PTF_RGBA8;
        int width = 0, height = 0;
        std::vector < uint8_t > data;
        int levels = 1;
        std::vector < size_t > levelOffset;
        bool exact = true;
        double maxErr255 = 0, meanErr255 = 0;
        std::string note;
        
    } ;
    PicaTexture chooseFormat(const std::vector < uint8_t > & rgba,
int w,
int h,
const TexConvOptions& = {}
    );
    struct MipImage 
    {
        int w = 0, h = 0;
        std::vector < uint8_t > rgba;
        int sourceWidth = 0;
        std::vector < int > sourceX, sourceY;
        const uint8_t * pixel(int x,
int y) const 
        {
            if (sourceX.empty()) return rgba.data() + (size_t(y) * w + x) * 4;
            return rgba.data() + (size_t(sourceY[y]) * sourceWidth + sourceX[x]) * 4;
            
        }
        
    } ;
    PicaTexture chooseFormatLevels(const std::vector < MipImage > & levels,
const TexConvOptions& = {}
    );
    
}
