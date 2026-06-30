#pragma once

#include <NumTypes.hpp>

namespace EGraphics {

enum class DrawType : u8
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

enum class DescriptorType : u8
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

enum class ShaderAccess : u8
{
    All = 0,
    Vertex,
    TessCtrl,
    TessEval,
    Geometry,
    Pixel
};

enum class ResourceAlignment : u8
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

enum class ResourceHeapUsageType : u8
{
    Default = 0,
    Upload,
    Read
};

enum class CommandListType : u8
{
    Graphics = 0,
    Secondary,
    Compute,
    Copy,
    VideoDecode,
    VideoProcess
};

}
