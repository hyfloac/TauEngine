/**
 * @file
 */
#pragma once

#include <NumTypes.hpp>
#include <Objects.hpp>
#include <TauCOM.hpp>
#include "graphics/GraphicsEnums.hpp"

namespace tau::graphics::driver {

DECL_OPAQUE_TYPE(EngineCommandListHandle);
DECL_OPAQUE_TYPE(CommandListHandle);

typedef void BeginCommandList_f(CommandListHandle commandList);
typedef void EndCommandList_f(CommandListHandle commandList);
typedef void Draw_f(CommandListHandle commandList, uSys vertexCount, uSys startVertex);
typedef void DrawIndexed_f(CommandListHandle commandList, uSys indexCount, uSys startIndex, iSys baseVertex);
typedef void DrawInstanced_f(CommandListHandle commandList, uSys vertexCount, uSys startVertex, uSys instanceCount, uSys startInstance);
typedef void DrawIndexedInstanced_f(CommandListHandle commandList, uSys indexCount, uSys startIndex, iSys baseVertex, uSys instanceCount, uSys startInstance);
typedef void SetDrawType_f(CommandListHandle commandList, DrawType drawType);

struct CommandListFuncs final
{
    BeginCommandList_f* Begin;
    EndCommandList_f* End;
    Draw_f* Draw;
    DrawIndexed_f* DrawIndexed;
    DrawInstanced_f* DrawInstanced;
    DrawIndexedInstanced_f* DrawIndexedInstanced;
    SetDrawType_f* SetDrawType;
};

struct CreateCommandListArgs final
{
    EngineCommandListHandle EngineCommandList;
    CommandListHandle CommandList;
    CommandListType Type;
};

}
