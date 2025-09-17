/**
 * @file
 */
#pragma once

#include <ArrayList.hpp>
#include <algorithm>

namespace tau {

namespace texture::internal {

    /**
     * @tparam CoordT
     *      An integer type for the coordinate. If your max texture size is
     *    less than 65,536 then you should use a ::std::uint16_t instead of
     *    a ::std::uint32_t to have better cache locality.
     */
    template<typename CoordT>
    struct Rect final
    {
        DEFAULT_CONSTRUCT_PUC(Rect);
        DEFAULT_DESTRUCT(Rect);
        DEFAULT_CM_PU(Rect);
    public:
        CoordT Top;
        CoordT Bottom;
        CoordT Left;
        CoordT Right;

        Rect(
            const CoordT top,
            const CoordT bottom,
            const CoordT left,
            const CoordT right
        ) noexcept
            : Top(top)
            , Bottom(bottom)
            , Left(left)
            , Right(right)
        { }
    };

    /**
     * @tparam HandleT
     *      A type for a handle back to the texture, this is used
     *    when actually packing in the texture data.
     * @tparam CoordT
     *      An integer type for the coordinate. If your max texture size is
     *    less than 65,536 then you should use a ::std::uint16_t instead of
     *    a ::std::uint32_t to have better cache locality.
     */
    template<typename HandleT, typename CoordT>
    struct Coord final
    {
        DEFAULT_CONSTRUCT_PUC(Coord);
        DEFAULT_DESTRUCT(Coord);
        DEFAULT_CM_PU(Coord);
    public:
        HandleT Handle;
        union
        {
            CoordT X;
            CoordT Width;
        };
        union
        {
            CoordT Y;
            CoordT Height;
        };

        Coord(
            const HandleT handle,
            const CoordT x,
            const CoordT y
        ) noexcept
            : Handle(handle)
            , X(x)
            , Y(y)
        { }

        /**
         *   Technically you're not supposed to switch which member of a
         * union you access. Thus, this constructor exists to set the width
         * and height members.
         */
        Coord(
            const HandleT handle,
            const CoordT width,
            const CoordT height,
            ::std::nullptr_t // This is just to let you select this constructor.
        ) noexcept
            : Handle(handle)
            , Width(width)
            , Height(height)
        { }
    };

    template<typename ChildT, typename CoordT>
    struct TextureGreaterComparator
    {
        [[nodiscard]] static int Compare(const CoordT& a, const CoordT& b) noexcept
        {
            const ::std::size_t aQuantity = ChildT::GetQuantity(a);
            const ::std::size_t bQuantity = ChildT::GetQuantity(b);

            if(aQuantity == bQuantity)
            {
                return 0;
            }

            return aQuantity > bQuantity ? 1 : -1;
        }

        [[nodiscard]] bool CompareB(const CoordT& a, const CoordT& b) const noexcept
        {
            return Compare(a, b) == 1;
        }

        [[nodiscard]] bool operator()(const CoordT& a, const CoordT& b) const noexcept
        {
            return CompareB(a, b);
        }
    };

    template<typename CoordT>
    struct TextureAreaGreater final : TextureGreaterComparator<TextureAreaGreater<CoordT>, CoordT>
    {
        [[nodiscard]] static ::std::size_t GetQuantity(const CoordT& c) noexcept
        {
            return c.Width * c.Height;
        }

        [[nodiscard]] bool operator()(const CoordT& a, const CoordT& b) const noexcept
        {
            return CompareB(a, b);
        }
    };

    template<typename CoordT>
    struct TexturePerimeterGreater final : TextureGreaterComparator<TexturePerimeterGreater<CoordT>, CoordT>
    {
        [[nodiscard]] static ::std::size_t GetQuantity(const CoordT& c) noexcept
        {
            return c.Width * 2 + c.Height * 2;
        }

        [[nodiscard]] bool operator()(const CoordT& a, const CoordT& b) const noexcept
        {
            return CompareB(a, b);
        }
    };

    template<typename CoordT>
    struct TextureSideGreater final : TextureGreaterComparator<TextureSideGreater<CoordT>, CoordT>
    {
        [[nodiscard]] static ::std::size_t GetQuantity(const CoordT& c) noexcept
        {
            return ::std::max(c.Width, c.Height);
        }

        [[nodiscard]] bool operator()(const CoordT& a, const CoordT& b) const noexcept
        {
            return CompareB(a, b);
        }
    };

    template<typename CoordT>
    struct TextureWidthGreater final : TextureGreaterComparator<TextureWidthGreater<CoordT>, CoordT>
    {
        [[nodiscard]] static ::std::size_t GetQuantity(const CoordT& c) noexcept
        {
            return c.Width;
        }

        [[nodiscard]] bool operator()(const CoordT& a, const CoordT& b) const noexcept
        {
            return CompareB(a, b);
        }
    };

    template<typename CoordT>
    struct TextureHeightGreater final : TextureGreaterComparator<TextureHeightGreater<CoordT>, CoordT>
    {
        [[nodiscard]] static ::std::size_t GetQuantity(const CoordT& c) noexcept
        {
            return c.Height;
        }

        [[nodiscard]] bool operator()(const CoordT& a, const CoordT& b) const noexcept
        {
            return CompareB(a, b);
        }
    };

    template<typename CoordT>
    struct TextureGreater final
    {
        [[nodiscard]] bool operator()(const CoordT& a, const CoordT& b) const noexcept
        {
            int compare = TextureAreaGreater<CoordT>::Compare(a, b);
            if(compare != 0)
            {
                return compare == 1;
            }

            compare = TexturePerimeterGreater<CoordT>::Compare(a, b);
            if(compare != 0)
            {
                return compare == 1;
            }

            compare = TextureSideGreater<CoordT>::Compare(a, b);
            if(compare != 0)
            {
                return compare == 1;
            }

            compare = TextureWidthGreater<CoordT>::Compare(a, b);
            if(compare != 0)
            {
                return compare == 1;
            }

            compare = TextureHeightGreater<CoordT>::Compare(a, b);
            return compare == 1;
        }
    };
}

/**
 * @tparam HandleT
 *      A type for a handle back to the texture, this is used
 *    when actually packing in the texture data.
 * @tparam CoordT
 *      An integer type for the coordinate. If your max texture
 *    size is less than 65,536 then you should use a u16
 *    instead of a u32 to have better cache locality.
 */
template<typename HandleT, typename CoordT = u16>
class TexturePacker2D final
{
public:
    using Rect = texture::internal::Rect<CoordT>;
    using Coord = texture::internal::Coord<HandleT, CoordT>;

    using TextureAreaGreater = texture::internal::TextureAreaGreater<Coord>;
    using TexturePerimeterGreater = texture::internal::TexturePerimeterGreater<Coord>;
    using TextureSideGreater = texture::internal::TextureSideGreater<Coord>;
    using TextureWidthGreater = texture::internal::TextureWidthGreater<Coord>;
    using TextureHeightGreater = texture::internal::TextureHeightGreater<Coord>;
    using TextureGreater = texture::internal::TextureGreater<Coord>;
public:
    TexturePacker2D(const uSys maxTextures) noexcept
        : m_FreeSpaces(maxTextures * 2)
        , m_AllocatedSpaces(maxTextures)
        , m_PackedWidth(0)
        , m_PackedHeight(0)
    { }

    [[nodiscard]] const ArrayList<Rect, ALMoveMethod::MemCopy>& FreeSpaces() const noexcept { return m_FreeSpaces; }
    [[nodiscard]] const ArrayList<Coord, ALMoveMethod::MemCopy>& AllocatedSpaces() const noexcept { return m_AllocatedSpaces; }

    [[nodiscard]] uSys PackedWidth() const noexcept { return m_PackedWidth; }
    [[nodiscard]] uSys PackedHeight() const noexcept { return m_PackedHeight; }

    template<typename _TextureComparator = TextureGreater>
    void Pack(const Coord* textures, uSys textureCount, uSys maxMapWidth, uSys maxMapHeight, iSys minRegression) noexcept;
    
    template<typename _TextureComparator = TextureGreater>
    void Pack(const Coord* const textures, const uSys textureCount, const uSys maxMapSize, const iSys minRegression = 32) noexcept
    { pack<_TextureComparator>(textures, textureCount, maxMapSize, maxMapSize, minRegression); }
private:
    bool InternalPack(const Coord* sortedTextures, uSys textureCount, uSys mapWidth, uSys mapHeight) noexcept;
private:
    ArrayList<Rect, ALMoveMethod::MemCopy> m_FreeSpaces;
    ArrayList<Coord, ALMoveMethod::MemCopy> m_AllocatedSpaces;

    uSys m_PackedWidth;
    uSys m_PackedHeight;
};

template<typename _HandleT, typename _CoordT>
template<typename _TextureComparator>
void TexturePacker2D<_HandleT, _CoordT>::Pack(const Coord* const textures, const uSys textureCount, const uSys maxMapWidth, const uSys maxMapHeight, const iSys minRegression) noexcept
{
    Coord* const sortedTextures = new(::std::nothrow) Coord[textureCount];
    ::std::memcpy(sortedTextures, textures, textureCount * sizeof(Coord));
    ::std::sort(sortedTextures, &sortedTextures[textureCount], _TextureComparator());

    uSys maxWidth = maxMapWidth;
    uSys maxHeight = maxMapHeight;
    uSys minWidth = 1;
    uSys minHeight = 1;

    uSys currentWidth = maxMapWidth / 2;
    uSys currentHeight = maxMapHeight / 2;

    i32 regression = 0;

    while(true)
    {
        const bool success = InternalPack(sortedTextures, textureCount, currentWidth, currentHeight);

        if(success)
        {
            maxWidth = currentWidth;
            maxHeight = currentHeight;
        }
        else
        {
            minWidth = currentWidth;
            minHeight = currentHeight;
        }

        const iSys nextWidth = (maxWidth + minWidth) / 2;
        const iSys nextHeight = (maxHeight + minHeight) / 2;

        if(regression == 0)
        {
            const bool finalWidth = ::std::abs(nextWidth - static_cast<iSys>(currentWidth)) <= minRegression;
            const bool finalHeight = ::std::abs(nextHeight - static_cast<iSys>(currentHeight)) <= minRegression;

            if(finalHeight)
            {
                regression = 1;
                currentHeight = maxHeight;
                minWidth = 0;
                currentWidth = nextWidth;
            }
            else if(finalWidth)
            {
                regression = 2;
                currentWidth = maxWidth;
                minHeight = 0;
                currentHeight = nextHeight;
            }
            else
            {
                currentWidth = nextWidth;
                currentHeight = nextHeight;
            }
        }
        else if(regression == 1)
        {
            if(::std::abs(nextWidth - static_cast<iSys>(currentWidth)) <= minRegression)
            {
                regression = 2;
                currentWidth = maxWidth;
                minHeight = 0;
                currentHeight = nextHeight;
            }
            else
            {
                currentWidth = nextWidth;
            }
        }
        else
        {
            if(::std::abs(nextHeight - static_cast<iSys>(currentHeight)) <= minRegression)
            {
                break;
            }

            currentHeight = nextHeight;
        }
    }

    if(currentWidth != maxWidth || 
       currentHeight != maxHeight)
    { InternalPack(sortedTextures, textureCount, maxWidth, maxHeight); }

    m_PackedWidth = maxWidth;
    m_PackedHeight = maxHeight;
}

template<typename HandleT, typename CoordT>
bool TexturePacker2D<HandleT, CoordT>::InternalPack(const Coord* const sortedTextures, const uSys textureCount, const uSys mapWidth, const uSys mapHeight) noexcept
{
    m_FreeSpaces.clear(false);
    m_AllocatedSpaces.clear(false);

    m_FreeSpaces.emplace(0, mapHeight, 0, mapWidth);

    for(uSys i = 0; i < textureCount; ++i)
    {
        const Coord& texture = sortedTextures[i];

        bool success = false;

        for(iSys j = m_FreeSpaces.count() - 1; j >= 0; --j)
        {
            const Rect& freeSpace = m_FreeSpaces[j];

            const uSys width = freeSpace.Right - freeSpace.Left;
            const uSys height = freeSpace.Bottom - freeSpace.Top;

            if(texture.Width <= width && texture.Height <= height)
            {
                // Affirm that we found a spot.
                success = true;

                // Add the spot to the allocated spaces.
                // We're only passing the texture handle and the (X, Y) coordinate of where it will end up.
                m_AllocatedSpaces.emplace(texture.Handle, freeSpace.Left, freeSpace.Top);

                if(texture.Width == width && texture.Height == height)
                {
                    // If the texture was a perfect match then we can just remove the space.
                    m_FreeSpaces.remove(j);
                }
                else if(texture.Width == width && texture.Height < height)
                {
                    //   If the width matched, but it was less than the height, then we can
                    // just shift the top to immediately below the texture and add the
                    // space to the end of the list.
                    const Rect rect(freeSpace.Top + texture.Height, freeSpace.Bottom, freeSpace.Left, freeSpace.Right);

                    m_FreeSpaces.remove(j);
                    m_FreeSpaces.add(rect);
                }
                else if(texture.Width < width && texture.Height == height)
                {
                    const Rect rect(freeSpace.Top, freeSpace.Bottom, freeSpace.Left + texture.Width, freeSpace.Right);

                    m_FreeSpaces.remove(j);
                    m_FreeSpaces.add(rect);
                }
                else
                {
                    // Otherwise we'll have to create two rectangles.
                    // The goal will be to create the largest rectangles possible.

                    const uSys remainingWidth = freeSpace.Right - (freeSpace.Left + texture.Width);
                    const uSys remainingHeight = freeSpace.Bottom - (freeSpace.Top + texture.Height);

                    //   The right edge of the first rectangle and the bottom edge of the
                    // second rectangle.
                    const CoordT aRight = remainingWidth >= remainingHeight ? freeSpace.Left + texture.Width : freeSpace.Right;
                    const CoordT bBottom = remainingWidth >= remainingHeight ? freeSpace.Bottom : freeSpace.Top + texture.Height;

                    // Create the new rects.
                    const Rect rectA(freeSpace.Top + texture.Height, freeSpace.Bottom, freeSpace.Left, aRight);
                    const Rect rectB(freeSpace.Top, bBottom, freeSpace.Left + texture.Width, freeSpace.Right);

                    // Calculate the area of the new rects.
                    const uSys areaA = (rectA.Bottom - rectA.Top) * (rectA.Right - rectA.Left);
                    const uSys areaB = (rectB.Bottom - rectB.Top) * (rectB.Right - rectB.Left);

                    // Remove the old space.
                    m_FreeSpaces.remove(j);

                    // Place the largest space first to keep a quasi sorting.
                    if(areaA > areaB)
                    {
                        m_FreeSpaces.add(rectA);
                        m_FreeSpaces.add(rectB);
                    }
                    else
                    {
                        m_FreeSpaces.add(rectB);
                        m_FreeSpaces.add(rectA);
                    }
                }

                break;
            }
        }

        if(!success)
        {
            return false;
        }
    }

    return true;
}

}
