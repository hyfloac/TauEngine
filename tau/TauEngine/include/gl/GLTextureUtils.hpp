#pragma once

#include <GL/glew.h>
#include "texture/TextureEnums.hpp"

namespace GLTextureUtils {

GLint glFilterType(ETexture::Filter filterType) noexcept;
GLint glWrapMode(ETexture::WrapMode wrapMode) noexcept;
GLint glDepthCompareFunc(ETexture::CompareFunc compareFunc) noexcept;
GLint glInternalFormat(ETexture::Format format) noexcept;
GLenum glInputFormat(ETexture::Format format) noexcept;
GLenum glInputDataType(ETexture::Format format) noexcept;
GLenum glCubeMapFace(ETexture::CubeSide cubeSide) noexcept;

}
