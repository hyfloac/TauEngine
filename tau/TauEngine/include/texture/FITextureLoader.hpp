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
    static void setMissingTexture(const Ref<ITexture>& missingTexture) noexcept;
    static Ref<ITexture> getMissingTexture() noexcept;

    static Ref<ITexture> generateMissingTexture(IRenderingContext& context) noexcept;

    static Ref<ITexture> generateDebugTexture8(IRenderingContext& context, uSys power) noexcept;
    static Ref<ITexture> generateDebugTexture16(IRenderingContext& context, uSys power) noexcept;
    static Ref<ITexture> generateDebugTexture(IRenderingContext& context, const uSys power) noexcept
    { return generateDebugTexture8(context, power); }
	
    static Ref<ITexture> generateColorTexture(IRenderingContext& context, RGBColor color) noexcept;

    static Ref<ITexture> generateWhiteTexture(IRenderingContext& context) noexcept
    { return generateColorTexture(context, { 255, 255, 255 }); }

    static Ref<ITexture> generateBlackTexture(IRenderingContext& context) noexcept
    { return generateColorTexture(context, { 0, 0, 0 }); }

    static Ref<ITexture> generateNormalTexture(IRenderingContext& context) noexcept
    { return generateColorTexture(context, { 127, 127, 255 }); }

    static Ref<ITexture> loadTextureEx(IRenderingContext& context, const char* RESTRICT fileName, i32 mipmapLevel, TextureLoadError* RESTRICT error = null) noexcept;
    static Ref<ITexture> loadTexture(IRenderingContext& context, const char* RESTRICT fileName, TextureLoadError* RESTRICT error = null) noexcept
    { return loadTextureEx(context, fileName, -1, error); }

    static Ref<ITextureCube> loadTextureCubeEx(IRenderingContext & context, const char* RESTRICT folderPath, const char* RESTRICT fileExtension, i32 mipmapLevel, TextureLoadError* RESTRICT error = null) noexcept;
    static Ref<ITextureCube> loadTextureCube(IRenderingContext& context, const char* RESTRICT folderPath, const char* RESTRICT fileExtension, TextureLoadError* RESTRICT error = null) noexcept
    { return loadTextureCubeEx(context, folderPath, fileExtension, -1, error); }
};
