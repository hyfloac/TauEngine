#pragma once

#include "graphics/DescriptorHeap.hpp"

class GLTextureView;
class GLUniformBufferView;

class TAU_DLL GLDescriptorTable final
{
    DELETE_CM(GLDescriptorTable);
private:
    TauAllocator* _allocator;
    DescriptorType _type;
    uSys _count;
    union
    {
        u8* _placement;
        GLTextureView* _texViews;
        GLUniformBufferView* _uniViews;
    };
public:
    GLDescriptorTable(TauAllocator* const allocator, const DescriptorType type, const uSys count, u8* const placement) noexcept
        : _allocator(allocator)
        , _type(type)
        , _count(count)
        , _placement(placement)
    { }

    ~GLDescriptorTable() noexcept;

    [[nodiscard]] DescriptorType type() const noexcept { return _type; }
    [[nodiscard]] uSys count() const noexcept { return _count; }

    [[nodiscard]] u8* placement() noexcept { return _placement; }

    [[nodiscard]]       GLTextureView* texViews()       noexcept { return _texViews; }
    [[nodiscard]] const GLTextureView* texViews() const noexcept { return _texViews; }

    [[nodiscard]]       GLUniformBufferView* uniViews()       noexcept { return _uniViews; }
    [[nodiscard]] const GLUniformBufferView* uniViews() const noexcept { return _uniViews; }
};

class TAU_DLL GLDescriptorHeap final : public IDescriptorHeap
{
    DEFAULT_CONSTRUCT_PU(GLDescriptorHeap);
    DEFAULT_DESTRUCT(GLDescriptorHeap);
    DEFAULT_CM_PU(GLDescriptorHeap);
public:
    [[nodiscard]] DescriptorTable allocateTable(uSys descriptors, DescriptorType type, TauAllocator* allocator) noexcept override;
    [[nodiscard]] void destroyTable(DescriptorTable table) noexcept override;
};
