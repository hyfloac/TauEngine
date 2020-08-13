#include "gl/GLCommandList.hpp"
#include "gl/GLVertexArray.hpp"
#include "gl/GLBuffer.hpp"
#include "gl/GLResourceBuffer.hpp"
#include "graphics/BufferView.hpp"
#include "gl/GLEnums.hpp"

void GLCommandList::reset() noexcept
{
    _commands = ArrayList<GLCL::Command>(_maxCommands);
    _refCountList.reset(true);
}

void GLCommandList::finish() noexcept
{ }

void GLCommandList::draw(const uSys vertexCount, const uSys startVertex) noexcept
{
    const GLCL::CommandDraw draw(startVertex, vertexCount);
    _commands.emplace(draw);
}

void GLCommandList::drawIndexed(const uSys indexCount, const uSys startIndex, const iSys baseVertex) noexcept
{
    if(baseVertex > 0)
    {
        const GLCL::CommandDrawIndexedBaseVertex drawIndexedBaseVertex(indexCount, startIndex, baseVertex);
        _commands.emplace(drawIndexedBaseVertex);
    }
    else
    {
        const GLCL::CommandDrawIndexed drawIndexed(indexCount, startIndex);
        _commands.emplace(drawIndexed);
    }
}

void GLCommandList::drawInstanced(const uSys vertexCount, const uSys startVertex, const uSys instanceCount, const uSys startInstance) noexcept
{
    if(startInstance > 0)
    {
        const GLCL::CommandDrawInstancedBaseInstance drawInstancedBaseInstance(startVertex, vertexCount, instanceCount, startInstance);
        _commands.emplace(drawInstancedBaseInstance);
    }
    else
    {
        const GLCL::CommandDrawInstanced drawInstanced(startVertex, vertexCount, instanceCount);
        _commands.emplace(drawInstanced);
    }
}

void GLCommandList::drawIndexedInstanced(const uSys indexCount, const uSys startIndex, const iSys baseVertex, const uSys instanceCount, const uSys startInstance) noexcept
{
    if(baseVertex > 0)
    {
        if(startInstance > 0)
        {
            const GLCL::CommandDrawIndexedBaseVertexInstancedBaseInstance drawIndexedBaseVertexInstancedBaseInstance(indexCount, startIndex, instanceCount, baseVertex, startInstance);
            _commands.emplace(drawIndexedBaseVertexInstancedBaseInstance);
        }
        else
        {
            const GLCL::CommandDrawIndexedBaseVertexInstanced drawIndexedBaseVertexInstanced(indexCount, startIndex, instanceCount, baseVertex);
            _commands.emplace(drawIndexedBaseVertexInstanced);
        }
    }
    else
    {
        if(startInstance > 0)
        {
            const GLCL::CommandDrawIndexedInstancedBaseInstance drawIndexedInstancedBaseInstance(indexCount, startIndex, instanceCount, startInstance);
            _commands.emplace(drawIndexedInstancedBaseInstance);
        }
        else
        {
            const GLCL::CommandDrawIndexedInstanced drawIndexedInstanced(indexCount, startIndex, instanceCount);
            _commands.emplace(drawIndexedInstanced);
        }
    }
}

void GLCommandList::setDrawType(const EGraphics::DrawType drawType) noexcept
{
    const GLCL::CommandSetDrawType setDrawType(GLUtils::glDrawType(drawType));
    _commands.emplace(setDrawType);
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
    const GLCL::CommandSetIndexBuffer setIndexBuffer(buffer->buffer(), GLUtils::glIndexSize(indexBufferView.indexSize));
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
