cmake_minimum_required(VERSION 3.25)
project(ResourceLibTests LANGUAGES CXX)

include(SetCompileFlags)

# Prefer a Conan/system-provided GoogleTest if it's available; otherwise
# fall back to FetchContent so the test target is buildable in any
# environment.
find_package(GTest QUIET)

if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.15.2
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

file(GLOB TEST_SOURCES "*.cpp")

add_executable(${PROJECT_NAME} ${TEST_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ResourceLib::ResourceLib
    GTest::gtest
    GTest::gtest_main
)

SetCompileFlags(${PROJECT_NAME} PRIVATE PRIVATE OFF)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    -DEA_PRAGMA_ONCE_SUPPORTED
    -DTOML_EXCEPTIONS=0
    -DTAU_COM_USE_TAU_UTILS
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
