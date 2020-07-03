#pragma once

#include "graphics/CommandQueue.hpp"
#include "GLCommandList.hpp"
#include "gl/GLDescriptorLayout.hpp"

class TAU_DLL GLCommandQueue final : public ICommandQueue
{
    DEFAULT_CONSTRUCT_PU(GLCommandQueue);
    DEFAULT_DESTRUCT(GLCommandQueue);
    DEFAULT_CM_PU(GLCommandQueue);
private:
    const GLDescriptorLayout* _currentLayout;
public:
    void executeCommandLists(uSys count, const ICommandList** lists) noexcept override;

    void executeCommandList(const ICommandList* list) noexcept;
private:
    void _draw(const GLCL::CommandDraw& cmd) noexcept;
    void _drawIndexed(const GLCL::CommandDrawIndexed& cmd) noexcept;
    void _drawInstanced(const GLCL::CommandDrawInstanced& cmd) noexcept;
    void _drawIndexedInstanced(const GLCL::CommandDrawIndexedInstanced& cmd) noexcept;
    void _setVertexArray(const GLCL::CommandSetVertexArray& cmd) noexcept;
    void _setIndexBuffer(const GLCL::CommandSetIndexBuffer& cmd) noexcept;

    void _setGDescriptorLayout(const GLCL::CommandSetGDescriptorLayout& cmd) noexcept;
    void _setGDescriptorTable(const GLCL::CommandSetGDescriptorTable& cmd) noexcept;
};
