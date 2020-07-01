#pragma once

#include "texture/TextureSampler.hpp"
#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

class TAU_DLL GLTextureSampler final : public ITextureSampler
{
    DEFAULT_CM_PU(GLTextureSampler);
    TEXTURE_SAMPLER_IMPL(GLTextureSampler);
private:
    GLuint _sampler;
public:
    GLTextureSampler(const GLuint sampler) noexcept
        : _sampler(sampler)
    { }

    ~GLTextureSampler() noexcept
    { glDeleteSamplers(1, &_sampler); }

    [[nodiscard]] GLuint sampler() const noexcept { return _sampler; }
};

class TAU_DLL GLTextureSamplerBuilder final : public ITextureSamplerBuilder
{
    DEFAULT_CONSTRUCT_PU(GLTextureSamplerBuilder);
    DEFAULT_DESTRUCT(GLTextureSamplerBuilder);
    DEFAULT_CM_PU(GLTextureSamplerBuilder);
public:
    struct GLTextureSamplerArgs final
    {
        GLuint sampler;
    };
public:
    static GLint glFilterType(ETexture::Filter filterType) noexcept;
    static GLint glMipMinType(ETexture::Filter minFilter, ETexture::Filter mipFilter) noexcept;
    static GLint glWrapMode(ETexture::WrapMode wrapMode) noexcept;
    static GLint glDepthCompareFunc(ETexture::CompareFunc func) noexcept;
public:
    [[nodiscard]] TextureSampler build(const TextureSamplerArgs& args, DescriptorSamplerTable table, uSys tableIndex, Error* error = null) const noexcept override;
private:
    static bool processArgs(const TextureSamplerArgs& args, [[tau::out]] GLTextureSamplerArgs* glArgs, [[tau::out]] Error* error) noexcept;
};
