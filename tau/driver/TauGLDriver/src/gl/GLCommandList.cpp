/**
 * @file
 */
#include "gl/GLCommandList.hpp"

#include <graphics/driver/GraphicsDriver.hpp>

#include "Logging.hpp"

namespace tau::graphics::driver::gl {

enum class ECommand : u32
{
    Nop = 0,
    Draw,
    DrawIndexed,
    DrawInstanced,
    DrawIndexedInstanced,
    SetDrawTypePoints,
    SetDrawTypeLines,
    SetDrawTypeLineStrip,
    SetDrawTypeLinesAdjacency,
    SetDrawTypeLineStripAdjacency,
    SetDrawTypeTriangles,
    SetDrawTypeTriangleStrip,
    SetDrawTypeTrianglesAdjacency,
    SetDrawTypeTriangleStripAdjacency,
    SetDrawTypeTriangleFan,
    SetDrawTypePatches,
};

GLCommandList::GLCommandList(
    const EngineCommandListHandle engine,
    const CommandListType type
) noexcept
    : m_Engine(engine)
    , m_Type(type)
{ }

void GLCommandList::Begin() noexcept
{
    m_PushBuffer.Reset();
}

void GLCommandList::End() noexcept
{
}

void GLCommandList::Draw(const uSys vertexCount, const uSys startVertex) noexcept
{
    m_PushBuffer.PushT(ECommand::Draw, static_cast<u32>(vertexCount), static_cast<u32>(startVertex));
}

void GLCommandList::DrawIndexed(uSys indexCount, uSys startIndex, iSys baseVertex) noexcept
{
    m_PushBuffer.PushT(ECommand::DrawIndexed, static_cast<u32>(indexCount), static_cast<u32>(startIndex), static_cast<i32>(baseVertex));
}

void GLCommandList::DrawInstanced(uSys vertexCount, uSys startVertex, uSys instanceCount, uSys startInstance) noexcept
{
    m_PushBuffer.PushT(ECommand::DrawInstanced, static_cast<u32>(vertexCount), static_cast<u32>(startVertex), static_cast<u32>(instanceCount));
    m_PushBuffer.PushT(static_cast<u32>(startInstance));
}

void GLCommandList::DrawIndexedInstanced(
    uSys indexCount,
    uSys startIndex,
    iSys baseVertex,
    uSys instanceCount,
    uSys startInstance
) noexcept
{
    m_PushBuffer.PushT(ECommand::DrawIndexedInstanced, static_cast<u32>(indexCount), static_cast<u32>(startIndex), static_cast<i32>(baseVertex));
    m_PushBuffer.PushT(static_cast<u32>(instanceCount), static_cast<u32>(startInstance));
}

void GLCommandList::SetDrawType(DrawType drawType) noexcept
{
    ECommand command = ECommand::Nop;
    switch(drawType)
    {
        case DrawType::Points: command = ECommand::SetDrawTypePoints; break;
        case DrawType::Lines: command = ECommand::SetDrawTypeLines; break;
        case DrawType::LineStrip: command = ECommand::SetDrawTypeLineStrip; break;
        case DrawType::LinesAdjacency: command = ECommand::SetDrawTypeLinesAdjacency; break;
        case DrawType::LineStripAdjacency: command = ECommand::SetDrawTypeLineStripAdjacency; break;
        case DrawType::Triangles: command = ECommand::SetDrawTypeTriangles; break;
        case DrawType::TriangleStrip: command = ECommand::SetDrawTypeTriangleStrip; break;
        case DrawType::TrianglesAdjacency: command = ECommand::SetDrawTypeTrianglesAdjacency; break;
        case DrawType::TriangleStripAdjacency: command = ECommand::SetDrawTypeTriangleStripAdjacency; break;
        case DrawType::TriangleFan: command = ECommand::SetDrawTypeTriangleFan; break;
        case DrawType::Patches: command = ECommand::SetDrawTypePatches; break;
        default:
            LOG_WARN("Unknown draw type 0x{X}", static_cast<u32>(drawType));
            break;
    }

    m_PushBuffer.PushT(command);
}

void GLBeginCommandList(CommandListHandle commandList)
{
}

void GLEndCommandList(CommandListHandle commandList)
{
}

void GLDraw(CommandListHandle commandList, uSys vertexCount, uSys startVertex)
{
}

void GLDrawIndexed(CommandListHandle commandList, uSys vertexCount, uSys startVertex, iSys baseVertex)
{
}

void GLDrawInstanced(CommandListHandle commandList, uSys vertexCount, uSys startVertex, uSys instanceCount,
    uSys startInstance)
{
}

void GLDrawIndexedInstanced(CommandListHandle commandList, uSys indexCount, uSys startIndex, iSys baseVertex,
    uSys instanceCount, uSys startInstance)
{
}

void GLSetDrawType(CommandListHandle commandList, DrawType drawType)
{
}
}
