#pragma once

#include "graphics/CommandList.hpp"

#ifdef _WIN32
#include <d3d12.h>

class DX12CommandAllocator;

class TAU_DLL DX12CommandList final : public ICommandList
{
    COMMAND_LIST_IMPL(DX12CommandList);
private:
    ID3D12GraphicsCommandList* _cmdList;
    NullableRef<DX12CommandAllocator> _cmdAllocator;
public:
    void reset(const NullableRef<ICommandAllocator>& allocator, PipelineState* initialState) noexcept override;
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
};
#endif
