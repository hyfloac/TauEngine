#pragma once
// ReSharper disable CppNotAllPathsReturnValue

#include <NumTypes.hpp>

namespace EBuffer {

enum class UsageType
{
    StreamDraw = 1,
    StreamRead,
    StreamCopy,
    StaticDraw,
    StaticRead,
    StaticCopy,
    DynamicDraw,
    DynamicRead,
    DynamicCopy
};

enum class IndexSize
{
    Uint32 = 0,
    Uint16
};

enum class MemoryStorage
{
    Default = 1,
    GPU,
    Shared,
    Managed
};

enum class ContentHandling
{
    /**
     * The old contents are discarded.
     */
    Discard = 1,
    NoOverwrite
};

static inline uSys indexSize(const IndexSize size) noexcept
{
    switch(size)
    {
        case IndexSize::Uint32: return 4;
        case IndexSize::Uint16: return 2;
    }
}

}
