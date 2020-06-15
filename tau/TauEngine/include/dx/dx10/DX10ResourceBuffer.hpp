#pragma once

#ifdef _WIN32
#include <d3d10.h>
#include "dx/DXUtils.hpp"
#include "DX10Resource.hpp"

class TAU_DLL DX10ResourceBuffer final : public DX10Resource
{
private:
    ResourceBufferArgs _args;
    ID3D10Buffer* _d3dBuffer;
public:
    DX10ResourceBuffer(const ResourceBufferArgs& args, ID3D10Buffer* const d3dBuffer) noexcept
        : _args(args)
        , _d3dBuffer(d3dBuffer)
    { }

    ~DX10ResourceBuffer() noexcept
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
    
    [[nodiscard]]       ID3D10Buffer* d3dBuffer()       noexcept { return _d3dBuffer; }
    [[nodiscard]] const ID3D10Buffer* d3dBuffer() const noexcept { return _d3dBuffer; }

    [[nodiscard]] EResource::Type resourceType() const noexcept override { return EResource::Type::Buffer; }

    [[nodiscard]] void* map(IRenderingContext& context, EResource::MapType mapType, uSys subResource, const ResourceMapRange* mapReadRange) noexcept override;
    void unmap(IRenderingContext& context, uSys subResource) noexcept override;
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }
};
#endif
