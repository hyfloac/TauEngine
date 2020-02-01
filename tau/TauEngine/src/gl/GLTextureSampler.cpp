#include "gl/GLTextureSampler.hpp"
#include <Safeties.hpp>

void GLTextureSampler::apply(const GLenum target) const noexcept
{
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, _magFilter);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, _minFilter);

    glTexParameteri(target, GL_TEXTURE_COMPARE_MODE, _depthCompareMode);
    glTexParameteri(target, GL_TEXTURE_COMPARE_FUNC, _depthCompareFunc);

    glTexParameteri(target, GL_TEXTURE_WRAP_S, _wrapU);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, _wrapV);
    glTexParameteri(target, GL_TEXTURE_WRAP_R, _wrapW);

    glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, _borderColor);
}

void GLTextureSamplerBuilder::setFilterMode(const ETexture::Filter magnificationFilter, const ETexture::Filter minificationFilter, const ETexture::Filter mipmapMinificationFilter) noexcept
{
    _magFilter = glFilterType(magnificationFilter);
    if(minificationFilter == ETexture::Filter::Nearest)
    {
        _minFilter = mipmapMinificationFilter == ETexture::Filter::Nearest ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_LINEAR;
    }
    else
    {
        _minFilter = mipmapMinificationFilter == ETexture::Filter::Nearest ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR_MIPMAP_LINEAR;
    }
}

void GLTextureSamplerBuilder::setWrapMode(const ETexture::WrapMode u, const ETexture::WrapMode v, const ETexture::WrapMode w) noexcept
{
    _wrapU = glWrapMode(u);
    _wrapV = glWrapMode(v);
    _wrapW = glWrapMode(w);
}

void GLTextureSamplerBuilder::setDepthComparison(const ETexture::DepthCompareFunc compareFunc) noexcept
{
    _depthCompareMode = compareFunc == ETexture::DepthCompareFunc::Never ? GL_NONE : GL_COMPARE_REF_TO_TEXTURE;
    _depthCompareFunc = glDepthCompareFunc(compareFunc);
}

void GLTextureSamplerBuilder::setBorderColor(const RGBAColor color) noexcept
{
    _borderColor[0] = static_cast<GLfloat>(color.r) / 255.0f;
    _borderColor[1] = static_cast<GLfloat>(color.g) / 255.0f;
    _borderColor[2] = static_cast<GLfloat>(color.b) / 255.0f;
    _borderColor[3] = static_cast<GLfloat>(color.a) / 255.0f;
}

GLTextureSampler* GLTextureSamplerBuilder::build(Error* const error) noexcept
{
    GLTextureSampler* const sampler = new(::std::nothrow) GLTextureSampler(_magFilter, _minFilter, _depthCompareMode, _depthCompareFunc, _wrapU, _wrapV, _wrapW, _borderColor);

    ERROR_CODE_COND_N(!sampler, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, sampler);
}

GLint GLTextureSamplerBuilder::glFilterType(const ETexture::Filter filterType) noexcept
{
    switch(filterType)
    {
        case ETexture::Filter::Nearest: return GL_NEAREST;
        case ETexture::Filter::Linear: return GL_LINEAR;
        default: return 0;
    }
}

GLint GLTextureSamplerBuilder::glWrapMode(const ETexture::WrapMode wrapMode) noexcept
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

GLint GLTextureSamplerBuilder::glDepthCompareFunc(const ETexture::DepthCompareFunc compareFunc) noexcept
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
