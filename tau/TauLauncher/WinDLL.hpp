#pragma once

#include <Windows.h>
#include <NumTypes.hpp>

FARPROC GetProcAddress_CppImpl(HMODULE hModule, const char* name, const char* signature) noexcept;

template <typename T>
T GetProcAddressCPP(HMODULE hModule, const char* name) noexcept
{
    const char* signature = __FUNCDNAME__ + 21; // Ignore prefix "??$GetProcAddress@"
    return reinterpret_cast<T>(GetProcAddress_CppImpl(hModule, name, signature));
}

template<typename T>
T GetProcAddressCPPI(HMODULE hModule, const char* name) noexcept
{
    const char* signature = __FUNCDNAME__ + 22; // Ignore prefix "??$GetProcAddress@"

    const size_t nameLen = strlen(name);
    const char* offset = strstr(signature, "@@YA");
    const u32 len = static_cast<u32>(offset - signature);

    char* funcName = new char[nameLen + len + 3];
    funcName[nameLen + len + 2] = '\0';
    funcName[0] = '?';

    memcpy(funcName + 1, name, nameLen);
    funcName[1 + nameLen] = '@';
    funcName[2 + nameLen] = '@';
    funcName[3 + nameLen] = 'Y';
    memcpy(funcName + 4 + nameLen, signature + 2, len - 2);

    FARPROC ret = ::GetProcAddress(hModule, funcName);

    delete[] funcName;

    return reinterpret_cast<T>(ret);
}
