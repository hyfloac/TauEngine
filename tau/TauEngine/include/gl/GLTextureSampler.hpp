#pragma once

#include "texture/TextureSampler.hpp"
#pragma warning(push, 0)
#include <GL/glew.h>
#include <cstring>
#pragma warning(pop)

class TAU_DLL GLTextureSampler final : public ITextureSampler
{
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
    GLTextureSampler(const GLint magFilter, const GLint minFilter, const GLint depthCompareMode,
        const GLint depthCompareFunc, const GLint wrapU, const GLint wrapV, const GLint wrapW, GLfloat borderColor[4]) noexcept
        : ITextureSampler(),
          _magFilter(magFilter), _minFilter(minFilter),
          _depthCompareMode(depthCompareMode), _depthCompareFunc(depthCompareFunc),
          _wrapU(wrapU), _wrapV(wrapV), _wrapW(wrapW)
    { ::std::memcpy(_borderColor, borderColor, 4 * sizeof(GLfloat)); }

    void apply(GLenum target) const noexcept;
};

class TAU_DLL GLTextureSamplerBuilder final : public ITextureSamplerBuilder
{
    DEFAULT_CONSTRUCT_PU(GLTextureSamplerBuilder);
    DEFAULT_DESTRUCT(GLTextureSamplerBuilder);
    DELETE_COPY(GLTextureSamplerBuilder);
public:
    static GLint glFilterType(ETexture::Filter filterType) noexcept;
    static GLint glWrapMode(ETexture::WrapMode wrapMode) noexcept;
    static GLint glDepthCompareFunc(ETexture::DepthCompareFunc func) noexcept;
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
    void setFilterMode(ETexture::Filter magnificationFilter, ETexture::Filter minificationFilter, ETexture::Filter mipmapMinificationFilter) noexcept override;
    void setWrapMode(ETexture::WrapMode u, ETexture::WrapMode v, ETexture::WrapMode w) noexcept override;
    void setDepthComparison(ETexture::DepthCompareFunc compareFunc) noexcept override;
    void setBorderColor(RGBAColor color) noexcept override;

    [[nodiscard]] GLTextureSampler* build(Error* error) noexcept override;
};
