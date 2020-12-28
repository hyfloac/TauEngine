#pragma once

#include "DLL.hpp"
#include <Objects.hpp>
#include <String.hpp>

#include <unordered_map>

class TAU_DLL GraphicsAPILoader final
{
    DELETE_CONSTRUCT(GraphicsAPILoader);
    DELETE_DESTRUCT(GraphicsAPILoader);
    DELETE_CM(GraphicsAPILoader);
public:
    struct APIData final
    {
        DEFAULT_CONSTRUCT_PU(APIData);

        WDynString libPath;
        void* libHandle;

        APIData(const WDynString& _libPath, void* const _libHandle) noexcept
            : libPath(_libPath)
            , libHandle(_libHandle)
        { }

        APIData(WDynString&& _libPath, void* const _libHandle) noexcept
            : libPath(::std::move(_libPath))
            , libHandle(_libHandle)
        { }
    };

    using APICache = ::std::unordered_map<DynString, APIData>;
public:
    static bool loadAPIs(const WDynString& folder) noexcept;
    
    static bool loadAPI(const DynString& api) noexcept;
    static bool freeAPI(const DynString& api) noexcept;
private:
    static APICache _apiCache;
};
