#include "UnitTest.hpp"
#include "TexturePackingTest.hpp"
#include <TexturePacker2D.hpp>
#include <cstdio>
#include <cstdlib>

#include "DynArray.hpp"

#define SHOULD_PRINT 1

void writeBitmap(const char* fileName, const u8* textureData, u32 width, u32 height) noexcept;

struct Color final
{
    u8 r, g, b;
};

static Color PALET[]
{
    {   0,   0,   0 },
    {  64,  64,  64 },
    { 128, 128, 128 },
    { 255, 255, 255 },
    { 255,   0,   0 },
    {   0, 255,   0 },
    {   0,   0, 255 },
    { 255, 255,   0 },
    {   0, 255, 255 },
    { 255,   0, 255 },
    { 128,   0,   0 },
    {   0, 128,   0 },
    {   0,   0, 128 },
    { 128, 128,   0 },
    {   0, 128, 128 },
    { 128,   0, 128 },
};

TAU_TEST(TexturePacking, squareTest)
{
    using TexturePacker = TexturePacker2D<u16, u16>;
    
    constexpr uSys randomCount = 32;
    constexpr uSys smallSquareCount = 196;
    constexpr uSys textureCount = randomCount + smallSquareCount;
    constexpr uSys mapSize = 256;

    TexturePacker packer(textureCount);

    DynArray<Color> colors(textureCount);
    DynArray<TexturePacker::Texture> textures(textureCount);

#if 1
    srand(0);
#else
    srand(time(nullptr));
#endif

    for(uSys i = 0; i < randomCount; ++i)
    {
        textures[i].handle = static_cast<u16>(i);

        textures[i].width = rand() % 24 + 2;
        textures[i].height = rand() % 24 + 2;

        colors[i].r = static_cast<u8>(rand() % 256);
        colors[i].g = static_cast<u8>(rand() % 256);
        colors[i].b = static_cast<u8>(rand() % 256);
    }

    for(uSys i = randomCount; i < randomCount + smallSquareCount; ++i)
    {
        textures[i].handle = static_cast<u16>(i);

        textures[i].width = 4;
        textures[i].height = 4;

        colors[i].r = static_cast<u8>(rand() % 256);
        colors[i].g = static_cast<u8>(rand() % 256);
        colors[i].b = static_cast<u8>(rand() % 256);
    }

    packer.pack(textures, textures.count(), mapSize, 1);

    const uSys textureWidth = packer.packedWidth();
    const uSys textureHeight = packer.packedHeight();

    u8* const rawTexture = reinterpret_cast<u8*>(operator new(textureWidth * textureHeight * 3, ::std::nothrow));

    ::std::memset(rawTexture, 0xCD, textureWidth * textureHeight * 3);

    for(uSys i = 0; i < packer.allocatedSpaces().count(); ++i)
    {
        const TexturePacker::Coord loc = packer.allocatedSpaces()[i];

        printf("Texture[%u]: (%u, %u)\n", static_cast<u32>(i), loc.x, loc.y);

        const TexturePacker::Texture& texture = textures[loc.handle];
        const Color& color = colors[loc.handle];

        const uSys maxY = loc.y + texture.height;
        const uSys maxX = loc.x + texture.width;

        for(uSys y = loc.y; y < maxY; ++y)
        {
            for(uSys x = loc.x; x < maxX; ++x)
            {
                const uSys pixel = y * textureWidth + x;

                if(rawTexture[pixel * 3 + 0] != 0xCD ||
                   rawTexture[pixel * 3 + 1] != 0xCD ||
                   rawTexture[pixel * 3 + 2] != 0xCD)
                {
                    printf("Overwrite Detected: (%u, %u)\n", static_cast<u32>(x), static_cast<u32>(y));
                }

                rawTexture[pixel * 3 + 0] = color.r;
                rawTexture[pixel * 3 + 1] = color.g;
                rawTexture[pixel * 3 + 2] = color.b;
            }
        }
    }

    writeBitmap("squareTest.bmp", rawTexture, static_cast<u32>(textureWidth), static_cast<u32>(textureHeight));

    operator delete(rawTexture);
}

void writeBitmap(const char* const fileName, const u8* const textureData, const u32 width, const u32 height) noexcept
{
    constexpr u32 fileHeaderSize = 0x0A;
    constexpr u32 bmpHeaderSize = 0x28;

    FILE* const file = fopen(fileName, "w");

    const u8* widthChar = reinterpret_cast<const u8*>(&width);
    const u8* heightChar = reinterpret_cast<const u8*>(&height);

    const u32 paddedWidth = ((24 * width + 31) / 32) * 4;

    const u32 widthDiff = paddedWidth - width * 3;

    const u32 fillSize = paddedWidth * height; // Includes padding
    const u8* fillSizeChar = reinterpret_cast<const u8*>(&fillSize);

    const u32 fileSize = fileHeaderSize + bmpHeaderSize + fillSize;
    const u8* fileSizeChar = reinterpret_cast<const u8*>(&fileSize);

    u8 fileHeader[] = {
        'B', 'M', // Signature
        fileSizeChar[0], fileSizeChar[1], fileSizeChar[2], fileSizeChar[3], // File Size
        0, 0, // Reserved
        0, 0, // Reserved
        0x36, 0, 0, 0 // Data Ptr
    };

    u8 bmpHeader[] = {
        0x28, 0, 0, 0, // Header size
        widthChar[0], widthChar[1], widthChar[2], widthChar[3], // Width
        heightChar[0], heightChar[1], heightChar[2], heightChar[3], // Height
        0x01, 0x00, // nb plan
        0x18, 0x00, // bpp
        0, 0, 0, 0, // compression (uncompressed)
        fillSizeChar[0], fillSizeChar[1], fillSizeChar[2], fillSizeChar[3], // Image Size
        0, 0, 0, 0, // Horizontal resolution (pixel per meter)
        0, 0, 0, 0, // Vertical resolution (pixel per meter)
        0, 0, 0, 0, // Total colors used, 0 defaults to 2**n
        0, 0, 0, 0, // The number of important colors used, 0 is all colors. Ignored
    };

    (void) fwrite(fileHeader, sizeof(fileHeader), 1, file);
    (void) fwrite(bmpHeader, sizeof(bmpHeader), 1, file);

    if(widthDiff > 0)
    {
        u8* fillBuffer = reinterpret_cast<u8*>(operator new(fillSize));

        ::std::memset(fillBuffer, 0, fillSize);

        uSys readIndex = 0;
        uSys writeIndex = 0;

        for(uSys y = 0; y < height; ++y)
        {
            ::std::memcpy(fillBuffer + writeIndex, textureData + readIndex, width * 3);
            writeIndex += paddedWidth;
            readIndex += width * 3;
        }

        (void) fwrite(fillBuffer, fillSize, 1, file);

        operator delete(fillBuffer);
    }
    else
    {
        (void) fwrite(textureData, fillSize, 1, file);
    }

    fclose(file);
}


namespace TexturePackingTests {
void runTests()
{
    RUN_ALL_TESTS();
}
}

