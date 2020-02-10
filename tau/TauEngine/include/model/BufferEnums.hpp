#pragma once

#include <NumTypes.hpp>

namespace EBuffer {

enum class UsageType : u8
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

enum class Type : u8
{
    ArrayBuffer = 1,
    AtomicCounterBuffer,
    CopyReadBuffer,
    CopyWriteBuffer,
    DispatchIndirectBuffer,
    DrawIndirectBuffer,
    ElementArrayBuffer,
    IndexBuffer = ElementArrayBuffer,
    PixelPackBuffer,
    PixelUnpackBuffer,
    QueryBuffer,
    ShaderStorageBuffer,
    TextureBuffer,
    TransformFeedbackBuffer,
    UniformBuffer
};

enum class MemoryStorage : u8
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

}
