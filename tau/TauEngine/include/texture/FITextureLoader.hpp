#pragma once

#pragma warning(push, 0)
#include <NumTypes.hpp>
#include <Utils.hpp>
#include <DLL.hpp>
#include <Objects.hpp>
#include <Safeties.hpp>
#pragma warning(pop)

#include "texture/Texture.hpp"
#include "Color.hpp"

class IGraphicsInterface;

class TAU_DLL TextureLoader final
{
    DEFAULT_CONSTRUCT_PI(TextureLoader);
    DEFAULT_DESTRUCT(TextureLoader);
    DELETE_COPY(TextureLoader);
public:
    enum class TextureLoadError : u8
    {
        NONE = 0,
        UNKNOWN_FORMAT,
        INVALID_PATH,
        TEXTURE_FAILED_TO_LOAD,
        NULL_TEXTURE_DATA,
        NULL_WIDTH,
        NULL_HEIGHT,
        BITS_PER_PIXEL_TOO_SMALL,
        BITS_PER_PIXEL_TOO_LARGE,
        TEXTURE_SIZES_DONT_MATCH
    };
public:
    static void setMissingTexture(const CPPRef<ITexture2D>& missingTexture) noexcept;
    static CPPRef<ITexture2D> getMissingTexture() noexcept;

    static CPPRef<ITexture2D> generateMissingTexture(IGraphicsInterface& gi, IRenderingContext& context) noexcept;

    static CPPRef<ITexture2D> generateDebugTexture8(IGraphicsInterface& gi, IRenderingContext& context, uSys power) noexcept;
    static CPPRef<ITexture2D> generateDebugTexture16(IGraphicsInterface& gi, IRenderingContext& context, uSys power) noexcept;
    static CPPRef<ITexture2D> generateDebugTexture(IGraphicsInterface& gi, IRenderingContext& context, const uSys power) noexcept
    { return generateDebugTexture8(gi, context, power); }
	
    static CPPRef<ITexture2D> generateColorTexture(IGraphicsInterface& gi, IRenderingContext& context, RGBColor color) noexcept;

    static CPPRef<ITexture2D> generateWhiteTexture(IGraphicsInterface& gi, IRenderingContext& context) noexcept
    { return generateColorTexture(gi, context, { 255, 255, 255 }); }

    static CPPRef<ITexture2D> generateBlackTexture(IGraphicsInterface& gi, IRenderingContext& context) noexcept
    { return generateColorTexture(gi, context, { 0, 0, 0 }); }

    static CPPRef<ITexture2D> generateNormalTexture(IGraphicsInterface& gi, IRenderingContext& context) noexcept
    { return generateColorTexture(gi, context, { 127, 127, 255 }); }

    static CPPRef<ITexture2D> loadTextureEx(IGraphicsInterface& gi, IRenderingContext& context, const char* RESTRICT fileName, i32 mipmapLevel, TextureLoadError* RESTRICT error = null) noexcept;
    static CPPRef<ITexture2D> loadTexture(IGraphicsInterface& gi, IRenderingContext& context, const char* RESTRICT fileName, TextureLoadError* RESTRICT error = null) noexcept
    { return loadTextureEx(gi, context, fileName, -1, error); }

    static CPPRef<ITextureCube> loadTextureCubeEx(IGraphicsInterface& gi, IRenderingContext& context, const char* RESTRICT folderPath, const char* RESTRICT fileExtension, i32 mipmapLevel, TextureLoadError* RESTRICT error = null) noexcept;
    static CPPRef<ITextureCube> loadTextureCube(IGraphicsInterface& gi, IRenderingContext& context, const char* RESTRICT folderPath, const char* RESTRICT fileExtension, TextureLoadError* RESTRICT error = null) noexcept
    { return loadTextureCubeEx(gi, context, folderPath, fileExtension, -1, error); }
};
