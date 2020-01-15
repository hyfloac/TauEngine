#include "gl/GLTexture.hpp"
#include "gl/GLUtils.hpp"

GLTexture2D::GLTexture2D(const u32 width, const u32 height, const ETexture::Format dataFormat, const GLuint texture) noexcept
    : ITexture(width, height, dataFormat),
      _texture(texture), _minFilter(GL_LINEAR), _magFilter(GL_LINEAR),
      _wrapS(GL_REPEAT), _wrapT(GL_REPEAT)
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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _minFilter);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapT);

    glTexImage2D(GL_TEXTURE_2D, level, internalFormat, _width, _height, 0, inputFormat, inputDataType, data);
}

void GLTexture2D::bind(u8 textureUnit) noexcept
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, _texture);
}

void GLTexture2D::unbind(u8 textureUnit) noexcept
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLDepthTexture::GLDepthTexture(const u32 width, const u32 height, const ETexture::Format dataFormat, const GLuint texture) noexcept
    : GLTexture2D(width, height, dataFormat, texture),
      _depthCompareMode(GL_COMPARE_REF_TO_TEXTURE), _depthCompareFunc(GL_LEQUAL)
{ }

void GLDepthTexture::setDepthComparison(bool enableDepthTest, ETexture::DepthCompareFunc compareFunc) noexcept
{
    _depthCompareMode = enableDepthTest ? GL_COMPARE_REF_TO_TEXTURE : GL_NONE;
    _depthCompareFunc = glDepthCompareFunc(compareFunc);
}

void GLDepthTexture::set(const u32 level, const void* data) noexcept
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, _depthCompareMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, _depthCompareFunc);

    GLTexture2D::set(level , data);
}

void GLTexture2D::setFilterMode(ETexture::Filter minificationFilter, ETexture::Filter magnificationFilter) noexcept
{
    _minFilter = glFilterType(minificationFilter);
    _magFilter = glFilterType(magnificationFilter);
}

void GLTexture2D::setWrapMode(ETexture::WrapMode s, ETexture::WrapMode t) noexcept
{
    _wrapS = glWrapMode(s);
    _wrapT = glWrapMode(t);
}

void GLTexture2D::generateMipmaps() noexcept
{
    glGenerateMipmap(GL_TEXTURE_2D);
}

ITexture* GLTexture2DBuilder::build(Error* error) const noexcept
{
    ERROR_CODE_COND_N(_width == 0, Error::WidthIsZero);
    ERROR_CODE_COND_N(_height == 0, Error::HeightIsZero);
    ERROR_CODE_COND_N(_mipmapLevels < 0, Error::MipMapLevelsIsUnset);
    ERROR_CODE_COND_N(_dataFormat == static_cast<ETexture::Format>(0), Error::DataFormatIsUnset);

    GLuint textureHandle;
    glGenTextures(1, &textureHandle);

    GLTexture2D* const texture = new(std::nothrow) GLTexture2D(_width, _height, _dataFormat, textureHandle);

    if(!textureHandle)
    {
        glDeleteTextures(1, &textureHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

ITexture* GLTextureNullBuilder::build([[tau::out]] Error* error) const noexcept
{
    GLNullTexture* const texture = new(std::nothrow) GLNullTexture;

    ERROR_CODE_COND_N(!texture, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, texture);
}

ITexture* GLTextureDepthBuilder::build(Error* error) const noexcept
{
    ERROR_CODE_COND_N(_width == 0, Error::WidthIsZero);
    ERROR_CODE_COND_N(_height == 0, Error::HeightIsZero);
    ERROR_CODE_COND_N(_mipmapLevels < 0, Error::MipMapLevelsIsUnset);
    ERROR_CODE_COND_N(_dataFormat == static_cast<ETexture::Format>(0), Error::DataFormatIsUnset);

    GLuint textureHandle;
    glGenTextures(1, &textureHandle);

    GLDepthTexture* const texture = new(std::nothrow) GLDepthTexture(_width, _height, _dataFormat, textureHandle);

    if(!textureHandle)
    {
        glDeleteTextures(1, &textureHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
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
        // case ETexture::Format::Red8UnsignedInt: return GL_R8UI;
        // case ETexture::Format::Red16UnsignedInt: return GL_R16UI;
        // case ETexture::Format::Red32UnsignedInt: return GL_R32UI;
        // case ETexture::Format::RedGreen8UnsignedInt:  return GL_RG8UI;
        // case ETexture::Format::RedGreen16UnsignedInt: return GL_RG16UI;
        // case ETexture::Format::RedGreen32UnsignedInt: return GL_RG32UI;
        // case ETexture::Format::RedGreenBlue8UnsignedInt:  return GL_RGB8UI;
        // case ETexture::Format::RedGreenBlue16UnsignedInt: return GL_RGB16UI;
        // case ETexture::Format::RedGreenBlue32UnsignedInt: return GL_RGB32UI;
        // case ETexture::Format::RedGreenBlueAlpha8UnsignedInt:  return GL_RGBA8UI;
        // case ETexture::Format::RedGreenBlueAlpha16UnsignedInt: return GL_RGBA16UI;
        // case ETexture::Format::RedGreenBlueAlpha32UnsignedInt: return GL_RGBA32UI;
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
            // case ETexture::Format::Red8UnsignedInt: return GL_RED_INTEGER;
            // case ETexture::Format::Red16UnsignedInt: return GL_RED_INTEGER;
            // case ETexture::Format::Red32UnsignedInt: return GL_RED_INTEGER;
            // case ETexture::Format::RedGreen8UnsignedInt:  return GL_RG_INTEGER;
            // case ETexture::Format::RedGreen16UnsignedInt: return GL_RG_INTEGER;
            // case ETexture::Format::RedGreen32UnsignedInt: return GL_RG_INTEGER;
            // case ETexture::Format::RedGreenBlue8UnsignedInt:  return GL_RGB_INTEGER;
            // case ETexture::Format::RedGreenBlue16UnsignedInt: return GL_RGB_INTEGER;
            // case ETexture::Format::RedGreenBlue32UnsignedInt: return GL_RGB_INTEGER;
            // case ETexture::Format::RedGreenBlueAlpha8UnsignedInt:  return GL_RGBA_INTEGER;
            // case ETexture::Format::RedGreenBlueAlpha16UnsignedInt: return GL_RGBA_INTEGER;
            // case ETexture::Format::RedGreenBlueAlpha32UnsignedInt: return GL_RGBA_INTEGER;
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

