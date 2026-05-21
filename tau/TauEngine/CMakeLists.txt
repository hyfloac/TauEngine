cmake_minimum_required(VERSION 3.25)
project(TauEngine VERSION 2.0.0 LANGUAGES CXX C)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# We use this to check for some compiler flags, mostly to disable warnings.
include(CheckCCompilerFlag)
# This is a helper utility for generating the folder layout in VS.
include(GenVsFilters)
include(SetCompileFlags)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c" "src/*.rc")
file(GLOB_RECURSE HEADERS "include/*.hpp" "include/*.h" "include/*.inl")

if(NOT WIN32)
    list(FILTER SOURCES EXCLUDE REGEX "/dx/dx12/")
    list(FILTER HEADERS EXCLUDE REGEX "/dx/dx12/")
endif()

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
find_package(glew REQUIRED)
find_package(EASTL REQUIRED)
find_package(tomlplusplus REQUIRED)
find_package(Vulkan REQUIRED)
find_package(OpenGL REQUIRED)
find_package(freetype REQUIRED)
if(NOT WIN32)
    find_package(Fontconfig REQUIRED)
endif()
find_package(freeimage REQUIRED)
find_package(openvr REQUIRED)
find_package(spdlog REQUIRED)

add_dependencies(${PROJECT_NAME} SDL3::SDL3)
add_dependencies(${PROJECT_NAME} ResourceLib::ResourceLib)

target_link_libraries(${PROJECT_NAME} PUBLIC
    tauutils::tauutils
    taucom::taucom
    glm::glm
    GLEW::glew
    EASTL::EASTL
    tomlplusplus::tomlplusplus
    Vulkan::Vulkan
    OpenGL::GL
    Freetype::Freetype
    freeimage::FreeImage
    freeimage::FreeImagePlus
    openvr::openvr
    spdlog::spdlog
    SDL3::SDL3
    ResourceLib::ResourceLib
)

if(NOT WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC Fontconfig::Fontconfig)
endif()

if(Vulkan_MoltenVK_FOUND)
    target_link_libraries(${PROJECT_NAME} Vulkan::MoltenVK)
endif()

SetCompileFlags(${PROJECT_NAME} PUBLIC PRIVATE ${BUILD_SHARED_LIBS})

target_compile_definitions(${PROJECT_NAME} PUBLIC
    -DEA_PRAGMA_ONCE_SUPPORTED
    -DGLM_ENABLE_EXPERIMENTAL
    -D_ALLOW_RANDOM_DISTRIBUTION_CONST_OPERATOR
    -DTOML_EXCEPTIONS=0
)

if(APPLE)
    # Enable Metal for Vulkan on MacOS
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DVK_USE_PLATFORM_METAL_EXT)
endif()

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME_DEPENDENCY_SET TauEngineDeps
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_BINDIR}
    BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}
    FILE_SET HEADERS
)

#install(
#    TARGETS SDL3-shared
#    RUNTIME_DEPENDENCY_SET TauEngineDeps
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#    FRAMEWORK DESTINATION ${CMAKE_INSTALL_BINDIR}
#    BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}
#)

if(APPLE)
    install(
        RUNTIME_DEPENDENCY_SET TauEngineDeps
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
        RUNTIME_DEPENDENCY_SET TauEngineDeps
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

add_library(TauEngine::TauEngine ALIAS ${PROJECT_NAME})
