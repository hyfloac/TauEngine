/**
 * @file 
 *
 * Some basic functions for controlling the Tau Engine.
 */
#pragma once

#include <DLL.hpp>

/**
 * @return
 *    Returns true if initialization was successful. 
 */
TAU_DLL bool tauInit() noexcept;

/**
 *   Gets up to `NUM_MESSAGES_TO_READ` [default `8`] messages 
 * and dispatches them.
 */
TAU_DLL void tauRunMessageLoop() noexcept;

/**
 * @return
 *    Returns true if the program should exit.
 */
TAU_DLL bool tauShouldExit() noexcept;

/**
 * Tells the application that we are ready to exit.
 * 
 * @param[in] code
 *    The exit code of the program.
 */
TAU_DLL void tauExit(int code) noexcept;
