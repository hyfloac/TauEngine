#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include "graphics/GraphicsEnums.hpp"
#include "graphics/BufferEnums.hpp"

namespace GLUtils {

inline constexpr GLenum glDrawType(const EGraphics::DrawType drawType) noexcept
{
    switch(drawType)
    {
        case EGraphics::DrawType::Points:                 return GL_POINTS;
        case EGraphics::DrawType::Lines:                  return GL_LINES;
        case EGraphics::DrawType::LineStrip:              return GL_LINE_STRIP;
        case EGraphics::DrawType::LinesAdjacency:         return GL_LINES_ADJACENCY;
        case EGraphics::DrawType::LineStripAdjacency:     return GL_LINE_STRIP;
        case EGraphics::DrawType::Triangles:              return GL_TRIANGLES;
        case EGraphics::DrawType::TriangleStrip:          return GL_TRIANGLE_STRIP;
        case EGraphics::DrawType::TrianglesAdjacency:     return GL_TRIANGLES_ADJACENCY;
        case EGraphics::DrawType::TriangleStripAdjacency: return GL_TRIANGLE_STRIP_ADJACENCY;
        case EGraphics::DrawType::Patches:                return GL_PATCHES;
        default:                                          return static_cast<GLenum>(-1);
    }
}

inline constexpr GLenum glUsageType(const EBuffer::UsageType usage) noexcept
{
    switch(usage)
    {
        case EBuffer::UsageType::StreamDraw:  return GL_STREAM_DRAW;
        case EBuffer::UsageType::StreamRead:  return GL_STREAM_READ;
        case EBuffer::UsageType::StreamCopy:  return GL_STREAM_COPY;
        case EBuffer::UsageType::StaticDraw:  return GL_STATIC_DRAW;
        case EBuffer::UsageType::StaticRead:  return GL_STATIC_READ;
        case EBuffer::UsageType::StaticCopy:  return GL_STATIC_COPY;
        case EBuffer::UsageType::DynamicDraw: return GL_DYNAMIC_DRAW;
        case EBuffer::UsageType::DynamicRead: return GL_DYNAMIC_READ;
        case EBuffer::UsageType::DynamicCopy: return GL_DYNAMIC_COPY;
        default:                              return static_cast<GLenum>(-1);
    }
}

inline constexpr EBuffer::UsageType glUsageType(const GLenum usage) noexcept
{
    switch(usage)
    {
        case  GL_STREAM_DRAW: return EBuffer::UsageType::StreamDraw;
        case  GL_STREAM_READ: return EBuffer::UsageType::StreamRead;
        case  GL_STREAM_COPY: return EBuffer::UsageType::StreamCopy;
        case  GL_STATIC_DRAW: return EBuffer::UsageType::StaticDraw;
        case  GL_STATIC_READ: return EBuffer::UsageType::StaticRead;
        case  GL_STATIC_COPY: return EBuffer::UsageType::StaticCopy;
        case GL_DYNAMIC_DRAW: return EBuffer::UsageType::DynamicDraw;
        case GL_DYNAMIC_READ: return EBuffer::UsageType::DynamicRead;
        case GL_DYNAMIC_COPY: return EBuffer::UsageType::DynamicCopy;
        default:              return static_cast<EBuffer::UsageType>(-1);
    }
}

inline constexpr GLenum glIndexSize(const EBuffer::IndexSize indexSize) noexcept
{
    switch(indexSize)
    {
        case EBuffer::IndexSize::Uint32: return GL_UNSIGNED_INT;
        case EBuffer::IndexSize::Uint16: return GL_UNSIGNED_SHORT;
        default:                         return static_cast<GLenum>(-1);
    }
}

}
