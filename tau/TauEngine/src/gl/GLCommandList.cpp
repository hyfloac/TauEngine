#include "gl/GLCommandList.hpp"
#include "gl/GLVertexArray.hpp"
#include "gl/GLResourceBuffer.hpp"
#include "gl/GLCommandAllocator.hpp"
#include "graphics/BufferView.hpp"
#include "gl/GLEnums.hpp"
#include "TauConfig.hpp"

static inline const void* computeHead(const NullableRef<GLCommandAllocator>& allocator) noexcept
{
    const void* head = allocator->head();
    return reinterpret_cast<const u8*>(head) + allocator->allocIndex();
}

GLCommandList::GLCommandList(const NullableRef<GLCommandAllocator>& allocator) noexcept
    : _commandAllocator(allocator)
    , _head(computeHead(allocator))
    , _commandCount(0)
{ }

void GLCommandList::reset(const NullableRef<ICommandAllocator>& allocator) noexcept
{
#if TAU_NULL_CHECK
    if(!allocator)
    { return; }
#endif
#if TAU_RTTI_CHECK
    if(!RTT_CHECK(allocator.get(), GLCommandAllocator))
    { return; }
#endif

    _commandAllocator = RefCast<GLCommandAllocator>(allocator);
    _head = computeHead(_commandAllocator);
    _commandCount = 0;
}

void GLCommandList::begin() noexcept
{ }

void GLCommandList::finish() noexcept
{ }

void GLCommandList::draw(const uSys vertexCount, const uSys startVertex) noexcept
{
    const GLCL::CommandDraw draw(static_cast<GLint>(startVertex), static_cast<GLsizei>(vertexCount));
    (void) _commandAllocator->allocateT<GLCL::Command>(draw);
    ++_commandCount;
}

void GLCommandList::drawIndexed(const uSys indexCount, const uSys startIndex, const iSys baseVertex) noexcept
{
    if(baseVertex > 0)
    {
        const GLCL::CommandDrawIndexedBaseVertex drawIndexedBaseVertex(static_cast<GLsizei>(indexCount), startIndex, static_cast<GLint>(baseVertex));
        (void) _commandAllocator->allocateT<GLCL::Command>(drawIndexedBaseVertex);
    }
    else
    {
        const GLCL::CommandDrawIndexed drawIndexed(static_cast<GLsizei>(indexCount), startIndex);
        (void) _commandAllocator->allocateT<GLCL::Command>(drawIndexed);
    }
    ++_commandCount;
}

void GLCommandList::drawInstanced(const uSys vertexCount, const uSys startVertex, const uSys instanceCount, const uSys startInstance) noexcept
{
    if(startInstance > 0)
    {
        const GLCL::CommandDrawInstancedBaseInstance drawInstancedBaseInstance(static_cast<GLint>(startVertex), static_cast<GLsizei>(vertexCount), static_cast<GLsizei>(instanceCount), static_cast<GLuint>(startInstance));
        (void) _commandAllocator->allocateT<GLCL::Command>(drawInstancedBaseInstance);
    }
    else
    {
        const GLCL::CommandDrawInstanced drawInstanced(static_cast<GLint>(startVertex), static_cast<GLsizei>(vertexCount), static_cast<GLsizei>(instanceCount));
        (void) _commandAllocator->allocateT<GLCL::Command>(drawInstanced);
    }
    ++_commandCount;
}

void GLCommandList::drawIndexedInstanced(const uSys indexCount, const uSys startIndex, const iSys baseVertex, const uSys instanceCount, const uSys startInstance) noexcept
{
    if(baseVertex > 0)
    {
        if(startInstance > 0)
        {
            const GLCL::CommandDrawIndexedBaseVertexInstancedBaseInstance drawIndexedBaseVertexInstancedBaseInstance(static_cast<GLsizei>(indexCount), startIndex, static_cast<GLsizei>(instanceCount), static_cast<GLint>(baseVertex), static_cast<GLuint>(startInstance));
            (void) _commandAllocator->allocateT<GLCL::Command>(drawIndexedBaseVertexInstancedBaseInstance);
        }
        else
        {
            const GLCL::CommandDrawIndexedBaseVertexInstanced drawIndexedBaseVertexInstanced(static_cast<GLsizei>(indexCount), startIndex, static_cast<GLsizei>(instanceCount), static_cast<GLint>(baseVertex));
            (void) _commandAllocator->allocateT<GLCL::Command>(drawIndexedBaseVertexInstanced);
        }
    }
    else
    {
        if(startInstance > 0)
        {
            const GLCL::CommandDrawIndexedInstancedBaseInstance drawIndexedInstancedBaseInstance(static_cast<GLsizei>(indexCount), startIndex, static_cast<GLsizei>(instanceCount), static_cast<GLuint>(startInstance));
            (void) _commandAllocator->allocateT<GLCL::Command>(drawIndexedInstancedBaseInstance);
        }
        else
        {
            const GLCL::CommandDrawIndexedInstanced drawIndexedInstanced(static_cast<GLsizei>(indexCount), startIndex, static_cast<GLsizei>(instanceCount));
            (void) _commandAllocator->allocateT<GLCL::Command>(drawIndexedInstanced);
        }
    }
    ++_commandCount;
}

void GLCommandList::setDrawType(const EGraphics::DrawType drawType) noexcept
{
    const GLCL::CommandSetDrawType setDrawType(GLUtils::glDrawType(drawType));
    (void) _commandAllocator->allocateT<GLCL::Command>(setDrawType);
    ++_commandCount;
}

void GLCommandList::setPipelineState(const PipelineState& pipelineState) noexcept
{
    const GLCL::CommandSetPipelineState setPipelineState(&pipelineState);
    (void) _commandAllocator->allocateT<GLCL::Command>(setPipelineState);
    ++_commandCount;
}

void GLCommandList::setStencilRef(const uSys stencilRef) noexcept
{
    const GLCL::CommandSetStencilRef setStencilRef(stencilRef);
    (void) _commandAllocator->allocateT<GLCL::Command>(setStencilRef);
    ++_commandCount;
}

void GLCommandList::setVertexArray(const NullableRef<IVertexArray>& va) noexcept
{
#if TAU_NULL_CHECK
    if(!va)
    { return; }
#endif
#if TAU_RTTI_CHECK
    if(!RTT_CHECK(va.get(), GLVertexArray))
    { return; }
#endif
    
    /*
     * Add the pointer to the free list.
     */
    (void) _commandAllocator->allocateFreeList<NullableRef<IVertexArray>>(va);

    const NullableRef<GLVertexArray> glVA = RefCast<GLVertexArray>(va);
    const GLCL::CommandSetVertexArray setVertexArray(glVA->vao());
    (void) _commandAllocator->allocateT<GLCL::Command>(setVertexArray);
    ++_commandCount;
}

void GLCommandList::setIndexBuffer(const IndexBufferView& indexBufferView) noexcept
{
#if TAU_NULL_CHECK
    if(!indexBufferView.buffer)
    { return; }
#endif
#if TAU_GENERAL_SAFETY_CHECK
    if(indexBufferView.buffer->resourceType() != EResource::Type::Buffer)
    { return; }
#endif
#if TAU_RTTI_CHECK
    if(!RTTD_CHECK(indexBufferView.buffer.get(), GLResource, IResource))
    { return; }
#endif

    /*
     * Add the pointer to the free list.
     */
    (void) _commandAllocator->allocateFreeList<NullableRef<IResource>>(indexBufferView.buffer);

    NullableRef<GLResourceBuffer> buffer = RefCast<GLResourceBuffer>(indexBufferView.buffer);
    const GLCL::CommandSetIndexBuffer setIndexBuffer(buffer->buffer(), GLUtils::glIndexSize(indexBufferView.indexSize));
    (void) _commandAllocator->allocateT<GLCL::Command>(setIndexBuffer);
    ++_commandCount;
}

void GLCommandList::setGraphicsDescriptorLayout(const DescriptorLayout layout) noexcept
{
    const GLCL::CommandSetGDescriptorLayout setGDescriptorLayout(layout);
    (void) _commandAllocator->allocateT<GLCL::Command>(setGDescriptorLayout);
    ++_commandCount;
}

void GLCommandList::setGraphicsDescriptorTable(const uSys index, const EGraphics::DescriptorType type, const uSys descriptorCount, const GPUDescriptorHandle handle) noexcept
{
    const GLCL::CommandSetGDescriptorTable setGDescriptorTable(index, type, descriptorCount, handle);
    (void) _commandAllocator->allocateT<GLCL::Command>(setGDescriptorTable);
    ++_commandCount;
}

void GLCommandList::executeBundle(const NullableRef<ICommandList>& bundle) noexcept
{
#if TAU_NULL_CHECK
    if(!bundle)
    { return; }
#endif
#if TAU_RTTI_CHECK
    if(!RTT_CHECK(bundle.get(), GLCommandList))
    { return; }
#endif
    
    /*
     * Add the pointer to the free list.
     */
    (void) _commandAllocator->allocateFreeList<NullableRef<ICommandList>>(bundle);

    const GLCL::CommandExecuteBundle executeBundle(RefCast<GLCommandList>(bundle).get());
    (void) _commandAllocator->allocateT<GLCL::Command>(executeBundle);
    ++_commandCount;
}
