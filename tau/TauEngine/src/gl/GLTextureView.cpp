#include "gl/GLTextureView.hpp"
#include "gl/GLResourceTexture.hpp"
#include "gl/GLDescriptorHeap.hpp"
#include <EnumBitFields.hpp>

TextureView GLTextureViewBuilder::build(const TextureViewArgs& args, DescriptorTable table, uSys tableIndex, Error* const error) const noexcept
{
    ERROR_CODE_COND_N(!table.raw, Error::DescriptorTableIsNull);
    GLDescriptorTable* const glTable = table.get<GLDescriptorTable>();
    ERROR_CODE_COND_N(glTable->type() != DescriptorType::TextureView, Error::InternalError);

    GLTextureViewArgs glArgs;
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLTextureView* view = new(glTable->texViews() + tableIndex) GLTextureView(glArgs.target, glArgs.texture);

    ERROR_CODE_V(Error::NoError, view);
}

bool GLTextureViewBuilder::processArgs(const TextureViewArgs& args, GLTextureViewArgs* glArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(!args.texture, Error::TextureIsNull);
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture1D, Error::InvalidTexture);
    ERROR_CODE_COND_F(args.dataFormat < ETexture::Format::MIN || args.dataFormat > ETexture::Format::MAX, Error::InvalidDataFormat);
    ERROR_CODE_COND_F(args.dataFormat >= ETexture::Format::MIN_TYPELESS && args.dataFormat <= ETexture::Format::MAX_TYPELESS, Error::InvalidDataFormat);

    GLResource* const glResource = RTTD_CAST(args.texture, GLResource, IResource);
    glArgs->texture = static_cast<GLResourceTexture*>(glResource);

    switch(args.type)
    {
        case ETexture::Type::Texture1D:        return        processArgs1D(args, glArgs, error);
        case ETexture::Type::Texture1DArray:   return   processArgs1DArray(args, glArgs, error);
        case ETexture::Type::Texture2D:        return        processArgs2D(args, glArgs, error);
        case ETexture::Type::Texture2DArray:   return   processArgs2DArray(args, glArgs, error);
        case ETexture::Type::Texture3D:        return        processArgs3D(args, glArgs, error);
        case ETexture::Type::TextureCube:      return      processArgsCube(args, glArgs, error);
        case ETexture::Type::TextureCubeArray: return processArgsCubeArray(args, glArgs, error);
        default:                               ERROR_CODE_F(Error::UnsupportedType);
    }
}

bool GLTextureViewBuilder::processArgs1D(const TextureViewArgs& args, GLTextureViewArgs* const glArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture1D, Error::InvalidTexture);

    const ResourceTexture1DArgs* texArgs = glArgs->texture->getArgs<ResourceTexture1DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);

    glArgs->target = GL_TEXTURE_1D;

    return true;
}

bool GLTextureViewBuilder::processArgs1DArray(const TextureViewArgs& args, GLTextureViewArgs* const glArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture1D, Error::InvalidTexture);

    const ResourceTexture1DArgs* texArgs = glArgs->texture->getArgs<ResourceTexture1DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);

    glArgs->target = GL_TEXTURE_1D_ARRAY;

    return true;
}

bool GLTextureViewBuilder::processArgs2D(const TextureViewArgs& args, GLTextureViewArgs* const glArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture2D, Error::InvalidTexture);

    const ResourceTexture2DArgs* texArgs = glArgs->texture->getArgs<ResourceTexture2DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);

    glArgs->target = GL_TEXTURE_2D;

    return true;
}

bool GLTextureViewBuilder::processArgs2DArray(const TextureViewArgs& args, GLTextureViewArgs* const glArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture2D, Error::InvalidTexture);

    const ResourceTexture2DArgs* texArgs = glArgs->texture->getArgs<ResourceTexture2DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);

    glArgs->target = GL_TEXTURE_2D_ARRAY;

    return true;
}

bool GLTextureViewBuilder::processArgs3D(const TextureViewArgs& args, GLTextureViewArgs* const glArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture3D, Error::InvalidTexture);

    const ResourceTexture3DArgs* texArgs = glArgs->texture->getArgs<ResourceTexture3DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);

    glArgs->target = GL_TEXTURE_3D;

    return true;
}

bool GLTextureViewBuilder::processArgsCube(const TextureViewArgs& args, GLTextureViewArgs* const glArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture2D, Error::InvalidTexture);

    const ResourceTexture2DArgs* texArgs = glArgs->texture->getArgs<ResourceTexture2DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);
    ERROR_CODE_COND_F(texArgs->arrayCount != 6, Error::TextureIsNotArray);

    glArgs->target = GL_TEXTURE_CUBE_MAP;

    return true;
}

bool GLTextureViewBuilder::processArgsCubeArray(const TextureViewArgs& args, GLTextureViewArgs* const glArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.texture->resourceType() != EResource::Type::Texture2D, Error::InvalidTexture);

    const ResourceTexture2DArgs* texArgs = glArgs->texture->getArgs<ResourceTexture2DArgs>();
    ERROR_CODE_COND_F(!texArgs, Error::InvalidTexture);
    ERROR_CODE_COND_F(!hasFlag(texArgs->flags, ETexture::BindFlags::ShaderAccess), Error::TextureDoesNotSupportView);
    ERROR_CODE_COND_F(!ETexture::isCompatible(texArgs->dataFormat, args.dataFormat), Error::InvalidDataFormat);
    ERROR_CODE_COND_F(texArgs->arrayCount % 6 != 0, Error::TextureIsNotArray);

    glArgs->target = GL_TEXTURE_CUBE_MAP_ARRAY;

    return true;
}
