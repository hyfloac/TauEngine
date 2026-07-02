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

typedef void Dispatch_f(CommandListHandle commandList, uSys threadGroupCountX, uSys threadGroupCountY, uSys threadGroupCountZ);

typedef void SetDrawType_f(CommandListHandle commandList, DrawType drawType);

typedef void SetBlendFactor_f(CommandListHandle commandList, const float blendFactor[4]);
typedef void SetStencilRef_f(CommandListHandle commandList, u32 stencilRef);

typedef void ExecuteCommandList_f(CommandListHandle commandList, CommandListHandle subCommandList);

struct CommandListFuncs final
{
    BeginCommandList_f* Begin;
    EndCommandList_f* End;

    Draw_f* Draw;
    DrawIndexed_f* DrawIndexed;
    DrawInstanced_f* DrawInstanced;
    DrawIndexedInstanced_f* DrawIndexedInstanced;

    Dispatch_f* Dispatch;

    SetDrawType_f* SetDrawType;

    SetBlendFactor_f* SetBlendFactor;
    SetStencilRef_f* SetStencilRef;

    ExecuteCommandList_f* ExecuteCommandList;
};

struct CreateCommandListArgs final
{
    EngineCommandListHandle EngineCommandList;
    CommandListHandle CommandList;
    CommandListType Type;
};

}
