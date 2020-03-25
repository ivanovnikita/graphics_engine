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

        self.options["SPIRV-Tools"].lto = self.options.lto
        self.options["glslang"].lto = self.options.lto
        self.options["shaderc"].lto = self.options.lto
        self.options["Vulkan-Loader"].lto = self.options.lto
        self.options["xau"].lto = self.options.lto
        self.options["xdmcp"].lto = self.options.lto
        self.options["xcb"].lto = self.options.lto
        self.options["xcb-util-wm"].lto = self.options.lto

        self.options["xcb"].shared = True
        self.options["xau"].shared = True
        self.options["xdmcp"].shared = True

    def requirements(self):
        self.requires.add("shaderc/f537926", private=False)
        self.requires.add("Vulkan-Loader/1.2.133", private=False)
        self.requires.add("glm/0.9.9.1@g-truc/stable", private=False)

        if self.options.with_xcb:
            self.requires.add("xcb/1.14", private=False)
            self.requires.add("xcb-util/0.4.0", private=False)
            self.requires.add("xcb-util-wm/0.4.1", private=False)

        if self.options.with_gtests:
            self.requires.add("gtest/1.10.0 ", private=True)

        if self.options.with_debug_layers:
            self.requires.add("Vulkan-ValidationLayers/1.2.133", private=False)

    def imports(self):
        self.copy("*xcb.so*", "lib", "lib")
        self.copy("*Xau.so*", "lib", "lib")
        self.copy("*Xdmcp.so*", "lib", "lib")