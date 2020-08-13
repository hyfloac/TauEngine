#pragma once

#include <d3d12.h>
#include <winrt/base.h>
#include "Allocation.hpp"
#include <list>

namespace AllocUtils {

struct FreeBlock final
{
    UINT64 offset;
    UINT64 size;
    UINT64 slackSpace;

    FreeBlock() noexcept = default;

    FreeBlock(const UINT64 offset, const UINT64 size, const UINT slackSpace) noexcept
        : offset(offset)
        , size(size)
        , slackSpace(slackSpace)
    { }

    [[nodiscard]] bool operator ==(const FreeBlock& other) const noexcept { return offset == other.offset && size == other.size; }
    [[nodiscard]] bool operator !=(const FreeBlock& other) const noexcept { return offset != other.offset || size != other.size; }
};

struct Block final
{
    winrt::com_ptr<ID3D12Heap> heap;
    UINT64 size;
    ::std::list<FreeBlock> freeBlocks;

    Block() noexcept = default;

    Block(const winrt::com_ptr<ID3D12Heap>& id3D12Heap, const UINT64 size) noexcept
        : heap(id3D12Heap)
        , size(size)
    { }
};

enum class Type
{
    Default = 0,
    Upload
};

}

class BufferAllocator
{
public:
    using FreeBlock = AllocUtils::FreeBlock;
    using Block = AllocUtils::Block;
    using Type = AllocUtils::Type;

    static constexpr UINT64 BlockSize = 256 * 1024 * 1024;
private:
    winrt::com_ptr<ID3D12Device> _device;
    Type _type;
    ::std::list<Block> _blocks;
public:
    BufferAllocator(const winrt::com_ptr<ID3D12Device>& device, const Type type) noexcept
        : _device(device)
        , _type(type)
    { }

    Allocation alloc(UINT size) noexcept;
public:
    Block& initBlock() noexcept;

    void pruneFreeBlocks() noexcept;
};

class TextureAllocator
{
public:
    using FreeBlock = AllocUtils::FreeBlock;
    using Block = AllocUtils::Block;
    using Type = AllocUtils::Type;

    static constexpr UINT64 BlockSize = 256 * 1024 * 1024;
private:
    winrt::com_ptr<ID3D12Device> _device;
    Type _type;
    ::std::list<Block> _blocks;
public:
    TextureAllocator(const winrt::com_ptr<ID3D12Device>& device, const Type type) noexcept
        : _device(device)
        , _type(type)
    { }

    Allocation alloc(const D3D12_RESOURCE_DESC& desc) noexcept;
public:
    Block& initBlock() noexcept;

    void pruneFreeBlocks() noexcept;
};

class RenderTargetAllocator
{
public:
    using FreeBlock = AllocUtils::FreeBlock;
    using Block = AllocUtils::Block;
    using Type = AllocUtils::Type;

    static constexpr UINT64 BlockSize = 256 * 1024 * 1024;
private:
    winrt::com_ptr<ID3D12Device> _device;
    Type _type;
    ::std::list<Block> _blocks;
public:
    RenderTargetAllocator(const winrt::com_ptr<ID3D12Device>& device, const Type type) noexcept
        : _device(device)
        , _type(type)
    { }

    Allocation alloc(const D3D12_RESOURCE_DESC& desc) noexcept;
public:
    Block& initBlock() noexcept;

    void pruneFreeBlocks() noexcept;
};
