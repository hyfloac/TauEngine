#include "gl/GLCommandList.hpp"
#include "gl/GLVertexArray.hpp"
#include "gl/GLBuffer.hpp"

void GLCommandList::reset() noexcept
{
    _commands = ArrayList<GLCL::Command>(_maxCommands);
}

void GLCommandList::finish() noexcept
{ }

void GLCommandList::draw(const uSys exCount, const uSys startIndex, const uSys startVertex) noexcept
{
    if(_currentVA->indexBuffer())
    {
        const GLCL::CommandDrawIndexed drawIndexed(_currentVA->glDrawType(), exCount, _currentVA->indexBuffer()->glIndexSize(), startIndex);
        _commands.emplace(drawIndexed);
    }
    else
    {
        const GLCL::CommandDraw draw(_currentVA->glDrawType(), startVertex, exCount);
        _commands.emplace(draw);
    }
}

void GLCommandList::drawInstanced(const uSys exCount, const uSys startIndex, const uSys startVertex, const uSys instanceCount, const uSys startInstance) noexcept
{
    if(_currentVA->indexBuffer())
    {
        const GLCL::CommandDrawIndexedInstanced drawIndexedInstanced(_currentVA->glDrawType(), exCount, _currentVA->indexBuffer()->glIndexSize(), startIndex, instanceCount);
        _commands.emplace(drawIndexedInstanced);
    }
    else
    {
        const GLCL::CommandDrawInstanced drawInstanced(_currentVA->glDrawType(), startVertex, exCount, instanceCount);
        _commands.emplace(drawInstanced);
    }
}

void GLCommandList::setPipelineState(const PipelineState& pipelineState) noexcept
{
    const GLCL::CommandSetPipelineState setPipelineState(pipelineState);
    _commands.emplace(setPipelineState);
}

void GLCommandList::setVertexArray(const IVertexArray& va) noexcept
{
    _currentVA = static_cast<const GLVertexArray*>(&va);
    if(_currentVA->indexBuffer())
    {
        const GLCL::CommandSetIndexBuffer setIndexBuffer(_currentVA->indexBuffer()->buffer());
        _commands.emplace(setIndexBuffer);
    }
    const GLCL::CommandSetVertexArray setVertexArray(_currentVA->vao());
    _commands.emplace(setVertexArray);
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
