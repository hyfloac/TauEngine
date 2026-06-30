/**
 * @file
 */
#pragma once

#include <Objects.hpp>
#include "DeviceChild.hpp"
#include "GraphicsEnums.hpp"

namespace tau::graphics {

class ICommandList : public IDeviceContextChild
{
    DEFAULT_CONSTRUCT_PO(ICommandList);
    DEFAULT_DESTRUCT_VI(ICommandList);
    DEFAULT_CM_PO(ICommandList);
public:
    struct ConstructionInfo final : com::BaseConstructionInfo
    {
        DEFAULT_CONSTRUCT_PU(ConstructionInfo);
        DEFAULT_DESTRUCT_O(ConstructionInfo);
        DEFAULT_CM_PU(ConstructionInfo);
    public:
        // ICommandAllocator* CommandAllocator;
        // IPipelineState* InitialState;
        CommandListType Type;
    };
public:
    /**
     * @brief Resets the command list to the initial state to begin
     * recording again.
     *
     * @param[in] initialState
     *        An optional initial pipeline state to use for the
     *      command list.
     */
    // virtual void Reset(const IPipelineState* initialState) noexcept = 0;

    /**
     * @brief Begins recording the command list.
     */
    virtual void Begin() noexcept = 0;

    /**
     * @brief Finishes recording the command list.
     */
    virtual void Finish() noexcept = 0;

    /**
     * @brief Issues a draw command.
     *
     *   This will assemble primitives by reading off vertices from
     * the vertex buffers in sequential order.
     *
     * @param[in] vertexCount
     *        The number of vertices that need to be drawn.
     * @param[in] startVertex
     *        The offset of the first vertex to be used by the
     *      input assembler.
     */
    virtual void Draw(uSys vertexCount, uSys startVertex) noexcept = 0;

    /**
     * @brief Issues a draw command.
     *
     *   This will assemble primitives by reading off indices from
     * the index buffer and selecting the corresponding vertices
     * from the vertex buffers.
     *
     * @param[in] indexCount
     *        The number of indices that need to be drawn.
     * @param[in] startIndex
     *        The offset of the first index to be used by the input
     *      assembler.
     * @param[in] baseVertex
     *        An offset added to each index.
     */
    virtual void DrawIndexed(uSys indexCount, uSys startIndex, iSys baseVertex) noexcept = 0;

    /**
     * @brief Issues a draw command with instanced rendering.
     *
     *   This will assemble primitives by reading off vertices from
     * the vertex buffers in sequential order.
     *
     *   This also uses and additional buffers for per instance
     * rendering.
     *
     * @param[in] vertexCount
     *        The number of vertices that need to be drawn.
     * @param[in] startVertex
     *        The offset of the first vertex to be used by the
     *      input assembler.
     * @param[in] instanceCount
     *      The number of instances to render.
     * @param[in] startInstance
     *        The offset of the first instance to be used by the
     *      input assembler.
     */
    virtual void DrawInstanced(uSys vertexCount, uSys startVertex, uSys instanceCount, uSys startInstance) noexcept = 0;

    /**
     * @brief Issues a draw command with instanced rendering.
     *
     *   This will assemble primitives by reading off indices from
     * the index buffer and selecting the corresponding vertices
     * from the vertex buffers.
     *
     *   This also uses and additional buffers for per instance
     * rendering.
     *
     * @param[in] indexCount
     *        The number of indices that need to be drawn.
     * @param[in] startIndex
     *        The offset of the first index to be used by the input
     *      assembler.
     * @param[in] baseVertex
     *        An offset added to each index.
     * @param[in] instanceCount
     *      The number of instances to render.
     * @param[in] startInstance
     *        The offset of the first instance to be used by the
     *      input assembler.
     */
    virtual void DrawIndexedInstanced(uSys indexCount, uSys startIndex, iSys baseVertex, uSys instanceCount, uSys startInstance) noexcept = 0;

    /**
     * @brief Sets the draw type.
     *
     *   This is used to control how the list of vertices and
     * indices should be interpreted.
     *
     * @param[in] drawType
     *      The method of interpreting the vertices and indices.
     */
    virtual void SetDrawType(DrawType drawType) noexcept = 0;

};

}

TAU_DECL_UUID(::tau::graphics::ICommandList, 0xEF0AB0661CF14E7Eull, 0xBB443544195033CAull);
