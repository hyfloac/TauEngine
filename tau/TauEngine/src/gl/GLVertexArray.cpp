#include "gl/GLVertexArray.hpp"
#include "Timings.hpp"
#include "gl/GLBuffer.hpp"
#include "gl/GLInputLayout.hpp"

static GLenum glDrawType(DrawType drawType) noexcept;

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

GLVertexArray::GLVertexArray(const u32 drawCount, const DrawType drawType, const RefDynArray<Ref<IBuffer>>& buffers, const Ref<GLIndexBuffer>& indexBuffer)
    : IVertexArray(drawCount), _glDrawType(glDrawType(drawType)), _buffers(buffers), _indexBuffer(indexBuffer), _attribCount(0)
{ }

GLVertexArray::~GLVertexArray() noexcept = default;

void GLVertexArray::internalSetup(IRenderingContext& context) noexcept
{
    PERF();
    u32 attribIndex = 0;
    for(uSys i = 0; i < _buffers.size(); ++i)
    {
        auto& buffer = _buffers[i];
        const BufferDescriptor& descriptor = buffer->descriptor();

        buffer->bind(context);

        for(uSys j = 0; j < descriptor.elements().size(); ++j)
        {
            // TODO: Properly handle matrices, integer, and float64 types.
            // Matrices have to stored as a number of vectors
            // Integers require glVertexAttribIPointer
            // Doubles require glVertexAttribLPointer

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
    if(_indexBuffer)
    {
        _indexBuffer->bind(context);
        glDrawElements(_glDrawType, _drawCount, GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        glDrawArrays(_glDrawType, 0, this->_drawCount);
    }
}

void GLVertexArrayBuilder::setVertexBuffer(uSys index, const Ref<IBuffer>& vertexBuffer) noexcept
{
    if(RTT_CHECK(vertexBuffer.get(), GLBuffer))
    {
        IVertexArrayBuilder::setVertexBuffer(index, vertexBuffer);
    }
}

void GLVertexArrayBuilder::indexBuffer(const Ref<IIndexBuffer>& indexBuffer) noexcept
{
    if(!indexBuffer)
    {
        IVertexArrayBuilder::indexBuffer(null);
        _indexBuffer = null;
    }
    else if(RTT_CHECK(indexBuffer.get(), GLIndexBuffer))
    {
        IVertexArrayBuilder::indexBuffer(indexBuffer);
        _indexBuffer = RefCast<GLIndexBuffer>(indexBuffer);
    }
}

void GLVertexArrayBuilder::inputLayout(const Ref<IInputLayout>& inputLayout) noexcept
{
    if(RTT_CHECK(inputLayout.get(), GLInputLayout))
    {
        IVertexArrayBuilder::inputLayout(inputLayout);
    }
}

GLVertexArray* GLVertexArrayBuilder::build(Error* error) noexcept
{
    ERROR_CODE_COND_N(!_inputLayout, Error::InputLayoutNotSet);
    ERROR_CODE_COND_N(_drawCount == 0, Error::DrawCountNotSet);
    ERROR_CODE_COND_N(_drawType == static_cast<DrawType>(0), Error::DrawTypeNotSet);

    for(uSys i = 0; i < _buffers.count(); ++i)
    {
        ERROR_CODE_COND_N(!_buffers[i], Error::BuffersNotSet);
    }

    GLVertexArray* va = new(::std::nothrow) GLVertexArray(_drawCount, _drawType, _buffers, RefCast<GLIndexBuffer>(_indexBuffer));
    ERROR_CODE_COND_N(!va, Error::MemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, va);
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

static GLenum glDrawType(const DrawType drawType) noexcept
{
    switch(drawType)
    {
        case DrawType::SeparatedTriangles: return GL_TRIANGLES;
        case DrawType::ConnectedTriangles: return GL_TRIANGLE_STRIP;
        case DrawType::PointConnectedTriangles: return GL_TRIANGLE_FAN;
        default: return 0;
    }
}
