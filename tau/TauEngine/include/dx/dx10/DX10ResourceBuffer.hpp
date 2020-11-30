#pragma once

#ifdef _WIN32
#include <d3d10.h>
#include "dx/DXUtils.hpp"
#include "DX10Resource.hpp"
#include "graphics/ResourceRawInterface.hpp"

class TAU_DLL DX10ResourceBufferRawInterface final : public IResourceRawInterface
{
private:
    ID3D10Buffer* _d3dBuffer;
public:
    DX10ResourceBufferRawInterface(ID3D10Buffer* const d3dBuffer) noexcept
        : _d3dBuffer(d3dBuffer)
    { }

    [[nodiscard]] void* rawHandle() const noexcept override { return _d3dBuffer; }

    [[nodiscard]] ID3D10Resource* dx10Resource() const noexcept override { return _d3dBuffer; }
    [[nodiscard]] ID3D10Buffer* dx10Buffer() const noexcept override { return _d3dBuffer; }
private:
    friend class DX10ResourceBuffer;
};

class TAU_DLL DX10ResourceBuffer final : public DX10Resource
{
    DELETE_CM(DX10ResourceBuffer);
public:
    static DXGI_FORMAT dxIndexSize(EBuffer::IndexSize indexSize) noexcept;
private:
    ResourceBufferArgs _args;
    DX10ResourceBufferRawInterface _rawInterface;
    __declspec(property(get = d3dBuffer, put = setD3DBuffer)) ID3D10Buffer* _d3dBuffer;
public:
    DX10ResourceBuffer(const ResourceBufferArgs& args, ID3D10Buffer* const d3dBuffer) noexcept
        : DX10Resource(args.size, EResource::Type::Buffer)
        , _args(args)
        , _rawInterface(d3dBuffer)
    { }

    ~DX10ResourceBuffer() noexcept override
    { RELEASE_DX(_d3dBuffer); }
    
    [[nodiscard]] ID3D10Buffer* d3dBuffer() const noexcept { return _rawInterface._d3dBuffer; }

    [[nodiscard]] void* map(ICommandList&, const EResource::MapType mapType, uSys, uSys, const ResourceMapRange* const mapReadRange, const ResourceMapRange* const mapWriteRange) noexcept override
    { return map(mapType, mapReadRange, mapWriteRange); }

    void unmap(ICommandList&, uSys, uSys, const ResourceMapRange*) noexcept override
    { unmap(); }

    [[nodiscard]] void* map(EResource::MapType mapType, const ResourceMapRange* mapReadRange, const ResourceMapRange* mapWriteRange) noexcept;
    void unmap() noexcept;

    [[nodiscard]] const IResourceRawInterface& _getRawHandle() const noexcept override { return _rawInterface; }
private:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }

#if defined(_MSVC_LANG) || 1
    void setD3DBuffer(ID3D10Buffer* const buffer) noexcept { _rawInterface._d3dBuffer = buffer; }
#endif
};

inline DXGI_FORMAT DX10ResourceBuffer::dxIndexSize(const EBuffer::IndexSize indexSize) noexcept
{
    switch(indexSize)
    {
        case EBuffer::IndexSize::Uint32: return DXGI_FORMAT_R32_UINT;
        case EBuffer::IndexSize::Uint16: return DXGI_FORMAT_R16_UINT;
        default: return static_cast<DXGI_FORMAT>(0);
    }
}
#endif
