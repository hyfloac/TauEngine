#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <Safeties.hpp>
#include <RunTimeType.hpp>

#include "DLL.hpp"
#include "_GraphicsOpaqueObjects.hpp"

struct PipelineState;
class IInputLayout;
class IVertexArray;

#define COMMAND_LIST_IMPL_BASE(_TYPE) \
    RTT_IMPL(_TYPE, ICommandList)

#define COMMAND_LIST_IMPL(_TYPE) COMMAND_LIST_IMPL_BASE(_TYPE)

class TAU_DLL TAU_NOVTABLE ICommandList
{
    DEFAULT_CONSTRUCT_PO(ICommandList);
    DEFAULT_DESTRUCT_VI(ICommandList);
    DEFAULT_CM_PO(ICommandList);
public:
    /**
     *   Resets the command list to the initial state to be
     * recording again.
     */
    virtual void reset() noexcept = 0;

    /**
     * Finishes recording the command list.
     */
    virtual void finish() noexcept = 0;

    /**
     * Issues a draw command.
     *
     *   This will automatically choose to draw with indexing
     * depending on whether or not an index buffer is bound within
     * the vertex array.
     *
     * @param[in] exCount
     *        The number of indices or vertices that need to be
     *      drawn.
     * @param[in] startIndex
     *        The offset of the first index to be used by the input
     *      assembler.
     * @param[in] startVertex
     *        The offset of the first vertex to be used by the
     *      input assembler.
     */
    virtual void draw(uSys exCount, uSys startIndex, uSys startVertex) noexcept = 0;

    /**
     * Issues a draw command with instanced rendering.
     *
     *   This will automatically choose to draw with indexing
     * depending on whether or not an index buffer is bound within
     * the vertex array.
     *
     * @param[in] exCount
     *        The number of indices or vertices that need to be
     *      drawn.
     * @param[in] startIndex
     *        The offset of the first index to be used by the input
     *      assembler.
     * @param[in] startVertex
     *        The offset of the first vertex to be used by the
     *      input assembler.
     * @param[in] instanceCount
     *      The number of instances to render.
     * @param[in] startInstance
     *        The offset of the first instance to be used by the
     *      input assembler.
     */
    virtual void drawInstanced(uSys exCount, uSys startIndex, uSys startVertex, uSys instanceCount, uSys startInstance) noexcept = 0;

    /**
     * Sets the pipeline state.
     *
     *   This contains all of the general purpose information
     * needed by the rendering pipeline. This is a homogenous
     * object of what used to be many individual state objects.
     * This also includes things like the input layout, and the
     * shader objects.
     *
     * @param[in] pipelineState
     *      The homogenous pipeline state to use.
     */
    virtual void setPipelineState(const PipelineState& pipelineState) noexcept = 0;

    /**
     * Sets the stencil reference target.
     *
     *   The stencil reference is used during the rendering to
     * determine how to respond to a specific stencil value. This
     * is used in combination with the values in the
     * DepthStencilState.
     *
     *   The classic example would be to stencil out a shape with
     * value `1` and leave everything else as `0`. Then you can
     * setup the stencil state to render only where the stencil
     * value is equal to the stencil ref. Then you can set the
     * stencil ref separately and do one render pass for stencil
     * values `1` and a separate render pass for stencil values `0`
     * without having to build multiple stencil states.
     *
     * @param[in] stencilRef
     *      The stencil reference value.
     */
    virtual void setStencilRef(uSys stencilRef) noexcept = 0;

    /**
     * Sets the vertex array to render from.
     *
     * @param[in] va
     *      The vertex array to use.
     */
    virtual void setVertexArray(const IVertexArray& va) noexcept = 0;

    /**
     * Sets the descriptor layout to use within the shader pipeline.
     *
     * @param[in] layout
     *      The layout of descriptors.
     */
    virtual void setGraphicsDescriptorLayout(DescriptorLayout layout) noexcept = 0;

    /**
     * Sets a descriptor table to use within the shader pipeline.
     *
     * @param[in] index
     *      The descriptor layout index.
     * @param[in] table
     *      The table to set.
     */
    virtual void setGraphicsDescriptorTable(uSys index, DescriptorTable table) noexcept = 0;

    /**
     *   Sets a descriptor table of samplers to use within the
     * shader pipeline.
     *
     * @param[in] index
     *      The descriptor layout index.
     * @param[in] table
     *      The sampler table to set.
     */
    virtual void setGraphicsDescriptorTable(uSys index, DescriptorSamplerTable table) noexcept = 0;

    RTT_BASE_IMPL(ICommandList);
    RTT_BASE_CHECK(ICommandList);
    RTT_BASE_CAST(ICommandList);
};

struct CommandListArgs final
{
    
};

class TAU_DLL TAU_NOVTABLE ICommandListBuilder
{
    DEFAULT_CONSTRUCT_PO(ICommandListBuilder);
    DEFAULT_DESTRUCT_VI(ICommandListBuilder);
    DEFAULT_CM_PO(ICommandListBuilder);
public:
    enum class Error
    {
        NoError = 0,
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure,
        InternalError
    };
public:
    [[nodiscard]] virtual IVertexArray* build(const CommandListArgs& args, [[tau::out]] Error* error) noexcept = 0;
    [[nodiscard]] virtual IVertexArray* build(const CommandListArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept = 0;
    [[nodiscard]] virtual CPPRef<IVertexArray> buildCPPRef(const CommandListArgs& args, [[tau::out]] Error* error) noexcept = 0;
    [[nodiscard]] virtual NullableRef<IVertexArray> buildTauRef(const CommandListArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IVertexArray> buildTauSRef(const CommandListArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept = 0;
};
