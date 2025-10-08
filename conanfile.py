from conan import ConanFile
from conan.tools.microsoft import MSBuildDeps
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps

class TauEngineRecipe(ConanFile):
    name = "tauengine"

    # Optional metadata
    license = "MIT"
    author = "Hyfloac <mail@hyfloac.com>"
    url = "https://github.com/hyfloac/TauEngine"
    description = ""

    settings = "os", "compiler", "arch", "build_type"
    requires = (
        "tauutils/[^1.4.6]",
        "taucom/[^0.3.1]",
        "glm/[^1.0.1]",
        "glew/[^2.2.0]",
        "vulkan-memory-allocator/[^3.0.1]",
        "eastl/[^3.21.12]",
        "tomlplusplus/[^3.4.0]",
        "freetype/2.13.2",
        "fontconfig/[^2.15.0]",
        "freeimage/[^3.18.0]",
        "openvr/[^1.16.8]"
    )
    options = {
        "shared": [True, False],
    }
    default_options = {
        "shared": False,
    }

    def configure(self):
        self.options["tauutils"].shared = True

        self.options["taucom"].shared = True
        self.options["taucom"].useTauUtils = True

        self.options["glew"].shared = True

    def layout(self):
        # self.folders.generators = "libs/conan"
        cmake_layout(self)

    def generate(self):
        ms = MSBuildDeps(self)
        # ms.generate()
        if self.settings.build_type == "Debug":
            ms.configuration = "OptimizedDebug"
            # ms.generate()
        if self.settings.build_type == "Release":
            ms.configuration = "Production"
            # ms.generate()
            ms.configuration = "TRG_Release"
            # ms.generate()
            
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.variables["BUILD_SHARED_LIBS"] = self.options.shared
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
