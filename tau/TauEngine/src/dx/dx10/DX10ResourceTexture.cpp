#include "dx/dx10/DX10ResourceTexture.hpp"

#ifdef _WIN32
#include "TauConfig.hpp"

static constexpr u64 Alignment = static_cast<u64>(DX10TransferResource::Alignment);

void DX10ResourceTransferTexture1D::transferMapping(void* const ptr) noexcept
{
#if TAU_NULL_CHECK
    if(!ptr)
    { return; }
#endif

    DX10TransferResource::deallocate(_mapping);
    _mapping = ptr;
}

void* DX10ResourceTransferTexture1D::transferMapping() noexcept
{
    void* const ret = _mapping;
    _mapping = DX10TransferResource::allocate(ETexture::computeAlignedSizeArrMip(_args.dataFormat, Alignment, Alignment, _args.width, _args.mipLevels, _args.arrayCount));
    return ret;
}

void DX10ResourceTransferTexture2D::transferMapping(void* const ptr) noexcept
{
#if TAU_NULL_CHECK
    if(!ptr)
    { return; }
#endif

    DX10TransferResource::deallocate(_mapping);
    _mapping = ptr;
}

void* DX10ResourceTransferTexture2D::transferMapping() noexcept
{
    void* const ret = _mapping;
    _mapping = DX10TransferResource::allocate(ETexture::computeAlignedSizeArrMip(_args.dataFormat, Alignment, Alignment, Alignment, _args.width, _args.height, _args.mipLevels, _args.arrayCount));
    return ret;
}

void DX10ResourceTransferTexture3D::transferMapping(void* const ptr) noexcept
{
#if TAU_NULL_CHECK
    if(!ptr)
    { return; }
#endif

    DX10TransferResource::deallocate(_mapping);
    _mapping = ptr;
}

void* DX10ResourceTransferTexture3D::transferMapping() noexcept
{
    void* const ret = _mapping;
    _mapping = DX10TransferResource::allocate(ETexture::computeAlignedSizeMip(_args.dataFormat, Alignment, Alignment, Alignment, _args.width, _args.height, _args.depth, _args.mipLevels));
    return ret;
}

void* DX10ResourceTransferTexture1D::map(const uSys mipLevel, const uSys arrayIndex, const ResourceMapRange* const mapReadRange, const ResourceMapRange* const mapWriteRange) noexcept
{
    const uSys offset = ETexture::computeOffsetAlignedArrMip(_args.dataFormat, Alignment, Alignment, _args.width, _args.mipLevels, static_cast<u16>(mipLevel), static_cast<u16>(arrayIndex));

    return reinterpret_cast<u8*>(_mapping) + offset;
}

void DX10ResourceTransferTexture1D::unmap(const uSys mipLevel, const uSys arrayIndex, const ResourceMapRange*) noexcept
{
}

void* DX10ResourceTransferTexture2D::map(const uSys mipLevel, const uSys arrayIndex, const ResourceMapRange* const mapReadRange, const ResourceMapRange* const mapWriteRange) noexcept
{
    const uSys offset = ETexture::computeOffsetAlignedArrMip(_args.dataFormat, Alignment, Alignment, Alignment, _args.width, _args.height, _args.mipLevels, static_cast<u16>(mipLevel), static_cast<u16>(arrayIndex));

    return reinterpret_cast<u8*>(_mapping) + offset;
}

void DX10ResourceTransferTexture2D::unmap(const uSys mipLevel, const uSys arrayIndex, const ResourceMapRange*) noexcept
{
}

void* DX10ResourceTransferTexture3D::map(const uSys mipLevel, const uSys arrayIndex, const ResourceMapRange* const mapReadRange, const ResourceMapRange* const mapWriteRange) noexcept
{
    const uSys offset = ETexture::computeOffsetAlignedArrMip(_args.dataFormat, Alignment, Alignment, Alignment, _args.width, _args.height, _args.depth, _args.mipLevels, static_cast<u16>(mipLevel), static_cast<u16>(arrayIndex));

    return reinterpret_cast<u8*>(_mapping) + offset;
}

void DX10ResourceTransferTexture3D::unmap(const uSys mipLevel, const uSys arrayIndex, const ResourceMapRange*) noexcept
{
}
#endif
