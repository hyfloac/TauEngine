#include "ResourceLoader.hpp"

void ResourceLoader::update() noexcept
{
    auto it = _futures.begin();
    while(it != _futures.end())
    {
        std::future<FutureData>& fut = *it;
        if(fut._Is_ready())
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

ResourceLoader::FutureData loadFileAsync(Ref<IFile> file, ResourceLoader::parseFile_f parseFile, void* parseParam, ResourceLoader::finalizeLoad_f finalizeLoad, void* finalizeParam) noexcept
{
    const RefDynArray<u8> fileData = file->readFile();
    void* fileParse = parseFile(fileData, parseParam);
    return { fileParse, finalizeParam, finalizeLoad };
}

void ResourceLoader::loadFile(Ref<IFile> file, parseFile_f parseFile, void* parseParam, finalizeLoad_f finalizeLoad, void* finalizeParam) noexcept
{
    if(!file || !parseFile || !finalizeLoad)
    {
        return;
    }

    _futures.push_back(std::async(std::launch::async, loadFileAsync, file, parseFile, parseParam, finalizeLoad, finalizeParam));
}
