#include "dx/dx12/DX12Resource.hpp"

#ifdef _WIN32
#include "dx/dx12/d3dx12.h"
#include "dx/dx12/D3D12MemAlloc.h"
#include "dx/dx12/DX12GraphicsInterface.hpp"
#include "dx/dx12/DX12ResourceBuffer.hpp"
#include "dx/dx12/DX12ResourceTexture.hpp"

NullableRef<IResource> DX12ResourceBuilder::buildTauRef(const ResourceBufferArgs& args, const ResourceHeap heap, Error* error, TauAllocator& allocator) const noexcept
{
    if(!heap.raw)
    {
        ERROR_CODE_COND_F(args.size == 0, Error::InvalidSize);

        D3D12_RESOURCE_DESC desc;
        desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Alignment = 0;
        desc.Width = args.size;
        desc.Height = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc = { 1, 0 };
        desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        desc.Flags = D3D12_RESOURCE_FLAG_NONE;

        D3D12MA::ALLOCATION_DESC allocDesc;
        allocDesc.Flags = D3D12MA::ALLOCATION_FLAG_NONE;

        D3D12_RESOURCE_STATES initialState = D3D12_RESOURCE_STATE_COPY_DEST;

        EGraphics::ResourceHeapUsageType heapUsage;

        switch(args.usageType)
        {
            case EResource::UsageType::Default:
            case EResource::UsageType::Immutable:
            case EResource::UsageType::Dynamic:
                allocDesc.HeapType = D3D12_HEAP_TYPE_DEFAULT;
                heapUsage = EGraphics::ResourceHeapUsageType::Default;
                break;
            case EResource::UsageType::Streaming:
                allocDesc.HeapType = D3D12_HEAP_TYPE_UPLOAD;
                initialState = D3D12_RESOURCE_STATE_GENERIC_READ;
                heapUsage = EGraphics::ResourceHeapUsageType::Upload;
                break;
            case EResource::UsageType::Readable:
                allocDesc.HeapType = D3D12_HEAP_TYPE_READBACK;
                heapUsage = EGraphics::ResourceHeapUsageType::Read;
                break;
        }

        D3D12MA::Allocation* allocation;
        ID3D12Resource* d3dResource;
        HRESULT res = _gi.gpuAllocator()->CreateResource(&allocDesc, &desc, initialState, nullptr, &allocation, IID_PPV_ARGS(&d3dResource));

        ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);



        bool keepUploadAllocation = false;
        NullableRef<DX12ResourceBuffer> resource = nullptr;

        switch(args.usageType)
        {
            case EResource::UsageType::Default:
                keepUploadAllocation = true;
            case EResource::UsageType::Dynamic:
            {
                resource = NullableRef<DX12ResourceBufferIndirectMapping>(allocator, args.size, d3dResource, allocation, heapUsage, args, _gi, keepUploadAllocation);
                break;
            }
            case EResource::UsageType::Immutable:
            case EResource::UsageType::Streaming:
            case EResource::UsageType::Readable:
            {
                resource = NullableRef<DX12ResourceBuffer>(allocator, args.size, d3dResource, allocation, heapUsage, args);
                break;
            }
        }

        if(args.initialBuffer)
        {
            switch(args.usageType)
            {
                case EResource::UsageType::Default:
                case EResource::UsageType::Dynamic:
                {
                    break;
                }
                case EResource::UsageType::Immutable:
                {
                    CD3DX12_RANGE readRange(0, 0);
                    void* mapping;
                    res = d3dResource->Map(0, &readRange, &mapping);

                    if(FAILED(res))
                    { break; }

                    (void) ::std::memcpy(mapping, args.initialBuffer, args.size);

                    d3dResource->Unmap(0, nullptr);

                    break;
                }
                case EResource::UsageType::Streaming:
                {
                    void* mapping = resource->map(EResource::MapType::Discard, ResourceMapRange::none());

                    if(!mapping)
                    { break; }

                    (void) ::std::memcpy(mapping, args.initialBuffer, args.size);

                    resource->unmap(ResourceMapRange::all());
                }
                case EResource::UsageType::Readable: break;
            }
        }

        ERROR_CODE_V(Error::NoError, ::std::move(resource));
    }
    else
    {
        ERROR_CODE_N(Error::InternalError);
    }

    ERROR_CODE_N(Error::InternalError);
}

NullableRef<IResource> DX12ResourceBuilder::buildTauRef(const ResourceTexture1DArgs& args, ResourceHeap heap, Error* error, TauAllocator& allocator) const noexcept
{
}

NullableRef<IResource> DX12ResourceBuilder::buildTauRef(const ResourceTexture2DArgs& args, ResourceHeap heap, Error* error, TauAllocator& allocator) const noexcept
{
}

NullableRef<IResource> DX12ResourceBuilder::buildTauRef(const ResourceTexture3DArgs& args, ResourceHeap heap, Error* error, TauAllocator& allocator) const noexcept
{
}

uSys DX12ResourceBuilder::_allocSize(const uSys type) const noexcept
{
    switch(type)
    {
        case RB_AS_BUFFER:     return sizeof(NullableRef<DX12ResourceBuffer>::allocSize());
        case RB_AS_TEXTURE_1D: return sizeof(NullableRef<DX12ResourceTexture1D>::allocSize());
        case RB_AS_TEXTURE_2D:
        case RB_AS_TEXTURE_3D:
        default: return static_cast<uSys>(-1);
    }
}
#endif
