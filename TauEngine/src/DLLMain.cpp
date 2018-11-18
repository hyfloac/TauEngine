#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#pragma warning(push, 0)
#include <windows.h>
#pragma warning(pop)
#include <Maths.hpp>
#include <TauEngine.hpp>

void initProgramStartTimes() noexcept;

BOOL APIENTRY DllMain(HMODULE hInstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            tauInit();
            initSinTable();
            initProgramStartTimes();
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
        default: break;
    }

    return TRUE;
}
