/**
 * @file
 */
#pragma once

#include <NumTypes.hpp>
#include <TUMaths.hpp>

/**
 *   Various enumerations and utility functions for use with
 * textures.
 */
namespace ETexture {
/**
 * The type of the texture.
 *
 *   This is used by texture views to interpret the raw binary
 * stream.
 */
enum class Type
{
    Texture1D = 1,
    Texture1DArray,
    Texture2D,
    Texture2DArray,
    Texture3D,
    TextureCube,
    TextureCubeArray
};

/**
 * How a texture sampler should filter a texture.
 */
enum class Filter
{
    /**
     * Uses the nearest pixel.
     *
     * This will produce a very pixelated view during magnification.
     */
    Nearest = 1,
    Point = Nearest,
    /**
     * Uses linear interpolation between pixels.
     *
     * This will produce a rather blurry view during magnification.
     */
    Linear
};

/**
 *   How a texture sampler should wrap a texture when
 * coordinates extend beyond the edge of a texture.
 */
enum class WrapMode
{
    /**
     *   Takes the last pixel and repeats until it reaches the end
     * of raster.
     */
    ClampToEdge = 1,
    /**
     *   Produces a colored border when outside the range of the
     * texture.
     */
    ClampToBorder,
    /**
     *   Tiles the texture, but every every tile is a mirror of all
     * other axis.
     */
    MirroredRepeat,
    /**
     * Tiles the texture many times.
     */
    Repeat,
    /**
     * Mirrors the texture once, then clamps to edge.
     *
     *   Never seen anyone use this, can't even find any examples
     * online.
     */
    MirrorClampToEdge
};

/**
 * The possible comparison operations.
 *
 * Mostly used by depth-stencil textures.
 */
enum class CompareFunc
{
    LessThanOrEqual = 1,
    GreaterThanOrEqual,
    LessThan,
    GreaterThan,
    Equal,
    NotEqual,
    Always,
    Never
};

/**
 * The layout of the texture upon upload.
 *
 *   The API may convert the texture in the backend to a more
 * optimal format for sampling.
 */
enum class Format
{
    Red8UnsignedInt = 1,
    Red16UnsignedInt,
    Red32UnsignedInt,
    RedGreen8UnsignedInt,
    RedGreen16UnsignedInt,
    RedGreen32UnsignedInt,
    RedGreenBlue8UnsignedInt,
    RedGreenBlue16UnsignedInt,
    RedGreenBlue32UnsignedInt,
    RedGreenBlueAlpha8UnsignedInt,
    RedGreenBlueAlpha16UnsignedInt,
    RedGreenBlueAlpha32UnsignedInt,
    Red16Float,
    Red32Float,
    RedGreen16Float,
    RedGreen32Float,
    RedGreenBlue16Float,
    RedGreenBlue32Float,
    RedGreenBlueAlpha16Float,
    RedGreenBlueAlpha32Float,
    Depth24Stencil8,
    Depth24Typeless8,
    Typeless24Stencil8,
    Red8Typeless,
    Red16Typeless,
    Red32Typeless,
    RedGreen8Typeless,
    RedGreen16Typeless,
    RedGreen32Typeless,
    RedGreenBlue8Typeless,
    RedGreenBlue16Typeless,
    RedGreenBlue32Typeless,
    RedGreenBlueAlpha8Typeless,
    RedGreenBlueAlpha16Typeless,
    RedGreenBlueAlpha32Typeless,
    Typeless24Typeless8,
    MIN = Red8UnsignedInt,
    MAX = Typeless24Typeless8,
    MIN_TYPELESS = Red8Typeless,
    MAX_TYPELESS = Typeless24Typeless8,
    MIN_UINT = Red8UnsignedInt,
    MAX_UINT = RedGreenBlueAlpha32UnsignedInt,
    MIN_FLOAT = Red16Float,
    MAX_FLOAT = RedGreenBlueAlpha32Float
};

/**
 * Represents all of the sides of a cubemap.
 */
enum class CubeSide
{
    Front = 1,
    Back,
    Left,
    Right,
    Top,
    Bottom
};

/**
 * Controls how a texture resource should be able to be bound.
 */
enum class BindFlags
{
    /**
     * No special bind flags.
     */
    None            = 0,
    /**
     * Allows for the texture to be bound as render target.
     *
     * Used for framebuffers.
     *
     *   Required for mipmap generation. This is because GPU's do
     * not have an intrinsic mipmap generation routine, and are
     * instead generated by the driver using a compute shader.
     */
    RenderTarget    = 1 << 0,
    /**
     * Allows for the texture to be read by a shader.
     */
    ShaderAccess    = 1 << 1,
    /**
     * Required for automatic mipmap generation.
     */
    GenerateMipmaps = 1 << 2
};

/**
 *   Controls how a depth-stencil texture resource should be
 * able to be bound.
 */
enum class DepthStencilBindFlags
{
    /**
     * No special bind flags.
     */
    None                = 0,
    /**
     * Allows for the texture to be bound as render target.
     *
     * Used for framebuffers.
     */
    RenderTarget        = 1 << 0,
    /**
     * Allows for the depth texture to be read by a shader.
     *
     *   This is useful if you only need to render to texture
     * but never need to read the contents (like a depth stencil
     * framebuffer).
     */
    DepthShaderAccess   = 1 << 1,
    /**
     * Allows for the stencil texture to be read by a shader.
     *
     *   This is useful if you only need to render to texture
     * but never need to read the contents (like a depth stencil
     * framebuffer).
     */
    StencilShaderAccess = 1 << 2
};

struct Coord final
{
    u32 x, y, z;
};

enum class Positioning
{
    Relative = 0,
    Exact
};

template<Positioning _Positioning>
struct Box final
{ };

using RBox = Box<Positioning::Relative>;
using EBox = Box<Positioning::Exact>;

template<>
struct Box<Positioning::Relative> final
{
    u32 xPos;
    u32 xSize;
    u32 yPos;
    u32 ySize;
    u32 zPos;
    u32 zSize;
public:
    [[nodiscard]] Box<Positioning::Exact> toExact() const noexcept;
    [[nodiscard]] operator Box<Positioning::Exact>() const noexcept { return toExact(); }
};

template<>
struct Box<Positioning::Exact> final
{
    u32 left;
    u32 right;
    u32 top;
    u32 bottom;
    u32 front;
    u32 back;
public:
    [[nodiscard]] Box<Positioning::Relative> toRelative() const noexcept;
    [[nodiscard]] operator Box<Positioning::Relative>() const noexcept { return toRelative(); }
};

inline Box<Positioning::Exact> Box<Positioning::Relative>::toExact() const noexcept
{
    return { xPos, xPos + xSize, yPos, yPos + ySize, zPos, zPos + zSize };
}

inline Box<Positioning::Relative> Box<Positioning::Exact>::toRelative() const noexcept
{
    return { left, right - left, top, bottom - top, front, back - front };
}

/**
 * Gets the number of bytes per component color of a pixel.
 *
 *   This is different from @link ETexture::bytesPerPixel() @endlink 
 * in that it only returns the number of bytes for one of the
 * color aspects, instead of the number of bytes for the
 * entire pixel.
 *
 * @param[in] format
 *      The format in question.
 */
static constexpr inline uSys bytesPerComponent(const Format format) noexcept
{
    switch(format)
    {
        case Format::Red8UnsignedInt:                return 1;
        case Format::Red16UnsignedInt:               return 2;
        case Format::Red32UnsignedInt:               return 4;
        case Format::RedGreen8UnsignedInt:           return 1;
        case Format::RedGreen16UnsignedInt:          return 2;
        case Format::RedGreen32UnsignedInt:          return 4;
        case Format::RedGreenBlue8UnsignedInt:       return 1;
        case Format::RedGreenBlue16UnsignedInt:      return 2;
        case Format::RedGreenBlue32UnsignedInt:      return 4;
        case Format::RedGreenBlueAlpha8UnsignedInt:  return 1;
        case Format::RedGreenBlueAlpha16UnsignedInt: return 2;
        case Format::RedGreenBlueAlpha32UnsignedInt: return 4;
        case Format::Red16Float:                     return 2;
        case Format::Red32Float:                     return 4;
        case Format::RedGreen16Float:                return 2;
        case Format::RedGreen32Float:                return 4;
        case Format::RedGreenBlue16Float:            return 2;
        case Format::RedGreenBlue32Float:            return 4;
        case Format::RedGreenBlueAlpha16Float:       return 2;
        case Format::RedGreenBlueAlpha32Float:       return 4;
        case Format::Red8Typeless:                   return 1;
        case Format::Red16Typeless:                  return 2;
        case Format::Red32Typeless:                  return 4;
        case Format::RedGreen8Typeless:              return 1;
        case Format::RedGreen16Typeless:             return 2;
        case Format::RedGreen32Typeless:             return 4;
        case Format::RedGreenBlue8Typeless:          return 1;
        case Format::RedGreenBlue16Typeless:         return 2;
        case Format::RedGreenBlue32Typeless:         return 4;
        case Format::RedGreenBlueAlpha8Typeless:     return 1;
        case Format::RedGreenBlueAlpha16Typeless:    return 2;
        case Format::RedGreenBlueAlpha32Typeless:    return 4;
        default:                                     return 0;
    } 
}

/**
 * Get the number of component aspects (r, g, b, a) in a pixel.
 *
 * @param[in] format
 *      The format in question.
 */
static constexpr inline uSys numComponents(const Format format) noexcept
{
    switch(format)
    {
        case Format::Red8UnsignedInt:                return 1;
        case Format::Red16UnsignedInt:               return 1;
        case Format::Red32UnsignedInt:               return 1;
        case Format::RedGreen8UnsignedInt:           return 2;
        case Format::RedGreen16UnsignedInt:          return 2;
        case Format::RedGreen32UnsignedInt:          return 2;
        case Format::RedGreenBlue8UnsignedInt:       return 3;
        case Format::RedGreenBlue16UnsignedInt:      return 3;
        case Format::RedGreenBlue32UnsignedInt:      return 3;
        case Format::RedGreenBlueAlpha8UnsignedInt:  return 4;
        case Format::RedGreenBlueAlpha16UnsignedInt: return 4;
        case Format::RedGreenBlueAlpha32UnsignedInt: return 4;
        case Format::Red16Float:                     return 1;
        case Format::Red32Float:                     return 1;
        case Format::RedGreen16Float:                return 2;
        case Format::RedGreen32Float:                return 2;
        case Format::RedGreenBlue16Float:            return 3;
        case Format::RedGreenBlue32Float:            return 3;
        case Format::RedGreenBlueAlpha16Float:       return 4;
        case Format::RedGreenBlueAlpha32Float:       return 4;
        case Format::Red8Typeless:                   return 1;
        case Format::Red16Typeless:                  return 1;
        case Format::Red32Typeless:                  return 1;
        case Format::RedGreen8Typeless:              return 2;
        case Format::RedGreen16Typeless:             return 2;
        case Format::RedGreen32Typeless:             return 2;
        case Format::RedGreenBlue8Typeless:          return 3;
        case Format::RedGreenBlue16Typeless:         return 3;
        case Format::RedGreenBlue32Typeless:         return 3;
        case Format::RedGreenBlueAlpha8Typeless:     return 4;
        case Format::RedGreenBlueAlpha16Typeless:    return 4;
        case Format::RedGreenBlueAlpha32Typeless:    return 4;
        default:                                     return 0;
    }
}

/**
 * Gets the number of bytes an pixel individual requires.
 *
 * @param[in] format
 *      The format in question.
 */
static constexpr inline uSys bytesPerPixel(const Format format) noexcept
{
    switch(format)
    {
        case Format::Depth24Stencil8:    
        case Format::Depth24Typeless8:   
        case Format::Typeless24Stencil8: 
        case Format::Typeless24Typeless8: return 4;
        default: break;
    }

    return bytesPerComponent(format) * numComponents(format);
}

/**
 * Whether or not the format is typeless (neither float nor int).
 *
 * @param[in] format
 *      The format in question.
 */
static constexpr inline bool isTypeless(const Format format) noexcept
{
    switch(format)
    {
        case Format::Red8Typeless:                   
        case Format::Red16Typeless:                  
        case Format::Red32Typeless:                  
        case Format::RedGreen8Typeless:              
        case Format::RedGreen16Typeless:             
        case Format::RedGreen32Typeless:             
        case Format::RedGreenBlue8Typeless:          
        case Format::RedGreenBlue16Typeless:         
        case Format::RedGreenBlue32Typeless:         
        case Format::RedGreenBlueAlpha8Typeless:     
        case Format::RedGreenBlueAlpha16Typeless:    
        case Format::RedGreenBlueAlpha32Typeless:    
        case Format::Typeless24Typeless8:         return true;
        default:                                  return false;
    }
}

/**
 * Whether or not the format uses an unsigned integer layout.
 *
 * @param[in] format
 *      The format in question.
 */
static constexpr inline bool isUInt(const Format format) noexcept
{
    switch(format)
    {
        case Format::Red8UnsignedInt:                
        case Format::Red16UnsignedInt:               
        case Format::Red32UnsignedInt:               
        case Format::RedGreen8UnsignedInt:           
        case Format::RedGreen16UnsignedInt:          
        case Format::RedGreen32UnsignedInt:          
        case Format::RedGreenBlue8UnsignedInt:       
        case Format::RedGreenBlue16UnsignedInt:      
        case Format::RedGreenBlue32UnsignedInt:      
        case Format::RedGreenBlueAlpha8UnsignedInt:  
        case Format::RedGreenBlueAlpha16UnsignedInt: 
        case Format::RedGreenBlueAlpha32UnsignedInt: return true;
        default:                                     return false;
    }
}

/**
 * Whether or not the format uses a floating point layout.
 *
 * @param[in] format
 *      The format in question.
 */
static constexpr inline bool isFloat(const Format format) noexcept
{
    switch(format)
    {
        case Format::Red16Float:                     
        case Format::Red32Float:                     
        case Format::RedGreen16Float:                
        case Format::RedGreen32Float:                
        case Format::RedGreenBlue16Float:            
        case Format::RedGreenBlue32Float:            
        case Format::RedGreenBlueAlpha16Float:       
        case Format::RedGreenBlueAlpha32Float: return true;
        default:                               return false;
    }
}

/**
 *   Whether or not the format for use with depth-stencil
 * textures.
 *
 * @param[in] format
 *      The format in question.
 */
static constexpr inline bool isDepthStencil(const Format format) noexcept
{
    switch(format)
    {
        case Format::Depth24Stencil8:
        case Format::Depth24Typeless8:
        case Format::Typeless24Stencil8:
        case Format::Typeless24Typeless8: return true;
        default:                          return false;
    }
}

/**
 * Determines if two formats are compatible with each other.
 *
 *   This is used for texture view data format aliasing. For a
 * type to be compatible, exactly one of them must be typeless,
 * or they must be the exact same type, unless both types are
 * typeless.
 *
 * Both types cannot be typeless.
 *   Both types must be in the same family (same number of
 * components, and same number of bytes per component).
 * Both types can be identical (unless they are typeless).
 * Otherwise One type must be typeless.
 */
static constexpr bool isCompatible(const Format formatA, const Format formatB)
{
    if(formatA == formatB)
    { return true; }

    switch(formatA)
    {
        case Format::Depth24Stencil8:    
        case Format::Depth24Typeless8:   
        case Format::Typeless24Stencil8:
        {
            return formatB == Format::Typeless24Typeless8;
        }
        case Format::Typeless24Typeless8:
        {
            switch(formatB)
            {
                case Format::Depth24Stencil8:
                case Format::Depth24Typeless8:
                case Format::Typeless24Stencil8:  return true;
                default:                          return false;
            }
        }
        default: break;
    }

    /*
     * Is A typeless?
     *   Is B typeless?
     *     return false.
     *   Else
     *     Continue.
     * Else
     *   Is B typeless?
     *     Continue.
     *   Else
     *     return false.
     *
     * Does A have the same number of bytes per component as B?
     *   Continue.
     * Else
     *   return false.
     *
     * Does A have the same number of components as B?
     *   return true.
     * Else
     *   return false.
     */
    return (isTypeless(formatA) ^ isTypeless(formatB)) && bytesPerComponent(formatA) == bytesPerComponent(formatB) && numComponents(formatA) == numComponents(formatB);
}

/**
 * Computes the next mipmap side length.
 *
 * This divides the side length by 2, and clamps it to 1.
 */
static inline u64 computeMipSide(const u64 width) noexcept
{ return maxT(width / 2, 1); }

/**
 * Computes the side length at a specific mip level.
 */
static inline u64 computeMipSide(const u64 width, const u16 mipLevel) noexcept
{
    const u32 divisor = 1 << mipLevel;
    return maxT(width / divisor, 1);
}

/**
 *   Computes the maximum number of mipmap levels this texture
 * can have.
 *
 * @param[in] width
 *      The width of the texture.
 */
static inline u64 computeMipLevels(const u64 width) noexcept
{ return 1 + log2i(width); }

/**
 *   Computes the maximum number of mipmap levels this texture
 * can have. This is the max for all dimensions.
 *
 * @param[in] width
 *      The width of the texture.
 * @param[in] height
 *      The height of the texture.
 */
static inline u64 computeMipLevels(const u64 width, const u32 height) noexcept
{ return 1 + log2i(maxT(width, height)); }

/**
 *   Computes the maximum number of mipmap levels this texture
 * can have. This is the max for all dimensions.
 *
 * @param[in] width
 *      The width of the texture.
 * @param[in] height
 *      The height of the texture.
 * @param[in] depth
 *      The depth of the texture.
 */
static inline u64 computeMipLevels(const u64 width, const u32 height, const u16 depth) noexcept
{ return 1 + log2i(maxT(width, height, depth)); }

/**
 * Computes the sub resource index.
 *
 *   This does not require the number of elements within the
 * array. This is because each index of the array stores the
 * entire texture and its mipmaps, thus the number of elements
 * is irrelevant in the equation.
 *
 * Not currently used.
 *
 * @param[in] mipTarget
 *      The mipmap level to target.
 * @param[in] arrayIndex
 *      The index into the array to target.
 * @param[in] mipmapLevels
 *      The number of mipmap levels each texture has.
 */
static inline u32 computeSubResource(const u16 mipTarget, const u16 arrayIndex, const u16 mipmapLevels) noexcept
{ return mipTarget + (arrayIndex * mipmapLevels); }

/**
 * Computes the data size of a given sub resource.
 *
 *   This differs from @link ETexture::computeSize() @endlink
 * in that it does not compute the size for the entire mip
 * chain.
 *
 * @param[in] format
 *      The format of the texture.
 * @param[in] width
 *      The width of the sub resource mipmap level.
 */
static inline u64 computeSubResourceSize(const Format format, const u64 width) noexcept
{ return width * bytesPerPixel(format); }

/**
 * Computes the data size of a given sub resource.
 *
 *   This differs from @link ETexture::computeSize() @endlink
 * in that it does not compute the size for the entire mip
 * chain.
 *
 * @param[in] format
 *      The format of the texture.
 * @param[in] width
 *      The width of the sub resource mipmap level.
 * @param[in] height
 *      The height of the sub resource mipmap level.
 */
static inline u64 computeSubResourceSize(const Format format, const u64 width, const u32 height) noexcept
{ return width * height * bytesPerPixel(format); }

/**
 * Computes the data size of a given sub resource.
 *
 *   This differs from @link ETexture::computeSize() @endlink
 * in that it does not compute the size for the entire mip
 * chain.
 *
 * @param[in] format
 *      The format of the texture.
 * @param[in] width
 *      The width of the sub resource mipmap level.
 * @param[in] height
 *      The height of the sub resource mipmap level.
 * @param[in] depth
 *      The depth of the sub resource mipmap level.
 */
static inline u64 computeSubResourceSize(const Format format, const u64 width, const u32 height, const u16 depth) noexcept
{ return width * height * depth * bytesPerPixel(format); }

/**
 * Computes the data size of a given sub resource.
 *
 *   This differs from @link ETexture::computeSize() @endlink
 * in that it does not compute the size for the entire mip
 * chain.
 *
 *   This differs from @link ETexture::computeSubResourceSize() @endlink
 * in that it will compute the size of the mipmap level from
 * the base size.
 *
 * @param[in] format
 *      The format of the texture.
 * @param[in] width
 *      The width of the sub resource mipmap level.
 * @param[in] mipLevel
 *        The mipmap level in question; this automatically
 *      dissolves the base size into the target mipmap size.
 */
static inline u64 computeSubResourceSizeMip(const Format format, const u64 width, const u16 mipLevel) noexcept
{
    const u32 divisor = 1 << mipLevel;
    return (width / divisor) * bytesPerPixel(format);
}

/**
 * Computes the data size of a given sub resource.
 *
 *   This differs from @link ETexture::computeSize() @endlink
 * in that it does not compute the size for the entire mip
 * chain.
 *
 *   This differs from @link ETexture::computeSubResourceSize() @endlink
 * in that it will compute the size of the mipmap level from
 * the base size.
 *
 * @param[in] format
 *      The format of the texture.
 * @param[in] width
 *      The width of the sub resource mipmap level.
 * @param[in] height
 *      The height of the sub resource mipmap level.
 * @param[in] mipLevel
 *        The mipmap level in question; this automatically
 *      dissolves the base size into the target mipmap size.
 */
static inline u64 computeSubResourceSizeMip(const Format format, const u64 width, const u32 height, const u16 mipLevel) noexcept
{
    const u32 divisor = 1 << mipLevel;
    return (width / divisor) * (height / divisor) * bytesPerPixel(format);
}

/**
 * Computes the data size of a given sub resource.
 *
 *   This differs from @link ETexture::computeSize() @endlink
 * in that it does not compute the size for the entire mip
 * chain.
 *
 *   This differs from @link ETexture::computeSubResourceSize() @endlink
 * in that it will compute the size of the mipmap level from
 * the base size.
 *
 * @param[in] format
 *      The format of the texture.
 * @param[in] width
 *      The width of the sub resource mipmap level.
 * @param[in] height
 *      The height of the sub resource mipmap level.
 * @param[in] depth
 *      The depth of the sub resource mipmap level.
 * @param[in] mipLevel
 *        The mipmap level in question; this automatically
 *      dissolves the base size into the target mipmap size.
 */
static inline u64 computeSubResourceSizeMip(const Format format, const u64 width, const u32 height, const u16 depth, const u16 mipLevel) noexcept
{
    const u32 divisor = 1 << mipLevel;
    return (width / divisor) * (height / divisor) * (depth / divisor) * bytesPerPixel(format);
}

/**
 * Computes the data size of a given texture and its mip chain.
 *
 *   I have been unable to find an `O(1)` formula that can
 * compute the entire mip chain size of any given start size,
 * specifically including non-square start sizes. WolframAlpha
 * was unable to deterministically find a formula given the use
 * of floor functions (we're dealing with integers, not reals).
 * Its only recommendations were `2x` and `2x-1`, which
 * appeared to only be accurate when using square sizes, or
 * when using reals as `x` approaches infinity.
 *
 * @param[in] format
 *      The format of the texture.
 * @param[in] width
 *      The base width of the texture.
 */
static inline u64 computeSize(const Format format, const u64 width) noexcept
{
    u64 size = 0;

    for(u64 w = width; w > 1; w = computeMipSide(w))
    { size += w; }

    return size * bytesPerPixel(format);
}

/**
 *   Computes the data size of a given array of textures and
 * their mip chain.
 *
 *   I have been unable to find an `O(1)` formula that can
 * compute the entire mip chain size of any given start size,
 * specifically including non-square start sizes. WolframAlpha
 * was unable to deterministically find a formula given the use
 * of floor functions (we're dealing with integers, not reals).
 * Its only recommendations were `2x` and `2x-1`, which
 * appeared to only be accurate when using square sizes, or
 * when using reals as `x` approaches infinity.
 *
 * @param[in] format
 *      The format of the texture.
 * @param[in] width
 *      The base width of the texture.
 * @param[in] arrayCount
 *      The number of textures in the array.
 */
static inline u64 computeSizeArr(const Format format, const u64 width, const u16 arrayCount) noexcept
{
    u64 size = 0;

    for(u64 w = width; w > 1; w = computeMipSide(w))
    { size += w; }

    return size * arrayCount * bytesPerPixel(format);
}

/**
 * Computes the data size of a given texture and its mip chain.
 *
 *   I have been unable to find an `O(1)` formula that can
 * compute the entire mip chain size of any given start size,
 * specifically including non-square start sizes. WolframAlpha
 * was unable to deterministically find a formula given the use
 * of floor functions (we're dealing with integers, not reals).
 * Its only recommendations were `2x` and `2x-1`, which
 * appeared to only be accurate when using square sizes, or
 * when using reals as `x` approaches infinity.
 *
 * @param[in] format
 *      The format of the texture.
 * @param[in] width
 *      The base width of the texture.
 * @param[in] height
 *      The base height of the texture.
 */
static inline u64 computeSize(const Format format, const u64 width, const u32 height) noexcept
{
    u64 size = 0;

    for(u64 w = width, h = height; w > 1 && h > 1; w = computeMipSide(w), h = computeMipSide(h))
    { size += w * h; }

    return size * bytesPerPixel(format);
}

/**
 *   Computes the data size of a given array of textures and
 * their mip chain.
 *
 *   I have been unable to find an `O(1)` formula that can
 * compute the entire mip chain size of any given start size,
 * specifically including non-square start sizes. WolframAlpha
 * was unable to deterministically find a formula given the use
 * of floor functions (we're dealing with integers, not reals).
 * Its only recommendations were `2x` and `2x-1`, which
 * appeared to only be accurate when using square sizes, or
 * when using reals as `x` approaches infinity.
 *
 * @param[in] format
 *      The format of the texture.
 * @param[in] width
 *      The base width of the texture.
 * @param[in] height
 *      The base height of the texture.
 * @param[in] arrayCount
 *      The number of textures in the array.
 */
static inline u64 computeSizeArr(const Format format, const u64 width, const u32 height, const u16 arrayCount) noexcept
{
    u64 size = 0;

    for(u64 w = width, h = height; w > 1 && h > 1; w = computeMipSide(w), h = computeMipSide(h))
    { size += w * h; }

    return size * arrayCount * bytesPerPixel(format);
}

/**
 * Computes the data size of a given texture and its mip chain.
 *
 *   I have been unable to find an `O(1)` formula that can
 * compute the entire mip chain size of any given start size,
 * specifically including non-square start sizes. WolframAlpha
 * was unable to deterministically find a formula given the use
 * of floor functions (we're dealing with integers, not reals).
 * Its only recommendations were `2x` and `2x-1`, which
 * appeared to only be accurate when using square sizes, or
 * when using reals as `x` approaches infinity.
 *
 * @param[in] format
 *      The format of the texture.
 * @param[in] width
 *      The base width of the texture.
 * @param[in] height
 *      The base height of the texture.
 * @param[in] depth
 *      The base depth of the texture.
 */
static inline u64 computeSize(const Format format, const u64 width, const u32 height, const u16 depth) noexcept
{
    u64 size = 0;

    for(u64 w = width, h = height, d = depth; w > 1 && h > 1 && d > 1; w = computeMipSide(w), h = computeMipSide(h), d = computeMipSide(d))
    { size += w * h * d; }

    return size * bytesPerPixel(format);
}

/**
 *   Computes the data size of a given texture and its mip
 * chain with a maximum number of mipmap levels.
 *
 *   I have been unable to find an `O(1)` formula that can
 * compute the entire mip chain size of any given start size,
 * specifically including non-square start sizes. WolframAlpha
 * was unable to deterministically find a formula given the use
 * of floor functions (we're dealing with integers, not reals).
 * Its only recommendations were `2x` and `2x-1`, which
 * appeared to only be accurate when using square sizes, or
 * when using reals as `x` approaches infinity.
 *
 * @param[in] format
 *      The format of the texture.
 * @param[in] width
 *      The base width of the texture.
 * @param[in] mipmapLevels
 *        The maximum number of mipmap levels this texture will
 *      have. The return value may be incorrect if mipmapLevels
 *      exceeds the maximum number of mipmap levels the texture
 *      can actually have. i.e. a texture with base size 8 only
 *      has 4 mipmap levels (8, 4, 2, 1), and thus mipmapLevels
 *      cannot be 5.
 */
static inline u64 computeSizeMip(const Format format, const u64 width, const u16 mipmapLevels) noexcept
{
    u64 size = 0;

    for(u64 w = width, m = mipmapLevels; w > 1 && m > 0; w = computeMipSide(w), --m)
    { size += w; }

    return size * bytesPerPixel(format);
}

/**
 *   Computes the data size of a given array of textures and
 * their mip chain with a maximum number of mipmap levels.
 *
 *   I have been unable to find an `O(1)` formula that can
 * compute the entire mip chain size of any given start size,
 * specifically including non-square start sizes. WolframAlpha
 * was unable to deterministically find a formula given the use
 * of floor functions (we're dealing with integers, not reals).
 * Its only recommendations were `2x` and `2x-1`, which
 * appeared to only be accurate when using square sizes, or
 * when using reals as `x` approaches infinity.
 *
 * @param[in] format
 *      The format of the texture.
 * @param[in] width
 *      The base width of the texture.
 * @param[in] mipmapLevels
 *        The maximum number of mipmap levels this texture will
 *      have. The return value may be incorrect if mipmapLevels
 *      exceeds the maximum number of mipmap levels the texture
 *      can actually have. i.e. a texture with base size 8 only
 *      has 4 mipmap levels (8, 4, 2, 1), and thus mipmapLevels
 *      cannot be 5.
 * @param[in] arrayCount
 *      The number of textures in the array.
 */
static inline u64 computeSizeMipArr(const Format format, const u64 width, const u16 mipmapLevels, const u16 arrayCount) noexcept
{
    u64 size = 0;

    for(u64 w = width, m = mipmapLevels; w > 1 && m > 0; w = computeMipSide(w), --m)
    { size += w; }

    return size * arrayCount * bytesPerPixel(format);
}

/**
 *   Computes the data size of a given texture and its mip
 * chain with a maximum number of mipmap levels.
 *
 *   I have been unable to find an `O(1)` formula that can
 * compute the entire mip chain size of any given start size,
 * specifically including non-square start sizes. WolframAlpha
 * was unable to deterministically find a formula given the use
 * of floor functions (we're dealing with integers, not reals).
 * Its only recommendations were `2x` and `2x-1`, which
 * appeared to only be accurate when using square sizes, or
 * when using reals as `x` approaches infinity.
 *
 * @param[in] format
 *      The format of the texture.
 * @param[in] width
 *      The base width of the texture.
 * @param[in] height
 *      The base height of the texture.
 * @param[in] mipmapLevels
 *        The maximum number of mipmap levels this texture will
 *      have. The return value may be incorrect if mipmapLevels
 *      exceeds the maximum number of mipmap levels the texture
 *      can actually have. i.e. a texture with base size 8 only
 *      has 4 mipmap levels (8, 4, 2, 1), and thus mipmapLevels
 *      cannot be 5.
 */
static inline u64 computeSizeMip(const Format format, const u64 width, const u32 height, const u16 mipmapLevels) noexcept
{
    u64 size = 0;

    for(u64 w = width, h = height, m = mipmapLevels; w > 1 && h > 1 && m > 0; w = computeMipSide(w), h = computeMipSide(h), --m)
    { size += w * h; }

    return size * bytesPerPixel(format);
}

/**
 *   Computes the data size of a given array of textures and
 * their mip chain with a maximum number of mipmap levels.
 *
 *   I have been unable to find an `O(1)` formula that can
 * compute the entire mip chain size of any given start size,
 * specifically including non-square start sizes. WolframAlpha
 * was unable to deterministically find a formula given the use
 * of floor functions (we're dealing with integers, not reals).
 * Its only recommendations were `2x` and `2x-1`, which
 * appeared to only be accurate when using square sizes, or
 * when using reals as `x` approaches infinity.
 *
 * @param[in] format
 *      The format of the texture.
 * @param[in] width
 *      The base width of the texture.
 * @param[in] height
 *      The base height of the texture.
 * @param[in] mipmapLevels
 *        The maximum number of mipmap levels this texture will
 *      have. The return value may be incorrect if mipmapLevels
 *      exceeds the maximum number of mipmap levels the texture
 *      can actually have. i.e. a texture with base size 8 only
 *      has 4 mipmap levels (8, 4, 2, 1), and thus mipmapLevels
 *      cannot be 5.
 * @param[in] arrayCount
 *      The number of textures in the array.
 */
static inline u64 computeSizeMipArr(const Format format, const u64 width, const u32 height, const u16 mipmapLevels, const u16 arrayCount) noexcept
{
    u64 size = 0;

    for(u64 w = width, h = height, m = mipmapLevels; w > 1 && h > 1 && m > 0; w = computeMipSide(w), h = computeMipSide(h), --m)
    { size += w * h; }

    return size * arrayCount * bytesPerPixel(format);
}

/**
 *   Computes the data size of a given texture and its mip
 * chain with a maximum number of mipmap levels.
 *
 *   I have been unable to find an `O(1)` formula that can
 * compute the entire mip chain size of any given start size,
 * specifically including non-square start sizes. WolframAlpha
 * was unable to deterministically find a formula given the use
 * of floor functions (we're dealing with integers, not reals).
 * Its only recommendations were `2x` and `2x-1`, which
 * appeared to only be accurate when using square sizes, or
 * when using reals as `x` approaches infinity.
 *
 * @param[in] format
 *      The format of the texture.
 * @param[in] width
 *      The base width of the texture.
 * @param[in] height
 *      The base height of the texture.
 * @param[in] depth
 *      The base depth of the texture.
 * @param[in] mipmapLevels
 *        The maximum number of mipmap levels this texture will
 *      have. The return value may be incorrect if mipmapLevels
 *      exceeds the maximum number of mipmap levels the texture
 *      can actually have. i.e. a texture with base size 8 only
 *      has 4 mipmap levels (8, 4, 2, 1), and thus mipmapLevels
 *      cannot be 5.
 */
static inline u64 computeSizeMip(const Format format, const u64 width, const u32 height, const u16 depth, const u16 mipmapLevels) noexcept
{
    u64 size = 0;

    for(u64 w = width, h = height, d = depth, m = mipmapLevels; w > 1 && h > 1 && d > 1 && m > 0; w = computeMipSide(w), h = computeMipSide(h), d = computeMipSide(d), --m)
    { size += w * h * d; }

    return size * bytesPerPixel(format);
}
}
