#pragma once

#include <IStream.hpp>
#include <DynArray.hpp>
#include <Objects.hpp>

class ResourceLoader final
{
    DELETE_CONSTRUCT(ResourceLoader);
    DELETE_DESTRUCT(ResourceLoader);
    DELETE_CM(ResourceLoader);
public:
    typedef void* (*__cdecl parseFile_f)(RefDynArray<u8> file, void* parseParam);
    typedef void (*__cdecl finalizeLoad_f)(void* file, void* finalizeParam);

    template<typename _T, typename _F>
    using parseFileT_f = _F* (*__cdecl)(RefDynArray<u8> file, _T* parseParam);

    template<typename _T, typename _F>
    using finalizeLoadT_f = void (*__cdecl)(_F* file, _T* finalizeParam);

    struct FutureData final
    {
        void* fileData;
        void* finalizeParam;
        finalizeLoad_f finalizeLoad;
    };
public:
    static void update() noexcept;

    static void loadFile(tau::IStream* file, parseFile_f parseFile, void* parseParam, finalizeLoad_f finalizeLoad, void* finalizeParam) noexcept;

    template<typename _TParse, typename _TFinalize, typename _F>
    static void loadFileT(tau::IStream* const file, const parseFileT_f<_TParse, _F> parseFile, _TParse* const parseParam, const finalizeLoadT_f<_TFinalize, _F> finalizeLoad, _TFinalize* const finalizeParam) noexcept
    {
        loadFile(file,
                 reinterpret_cast<parseFile_f>(parseFile), parseParam,
                 reinterpret_cast<finalizeLoad_f>(finalizeLoad), finalizeParam);
    }
};
