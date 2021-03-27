from conans import ConanFile
from conans.errors import ConanInvalidConfiguration


class GraphicsEngineConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "with_xcb": [True, False],
        "with_debug_layers": [True, False],
        "with_gtests": [True, False]
    }
    default_options = {
        "shared": False,
        "with_xcb": False,
        "with_debug_layers": False,
        "with_gtests": False
    }
    generators = "cmake"

    def configure(self):
        if self.settings.os != "Linux":
            raise ConanInvalidConfiguration("GraphicsEngine is only supported for Linux")

    def requirements(self):
        self.requires.add("vulkan-loader/1.2.162.0", private=False)
        self.requires.add("shaderc/2019.0", private=False)
        self.requires.add("glm/0.9.9.7", private=False)

        if self.options.with_xcb:
            self.requires.add("xorg/system", private=False)

        if self.options.with_gtests:
            self.requires.add("gtest/1.10.0", private=True)
