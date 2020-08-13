#include "TriangleRenderer.hpp"
#include "d3dx12.h"

#include <FreeImage.h>

void TriangleRenderer::initBuffers(BufferAllocator& bufferAllocator, BufferAllocator& uploadAllocator, const Window& window, ID3D12GraphicsCommandList* const uploadCmdList) noexcept
{
    _positionsBuffer = bufferAllocator.alloc(sizeof(float) * 3 * 3);
    _colorBuffer = bufferAllocator.alloc(sizeof(float) * 4 * 3 * DynamicCycles);
    _uploadBuffer = uploadAllocator.alloc(_positionsBuffer.size() + _colorBuffer.size());

    void* buffer;

    CD3DX12_RANGE readRange(0, 0);
    _uploadBuffer.map(&readRange, &buffer);

    constexpr float root3_4 = 0.43301270189221932338186158537647;

    const float w = 1000.0f / window.cWidth();
    const float h = 1000.0f / window.cHeight();

    // const float vertices[] = {
    //       0.0f * w, (root3_4 - 0.25f) * h, 0.0f,
    //      0.25f * w, -0.25f * h, 0.0f,
    //     -0.25f * w, -0.25f * h, 0.0f
    // };
    
    const float vertices[] = {
        -1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f
    };

    const float colors[] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
    };

    (void) ::std::memcpy(buffer, vertices, sizeof(vertices));
    (void) ::std::memcpy(reinterpret_cast<BYTE*>(buffer) + sizeof(vertices), colors, sizeof(colors));

    CD3DX12_RANGE writeRangePos(0, sizeof(vertices) + sizeof(colors));
    _uploadBuffer.unmap(&writeRangePos);

    uploadCmdList->CopyBufferRegion(_positionsBuffer.resource().get(), 0, _uploadBuffer.resource().get(), 0, sizeof(vertices));
    uploadCmdList->CopyBufferRegion(_colorBuffer.resource().get(), 0, _uploadBuffer.resource().get(), sizeof(vertices), sizeof(colors));

    _dynamicCycle = 1;
}

void TriangleRenderer::initTexture(TextureAllocator& textureAllocator, Allocation& uploadTexture, ID3D12GraphicsCommandList* uploadCmdList) noexcept
{
    constexpr const char* filePath = R"(D:\TauEngine\test\RayMarchingTest\resources\brickwall.png)";

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filePath);

    if(format == FIF_UNKNOWN)
    {
        format = FreeImage_GetFIFFromFilename(filePath);
        if(format == FIF_UNKNOWN)
        {
            return;
        }
    }

    FIBITMAP* texture = nullptr;

    if(FreeImage_FIFSupportsReading(format))
    {
        texture = FreeImage_Load(format, filePath);
    }

    FIBITMAP* tmp = FreeImage_ConvertTo32Bits(texture);
    FreeImage_Unload(texture);
    texture = tmp;

    BYTE* textureData = FreeImage_GetBits(texture);
    const UINT width  = FreeImage_GetWidth(texture);
    const UINT height = FreeImage_GetHeight(texture);

    const CD3DX12_RESOURCE_DESC texArgs = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UINT, width, height);
    _texture = textureAllocator.alloc(texArgs);

    void* textureRaw;

    CD3DX12_RANGE readRange(0, 0);
    uploadTexture.map(&readRange, &textureRaw);

    ::std::memcpy(textureRaw, textureData, width * height * 4);

    FreeImage_Unload(texture);

    CD3DX12_RANGE writeRangePos(0, width * height * 4);
    uploadTexture.unmap(&writeRangePos);

    uploadCmdList->CopyBufferRegion(_texture.resource().get(), 0, uploadTexture.resource().get(), 0, 0);
}

void TriangleRenderer::initCmdLists(ID3D12Device* const device, ID3D12CommandAllocator* const bundleAllocator, ID3D12PipelineState* const baseState) noexcept
{
    constexpr UINT colorSize = sizeof(float) * 4 * 3;

    for(UINT i = 0; i < DynamicCycles; ++i)
    {
        device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_BUNDLE, bundleAllocator, baseState, IID_PPV_ARGS(&_dynamicBundles[i]));
        
        _dynamicBundles[i]->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        D3D12_VERTEX_BUFFER_VIEW buffers[2];
        buffers[0].BufferLocation = _positionsBuffer.getGPUVirtualAddress();
        buffers[0].SizeInBytes = _positionsBuffer.size();
        buffers[0].StrideInBytes = sizeof(float) * 3;
        buffers[1].BufferLocation = _colorBuffer.getGPUVirtualAddress() + colorSize * i;
        buffers[1].SizeInBytes = colorSize;
        buffers[1].StrideInBytes = sizeof(float) * 4;

        _dynamicBundles[i]->IASetVertexBuffers(0, 2, buffers);
        _dynamicBundles[i]->DrawInstanced(3, 1, 0, 0);
        _dynamicBundles[i]->Close();
    }
}

void TriangleRenderer::updateSize(const UINT width, const UINT height, ID3D12GraphicsCommandList* uploadCmdList) noexcept
{
    void* buffer;

    CD3DX12_RANGE readRange(0, 0);
    _uploadBuffer.map(&readRange, &buffer);

    constexpr float root3_4 = 0.43301270189221932338186158537647;

    const float w = 1000.0f / width;
    const float h = 1000.0f / height;

    const float vertices[] = {
          0.0f * w, (root3_4 - 0.25f) * h, 0.0f,
         0.25f * w, -0.25f * h, 0.0f,
        -0.25f * w, -0.25f * h, 0.0f
    };

    (void) ::std::memcpy(buffer, vertices, sizeof(vertices));

    CD3DX12_RANGE writeRangePos(0, sizeof(vertices));
    _uploadBuffer.unmap(&writeRangePos);

    uploadCmdList->CopyBufferRegion(_positionsBuffer.resource().get(), 0, _uploadBuffer.resource().get(), 0, sizeof(vertices));
}

void TriangleRenderer::updateColor(const ColorCycler& top, const ColorCycler& left, const ColorCycler& right, ID3D12GraphicsCommandList* uploadCmdList) noexcept
{
    constexpr UINT verticesSize = sizeof(float) * 3 * 3;

    void* buffer;

    CD3DX12_RANGE readRange(0, 0);
    _uploadBuffer.map(&readRange, &buffer);

    const float colors[] = {
        top.rf(), top.gf(), top.bf(), 1.0f,
        left.rf(), left.gf(), left.bf(), 1.0f,
        right.rf(), right.gf(), right.bf(), 1.0f
    };

    const UINT offset = verticesSize + sizeof(colors) * _dynamicCycle;

    (void) ::std::memcpy(reinterpret_cast<BYTE*>(buffer) + offset, colors, sizeof(colors));

    CD3DX12_RANGE writeRangePos(offset, offset + sizeof(colors));
    _uploadBuffer.unmap(&writeRangePos);

    uploadCmdList->CopyBufferRegion(_colorBuffer.resource().get(), sizeof(colors) * _dynamicCycle, _uploadBuffer.resource().get(), offset, sizeof(colors));

    ++_dynamicCycle;
    if(_dynamicCycle == DynamicCycles)
    {
        _dynamicCycle = 0;
    }
}

void TriangleRenderer::render(ID3D12GraphicsCommandList* renderCmdList) noexcept
{
    UINT cycleIndex = _dynamicCycle;
    if(cycleIndex == 0)
    {
        cycleIndex = DynamicCycles;
    }
    --cycleIndex;

    renderCmdList->ExecuteBundle(_dynamicBundles[cycleIndex].get());
}
