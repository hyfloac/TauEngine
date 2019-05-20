#include <iostream>
#include <cstdio>
#include <Windows.h>
#include <Utils.hpp>
#include <NumTypes.hpp>
#include "WinDLL.hpp"
#include <cmath>
#include <CoreFuncs.h>

#define MAX_GAME_DLL_NAME_SIZE (64)

void changeFileInPath(char* buffer, const char* newFile) noexcept;

u64 currentTimeMillis() noexcept
{
    /* Windows */
    FILETIME fileTime;
    ULARGE_INTEGER fileTimeInt;

    /* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
     * to a LARGE_INTEGER structure. */
    GetSystemTimeAsFileTime(&fileTime);
    fileTimeInt.LowPart = fileTime.dwLowDateTime;
    fileTimeInt.HighPart = fileTime.dwHighDateTime;

    u64 ret = static_cast<u64>(fileTimeInt.QuadPart);
    ret -= 116444736000000000uLL; /* Convert from file time to UNIX epoch time. */
    ret /= 10000; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals */

    return ret;
}

int main(int argCount, char* args[]) noexcept
{
    const char* propertiesFileName = "application.props";

    char path[MAX_PATH + 64];
    const DWORD pathError = GetModuleFileNameA(null, path, MAX_PATH);

    if(!pathError)
    {
        std::cerr << "Error retrieving path." << std::endl;
        std::cin.get();
        return 3;
    }

    std::cout << "Executable Path: " << path << std::endl;

    changeFileInPath(path, propertiesFileName);

    std::cout << "Properties Path: " << path << std::endl;

    FILE* propertiesFile;
    const errno_t fileOpenError = fopen_s(&propertiesFile, path, "r");

    if(fileOpenError)
    {
        std::cerr << "File `" << path << "` failed to open." << std::endl;
        std::cin.get();
        return 1;
    }

    char gameDLLName[MAX_GAME_DLL_NAME_SIZE];
    char* str = fgets(gameDLLName, MAX_GAME_DLL_NAME_SIZE, propertiesFile);
    if(!str)
    {
        std::cerr << "File `" << path << "` didn't have any contents." << std::endl;
        std::cin.get();
        return 2;
    }

    changeFileInPath(path, gameDLLName);

    std::cout << "Game DLL Name: " << gameDLLName << std::endl;
    std::cout << "Game DLL Path: " << path << std::endl;

    HMODULE gameModule = LoadLibraryA(path);

    auto receiveArguments_f = GetProcAddressCPPI<void(__cdecl *)(const Arguments*)>(gameModule, "receiveArguments");
    auto initGame_f = GetProcAddressCPPI<int(__cdecl *)(void)>(gameModule, "initGame");
    auto finalizeGame_f = GetProcAddressCPPI<int(__cdecl *)(void)>(gameModule, "finalizeGame");
    auto getGameLoopData_f = GetProcAddressCPPI<GameLoopData(__cdecl *)(void)>(gameModule, "getGameLoopData");

    if(receiveArguments_f &&
       initGame_f &&
       finalizeGame_f &&
       getGameLoopData_f)
    {
        {
            Arguments arguments = { argCount, args };
            receiveArguments_f(&arguments);
        }
        initGame_f();
        // GameLoopData gld = getGameLoopData_f();

        finalizeGame_f();
    }

    return 0;
}

void changeFileInPath(char* buffer, const char* newFile) noexcept
{
    const size_t pathLen = strlen(buffer);
    const size_t newFileLen = strlen(newFile);
    size_t slashIndex = 0;

    for(size_t i = pathLen - 1; i >= 0; --i)
    {
        if(buffer[i] == '\\')
        {
            slashIndex = i + 1;
            break;
        }
    }

    for(u32 i = 0; i < newFileLen; ++i)
    {
        buffer[i + slashIndex] = newFile[i];
    }
    buffer[slashIndex + newFileLen] = '\0';
}

// void tauGameLoop(u32 targetUPS, update_f updateF, render_f renderF, u8 printFPSInterval) noexcept
// {
//     const float MS_PER_UPDATE = 1000.0f / targetUPS;
//     u64 lastTime = currentTimeMillis();
//     float lag = 0.0f;
//
//     u64 counterTime = lastTime;
//     u32 fps = 0;
//     u32 ups = 0;
//
//     const u64 _timeInterval = printFPSInterval * 1000;
//
//     while(!should_exit)
//     {
//         const u64 currentTime = currentTimeMillis();
//         const u64 elapsed = currentTime - lastTime;
//         lastTime = currentTime;
//         lag += static_cast<float>(elapsed);
//
//         while(lag >= MS_PER_UPDATE)
//         {
//             runMessageLoop();
//
//             updateF(MS_PER_UPDATE);
//             ++ups;
//             lag -= MS_PER_UPDATE;
//         }
//
//         renderF(lag / MS_PER_UPDATE);
//         ++fps;
//
//         const u64 currentCounterTime = currentTimeMillis();
//
//         if(printFPSInterval && currentCounterTime - counterTime >= _timeInterval)
//         {
//             counterTime = currentCounterTime;
//             engineLogger->debug("UPS {}", ups / printFPSInterval);
//             engineLogger->debug("FPS {}", fps / printFPSInterval);
//
//             ups = 0;
//             fps = 0;
//         }
//     }
// }
