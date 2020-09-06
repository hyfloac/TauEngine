#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <Safeties.hpp>
#include <RunTimeType.hpp>

#include "DLL.hpp"
#include "DescriptorHeap.hpp"
#include "_GraphicsOpaqueObjects.hpp"
#include "GraphicsEnums.hpp"
#include "texture/TextureEnums.hpp"

struct PipelineState;
class IInputLayout;
class IVertexArray;
struct IndexBufferView;
class ICommandAllocator;
class IResource;

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
     * @brief Resets the command list to the initial state to begin
     * recording again.
     *
     * @param[in] allocator
     *        The allocator that should be used to store all the
     *      commands.
     * @param[in] initialState
     *        An optional initial pipeline state to use for the
     *      command list.
     */
    virtual void reset(const NullableRef<ICommandAllocator>& allocator, PipelineState* initialState) noexcept = 0;

    /**
     * @brief Begins recording the command list.
     */
    virtual void begin() noexcept = 0;

    /**
     * @brief Finishes recording the command list.
     */
    virtual void finish() noexcept = 0;

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
    virtual void draw(uSys vertexCount, uSys startVertex) noexcept = 0;

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
    virtual void drawIndexed(uSys indexCount, uSys startIndex, iSys baseVertex) noexcept = 0;

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
    virtual void drawInstanced(uSys vertexCount, uSys startVertex, uSys instanceCount, uSys startInstance) noexcept = 0;

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
    virtual void drawIndexedInstanced(uSys indexCount, uSys startIndex, iSys baseVertex, uSys instanceCount, uSys startInstance) noexcept = 0;

    /**
     * @brief Sets the draw type.
     *
     *   This is used to control how the list of vertices and
     * indices should be interpreted.
     *
     * @param[in] drawType
     *      The method of interpreting the vertices and indices.
     */
    virtual void setDrawType(EGraphics::DrawType drawType) noexcept = 0;

    /**
     * @brief Sets the pipeline state.
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
     * @brief Sets the stencil reference target.
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
     * @brief Sets the vertex array to render from.
     *
     * @param[in] va
     *      The vertex array to use.
     */
    virtual void setVertexArray(const NullableRef<IVertexArray>& va) noexcept = 0;

    /**
     * @brief Sets the index buffer.
     *
     *   This is used to decode the buffers inside the vertex array
     * into triangles.
     *
     * @param[in] indexBufferView
     *        The view into the index buffer. This includes the
     *      buffer and the size of the indices.
     */
    virtual void setIndexBuffer(const IndexBufferView& indexBufferView) noexcept = 0;

    /**
     * @brief Sets the descriptor layout to use within the shader pipeline.
     *
     * @param[in] layout
     *      The layout of descriptors.
     */
    virtual void setGraphicsDescriptorLayout(DescriptorLayout layout) noexcept = 0;

    /**
     * @brief Sets a descriptor table to use within the shader pipeline.
     *
     * @param[in] index
     *      The descriptor layout index.
     * @param[in] type
     *      The type of descriptor stored in the table.
     * @param[in] descriptorCount
     *      The number of descriptors in the table.
     * @param[in] handle
     *      The address of the table to set.
     */
    virtual void setGraphicsDescriptorTable(uSys index, EGraphics::DescriptorType type, uSys descriptorCount, GPUDescriptorHandle handle) noexcept = 0;

    /**
     * @brief Executes a command list bundle.
     *
     *   This allows you to record a small set of command and reuse
     * them many times. The benefit of doing this is that you can
     * do all of the validation processing once, then every time
     * you execute a bundle you're simply copying the commands in.
     *
     * @param[in] bundle
     *      The command list bundle to execute.
     */
    virtual void executeBundle(const NullableRef<ICommandList>& bundle) noexcept = 0;

    /**
     * @brief Copies an entire resource.
     *
     *   Performs a memory copy for a resource on a GPU. This is
     * required to copy resources that are in VRAM, or to copy
     * resources that are in system memory to VRAM or vice versa.
     *
     * @param[in] dst
     *      The resource to write to.
     * @param[in] src
     *      The resource to copy.
     */
    virtual void copyResource(const NullableRef<IResource>& dst, const NullableRef<IResource>& src) noexcept = 0;

    /**
     * @brief Copies a portion of a buffer.
     *
     *   Performs a memory copy for a resource on a GPU. This is
     * required to copy resources that are in VRAM, or to copy
     * resources that are in system memory to VRAM or vice versa.
     *
     * @param[in] dstBuffer
     *      The buffer to write to.
     * @param[in] dstOffset
     *      The offset to write to in @p dstBuffer.
     * @param[in] srcBuffer
     *      The buffer to copy.
     * @param[in] srcOffset
     *      The offset to read from in @p srcBuffer.
     * @param[in] byteCount
     *      The number of bytes to copy.
     */
    virtual void copyBuffer(const NullableRef<IResource>& dstBuffer, u64 dstOffset, const NullableRef<IResource>& srcBuffer, u64 srcOffset, u64 byteCount) noexcept = 0;
    
    /**
     * @brief Copies a portion of a texture.
     *
     *   Performs a memory copy for a resource on a GPU. This is
     * required to copy resources that are in VRAM, or to copy
     * resources that are in system memory to VRAM or vice versa.
     *
     * Specifically used to copy a subresource of a texture.
     *
     * @param[in] dstTexture
     *      The texture to write to.
     * @param[in] dstSubResource
     *      The subresource to write to in @p dstTexture.
     * @param[in] srcTexture
     *      The texture to copy.
     * @param[in] srcSubResource
     *      The subresource to copy from @p srcTexture.
     */
    virtual void copyTexture(const NullableRef<IResource>& dstTexture, u32 dstSubResource, const NullableRef<IResource>& srcTexture, u32 srcSubResource) noexcept = 0;
    
    /**
     * @brief Copies a portion of a texture.
     *
     *   Performs a memory copy for a resource on a GPU. This is
     * required to copy resources that are in VRAM, or to copy
     * resources that are in system memory to VRAM or vice versa.
     *
     *   Specifically used to copy a portion of a subresource of a
     * texture. Use the optional @p srcBox to specify a region of the
     * source texture to copy. This should only use the appropriate
     * number of dimensions for that texture, all other values
     * should be 0.
     *
     * @param[in] dstTexture
     *      The texture to write to.
     * @param[in] dstSubResource
     *      The subresource to write to in @p dstTexture.
     * @param[in] coord
     *      The coordinate to write to within the subresource.
     * @param[in] srcTexture
     *      The texture to copy.
     * @param[in] srcSubResource
     *      The subresource to copy from @p srcTexture.
     * @param[in] srcBox
     *      An optional region box to copy from.
     */
    virtual void copyTexture(const NullableRef<IResource>& dstTexture, u32 dstSubResource, const ETexture::Coord& coord, const NullableRef<IResource>& srcTexture, u32 srcSubResource, const ETexture::EBox* srcBox) noexcept = 0;

    RTT_BASE_IMPL(ICommandList);
    RTT_BASE_CHECK(ICommandList);
    RTT_BASE_CAST(ICommandList);
};

struct CommandListArgs final
{
    NullableRef<ICommandAllocator> commandAllocator;
    const PipelineState* pipelineState;
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
    [[nodiscard]] virtual ICommandList* build(const CommandListArgs& args, [[tau::out]] Error* error) noexcept = 0;
    [[nodiscard]] virtual ICommandList* build(const CommandListArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept = 0;
    [[nodiscard]] virtual CPPRef<ICommandList> buildCPPRef(const CommandListArgs& args, [[tau::out]] Error* error) noexcept = 0;
    [[nodiscard]] virtual NullableRef<ICommandList> buildTauRef(const CommandListArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ICommandList> buildTauSRef(const CommandListArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept = 0;
};
