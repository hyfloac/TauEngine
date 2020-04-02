#pragma once

#include "texture/TextureSampler.hpp"
#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

class TAU_DLL GLTextureSampler final : public ITextureSampler
{
    DEFAULT_CONSTRUCT_PU(GLTextureSampler);
    DEFAULT_DESTRUCT(GLTextureSampler);
    TEXTURE_SAMPLER_IMPL(GLTextureSampler);
private:
    GLint _magFilter;
    GLint _minFilter;
    GLint _depthCompareMode;
    GLint _depthCompareFunc;
    GLint _wrapU;
    GLint _wrapV;
    GLint _wrapW;
    GLfloat _borderColor[4];
public:
    void apply(GLenum target) const noexcept;
private:
    friend class GLTextureSamplerBuilder;
};

class TAU_DLL GLTextureSamplerBuilder final : public ITextureSamplerBuilder
{
    DEFAULT_CONSTRUCT_PU(GLTextureSamplerBuilder);
    DEFAULT_DESTRUCT(GLTextureSamplerBuilder);
    DELETE_COPY(GLTextureSamplerBuilder);
public:
    static GLint glFilterType(ETexture::Filter filterType) noexcept;
    static GLint glMipMinType(ETexture::Filter minFilter, ETexture::Filter mipFilter) noexcept;
    static GLint glWrapMode(ETexture::WrapMode wrapMode) noexcept;
    static GLint glDepthCompareFunc(ETexture::DepthCompareFunc func) noexcept;
public:
    [[nodiscard]] GLTextureSampler* build(const TextureSamplerArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] GLTextureSampler* build(const TextureSamplerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureSampler> buildCPPRef(const TextureSamplerArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureSampler> buildTauRef(const TextureSamplerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureSampler> buildTauSRef(const TextureSamplerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    static bool processArgs(const TextureSamplerArgs& args, [[tau::out]] GLTextureSampler* glArgs, [[tau::out]] Error* error) noexcept;
};
