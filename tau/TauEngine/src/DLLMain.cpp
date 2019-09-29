#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#pragma warning(push, 0)
#include <Windows.h>
#pragma warning(pop)
#include <maths/Maths.hpp>
#include <TauEngine.hpp>

void initProgramStartTimes() noexcept;

HMODULE ___currDLL__HMODULE = 0;

HMODULE tauGetDLLModule() noexcept
{
    return ___currDLL__HMODULE;
}

void tauMain()
{
    tauInit();
    initSinTable();
    initProgramStartTimes();
}

BOOL APIENTRY DllMain(HMODULE hInstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            ___currDLL__HMODULE = hInstDLL;
            tauMain();
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
        default: break;
    }

    return TRUE;
}
