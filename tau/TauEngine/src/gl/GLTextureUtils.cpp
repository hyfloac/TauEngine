#include "gl/GLTextureUtils.hpp"

namespace GLTextureUtils {

GLint glFilterType(const ETexture::Filter filterType) noexcept
{
    switch(filterType)
    {
        case ETexture::Filter::Nearest: return GL_NEAREST;
        case ETexture::Filter::Linear: return GL_LINEAR;
        default: return 0;
    }
}

GLint glWrapMode(const ETexture::WrapMode wrapMode) noexcept
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

GLint glDepthCompareFunc(const ETexture::CompareFunc compareFunc) noexcept
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

GLint glInternalFormat(const ETexture::Format format) noexcept
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

GLenum glInputFormat(const ETexture::Format format) noexcept
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

GLenum glInputDataType(const ETexture::Format format) noexcept
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

GLenum glCubeMapFace(const ETexture::CubeSide cubeSide) noexcept
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

}
