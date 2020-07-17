#include "gl/GLCommandQueue.hpp"
#include "gl/GLStateHelper.hpp"
#include "gl/GLDescriptorHeap.hpp"
#include "gl/GLResourceTexture.hpp"
#include "gl/GLTextureView.hpp"
#include "graphics/PipelineState.hpp"
#include "gl/GLBlendingState.hpp"
#include "gl/GLDepthStencilState.hpp"
#include "gl/GLShaderProgram.hpp"
#include "TauConfig.hpp"

void GLCommandQueue::executeCommandLists(uSys count, const ICommandList** lists) noexcept
{
#if TAU_NULL_CHECK
    if(!lists)
    { return; }

    for(uSys i = 0; i < count; ++i)
    {
        if(!lists[i])
        { return; }
    }
#endif

#if TAU_RTTI_CHECK
    for(uSys i = 0; i < count; ++i)
    {
        if(!RTT_CHECK(lists[i], GLCommandList))
        { return; }
    }
#endif

    for(uSys i = 0; i < count; ++i)
    {
        executeCommandList(lists[i]);
    }
}

void GLCommandQueue::executeCommandList(const ICommandList* const list) noexcept
{
    const GLCommandList* const glList = static_cast<const GLCommandList*>(list);
    const ArrayList<GLCL::Command>& commands = glList->commands();

    for(auto cmd : commands)
    {
        switch(cmd.type)
        {
            case GLCL::CommandType::Draw:                 _draw(cmd.draw); break;
            case GLCL::CommandType::DrawIndexed:          _drawIndexed(cmd.drawIndexed); break;
            case GLCL::CommandType::DrawInstanced:        _drawInstanced(cmd.drawInstanced); break;
            case GLCL::CommandType::DrawIndexedInstanced: _drawIndexedInstanced(cmd.drawIndexedInstanced); break;
            case GLCL::CommandType::SetVertexArray:       _setVertexArray(cmd.setVertexArray); break;
            case GLCL::CommandType::SetIndexBuffer:       _setIndexBuffer(cmd.setIndexBuffer); break;
            case GLCL::CommandType::SetGDescriptorLayout: _setGDescriptorLayout(cmd.setGDescriptorLayout); break;
            case GLCL::CommandType::SetGDescriptorTable:  _setGDescriptorTable(cmd.setGDescriptorTable); break;
            default: break;
        }
    }
}

void GLCommandQueue::_draw(const GLCL::CommandDraw& cmd) noexcept
{
    glDrawArrays(cmd.mode, cmd.startVertex, cmd.vertexCount);
}

void GLCommandQueue::_drawIndexed(const GLCL::CommandDrawIndexed& cmd) noexcept
{
    glDrawElements(cmd.mode, cmd.indexCount, cmd.indexSize, cmd.indexOffset);
}

void GLCommandQueue::_drawInstanced(const GLCL::CommandDrawInstanced& cmd) noexcept
{
    glDrawArraysInstanced(cmd.mode, cmd.startVertex, cmd.vertexCount, cmd.instanceCount);
}

void GLCommandQueue::_drawIndexedInstanced(const GLCL::CommandDrawIndexedInstanced& cmd) noexcept
{
    glDrawElementsInstanced(cmd.mode, cmd.indexCount, cmd.indexSize, cmd.indexOffset, cmd.instanceCount);
}

void GLCommandQueue::_setPipelineState(const GLCL::CommandSetPipelineState& cmd) noexcept
{
    _currentPipelineState = &cmd.pipelineState;

    const GLBlendingState* const blendingState = RTT_CAST(_currentPipelineState->args().blendingState.get(), GLBlendingState);
    blendingState->apply(_glStateManager);

    const GLDepthStencilState* const depthStencilState = RTT_CAST(_currentPipelineState->args().depthStencilState.get(), GLDepthStencilState);
    depthStencilState->apply(_glStateManager);

    const GLShaderProgram* shaderProgram = _currentPipelineState->args().shaderProgram.get<GLShaderProgram>();
    _glStateManager.bindShaderProgram(shaderProgram->programHandle());
}

void GLCommandQueue::_setVertexArray(const GLCL::CommandSetVertexArray& cmd) noexcept
{
    _glStateManager.bindVertexArray(cmd.vao);
}

void GLCommandQueue::_setIndexBuffer(const GLCL::CommandSetIndexBuffer& cmd) noexcept
{
    _glStateManager.bindElementArrayBuffer(cmd.ibo);
}

void GLCommandQueue::_setGDescriptorLayout(const GLCL::CommandSetGDescriptorLayout& cmd) noexcept
{
    _currentLayout = cmd.layout.get<GLDescriptorLayout>();
}

void GLCommandQueue::_setGDescriptorTable(const GLCL::CommandSetGDescriptorTable& cmd) noexcept
{
    const GLDescriptorTable* table = cmd.table.get<GLDescriptorTable>();

#if TAU_GENERAL_SAFETY_CHECK
    // Counts don't match
    if(table->count() != _currentLayout->entries()[cmd.index].count)
    { return; }
#endif

    if(table->type() == DescriptorType::TextureView)
    {
#if TAU_GENERAL_SAFETY_CHECK
        // Types don't match
        if(_currentLayout->entries()[cmd.index].type != DescriptorLayoutEntry::Type::TextureView)
        { return; }
#endif

        const uSys begin = _currentLayout->entries()[cmd.index].begin;

        for(uSys i = 0; i < table->count(); ++i)
        {
            GLTextureView* const texView = table->texViews()[i];
            _glStateManager.bindTexture(texView->target(), texView->texture()->texture(), i + begin);
        }
    }
    else if(table->type() == DescriptorType::UniformBufferView)
    {
#if TAU_GENERAL_SAFETY_CHECK
        // Types don't match
        if(_currentLayout->entries()[cmd.index].type != DescriptorLayoutEntry::Type::UniformBufferView)
        { return; }
#endif

        const uSys begin = _currentLayout->entries()[cmd.index].begin;

        for(uSys i = 0; i < table->count(); ++i)
        {
            _glStateManager.bindUniformBufferBase(begin + i, table->uniViews()[i]);
        }
    }
}
