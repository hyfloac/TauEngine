cmake_minimum_required(VERSION 3.25)
project(TauEngine VERSION 2.0.0 LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(CTest)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(TAU_COMMON_BUILD_SHARED_LIBS ON)
set(TAU_GRAPHICS_BUILD_SHARED_LIBS ON)
set(TAU_GL_DRIVER_BUILD_SHARED_LIBS ON)

add_subdirectory(libs/SDL3 EXCLUDE_FROM_ALL)
add_subdirectory(libs/LZMA)
add_subdirectory(utils/ResourceLib)
add_subdirectory(tau/TauCommon)
add_subdirectory(tau/TauGraphics)
add_subdirectory(tau/driver/TauGLDriver)
add_subdirectory(tau/TauEngine)
