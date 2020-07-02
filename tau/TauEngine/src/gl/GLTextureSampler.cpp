#include "gl/GLTextureSampler.hpp"
#include "gl/GLDescriptorHeap.hpp"

TextureSampler GLTextureSamplerBuilder::build(const TextureSamplerArgs& args, DescriptorSamplerTable table, uSys tableIndex, Error* error) const noexcept
{
    ERROR_CODE_COND_N(!table.raw, Error::DescriptorTableIsNull);
    GLDescriptorSamplerTable* const glTable = table.get<GLDescriptorSamplerTable>();

    GLTextureSamplerArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    glTable->samplers()[tableIndex] = glArgs.sampler;
    ERROR_CODE_V(Error::NoError, &glTable->samplers()[tableIndex]);
}

bool GLTextureSamplerBuilder::processArgs(const TextureSamplerArgs& args, GLTextureSamplerArgs* const glArgs, Error* const error) noexcept
{
    ERROR_CODE_COND_F(args.magFilter() == static_cast<ETexture::Filter>(0), Error::FilterIsUnset);
    ERROR_CODE_COND_F(args.minFilter() == static_cast<ETexture::Filter>(0), Error::FilterIsUnset);
    ERROR_CODE_COND_F(args.mipFilter() == static_cast<ETexture::Filter>(0), Error::FilterIsUnset);
    ERROR_CODE_COND_F(args.wrapU == static_cast<ETexture::WrapMode>(0), Error::WrapModeIsUnset);
    ERROR_CODE_COND_F(args.wrapV == static_cast<ETexture::WrapMode>(0), Error::WrapModeIsUnset);
    ERROR_CODE_COND_F(args.wrapW == static_cast<ETexture::WrapMode>(0), Error::WrapModeIsUnset);
    ERROR_CODE_COND_F(args.depthCompareFunc == static_cast<ETexture::CompareFunc>(0), Error::DepthComparisonIsUnset);

    glGenSamplers(1, &glArgs->sampler);

    glSamplerParameteri(glArgs->sampler, GL_TEXTURE_MAG_FILTER, glFilterType(args.magFilter()));
    glSamplerParameteri(glArgs->sampler, GL_TEXTURE_MAG_FILTER, glMipMinType(args.minFilter(), args.mipFilter()));

    glSamplerParameteri(glArgs->sampler, GL_TEXTURE_WRAP_S, glWrapMode(args.wrapU));
    glSamplerParameteri(glArgs->sampler, GL_TEXTURE_WRAP_T, glWrapMode(args.wrapV));
    glSamplerParameteri(glArgs->sampler, GL_TEXTURE_WRAP_R, glWrapMode(args.wrapW));

    glSamplerParameteri(glArgs->sampler, GL_TEXTURE_COMPARE_MODE, args.depthCompareFunc == ETexture::CompareFunc::Never ? GL_NONE : GL_COMPARE_REF_TO_TEXTURE);
    glSamplerParameteri(glArgs->sampler, GL_TEXTURE_COMPARE_FUNC, glDepthCompareFunc(args.depthCompareFunc));

    GLfloat borderColor[4];

    borderColor[0] = static_cast<GLfloat>(args.borderColor.r) / 255.0f;
    borderColor[1] = static_cast<GLfloat>(args.borderColor.g) / 255.0f;
    borderColor[2] = static_cast<GLfloat>(args.borderColor.b) / 255.0f;
    borderColor[3] = static_cast<GLfloat>(args.borderColor.a) / 255.0f;

    glSamplerParameterfv(glArgs->sampler, GL_TEXTURE_BORDER_COLOR, borderColor);

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
        case ETexture::WrapMode::ClampToEdge:       return GL_CLAMP_TO_EDGE;
        case ETexture::WrapMode::ClampToBorder:     return GL_CLAMP_TO_BORDER;
        case ETexture::WrapMode::MirroredRepeat:    return GL_MIRRORED_REPEAT;
        case ETexture::WrapMode::Repeat:            return GL_REPEAT;
        case ETexture::WrapMode::MirrorClampToEdge: return GL_MIRROR_CLAMP_TO_EDGE;
        default: return 0;
    }
}

GLint GLTextureSamplerBuilder::glDepthCompareFunc(const ETexture::CompareFunc compareFunc) noexcept
{
    switch(compareFunc)
    {
        case ETexture::CompareFunc::LessThanOrEqual:    return GL_LEQUAL;
        case ETexture::CompareFunc::GreaterThanOrEqual: return GL_GEQUAL;
        case ETexture::CompareFunc::LessThan:           return GL_LESS;
        case ETexture::CompareFunc::GreaterThan:        return GL_GREATER;
        case ETexture::CompareFunc::Equal:              return GL_EQUAL;
        case ETexture::CompareFunc::NotEqual:           return GL_NOTEQUAL;
        case ETexture::CompareFunc::Always:             return GL_ALWAYS;
        case ETexture::CompareFunc::Never:              return GL_NEVER;
        default:                                        return 0;
    }
}
