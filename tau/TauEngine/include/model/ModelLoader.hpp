#pragma once

#include <TauEngine.hpp>
#include <DLL.hpp>

class ITexture;

union Material final
{
    struct Basic
    {
        ITexture* diffuse;
        ITexture* overlay;
        ITexture* normal;
        ITexture* specular;
        ITexture* ao;
    };

    struct PBRMetalness
    {
        ITexture* albedo;
        ITexture* normal;
        ITexture* metalness;
        ITexture* roughness;
        ITexture* ao;
    };

    struct PBRSpecular
    {
        ITexture* albedo;
        ITexture* normal;
        ITexture* specular;
        ITexture* gloss;
        ITexture* ao;
    };
};

struct Model final
{

};

class TAU_DLL IModelLoader
{
public:
    virtual ~IModelLoader() noexcept = default;

    virtual CPPRef<Model> load(const char* path) const noexcept = 0;
private:
    IModelLoader() noexcept = default;

    IModelLoader(const IModelLoader& copy) noexcept = delete;
    IModelLoader(IModelLoader&& move) noexcept = delete;

    IModelLoader& operator =(const IModelLoader& copy) noexcept = delete;
    IModelLoader& operator =(IModelLoader&& move) noexcept = delete;
};
