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

    void SetDrawType(DrawType drawType) noexcept;
private:
    GLDriverHeader m_Magic;
    EngineCommandListHandle m_Engine;
    CommandListType m_Type;
    PushBuffer m_PushBuffer;
};

void GLBeginCommandList(CommandListHandle commandList);
void GLEndCommandList(CommandListHandle commandList);
void GLDraw(CommandListHandle commandList, uSys vertexCount, uSys startVertex);
void GLDrawIndexed(CommandListHandle commandList, uSys vertexCount, uSys startVertex, iSys baseVertex);
void GLDrawInstanced(CommandListHandle commandList, uSys vertexCount, uSys startVertex, uSys instanceCount, uSys startInstance);
void GLDrawIndexedInstanced(CommandListHandle commandList, uSys indexCount, uSys startIndex, iSys baseVertex, uSys instanceCount, uSys startInstance);
void GLSetDrawType(CommandListHandle commandList, DrawType drawType);

}
