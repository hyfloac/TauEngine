#pragma once

#include "graphics/CommandQueue.hpp"
#include "GLCommandList.hpp"
#include "GLDescriptorLayout.hpp"
#include "GLStateManager.hpp"

struct PipelineState;

class TAU_DLL GLCommandQueue final : public ICommandQueue
{
    DEFAULT_CONSTRUCT_PU(GLCommandQueue);
    DEFAULT_DESTRUCT(GLCommandQueue);
    DELETE_CM(GLCommandQueue);
private:
    const PipelineState* _currentPipelineState;
    const GLDescriptorLayout* _currentLayout;
    GLStateManager _glStateManager;
    GLenum _currentDrawType;
    GLenum _currentIndexSize;
public:
    void executeCommandLists(uSys count, const ICommandList* const * lists) noexcept override;

    void executeCommandList(const ICommandList* list) noexcept;
private:
    void _draw(const GLCL::CommandDraw& cmd) noexcept;
    void _drawIndexed(const GLCL::CommandDrawIndexed& cmd) noexcept;
    void _drawIndexedBaseVertex(const GLCL::CommandDrawIndexedBaseVertex& cmd) noexcept;
    void _drawInstanced(const GLCL::CommandDrawInstanced& cmd) noexcept;
    void _drawInstancedBaseInstance(const GLCL::CommandDrawInstancedBaseInstance& cmd) noexcept;
    void _drawIndexedInstanced(const GLCL::CommandDrawIndexedInstanced& cmd) noexcept;
    void _drawIndexedBaseVertexInstanced(const GLCL::CommandDrawIndexedBaseVertexInstanced& cmd) noexcept;
    void _drawIndexedInstancedBaseInstance(const GLCL::CommandDrawIndexedInstancedBaseInstance& cmd) noexcept;
    void _drawIndexedBaseVertexInstancedBaseInstance(const GLCL::CommandDrawIndexedBaseVertexInstancedBaseInstance& cmd) noexcept;
    void _setDrawType(const GLCL::CommandSetDrawType& cmd) noexcept;
    void _setPipelineState(const GLCL::CommandSetPipelineState& cmd) noexcept;
    void _setStencilRef(const GLCL::CommandSetStencilRef& cmd) noexcept;
    void _setVertexArray(const GLCL::CommandSetVertexArray& cmd) noexcept;
    void _setIndexBuffer(const GLCL::CommandSetIndexBuffer& cmd) noexcept;

    void _setGDescriptorLayout(const GLCL::CommandSetGDescriptorLayout& cmd) noexcept;
    void _setGDescriptorTable(const GLCL::CommandSetGDescriptorTable& cmd) noexcept;

    void _executeBundle(const GLCL::CommandExecuteBundle& cmd) noexcept;
};
