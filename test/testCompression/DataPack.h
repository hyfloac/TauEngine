#pragma once

#include "NumTypes.h"

#ifdef _WIN32
  #pragma pack(push, 1)
  #define PACKED
#else
  #define PACKED __attribute__((packed))
#endif

#ifdef __cplusplus
  #define ENUM_SIZE(__TYPE) : __TYPE
#else
  #define ENUM_SIZE(__TYPE)
#endif

#define DP_MAGIC 0xAA153DE9

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef enum ENUM_SIZE(u8)
{
    NONE = 0,
    DEFLATE,
    LZMA,
    LZMA2
} DPCompression;

static inline const char* dpCompStr(const DPCompression dpCompression)
{
    switch(dpCompression)
    {
        case NONE:    return "None";
        case DEFLATE: return "DEFLATE";
        case LZMA:    return "LZMA";
        case LZMA2:   return "LZMA2";
        default:      return "Unknown";
    }
}

typedef enum ENUM_SIZE(u16)
{
    TEXT = 0,
    MODEL,
    TEXTURE,
    WAV_OBJ,
    FBX_ASCII,
    FBX_BINARY,
    PNG,
    JPEG,
    JPEG2000,
    TIFF_LE,
    TIFF_BE,
    BMP,
    GIF,
    WAV_AUDIO,
    MP3,
    MP4,
    M4V,
    MOV,
    PDF,
    HTML,
    BINARY_HTML,
    XML,
    BINARY_XML,
    YAML,
    BINARY_YAML,
    JSON,
    BSON,
    JAVASCIRPT,
    LUA,
    PYTHON2,
    PYTHON3
} DPFileType;

static inline const char* dpFTStr(const DPFileType dpFileType)
{
    switch(dpFileType)
    {
        case TEXT:        return "Text";
        case MODEL:       return "Model";
        case TEXTURE:     return "Texture";
        case WAV_OBJ:     return "Wavefront Obj";
        case FBX_ASCII:   return "FBX ASCII";
        case FBX_BINARY:  return "FBX Binary";
        case PNG:         return "PNG";
        case JPEG:        return "JPEG";
        case JPEG2000:    return "JPEG 2000";
        case TIFF_LE:     return "TIFF Little Endian";
        case TIFF_BE:     return "TIFF Big Endian";
        case BMP:         return "BMP";
        case GIF:         return "GIF";
        case WAV_AUDIO:   return "WAVE Audio";
        case MP3:         return "MP3";
        case MP4:         return "MP4";
        case M4V:         return "M4V";
        case MOV:         return "MOV";
#ifdef SPECIAL_PDF
        case PDF:         return "Porn Data File";
#else
        case PDF:         return "PDF";
#endif   
        case HTML:        return "HTML";
        case BINARY_HTML: return "Binary HTML";
        case XML:         return "XML";
        case BINARY_XML:  return "Binary XML";
        case YAML:        return "YAML";
        case BINARY_YAML: return "Binary YAML";
        case JSON:        return "JSON";
        case BSON:        return "Binary JSON";
        case JAVASCIRPT:  return "JavaScript";
        case LUA:         return "Lua Script";
        case PYTHON2:     return "Python 2.7 Script";
        case PYTHON3:     return "Python 3.7 Script";
        default:          return "Unknown";
    }
}

typedef struct PACKED
{
    u32 magic;
    u16 fileCount;
    DPCompression compressionType;
} DPHeader;

typedef struct PACKED
{
    u32 fileSize;
    DPFileType fileType;
    u8 sha1[20];
    u8 md5[16];
    u64 ptrToFile;
} DPFileHeader;

typedef struct PACKED
{
    u32 byteCount;
    u8 sha1[20];
    u8 md5[16];
    u8* bytes; // u8 bytes[byteCount];
} DPCompressedData;

typedef u8* fileData_t;

typedef struct PACKED
{
    DPHeader header;
    const char** fileNames; // string fileNames[fileCount];
    DPFileHeader* dictionary;
    fileData_t* files; // fileData_t files[fileCount];
} DataPack;

#ifdef _WIN32
  #pragma pack(pop)
#endif
