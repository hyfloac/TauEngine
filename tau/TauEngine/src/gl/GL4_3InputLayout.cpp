#include "gl/gl4_3/GL4_3InputLayout.hpp"
#include "gl/GLRenderingContext.hpp"

#include "shader/Shader.hpp"

#if TAU_RTTI_CHECK
  #include "TauEngine.hpp"
  #define CTX() \
      if(!RTT_CHECK(context, GLRenderingContext)) \
      { TAU_THROW(IncorrectContextException); } \
      auto& ctx = reinterpret_cast<GLRenderingContext&>(context)
#else
  #define CTX() \
      auto& ctx = reinterpret_cast<GLRenderingContext&>(context)
#endif

void GL4_3InputLayout::bind(IRenderingContext& context) noexcept
{
    CTX();
    glBindVertexArray(_vao);
    ctx.setBufferData(_offsets, _strides);
}

void GL4_3InputLayout::unbind(IRenderingContext& context) noexcept
{
    CTX();
    glBindVertexArray(0);
    ctx.setBufferData(null, null);
}

GL4_3InputLayout* GL4_3InputLayoutBuilder::build(const InputLayoutArgs& args, Error* const error) noexcept
{
    GL4_3InputLayoutArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GL4_3InputLayout* const inputLayout = new(::std::nothrow) GL4_3InputLayout(glArgs);
    ERROR_CODE_COND_N(!inputLayout, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, inputLayout);
}

GL4_3InputLayout* GL4_3InputLayoutBuilder::build(const InputLayoutArgs& args, Error* const error, TauAllocator& allocator) noexcept
{
    GL4_3InputLayoutArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GL4_3InputLayout* const inputLayout = allocator.allocateT<GL4_3InputLayout>(glArgs);
    ERROR_CODE_COND_N(!inputLayout, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, inputLayout);
}

CPPRef<IInputLayout> GL4_3InputLayoutBuilder::buildCPPRef(const InputLayoutArgs& args, Error* const error) noexcept
{
    GL4_3InputLayoutArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const CPPRef<GL4_3InputLayout> inputLayout = CPPRef<GL4_3InputLayout>(new(::std::nothrow) GL4_3InputLayout(glArgs));
    ERROR_CODE_COND_N(!inputLayout, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, inputLayout);
}

NullableRef<IInputLayout> GL4_3InputLayoutBuilder::buildTauRef(const InputLayoutArgs& args, Error* const error, TauAllocator& allocator) noexcept
{
    GL4_3InputLayoutArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableRef<GL4_3InputLayout> inputLayout(allocator, glArgs);
    ERROR_CODE_COND_N(!inputLayout, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, inputLayout);
}

NullableStrongRef<IInputLayout> GL4_3InputLayoutBuilder::buildTauSRef(const InputLayoutArgs& args, Error* const error, TauAllocator& allocator) noexcept
{
    GL4_3InputLayoutArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableStrongRef<GL4_3InputLayout> inputLayout(allocator, glArgs);
    ERROR_CODE_COND_N(!inputLayout, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, inputLayout);
}

bool GL4_3InputLayoutBuilder::processArgs(const InputLayoutArgs& args, GL4_3InputLayoutArgs* const glArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(!args.shader, Error::ShaderNotSet);
    ERROR_CODE_COND_F(args.shader->shaderStage() != EShader::Stage::Vertex, Error::ShaderMustBeVertexShader);

    glArgs->offsets = new GLintptr[args.descriptors.size()];
    glArgs->strides = new GLsizei[args.descriptors.size()];

    ERROR_CODE_COND_F(!glArgs->strides, Error::SystemMemoryAllocationFailure);
    ERROR_CODE_COND_F(!glArgs->offsets, Error::SystemMemoryAllocationFailure);
    ::std::memset(glArgs->offsets, 0, args.descriptors.size() * sizeof(GLsizei));

    glGenVertexArrays(1, &glArgs->vao);
    glBindVertexArray(glArgs->vao);

    uSys attribIndex = 0;
    for(uSys i = 0; i < args.descriptors.size(); ++i)
    {
        const BufferDescriptor& descriptor = args.descriptors[i];

        for(uSys j = 0; j < descriptor.elements().size(); ++j)
        {
            auto& element = descriptor.elements()[j];
            const ShaderDataType::Type uType = ShaderDataType::underlyingType(element.type());
            const uSys columns = ShaderDataType::columnCount(element.type());
            GLuint offset = element.offset();

            const GLint size = ShaderDataType::componentCount(uType);
            const GLenum type = getGLType(element.type());
            const GLboolean normalized = element.normalized() ? GL_TRUE : GL_FALSE;

            glArgs->strides[i] = descriptor.stride();

            for(uSys m = 0; m < columns; ++m)
            {
                glEnableVertexAttribArray(attribIndex);

                if(type == GL_DOUBLE)
                {
                    glVertexAttribLFormat(attribIndex, size, GL_DOUBLE, offset);
                }
                else if((type == GL_INT || type == GL_UNSIGNED_INT) && !normalized)
                {
                    glVertexAttribIFormat(attribIndex, size, type, offset);
                }
                else
                {
                    glVertexAttribFormat(attribIndex, size, type, normalized, offset);
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
            }
        }
    }

    glBindVertexArray(0);

    return true;
}

GLenum GL4_3InputLayoutBuilder::getGLType(const ShaderDataType::Type type) noexcept
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
