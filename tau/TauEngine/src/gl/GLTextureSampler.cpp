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

GLTextureSampler* GLTextureSamplerBuilder::build(const TextureSamplerArgs& args, Error* const error) const noexcept
{
    GLTextureSampler* const sampler = new(::std::nothrow) GLTextureSampler;
    ERROR_CODE_COND_N(!sampler, Error::SystemMemoryAllocationFailure);

    if(!processArgs(args, sampler, error))
    {
        delete sampler;
        return null;
    }

    ERROR_CODE_V(Error::NoError, sampler);
}

GLTextureSampler* GLTextureSamplerBuilder::build(const TextureSamplerArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    GLTextureSampler* const sampler = allocator.allocateT<GLTextureSampler>();
    ERROR_CODE_COND_N(!sampler, Error::SystemMemoryAllocationFailure);

    if(!processArgs(args, sampler, error))
    {
        delete sampler;
        return null;
    }

    ERROR_CODE_V(Error::NoError, sampler);
}

CPPRef<ITextureSampler> GLTextureSamplerBuilder::buildCPPRef(const TextureSamplerArgs& args, Error* const error) const noexcept
{
    CPPRef<GLTextureSampler> const sampler = CPPRef<GLTextureSampler>(new GLTextureSampler);
    ERROR_CODE_COND_N(!sampler, Error::SystemMemoryAllocationFailure);

    if(!processArgs(args, sampler.get(), error))
    { return null; }

    ERROR_CODE_V(Error::NoError, sampler);
}

NullableRef<ITextureSampler> GLTextureSamplerBuilder::buildTauRef(const TextureSamplerArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    NullableRef<GLTextureSampler> sampler(allocator);
    ERROR_CODE_COND_N(!sampler, Error::SystemMemoryAllocationFailure);

    if(!processArgs(args, sampler.get(), error))
    { return null; }

    ERROR_CODE_V(Error::NoError, sampler);
}

NullableStrongRef<ITextureSampler> GLTextureSamplerBuilder::buildTauSRef(const TextureSamplerArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    NullableStrongRef<GLTextureSampler> sampler(allocator);
    ERROR_CODE_COND_N(!sampler, Error::SystemMemoryAllocationFailure);

    if(!processArgs(args, sampler.get(), error))
    { return null; }

    ERROR_CODE_V(Error::NoError, sampler);
}

bool GLTextureSamplerBuilder::processArgs(const TextureSamplerArgs& args, GLTextureSampler* const glArgs, Error* const error) noexcept
{
    ERROR_CODE_COND_F(args.magFilter() == static_cast<ETexture::Filter>(0), Error::FilterIsUnset);
    ERROR_CODE_COND_F(args.minFilter() == static_cast<ETexture::Filter>(0), Error::FilterIsUnset);
    ERROR_CODE_COND_F(args.mipFilter() == static_cast<ETexture::Filter>(0), Error::FilterIsUnset);
    ERROR_CODE_COND_F(args.wrapU == static_cast<ETexture::WrapMode>(0), Error::WrapModeIsUnset);
    ERROR_CODE_COND_F(args.wrapV == static_cast<ETexture::WrapMode>(0), Error::WrapModeIsUnset);
    ERROR_CODE_COND_F(args.wrapW == static_cast<ETexture::WrapMode>(0), Error::WrapModeIsUnset);
    ERROR_CODE_COND_F(args.depthCompareFunc == static_cast<ETexture::CompareFunc>(0), Error::DepthComparisonIsUnset);

    glArgs->_magFilter = glFilterType(args.magFilter());
    glArgs->_minFilter = glMipMinType(args.minFilter(), args.mipFilter());

    glArgs->_wrapU = glWrapMode(args.wrapU);
    glArgs->_wrapV = glWrapMode(args.wrapV);
    glArgs->_wrapW = glWrapMode(args.wrapW);

    glArgs->_depthCompareMode = args.depthCompareFunc == ETexture::CompareFunc::Never ? GL_NONE : GL_COMPARE_REF_TO_TEXTURE;
    glArgs->_depthCompareFunc = glDepthCompareFunc(args.depthCompareFunc);

    glArgs->_borderColor[0] = static_cast<GLfloat>(args.borderColor.r) / 255.0f;
    glArgs->_borderColor[1] = static_cast<GLfloat>(args.borderColor.g) / 255.0f;
    glArgs->_borderColor[2] = static_cast<GLfloat>(args.borderColor.b) / 255.0f;
    glArgs->_borderColor[3] = static_cast<GLfloat>(args.borderColor.a) / 255.0f;

    return true;
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

GLint GLTextureSamplerBuilder::glMipMinType(const ETexture::Filter minFilter, const ETexture::Filter mipFilter) noexcept
{
    if(minFilter == ETexture::Filter::Nearest)
    {
        return mipFilter == ETexture::Filter::Nearest ? GL_NEAREST : GL_NEAREST_MIPMAP_LINEAR;
    }
    else
    {
        return mipFilter == ETexture::Filter::Nearest ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;
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

GLint GLTextureSamplerBuilder::glDepthCompareFunc(const ETexture::CompareFunc compareFunc) noexcept
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
