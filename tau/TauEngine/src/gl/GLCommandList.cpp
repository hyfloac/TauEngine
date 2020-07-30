#include "gl/GLCommandList.hpp"
#include "gl/GLVertexArray.hpp"
#include "gl/GLBuffer.hpp"
#include "gl/GLResourceBuffer.hpp"
#include "graphics/BufferView.hpp"

void GLCommandList::reset() noexcept
{
    _commands = ArrayList<GLCL::Command>(_maxCommands);
    _refCountList.reset(true);
}

void GLCommandList::finish() noexcept
{ }

void GLCommandList::draw(const uSys exCount, const uSys startVertex) noexcept
{
    const GLCL::CommandDraw draw(_currentVA->glDrawType(), startVertex, exCount);
    _commands.emplace(draw);
}

void GLCommandList::drawIndexed(const uSys exCount, const uSys startIndex, const iSys baseVertex) noexcept
{
    if(baseVertex > 0)
    {
        const GLCL::CommandDrawIndexed drawIndexed(_currentVA->glDrawType(), exCount, _currentVA->indexBuffer()->glIndexSize(), startIndex);
        _commands.emplace(drawIndexed);
    }
    else
    {
        const GLCL::CommandDrawIndexedBaseVertex drawIndexedBaseVertex(_currentVA->glDrawType(), exCount, _currentVA->indexBuffer()->glIndexSize(), startIndex, baseVertex);
        _commands.emplace(drawIndexedBaseVertex);
    }
}

void GLCommandList::drawInstanced(const uSys exCount, const uSys startVertex, const uSys instanceCount, const uSys startInstance) noexcept
{
    const GLCL::CommandDrawInstanced drawInstanced(_currentVA->glDrawType(), startVertex, exCount, instanceCount);
    _commands.emplace(drawInstanced);
}

void GLCommandList::drawIndexedInstanced(const uSys exCount, const uSys startIndex, const iSys baseVertex, const uSys instanceCount, const uSys startInstance) noexcept
{
    if(baseVertex > 0)
    {
        const GLCL::CommandDrawIndexedInstanced drawIndexedInstanced(_currentVA->glDrawType(), exCount, _currentVA->indexBuffer()->glIndexSize(), startIndex, instanceCount);
        _commands.emplace(drawIndexedInstanced);
    }
    else
    {
        const GLCL::CommandDrawIndexedBaseVertexInstanced drawIndexedBaseVertexInstanced(_currentVA->glDrawType(), exCount, _currentVA->indexBuffer()->glIndexSize(), startIndex, instanceCount, baseVertex);
        _commands.emplace(drawIndexedBaseVertexInstanced);
    }
}

void GLCommandList::setPipelineState(const PipelineState& pipelineState) noexcept
{
    const GLCL::CommandSetPipelineState setPipelineState(&pipelineState);
    _commands.emplace(setPipelineState);
}

void GLCommandList::setStencilRef(const uSys stencilRef) noexcept
{
    const GLCL::CommandSetStencilRef setStencilRef(stencilRef);
    _commands.emplace(setStencilRef);
}

void GLCommandList::setVertexArray(const NullableRef<IVertexArray>& va) noexcept
{
    if(!RTT_CHECK(va.get(), GLVertexArray))
    { return; }
    
    /*
     * Add the pointer to the free list.
     */
    (void) _refCountList.allocateT<NullableRef<IVertexArray>>(va);

    const NullableRef<GLVertexArray> glVA = RefCast<GLVertexArray>(va);
    const GLCL::CommandSetVertexArray setVertexArray(glVA->vao());
    _commands.emplace(setVertexArray);
}

void GLCommandList::setIndexBuffer(const IndexBufferView& indexBufferView) noexcept
{
    if(!indexBufferView.buffer || 
        indexBufferView.buffer->resourceType() != EResource::Type::Buffer || 
       !RTTD_CHECK(indexBufferView.buffer.get(), GLResource, IResource))
    { return; }

    /*
     * Add the pointer to the free list.
     */
    (void) _refCountList.allocateT<NullableRef<IResource>>(indexBufferView.buffer);

    NullableRef<GLResourceBuffer> buffer = RefCast<GLResourceBuffer>(indexBufferView.buffer);
    const GLCL::CommandSetIndexBuffer setIndexBuffer(buffer->buffer());
    _commands.emplace(setIndexBuffer);
}


void GLCommandList::setGraphicsDescriptorLayout(const DescriptorLayout layout) noexcept
{
    const GLCL::CommandSetGDescriptorLayout setGDescriptorLayout(layout);
    _commands.emplace(setGDescriptorLayout);
}

void GLCommandList::setGraphicsDescriptorTable(const uSys index, const DescriptorTable table) noexcept
{
    const GLCL::CommandSetGDescriptorTable setGDescriptorTable(index, table);
    _commands.emplace(setGDescriptorTable);
}

void GLCommandList::setGraphicsDescriptorTable(const uSys index, const DescriptorSamplerTable table) noexcept
{
    const GLCL::CommandSetGDescriptorSamplerTable setGDescriptorSamplerTable(index, table);
    _commands.emplace(setGDescriptorSamplerTable);
}
