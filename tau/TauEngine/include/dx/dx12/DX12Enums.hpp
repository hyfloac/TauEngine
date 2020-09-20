#pragma once

#ifdef _WIN32
#include <d3d12.h>

#include "graphics/GraphicsEnums.hpp"
#include "graphics/ResourceEnums.hpp"
#include "graphics/BufferEnums.hpp"

namespace DX12Utils {

inline constexpr D3D_PRIMITIVE_TOPOLOGY getDXDrawType(const EGraphics::DrawType drawType) noexcept
{
    switch(drawType)
    {
        case EGraphics::DrawType::Points:                 return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
        case EGraphics::DrawType::Lines:                  return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
        case EGraphics::DrawType::LineStrip:              return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
        case EGraphics::DrawType::LinesAdjacency:         return D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
        case EGraphics::DrawType::LineStripAdjacency:     return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
        case EGraphics::DrawType::Triangles:              return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        case EGraphics::DrawType::TriangleStrip:          return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
        case EGraphics::DrawType::TrianglesAdjacency:     return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
        case EGraphics::DrawType::TriangleStripAdjacency: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
        case EGraphics::DrawType::Patches:                return D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
        default:                                          return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
    }
}

inline constexpr DXGI_FORMAT getDXIndexSize(const EBuffer::IndexSize indexSize)
{
    switch(indexSize)
    {
        case EBuffer::IndexSize::Uint16: return DXGI_FORMAT_R16_UINT;
        case EBuffer::IndexSize::Uint32: return DXGI_FORMAT_R32_UINT;
        default:                         return DXGI_FORMAT_UNKNOWN;
    }
}

}
#endif
