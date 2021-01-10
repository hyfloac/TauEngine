#pragma once
#include "DynArray.hpp"

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

class TAU_DLL DX10ResourceBuffer : public DX10Resource
{
    DELETE_CM(DX10ResourceBuffer);
public:
    static DXGI_FORMAT dxIndexSize(EBuffer::IndexSize indexSize) noexcept;
protected:
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

    [[nodiscard]] void* map(uSys, uSys, const ResourceMapRange* const mapReadRange, const ResourceMapRange* const mapWriteRange) noexcept override
    { return map(mapReadRange, mapWriteRange); }

    void unmap(uSys, uSys, const ResourceMapRange*) noexcept override
    { unmap(); }

    [[nodiscard]] void* map(const ResourceMapRange* mapReadRange, const ResourceMapRange* mapWriteRange) noexcept;

    [[nodiscard]] const IResourceRawInterface& _getRawHandle() const noexcept override final { return _rawInterface; }
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override final { return &_args; }

#if defined(_MSVC_LANG) || 1
    void setD3DBuffer(ID3D10Buffer* const buffer) noexcept { _rawInterface._d3dBuffer = buffer; }
#endif
private:
    friend class DX10CommandList;
};

class TAU_DLL DX10ResourceDefaultBuffer final : public DX10ResourceBuffer
{
    DEFAULT_DESTRUCT(DX10ResourceDefaultBuffer);
    DELETE_CM(DX10ResourceDefaultBuffer);
public:
    DX10ResourceDefaultBuffer(const ResourceBufferArgs& args, ID3D10Buffer* const d3dBuffer) noexcept
        : DX10ResourceBuffer(args, d3dBuffer)
    { }

    [[nodiscard]] void* map(uSys, uSys, const ResourceMapRange* mapReadRange, const ResourceMapRange* mapWriteRange) noexcept override { return nullptr; }
    void unmap(uSys, uSys, const ResourceMapRange*) noexcept override { }
};

class TAU_DLL DX10ResourceTransferBuffer final : public DX10ResourceBuffer
{
    DELETE_CM(DX10ResourceTransferBuffer);
public:
    static constexpr ::std::align_val_t Alignment { 128 };

    static void* allocate(const uSys size) noexcept
    { return operator new(size, Alignment, ::std::nothrow); }

    static void deallocate(void* const ptr) noexcept
    { operator delete(ptr, Alignment, ::std::nothrow); }
private:
    void* _mapping;
public:
    DX10ResourceTransferBuffer(const ResourceBufferArgs& args, ID3D10Buffer* const d3dBuffer) noexcept
        : DX10ResourceBuffer(args, d3dBuffer)
        , _mapping(allocate(args.size))
    { }

    ~DX10ResourceTransferBuffer() noexcept override
    { deallocate(_mapping); }

    void transferMapping(void* ptr) noexcept;
    void* transferMapping() noexcept;

    [[nodiscard]] void* mapping() const noexcept { return _mapping; }

    [[nodiscard]] void* map(uSys, uSys, const ResourceMapRange* mapReadRange, const ResourceMapRange* mapWriteRange) noexcept override { return map(); }
    void unmap(uSys, uSys, const ResourceMapRange*) noexcept override { return unmap(); }
    
    [[nodiscard]] void* map() noexcept;
    void unmap() noexcept;
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
