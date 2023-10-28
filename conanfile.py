from conans import ConanFile, CMake
from conans.errors import ConanInvalidConfiguration


class GraphicsEngineConan(ConanFile):
    name = "GraphicsEngine"
    version = "0.2.0"
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "enable_lto": [True, False],
        "enable_debug_layers": [True, False],
        "build_examples": [True, False],
        "build_tests": [True, False],
    }
    default_options = {
        "shared": False,
        "enable_lto": False,
        "enable_debug_layers": True,
        "build_examples": False,
        "build_tests": False,
    }
    generators = "cmake"

    _cmake = None

    def configure(self):
        if self.settings.os != "Linux":
            raise ConanInvalidConfiguration("GraphicsEngine is only supported for Linux")

    #def build_requirements(self):
        #self.build_requires("shaderc/2021.1")

    def requirements(self):
        self.requires.add("shaderc/2021.1", private=False)

        self.requires.add("vulkan-headers/1.3.236.0", private=False)
        self.requires.add("vulkan-loader/1.3.236.0", private=False)
        self.requires.add("glm/0.9.9.7", private=False)

        self.requires.add("stb/cci.20230920", private=False)

        if self.settings.os == "Linux":
            self.requires.add("xorg/system", private=False)

        if self.options.enable_debug_layers:
            self.requires.add("vulkan-validationlayers/1.3.236.0", private=False)

            self.requires.add("spirv-tools/1.3.236.0", private=False, override=True)
            self.requires.add("spirv-headers/1.3.236.0", private=False, override=True)

        if self.options.build_tests or self.options.build_graphics_tests:
            self.requires.add("gtest/1.10.0", private=True)

    def source(self):
        self.run("git clone git@github.com:ivanovnikita/graphics_engine.git")
        self.run("cd graphics_engine && git checkout %s" % self.version)
        self.run("cp -r graphics_engine/* . && rm -rf graphics_engine")

    def _configure_cmake(self):
        if self._cmake:
            return self._cmake

        self._cmake = CMake(self)

        self._cmake.definitions["GE_ENABLE_LTO"] = self.options.enable_lto
        self._cmake.definitions["GE_ENABLE_DEBUG_LAYERS"] = self.options.enable_debug_layers
        self._cmake.definitions["GE_BUILD_EXAMPLES"] = self.options.build_examples
        self._cmake.definitions["GE_BUILD_TESTS"] = self.options.build_tests
        self._cmake.definitions["GE_USE_LIBC++"] = self.settings.compiler == "clang" and self.settings.compiler.libcxx == "libc++"

        self._cmake.configure()
        return self._cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()
