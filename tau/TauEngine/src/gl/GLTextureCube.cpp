#include "gl/GLTexture.hpp"
#include <Safeties.hpp>

GLTextureCube::GLTextureCube(const u32 width, const u32 height, const ETexture::Format dataFormat, const GLuint texture) noexcept
    : ITextureCube(width, height, dataFormat), 
      _texture(texture), _minFilter(GL_LINEAR), _magFilter(GL_LINEAR),
      _wrapS(GL_REPEAT), _wrapT(GL_REPEAT), _wrapR(GL_REPEAT)
{
    glGenTextures(1, &_texture);
}

GLTextureCube::~GLTextureCube() noexcept
{
    glDeleteTextures(1, &_texture);
}

// void GLTextureCube::setFilterMode(ETexture::Filter minificationFilter, ETexture::Filter magnificationFilter) noexcept
// {
//     _minFilter = GLTexture2D::glFilterType(minificationFilter);
//     _magFilter = GLTexture2D::glFilterType(magnificationFilter);
// }
//
// void GLTextureCube::setWrapModeCube(const ETexture::WrapMode s, const ETexture::WrapMode t, const ETexture::WrapMode r) noexcept
// {
//     _wrapS = GLTexture2D::glWrapMode(s);
//     _wrapT = GLTexture2D::glWrapMode(t);
//     _wrapR = GLTexture2D::glWrapMode(r);
// }

void GLTextureCube::setCube(const u32 level, ETexture::CubeSide side, const void* data) noexcept
{
    const GLint internalFormat = GLTexture2D::glInternalFormat(_dataFormat);
    const GLenum inputFormat = GLTexture2D::glInputFormat(_dataFormat);
    const GLenum inputDataType = GLTexture2D::glInputDataType(_dataFormat);

    glBindTexture(GL_TEXTURE_CUBE_MAP, _texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, _magFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, _minFilter);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, _wrapS);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, _wrapT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, _wrapR);

    glTexImage2D(glCubeMapFace(side), level, internalFormat, _width, _height, 0, inputFormat, inputDataType, data);
}

void GLTextureCube::bind(u8 textureUnit) noexcept
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _texture);
}

void GLTextureCube::unbind(u8 textureUnit) noexcept
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

ITextureCube* GLTextureCubeBuilder::build(Error* error) const noexcept
{
    ERROR_CODE_COND_N(_width == 0, Error::WidthIsZero);
    ERROR_CODE_COND_N(_height == 0, Error::HeightIsZero);
    ERROR_CODE_COND_N(_mipmapLevels < 0, Error::MipMapLevelsIsUnset);
    ERROR_CODE_COND_N(_dataFormat == static_cast<ETexture::Format>(0), Error::DataFormatIsUnset);

    GLuint textureHandle;
    glGenTextures(1, &textureHandle);

    GLTextureCube* const texture = new(std::nothrow) GLTextureCube(_width, _height, _dataFormat, textureHandle);

    if(!textureHandle)
    {
        glDeleteTextures(1, &textureHandle);
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}
