#include "gl/GLTexture.hpp"
#include "gl/GLUtils.hpp"

GLTexture2D::GLTexture2D(const u32 width, const u32 height, const ETexture::Format dataFormat, const GLuint texture) noexcept
    : ITexture(width, height, dataFormat), _texture(texture)
{ }

GLTexture2D::~GLTexture2D() noexcept
{
    glDeleteTextures(1, &_texture);
}

void GLTexture2D::set(const u32 level, const void* data) noexcept
{
    const GLint internalFormat = glInternalFormat(_dataFormat);
    const GLenum inputFormat = glInputFormat(_dataFormat);
    const GLenum inputDataType = glInputDataType(_dataFormat);

    glBindTexture(GL_TEXTURE_2D, _texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, level, internalFormat, _width, _height, 0, inputFormat, inputDataType, data);
}

void GLTexture2D::bind(u8 textureUnit, EShader::Stage) noexcept
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, _texture);
}

void GLTexture2D::unbind(u8 textureUnit, EShader::Stage) noexcept
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLDepthTexture::GLDepthTexture(const u32 width, const u32 height, const ETexture::Format dataFormat, const GLuint texture) noexcept
    : GLTexture2D(width, height, dataFormat, texture)
{ }

void GLDepthTexture::set(const u32 level, const void* data) noexcept
{
    GLTexture2D::set(level , data);
}

void GLTexture2D::generateMipmaps() noexcept
{
    glGenerateMipmap(GL_TEXTURE_2D);
}

GLTexture2D* GLTexture2DBuilder::build(const TextureArgs& args, Error* error) const noexcept
{
    GLTextureArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLTexture2D* const texture = new(std::nothrow) GLTexture2D(args.width, args.height, args.dataFormat, glArgs.textureHandle);

    if(!texture)
    {
        glDeleteTextures(1, &glArgs.textureHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    texture->set(0, args.initialBuffer);

    ERROR_CODE_V(Error::NoError, texture);
}

GLTexture2D* GLTexture2DBuilder::build(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLTextureArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLTexture2D* const texture = allocator.allocateT<GLTexture2D>(args.width, args.height, args.dataFormat, glArgs.textureHandle);

    if(!texture)
    {
        glDeleteTextures(1, &glArgs.textureHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    texture->set(0, args.initialBuffer);

    ERROR_CODE_V(Error::NoError, texture);
}

Ref<ITexture> GLTexture2DBuilder::buildCPPRef(const TextureArgs& args, Error* error) const noexcept
{
    GLTextureArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const Ref<GLTexture2D> texture = Ref<GLTexture2D>(new(std::nothrow) GLTexture2D(args.width, args.height, args.dataFormat, glArgs.textureHandle));

    if(!texture)
    {
        glDeleteTextures(1, &glArgs.textureHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    texture->set(0, args.initialBuffer);

    ERROR_CODE_V(Error::NoError, texture);
}

NullableReferenceCountingPointer<ITexture> GLTexture2DBuilder::buildTauRef(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLTextureArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    NullableReferenceCountingPointer<GLTexture2D> texture(allocator, args.width, args.height, args.dataFormat, glArgs.textureHandle);

    if(!texture)
    {
        glDeleteTextures(1, &glArgs.textureHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    texture->set(0, args.initialBuffer);

    ERROR_CODE_V(Error::NoError, RCPCast<ITexture>(texture));
}

NullableStrongReferenceCountingPointer<ITexture> GLTexture2DBuilder::buildTauSRef(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLTextureArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    NullableStrongReferenceCountingPointer<GLTexture2D> texture(allocator, args.width, args.height, args.dataFormat, glArgs.textureHandle);

    if(!texture)
    {
        glDeleteTextures(1, &glArgs.textureHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    texture->set(0, args.initialBuffer);

    ERROR_CODE_V(Error::NoError, RCPCast<ITexture>(texture));
}

bool GLTexture2DBuilder::processArgs(const TextureArgs& args, GLTextureArgs* glArgs, Error* error) noexcept
{
    ERROR_CODE_COND_F(args.width == 0, Error::WidthIsZero);
    ERROR_CODE_COND_F(args.height == 0, Error::HeightIsZero);
    ERROR_CODE_COND_F(args.mipmapLevels < 0, Error::MipMapLevelsIsUnset);
    ERROR_CODE_COND_F(args.dataFormat == static_cast<ETexture::Format>(0), Error::DataFormatIsUnset);

    glGenTextures(1, &glArgs->textureHandle);

    return true;
}

GLNullTexture* GLTextureNullBuilder::build(const TextureArgs& args, Error* error) const noexcept
{
    GLNullTexture* const texture = new(std::nothrow) GLNullTexture;
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);
    ERROR_CODE_V(Error::NoError, texture);
}

GLNullTexture* GLTextureNullBuilder::build(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLNullTexture* const texture = allocator.allocateT<GLNullTexture>();
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);
    ERROR_CODE_V(Error::NoError, texture);
}

Ref<ITexture> GLTextureNullBuilder::buildCPPRef(const TextureArgs& args, Error* error) const noexcept
{
    const Ref<GLNullTexture> texture = Ref<GLNullTexture>(new(std::nothrow) GLNullTexture);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);
    ERROR_CODE_V(Error::NoError, texture);
}

NullableReferenceCountingPointer<ITexture> GLTextureNullBuilder::buildTauRef(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    const NullableReferenceCountingPointer<GLNullTexture> texture(allocator);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);
    ERROR_CODE_V(Error::NoError, RCPCast<ITexture>(texture));
}

NullableStrongReferenceCountingPointer<ITexture> GLTextureNullBuilder::buildTauSRef(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    const NullableStrongReferenceCountingPointer<GLNullTexture> texture(allocator);
    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);
    ERROR_CODE_V(Error::NoError, RCPCast<ITexture>(texture));
}

GLDepthTexture* GLTextureDepthBuilder::build(const TextureArgs& args, Error* error) const noexcept
{
    GLTextureArgs glArgs;
    if(!GLTexture2DBuilder::processArgs(args, &glArgs, error))
    { return null; }

    GLDepthTexture* const texture = new(std::nothrow) GLDepthTexture(args.width, args.height, args.dataFormat, glArgs.textureHandle);

    if(!texture)
    {
        glDeleteTextures(1, &glArgs.textureHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    texture->set(0, args.initialBuffer);

    ERROR_CODE_V(Error::NoError, texture);
}

GLDepthTexture* GLTextureDepthBuilder::build(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLTextureArgs glArgs;
    if(!GLTexture2DBuilder::processArgs(args, &glArgs, error))
    { return null; }

    GLDepthTexture* const texture = allocator.allocateT<GLDepthTexture>(args.width, args.height, args.dataFormat, glArgs.textureHandle);

    if(!texture)
    {
        glDeleteTextures(1, &glArgs.textureHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    texture->set(0, args.initialBuffer);

    ERROR_CODE_V(Error::NoError, texture);
}

Ref<ITexture> GLTextureDepthBuilder::buildCPPRef(const TextureArgs& args, Error* error) const noexcept
{
    GLTextureArgs glArgs;
    if(!GLTexture2DBuilder::processArgs(args, &glArgs, error))
    { return null; }

    const Ref<GLDepthTexture> texture = Ref<GLDepthTexture>(new(std::nothrow) GLDepthTexture(args.width, args.height, args.dataFormat, glArgs.textureHandle));

    if(!texture)
    {
        glDeleteTextures(1, &glArgs.textureHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    texture->set(0, args.initialBuffer);

    ERROR_CODE_V(Error::NoError, texture);
}

NullableReferenceCountingPointer<ITexture> GLTextureDepthBuilder::buildTauRef(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLTextureArgs glArgs;
    if(!GLTexture2DBuilder::processArgs(args, &glArgs, error))
    { return null; }

    NullableReferenceCountingPointer<GLDepthTexture> texture(allocator, args.width, args.height, args.dataFormat, glArgs.textureHandle);

    if(!texture)
    {
        glDeleteTextures(1, &glArgs.textureHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    texture->set(0, args.initialBuffer);

    ERROR_CODE_V(Error::NoError, RCPCast<ITexture>(texture));
}

NullableStrongReferenceCountingPointer<ITexture> GLTextureDepthBuilder::buildTauSRef(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLTextureArgs glArgs;
    if(!GLTexture2DBuilder::processArgs(args, &glArgs, error))
    { return null; }

    NullableStrongReferenceCountingPointer<GLDepthTexture> texture(allocator, args.width, args.height, args.dataFormat, glArgs.textureHandle);

    if(!texture)
    {
        glDeleteTextures(1, &glArgs.textureHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    texture->set(0, args.initialBuffer);

    ERROR_CODE_V(Error::NoError, RCPCast<ITexture>(texture));
}

GLint GLTexture2D::glFilterType(const ETexture::Filter filterType) noexcept
{
    switch(filterType)
    {
        case ETexture::Filter::Nearest: return GL_NEAREST;
        case ETexture::Filter::Linear: return GL_LINEAR;
        default: return 0;
    }
}

GLint GLTexture2D::glWrapMode(const ETexture::WrapMode wrapMode) noexcept
{
    switch(wrapMode)
    {
        case ETexture::WrapMode::ClampToEdge: return GL_CLAMP_TO_EDGE;
        case ETexture::WrapMode::ClampToBorder: return GL_CLAMP_TO_BORDER;
        case ETexture::WrapMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
        case ETexture::WrapMode::Repeat: return GL_REPEAT;
        case ETexture::WrapMode::MirrorClampToEdge: return GL_MIRROR_CLAMP_TO_EDGE;
        default: return 0;
    }
}

GLint GLTexture2D::glDepthCompareFunc(const ETexture::DepthCompareFunc compareFunc) noexcept
{
    switch(compareFunc)
    {
        case ETexture::DepthCompareFunc::LessThanOrEqual: return GL_LEQUAL;
        case ETexture::DepthCompareFunc::GreaterThanOrEqual: return GL_GEQUAL;
        case ETexture::DepthCompareFunc::LessThan: return GL_LESS;
        case ETexture::DepthCompareFunc::GreaterThan: return GL_GREATER;
        case ETexture::DepthCompareFunc::Equal: return GL_EQUAL;
        case ETexture::DepthCompareFunc::NotEqual: return GL_NOTEQUAL;
        case ETexture::DepthCompareFunc::Always: return GL_ALWAYS;
        case ETexture::DepthCompareFunc::Never: return GL_NEVER;
        default: return 0;
    }
}

GLint GLTexture2D::glInternalFormat(const ETexture::Format format) noexcept
{
    switch(format)
    {
        case ETexture::Format::Red8UnsignedInt: return GL_R8;
        case ETexture::Format::Red16UnsignedInt: return GL_R16;
        case ETexture::Format::Red32UnsignedInt: return GL_R32UI;
        case ETexture::Format::RedGreen8UnsignedInt:  return GL_RG8;
        case ETexture::Format::RedGreen16UnsignedInt: return GL_RG16;
        case ETexture::Format::RedGreen32UnsignedInt: return GL_RG32UI;
        case ETexture::Format::RedGreenBlue8UnsignedInt:  return GL_RGB8;
        case ETexture::Format::RedGreenBlue16UnsignedInt: return GL_RGB16;
        case ETexture::Format::RedGreenBlue32UnsignedInt: return GL_RGB32UI;
        case ETexture::Format::RedGreenBlueAlpha8UnsignedInt:  return GL_RGBA8;
        case ETexture::Format::RedGreenBlueAlpha16UnsignedInt: return GL_RGBA16;
        case ETexture::Format::RedGreenBlueAlpha32UnsignedInt: return GL_RGBA32UI;
        case ETexture::Format::Red16Float: return GL_R16F;
        case ETexture::Format::Red32Float: return GL_R32F;
        case ETexture::Format::RedGreen16Float: return GL_RG16F;
        case ETexture::Format::RedGreen32Float: return GL_RG32F;
        case ETexture::Format::RedGreenBlue16Float: return GL_RGB16F;
        case ETexture::Format::RedGreenBlue32Float: return GL_RGB32F;
        case ETexture::Format::RedGreenBlueAlpha16Float: return GL_RGBA16F;
        case ETexture::Format::RedGreenBlueAlpha32Float: return GL_RGBA32F;
        case ETexture::Format::Depth16UnsignedInt: return GL_DEPTH_COMPONENT16;
        case ETexture::Format::Depth32UnsignedInt: return GL_DEPTH_COMPONENT32;
        case ETexture::Format::Depth32Float: return GL_DEPTH_COMPONENT32F;
        case ETexture::Format::Stencil8UnsignedInt: return GL_STENCIL_INDEX8;
        case ETexture::Format::Stencil16UnsignedInt: return GL_STENCIL_INDEX16;
        case ETexture::Format::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
        case ETexture::Format::Depth32FloatStencil8UnsignedInt: return GL_DEPTH32F_STENCIL8;
        default: return 0;
    }
}

GLenum GLTexture2D::glInputFormat(const ETexture::Format format) noexcept
{
    switch(format)
    {
        case ETexture::Format::Red8UnsignedInt: return GL_RED;
        case ETexture::Format::Red16UnsignedInt: return GL_RED;
        case ETexture::Format::Red32UnsignedInt: return GL_RED;
        case ETexture::Format::RedGreen8UnsignedInt:  return GL_RG;
        case ETexture::Format::RedGreen16UnsignedInt: return GL_RG;
        case ETexture::Format::RedGreen32UnsignedInt: return GL_RG;
        case ETexture::Format::RedGreenBlue8UnsignedInt:  return GL_RGB;
        case ETexture::Format::RedGreenBlue16UnsignedInt: return GL_RGB;
        case ETexture::Format::RedGreenBlue32UnsignedInt: return GL_RGB;
        case ETexture::Format::RedGreenBlueAlpha8UnsignedInt:  return GL_RGBA;
        case ETexture::Format::RedGreenBlueAlpha16UnsignedInt: return GL_RGBA;
        case ETexture::Format::RedGreenBlueAlpha32UnsignedInt: return GL_RGBA;
        case ETexture::Format::Red16Float: return GL_RED;
        case ETexture::Format::Red32Float: return GL_RED;
        case ETexture::Format::RedGreen16Float: return GL_RG;
        case ETexture::Format::RedGreen32Float: return GL_RG;
        case ETexture::Format::RedGreenBlue16Float: return GL_RGB;
        case ETexture::Format::RedGreenBlue32Float: return GL_RGB;
        case ETexture::Format::RedGreenBlueAlpha16Float: return GL_RGB;
        case ETexture::Format::RedGreenBlueAlpha32Float: return GL_RGB;
        case ETexture::Format::Depth16UnsignedInt: return GL_DEPTH_COMPONENT;
        case ETexture::Format::Depth32UnsignedInt: return GL_DEPTH_COMPONENT;
        case ETexture::Format::Depth32Float: return GL_DEPTH_COMPONENT;
        case ETexture::Format::Stencil8UnsignedInt: return GL_STENCIL_INDEX;
        case ETexture::Format::Stencil16UnsignedInt: return GL_STENCIL_INDEX;
        case ETexture::Format::Depth24Stencil8: return GL_DEPTH_STENCIL;
        case ETexture::Format::Depth32FloatStencil8UnsignedInt: return GL_DEPTH_STENCIL;
        default: return 0;
    }
}

GLenum GLTexture2D::glInputDataType(const ETexture::Format format) noexcept
{
    switch(format)
    {
        case ETexture::Format::Red8UnsignedInt: return GL_UNSIGNED_BYTE;
        case ETexture::Format::Red16UnsignedInt: return GL_UNSIGNED_SHORT;
        case ETexture::Format::Red32UnsignedInt: return GL_UNSIGNED_INT;
        case ETexture::Format::RedGreen8UnsignedInt:  return GL_UNSIGNED_BYTE;
        case ETexture::Format::RedGreen16UnsignedInt: return GL_UNSIGNED_SHORT;
        case ETexture::Format::RedGreen32UnsignedInt: return GL_UNSIGNED_INT;
        case ETexture::Format::RedGreenBlue8UnsignedInt:  return GL_UNSIGNED_BYTE;
        case ETexture::Format::RedGreenBlue16UnsignedInt: return GL_UNSIGNED_SHORT;
        case ETexture::Format::RedGreenBlue32UnsignedInt: return GL_UNSIGNED_INT;
        case ETexture::Format::RedGreenBlueAlpha8UnsignedInt:  return GL_UNSIGNED_BYTE;
        case ETexture::Format::RedGreenBlueAlpha16UnsignedInt: return GL_UNSIGNED_SHORT;
        case ETexture::Format::RedGreenBlueAlpha32UnsignedInt: return GL_UNSIGNED_INT;
        case ETexture::Format::Red16Float: return GL_HALF_FLOAT;
        case ETexture::Format::Red32Float: return GL_FLOAT;
        case ETexture::Format::RedGreen16Float: return GL_HALF_FLOAT;
        case ETexture::Format::RedGreen32Float: return GL_FLOAT;
        case ETexture::Format::RedGreenBlue16Float: return GL_HALF_FLOAT;
        case ETexture::Format::RedGreenBlue32Float: return GL_FLOAT;
        case ETexture::Format::RedGreenBlueAlpha16Float: return GL_HALF_FLOAT;
        case ETexture::Format::RedGreenBlueAlpha32Float: return GL_FLOAT;
        case ETexture::Format::Depth16UnsignedInt: return GL_UNSIGNED_SHORT;
        case ETexture::Format::Depth32UnsignedInt: return GL_UNSIGNED_INT;
        case ETexture::Format::Depth32Float: return GL_FLOAT;
        case ETexture::Format::Stencil8UnsignedInt: return GL_UNSIGNED_BYTE;
        case ETexture::Format::Stencil16UnsignedInt: return GL_UNSIGNED_SHORT;
        case ETexture::Format::Depth24Stencil8: return GL_UNSIGNED_INT_24_8;
        case ETexture::Format::Depth32FloatStencil8UnsignedInt: return  GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
        default: return 0;
    }
}
