#include "gl/GLVertexArray.hpp"
#include "Timings.hpp"

GLuint GLVertexArray::generate() noexcept
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    return vao;
}

void GLVertexArray::_bind(GLuint vao) noexcept
{
    glBindVertexArray(vao);
}

void GLVertexArray::destroy(const GLuint vao) noexcept
{
    glDeleteVertexArrays(1, &vao);
}

GLVertexArray::GLVertexArray(const std::size_t bufferCount)
    : IVertexArray(bufferCount)
{ }

GLVertexArray::~GLVertexArray() noexcept = default;

void GLVertexArray::internalSetup(IRenderingContext& context) noexcept
{
    PERF();
    u32 attribIndex = 0;
    for(std::size_t i = 0; i < _buffers.size(); ++i)
    {
        auto& buffer = _buffers[i];
        BufferDescriptor& descriptor = buffer->descriptor();

        buffer->bind(context);

        for(std::size_t j = 0; j < descriptor.elements().size(); ++j)
        {

            auto& element = descriptor.elements()[j];
            glEnableVertexAttribArray(attribIndex);
            glVertexAttribPointer(attribIndex,
                                  ShaderDataType::componentCount(element.type()),
                                  getGLType(element.type()),
                                  element.normalized() ? GL_TRUE : GL_FALSE,
                                  descriptor.stride(),
                                  reinterpret_cast<const void*>(static_cast<intptr_t>(element.offset())));
            ++attribIndex;
        }

        buffer->unbind(context);
    }
    _attribCount = attribIndex;
}

void GLVertexArray::bind(IRenderingContext& context) noexcept
{
    _bind(*context.getVertexArrayHandle<GLuint>(this));
}

void GLVertexArray::unbind(IRenderingContext& context) noexcept
{
    _bind(0);
}

void GLVertexArray::preDraw(IRenderingContext& context) noexcept
{
    for(GLuint attrib = 0; attrib < _attribCount; ++attrib)
    {
        glEnableVertexAttribArray(attrib);
    }
}

void GLVertexArray::postDraw(IRenderingContext& context) noexcept
{
    for(GLuint attrib = _attribCount; attrib > 0; --attrib)
    {
        glDisableVertexAttribArray(attrib - 1);
    }
}

void GLVertexArray::draw(IRenderingContext& context) noexcept
{
    glDrawArrays(GL_TRIANGLES, 0, this->_drawCount);
}

void GLVertexArray::drawIndexed(IRenderingContext& context) noexcept
{
    _indexBuffer->bind(context);
    glDrawElements(GL_TRIANGLES, this->_drawCount, GL_UNSIGNED_INT, nullptr);
}

GLenum GLVertexArray::getGLType(const ShaderDataType::Type type) noexcept
{
    switch(type)
    {
        case ShaderDataType::Unknown: break;
        case ShaderDataType::Vector2Bool:
        case ShaderDataType::Vector3Bool:
        case ShaderDataType::Vector4Bool:
        case ShaderDataType::Bool: return GL_BOOL;

        case ShaderDataType::Vector2Int:
        case ShaderDataType::Vector3Int:
        case ShaderDataType::Vector4Int:
        case ShaderDataType::Int: return GL_INT;

        case ShaderDataType::Vector2UInt:
        case ShaderDataType::Vector3UInt:
        case ShaderDataType::Vector4UInt:
        case ShaderDataType::UInt: return GL_UNSIGNED_INT;

        case ShaderDataType::Vector2Float:
        case ShaderDataType::Vector3Float:
        case ShaderDataType::Vector4Float:
        case ShaderDataType::Matrix2x2Float:
        case ShaderDataType::Matrix2x3Float:
        case ShaderDataType::Matrix2x4Float:
        case ShaderDataType::Matrix3x2Float:
        case ShaderDataType::Matrix3x3Float:
        case ShaderDataType::Matrix3x4Float:
        case ShaderDataType::Matrix4x2Float:
        case ShaderDataType::Matrix4x3Float:
        case ShaderDataType::Matrix4x4Float:
        case ShaderDataType::Float: return GL_FLOAT;

        case ShaderDataType::Vector2Double:
        case ShaderDataType::Vector3Double:
        case ShaderDataType::Vector4Double:
        case ShaderDataType::Matrix2x2Double:
        case ShaderDataType::Matrix2x3Double:
        case ShaderDataType::Matrix2x4Double:
        case ShaderDataType::Matrix3x2Double:
        case ShaderDataType::Matrix3x3Double:
        case ShaderDataType::Matrix3x4Double:
        case ShaderDataType::Matrix4x2Double:
        case ShaderDataType::Matrix4x3Double:
        case ShaderDataType::Matrix4x4Double:
        case ShaderDataType::Double: return GL_DOUBLE;
        default: break;
    }
    return 0;
}
