#pragma once

#include <NumTypes.hpp>

#define EXPORT __declspec(dllexport)

typedef struct 
{
    int argCount;
    char** args;
} Arguments;


typedef void (* update_f)(float);
typedef void (* render_f)(float);

typedef struct
{
    u32 targetUPS;
    update_f updateFunc;
    render_f renderFunc;
    u8 printFPSInterval;
} GameLoopData;

void EXPORT __cdecl receiveArguments(const Arguments* args) noexcept;

int EXPORT __cdecl initGame() noexcept;

int EXPORT __cdecl finalizeGame() noexcept;

GameLoopData EXPORT __cdecl getGameLoopData() noexcept;


