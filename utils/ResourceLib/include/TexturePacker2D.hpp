/**
 * @file
 */
#pragma once

#include <ArrayList.hpp>
#include <algorithm>

/**
 * @tparam _HandleT
 *      A type for a handle back to the texture, this is used
 *    when actually packing in the texture data.
 * @tparam _CoordT
 *      An integer type for the coordinate. If your max texture
 *    size is less than 65,536 then you should use a u16
 *    instead of a u32 to have better cache locality.
 */
template<typename _HandleT, typename _CoordT = u16>
class TexturePacker2D final
{
public:
    struct Rect final
    {
        _CoordT top;
        _CoordT left;
        _CoordT bottom;
        _CoordT right;

        Rect(const _CoordT _top, const _CoordT _left, const _CoordT _bottom, const _CoordT _right) noexcept
            : top(_top)
            , left(_left)
            , bottom(_bottom)
            , right(_right)
        { }
    };

    struct Coord final
    {
        DEFAULT_CONSTRUCT_PU(Coord);

        _HandleT handle;
        union
        {
            _CoordT x;
            _CoordT width;
        };
        union
        {
            _CoordT y;
            _CoordT height;
        };

        Coord(const _HandleT _handle, const _CoordT _x, const _CoordT _y) noexcept
            : handle(_handle)
            , x(_x)
            , y(_y)
        { }
    };

    using Texture = Coord;

    struct TextureAreaGreater final
    {
        static i32 compare(const Texture& a, const Texture& b) noexcept
        {
            const uSys aArea = (a.width * a.height);
            const uSys bArea = (b.width * b.height);

            if(aArea == bArea)
            { return 0; }

            return aArea > bArea ? 1 : -1;
        }

        bool operator()(const Texture& a, const Texture& b) noexcept
        { return compare(a, b) == 1; }
    };

    struct TexturePerimeterGreater final
    {
        static i32 compare(const Texture& a, const Texture& b) noexcept
        {
            const uSys aPerimeter = (a.width * 2 + a.height * 2);
            const uSys bPerimeter = (b.width * 2 + b.height * 2);
            
            if(aPerimeter == bPerimeter)
            { return 0; }

            return aPerimeter > bPerimeter ? 1 : -1;
        }

        bool operator()(const Texture& a, const Texture& b) noexcept
        { return compare(a, b) == 1; }
    };

    struct TextureSideGreater final
    {
        static i32 compare(const Texture& a, const Texture& b) noexcept
        {
            const uSys aLargestSide = maxT(a.width, a.height);
            const uSys bLargestSide = maxT(b.width, b.height);
            
            if(aLargestSide == bLargestSide)
            { return 0; }

            return aLargestSide > bLargestSide ? 1 : -1;
        }

        bool operator()(const Texture& a, const Texture& b) noexcept
        { return compare(a, b) == 1; }
    };

    struct TextureWidthGreater final
    {
        static i32 compare(const Texture& a, const Texture& b) noexcept
        {
            if(a.width == b.width)
            { return 0; }

            return a.width > b.width ? 1 : -1;
        }

        bool operator()(const Texture& a, const Texture& b) noexcept
        { return compare(a, b) == 1; }
    };

    struct TextureHeightGreater final
    {
        static i32 compare(const Texture& a, const Texture& b) noexcept
        {
            if(a.height == b.height)
            { return 0; }

            return a.height > b.height ? 1 : -1;
        }

        bool operator()(const Texture& a, const Texture& b) noexcept
        { return compare(a, b) == 1; }
    };

    struct TextureGreater final
    {
        bool operator()(const Texture& a, const Texture& b) noexcept
        {
            const i32 areaCompare = TextureAreaGreater::compare(a, b);
            if(areaCompare != 0)
            { return areaCompare == 1; }
            
            const i32 perimeterCompare = TexturePerimeterGreater::compare(a, b);
            if(perimeterCompare != 0)
            { return perimeterCompare == 1; }

            const i32 sideCompare = TextureSideGreater::compare(a, b);
            if(sideCompare != 0)
            { return sideCompare == 1; }

            const i32 widthCompare = TextureWidthGreater::compare(a, b);
            if(widthCompare != 0)
            { return widthCompare == 1; }

            const i32 heightCompare = TextureHeightGreater::compare(a, b);
            return heightCompare == 1;
        }
    };
private:
    ArrayList<Rect, ALMoveMethod::MemCopy> _freeSpaces;
    ArrayList<Coord, ALMoveMethod::MemCopy> _allocatedSpaces;

    uSys _packedWidth;
    uSys _packedHeight;
public:
    TexturePacker2D(const uSys maxTextures) noexcept
        : _freeSpaces(maxTextures * 2)
        , _allocatedSpaces(maxTextures)
        , _packedWidth(0)
        , _packedHeight(0)
    { }

    [[nodiscard]] const ArrayList<Coord, ALMoveMethod::MemCopy>& allocatedSpaces() const noexcept { return _allocatedSpaces; }

    [[nodiscard]] uSys packedWidth() const noexcept { return _packedWidth; }
    [[nodiscard]] uSys packedHeight() const noexcept { return _packedHeight; }

    template<typename _TextureComparator = TextureGreater>
    void pack(const Texture* textures, uSys textureCount, uSys maxMapWidth, uSys maxMapHeight, iSys minRegression) noexcept;
    
    template<typename _TextureComparator = TextureGreater>
    void pack(const Texture* const textures, const uSys textureCount, const uSys maxMapSize, const iSys minRegression = 32) noexcept
    { pack<_TextureComparator>(textures, textureCount, maxMapSize, maxMapSize, minRegression); }
private:
    bool _pack(const Texture* sortedTextures, uSys textureCount, uSys mapWidth, uSys mapHeight) noexcept;
};

template<typename _HandleT, typename _CoordT>
template<typename _TextureComparator>
void TexturePacker2D<_HandleT, _CoordT>::pack(const Texture* const textures, const uSys textureCount, const uSys maxMapWidth, const uSys maxMapHeight, const iSys minRegression) noexcept
{
    Texture* const sortedTextures = new(::std::nothrow) Texture[textureCount];
    ::std::memcpy(sortedTextures, textures, textureCount * sizeof(Texture));
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
        const bool success = _pack(sortedTextures, textureCount, currentWidth, currentHeight);

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
            const bool finalWidth = abs(nextWidth - static_cast<iSys>(currentWidth)) <= minRegression;
            const bool finalHeight = abs(nextHeight - static_cast<iSys>(currentHeight)) <= minRegression;

            if((finalWidth && finalHeight) || (!finalWidth && finalHeight))
            {
                regression = 1;
                currentHeight = maxHeight;
                minWidth = 0;
                currentWidth = (maxWidth + minWidth) / 2;
                continue;
            }
            else if(finalWidth)
            {
                regression = 2;
                currentWidth = maxWidth;
                minHeight = 0;
                currentHeight = (maxHeight + minHeight) / 2;
                continue;
            }

            currentWidth = nextWidth;
            currentHeight = nextHeight;
        }
        else if(regression == 1)
        {
            if(abs(nextWidth - static_cast<iSys>(currentWidth)) <= minRegression)
            {
                regression = 2;
                currentWidth = maxWidth;
                minHeight = 0;
                currentHeight = (maxHeight + minHeight) / 2;
                continue;
            }

            currentWidth = nextWidth;
        }
        else
        {
            if(abs(nextHeight - static_cast<iSys>(currentHeight)) <= minRegression)
            { break; }

            currentHeight = nextHeight;
        }
    }

    if(currentWidth != maxWidth || 
       currentHeight != maxHeight)
    { _pack(sortedTextures, textureCount, maxWidth, maxHeight); }

    _packedWidth = maxWidth;
    _packedHeight = maxHeight;
}

template<typename _HandleT, typename _CoordT>
bool TexturePacker2D<_HandleT, _CoordT>::_pack(const Texture* const sortedTextures, const uSys textureCount, const uSys mapWidth, const uSys mapHeight) noexcept
{
    _freeSpaces.clear(false);
    _allocatedSpaces.clear(false);

    _freeSpaces.emplace(0, 0, mapHeight, mapWidth);

    for(uSys i = 0; i < textureCount; ++i)
    {
        const Texture& texture = sortedTextures[i];

        bool success = false;

        for(iSys j = _freeSpaces.count() - 1; j >= 0; --j)
        {
            const Rect& freeSpace = _freeSpaces[j];
            
            const uSys width = freeSpace.right - freeSpace.left;
            const uSys height = freeSpace.bottom - freeSpace.top;

            if(texture.width > width || texture.height > height)
            { continue; }

            success = true;

            _allocatedSpaces.emplace(texture.handle, freeSpace.left, freeSpace.top);

            if(texture.width == width && texture.height == height)
            {
                _freeSpaces.remove(j);
            }
            else if(texture.width == width && texture.height < height)
            {
                const Rect rect(freeSpace.top + texture.height, freeSpace.left, freeSpace.bottom, freeSpace.right);

                _freeSpaces.remove(j);
                _freeSpaces.add(rect);
            }
            else if(texture.width < width && texture.height == height)
            {
                const Rect rect(freeSpace.top, freeSpace.left + texture.width, freeSpace.bottom, freeSpace.right);

                _freeSpaces.remove(j);
                _freeSpaces.add(rect);
            }
            else
            {
                const _CoordT aRight = texture.height > texture.width ? freeSpace.right : freeSpace.left + texture.width;
                const _CoordT bBottom = texture.height > texture.width ? freeSpace.top + texture.height : freeSpace.bottom;

                const Rect rectA(freeSpace.top + texture.height, freeSpace.left, freeSpace.bottom, aRight);
                const Rect rectB(freeSpace.top, freeSpace.left + texture.width, bBottom, freeSpace.right);

                const uSys areaA = (rectA.bottom - rectA.top) * (rectA.right - rectA.left);
                const uSys areaB = (rectB.bottom - rectB.top) * (rectB.right - rectB.left);
                
                _freeSpaces.remove(j);

                if(areaA > areaB)
                {
                    _freeSpaces.add(rectA);
                    _freeSpaces.add(rectB);
                }
                else
                {
                    _freeSpaces.add(rectB);
                    _freeSpaces.add(rectA);
                }
            }

            break;
        }

        if(!success)
        { return false; }
    }

    return true;
}
