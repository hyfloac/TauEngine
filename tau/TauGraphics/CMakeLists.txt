cmake_minimum_required(VERSION 3.25)
project(TauGraphics VERSION 0.0.1 LANGUAGES CXX C)

option(TAU_GRAPHICS_BUILD_SHARED_LIBS "Build shared libraries" OFF)

# We use this to check for some compiler flags, mostly to disable warnings.
include(CheckCCompilerFlag)
# This is a helper utility for generating the folder layout in VS.
include(GenVsFilters)
include(SetCompileFlags)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c" "src/*.rc")
file(GLOB_RECURSE HEADERS "include/*.hpp" "include/*.h" "include/*.inl")

set(TAU_GRAPHICS_SOURCE_FILES ${SOURCES} ${HEADERS})

add_library(${PROJECT_NAME} ${TAU_GRAPHICS_SOURCE_FILES})

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
find_package(EASTL REQUIRED)
find_package(spdlog REQUIRED)

add_dependencies(${PROJECT_NAME} ResourceLib::ResourceLib)
add_dependencies(${PROJECT_NAME} TauCommon::TauCommon)

target_link_libraries(${PROJECT_NAME} PUBLIC
    tauutils::tauutils
    taucom::taucom
    EASTL::EASTL
    spdlog::spdlog
    ResourceLib::ResourceLib
    TauCommon::TauCommon
)

SetCompileFlags(${PROJECT_NAME} PUBLIC PRIVATE ${TAU_GRAPHICS_BUILD_SHARED_LIBS})

target_compile_definitions(${PROJECT_NAME} PUBLIC
    -DEA_PRAGMA_ONCE_SUPPORTED
    -D_ALLOW_RANDOM_DISTRIBUTION_CONST_OPERATOR
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:Debug>:TAU_RTTI_DEBUG=1>"
)

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME_DEPENDENCY_SET TauGraphicsDeps
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_BINDIR}
    BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}
    FILE_SET HEADERS
)

if(APPLE)
    install(
        RUNTIME_DEPENDENCY_SET TauGraphicsDeps
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
        RUNTIME_DEPENDENCY_SET TauGraphicsDeps
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

add_library(TauGraphics::TauGraphics ALIAS ${PROJECT_NAME})
