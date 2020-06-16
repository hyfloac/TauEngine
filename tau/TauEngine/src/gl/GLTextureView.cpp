#include "gl/GLTextureView.hpp"
#include "gl/GLResourceTexture.hpp"
#include <EnumBitFields.hpp>

void GLTextureView::generateMipmaps(IRenderingContext&) noexcept
{
    glBindTexture(_target, _texture);
    glGenerateMipmap(_target);
}

bool GLTextureViewBuilder::processArgs(const Texture1DViewArgs& args, GLTextureViewArgs* glArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(!args.texture, Error::TextureIsNull);
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture1D, Error::InvalidTexture);
    ERROR_CODE_COND_F(args.dataFormat < ETexture::Format::MIN || args.dataFormat > ETexture::Format::MAX, Error::InvalidDataFormat);
    ERROR_CODE_COND_F(args.dataFormat >= ETexture::Format::MIN_TYPELESS && args.dataFormat <= ETexture::Format::MAX_TYPELESS, Error::InvalidDataFormat);

    const ResourceTexture1DArgs* texArgs = args.texture->getArgs<ResourceTexture1DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);

    GLResource* const glResource = RTTD_CAST(args.texture, GLResource, IResource);
    GLResourceTexture1D* const glTexture = static_cast<GLResourceTexture1D* const>(glResource);

    glArgs->dataFormat = args.dataFormat;
    glArgs->target = GL_TEXTURE_1D;
    glArgs->texture = glTexture->texture();
    glArgs->width = texArgs->width;
    glArgs->mipLevels = texArgs->mipLevels;

    return true;
}

bool GLTextureViewBuilder::processArgs(const Texture1DArrayViewArgs& args, GLTextureViewArgs* glArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(!args.texture, Error::TextureIsNull);
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture1D, Error::InvalidTexture);
    ERROR_CODE_COND_F(args.dataFormat < ETexture::Format::MIN || args.dataFormat > ETexture::Format::MAX, Error::InvalidDataFormat);
    ERROR_CODE_COND_F(args.dataFormat >= ETexture::Format::MIN_TYPELESS && args.dataFormat <= ETexture::Format::MAX_TYPELESS, Error::InvalidDataFormat);

    const ResourceTexture1DArgs* texArgs = args.texture->getArgs<ResourceTexture1DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);

    GLResource* const glResource = RTTD_CAST(args.texture, GLResource, IResource);
    GLResourceTexture1D* const glTexture = static_cast<GLResourceTexture1D* const>(glResource);

    glArgs->dataFormat = args.dataFormat;
    glArgs->target = GL_TEXTURE_1D_ARRAY;
    glArgs->texture = glTexture->texture();
    glArgs->width = texArgs->width;
    glArgs->mipLevels = texArgs->mipLevels;
    glArgs->arrayCount = texArgs->arrayCount;

    return true;
}

bool GLTextureViewBuilder::processArgs(const Texture2DViewArgs& args, GLTextureViewArgs* glArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(!args.texture, Error::TextureIsNull);
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture2D, Error::InvalidTexture);
    ERROR_CODE_COND_F(args.dataFormat < ETexture::Format::MIN || args.dataFormat > ETexture::Format::MAX, Error::InvalidDataFormat);
    ERROR_CODE_COND_F(args.dataFormat >= ETexture::Format::MIN_TYPELESS && args.dataFormat <= ETexture::Format::MAX_TYPELESS, Error::InvalidDataFormat);

    const ResourceTexture2DArgs* texArgs = args.texture->getArgs<ResourceTexture2DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);

    GLResource* const glResource = RTTD_CAST(args.texture, GLResource, IResource);
    GLResourceTexture2D* const glTexture = static_cast<GLResourceTexture2D* const>(glResource);

    glArgs->dataFormat = args.dataFormat;
    glArgs->target = GL_TEXTURE_2D;
    glArgs->texture = glTexture->texture();
    glArgs->width = texArgs->width;
    glArgs->height = texArgs->height;
    glArgs->mipLevels = texArgs->mipLevels;

    return true;
}

bool GLTextureViewBuilder::processArgs(const Texture2DArrayViewArgs& args, GLTextureViewArgs* glArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(!args.texture, Error::TextureIsNull);
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture2D, Error::InvalidTexture);
    ERROR_CODE_COND_F(args.dataFormat < ETexture::Format::MIN || args.dataFormat > ETexture::Format::MAX, Error::InvalidDataFormat);
    ERROR_CODE_COND_F(args.dataFormat >= ETexture::Format::MIN_TYPELESS && args.dataFormat <= ETexture::Format::MAX_TYPELESS, Error::InvalidDataFormat);

    const ResourceTexture2DArgs* texArgs = args.texture->getArgs<ResourceTexture2DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);

    GLResource* const glResource = RTTD_CAST(args.texture, GLResource, IResource);
    GLResourceTexture2D* const glTexture = static_cast<GLResourceTexture2D* const>(glResource);

    glArgs->dataFormat = args.dataFormat;
    glArgs->target = GL_TEXTURE_2D_ARRAY;
    glArgs->texture = glTexture->texture();
    glArgs->width = texArgs->width;
    glArgs->height = texArgs->height;
    glArgs->mipLevels = texArgs->mipLevels;
    glArgs->arrayCount = texArgs->arrayCount;

    return true;
}

bool GLTextureViewBuilder::processArgs(const Texture3DViewArgs& args, GLTextureViewArgs* glArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(!args.texture, Error::TextureIsNull);
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture3D, Error::InvalidTexture);
    ERROR_CODE_COND_F(args.dataFormat < ETexture::Format::MIN || args.dataFormat > ETexture::Format::MAX, Error::InvalidDataFormat);
    ERROR_CODE_COND_F(args.dataFormat >= ETexture::Format::MIN_TYPELESS && args.dataFormat <= ETexture::Format::MAX_TYPELESS, Error::InvalidDataFormat);

    const ResourceTexture3DArgs* texArgs = args.texture->getArgs<ResourceTexture3DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);

    GLResource* const glResource = RTTD_CAST(args.texture, GLResource, IResource);
    GLResourceTexture2D* const glTexture = static_cast<GLResourceTexture2D* const>(glResource);

    glArgs->dataFormat = args.dataFormat;
    glArgs->target = GL_TEXTURE_3D;
    glArgs->texture = glTexture->texture();
    glArgs->width = texArgs->width;
    glArgs->height = texArgs->height;
    glArgs->depth = texArgs->depth;
    glArgs->mipLevels = texArgs->mipLevels;

    return true;
}

bool GLTextureViewBuilder::processArgs(const TextureCubeViewArgs& args, GLTextureViewArgs* glArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(!args.texture, Error::TextureIsNull);
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture2D, Error::InvalidTexture);
    ERROR_CODE_COND_F(args.dataFormat < ETexture::Format::MIN || args.dataFormat > ETexture::Format::MAX, Error::InvalidDataFormat);
    ERROR_CODE_COND_F(args.dataFormat >= ETexture::Format::MIN_TYPELESS && args.dataFormat <= ETexture::Format::MAX_TYPELESS, Error::InvalidDataFormat);

    const ResourceTexture2DArgs* texArgs = args.texture->getArgs<ResourceTexture2DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);
    ERROR_CODE_COND_F(texArgs->arrayCount != 6, Error::TextureIsNotArray);

    GLResource* const glResource = RTTD_CAST(args.texture, GLResource, IResource);
    GLResourceTexture2D* const glTexture = static_cast<GLResourceTexture2D* const>(glResource);

    glArgs->dataFormat = args.dataFormat;
    glArgs->target = GL_TEXTURE_CUBE_MAP;
    glArgs->texture = glTexture->texture();
    glArgs->width = texArgs->width;
    glArgs->height = texArgs->height;
    glArgs->mipLevels = texArgs->mipLevels;

    return true;
}

bool GLTextureViewBuilder::processArgs(const TextureCubeArrayViewArgs& args, GLTextureViewArgs* glArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(!args.texture, Error::TextureIsNull);
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture2D, Error::InvalidTexture);
    ERROR_CODE_COND_F(args.dataFormat < ETexture::Format::MIN || args.dataFormat > ETexture::Format::MAX, Error::InvalidDataFormat);
    ERROR_CODE_COND_F(args.dataFormat >= ETexture::Format::MIN_TYPELESS && args.dataFormat <= ETexture::Format::MAX_TYPELESS, Error::InvalidDataFormat);

    const ResourceTexture2DArgs* texArgs = args.texture->getArgs<ResourceTexture2DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);
    ERROR_CODE_COND_F(texArgs->arrayCount % 6 != 0, Error::TextureIsNotArray);

    GLResource* const glResource = RTTD_CAST(args.texture, GLResource, IResource);
    GLResourceTexture2D* const glTexture = static_cast<GLResourceTexture2D* const>(glResource);

    glArgs->dataFormat = args.dataFormat;
    glArgs->target = GL_TEXTURE_CUBE_MAP_ARRAY;
    glArgs->texture = glTexture->texture();
    glArgs->width = texArgs->width;
    glArgs->height = texArgs->height;
    glArgs->mipLevels = texArgs->mipLevels;
    glArgs->arrayCount = texArgs->arrayCount;

    return true;
}
