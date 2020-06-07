#include "gl/GLTexture.hpp"
#include "gl/GLUtils.hpp"

GLTexture2D::~GLTexture2D() noexcept
{ glDeleteTextures(1, &_texture); }

GLTexture3D::~GLTexture3D() noexcept
{ glDeleteTextures(1, &_texture); }

GLTextureCube::~GLTextureCube() noexcept
{ glDeleteTextures(1, &_texture); }

GLTextureDepthStencil::~GLTextureDepthStencil() noexcept
{ glDeleteTextures(1, &_texture); }

void GLTexture2D::set(const u32 mipLevel, const void* const data) const noexcept
{
    const GLint internalFormat = glInternalFormat(_dataFormat);
    const GLenum inputFormat = glInputFormat(_dataFormat);
    const GLenum inputDataType = glInputDataType(_dataFormat);

    glBindTexture(GL_TEXTURE_2D, _texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, mipLevel, internalFormat, _width, _height, 0, inputFormat, inputDataType, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture3D::set(const u32 depthLevel, const u32 mipLevel, const void* const data) const noexcept
{
    const GLenum inputFormat = GLTexture2D::glInputFormat(_dataFormat);
    const GLenum inputDataType = GLTexture2D::glInputDataType(_dataFormat);

    glBindTexture(GL_TEXTURE_3D, _texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexSubImage3D(GL_TEXTURE_3D, mipLevel, 0, 0, depthLevel, _width, _height, _depth, inputFormat, inputDataType, data);
    glBindTexture(GL_TEXTURE_3D, 0);
}

void GLTextureCube::set(const u32 mipLevel, const ETexture::CubeSide side, const void* const data) const noexcept
{
    const GLint internalFormat = GLTexture2D::glInternalFormat(_dataFormat);
    const GLenum inputFormat = GLTexture2D::glInputFormat(_dataFormat);
    const GLenum inputDataType = GLTexture2D::glInputDataType(_dataFormat);

    glBindTexture(GL_TEXTURE_CUBE_MAP, _texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(glCubeMapFace(side), mipLevel, internalFormat, _width, _height, 0, inputFormat, inputDataType, data);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void GLTextureDepthStencil::set(const void* const data) const noexcept
{
    glBindTexture(GL_TEXTURE_2D, _texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, _width, _height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLTexture2D* GLTextureBuilder::build(const Texture2DArgs& args, Error* error) const noexcept
{
    GLTexture2DArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLTexture2D* const texture = new(::std::nothrow) GLTexture2D(args.width, args.height, args.dataFormat, glArgs.texture);
    if(!texture)
    {
        glDeleteTextures(1, &glArgs.texture);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

GLTexture2D* GLTextureBuilder::build(const Texture2DArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLTexture2DArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLTexture2D* const texture = allocator.allocateT<GLTexture2D>(args.width, args.height, args.dataFormat, glArgs.texture);
    if(!texture)
    {
        glDeleteTextures(1, &glArgs.texture);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

CPPRef<ITexture2D> GLTextureBuilder::buildCPPRef(const Texture2DArgs& args, Error* error) const noexcept
{
    GLTexture2DArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const CPPRef<GLTexture2D> texture(new(::std::nothrow) GLTexture2D(args.width, args.height, args.dataFormat, glArgs.texture));
    if(!texture)
    {
        glDeleteTextures(1, &glArgs.texture);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

NullableRef<ITexture2D> GLTextureBuilder::buildTauRef(const Texture2DArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLTexture2DArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableRef<GLTexture2D> texture(allocator, args.width, args.height, args.dataFormat, glArgs.texture);
    if(!texture)
    {
        glDeleteTextures(1, &glArgs.texture);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

NullableStrongRef<ITexture2D> GLTextureBuilder::buildTauSRef(const Texture2DArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLTexture2DArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableStrongRef<GLTexture2D> texture(allocator, args.width, args.height, args.dataFormat, glArgs.texture);
    if(!texture)
    {
        glDeleteTextures(1, &glArgs.texture);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

GLTexture3D* GLTextureBuilder::build(const Texture3DArgs& args, Error* error) const noexcept
{
    GLTexture3DArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLTexture3D* const texture = new(::std::nothrow) GLTexture3D(args.width, args.height, args.depth, args.dataFormat, glArgs.texture);
    if(!texture)
    {
        glDeleteTextures(1, &glArgs.texture);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

GLTexture3D* GLTextureBuilder::build(const Texture3DArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLTexture3DArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLTexture3D* const texture = allocator.allocateT<GLTexture3D>(args.width, args.height, args.depth, args.dataFormat, glArgs.texture);
    if(!texture)
    {
        glDeleteTextures(1, &glArgs.texture);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

CPPRef<ITexture3D> GLTextureBuilder::buildCPPRef(const Texture3DArgs& args, Error* error) const noexcept
{
    GLTexture3DArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const CPPRef<GLTexture3D> texture(new(::std::nothrow) GLTexture3D(args.width, args.height, args.depth, args.dataFormat, glArgs.texture));
    if(!texture)
    {
        glDeleteTextures(1, &glArgs.texture);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

NullableRef<ITexture3D> GLTextureBuilder::buildTauRef(const Texture3DArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLTexture3DArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableRef<GLTexture3D> texture(allocator, args.width, args.height, args.depth, args.dataFormat, glArgs.texture);
    if(!texture)
    {
        glDeleteTextures(1, &glArgs.texture);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

NullableStrongRef<ITexture3D> GLTextureBuilder::buildTauSRef(const Texture3DArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLTexture3DArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableStrongRef<GLTexture3D> texture(allocator, args.width, args.height, args.depth, args.dataFormat, glArgs.texture);
    if(!texture)
    {
        glDeleteTextures(1, &glArgs.texture);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

GLTextureCube* GLTextureBuilder::build(const TextureCubeArgs& args, Error* error) const noexcept
{
    GLTextureCubeArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLTextureCube* const texture = new(::std::nothrow) GLTextureCube(args.width, args.height, args.dataFormat, glArgs.texture);
    if(!texture)
    {
        glDeleteTextures(1, &glArgs.texture);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

GLTextureCube* GLTextureBuilder::build(const TextureCubeArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLTextureCubeArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLTextureCube* const texture = allocator.allocateT<GLTextureCube>(args.width, args.height, args.dataFormat, glArgs.texture);
    if(!texture)
    {
        glDeleteTextures(1, &glArgs.texture);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

CPPRef<ITextureCube> GLTextureBuilder::buildCPPRef(const TextureCubeArgs& args, Error* error) const noexcept
{
    GLTextureCubeArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const CPPRef<GLTextureCube> texture(new(::std::nothrow) GLTextureCube(args.width, args.height, args.dataFormat, glArgs.texture));
    if(!texture)
    {
        glDeleteTextures(1, &glArgs.texture);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

NullableRef<ITextureCube> GLTextureBuilder::buildTauRef(const TextureCubeArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLTextureCubeArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableRef<GLTextureCube> texture(allocator, args.width, args.height, args.dataFormat, glArgs.texture);
    if(!texture)
    {
        glDeleteTextures(1, &glArgs.texture);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

NullableStrongRef<ITextureCube> GLTextureBuilder::buildTauSRef(const TextureCubeArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLTextureCubeArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableStrongRef<GLTextureCube> texture(allocator, args.width, args.height, args.dataFormat, glArgs.texture);
    if(!texture)
    {
        glDeleteTextures(1, &glArgs.texture);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

GLTextureDepthStencil* GLTextureBuilder::build(const TextureDepthStencilArgs& args, Error* error) const noexcept
{
    GLTextureCubeArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLTextureDepthStencil* const texture = new(::std::nothrow) GLTextureDepthStencil(args.width, args.height, glArgs.texture);
    if(!texture)
    {
        glDeleteTextures(1, &glArgs.texture);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

GLTextureDepthStencil* GLTextureBuilder::build(const TextureDepthStencilArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLTextureCubeArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLTextureDepthStencil* const texture = allocator.allocateT<GLTextureDepthStencil>(args.width, args.height, glArgs.texture);
    if(!texture)
    {
        glDeleteTextures(1, &glArgs.texture);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

CPPRef<ITextureDepthStencil> GLTextureBuilder::buildCPPRef(const TextureDepthStencilArgs& args, Error* error) const noexcept
{
    GLTextureCubeArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const CPPRef<GLTextureDepthStencil> texture(new(::std::nothrow) GLTextureDepthStencil(args.width, args.height, glArgs.texture));
    if(!texture)
    {
        glDeleteTextures(1, &glArgs.texture);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

NullableRef<ITextureDepthStencil> GLTextureBuilder::buildTauRef(const TextureDepthStencilArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLTextureCubeArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableRef<GLTextureDepthStencil> texture(allocator, args.width, args.height, glArgs.texture);
    if(!texture)
    {
        glDeleteTextures(1, &glArgs.texture);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

NullableStrongRef<ITextureDepthStencil> GLTextureBuilder::buildTauSRef(const TextureDepthStencilArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLTextureCubeArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableStrongRef<GLTextureDepthStencil> texture(allocator, args.width, args.height, glArgs.texture);
    if(!texture)
    {
        glDeleteTextures(1, &glArgs.texture);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

bool GLTextureBuilder::processArgs(const Texture2DArgs& args, GLTexture2DArgs* glArgs, Error* error) noexcept
{
    ERROR_CODE_COND_F(args.width == 0, Error::WidthIsZero);
    ERROR_CODE_COND_F(args.height == 0, Error::HeightIsZero);
    ERROR_CODE_COND_F(args.mipmapLevels < 0, Error::MipMapLevelsIsUnset);
    ERROR_CODE_COND_F(args.dataFormat == static_cast<ETexture::Format>(0), Error::DataFormatIsUnset);
    ERROR_CODE_COND_F(args.dataFormat == ETexture::Format::Depth24Stencil8, Error::DataFormatIsInvalid);

    glGenTextures(1, &glArgs->texture);

    const GLint internalFormat = GLTexture2D::glInternalFormat(args.dataFormat);
    const GLenum inputFormat = GLTexture2D::glInputFormat(args.dataFormat);
    const GLenum inputDataType = GLTexture2D::glInputDataType(args.dataFormat);

    glBindTexture(GL_TEXTURE_2D, glArgs->texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, args.width, args.height, 0, inputFormat, inputDataType, args.initialBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

bool GLTextureBuilder::processArgs(const Texture3DArgs& args, GLTexture3DArgs* glArgs, Error* error) noexcept
{
    ERROR_CODE_COND_F(args.width == 0, Error::WidthIsZero);
    ERROR_CODE_COND_F(args.height == 0, Error::HeightIsZero);
    ERROR_CODE_COND_F(args.depth == 0, Error::DepthIsZero);
    ERROR_CODE_COND_F(args.mipmapLevels < 0, Error::MipMapLevelsIsUnset);
    ERROR_CODE_COND_F(args.dataFormat == static_cast<ETexture::Format>(0), Error::DataFormatIsUnset);
    ERROR_CODE_COND_F(args.dataFormat == ETexture::Format::Depth24Stencil8, Error::DataFormatIsInvalid);

    glGenTextures(1, &glArgs->texture);

    const GLint internalFormat = GLTexture2D::glInternalFormat(args.dataFormat);
    const GLenum inputFormat = GLTexture2D::glInputFormat(args.dataFormat);
    const GLenum inputDataType = GLTexture2D::glInputDataType(args.dataFormat);

    glBindTexture(GL_TEXTURE_3D, glArgs->texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    /**
     *   To set a layer of a 3D texture in OpenGL you need to use
     * glTexSubImage3D which does not take in an internal format. Thus
     * you either need to use glTexStorage3D which requires OpenGL 4.2,
     * or use glTexImage3D and pass in no data. GL_PROXY_TEXTURE_3D is
     * a special texture target which appears to basically emulate in
     * glTexStorage3D in older version of OpenGL.
     */
    glTexImage3D(GL_PROXY_TEXTURE_3D, 0, internalFormat, args.width, args.height, args.depth, 0, inputFormat, inputDataType, null);
    glBindTexture(GL_TEXTURE_3D, 0);

    return true;
}

bool GLTextureBuilder::processArgs(const TextureCubeArgs& args, GLTextureCubeArgs* glArgs, Error* error) noexcept
{
    ERROR_CODE_COND_F(args.width == 0, Error::WidthIsZero);
    ERROR_CODE_COND_F(args.height == 0, Error::HeightIsZero);
    ERROR_CODE_COND_F(args.mipmapLevels < 0, Error::MipMapLevelsIsUnset);
    ERROR_CODE_COND_F(args.dataFormat == static_cast<ETexture::Format>(0), Error::DataFormatIsUnset);
    ERROR_CODE_COND_F(args.dataFormat == ETexture::Format::Depth24Stencil8, Error::DataFormatIsInvalid);

    glGenTextures(1, &glArgs->texture);

    const GLint internalFormat = GLTexture2D::glInternalFormat(args.dataFormat);
    const GLenum inputFormat = GLTexture2D::glInputFormat(args.dataFormat);
    const GLenum inputDataType = GLTexture2D::glInputDataType(args.dataFormat);

    glBindTexture(GL_TEXTURE_CUBE_MAP, glArgs->texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if(args.initialBuffers[0] || args.initialBuffers[1] ||
       args.initialBuffers[2] || args.initialBuffers[3] || 
       args.initialBuffers[4] || args.initialBuffers[5])
    {
        glTexImage2D(GLTextureCube::glCubeMapFace(ETexture::CubeSide::Front),  0, internalFormat, args.width, args.height, 0, inputFormat, inputDataType, args.initialBuffers[0]);
        glTexImage2D(GLTextureCube::glCubeMapFace(ETexture::CubeSide::Back),   0, internalFormat, args.width, args.height, 0, inputFormat, inputDataType, args.initialBuffers[1]);
        glTexImage2D(GLTextureCube::glCubeMapFace(ETexture::CubeSide::Left),   0, internalFormat, args.width, args.height, 0, inputFormat, inputDataType, args.initialBuffers[2]);
        glTexImage2D(GLTextureCube::glCubeMapFace(ETexture::CubeSide::Right),  0, internalFormat, args.width, args.height, 0, inputFormat, inputDataType, args.initialBuffers[3]);
        glTexImage2D(GLTextureCube::glCubeMapFace(ETexture::CubeSide::Top),    0, internalFormat, args.width, args.height, 0, inputFormat, inputDataType, args.initialBuffers[4]);
        glTexImage2D(GLTextureCube::glCubeMapFace(ETexture::CubeSide::Bottom), 0, internalFormat, args.width, args.height, 0, inputFormat, inputDataType, args.initialBuffers[5]);
    }
    else
    {
        glTexImage2D(GL_PROXY_TEXTURE_CUBE_MAP, 0, internalFormat, args.width, args.height, 0, inputFormat, inputDataType, null);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return true;
}

bool GLTextureBuilder::processArgs(const TextureDepthStencilArgs& args, GLTextureDepthStencilArgs* glArgs, Error* error) noexcept
{
    ERROR_CODE_COND_F(args.width == 0, Error::WidthIsZero);
    ERROR_CODE_COND_F(args.height == 0, Error::HeightIsZero);

    glGenTextures(1, &glArgs->texture);

    glBindTexture(GL_TEXTURE_2D, glArgs->texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, args.width, args.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, args.initialBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
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

GLint GLTexture2D::glDepthCompareFunc(const ETexture::CompareFunc compareFunc) noexcept
{
    switch(compareFunc)
    {
        case ETexture::CompareFunc::LessThanOrEqual: return GL_LEQUAL;
        case ETexture::CompareFunc::GreaterThanOrEqual: return GL_GEQUAL;
        case ETexture::CompareFunc::LessThan: return GL_LESS;
        case ETexture::CompareFunc::GreaterThan: return GL_GREATER;
        case ETexture::CompareFunc::Equal: return GL_EQUAL;
        case ETexture::CompareFunc::NotEqual: return GL_NOTEQUAL;
        case ETexture::CompareFunc::Always: return GL_ALWAYS;
        case ETexture::CompareFunc::Never: return GL_NEVER;
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
        case ETexture::Format::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
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
        case ETexture::Format::Depth24Stencil8: return GL_DEPTH_STENCIL;
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
        case ETexture::Format::Depth24Stencil8: return GL_UNSIGNED_INT_24_8;
        default: return 0;
    }
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
