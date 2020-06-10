#include "dx/dx10/DX10Resource.hpp"

#ifdef _WIN32
void* DX10ResourceBuffer::map(IRenderingContext&, const ResourceMapRange* mapReadRange) noexcept
{
    if(_currentMapping)
    { return _currentMapping; }

    if(!mapReadRange || mapReadRange->begin < mapReadRange->end)
    {
        _d3dBuffer->Map(D3D10_MAP_READ_WRITE, 0, &_currentMapping);
    }
    else
    {
        _d3dBuffer->Map(D3D10_MAP_WRITE, 0, &_currentMapping);
    }

    return _currentMapping;
}

void DX10ResourceBuffer::unmap(IRenderingContext&) noexcept
{
    if(_currentMapping)
    {
        _d3dBuffer->Unmap();
    }
}
#endif
