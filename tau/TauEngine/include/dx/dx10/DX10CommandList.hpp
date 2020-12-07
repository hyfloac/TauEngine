#pragma once

#include "graphics/CommandList.hpp"

#ifdef _WIN32
#include <d3d10.h>
#include <ArrayList.hpp>
#include <allocator/FreeListAllocator.hpp>

class DX10VertexArray;
class DX10CommandAllocator;

namespace DX10CL {
enum class CommandType
{
    Draw = 1,
    DrawIndexed,
    DrawInstanced,
    DrawIndexedInstanced,
    SetDrawType,
    SetPipelineState,
    SetRenderTargets,
    ClearRenderTarget,
    ClearDepthStencil,
    SetBlendFactor,
    SetStencilRef,
    SetVertexArray,
    SetIndexBuffer,
    SetGDescriptorLayout,
    SetGDescriptorTable,
    CopyResource,
    CopySubresourceRegion0,
    CopySubresourceRegion1,
    CopySubresourceRegion2
};

struct CommandDraw final
{
    DEFAULT_CONSTRUCT_PU(CommandDraw);
    DEFAULT_DESTRUCT(CommandDraw);
    DEFAULT_CM_PU(CommandDraw);
public:
    UINT vertexCount;
    UINT startVertex;
public:
    CommandDraw(const UINT _vertexCount, const UINT _startVertex) noexcept
        : vertexCount(_vertexCount)
        , startVertex(_startVertex)
    { }
};

struct CommandDrawIndexed final
{
    DEFAULT_CONSTRUCT_PU(CommandDrawIndexed);
    DEFAULT_DESTRUCT(CommandDrawIndexed);
    DEFAULT_CM_PU(CommandDrawIndexed);
public:
    UINT indexCount;
    UINT startIndex;
    INT baseVertex;
public:
    CommandDrawIndexed(const UINT _indexCount, const UINT _startIndex, const INT _baseVertex) noexcept
        : indexCount(_indexCount)
        , startIndex(_startIndex)
        , baseVertex(_baseVertex)
    { }
};

struct CommandDrawInstanced final
{
    DEFAULT_CONSTRUCT_PU(CommandDrawInstanced);
    DEFAULT_DESTRUCT(CommandDrawInstanced);
    DEFAULT_CM_PU(CommandDrawInstanced);
public:
    UINT vertexCount;
    UINT instanceCount;
    UINT startVertex;
    UINT startInstance;
public:
    CommandDrawInstanced(const UINT _vertexCount, const UINT _instanceCount, const UINT _startVertex, const UINT _startInstance) noexcept
        : vertexCount(_vertexCount)
        , instanceCount(_instanceCount)
        , startVertex(_startVertex)
        , startInstance(_startInstance)
    { }
};

struct CommandDrawIndexedInstanced final
{
    DEFAULT_CONSTRUCT_PU(CommandDrawIndexedInstanced);
    DEFAULT_DESTRUCT(CommandDrawIndexedInstanced);
    DEFAULT_CM_PU(CommandDrawIndexedInstanced);
public:
    UINT indexCount;
    UINT instanceCount;
    UINT startIndex;
    INT baseVertex;
    UINT startInstance;
public:
    CommandDrawIndexedInstanced(const UINT _indexCount, const UINT _instanceCount, const UINT _startIndex, const INT _baseVertex, const UINT _startInstance) noexcept
        : indexCount(_indexCount)
        , instanceCount(_instanceCount)
        , startIndex(_startIndex)
        , baseVertex(_baseVertex)
        , startInstance(_startInstance)
    { }
};

struct CommandSetDrawType final
{
    DEFAULT_CONSTRUCT_PU(CommandSetDrawType);
    DEFAULT_DESTRUCT(CommandSetDrawType);
    DEFAULT_CM_PU(CommandSetDrawType);
public:
    D3D10_PRIMITIVE_TOPOLOGY drawType;
public:
    CommandSetDrawType(const D3D10_PRIMITIVE_TOPOLOGY _drawType) noexcept
        : drawType(_drawType)
    { }
};

struct CommandSetPipelineState final
{
    DEFAULT_CONSTRUCT_PU(CommandSetPipelineState);
    DEFAULT_DESTRUCT(CommandSetPipelineState);
    DEFAULT_CM_PU(CommandSetPipelineState);
public:
    const PipelineState* pipelineState;
public:
    CommandSetPipelineState(const PipelineState* const _pipelineState) noexcept
        : pipelineState(_pipelineState)
    { }
};

struct CommandSetRenderTargets final
{
    DEFAULT_CONSTRUCT_PU(CommandSetRenderTargets);
    DEFAULT_DESTRUCT(CommandSetRenderTargets);
    DEFAULT_CM_PU(CommandSetRenderTargets);
public:
    UINT numRenderTargets;
    ID3D10RenderTargetView* const * renderTargets;
    ID3D10DepthStencilView*  depthStencil;
public:
    CommandSetRenderTargets(const UINT _numRenderTargets, ID3D10RenderTargetView* const * const _renderTargets, ID3D10DepthStencilView* const _depthStencil) noexcept
        : numRenderTargets(_numRenderTargets)
        , renderTargets(_renderTargets)
        , depthStencil(_depthStencil)
    { }
};

struct CommandClearRenderTarget final
{
    DEFAULT_DESTRUCT(CommandClearRenderTarget);
    DEFAULT_CM_PU(CommandClearRenderTarget);
public:
    ID3D10RenderTargetView* renderTarget;
    const float color[4];
public:
    CommandClearRenderTarget() noexcept
        : renderTarget(nullptr)
        , color{ 0.0f, 0.0f, 0.0f, 0.0f }
    { }

    CommandClearRenderTarget(ID3D10RenderTargetView* const _renderTarget, const float _color[4]) noexcept
        : renderTarget(_renderTarget)
        , color{ _color[0], _color[1], _color[2], _color[3] }
    { }
};

struct CommandClearDepthStencil final
{
    DEFAULT_CONSTRUCT_PU(CommandClearDepthStencil);
    DEFAULT_DESTRUCT(CommandClearDepthStencil);
    DEFAULT_CM_PU(CommandClearDepthStencil);
public:
    ID3D10DepthStencilView* depthStencil;
    UINT clearFlags;
    float depth;
    u8 stencil;
public:
    CommandClearDepthStencil(ID3D10DepthStencilView* const _depthStencil, const UINT _clearFlags, const float _depth, const u8 _stencil) noexcept
        : depthStencil(_depthStencil)
        , clearFlags(_clearFlags)
        , depth(_depth)
        , stencil(_stencil)
    { }
};

struct CommandSetBlendFactor final
{
    DEFAULT_DESTRUCT(CommandSetBlendFactor);
    DEFAULT_CM_PU(CommandSetBlendFactor);
public:
    const float blendFactor[4];
public:
    CommandSetBlendFactor() noexcept
        : blendFactor { 1.0f, 1.0f, 1.0f, 1.0f }
    { }

    CommandSetBlendFactor(const float _blendFactor[4]) noexcept
        : blendFactor{ _blendFactor[0], _blendFactor[1], _blendFactor[2], _blendFactor[3] }
    { }
};

struct CommandSetStencilRef final
{
    DEFAULT_CONSTRUCT_PU(CommandSetStencilRef);
    DEFAULT_DESTRUCT(CommandSetStencilRef);
    DEFAULT_CM_PU(CommandSetStencilRef);
public:
    uSys stencilRef;
public:
    CommandSetStencilRef(const uSys _stencilRef) noexcept
        : stencilRef(_stencilRef)
    { }
};

struct CommandSetVertexArray final
{
    DEFAULT_CONSTRUCT_PU(CommandSetVertexArray);
    DEFAULT_DESTRUCT(CommandSetVertexArray);
    DEFAULT_CM_PU(CommandSetVertexArray);
public:
    uSys startSlot;
    uSys bufferCount;
    ID3D10Buffer** buffers;
public:
    CommandSetVertexArray(const uSys _startSlot, const uSys _bufferCount, ID3D10Buffer** const _buffers) noexcept
        : startSlot(_startSlot)
        , bufferCount(_bufferCount)
        , buffers(_buffers)
    { }
};

struct CommandSetIndexBuffer final
{
    DEFAULT_CONSTRUCT_PU(CommandSetIndexBuffer);
    DEFAULT_DESTRUCT(CommandSetIndexBuffer);
    DEFAULT_CM_PU(CommandSetIndexBuffer);
public:
    ID3D10Buffer* buffer;
    DXGI_FORMAT format;
public:
    CommandSetIndexBuffer(ID3D10Buffer* const _buffer, const DXGI_FORMAT _format) noexcept
        : buffer(_buffer)
        , format(_format)
    { }
};

struct CommandSetGDescriptorLayout final
{
    DEFAULT_CONSTRUCT_PU(CommandSetGDescriptorLayout);
    DEFAULT_DESTRUCT(CommandSetGDescriptorLayout);
    DEFAULT_CM_PU(CommandSetGDescriptorLayout);
public:
    DescriptorLayout layout;
public:
    CommandSetGDescriptorLayout(const DescriptorLayout _layout) noexcept
        : layout(_layout)
    { }
};

struct CommandSetGDescriptorTable final
{
    DEFAULT_CONSTRUCT_PU(CommandSetGDescriptorTable);
    DEFAULT_DESTRUCT(CommandSetGDescriptorTable);
    DEFAULT_CM_PU(CommandSetGDescriptorTable);
public:
    uSys index;
    EGraphics::DescriptorType type;
    uSys descriptorCount;
    GPUDescriptorHandle handle;
public:
    CommandSetGDescriptorTable(const uSys _index, const EGraphics::DescriptorType _type, const uSys _descriptorCount, const GPUDescriptorHandle _handle) noexcept
        : index(_index)
        , type(_type)
        , descriptorCount(_descriptorCount)
        , handle(_handle)
    { }
};

struct CommandCopyResource final
{
    DEFAULT_CONSTRUCT_PU(CommandCopyResource);
    DEFAULT_DESTRUCT(CommandCopyResource);
    DEFAULT_CM_PU(CommandCopyResource);
public:
    ID3D10Resource* dst;
    ID3D10Resource* src;
public:
    CommandCopyResource(ID3D10Resource* const _dst, ID3D10Resource* const _src) noexcept
        : dst(_dst)
        , src(_src)
    { }
};

/**
 *   This is split into 3 different commands to reduce the
 * overall size of all commands, because unions. Ultimately
 * a small price to pay to get a 65% reduction in command
 * size.
 */
struct CommandCopySubresourceRegion0 final
{
    DEFAULT_CONSTRUCT_PU(CommandCopySubresourceRegion0);
    DEFAULT_DESTRUCT(CommandCopySubresourceRegion0);
    DEFAULT_CM_PU(CommandCopySubresourceRegion0);
public:
    ID3D10Resource* dst;
    UINT dstSubresource;
    ID3D10Resource* src;
    UINT srcSubresource;
public:
    CommandCopySubresourceRegion0(ID3D10Resource* const _dst, const UINT _dstSubresource, ID3D10Resource* const _src, const UINT _srcSubresource) noexcept
        : dst(_dst)
        , dstSubresource(_dstSubresource)
        , src(_src)
        , srcSubresource(_srcSubresource)
    { }
};
    
struct CommandCopySubresourceRegion1 final
{
    DEFAULT_CONSTRUCT_PU(CommandCopySubresourceRegion1);
    DEFAULT_DESTRUCT(CommandCopySubresourceRegion1);
    DEFAULT_CM_PU(CommandCopySubresourceRegion1);
public:
    ETexture::Coord coord;
    bool hasSrcBox;
public:
    CommandCopySubresourceRegion1(const ETexture::Coord& _coord, const ETexture::EBox* const _srcBox) noexcept
        : coord(_coord)
        , hasSrcBox(_srcBox)
    { }
};
    
struct CommandCopySubresourceRegion2 final
{
    DEFAULT_CONSTRUCT_PU(CommandCopySubresourceRegion2);
    DEFAULT_DESTRUCT(CommandCopySubresourceRegion2);
    DEFAULT_CM_PU(CommandCopySubresourceRegion2);
public:
    ETexture::EBox srcBox;
public:
    CommandCopySubresourceRegion2(const ETexture::EBox* const _srcBox) noexcept
    {
        if(_srcBox)
        { srcBox = *_srcBox; }
    }
};

struct Command final
{
    DEFAULT_DESTRUCT(Command);
    DEFAULT_CM_PU(Command);
public:
    CommandType type;
    union
    {
        uSys _;
        CommandDraw draw;
        CommandDrawIndexed drawIndexed;
        CommandDrawInstanced drawInstanced;
        CommandDrawIndexedInstanced drawIndexedInstanced;
        CommandSetDrawType setDrawType;
        CommandSetPipelineState setPipelineState;
        CommandSetRenderTargets setRenderTargets;
        CommandClearRenderTarget clearRenderTarget;
        CommandClearDepthStencil clearDepthStencil;
        CommandSetBlendFactor setBlendFactor;
        CommandSetStencilRef setStencilRef;
        CommandSetVertexArray setVertexArray;
        CommandSetIndexBuffer setIndexBuffer;
        CommandSetGDescriptorLayout setGDescriptorLayout;
        CommandSetGDescriptorTable setGDescriptorTable;
        CommandCopyResource copyResource;
        CommandCopySubresourceRegion0 copySubresourceRegion0;
        CommandCopySubresourceRegion1 copySubresourceRegion1;
        CommandCopySubresourceRegion2 copySubresourceRegion2;
    };
public:
    Command() noexcept
        : type(static_cast<CommandType>(0))
        , _(0)
    { }

    Command(const CommandDraw& _draw) noexcept
        : type(CommandType::Draw)
        , draw(_draw)
    { }

    Command(const CommandDrawIndexed& _drawIndexed) noexcept
        : type(CommandType::DrawIndexed)
        , drawIndexed(_drawIndexed)
    { }

    Command(const CommandDrawInstanced& _drawInstanced) noexcept
        : type(CommandType::DrawInstanced)
        , drawInstanced(_drawInstanced)
    { }

    Command(const CommandDrawIndexedInstanced& _drawIndexedInstanced) noexcept
        : type(CommandType::DrawIndexedInstanced)
        , drawIndexedInstanced(_drawIndexedInstanced)
    { }

    Command(const CommandSetDrawType& _setDrawType) noexcept
        : type(CommandType::SetDrawType)
        , setDrawType(_setDrawType)
    { }

    Command(const CommandSetPipelineState& _setPipelineState) noexcept
        : type(CommandType::SetPipelineState)
        , setPipelineState(_setPipelineState)
    { }

    Command(const CommandSetRenderTargets& _setRenderTargets) noexcept
        : type(CommandType::SetRenderTargets)
        , setRenderTargets(_setRenderTargets)
    { }

    Command(const CommandClearRenderTarget& _clearRenderTarget) noexcept
        : type(CommandType::ClearRenderTarget)
        , clearRenderTarget(_clearRenderTarget)
    { }

    Command(const CommandClearDepthStencil& _clearDepthStencil) noexcept
        : type(CommandType::ClearDepthStencil)
        , clearDepthStencil(_clearDepthStencil)
    { }

    Command(const CommandSetBlendFactor& _setBlendFactor) noexcept
        : type(CommandType::SetBlendFactor)
        , setBlendFactor(_setBlendFactor)
    { }

    Command(const CommandSetStencilRef& _setStencilRef) noexcept
        : type(CommandType::SetStencilRef)
        , setStencilRef(_setStencilRef)
    { }

    Command(const CommandSetVertexArray& _setVertexArray) noexcept
        : type(CommandType::SetVertexArray)
        , setVertexArray(_setVertexArray)
    { }

    Command(const CommandSetIndexBuffer& _setIndexBuffer) noexcept
        : type(CommandType::SetIndexBuffer)
        , setIndexBuffer(_setIndexBuffer)
    { }

    Command(const CommandSetGDescriptorLayout& _setGDescriptorLayout) noexcept
        : type(CommandType::SetGDescriptorLayout)
        , setGDescriptorLayout(_setGDescriptorLayout)
    { }

    Command(const CommandSetGDescriptorTable& _setGDescriptorTable) noexcept
        : type(CommandType::SetGDescriptorTable)
        , setGDescriptorTable(_setGDescriptorTable)
    { }

    Command(const CommandCopyResource& _copyResource) noexcept
        : type(CommandType::CopyResource)
        , copyResource(_copyResource)
    { }

    Command(const CommandCopySubresourceRegion0& _copySubresourceRegion0) noexcept
        : type(CommandType::CopySubresourceRegion0)
        , copySubresourceRegion0(_copySubresourceRegion0)
    { }

    Command(const CommandCopySubresourceRegion1& _copySubresourceRegion1) noexcept
        : type(CommandType::CopySubresourceRegion1)
        , copySubresourceRegion1(_copySubresourceRegion1)
    { }

    Command(const CommandCopySubresourceRegion2& _copySubresourceRegion2) noexcept
        : type(CommandType::CopySubresourceRegion2)
        , copySubresourceRegion2(_copySubresourceRegion2)
    { }
};
}

class TAU_DLL DX10CommandList final : public ICommandList
{
    DEFAULT_DESTRUCT(DX10CommandList);
    DELETE_CM(DX10CommandList);
    COMMAND_LIST_IMPL(DX10CommandList);
private:
    NullableRef<DX10CommandAllocator> _commandAllocator;
    const void* _head;
    uSys _commandCount;
public:
    DX10CommandList(const NullableRef<DX10CommandAllocator>& allocator) noexcept;

    [[nodiscard]] const void* head() const noexcept { return _head; }
    [[nodiscard]] uSys commandCount() const noexcept { return _commandCount; }

    void reset(const NullableRef<ICommandAllocator>& allocator, const PipelineState* initialState) noexcept override;
    void begin() noexcept override;
    void finish() noexcept override;
    void draw(uSys exCount, uSys startVertex) noexcept override;
    void drawIndexed(uSys exCount, uSys startIndex, iSys baseVertex) noexcept override;
    void drawInstanced(uSys exCount, uSys startVertex, uSys instanceCount, uSys startInstance) noexcept override;
    void drawIndexedInstanced(uSys exCount, uSys startIndex, iSys baseVertex, uSys instanceCount, uSys startInstance) noexcept override;
    void setDrawType(EGraphics::DrawType drawType) noexcept override;
    void setPipelineState(const PipelineState& pipelineState) noexcept override;
    void setFrameBuffer(const NullableRef<IFrameBuffer>& frameBuffer) noexcept override;
    void clearRenderTargetView(const NullableRef<IFrameBuffer>& frameBuffer, uSys renderTargetIndex, const float color[4], uSys rectCount, const ETexture::ERect* rects) noexcept override;
    void clearDepthStencilView(const NullableRef<IFrameBuffer>& frameBuffer, bool clearDepth, bool clearStencil, float depth, u8 stencil, uSys rectCount, const ETexture::ERect* rects) noexcept override;
    void setBlendFactor(const float blendFactor[4]) noexcept override;
    void setStencilRef(uSys stencilRef) noexcept override;
    void setVertexArray(const NullableRef<IVertexArray>& va) noexcept override;
    void setIndexBuffer(const IndexBufferView& indexBufferView) noexcept override;
    void setGraphicsDescriptorLayout(DescriptorLayout layout) noexcept override;
    void setGraphicsDescriptorTable(uSys index, EGraphics::DescriptorType type, uSys descriptorCount, GPUDescriptorHandle handle) noexcept override;
    void executeBundle(const NullableRef<ICommandList>& bundle) noexcept override;
    void copyResource(const NullableRef<IResource>& dst, const NullableRef<IResource>& src) noexcept override;
    void copyBuffer(const NullableRef<IResource>& dstBuffer, u64 dstOffset, const NullableRef<IResource>& srcBuffer, u64 srcOffset, u64 byteCount) noexcept override;
    void copyTexture(const NullableRef<IResource>& dstTexture, u32 dstSubResource, const NullableRef<IResource>& srcTexture, u32 srcSubResource) noexcept override;
    void copyTexture(const NullableRef<IResource>& dstTexture, u32 dstSubResource, const ETexture::Coord& coord, const NullableRef<IResource>& srcTexture, u32 srcSubResource, const ETexture::EBox* srcBox) noexcept override;
private:
    friend class DX10CommandQueue;
};

#endif
