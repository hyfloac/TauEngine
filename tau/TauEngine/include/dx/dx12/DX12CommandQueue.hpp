#pragma once

#include "graphics/CommandQueue.hpp"

#ifdef _WIN32
#include <d3d12.h>

class TAU_DLL DX12CommandQueue final : public ICommandQueue
{
    DELETE_CM(DX12CommandQueue);
private:
    ID3D12CommandQueue* _d3dQueue;
public:
    DX12CommandQueue(ID3D12CommandQueue* const d3dQueue) noexcept
        : _d3dQueue(d3dQueue)
    { }

    ~DX12CommandQueue() noexcept override
    { _d3dQueue->Release(); }

    [[nodiscard]] ID3D12CommandQueue* d3dQueue() const noexcept { return _d3dQueue; }

    void executeCommandLists(uSys count, const ICommandList* const* lists) noexcept override;
    void executeCommandLists(UINT count, ID3D12CommandList* const* lists) noexcept;
};
#endif
