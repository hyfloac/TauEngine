#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include <ArrayList.hpp>

#include "graphics/CommandList.hpp"
#include "graphics/DescriptorHeap.hpp"
#include "graphics/DescriptorLayout.hpp"

class GLVertexArray;

// OpenGL Command List
namespace GLCL {

enum class CommandType
{
    Draw = 1,
    DrawIndexed,
    DrawInstanced,
    DrawIndexedInstanced,
    SetVertexArray,
    SetIndexBuffer,
    SetGDescriptorLayout,
    SetGConstant,
    SetGConstants,
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
    GLsizei vertexCount;
    GLenum indexSize;
    const void* indexOffset;
    GLsizei instanceCount;
public:
    CommandDrawIndexedInstanced(const GLenum _mode, const GLsizei _vertexCount, const GLenum _indexSize, const void* const _indexOffset, const GLsizei _instanceCount) noexcept
        : mode(_mode)
        , vertexCount(_vertexCount)
        , indexSize(_indexSize)
        , indexOffset(_indexOffset)
        , instanceCount(_instanceCount)
    { }

    CommandDrawIndexedInstanced(const GLenum _mode, const GLsizei _vertexCount, const GLenum _indexSize, const uSys _indexOffset, const GLsizei _instanceCount) noexcept
        : mode(_mode)
        , vertexCount(_vertexCount)
        , indexSize(_indexSize)
        , indexOffset(reinterpret_cast<const void*>(static_cast<uPtr>(_indexOffset)))
        , instanceCount(_instanceCount)
    { }
};

struct CommandSetVertexArray final
{
    DEFAULT_CONSTRUCT_PU(CommandSetVertexArray);
    DEFAULT_DESTRUCT(CommandSetVertexArray);
    DEFAULT_CM_PU(CommandSetVertexArray);
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
    GLuint ibo;
public:
    CommandSetIndexBuffer(const GLuint _ibo) noexcept
        : ibo(_ibo)
    { }
};

struct CommandSetGDescriptorLayout final
{
    DescriptorLayout layout;
};

struct CommandSetGConstant final
{
    uSys index;
    uSys offset;
    u32 value;
};

struct CommandSetGConstants final
{
    uSys index;
    uSys offset;
    uSys count;
    const u32* values;
};

struct CommandSetGDescriptorTable final
{
    uSys index;
    const DescriptorTable table;
};

struct CommandSetGDescriptorSamplerTable final
{
    uSys index;
    const DescriptorSamplerTable table;
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
        CommandSetVertexArray setVertexArray;
        CommandSetIndexBuffer setIndexBuffer;
        CommandSetGDescriptorLayout setGDescriptorLayout;
        CommandSetGConstant setGConstant;
        CommandSetGConstants setGConstants;
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

    void reset() noexcept override;
    void finish() noexcept override;
    void draw(uSys exCount, uSys startIndex, uSys startVertex) noexcept override;
    void drawInstanced(uSys exCount, uSys startIndex, uSys startVertex, uSys instanceCount, uSys startInstance) noexcept override;
    void setVertexArray(const IVertexArray& va) noexcept override;
    void setInputLayout(const IInputLayout& layout) noexcept override { }
    void setGraphicsDescriptorLayout(const DescriptorLayout& layout) noexcept override;
    void setGraphicsConstant(uSys index, uSys offset, u32 value) noexcept override;
    void setGraphicsConstants(uSys index, uSys offset, uSys count, const u32* values) noexcept override;
    void setGraphicsDescriptorTable(uSys index, const DescriptorTable& table) noexcept override;
    void setGraphicsDescriptorTable(uSys index, const DescriptorSamplerTable& table) noexcept override;
};
