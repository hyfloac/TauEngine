#ifdef BUILD_EXE
#include <TextureLoader.h>
#include <TextureBlend.hpp>
#include <cstdio>

int main0(u32 argCount, char* args[])
{
    if(argCount < 4)
    {
        return 1;
    }

    const char* img0 = args[1];
    const char* img1 = args[2];
    const char* outImg = args[3];

    printf("Img0: %s\n", img0);
    printf("Img1: %s\n", img1);
    printf("Out: %s\n", outImg);

    TextureLoadError error;

    std::shared_ptr<FITexture> tex0 = loadTexture(img0, &error);
    
    if(error != TextureLoadError::NONE)
    {
        return 2;
    }
    
    std::shared_ptr<FITexture> tex1 = loadTexture(img1, &error);

    if(error != TextureLoadError::NONE)
    {
        return 3;
    }

    TextureBlend* blend = xorImagesSIMD<FITexture, FITexture>(&(*tex0), &(*tex1));

    FIBITMAP* fiBitmap = FreeImage_Allocate(blend->texture->width, blend->texture->height, blend->texture->bitsPerPixel);

    memcpy(FreeImage_GetBits(fiBitmap), blend->texture->pixels, blend->texture->width * blend->texture->height * 4);

    const bool success = saveTexture(fiBitmap, outImg);

    if(!success)
    {
        printf("Failed to save image.\n");
        return 4;
    }

    delete blend;

    FreeImage_Unload(fiBitmap);

    return 0;
}

int main1(int argCount, char* args[])
{
    if(argCount < 3)
    {
        return 1;
    }

    const char* img0 = args[1];
    const char* outImg = args[2];

    printf("Img0: %s\n", img0);
    printf("Out: %s\n", outImg);

    TextureLoadError error;

    std::shared_ptr<FITexture> tex0 = loadTexture(img0, &error);

    if(error != TextureLoadError::NONE)
    {
        return 2;
    }

    TextureBlend* blend = filterImage(&(*tex0));

    FIBITMAP* fiBitmap = FreeImage_Allocate(blend->texture->width, blend->texture->height, blend->texture->bitsPerPixel);

    memcpy(FreeImage_GetBits(fiBitmap), blend->texture->pixels, static_cast<u64>(blend->texture->width) * static_cast<u64>(blend->texture->height) * 4);

    const bool success = saveTexture(fiBitmap, outImg);

    if(!success)
    {
        printf("Failed to save image.\n");
        return 4;
    }

    delete blend;

    FreeImage_Unload(fiBitmap);

    return 0;
}

int main2(int argCount, char* args[])
{
    if(argCount < 2)
    {
        return 1;
    }

    const char* outImg = args[1];

    printf("Out: %s\n", outImg);

    TextureBlend* blend = createDebug<64>();

    FIBITMAP* fiBitmap = FreeImage_Allocate(blend->texture->width, blend->texture->height, blend->texture->bitsPerPixel);

    memcpy(FreeImage_GetBits(fiBitmap), blend->texture->pixels, static_cast<u64>(blend->texture->width)* static_cast<u64>(blend->texture->height) * 4);

    const bool success = saveTexture(fiBitmap, outImg);

    if(!success)
    {
        printf("Failed to save image.\n");
        return 4;
    }

    delete blend;

    FreeImage_Unload(fiBitmap);

    return 0;
}

int main(int argCount, char* args[])
{
    int ret = -1;
    try
    {
        ret = main2(argCount, args);
    }
    catch(...)
    {
        printf("Exception Encountered.\n");
    }
    printf("Return code: %d\n", ret);
    return ret;
}
#endif
