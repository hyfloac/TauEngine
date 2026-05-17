cmake_minimum_required(VERSION 3.25)
project(ResourceLib VERSION 1.0.0 LANGUAGES CXX C)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# This is a helper utility for generating the folder layout in VS.
include(GenVsFilters)
include(SetCompileFlags)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c" "src/*.rc")
file(GLOB_RECURSE HEADERS "include/*.hpp" "include/*.h" "include/*.inl")

set(TAU_ENGINE_SOURCE_FILES ${SOURCES} ${HEADERS})

add_library(${PROJECT_NAME} ${TAU_ENGINE_SOURCE_FILES})

GenVsFilters(HEADERS)
GenVsFilters(SOURCES)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})
target_sources(${PROJECT_NAME} PUBLIC FILE_SET "HEADERS" BASE_DIRS "include" FILES ${HEADERS})

# Set the include directory.
target_include_directories(${PROJECT_NAME} PUBLIC include)
# Set the source directory.
target_include_directories(${PROJECT_NAME} PRIVATE src)

find_package(TauUtils REQUIRED)
find_package(TauCOM REQUIRED)
find_package(glm REQUIRED)
find_package(EASTL REQUIRED)
find_package(tomlplusplus REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC
    tauutils::tauutils
    taucom::taucom
    glm::glm
    EASTL::EASTL
    tomlplusplus::tomlplusplus
)

target_link_libraries(${PROJECT_NAME} PRIVATE LZMA::LZMA)

# NVIDIA stdexec — reference implementation of the C++26 std::execution
# proposal. Used for the io_uring async file backend on Linux.
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    include(FetchContent)
    FetchContent_Declare(
        stdexec
        GIT_REPOSITORY https://github.com/NVIDIA/stdexec.git
        GIT_TAG        nvhpc-25.09
        GIT_SHALLOW    TRUE
    )
    set(STDEXEC_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(STDEXEC_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(stdexec)

    target_link_libraries(${PROJECT_NAME} PUBLIC STDEXEC::stdexec)

    # liburing — userspace wrapper around the io_uring syscalls. Used
    # by the IoUringFile async backend.
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBURING REQUIRED IMPORTED_TARGET liburing)
    target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::LIBURING)
endif()

SetCompileFlags(${PROJECT_NAME} PUBLIC PRIVATE ${BUILD_SHARED_LIBS})

target_compile_definitions(${PROJECT_NAME} PUBLIC
    -DEA_PRAGMA_ONCE_SUPPORTED
    -DGLM_ENABLE_EXPERIMENTAL
    -D_ALLOW_RANDOM_DISTRIBUTION_CONST_OPERATOR
    -DTOML_EXCEPTIONS=0
    -DTAU_COM_USE_TAU_UTILS
)

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME_DEPENDENCY_SET ResourceLibDeps
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_BINDIR}
    BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}
    FILE_SET HEADERS
)

if(APPLE)
    install(
        RUNTIME_DEPENDENCY_SET ResourceLibDeps
        PRE_EXCLUDE_REGEXES
            [=[api-ms-]=]
            [=[ext-ms-]=]
            [[kernel32\.dll]]
            [[libc\.so\..*]] [[libgcc_s\.so\..*]] [[libm\.so\..*]] [[libstdc\+\+\.so\..*]]
        POST_EXCLUDE_REGEXES
            [=[.*system32\/.*\.dll]=]
            [=[^\/(lib|usr\/lib|usr\/local\/lib)]=]
        LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}.app/Contents/MacOS
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}.app/Contents/MacOS
        FRAMEWORK DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}.app/Contents/MacOS
    )
else()
    install(
        RUNTIME_DEPENDENCY_SET ResourceLibDeps
        PRE_EXCLUDE_REGEXES
            [=[api-ms-]=]
            [=[ext-ms-]=]
            [[kernel32\.dll]]
            [[libc\.so\..*]] [[libgcc_s\.so\..*]] [[libm\.so\..*]] [[libstdc\+\+\.so\..*]]
        POST_EXCLUDE_REGEXES
            [=[.*system32\/.*\.dll]=]
            [=[^\/(lib|usr\/lib|usr\/local\/lib)]=]
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        FRAMEWORK DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    FILE_SET HEADERS
)

add_library(ResourceLib::ResourceLib ALIAS ${PROJECT_NAME})

# Tests (enabled when BUILD_TESTING is ON, set by include(CTest) in the
# top-level CMakeLists.txt).
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
