#include "gl/GLTexture.hpp"
#include <Safeties.hpp>

GLTextureCube::GLTextureCube(const u32 width, const u32 height, const ETexture::Format dataFormat, const GLuint texture) noexcept
    : ITextureCube(width, height, dataFormat), 
      _texture(texture)
{
    glGenTextures(1, &_texture);
}

GLTextureCube::~GLTextureCube() noexcept
{
    glDeleteTextures(1, &_texture);
}

void GLTextureCube::setCube(const u32 level, ETexture::CubeSide side, const void* data) noexcept
{
    const GLint internalFormat = GLTexture2D::glInternalFormat(_dataFormat);
    const GLenum inputFormat = GLTexture2D::glInputFormat(_dataFormat);
    const GLenum inputDataType = GLTexture2D::glInputDataType(_dataFormat);

    glBindTexture(GL_TEXTURE_CUBE_MAP, _texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(glCubeMapFace(side), level, internalFormat, _width, _height, 0, inputFormat, inputDataType, data);
}

void GLTextureCube::bind(const u8 textureUnit, EShader::Stage) noexcept
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _texture);
}

void GLTextureCube::unbind(const u8 textureUnit, EShader::Stage) noexcept
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void GLTextureCube::generateMipmaps() noexcept
{
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

GLenum GLTextureCube::glCubeMapFace(ETexture::CubeSide cubeSide) noexcept
{
    switch(cubeSide)
    {
        case ETexture::CubeSide::Front:  return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
        case ETexture::CubeSide::Back:   return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
        case ETexture::CubeSide::Left:   return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
        case ETexture::CubeSide::Right:  return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
        case ETexture::CubeSide::Top:    return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
        case ETexture::CubeSide::Bottom: return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
        default: return 0;
    }
}

GLTextureCube* GLTextureCubeBuilder::build(const TextureCubeArgs& args, Error* error) const noexcept
{
    GLTextureArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLTextureCube* const texture = new(std::nothrow) GLTextureCube(args.width, args.height, args.dataFormat, glArgs.textureHandle);

    if(!texture)
    {
        glDeleteTextures(1, &glArgs.textureHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    setupInitial(*texture, const_cast<const void**>(args.initialBuffer));

    ERROR_CODE_V(Error::NoError, texture);
}

GLTextureCube* GLTextureCubeBuilder::build(const TextureCubeArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLTextureArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLTextureCube* const texture = allocator.allocateT<GLTextureCube>(args.width, args.height, args.dataFormat, glArgs.textureHandle);

    if(!texture)
    {
        glDeleteTextures(1, &glArgs.textureHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    setupInitial(*texture, const_cast<const void**>(args.initialBuffer));

    ERROR_CODE_V(Error::NoError, texture);
}

Ref<ITextureCube> GLTextureCubeBuilder::buildCPPRef(const TextureCubeArgs& args, Error* error) const noexcept
{
    GLTextureArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const Ref<GLTextureCube> texture = Ref<GLTextureCube>(new(std::nothrow) GLTextureCube(args.width, args.height, args.dataFormat, glArgs.textureHandle));

    if(!texture)
    {
        glDeleteTextures(1, &glArgs.textureHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    setupInitial(*texture, const_cast<const void**>(args.initialBuffer));

    ERROR_CODE_V(Error::NoError, texture);
}

NullableReferenceCountingPointer<ITextureCube> GLTextureCubeBuilder::buildTauRef(const TextureCubeArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLTextureArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    NullableReferenceCountingPointer<GLTextureCube> texture(allocator, args.width, args.height, args.dataFormat, glArgs.textureHandle);

    if(!texture)
    {
        glDeleteTextures(1, &glArgs.textureHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    setupInitial(*texture, const_cast<const void**>(args.initialBuffer));

    ERROR_CODE_V(Error::NoError, RCPCast<ITextureCube>(texture));
}

NullableStrongReferenceCountingPointer<ITextureCube> GLTextureCubeBuilder::buildTauSRef(const TextureCubeArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLTextureArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    NullableStrongReferenceCountingPointer<GLTextureCube> texture(allocator, args.width, args.height, args.dataFormat, glArgs.textureHandle);

    if(!texture)
    {
        glDeleteTextures(1, &glArgs.textureHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    setupInitial(*texture, const_cast<const void**>(args.initialBuffer));

    ERROR_CODE_V(Error::NoError, RCPCast<ITextureCube>(texture));
}

bool GLTextureCubeBuilder::processArgs(const TextureCubeArgs& args, GLTextureArgs* glArgs, Error* error) noexcept
{
    ERROR_CODE_COND_F(args.width == 0, Error::WidthIsZero);
    ERROR_CODE_COND_F(args.height == 0, Error::HeightIsZero);
    ERROR_CODE_COND_F(args.mipmapLevels < 0, Error::MipMapLevelsIsUnset);
    ERROR_CODE_COND_F(args.dataFormat == static_cast<ETexture::Format>(0), Error::DataFormatIsUnset);

    glGenTextures(1, &glArgs->textureHandle);

    return true;
}

void GLTextureCubeBuilder::setupInitial(GLTextureCube& texture, const void* initialBuffers[6])
{
    for(uSys i = 0; i < 6; ++i)
    {
        texture.setCube(0, static_cast<ETexture::CubeSide>(i + static_cast<uSys>(ETexture::CubeSide::Front)), initialBuffers[i]);
    }
}
