#include "gl/GLDescriptorHeap.hpp"
#include "gl/GLTextureView.hpp"
#include "gl/GLBufferView.hpp"

GLDescriptorTable::~GLDescriptorTable() noexcept
{
    switch(_type)
    {
        case DescriptorType::TextureView:
            for(uSys i = 0; i < _count; ++i)
            {
                _texViews[i].~GLTextureView();
            }
            break;
        case DescriptorType::UniformBufferView:
            for(uSys i = 0; i < _count; ++i)
            {
                _uniViews[i].~GLUniformBufferView();
            }
            break;
        default: break;
    }

    if(_allocator)
    {
        _allocator->deallocate(_placement);
    }
    else
    {
        delete[] _placement;
    }
}

DescriptorTable GLDescriptorHeap::allocateTable(const uSys descriptors, const DescriptorType type, TauAllocator* allocator) noexcept
{
    u8* placement;

    if(allocator)
    {
        switch(type)
        {
            case DescriptorType::TextureView:
                placement = allocator->allocateT<u8>(descriptors * sizeof(GLTextureView));
                break;
            case DescriptorType::UniformBufferView:
                placement = allocator->allocateT<u8>(descriptors * sizeof(GLUniformBufferView));
                break;
            default: return { null };
        }
    }
    else
    {
        switch(type)
        {
            case DescriptorType::TextureView:
                placement = new(::std::nothrow) u8[descriptors * sizeof(GLTextureView)];
                break;
            case DescriptorType::UniformBufferView:
                placement = new(::std::nothrow) u8[descriptors * sizeof(GLUniformBufferView)];
                break;
            default: return { null };
        }
    }

    if(!placement)
    { return { null }; }

    GLDescriptorTable* ret = new(::std::nothrow) GLDescriptorTable(allocator, type, descriptors, placement);
    return { ret };
}

void GLDescriptorHeap::destroyTable(const DescriptorTable table) noexcept
{
    GLDescriptorTable* glTable = reinterpret_cast<GLDescriptorTable*>(table.raw);
    delete glTable;
}
