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

enum class ResourceAlignment
{
    Default = 0,
    MSAA
};

enum class ResourceHeapDataType
{
    Buffer       = 1 << 0,
    Texture      = 1 << 1,
    RenderTarget = 1 << 2
};

#define RHDT_Buffer EGraphics::ResourceHeapDataType::Buffer
#define RHDT_Texture EGraphics::ResourceHeapDataType::Texture
#define RHDT_RenderTarget EGraphics::ResourceHeapDataType::RenderTarget

enum class ResourceHeapUsageType
{
    Default = 0,
    Upload,
    Read
};

enum class CommandListType
{
    Graphics = 0,
    Compute,
    Upload
};

}
