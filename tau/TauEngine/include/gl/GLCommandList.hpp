#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include <ArrayList.hpp>

#include "graphics/CommandList.hpp"
#include "graphics/_GraphicsOpaqueObjects.hpp"

class GLVertexArray;

// OpenGL Command List
namespace GLCL {

enum class CommandType
{
    Draw = 1,
    DrawIndexed,
    DrawInstanced,
    DrawIndexedInstanced,
    SetPipelineState,
    SetVertexArray,
    SetIndexBuffer,
    SetGDescriptorLayout,
    SetGDescriptorTable,
    SetGDescriptorSamplerTable
};

struct CommandDraw final
{
    DEFAULT_CONSTRUCT_PU(CommandDraw);
    DEFAULT_DESTRUCT(CommandDraw);
    DEFAULT_CM_PU(CommandDraw);
public:
    GLenum mode;
    GLint startVertex;
    GLsizei vertexCount;
public:
    CommandDraw(const GLenum _mode, const GLint _startVertex, const GLsizei _vertexCount) noexcept
        : mode(_mode)
        , startVertex(_startVertex)
        , vertexCount(_vertexCount)
    { }
};

struct CommandDrawIndexed final
{
    DEFAULT_CONSTRUCT_PU(CommandDrawIndexed);
    DEFAULT_DESTRUCT(CommandDrawIndexed);
    DEFAULT_CM_PU(CommandDrawIndexed);
public:
    GLenum mode;
    GLsizei indexCount;
    GLenum indexSize;
    const void* indexOffset;
public:
    CommandDrawIndexed(const GLenum _mode, const GLsizei _indexCount, const GLenum _indexSize, const void* const _indexOffset) noexcept
        : mode(_mode)
        , indexCount(_indexCount)
        , indexSize(_indexSize)
        , indexOffset(_indexOffset)
    { }

    CommandDrawIndexed(const GLenum _mode, const GLsizei _indexCount, const GLenum _indexSize, const uSys _indexOffset) noexcept
        : mode(_mode)
        , indexCount(_indexCount)
        , indexSize(_indexSize)
        , indexOffset(reinterpret_cast<const void*>(static_cast<uPtr>(_indexOffset)))
    { }
};

struct CommandDrawInstanced final
{
    DEFAULT_CONSTRUCT_PU(CommandDrawInstanced);
    DEFAULT_DESTRUCT(CommandDrawInstanced);
    DEFAULT_CM_PU(CommandDrawInstanced);
public:
    GLenum mode;
    GLint startVertex;
    GLsizei vertexCount;
    GLsizei instanceCount;
public:
    CommandDrawInstanced(const GLenum _mode, const GLint _startVertex, const GLsizei _vertexCount, const GLsizei _instanceCount) noexcept
        : mode(_mode)
        , startVertex(_startVertex)
        , vertexCount(_vertexCount)
        , instanceCount(_instanceCount)
    { }
};

struct CommandDrawIndexedInstanced final
{
    DEFAULT_CONSTRUCT_PU(CommandDrawIndexedInstanced);
    DEFAULT_DESTRUCT(CommandDrawIndexedInstanced);
    DEFAULT_CM_PU(CommandDrawIndexedInstanced);
public:
    GLenum mode;
    GLsizei indexCount;
    GLenum indexSize;
    const void* indexOffset;
    GLsizei instanceCount;
public:
    CommandDrawIndexedInstanced(const GLenum _mode, const GLsizei _indexCount, const GLenum _indexSize, const void* const _indexOffset, const GLsizei _instanceCount) noexcept
        : mode(_mode)
        , indexCount(_indexCount)
        , indexSize(_indexSize)
        , indexOffset(_indexOffset)
        , instanceCount(_instanceCount)
    { }

    CommandDrawIndexedInstanced(const GLenum _mode, const GLsizei _indexCount, const GLenum _indexSize, const uSys _indexOffset, const GLsizei _instanceCount) noexcept
        : mode(_mode)
        , indexCount(_indexCount)
        , indexSize(_indexSize)
        , indexOffset(reinterpret_cast<const void*>(static_cast<uPtr>(_indexOffset)))
        , instanceCount(_instanceCount)
    { }
};

struct CommandSetPipelineState final
{
    DEFAULT_CONSTRUCT_PU(CommandSetPipelineState);
    DEFAULT_DESTRUCT(CommandSetPipelineState);
    DEFAULT_CM_PU(CommandSetPipelineState);
public:
    const PipelineState& pipelineState;
public:
    CommandSetPipelineState(const PipelineState& _pipelineState) noexcept
        : pipelineState(_pipelineState)
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
public:
    CommandSetIndexBuffer(const GLuint _ibo) noexcept
        : ibo(_ibo)
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
    DescriptorTable table;
public:
    CommandSetGDescriptorTable(const uSys _index, const DescriptorTable _table) noexcept
        : index(_index)
        , table(_table)
    { }
};

struct CommandSetGDescriptorSamplerTable final
{
    DEFAULT_CONSTRUCT_PU(CommandSetGDescriptorSamplerTable);
    DEFAULT_DESTRUCT(CommandSetGDescriptorSamplerTable);
    DEFAULT_CM_PU(CommandSetGDescriptorSamplerTable);
public:
    uSys index;
    DescriptorSamplerTable table;
public:
    CommandSetGDescriptorSamplerTable(const uSys _index, const DescriptorSamplerTable _table) noexcept
        : index(_index)
        , table(_table)
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
        CommandDrawInstanced drawInstanced;
        CommandDrawIndexedInstanced drawIndexedInstanced;
        CommandSetPipelineState setPipelineState;
        CommandSetVertexArray setVertexArray;
        CommandSetIndexBuffer setIndexBuffer;
        CommandSetGDescriptorLayout setGDescriptorLayout;
        CommandSetGDescriptorTable setGDescriptorTable;
        CommandSetGDescriptorSamplerTable setGDescriptorSamplerTable;
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

    Command(const CommandDrawInstanced& _drawInstanced) noexcept
        : type(CommandType::DrawInstanced)
        , drawInstanced(_drawInstanced)
    { }

    Command(const CommandDrawIndexedInstanced& _drawIndexedInstanced) noexcept
        : type(CommandType::DrawIndexedInstanced)
        , drawIndexedInstanced(_drawIndexedInstanced)
    { }

    Command(const CommandSetPipelineState& _setPipelineState) noexcept
        : type(CommandType::SetPipelineState)
        , setPipelineState(_setPipelineState)
    { }

    Command(const CommandSetVertexArray& _setVertexArray) noexcept
        : type(CommandType::SetVertexArray)
        , setVertexArray(_setVertexArray)
    { }

    Command(const CommandSetIndexBuffer& _setIndexBuffer) noexcept
        : type(CommandType::SetIndexBuffer)
        , setIndexBuffer(_setIndexBuffer)
    { }
};
}

class TAU_DLL GLCommandList final : public ICommandList
{
    DEFAULT_DESTRUCT(GLCommandList);
    DELETE_CM(GLCommandList);
    COMMAND_LIST_IMPL(GLCommandList);
private:
    uSys _maxCommands;
    ArrayList<GLCL::Command> _commands;
    const GLVertexArray* _currentVA;
public:
    GLCommandList(uSys maxCommands = 4096) noexcept
        : _maxCommands(maxCommands)
        , _commands(maxCommands)
        , _currentVA(null)
    { }

    [[nodiscard]] const ArrayList<GLCL::Command>& commands() const noexcept { return _commands; }

    void reset() noexcept override;
    void finish() noexcept override;
    void draw(uSys exCount, uSys startIndex, uSys startVertex) noexcept override;
    void drawInstanced(uSys exCount, uSys startIndex, uSys startVertex, uSys instanceCount, uSys startInstance) noexcept override;
    void setVertexArray(const IVertexArray& va) noexcept override;
    void setPipelineState(const PipelineState& pipelineState) noexcept override;
    void setGraphicsDescriptorLayout(DescriptorLayout layout) noexcept override;
    void setGraphicsDescriptorTable(uSys index, DescriptorTable table) noexcept override;
    void setGraphicsDescriptorTable(uSys index, DescriptorSamplerTable table) noexcept override;
};
