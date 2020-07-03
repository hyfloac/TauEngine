#include "gl/GLVertexArray.hpp"
#include "Timings.hpp"
#include "gl/GLBuffer.hpp"

static GLenum glDrawType(DrawType drawType) noexcept;

void GLVertexArray::bind(IRenderingContext& context) noexcept
{
    glBindVertexArray(_vao);
    if(_indexBuffer)
    {
        _indexBuffer->bind();
    }
}

void GLVertexArray::unbind(IRenderingContext& context) noexcept
{
    if(_indexBuffer)
    {
        _indexBuffer->unbind();
    }
    glBindVertexArray(0);
}

void GLVertexArray::draw(IRenderingContext& context, uSys drawCount, const uSys drawOffset) noexcept
{
    if(drawCount == 0)
    { drawCount = _drawCount; }

    if(_indexBuffer)
    {
        glDrawElements(_glDrawType, drawCount, _indexBuffer->glIndexSize(), reinterpret_cast<const void*>(drawOffset));
    }
    else
    {
        glDrawArrays(_glDrawType, drawOffset, drawCount);
    }
}

void GLVertexArray::drawInstanced(IRenderingContext& context, const uSys instanceCount, uSys drawCount, const uSys drawOffset) noexcept
{
    if(drawCount == 0)
    { drawCount = _drawCount; }

    if(this->_indexBuffer)
    {
        glDrawElementsInstanced(_glDrawType, drawCount, _indexBuffer->glIndexSize(), reinterpret_cast<const void*>(drawOffset), instanceCount);
    }
    else
    {
        glDrawArraysInstanced(_glDrawType, drawOffset, drawCount, instanceCount);
    }
}

IVertexArray* GLVertexArrayBuilder::build(const VertexArrayArgs& args, Error* const error) noexcept
{
    GLVertexArrayArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLVertexArray* const va = new(::std::nothrow) GLVertexArray(args.drawCount, args.buffers, glArgs.vao, glArgs.drawType, glArgs.indexBuffer);
    ERROR_CODE_COND_N(!va, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, va);
}

IVertexArray* GLVertexArrayBuilder::build(const VertexArrayArgs& args, Error* const error, TauAllocator& allocator) noexcept
{
    GLVertexArrayArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLVertexArray* const va = allocator.allocateT<GLVertexArray>(args.drawCount, args.buffers, glArgs.vao, glArgs.drawType, glArgs.indexBuffer);
    ERROR_CODE_COND_N(!va, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, va);
}

CPPRef<IVertexArray> GLVertexArrayBuilder::buildCPPRef(const VertexArrayArgs& args, Error* const error) noexcept
{
    GLVertexArrayArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const CPPRef<GLVertexArray> va = CPPRef<GLVertexArray>(new(::std::nothrow) GLVertexArray(args.drawCount, args.buffers, glArgs.vao, glArgs.drawType, glArgs.indexBuffer));
    ERROR_CODE_COND_N(!va, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, va);
}

NullableRef<IVertexArray> GLVertexArrayBuilder::buildTauRef(const VertexArrayArgs& args, Error* const error, TauAllocator& allocator) noexcept
{
    GLVertexArrayArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableRef<GLVertexArray> va(allocator, args.drawCount, args.buffers, glArgs.vao, glArgs.drawType, glArgs.indexBuffer);
    ERROR_CODE_COND_N(!va, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, RefCast<IVertexArray>(va));
}

NullableStrongRef<IVertexArray> GLVertexArrayBuilder::buildTauSRef(const VertexArrayArgs& args, Error* const error, TauAllocator& allocator) noexcept
{
    GLVertexArrayArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableStrongRef<GLVertexArray> va(allocator, args.drawCount, args.buffers, glArgs.vao, glArgs.drawType, glArgs.indexBuffer);
    ERROR_CODE_COND_N(!va, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, RefCast<IVertexArray>(va));
}

bool GLVertexArrayBuilder::processArgs(const VertexArrayArgs& args, GLVertexArrayArgs* glArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.drawCount == 0, Error::DrawCountNotSet);
    ERROR_CODE_COND_F(args.drawType == static_cast<DrawType>(0), Error::DrawTypeNotSet);

    for(uSys i = 0; i < args.buffers.count(); ++i)
    {
        ERROR_CODE_COND_F(!args.buffers[i], Error::BuffersNotSet);
        ERROR_CODE_COND_F(!RTT_CHECK(args.buffers[i].get(), GLVertexBuffer), Error::InternalError);
    }

    if(args.indexBuffer)
    {
        ERROR_CODE_COND_F(!RTT_CHECK(args.indexBuffer.get(), GLIndexBuffer), Error::InternalError);
        glArgs->indexBuffer = RefCast<GLIndexBuffer>(args.indexBuffer);
    }
    else
    {
        glArgs->indexBuffer = null;
    }

    glArgs->drawType = glDrawType(args.drawType);

    glGenVertexArrays(1, &glArgs->vao);
    glBindVertexArray(glArgs->vao);

    uSys attribIndex = 0;
    for(uSys i = 0; i < args.buffers.size(); ++i)
    {
        auto& buffer = args.buffers[i];
        const BufferDescriptor& descriptor = buffer->descriptor();

        const auto glBuffer = RefCast<GLVertexBuffer>(buffer);

        glBuffer->bind();

        for(uSys j = 0; j < descriptor.elements().size(); ++j)
        {
            auto& element = descriptor.elements()[j];
            const ShaderDataType::Type uType = ShaderDataType::underlyingType(element.type());
            const uSys columns = ShaderDataType::columnCount(element.type());
            intptr_t offset = static_cast<intptr_t>(element.offset());

            const GLint size = ShaderDataType::componentCount(uType);
            const GLenum type = GLVertexArray::getGLType(element.type());
            const GLboolean normalized = element.normalized() ? GL_TRUE : GL_FALSE;
            const GLsizei stride = descriptor.stride();
            const void* pointer = reinterpret_cast<const void*>(offset);

            for(uSys m = 0; m < columns; ++m)
            {
                glEnableVertexAttribArray(attribIndex);

                if(type == GL_DOUBLE)
                {
                    glVertexAttribLPointer(attribIndex, size, GL_DOUBLE, stride, pointer);
                }
                else if((type == GL_INT || type == GL_UNSIGNED_INT) && !normalized)
                {
                    glVertexAttribIPointer(attribIndex, size, type, stride, pointer);
                }
                else
                {
                    glVertexAttribPointer(attribIndex, size, type, normalized, stride, pointer);
                }

                if(descriptor.instanced())
                {
                    glVertexAttribDivisor(attribIndex, 1);
                }
                else
                {
                    glVertexAttribDivisor(attribIndex, 0);
                }

                ++attribIndex;
                offset += ShaderDataType::size(uType);
                pointer = reinterpret_cast<const void*>(offset);
            }
        }

        glBuffer->unbind();
    }

    glBindVertexArray(0);

    glArgs->attribCount = static_cast<GLuint>(attribIndex);

    return true;
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
