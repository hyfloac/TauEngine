#pragma once

namespace EResource {

enum class Type
{
    Buffer = 1,
    Texture1D,
    Texture2D,
    Texture3D
};

enum class MapType
{
    Default = 1,
    Discard,
    NoOverwrite,
    NoWrite
};

enum class UsageType
{
    Default = 1,
    Immutable,
    Dynamic,
    Readable
};

}
