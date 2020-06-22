#pragma once

#pragma warning(push, 0)
#include <NumTypes.hpp>
#include <Utils.hpp>
#include <DLL.hpp>
#include <Objects.hpp>
#include <Safeties.hpp>
#pragma warning(pop)

#include "graphics/Resource.hpp"
#include "Color.hpp"

class IRenderingContext;
class IGraphicsInterface;

class TAU_DLL TextureLoader final
{
    DELETE_CONSTRUCT(TextureLoader);
    DELETE_DESTRUCT(TextureLoader);
    DELETE_CM(TextureLoader);
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
private:
    static NullableRef<IResource> _missingTexture;
public:
    static void setMissingTexture(const NullableRef<IResource>& missingTexture) noexcept
    { _missingTexture = missingTexture; }
    static NullableRef<IResource> getMissingTexture() noexcept
    { return _missingTexture; }

    static NullableRef<IResource> generateMissingTexture(IGraphicsInterface& gi, IRenderingContext& context) noexcept;

    static NullableRef<IResource> generateDebugTexture8(IGraphicsInterface& gi, IRenderingContext& context, uSys power) noexcept;
    static NullableRef<IResource> generateDebugTexture16(IGraphicsInterface& gi, IRenderingContext& context, uSys power) noexcept;
    static NullableRef<IResource> generateDebugTexture(IGraphicsInterface& gi, IRenderingContext& context, const uSys power) noexcept
    { return generateDebugTexture8(gi, context, power); }
	
    static NullableRef<IResource> generateColorTexture(IGraphicsInterface& gi, IRenderingContext& context, RGBColor color) noexcept;

    static NullableRef<IResource> generateWhiteTexture(IGraphicsInterface& gi, IRenderingContext& context) noexcept
    { return generateColorTexture(gi, context, { 255, 255, 255 }); }

    static NullableRef<IResource> generateBlackTexture(IGraphicsInterface& gi, IRenderingContext& context) noexcept
    { return generateColorTexture(gi, context, { 0, 0, 0 }); }

    static NullableRef<IResource> generateNormalTexture(IGraphicsInterface& gi, IRenderingContext& context) noexcept
    { return generateColorTexture(gi, context, { 127, 127, 255 }); }

    static NullableRef<IResource> loadTexture(IGraphicsInterface& gi, IRenderingContext& context, const char* RESTRICT fileName, TextureLoadError* RESTRICT error = null) noexcept;

    static NullableRef<IResource> loadTextureCube(IGraphicsInterface& gi, IRenderingContext& context, const char* RESTRICT folderPath, const char* RESTRICT fileExtension, TextureLoadError* RESTRICT error = null) noexcept;
};
