#include "ResourceLoader.hpp"

#pragma warning(push, 0)
#include <list>
#include <future>
#include <Safeties.hpp>
#include <StreamUtils.hpp>
#pragma warning(pop)

static std::list<std::future<ResourceLoader::FutureData>> _futures;

void ResourceLoader::update() noexcept
{
    auto it = _futures.begin();
    while(it != _futures.end())
    {
        std::future<FutureData>& fut = *it;
        // if(fut._Is_ready())
        if(fut.wait_for(::std::chrono::microseconds(0)) == ::std::future_status::ready)
        {
            const FutureData data = fut.get();
            data.finalizeLoad(data.fileData, data.finalizeParam);
            it = _futures.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

static ResourceLoader::FutureData loadFileAsync(tau::com::ComRef<tau::IStream> file, ResourceLoader::parseFile_f parseFile, void* parseParam, ResourceLoader::finalizeLoad_f finalizeLoad, void* finalizeParam) noexcept
{
    const RefDynArray<u8> fileData = tau::ReadAll(file.Get());
    void* fileParse = parseFile(fileData, parseParam);
    return { fileParse, finalizeParam, finalizeLoad };
}

void ResourceLoader::loadFile(tau::IStream* const file, parseFile_f parseFile, void* parseParam, finalizeLoad_f finalizeLoad, void* finalizeParam) noexcept
{
    if(!file || !parseFile || !finalizeLoad)
    { return; }

    file->AddReference();
    _futures.push_back(std::async(std::launch::async, loadFileAsync, tau::com::ComRef<tau::IStream>(file), parseFile, parseParam, finalizeLoad, finalizeParam));
}
