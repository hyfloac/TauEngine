#include "TauModelPart.hpp"

#define MAKE_VERSION(_MAJOR, _MINOR) (((_MAJOR) << 8) | (_MINOR))

static constexpr u32 TAU_MODEL_MAGIC = 0x5461756D; // TauM
static constexpr u16 TAU_MODEL_VERSION = MAKE_VERSION(0, 1);

#pragma pack(push, 1)
struct TauModelDebugHeader final
{
    const char* modelName;

};

struct TauModelHeader final
{
    u32 magic;
    u16 version;
    bool hasDebugData;
    uSys modelPartCount;
};

struct TauModelPartDebugHeader final
{
    const char* modelPartName;
    bool hasBiTangents;
};

struct TauModelPartHeader final
{
    uSys vertexCount;
    uSys indiceCount;
    bool hasTangents;
};
#pragma pack(pop)

DynArray<TauModelPart> TauModelPart::parse(const CPPRef<IFile>& file) noexcept
{
    const uSys fileSize = file->size();
    if(fileSize < sizeof(TauModelHeader))
    { return DynArray<TauModelPart>(0); }

    uSys index = 0;

    TauModelHeader modelHeader {};
    file->readType(&modelHeader);
    index += sizeof(modelHeader);

    if(modelHeader.magic != TAU_MODEL_MAGIC)
    { return DynArray<TauModelPart>(0); }

    if(modelHeader.version > TAU_MODEL_VERSION)
    { return DynArray<TauModelPart>(0); }
    else if(modelHeader.version < TAU_MODEL_VERSION)
    { return DynArray<TauModelPart>(0); }

    TauModelDebugHeader debugModelHeader {};

    if(modelHeader.hasDebugData)
    {
        if(index + sizeof(TauModelDebugHeader) > fileSize)
        { return DynArray<TauModelPart>(0); }

        file->readType(&debugModelHeader);
        index += sizeof(debugModelHeader);
    }

    DynArray<TauModelPart> modelParts(modelHeader.modelPartCount);




    return DynArray<TauModelPart>(0);
}
