from conans import ConanFile, CMake
import os


class GraphicsEngineConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False]
        , "lto": [True, False]
        , "with_xcb": [True, False]
        , "with_debug_layers": [True, False]
        , "with_gtests": [True, False]
    }
    default_options = {
        "shared": False
        , "lto": False
        , "with_xcb": False
        , "with_debug_layers": False
        , "with_gtests": False
    }
    generators = "cmake"

    def configure(self):
        if self.settings.os != "Linux":
            raise ConanInvalidConfiguration("GraphicsEngine is only supported for Linux")

        self.options["Vulkan-Loader"].with_xcb = self.options.with_xcb
        self.options["Vulkan-ValidationLayers"].with_xcb = self.options.with_xcb

        self.options["googletest"].lto = self.options.lto
        self.options["SPIRV-Tools"].lto = self.options.lto
        self.options["glslang"].lto = self.options.lto
        self.options["shaderc"].lto = self.options.lto
        self.options["Vulkan-Loader"].lto = self.options.lto
        self.options["xau"].lto = self.options.lto
        self.options["xdmcp"].lto = self.options.lto
        self.options["xcb"].lto = self.options.lto

    def requirements(self):
        self.requires.add("shaderc/14ae0de@ivanovnikita/stable", private=False)
        self.requires.add("Vulkan-Loader/1.1.97@ivanovnikita/stable", private=False)

        if self.options.with_xcb:
            self.requires.add("xcb/1.13.1@ivanovnikita/stable", private=False)

        if self.options.with_gtests:
            self.requires.add("googletest/1.8@ivanovnikita/stable", private=True)

        if self.options.with_debug_layers:
            self.requires.add("Vulkan-ValidationLayers/1.1.97@ivanovnikita/stable", private=False)
