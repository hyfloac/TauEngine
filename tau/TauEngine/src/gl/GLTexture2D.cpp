#include <gl/GLTexture.hpp>
#include <gl/GLUtils.hpp>

GLTexture2D::GLTexture2D(const u32 width, const u32 height, const ETexture::Format dataFormat) noexcept
    : ITexture(width, height, dataFormat),
      _texture(0), _minFilter(GL_LINEAR), _magFilter(GL_LINEAR)
{
    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);
}

GLTexture2D::~GLTexture2D() noexcept
{
    glDeleteTextures(1, &_texture);
}

static GLint glFilterType(const ETexture::Filter filterType) noexcept;

void GLTexture2D::setFilterMode(ETexture::Filter minificationFilter, ETexture::Filter magnificationFilter) noexcept
{
    _minFilter = glFilterType(minificationFilter);
    _magFilter = glFilterType(magnificationFilter);
}

static GLint glInternalFormat(const ETexture::Format format) noexcept;
static GLenum glInputFormat(const ETexture::Format format) noexcept;
static GLenum glInputDataType(const ETexture::Format format) noexcept;

void GLTexture2D::set(const void* data) noexcept
{
    const GLint internalFormat = glInternalFormat(_dataFormat);
    const GLenum inputFormat = glInputFormat(_dataFormat);
    const GLenum inputDataType = glInputDataType(_dataFormat);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _minFilter);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, inputFormat, inputDataType, data);
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

static GLint glFilterType(const ETexture::Filter filterType) noexcept
{
    switch(filterType)
    {
        case ETexture::Filter::Nearest: return GL_NEAREST;
        case ETexture::Filter::Linear: return GL_LINEAR;
        default: return 0;
    }
}

static GLint glInternalFormat(const ETexture::Format format) noexcept 
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
        default: return 0;
    }
}

static GLenum glInputFormat(const ETexture::Format format) noexcept
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
        default: return 0;
    }
}

static GLenum glInputDataType(const ETexture::Format format) noexcept
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
        default: return 0;
    }
}
