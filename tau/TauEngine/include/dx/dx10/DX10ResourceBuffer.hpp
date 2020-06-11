#pragma once

#ifdef _WIN32
#include <d3d10.h>
#include "dx/DXUtils.hpp"
#include "DX10Resource.hpp"

class TAU_DLL DX10ResourceBuffer final : public DX10Resource
{
    DELETE_COPY(DX10ResourceBuffer);
private:
    ResourceBufferArgs _args;
    ID3D10Buffer* _d3dBuffer;
    void* _currentMapping;
public:
    DX10ResourceBuffer(const ResourceBufferArgs& args, ID3D10Buffer* const d3dBuffer) noexcept
        : DX10Resource(args.size)
        , _args(args)
        , _d3dBuffer(d3dBuffer)
        , _currentMapping(null)
    { }

    ~DX10ResourceBuffer() noexcept
    { RELEASE_DX(_d3dBuffer); }

    DX10ResourceBuffer(DX10ResourceBuffer&& move) noexcept
        : DX10Resource(::std::move(move))
        , _args(::std::move(move._args))
        , _d3dBuffer(move._d3dBuffer)
        , _currentMapping(move._currentMapping)
    { move._d3dBuffer = null; }

    DX10ResourceBuffer& operator=(DX10ResourceBuffer&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        RELEASE_DX(_d3dBuffer);

        _args = move._args;
        _d3dBuffer = move._d3dBuffer;
        _currentMapping = move._currentMapping;

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
