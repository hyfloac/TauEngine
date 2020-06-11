#pragma once

#include "graphics/Resource.hpp"

#ifdef _WIN32
#include <d3d10.h>
#include "dx/DXUtils.hpp"
#include "DX10Resource.hpp"

class TAU_DLL DX10ResourceBuffer final : public DX10Resource
{
    DELETE_COPY(DX10ResourceBuffer);
private:
    ID3D10Buffer* _d3dBuffer;
    void* _currentMapping;
public:
    DX10ResourceBuffer(const uSys size, ID3D10Buffer* const d3dBuffer) noexcept
        : DX10Resource(size)
        , _d3dBuffer(d3dBuffer)
        , _currentMapping(null)
    { }

    ~DX10ResourceBuffer() noexcept
    { RELEASE_DX(_d3dBuffer); }

    DX10ResourceBuffer(DX10ResourceBuffer&& move) noexcept
        : DX10Resource(::std::move(move))
        , _d3dBuffer(move._d3dBuffer)
        , _currentMapping(move._currentMapping)
    {
 move._d3dBuffer = null;
}

DX10ResourceBuffer& operator=(DX10ResourceBuffer&& move) noexcept
{
    if(this == &move)
    { return *this; }

    RELEASE_DX(_d3dBuffer);

    _d3dBuffer = move._d3dBuffer;
    _currentMapping = move._currentMapping;

    move._d3dBuffer = null;

    return *this;
}

[[nodiscard]] EResource::Type resourceType() const noexcept override { return EResource::Type::Buffer; }

[[nodiscard]] void* map(IRenderingContext& context, EResource::MapType mapType, const ResourceMapRange* mapReadRange) noexcept override;
void unmap(IRenderingContext& context) noexcept override;
};
#endif
