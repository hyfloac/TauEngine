#pragma once

#ifdef _WIN32
#include <d3d10.h>

#include "graphics/GraphicsEnums.hpp"

namespace DX10Utils {

inline constexpr D3D10_PRIMITIVE_TOPOLOGY getDXDrawType(const EGraphics::DrawType drawType) noexcept
{
    switch(drawType)
    {
        case EGraphics::DrawType::Points:                 return D3D10_PRIMITIVE_TOPOLOGY_POINTLIST;
        case EGraphics::DrawType::Lines:                  return D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
        case EGraphics::DrawType::LineStrip:              return D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP;
        case EGraphics::DrawType::LinesAdjacency:         return D3D10_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
        case EGraphics::DrawType::LineStripAdjacency:     return D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
        case EGraphics::DrawType::Triangles:              return D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        case EGraphics::DrawType::TriangleStrip:          return D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
        case EGraphics::DrawType::TrianglesAdjacency:     return D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
        case EGraphics::DrawType::TriangleStripAdjacency: return D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
        case EGraphics::DrawType::Patches:
        default:                                          return D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED;
    }
}

}

#endif
