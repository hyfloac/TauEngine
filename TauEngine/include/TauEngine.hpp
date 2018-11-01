#pragma once

#include <DLL.hpp>

/**
 * Returns true if initialization was successful. 
 */
TAU_DLL bool tauInit() noexcept;

/**
 * Returns true if the program should exit;
 */
TAU_DLL void tauRunMessageLoop() noexcept;

TAU_DLL bool tauShouldExit() noexcept;

TAU_DLL void tauExit(int code) noexcept;
