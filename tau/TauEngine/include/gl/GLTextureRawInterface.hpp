#pragma once

#include "texture/TextureRawInterface.hpp"

class TAU_DLL GLTextureRawInterface final : public ITextureRawInterface
{
    DEFAULT_DESTRUCT(GLTextureRawInterface);
    DEFAULT_CM_PU(GLTextureRawInterface);
private:
    GLuint _texture;
public:
    GLTextureRawInterface(const GLuint texture) noexcept
        : _texture(texture)
    { }

    GLTextureRawInterface& operator=(const GLuint texture) noexcept
    {
        _texture = texture;
        return *this;
    }

    [[nodiscard]] void* rawHandle() const noexcept override { return reinterpret_cast<void*>(static_cast<uintptr_t>(_texture)); }

    [[nodiscard]] GLuint glTexture() const noexcept override { return _texture; }
};
