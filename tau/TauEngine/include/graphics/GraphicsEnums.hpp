#pragma once

namespace EGraphics {

enum class DrawType
{
    Points = 1,
    Lines,
    LineStrip,
    LinesAdjacency,
    LineStripAdjacency,
    Triangles,
    TriangleStrip,
    TrianglesAdjacency,
    TriangleStripAdjacency,
    Patches
};

enum class DescriptorType
{
    TextureView = 1,
    RenderTargetView,
    DepthStencilView,
    UniformBufferView,
    UnorderedAccessView,
    Sampler
};

enum class DescriptorHeapFlags
{
    None = 0,
    ShaderAccess = 1 << 0
};

#define DHF_None EGraphics::DescriptorHeapFlags::None
#define DHF_ShaderAccess EGraphics::DescriptorHeapFlags::ShaderAccess

enum class ShaderAccess
{
    All = 0,
    Vertex,
    TessCtrl,
    TessEval,
    Geometry,
    Pixel
};

}
