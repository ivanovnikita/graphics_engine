from conans import ConanFile
from conans.errors import ConanInvalidConfiguration


class GraphicsEngineConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "enable_lto": [True, False],
        "enable_debug_layers": [True, False],
        "build_tests": [True, False],
        "build_graphics_tests": [True, False],
    }
    default_options = {
        "shared": False,
        "enable_lto": False,
        "enable_debug_layers": False,
        "build_tests": False,
        "build_graphics_tests": False
    }
    generators = "cmake"

    def configure(self):
        if self.settings.os != "Linux":
            raise ConanInvalidConfiguration("GraphicsEngine is only supported for Linux")

    def requirements(self):
        self.requires.add("vulkan-headers/1.2.154.0 ", private=False)
        self.requires.add("vulkan-loader/1.2.154.0", private=False)
        self.requires.add("glm/0.9.9.7", private=False)

        self.requires.add("shaderc/2019.0", private=True)

        if self.settings.os == "Linux":
            self.requires.add("xorg/system", private=False)

        if self.options.enable_debug_layers:
            self.requires.add("vulkan-validationlayers/1.2.154.0", private=False)

        if self.options.build_tests or self.options.build_graphics_tests:
            self.requires.add("gtest/1.10.0", private=True)
