/**
 * @file
 */
#pragma once

#include <graphics/driver/GraphicsDriver.hpp>
#include <PushBuffer.hpp>

#include "GLHeader.hpp"

namespace tau::graphics::driver::gl {

class GLCommandList final
{
public:
    GLCommandList(
        EngineCommandListHandle engine,
        CommandListType type
    ) noexcept;

    void Begin() noexcept;
    void End() noexcept;

    void Draw(uSys vertexCount, uSys startVertex) noexcept;
    void DrawIndexed(uSys indexCount, uSys startIndex, iSys baseVertex) noexcept;
    void DrawInstanced(uSys vertexCount, uSys startVertex, uSys instanceCount, uSys startInstance) noexcept;
    void DrawIndexedInstanced(uSys indexCount, uSys startIndex, iSys baseVertex, uSys instanceCount, uSys startInstance) noexcept;

    void Dispatch(uSys threadGroupCountX, uSys threadGroupCountY, uSys threadGroupCountZ) noexcept;

    void SetDrawType(DrawType drawType) noexcept;

    void SetBlendFactor(const float blendFactor[4]) noexcept;
    void SetStencilRef(u32 stencilRef) noexcept;

    void ExecuteCommandList(CommandListHandle subCommandList) noexcept;
private:
    GLDriverHeader m_Magic;
    EngineCommandListHandle m_Engine;
    CommandListType m_Type;
    PushBuffer m_PushBuffer;
};

void GLBeginCommandList(CommandListHandle commandList);
void GLEndCommandList(CommandListHandle commandList);

void GLDraw(CommandListHandle commandList, uSys vertexCount, uSys startVertex);
void GLDrawIndexed(CommandListHandle commandList, uSys indexCount, uSys startIndex, iSys baseVertex);
void GLDrawInstanced(CommandListHandle commandList, uSys vertexCount, uSys startVertex, uSys instanceCount, uSys startInstance);
void GLDrawIndexedInstanced(CommandListHandle commandList, uSys indexCount, uSys startIndex, iSys baseVertex, uSys instanceCount, uSys startInstance);

void GLDispatch(CommandListHandle commandList, uSys threadGroupCountX, uSys threadGroupCountY, uSys threadGroupCountZ);

void GLSetDrawType(CommandListHandle commandList, DrawType drawType);

void GLSetBlendFactor(CommandListHandle commandList, const float blendFactor[4]);
void GLSetStencilRef(CommandListHandle commandList, u32 stencilRef);

void GLExecuteCommandList(CommandListHandle commandList, CommandListHandle subCommandList);

}
