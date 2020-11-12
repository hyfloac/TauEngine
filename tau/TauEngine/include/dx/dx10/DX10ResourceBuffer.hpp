#pragma once

#ifdef _WIN32
#include <d3d10.h>
#include "dx/DXUtils.hpp"
#include "DX10Resource.hpp"

class TAU_DLL DX10ResourceBuffer final : public DX10Resource
{
public:
    static DXGI_FORMAT dxIndexSize(EBuffer::IndexSize indexSize) noexcept;
private:
    ResourceBufferArgs _args;
    ID3D10Buffer* _d3dBuffer;
public:
    DX10ResourceBuffer(const ResourceBufferArgs& args, ID3D10Buffer* const d3dBuffer) noexcept
        : DX10Resource(args.size, EResource::Type::Buffer)
        , _args(args)
        , _d3dBuffer(d3dBuffer)
    { }

    ~DX10ResourceBuffer() noexcept override
    { RELEASE_DX(_d3dBuffer); }

    DX10ResourceBuffer(const DX10ResourceBuffer& copy) noexcept
        : DX10Resource(copy)
        , _args(copy._args)
        , _d3dBuffer(copy._d3dBuffer)
    { _d3dBuffer->AddRef(); }

    DX10ResourceBuffer(DX10ResourceBuffer&& move) noexcept
        : DX10Resource(::std::move(move))
        , _args(::std::move(move._args))
        , _d3dBuffer(move._d3dBuffer)
    { move._d3dBuffer = null; }

    DX10ResourceBuffer& operator=(const DX10ResourceBuffer& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        RELEASE_DX(_d3dBuffer);

        DX10Resource::operator=(copy);

        _args = copy._args;
        _d3dBuffer = copy._d3dBuffer;

        _d3dBuffer->AddRef();

        return *this;
    }

    DX10ResourceBuffer& operator=(DX10ResourceBuffer&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        RELEASE_DX(_d3dBuffer);

        DX10Resource::operator=(::std::move(move));

        _args = move._args;
        _d3dBuffer = move._d3dBuffer;

        move._d3dBuffer = null;

        return *this;
    }
    
    [[nodiscard]] ID3D10Buffer* d3dBuffer() const noexcept { return _d3dBuffer; }

    [[nodiscard]] void* map(ICommandList& context, EResource::MapType mapType, uSys mipLevel, uSys arrayIndex, const ResourceMapRange* mapReadRange) noexcept override;
    void unmap(ICommandList& context, uSys mipLevel, uSys arrayIndex) noexcept override;
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }
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
