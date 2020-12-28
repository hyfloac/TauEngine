#include "system/GraphicsAPILoader.hpp"

#ifdef _WIN32
#include "system/GraphicsAPI.h"
#include <Windows.h>

GraphicsAPILoader::APICache GraphicsAPILoader::_apiCache;

bool GraphicsAPILoader::loadAPIs(const WDynString& folder) noexcept
{
    // UNC Path Prefix
    static const WDynString prefix(L"\\\\?\\");

    // Convert folder to a UNC path
    const WDynString uncPath = prefix.concat(folder).concat(L"\\");
    // Append a '*' for the search
    const WDynString searchPath = uncPath.concat(L"*");

    // Load the first file
    WIN32_FIND_DATAW ffd;
    const HANDLE find = FindFirstFileW(searchPath.c_str(), &ffd);

    // Validate the find handle
    if(find == INVALID_HANDLE_VALUE)
    { return false; }

    do
    {
        // Is this file a directory?
        if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        { continue; }

        // Create the full path to the library
        WDynString filePath = uncPath.concat(ffd.cFileName);

        // Load the library
        HMODULE library = LoadLibraryW(filePath.c_str());

        // Load tauGetGraphicsAPIName
        const tauGetGraphicsAPIName_f getGraphicsAPIName = reinterpret_cast<tauGetGraphicsAPIName_f>(GetProcAddress(library, tauGetGraphicsAPIName_name));
        // Validate tauGetGraphicsAPIName
        if(!getGraphicsAPIName)
        {
            FreeLibrary(library);
            continue;
        }

        // Load tauLoadGraphicsAPI
        const tauLoadGraphicsAPI_f loadGraphicsAPI = reinterpret_cast<tauLoadGraphicsAPI_f>(GetProcAddress(library, tauLoadGraphicsAPI_name));
        // Validate tauLoadGraphicsAPI
        if(!loadGraphicsAPI)
        {
            FreeLibrary(library);
            continue;
        }

        // Load the API
        if(!loadGraphicsAPI())
        {
            FreeLibrary(library);
            continue;
        }

        // Cache the API
        const DynString apiName = getGraphicsAPIName();
        _apiCache[apiName] = APIData(::std::move(filePath), library);

    } while(FindNextFileW(find, &ffd));

    return true;
}

bool GraphicsAPILoader::loadAPI(const DynString& api) noexcept
{
    return true;
}

bool GraphicsAPILoader::freeAPI(const DynString& api) noexcept
{
    return true;
}

#endif
