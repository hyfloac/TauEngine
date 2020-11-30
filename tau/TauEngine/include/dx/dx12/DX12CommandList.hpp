#pragma once

#include "graphics/CommandList.hpp"

#ifdef _WIN32
#include <d3d12.h>
#include <atomic>
#include "system/Win32Event.hpp"
#include <ArrayList.hpp>
#include "DX12ResourceStateManager.hpp"

namespace DX12CL {
enum class CommandType
{
    DrawInstanced = 1,
    DrawIndexedInstanced,
    SetDrawType,
    SetPipelineState,
    SetStencilRef,
    SetVertexArray,
    SetIndexBuffer,
    SetGDescriptorLayout,
    SetGDescriptorTable,
    CopyResource,
    CopyBuffer,
    CopyTexture,
    CopyTextureBox,
    ResourceUsage
};

enum class ResourceUsagePhase
{
    PreUsageFirst = 0,
    PreUsage,
    PostUsage,
    PostUsageFinal
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
    D3D12_PRIMITIVE_TOPOLOGY drawType;
public:
    CommandSetDrawType(const D3D12_PRIMITIVE_TOPOLOGY _drawType) noexcept
        : drawType(_drawType)
    { }
};

struct CommandSetPipelineState final
{
    DEFAULT_CONSTRUCT_PU(CommandSetPipelineState);
    DEFAULT_DESTRUCT(CommandSetPipelineState);
    DEFAULT_CM_PU(CommandSetPipelineState);
public:
    ID3D12PipelineState* pipelineState;
public:
    CommandSetPipelineState(ID3D12PipelineState* const _pipelineState) noexcept
        : pipelineState(_pipelineState)
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
    const D3D12_VERTEX_BUFFER_VIEW* buffers;
public:
    CommandSetVertexArray(const uSys _startSlot, const uSys _bufferCount, const D3D12_VERTEX_BUFFER_VIEW* const _buffers) noexcept
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
    D3D12_INDEX_BUFFER_VIEW view;
public:
    CommandSetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW& _view) noexcept
        : view(_view)
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
    ID3D12Resource* dst;
    ID3D12Resource* src;
public:
    CommandCopyResource(ID3D12Resource* const _dst, ID3D12Resource* const _src) noexcept
        : dst(_dst)
        , src(_src)
    { }
};

struct CommandCopyBuffer final
{
    DEFAULT_CONSTRUCT_PU(CommandCopyBuffer);
    DEFAULT_DESTRUCT(CommandCopyBuffer);
    DEFAULT_CM_PU(CommandCopyBuffer);
public:
    ID3D12Resource* dst;
    u64 dstOffset;
    ID3D12Resource* src;
    u64 srcOffset;
    u64 byteCount;
public:
    CommandCopyBuffer(ID3D12Resource* const _dst, const u64 _dstOffset, ID3D12Resource* const _src, const u64 _srcOffset, const u64 _byteCount) noexcept
        : dst(_dst)
        , dstOffset(_dstOffset)
        , src(_src)
        , srcOffset(_srcOffset)
        , byteCount(_byteCount)
    { }
};

struct CommandCopyTexture final
{
    DEFAULT_CONSTRUCT_PU(CommandCopyTexture);
    DEFAULT_DESTRUCT(CommandCopyTexture);
    DEFAULT_CM_PU(CommandCopyTexture);
public:
    ID3D12Resource* dst;
    u64 dstSubresource;
    ID3D12Resource* src;
    u64 srcSubresource;
public:
    CommandCopyTexture(ID3D12Resource* const _dst, const u64 _dstSubresource, ID3D12Resource* const _src, const u64 _srcSubresource) noexcept
        : dst(_dst)
        , dstSubresource(_dstSubresource)
        , src(_src)
        , srcSubresource(_srcSubresource)
    { }
};

struct CommandCopyTextureBox final
{
    DEFAULT_CONSTRUCT_PU(CommandCopyTextureBox);
    DEFAULT_DESTRUCT(CommandCopyTextureBox);
    DEFAULT_CM_PU(CommandCopyTextureBox);
public:
    ID3D12Resource* dst;
    u64 dstSubresource;
    ETexture::Coord coord;
    ID3D12Resource* src;
    u64 srcSubresource;
    ETexture::EBox srcBoxData;
    ETexture::EBox* srcBox;
public:
    CommandCopyTextureBox(ID3D12Resource* const _dst, const u64 _dstSubresource, const ETexture::Coord& _coord, ID3D12Resource* const _src, const u64 _srcSubresource, const ETexture::EBox* const _srcBox) noexcept
        : dst(_dst)
        , dstSubresource(_dstSubresource)
        , coord(_coord)
        , src(_src)
        , srcSubresource(_srcSubresource)
    {
        if(_srcBox)
        {
            srcBoxData = *_srcBox;
            srcBox = &srcBoxData;
        }
        else
        {
            srcBox = nullptr;
        }
    }
};

struct CommandResourceUsage final
{
    DEFAULT_CONSTRUCT_PU(CommandResourceUsage);
    DEFAULT_DESTRUCT(CommandResourceUsage);
    DEFAULT_CM_PU(CommandResourceUsage);
public:
    ID3D12Resource* resource;
    D3D12_RESOURCE_STATES state;
    uSys subresource;
    ResourceUsagePhase usage;
    uSys nextUsageChange;
public:
    CommandResourceUsage(ID3D12Resource* const _resource, const D3D12_RESOURCE_STATES _state, const uSys _subresource, const ResourceUsagePhase _usage, const uSys _nextUsageChange = static_cast<uSys>(-1)) noexcept
        : resource(_resource)
        , state(_state)
        , subresource(_subresource)
        , usage(_usage)
        , nextUsageChange(_nextUsageChange)
    { }

    [[nodiscard]] bool isWriteBit() const noexcept
    {
        switch(state)
        {
            case D3D12_RESOURCE_STATE_RENDER_TARGET:
            case D3D12_RESOURCE_STATE_UNORDERED_ACCESS:
            case D3D12_RESOURCE_STATE_DEPTH_WRITE:
            case D3D12_RESOURCE_STATE_STREAM_OUT:
            case D3D12_RESOURCE_STATE_COPY_DEST: return true;
            default: return false;
        }
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
        CommandDrawInstanced drawInstanced;
        CommandDrawIndexedInstanced drawIndexedInstanced;
        CommandSetDrawType setDrawType;
        CommandSetPipelineState setPipelineState;
        CommandSetStencilRef setStencilRef;
        CommandSetVertexArray setVertexArray;
        CommandSetIndexBuffer setIndexBuffer;
        CommandSetGDescriptorLayout setGDescriptorLayout;
        CommandSetGDescriptorTable setGDescriptorTable;
        CommandCopyResource copyResource;
        CommandCopyBuffer copyBuffer;
        CommandCopyTexture copyTexture;
        CommandCopyTextureBox copyTextureBox;
        CommandResourceUsage resourceUsage;
    };
public:
    Command() noexcept
        : type(static_cast<CommandType>(0))
        , _(0)
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

    Command(const CommandCopyBuffer& _copyBuffer) noexcept
        : type(CommandType::CopyBuffer)
        , copyBuffer(_copyBuffer)
    { }

    Command(const CommandCopyTexture& _copyTexture) noexcept
        : type(CommandType::CopyTexture)
        , copyTexture(_copyTexture)
    { }

    Command(const CommandCopyTextureBox& _copyTextureBox) noexcept
        : type(CommandType::CopyTextureBox)
        , copyTextureBox(_copyTextureBox)
    { }

    Command(const CommandResourceUsage& _resourceUsage) noexcept
        : type(CommandType::ResourceUsage)
        , resourceUsage(_resourceUsage)
    { }
};
}

class DX12CommandAllocator;
class DX12CommandQueue;
class DX12VertexArray;

class TAU_DLL DX12CommandList final : public ICommandList
{
    DELETE_CM(DX12CommandList);
    COMMAND_LIST_IMPL(DX12CommandList);
private:
    ID3D12GraphicsCommandList* _cmdList;
    NullableRef<DX12CommandAllocator> _cmdAllocator;
    DX12ResourceStateManager _stateManager;
    uSys _maxCommands;
    ArrayList<DX12CL::Command> _commands;
    const DX12VertexArray* _currentVA;
    const DX12Resource* _currentIndexBuffer;
public:
    DX12CommandList(ID3D12GraphicsCommandList* const cmdList, const NullableRef<DX12CommandAllocator>& cmdAllocator, const uSys maxCommands) noexcept
        : _cmdList(cmdList)
        , _cmdAllocator(cmdAllocator)
        , _maxCommands(maxCommands * 2)
        , _commands(maxCommands * 2)
        , _currentVA(nullptr)
        , _currentIndexBuffer(nullptr)
    { }

    ~DX12CommandList() noexcept override
    { _cmdList->Release(); }

    [[nodiscard]] ID3D12CommandList* cmdList() const noexcept { return _cmdList; }

    void reset(const NullableRef<ICommandAllocator>& allocator, const PipelineState* initialState) noexcept override;
    void begin() noexcept override;
    void finish() noexcept override;
    void draw(uSys vertexCount, uSys startVertex) noexcept override;
    void drawIndexed(uSys indexCount, uSys startIndex, iSys baseVertex) noexcept override;
    void drawInstanced(uSys vertexCount, uSys startVertex, uSys instanceCount, uSys startInstance) noexcept override;
    void drawIndexedInstanced(uSys indexCount, uSys startIndex, iSys baseVertex, uSys instanceCount, uSys startInstance) noexcept override;
    void setDrawType(EGraphics::DrawType drawType) noexcept override;
    void setPipelineState(const PipelineState& pipelineState) noexcept override;
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
    void trackVertexBuffersPre() noexcept;
    void trackVertexBuffersPost() noexcept;
    void trackIndexBufferPre() noexcept;
    void trackIndexBufferPost() noexcept;

    void trackResourcePreWrite(ID3D12Resource* resource, uSys subresource, D3D12_RESOURCE_STATES state) noexcept;
    void trackResourcePostWrite(ID3D12Resource* resource, uSys subresource, D3D12_RESOURCE_STATES state) noexcept;
    void trackResourcePreRead(ID3D12Resource* resource, uSys subresource, D3D12_RESOURCE_STATES state) noexcept;
    void trackResourcePostRead(ID3D12Resource* resource, uSys subresource, D3D12_RESOURCE_STATES state) noexcept;

    void populateCommandList();

    void _drawInstanced(const DX12CL::CommandDrawInstanced& cmd) noexcept;
    void _drawIndexedInstanced(const DX12CL::CommandDrawIndexedInstanced& cmd) noexcept;
    void _setDrawType(const DX12CL::CommandSetDrawType& cmd) noexcept;
    void _setPipelineState(const DX12CL::CommandSetPipelineState& cmd) noexcept;
    void _setStencilRef(const DX12CL::CommandSetStencilRef& cmd) noexcept;
    void _setVertexArray(const DX12CL::CommandSetVertexArray& cmd) noexcept;
    void _setIndexBuffer(const DX12CL::CommandSetIndexBuffer& cmd) noexcept;
    void _copyResource(const DX12CL::CommandCopyResource& cmd) noexcept;
    void _copyBuffer(const DX12CL::CommandCopyBuffer& cmd) noexcept;
    void _copyTexture(const DX12CL::CommandCopyTexture& cmd) noexcept;
    void _copyTextureBox(const DX12CL::CommandCopyTextureBox& cmd) noexcept;
    void _resourceUsage(const DX12CL::CommandResourceUsage& cmd) noexcept;
};
#endif
