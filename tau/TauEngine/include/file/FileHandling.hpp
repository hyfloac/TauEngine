/** 
 * @file
 */
#pragma once

#include <DLL.hpp>

TAU_DLL char* readFile(const char* filePath) noexcept;

#ifdef _WIN32
/**
 * Relies on system specific operations to optimize file reading.
 * 
 *   The C API has to map Windows' HANDLE to FILE*, this has a
 * non-insignificant overhead.
 */
TAU_DLL char* readFileFast(const char* filePath) noexcept;
#else
static inline char* readFileFast(const char* filePath) noexcept
{
    return readFile(filePath);
}
#endif

/**
 *   Due to DLL boundaries this may be necessary to ensure the 
 * proper deletion of the data without memory corruption.
 */
TAU_DLL void freeFileData(char* fileData) noexcept;
