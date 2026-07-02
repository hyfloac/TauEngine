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
    Dispatch,
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
    SetBlendFactor,
    SetStencilRef,
    ExecuteCommandList,
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
    const uSys indexCount,
    const uSys startIndex,
    const iSys baseVertex,
    const uSys instanceCount,
    const uSys startInstance
) noexcept
{
    m_PushBuffer.PushT(ECommand::DrawIndexedInstanced, static_cast<u32>(indexCount), static_cast<u32>(startIndex), static_cast<i32>(baseVertex));
    m_PushBuffer.PushT(static_cast<u32>(instanceCount), static_cast<u32>(startInstance));
}

void GLCommandList::Dispatch(
    const uSys threadGroupCountX,
    const uSys threadGroupCountY,
    const uSys threadGroupCountZ
) noexcept
{
    m_PushBuffer.PushT(ECommand::Dispatch, static_cast<u32>(threadGroupCountX), static_cast<u32>(threadGroupCountY), static_cast<u32>(threadGroupCountZ));
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

void GLCommandList::SetBlendFactor(const float blendFactor[4]) noexcept
{
    m_PushBuffer.PushT(ECommand::SetBlendFactor, blendFactor[0], blendFactor[1], blendFactor[2]);
    m_PushBuffer.PushT(blendFactor[3]);
}

void GLCommandList::SetStencilRef(const u32 stencilRef) noexcept
{
    m_PushBuffer.PushT(ECommand::SetStencilRef, stencilRef);
}

void GLCommandList::ExecuteCommandList(CommandListHandle subCommandList) noexcept
{

    if(!subCommandList.raw)
    {
        LOG_ERROR(u8"subCommandList was not set.");
        return;
    }

    if(subCommandList.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        LOG_ERROR(u8"Sub Command List header did not match TaGL.");
        return;
    }

    m_PushBuffer.PushT(ECommand::ExecuteCommandList, subCommandList.raw);
}

void GLBeginCommandList(CommandListHandle commandList)
{
    TRACE_ENTRYPOINT();

    if(!commandList.raw)
    {
        LOG_ERROR(u8"commandList was not set.");
        return;
    }

    if(commandList.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        LOG_ERROR(u8"Command List header did not match TaGL.");
        return;
    }

    return commandList.get<GLCommandList>()->Begin();
}

void GLEndCommandList(CommandListHandle commandList)
{
    TRACE_ENTRYPOINT();

    if(!commandList.raw)
    {
        LOG_ERROR(u8"commandList was not set.");
        return;
    }

    if(commandList.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        LOG_ERROR(u8"Command List header did not match TaGL.");
        return;
    }

    return commandList.get<GLCommandList>()->End();
}

void GLDraw(
    CommandListHandle commandList,
    const uSys vertexCount,
    const uSys startVertex
)
{
    TRACE_ENTRYPOINT_ARG(
        u8"vertexCount: {}, startVertex: {}",
        vertexCount,
        startVertex
    );

    if(!commandList.raw)
    {
        LOG_ERROR(u8"commandList was not set.");
        return;
    }

    if(commandList.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        LOG_ERROR(u8"Command List header did not match TaGL.");
        return;
    }

    return commandList.get<GLCommandList>()->Draw(
        vertexCount,
        startVertex
    );
}

void GLDrawIndexed(
    CommandListHandle commandList,
    const uSys indexCount,
    const uSys startIndex,
    const iSys baseVertex
)
{
    TRACE_ENTRYPOINT_ARG(
        u8"indexCount: {}, startIndex: {}, baseVertex: {}",
        indexCount,
        startIndex,
        baseVertex
    );

    if(!commandList.raw)
    {
        LOG_ERROR(u8"commandList was not set.");
        return;
    }

    if(commandList.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        LOG_ERROR(u8"Command List header did not match TaGL.");
        return;
    }

    return commandList.get<GLCommandList>()->DrawIndexed(
        indexCount,
        startIndex,
        baseVertex
    );
}

void GLDrawInstanced(
    CommandListHandle commandList,
    const uSys vertexCount,
    const uSys startVertex,
    const uSys instanceCount,
    const uSys startInstance
)
{
    TRACE_ENTRYPOINT_ARG(
        u8"vertexCount: {}, startVertex: {}, instanceCount: {}, startInstance: {}",
        vertexCount,
        startVertex,
        instanceCount,
        startInstance
    );

    if(!commandList.raw)
    {
        LOG_ERROR(u8"commandList was not set.");
        return;
    }

    if(commandList.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        LOG_ERROR(u8"Command List header did not match TaGL.");
        return;
    }

    return commandList.get<GLCommandList>()->DrawInstanced(
        vertexCount,
        startVertex,
        instanceCount,
        startInstance
    );
}

void GLDrawIndexedInstanced(
    CommandListHandle commandList,
    const uSys indexCount,
    const uSys startIndex,
    const iSys baseVertex,
    const uSys instanceCount,
    const uSys startInstance
)
{
    TRACE_ENTRYPOINT_ARG(
        u8"indexCount: {}, startIndex: {}, baseVertex: {}, instanceCount: {}, startInstance: {}",
        indexCount,
        startIndex,
        baseVertex,
        instanceCount,
        startInstance
    );

    if(!commandList.raw)
    {
        LOG_ERROR(u8"commandList was not set.");
        return;
    }

    if(commandList.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        LOG_ERROR(u8"Command List header did not match TaGL.");
        return;
    }

    return commandList.get<GLCommandList>()->DrawIndexedInstanced(
        indexCount,
        startIndex,
        baseVertex,
        instanceCount,
        startInstance
    );
}

void GLDispatch(
    CommandListHandle commandList,
    const uSys threadGroupCountX,
    const uSys threadGroupCountY,
    const uSys threadGroupCountZ
)
{
    TRACE_ENTRYPOINT_ARG(
        u8"threadGroupCountX: 0x{X}, threadGroupCountY: 0x{X}, threadGroupCountZ: 0x{X}",
        threadGroupCountX,
        threadGroupCountY,
        threadGroupCountZ
    );

    if(!commandList.raw)
    {
        LOG_ERROR(u8"commandList was not set.");
        return;
    }

    if(commandList.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        LOG_ERROR(u8"Command List header did not match TaGL.");
        return;
    }

    return commandList.get<GLCommandList>()->Dispatch(
        threadGroupCountX,
        threadGroupCountY,
        threadGroupCountZ
    );
}

void GLSetDrawType(
    CommandListHandle commandList,
    const DrawType drawType
)
{
    TRACE_ENTRYPOINT_ARG(
        u8"drawType: {}",
        static_cast<u32>(drawType)
    );

    if(!commandList.raw)
    {
        LOG_ERROR(u8"commandList was not set.");
        return;
    }

    if(commandList.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        LOG_ERROR(u8"Command List header did not match TaGL.");
        return;
    }

    return commandList.get<GLCommandList>()->SetDrawType(drawType);
}

void GLSetBlendFactor(CommandListHandle commandList, const float blendFactor[4])
{
    TRACE_ENTRYPOINT_ARG(
        u8"blendFactor[0]: {}, blendFactor[1]: {}, blendFactor[2]: {}, blendFactor[3]: {}",
        blendFactor[0],
        blendFactor[1],
        blendFactor[2],
        blendFactor[3]
    );

    if(!commandList.raw)
    {
        LOG_ERROR(u8"commandList was not set.");
        return;
    }

    if(commandList.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        LOG_ERROR(u8"Command List header did not match TaGL.");
        return;
    }

    return commandList.get<GLCommandList>()->SetBlendFactor(blendFactor);
}

void GLSetStencilRef(CommandListHandle commandList, const u32 stencilRef)
{
    TRACE_ENTRYPOINT_ARG(
        u8"stencilRef: {}",
        stencilRef
    );

    if(!commandList.raw)
    {
        LOG_ERROR(u8"commandList was not set.");
        return;
    }

    if(commandList.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        LOG_ERROR(u8"Command List header did not match TaGL.");
        return;
    }

    return commandList.get<GLCommandList>()->SetStencilRef(stencilRef);
}

void GLExecuteCommandList(CommandListHandle commandList, const CommandListHandle subCommandList)
{
    TRACE_ENTRYPOINT_ARG(
        u8"subCommandList: 0x{XP0}",
        subCommandList.raw
    );

    if(!commandList.raw)
    {
        LOG_ERROR(u8"commandList was not set.");
        return;
    }

    if(commandList.get<GLDriverHeader>()->Magic != GLDriverMagic)
    {
        LOG_ERROR(u8"Command List header did not match TaGL.");
        return;
    }

    return commandList.get<GLCommandList>()->ExecuteCommandList(subCommandList);
}

}
