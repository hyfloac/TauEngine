/**
 * @file 
 *
 * Some basic functions for controlling the Tau Engine.
 */
#pragma once

#include <NumTypes.hpp>
#include "DLL.hpp"
#include "events/Exception.hpp"

/**
 * @return
 *    Returns true if initialization was successful. 
 */
bool tauInit() noexcept;

void tauMain() noexcept;

TAU_DLL void tauThrowException(Exception& e) noexcept;

TAU_DLL Exception* tauGetException() noexcept;

// /**
//  *   Gets up to `NUM_MESSAGES_TO_READ` [default `8`] messages 
//  * and dispatches them.
//  */
// TAU_DLL void tauRunMessageLoop() noexcept;

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
TAU_DLL void tauExit(i32 code) noexcept;

TAU_DLL i32 tauExitCode() noexcept;

typedef void (* update_f)(float);
typedef void (* render_f)(float);
typedef void (* renderFPS_f)(u32, u32);

class Window;

/**
 * Runs the game loop.
 * 
 * @param[in] targetUPS
 *    Specifies how many updates are desired per second.
 * @param[in] updateF
 *    The update function.
 * @param[in] renderF
 *    The render function.
 * @param[in] renderFPS
 *    A function to render the current FPS.
 */
TAU_DLL void tauGameLoop(u32 targetUPS, update_f updateF, render_f renderF, renderFPS_f renderFPS) noexcept;
