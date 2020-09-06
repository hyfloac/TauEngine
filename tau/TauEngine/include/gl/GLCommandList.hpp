#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include <ArrayList.hpp>
#include <allocator/FreeListAllocator.hpp>

#include "system/Win32Event.hpp"
#include "graphics/CommandList.hpp"

class GLVertexArray;
class GLCommandList;
class GLCommandAllocator;

// OpenGL Command List
namespace GLCL {
enum class CommandType
{
    Draw = 1,
    DrawIndexed,
    DrawIndexedBaseVertex,
    DrawInstanced,
    DrawInstancedBaseInstance,
    DrawIndexedInstanced,
    DrawIndexedBaseVertexInstanced,
    DrawIndexedInstancedBaseInstance,
    DrawIndexedBaseVertexInstancedBaseInstance,
    SetDrawType,
    SetPipelineState,
    SetStencilRef,
    SetVertexArray,
    SetIndexBuffer,
    SetGDescriptorLayout,
    SetGDescriptorTable,
    ExecuteBundle
};

struct CommandDraw final
{
    DEFAULT_CONSTRUCT_PU(CommandDraw);
    DEFAULT_DESTRUCT(CommandDraw);
    DEFAULT_CM_PU(CommandDraw);
public:
    GLint startVertex;
    GLsizei vertexCount;
public:
    CommandDraw(const GLint _startVertex, const GLsizei _vertexCount) noexcept
        : startVertex(_startVertex)
        , vertexCount(_vertexCount)
    { }
};

struct CommandDrawIndexed final
{
    DEFAULT_CONSTRUCT_PU(CommandDrawIndexed);
    DEFAULT_DESTRUCT(CommandDrawIndexed);
    DEFAULT_CM_PU(CommandDrawIndexed);
public:
    GLsizei indexCount;
    const void* indexOffset;
public:
    CommandDrawIndexed(const GLsizei _indexCount, const void* const _indexOffset) noexcept
        : indexCount(_indexCount)
        , indexOffset(_indexOffset)
    { }

    CommandDrawIndexed(const GLsizei _indexCount, const uSys _indexOffset) noexcept
        : indexCount(_indexCount)
        , indexOffset(reinterpret_cast<const void*>(static_cast<uPtr>(_indexOffset)))
    { }
};

struct CommandDrawIndexedBaseVertex final
{
    DEFAULT_CONSTRUCT_PU(CommandDrawIndexedBaseVertex);
    DEFAULT_DESTRUCT(CommandDrawIndexedBaseVertex);
    DEFAULT_CM_PU(CommandDrawIndexedBaseVertex);
public:
    GLsizei indexCount;
    void* indexOffset;
    GLint baseVertex;
public:
    CommandDrawIndexedBaseVertex(const GLsizei _indexCount, void* const _indexOffset, const GLint _baseVertex) noexcept
        : indexCount(_indexCount)
        , indexOffset(_indexOffset)
        , baseVertex(_baseVertex)
    { }

    CommandDrawIndexedBaseVertex(const GLsizei _indexCount, const uSys _indexOffset, const GLint _baseVertex) noexcept
        : indexCount(_indexCount)
        , indexOffset(reinterpret_cast<void*>(static_cast<uPtr>(_indexOffset)))
        , baseVertex(_baseVertex)
    { }
};

struct CommandDrawInstanced final
{
    DEFAULT_CONSTRUCT_PU(CommandDrawInstanced);
    DEFAULT_DESTRUCT(CommandDrawInstanced);
    DEFAULT_CM_PU(CommandDrawInstanced);
public:
    GLint startVertex;
    GLsizei vertexCount;
    GLsizei instanceCount;
public:
    CommandDrawInstanced(const GLint _startVertex, const GLsizei _vertexCount, const GLsizei _instanceCount) noexcept
        : startVertex(_startVertex)
        , vertexCount(_vertexCount)
        , instanceCount(_instanceCount)
    { }
};

struct CommandDrawInstancedBaseInstance final
{
    DEFAULT_CONSTRUCT_PU(CommandDrawInstancedBaseInstance);
    DEFAULT_DESTRUCT(CommandDrawInstancedBaseInstance);
    DEFAULT_CM_PU(CommandDrawInstancedBaseInstance);
public:
    GLint startVertex;
    GLsizei vertexCount;
    GLsizei instanceCount;
    GLuint baseInstance;
public:
    CommandDrawInstancedBaseInstance(const GLint _startVertex, const GLsizei _vertexCount, const GLsizei _instanceCount, const GLuint _baseInstance) noexcept
        : startVertex(_startVertex)
        , vertexCount(_vertexCount)
        , instanceCount(_instanceCount)
        , baseInstance(_baseInstance)
    { }
};

struct CommandDrawIndexedInstanced final
{
    DEFAULT_CONSTRUCT_PU(CommandDrawIndexedInstanced);
    DEFAULT_DESTRUCT(CommandDrawIndexedInstanced);
    DEFAULT_CM_PU(CommandDrawIndexedInstanced);
public:
    GLsizei indexCount;
    const void* indexOffset;
    GLsizei instanceCount;
public:
    CommandDrawIndexedInstanced(const GLsizei _indexCount, const void* const _indexOffset, const GLsizei _instanceCount) noexcept
        : indexCount(_indexCount)
        , indexOffset(_indexOffset)
        , instanceCount(_instanceCount)
    { }

    CommandDrawIndexedInstanced(const GLsizei _indexCount, const uSys _indexOffset, const GLsizei _instanceCount) noexcept
        : indexCount(_indexCount)
        , indexOffset(reinterpret_cast<const void*>(static_cast<uPtr>(_indexOffset)))
        , instanceCount(_instanceCount)
    { }
};

struct CommandDrawIndexedBaseVertexInstanced final
{
    DEFAULT_CONSTRUCT_PU(CommandDrawIndexedBaseVertexInstanced);
    DEFAULT_DESTRUCT(CommandDrawIndexedBaseVertexInstanced);
    DEFAULT_CM_PU(CommandDrawIndexedBaseVertexInstanced);
public:
    GLsizei indexCount;
    const void* indexOffset;
    GLsizei instanceCount;
    GLint baseVertex;
public:
    CommandDrawIndexedBaseVertexInstanced(const GLsizei _indexCount, const void* const _indexOffset, const GLsizei _instanceCount, const GLint _baseVertex) noexcept
        : indexCount(_indexCount)
        , indexOffset(_indexOffset)
        , instanceCount(_instanceCount)
        , baseVertex(_baseVertex)
    { }

    CommandDrawIndexedBaseVertexInstanced(const GLsizei _indexCount, const uSys _indexOffset, const GLsizei _instanceCount, const GLint _baseVertex) noexcept
        : indexCount(_indexCount)
        , indexOffset(reinterpret_cast<const void*>(static_cast<uPtr>(_indexOffset)))
        , instanceCount(_instanceCount)
        , baseVertex(_baseVertex)
    { }
};

struct CommandDrawIndexedInstancedBaseInstance final
{
    DEFAULT_CONSTRUCT_PU(CommandDrawIndexedInstancedBaseInstance);
    DEFAULT_DESTRUCT(CommandDrawIndexedInstancedBaseInstance);
    DEFAULT_CM_PU(CommandDrawIndexedInstancedBaseInstance);
public:
    GLsizei indexCount;
    const void* indexOffset;
    GLsizei instanceCount;
    GLuint baseInstance;
public:
    CommandDrawIndexedInstancedBaseInstance(const GLsizei _indexCount, const void* const _indexOffset, const GLsizei _instanceCount, const GLuint _baseInstance) noexcept
        : indexCount(_indexCount)
        , indexOffset(_indexOffset)
        , instanceCount(_instanceCount)
        , baseInstance(_baseInstance)
    { }

    CommandDrawIndexedInstancedBaseInstance(const GLsizei _indexCount, const uSys _indexOffset, const GLsizei _instanceCount, const GLuint _baseInstance) noexcept
        : indexCount(_indexCount)
        , indexOffset(reinterpret_cast<const void*>(static_cast<uPtr>(_indexOffset)))
        , instanceCount(_instanceCount)
        , baseInstance(_baseInstance)
    { }
};

struct CommandDrawIndexedBaseVertexInstancedBaseInstance final
{
    DEFAULT_CONSTRUCT_PU(CommandDrawIndexedBaseVertexInstancedBaseInstance);
    DEFAULT_DESTRUCT(CommandDrawIndexedBaseVertexInstancedBaseInstance);
    DEFAULT_CM_PU(CommandDrawIndexedBaseVertexInstancedBaseInstance);
public:
    GLsizei indexCount;
    const void* indexOffset;
    GLsizei instanceCount;
    GLint baseVertex;
    GLuint baseInstance;
public:
    CommandDrawIndexedBaseVertexInstancedBaseInstance(const GLsizei _indexCount, const void* const _indexOffset, const GLsizei _instanceCount, const GLint _baseVertex, const GLuint _baseInstance) noexcept
        : indexCount(_indexCount)
        , indexOffset(_indexOffset)
        , instanceCount(_instanceCount)
        , baseVertex(_baseVertex)
        , baseInstance(_baseInstance)
    { }

    CommandDrawIndexedBaseVertexInstancedBaseInstance(const GLsizei _indexCount, const uSys _indexOffset, const GLsizei _instanceCount, const GLint _baseVertex, const GLuint _baseInstance) noexcept
        : indexCount(_indexCount)
        , indexOffset(reinterpret_cast<const void*>(static_cast<uPtr>(_indexOffset)))
        , instanceCount(_instanceCount)
        , baseVertex(_baseVertex)
        , baseInstance(_baseInstance)
    { }
};

struct CommandSetDrawType final
{
    DEFAULT_CONSTRUCT_PU(CommandSetDrawType);
    DEFAULT_DESTRUCT(CommandSetDrawType);
    DEFAULT_CM_PU(CommandSetDrawType);
public:
    GLenum glDrawType;
public:
    CommandSetDrawType(const GLenum _glDrawType) noexcept
        : glDrawType(_glDrawType)
    { }
};

struct CommandSetPipelineState final
{
    DEFAULT_CONSTRUCT_PU(CommandSetPipelineState);
    DEFAULT_DESTRUCT(CommandSetPipelineState);
    DEFAULT_CM_PU(CommandSetPipelineState);
public:
    const PipelineState* pipelineState;
public:
    CommandSetPipelineState(const PipelineState* const _pipelineState) noexcept
        : pipelineState(_pipelineState)
    { }
};

struct CommandSetStencilRef final
{
    DEFAULT_CONSTRUCT_PU(CommandSetStencilRef);
    DEFAULT_DESTRUCT(CommandSetStencilRef);
    DEFAULT_CM_PU(CommandSetStencilRef);
public:
    uSys stencilRef;
public:
    CommandSetStencilRef(const uSys _stencilRef) noexcept
        : stencilRef(_stencilRef)
    { }
};

struct CommandSetVertexArray final
{
    DEFAULT_CONSTRUCT_PU(CommandSetVertexArray);
    DEFAULT_DESTRUCT(CommandSetVertexArray);
    DEFAULT_CM_PU(CommandSetVertexArray);
public:
    GLuint vao;
public:
    CommandSetVertexArray(const GLuint _vao) noexcept
        : vao(_vao)
    { }
};

struct CommandSetIndexBuffer final
{
    DEFAULT_CONSTRUCT_PU(CommandSetIndexBuffer);
    DEFAULT_DESTRUCT(CommandSetIndexBuffer);
    DEFAULT_CM_PU(CommandSetIndexBuffer);
public:
    GLuint ibo;
    GLenum indexSize;
public:
    CommandSetIndexBuffer(const GLuint _ibo, const GLenum _indexSize) noexcept
        : ibo(_ibo)
        , indexSize(_indexSize)
    { }
};

struct CommandSetGDescriptorLayout final
{
    DEFAULT_CONSTRUCT_PU(CommandSetGDescriptorLayout);
    DEFAULT_DESTRUCT(CommandSetGDescriptorLayout);
    DEFAULT_CM_PU(CommandSetGDescriptorLayout);
public:
    DescriptorLayout layout;
public:
    CommandSetGDescriptorLayout(const DescriptorLayout _layout) noexcept
        : layout(_layout)
    { }
};

struct CommandSetGDescriptorTable final
{
    DEFAULT_CONSTRUCT_PU(CommandSetGDescriptorTable);
    DEFAULT_DESTRUCT(CommandSetGDescriptorTable);
    DEFAULT_CM_PU(CommandSetGDescriptorTable);
public:
    uSys index;
    EGraphics::DescriptorType type;
    uSys descriptorCount;
    GPUDescriptorHandle handle;
public:
    CommandSetGDescriptorTable(const uSys _index, const EGraphics::DescriptorType _type, const uSys _descriptorCount, const GPUDescriptorHandle _handle) noexcept
        : index(_index)
        , type(_type)
        , descriptorCount(_descriptorCount)
        , handle(_handle)
    { }
};

struct CommandExecuteBundle final
{
    DEFAULT_CONSTRUCT_PU(CommandExecuteBundle);
    DEFAULT_DESTRUCT(CommandExecuteBundle);
    DEFAULT_CM_PU(CommandExecuteBundle);
public:
    const GLCommandList* bundle;
public:
    CommandExecuteBundle(const GLCommandList* const _bundle) noexcept
        : bundle(_bundle)
    { }
};

struct Command final
{
    DEFAULT_DESTRUCT(Command);
    DEFAULT_CM_PU(Command);
public:
    CommandType type;
    union
    {
        uSys _;
        CommandDraw draw;
        CommandDrawIndexed drawIndexed;
        CommandDrawIndexedBaseVertex drawIndexedBaseVertex;
        CommandDrawInstanced drawInstanced;
        CommandDrawInstancedBaseInstance drawInstancedBaseInstance;
        CommandDrawIndexedInstanced drawIndexedInstanced;
        CommandDrawIndexedBaseVertexInstanced drawIndexedBaseVertexInstanced;
        CommandDrawIndexedInstancedBaseInstance drawIndexedInstancedBaseInstance;
        CommandDrawIndexedBaseVertexInstancedBaseInstance drawIndexedBaseVertexInstancedBaseInstance;
        CommandSetDrawType setDrawType;
        CommandSetPipelineState setPipelineState;
        CommandSetStencilRef setStencilRef;
        CommandSetVertexArray setVertexArray;
        CommandSetIndexBuffer setIndexBuffer;
        CommandSetGDescriptorLayout setGDescriptorLayout;
        CommandSetGDescriptorTable setGDescriptorTable;
        CommandExecuteBundle executeBundle;
    };
public:
    Command() noexcept
        : type(static_cast<CommandType>(0))
        , _(0)
    { }

    Command(const CommandDraw& _draw) noexcept
        : type(CommandType::Draw)
        , draw(_draw)
    { }

    Command(const CommandDrawIndexed& _drawIndexed) noexcept
        : type(CommandType::DrawIndexed)
        , drawIndexed(_drawIndexed)
    { }

    Command(const CommandDrawIndexedBaseVertex& _drawIndexedBaseVertex) noexcept
        : type(CommandType::DrawIndexedBaseVertex)
        , drawIndexedBaseVertex(_drawIndexedBaseVertex)
    { }

    Command(const CommandDrawInstanced& _drawInstanced) noexcept
        : type(CommandType::DrawInstanced)
        , drawInstanced(_drawInstanced)
    { }

    Command(const CommandDrawInstancedBaseInstance& _drawInstancedBaseInstance) noexcept
        : type(CommandType::DrawInstancedBaseInstance)
        , drawInstancedBaseInstance(_drawInstancedBaseInstance)
    { }

    Command(const CommandDrawIndexedInstanced& _drawIndexedInstanced) noexcept
        : type(CommandType::DrawIndexedInstanced)
        , drawIndexedInstanced(_drawIndexedInstanced)
    { }

    Command(const CommandDrawIndexedBaseVertexInstanced& _drawIndexedBaseVertexInstanced) noexcept
        : type(CommandType::DrawIndexedBaseVertexInstanced)
        , drawIndexedBaseVertexInstanced(_drawIndexedBaseVertexInstanced)
    { }

    Command(const CommandDrawIndexedInstancedBaseInstance& _drawIndexedInstancedBaseInstance) noexcept
        : type(CommandType::DrawIndexedInstancedBaseInstance)
        , drawIndexedInstancedBaseInstance(_drawIndexedInstancedBaseInstance)
    { }

    Command(const CommandDrawIndexedBaseVertexInstancedBaseInstance& _drawIndexedBaseVertexInstancedBaseInstance) noexcept
        : type(CommandType::DrawIndexedBaseVertexInstancedBaseInstance)
        , drawIndexedBaseVertexInstancedBaseInstance(_drawIndexedBaseVertexInstancedBaseInstance)
    { }

    Command(const CommandSetDrawType& _setDrawType) noexcept
        : type(CommandType::SetDrawType)
        , setDrawType(_setDrawType)
    { }

    Command(const CommandSetPipelineState& _setPipelineState) noexcept
        : type(CommandType::SetPipelineState)
        , setPipelineState(_setPipelineState)
    { }

    Command(const CommandSetStencilRef& _setStencilRef) noexcept
        : type(CommandType::SetStencilRef)
        , setStencilRef(_setStencilRef)
    { }

    Command(const CommandSetVertexArray& _setVertexArray) noexcept
        : type(CommandType::SetVertexArray)
        , setVertexArray(_setVertexArray)
    { }

    Command(const CommandSetIndexBuffer& _setIndexBuffer) noexcept
        : type(CommandType::SetIndexBuffer)
        , setIndexBuffer(_setIndexBuffer)
    { }

    Command(const CommandSetGDescriptorLayout& _setGDescriptorLayout) noexcept
        : type(CommandType::SetGDescriptorLayout)
        , setGDescriptorLayout(_setGDescriptorLayout)
    { }

    Command(const CommandSetGDescriptorTable& _setGDescriptorTable) noexcept
        : type(CommandType::SetGDescriptorTable)
        , setGDescriptorTable(_setGDescriptorTable)
    { }

    Command(const CommandExecuteBundle& _executeBundle) noexcept
        : type(CommandType::ExecuteBundle)
        , executeBundle(_executeBundle)
    { }
};
}

class TAU_DLL GLCommandList final : public ICommandList
{
    DEFAULT_DESTRUCT(GLCommandList);
    DELETE_CM(GLCommandList);
    COMMAND_LIST_IMPL(GLCommandList);
private:
    static constexpr uSys MaxRefCounters = 3;
    static constexpr uSys MaxVertexBuffers = 64;
private:
    NullableRef<GLCommandAllocator> _commandAllocator;
    const void* _head;
    uSys _commandCount;
public:
    GLCommandList(const NullableRef<GLCommandAllocator>& allocator) noexcept;

    [[nodiscard]] const void* head() const noexcept { return _head; }
    [[nodiscard]] uSys commandCount() const noexcept { return _commandCount; }

    void reset(const NullableRef<ICommandAllocator>& allocator) noexcept override;
    void begin() noexcept override;
    void finish() noexcept override;
    void draw(uSys vertexCount, uSys startVertex) noexcept override;
    void drawIndexed(uSys indexCount, uSys startIndex, iSys baseVertex) noexcept override;
    void drawInstanced(uSys vertexCount, uSys startVertex, uSys instanceCount, uSys startInstance) noexcept override;
    void drawIndexedInstanced(uSys indexCount, uSys startIndex, iSys baseVertex, uSys instanceCount, uSys startInstance) noexcept override;
    void setDrawType(EGraphics::DrawType drawType) noexcept override;
    void setPipelineState(const PipelineState& pipelineState) noexcept override;
    void setStencilRef(uSys stencilRef) noexcept override;
    void setVertexArray(const NullableRef<IVertexArray>& va) noexcept override;
    void setIndexBuffer(const IndexBufferView& indexBufferView) noexcept override;
    void setGraphicsDescriptorLayout(DescriptorLayout layout) noexcept override;
    void setGraphicsDescriptorTable(uSys index, EGraphics::DescriptorType type, uSys descriptorCount, GPUDescriptorHandle handle) noexcept override;
    void executeBundle(const NullableRef<ICommandList>& bundle) noexcept override;
private:
    friend class GLCommandQueue;
};
