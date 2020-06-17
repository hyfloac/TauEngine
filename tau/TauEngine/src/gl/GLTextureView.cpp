#include "gl/GLTextureView.hpp"
#include "gl/GLResourceTexture.hpp"
#include "gl/GLDescriptorHeap.hpp"
#include <EnumBitFields.hpp>

void GLTextureView::generateMipmaps(IRenderingContext&) noexcept
{
    glBindTexture(_target, _texture);
    glGenerateMipmap(_target);
}

GLTextureView* GLTextureViewBuilder::build(const TextureViewArgs& args, Error* const error, const DescriptorTable table, const uSys tableIndex) const noexcept
{
    ERROR_CODE_COND_N(!table.raw, Error::DescriptorTableIsNull);
    GLDescriptorTable* const glTable = reinterpret_cast<GLDescriptorTable*>(table.raw);
    ERROR_CODE_COND_N(glTable->type() != DescriptorType::TextureView, Error::InternalError);

    GLTextureViewArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    if(glArgs.genMipmaps)
    {
        GLNoMipmapTextureView* const textureView = new(glTable->placement() + (tableIndex * sizeof(GLTextureView))) GLNoMipmapTextureView(glArgs);
        ERROR_CODE_V(Error::NoError, textureView);
    }
    else
    {
        GLTextureView* const textureView = new(glTable->placement() + (tableIndex * sizeof(GLTextureView))) GLTextureView(glArgs);
        ERROR_CODE_V(Error::NoError, textureView);
    }
}

bool GLTextureViewBuilder::processArgs(const TextureViewArgs& args, GLTextureViewArgs* glArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(!args.texture, Error::TextureIsNull);
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture1D, Error::InvalidTexture);
    ERROR_CODE_COND_F(args.dataFormat < ETexture::Format::MIN || args.dataFormat > ETexture::Format::MAX, Error::InvalidDataFormat);
    ERROR_CODE_COND_F(args.dataFormat >= ETexture::Format::MIN_TYPELESS && args.dataFormat <= ETexture::Format::MAX_TYPELESS, Error::InvalidDataFormat);

    GLResource* const glResource = RTTD_CAST(args.texture, GLResource, IResource);
    GLResourceTexture* const texture = static_cast<GLResourceTexture* const>(glResource);

    glArgs->dataFormat = args.dataFormat;
    glArgs->texture = texture->texture();

    switch(args.type)
    {
        case ETexture::Type::Texture1D:        return processArgs1D(args, glArgs, texture, error);
        case ETexture::Type::Texture1DArray:   return processArgs1DArray(args, glArgs, texture, error);
        case ETexture::Type::Texture2D:        return processArgs2D(args, glArgs, texture, error);
        case ETexture::Type::Texture2DArray:   return processArgs2DArray(args, glArgs, texture, error);
        case ETexture::Type::Texture3D:        return processArgs3D(args, glArgs, texture, error);
        case ETexture::Type::TextureCube:      return processArgsCube(args, glArgs, texture, error);
        case ETexture::Type::TextureCubeArray: return processArgsCubeArray(args, glArgs, texture, error);
        default: return false;
    }
}

bool GLTextureViewBuilder::processArgs1D(const TextureViewArgs& args, GLTextureViewArgs* const glArgs, GLResourceTexture* const texture, Error* const error) const noexcept
{
    const ResourceTexture1DArgs* texArgs = texture->getArgs<ResourceTexture1DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);

    glArgs->type = ETexture::Type::Texture1D;
    glArgs->width = texArgs->width;
    glArgs->height = 0;
    glArgs->depth = 0;
    glArgs->mipLevels = texArgs->mipLevels;
    glArgs->target = GL_TEXTURE_1D;
    glArgs->genMipmaps = hasFlag(texArgs->flags, ETexture::BindFlags::GenerateMipmaps);

    return true;
}

bool GLTextureViewBuilder::processArgs1DArray(const TextureViewArgs& args, GLTextureViewArgs* const glArgs, GLResourceTexture* const texture, Error* const error) const noexcept
{
    const ResourceTexture1DArgs* texArgs = args.texture->getArgs<ResourceTexture1DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);

    glArgs->type = ETexture::Type::Texture1DArray;
    glArgs->width = texArgs->width;
    glArgs->height = 0;
    glArgs->arrayCount = texArgs->arrayCount;
    glArgs->mipLevels = texArgs->mipLevels;
    glArgs->target = GL_TEXTURE_1D_ARRAY;
    glArgs->genMipmaps = hasFlag(texArgs->flags, ETexture::BindFlags::GenerateMipmaps);

    return true;
}

bool GLTextureViewBuilder::processArgs2D(const TextureViewArgs& args, GLTextureViewArgs* const glArgs, GLResourceTexture* const texture, Error* const error) const noexcept
{
    const ResourceTexture2DArgs* texArgs = args.texture->getArgs<ResourceTexture2DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);

    glArgs->type = ETexture::Type::Texture2D;
    glArgs->width = texArgs->width;
    glArgs->height = texArgs->height;
    glArgs->depth = 0;
    glArgs->mipLevels = texArgs->mipLevels;
    glArgs->target = GL_TEXTURE_2D;
    glArgs->genMipmaps = hasFlag(texArgs->flags, ETexture::BindFlags::GenerateMipmaps);

    return true;
}

bool GLTextureViewBuilder::processArgs2DArray(const TextureViewArgs& args, GLTextureViewArgs* const glArgs, GLResourceTexture* const texture, Error* const error) const noexcept
{
    const ResourceTexture2DArgs* texArgs = args.texture->getArgs<ResourceTexture2DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);

    glArgs->type = ETexture::Type::Texture2DArray;
    glArgs->width = texArgs->width;
    glArgs->height = texArgs->height;
    glArgs->arrayCount = texArgs->arrayCount;
    glArgs->mipLevels = texArgs->mipLevels;
    glArgs->target = GL_TEXTURE_2D_ARRAY;
    glArgs->genMipmaps = hasFlag(texArgs->flags, ETexture::BindFlags::GenerateMipmaps);

    return true;
}

bool GLTextureViewBuilder::processArgs3D(const TextureViewArgs& args, GLTextureViewArgs* const glArgs, GLResourceTexture* const texture, Error* const error) const noexcept
{
    const ResourceTexture3DArgs* texArgs = args.texture->getArgs<ResourceTexture3DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);

    glArgs->type = ETexture::Type::Texture3D;
    glArgs->width = texArgs->width;
    glArgs->height = texArgs->height;
    glArgs->depth = texArgs->depth;
    glArgs->mipLevels = texArgs->mipLevels;
    glArgs->target = GL_TEXTURE_3D;
    glArgs->genMipmaps = hasFlag(texArgs->flags, ETexture::BindFlags::GenerateMipmaps);

    return true;
}

bool GLTextureViewBuilder::processArgsCube(const TextureViewArgs& args, GLTextureViewArgs* const glArgs, GLResourceTexture* const texture, Error* const error) const noexcept
{
    const ResourceTexture2DArgs* texArgs = args.texture->getArgs<ResourceTexture2DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);
    ERROR_CODE_COND_F(texArgs->arrayCount != 6, Error::TextureIsNotArray);

    glArgs->type = ETexture::Type::TextureCube;
    glArgs->width = texArgs->width;
    glArgs->height = texArgs->height;
    glArgs->depth = 0;
    glArgs->mipLevels = texArgs->mipLevels;
    glArgs->target = GL_TEXTURE_CUBE_MAP;
    glArgs->genMipmaps = hasFlag(texArgs->flags, ETexture::BindFlags::GenerateMipmaps);

    return true;
}

bool GLTextureViewBuilder::processArgsCubeArray(const TextureViewArgs& args, GLTextureViewArgs* const glArgs, GLResourceTexture* const texture, Error* const error) const noexcept
{
    const ResourceTexture2DArgs* texArgs = args.texture->getArgs<ResourceTexture2DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);
    ERROR_CODE_COND_F(texArgs->arrayCount % 6 != 0, Error::TextureIsNotArray);

    glArgs->type = ETexture::Type::TextureCubeArray;
    glArgs->width = texArgs->width;
    glArgs->height = texArgs->height;
    glArgs->arrayCount = texArgs->arrayCount;
    glArgs->mipLevels = texArgs->mipLevels;
    glArgs->target = GL_TEXTURE_CUBE_MAP_ARRAY;
    glArgs->genMipmaps = hasFlag(texArgs->flags, ETexture::BindFlags::GenerateMipmaps);

    return true;
}
