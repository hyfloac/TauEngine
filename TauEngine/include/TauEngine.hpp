/**
 * @file 
 *
 * Some basic functions for controlling the Tau Engine.
 */
#pragma once

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)
#include <DLL.hpp>

std::shared_ptr<spdlog::logger> getEngineLogger() noexcept;

/**
 * @return
 *    Returns true if initialization was successful. 
 */
bool tauInit() noexcept;

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
