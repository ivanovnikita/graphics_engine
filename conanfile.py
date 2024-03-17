from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy
from conan.tools.scm import Git
import os

required_conan_version = ">=2.1.0"


class GraphicsEngineConan(ConanFile):
    name = "GraphicsEngine"
    version = "0.2.0"
    settings = "os", "compiler", "build_type", "arch"

    package_type = "application"

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
        "enable_debug_layers": False,
        "build_examples": False,
        "build_tests": False,
    }

    generators = "CMakeDeps", "CMakeToolchain"

    def configure(self):
        if self.settings.os != "Linux":
            raise ConanInvalidConfiguration("GraphicsEngine is only supported for Linux")

    def layout(self):
        cmake_layout(self, src_folder="src")

    #def build_requirements(self):
        #self.build_requires("shaderc/2021.1")

    def requirements(self):
        #self.requires("shaderc/2021.1")

        self.requires("vulkan-headers/1.3.236.0")
        self.requires("vulkan-loader/1.3.236.0")
        self.requires("glm/0.9.9.7")

        self.requires("stb/cci.20230920")
        self.requires("tinyobjloader/2.0.0-rc10")

        if self.settings.os == "Linux":
            self.requires("xorg/system")

        if self.options.enable_debug_layers:
            self.requires("vulkan-validationlayers/1.3.236.0")

            self.requires("spirv-tools/1.3.236.0", override=True)
            self.requires("spirv-headers/1.3.236.0", override=True)

        if self.options.build_tests or self.options.build_graphics_tests:
            self.requires("gtest/1.10.0")