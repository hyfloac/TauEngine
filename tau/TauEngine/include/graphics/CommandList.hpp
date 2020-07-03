#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <Safeties.hpp>

#include "DLL.hpp"

class IInputLayout;
class IVertexArray;
struct DescriptorLayout;
struct DescriptorTable;
struct DescriptorSamplerTable;

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
     * Sets the input layout to use for decoding the vertex array.
     *
     * @param[in] layout
     *      The input layout to use.
     */
    virtual void setInputLayout(const IInputLayout& layout) noexcept = 0;

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
    virtual void setGraphicsDescriptorLayout(const DescriptorLayout& layout) noexcept = 0;

    /**
     * Sets a constant to use within the shader pipeline.
     *
     * @param[in] index
     *      The descriptor layout index.
     * @param[in] offset
     *      The offset within group of constants.
     * @param[in] value
     *      The value to set.
     */
    virtual void setGraphicsConstant(uSys index, uSys offset, u32 value) noexcept = 0;

    /**
     * Sets a a series constants to use within the shader pipeline.
     *
     * @param[in] index
     *      The descriptor layout index.
     * @param[in] offset
     *      The offset within group of constants.
     * @param[in] count
     *      The number of values in the buffer.
     * @param[in] values
     *      The buffer of values to set.
     */
    virtual void setGraphicsConstants(uSys index, uSys offset, uSys count, const u32* values) noexcept = 0;

    /**
     * Sets a descriptor table to use within the shader pipeline.
     *
     * @param[in] index
     *      The descriptor layout index.
     * @param[in] table
     *      The table to set.
     */
    virtual void setGraphicsDescriptorTable(uSys index, const DescriptorTable& table) noexcept = 0;

    /**
     *   Sets a descriptor table of samplers to use within the
     * shader pipeline.
     *
     * @param[in] index
     *      The descriptor layout index.
     * @param[in] table
     *      The sampler table to set.
     */
    virtual void setGraphicsDescriptorTable(uSys index, const DescriptorSamplerTable& table) noexcept = 0;
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
