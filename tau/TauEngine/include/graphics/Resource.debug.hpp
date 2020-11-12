#pragma once

#include <String.hpp>

#include "DLL.hpp"

#ifndef TAU_RESOURCE_DEBUG_DATA
  #if defined(TAU_PRODUCTION)
    #define TAU_RESOURCE_DEBUG_DATA 0
  #else
    #define TAU_RESOURCE_DEBUG_DATA 1
  #endif
#endif

#if TAU_RESOURCE_DEBUG_DATA
namespace tau { namespace debug {

class TAU_DLL ResourceDebugCategory final
{
    DEFAULT_DESTRUCT(ResourceDebugCategory);
    DELETE_CM(ResourceDebugCategory);
public:
    static ResourceDebugCategory PositionVertexBuffer;
    static ResourceDebugCategory NormalVertexBuffer;
    static ResourceDebugCategory TangentVertexBuffer;
    static ResourceDebugCategory BitangentVertexBuffer;
    static ResourceDebugCategory TextureUVVertexBuffer;
    static ResourceDebugCategory IndexBuffer;
    static ResourceDebugCategory UniformBuffer;
    static ResourceDebugCategory UploadBuffer;

    static ResourceDebugCategory DiffuseTexture;
    static ResourceDebugCategory NormalTexture;
    static ResourceDebugCategory HeightTexture;
    static ResourceDebugCategory AmbientOcclusionTexture;

    static ResourceDebugCategory MetalnessTexture;
    static ResourceDebugCategory RoughnessTexture;
    static ResourceDebugCategory MetalnessF0ControlTexture;

    static ResourceDebugCategory SpecularTexture;
    static ResourceDebugCategory GlossinessTexture;

    static ResourceDebugCategory OtherBuffer;
    static ResourceDebugCategory OtherTexture;
    static ResourceDebugCategory Other;
private:
    WDynString _category;
public:
    ResourceDebugCategory(const WDynString& category) noexcept
        : _category(category)
    { }

    ResourceDebugCategory(WDynString&& category) noexcept
        : _category(::std::move(category))
    { }

    [[nodiscard]] const WDynString& category() const noexcept { return _category; }
};

class TAU_DLL ResourceDebugData final
{
    DEFAULT_DESTRUCT(ResourceDebugData);
private:
    const ResourceDebugCategory& _category;
    WDynString _name;
public:
    ResourceDebugData(const ResourceDebugCategory& category, const WDynString& name) noexcept
        : _category(category)
        , _name(name)
    { }

    ResourceDebugData(const ResourceDebugCategory& category, WDynString&& name) noexcept
        : _category(category)
        , _name(::std::move(name))
    { }

    ResourceDebugData(const ResourceDebugData& copy) noexcept = default;
    ResourceDebugData(ResourceDebugData&& move) noexcept = default;

    ResourceDebugData& operator=(const ResourceDebugData& copy) noexcept = delete;
    ResourceDebugData& operator=(ResourceDebugData&& move) noexcept = delete;

    [[nodiscard]] const ResourceDebugCategory& category() const noexcept { return _category; }
    [[nodiscard]] const WDynString& name() const noexcept { return _name; }
};

} }
#endif
