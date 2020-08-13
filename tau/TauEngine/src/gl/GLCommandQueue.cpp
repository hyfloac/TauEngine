#include "gl/GLCommandQueue.hpp"
#include "gl/GLDescriptorHeap.hpp"
#include "gl/GLResourceTexture.hpp"
#include "gl/GLTextureView.hpp"
#include "graphics/PipelineState.hpp"
#include "gl/GLBlendingState.hpp"
#include "gl/GLDepthStencilState.hpp"
#include "gl/GLRasterizerState.hpp"
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
    /*
     * Copy the command array so that the list can be reset.
     * This will perform a reference count copy.
     */
    const ArrayList<GLCL::Command> commands = glList->commands();  // NOLINT(performance-unnecessary-copy-initialization)

    for(auto cmd : commands)
    {
#define DISPATCH(__CMD, __FUNC, __ARG) case GLCL::CommandType::__CMD: __FUNC(cmd.__ARG); break

        switch(cmd.type)
        {
            DISPATCH(Draw, _draw, draw);
            DISPATCH(DrawIndexed, _drawIndexed, drawIndexed);
            DISPATCH(DrawIndexedBaseVertex, _drawIndexedBaseVertex, drawIndexedBaseVertex);
            DISPATCH(DrawInstanced, _drawInstanced, drawInstanced);
            DISPATCH(DrawInstancedBaseInstance, _drawInstancedBaseInstance, drawInstancedBaseInstance);
            DISPATCH(DrawIndexedInstanced, _drawIndexedInstanced, drawIndexedInstanced);
            DISPATCH(DrawIndexedBaseVertexInstanced, _drawIndexedBaseVertexInstanced, drawIndexedBaseVertexInstanced);
            DISPATCH(DrawIndexedInstancedBaseInstance, _drawIndexedInstancedBaseInstance, drawIndexedInstancedBaseInstance);
            DISPATCH(DrawIndexedBaseVertexInstancedBaseInstance, _drawIndexedBaseVertexInstancedBaseInstance, drawIndexedBaseVertexInstancedBaseInstance);
            DISPATCH(SetDrawType, _setDrawType, setDrawType);
            DISPATCH(SetPipelineState, _setPipelineState, setPipelineState);
            DISPATCH(SetStencilRef, _setStencilRef, setStencilRef);
            DISPATCH(SetVertexArray, _setVertexArray, setVertexArray);
            DISPATCH(SetIndexBuffer, _setIndexBuffer, setIndexBuffer);
            DISPATCH(SetGDescriptorLayout, _setGDescriptorLayout, setGDescriptorLayout);
            DISPATCH(SetGDescriptorTable, _setGDescriptorTable, setGDescriptorTable);
            default: break;
        }
#undef DISPATCH
    }
}

void GLCommandQueue::_draw(const GLCL::CommandDraw& cmd) noexcept
{
    glDrawArrays(_currentDrawType, cmd.startVertex, cmd.vertexCount);
}

void GLCommandQueue::_drawIndexed(const GLCL::CommandDrawIndexed& cmd) noexcept
{
    glDrawElements(_currentDrawType, cmd.indexCount, _currentIndexSize, cmd.indexOffset);
}

void GLCommandQueue::_drawIndexedBaseVertex(const GLCL::CommandDrawIndexedBaseVertex& cmd) noexcept
{
    glDrawElementsBaseVertex(_currentDrawType, cmd.indexCount, _currentIndexSize, cmd.indexOffset, cmd.baseVertex);
}

void GLCommandQueue::_drawInstanced(const GLCL::CommandDrawInstanced& cmd) noexcept
{
    glDrawArraysInstanced(_currentDrawType, cmd.startVertex, cmd.vertexCount, cmd.instanceCount);
}

void GLCommandQueue::_drawInstancedBaseInstance(const GLCL::CommandDrawInstancedBaseInstance& cmd) noexcept
{
    glDrawArraysInstancedBaseInstance(_currentDrawType, cmd.startVertex, cmd.vertexCount, cmd.instanceCount, cmd.baseInstance);
}

void GLCommandQueue::_drawIndexedInstanced(const GLCL::CommandDrawIndexedInstanced& cmd) noexcept
{
    glDrawElementsInstanced(_currentDrawType, cmd.indexCount, _currentIndexSize, cmd.indexOffset, cmd.instanceCount);
}

void GLCommandQueue::_drawIndexedBaseVertexInstanced(const GLCL::CommandDrawIndexedBaseVertexInstanced& cmd) noexcept
{
    glDrawElementsInstancedBaseVertex(_currentDrawType, cmd.indexCount, _currentIndexSize, cmd.indexOffset, cmd.instanceCount, cmd.baseVertex);
}

void GLCommandQueue::_drawIndexedInstancedBaseInstance(const GLCL::CommandDrawIndexedInstancedBaseInstance& cmd) noexcept
{
    glDrawElementsInstancedBaseInstance(_currentDrawType, cmd.indexCount, _currentIndexSize, cmd.indexOffset, cmd.instanceCount, cmd.baseInstance);
}

void GLCommandQueue::_drawIndexedBaseVertexInstancedBaseInstance(const GLCL::CommandDrawIndexedBaseVertexInstancedBaseInstance& cmd) noexcept
{
    glDrawElementsInstancedBaseVertexBaseInstance(_currentDrawType, cmd.indexCount, _currentIndexSize, cmd.indexOffset, cmd.instanceCount, cmd.baseVertex, cmd.baseInstance);
}

void GLCommandQueue::_setDrawType(const GLCL::CommandSetDrawType& cmd) noexcept
{
    _currentDrawType = cmd.glDrawType;
}

void GLCommandQueue::_setPipelineState(const GLCL::CommandSetPipelineState& cmd) noexcept
{
    _currentPipelineState = cmd.pipelineState;

    const GLBlendingState* const blendingState = RTT_CAST(_currentPipelineState->args().blendingState.get(), GLBlendingState);
    blendingState->apply(_glStateManager);

    const GLDepthStencilState* const depthStencilState = RTT_CAST(_currentPipelineState->args().depthStencilState.get(), GLDepthStencilState);
    depthStencilState->apply(_glStateManager);

    const GLRasterizerState* const rasterizerState = RTT_CAST(_currentPipelineState->args().rasterizerState.get(), GLRasterizerState);
    rasterizerState->apply(_glStateManager);

    const GLShaderProgram* shaderProgram = _currentPipelineState->args().shaderProgram.get<GLShaderProgram>();
    _glStateManager.bindShaderProgram(shaderProgram->programHandle());
}

void GLCommandQueue::_setStencilRef(const GLCL::CommandSetStencilRef& cmd) noexcept
{
    _glStateManager.stencilRef(cmd.stencilRef);
}

void GLCommandQueue::_setVertexArray(const GLCL::CommandSetVertexArray& cmd) noexcept
{
    _glStateManager.bindVertexArray(cmd.vao);
}

void GLCommandQueue::_setIndexBuffer(const GLCL::CommandSetIndexBuffer& cmd) noexcept
{
    _glStateManager.bindElementArrayBuffer(cmd.ibo);
    _currentIndexSize = cmd.indexSize;
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
