#pragma once

#include <future>
#include <DynArray.hpp>
#include <IFile.hpp>
#include <list>

class ResourceLoader final
{
public:
    typedef void* (*__cdecl parseFile_f)(RefDynArray<u8> file, void* parseParam);
    typedef void (*__cdecl finalizeLoad_f)(void* file, void* finalizeParam);

    struct FutureData final
    {
        void* fileData;
        void* finalizeParam;
        finalizeLoad_f finalizeLoad;
    };
private:
    std::list<std::future<FutureData>> _futures;
public:
    void update() noexcept;

    void loadFile(Ref<IFile> file, parseFile_f parseFile, void* parseParam, finalizeLoad_f finalizeLoad, void* finalizeParam) noexcept;
};
