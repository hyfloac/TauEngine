#pragma pack(1)

enum DPCompression : u8
{
    NONE = 0,
    DEFLATE,
    LZMA,
    LZMA2
};

struct DPHeader
{
    u32 magic;
    u32 fileCount;
    DPCompression compressionType;
};

enum DPFileType : u16;

struct DPFileHeader
{
    u32 fileSize;
    const char* fileName;
    DPFileType fileType;
    u8 sha1[20];
    u8 md5[16];
};

struct DPContents
{
    struct DPFileHeader header;
    u8* bytes; // u8 bytes[header.fileSize];
};

struct DPDecompressedData
{
    struct DPContents* files; // DPContents files[header.fileCount]
}

struct DPCompressedData
{
    u32 byteCount;
    u8 sha1[20];
    u8 md5[16];
    u8* bytes; // u8 bytes[byteCount];
}

struct DataPack
{
    struct DPHeader header;
    struct DPCompressedData data;
};

