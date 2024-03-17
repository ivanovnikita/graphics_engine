if (${GE_USE_LIBC++})
    set(settings_stdlib "-s:a=compiler.libcxx=libc++")
endif()

if (${GE_ENABLE_LTO})
    set(option_lto "-o:a=enable_lto=True;")
endif()

if (${GE_ENABLE_DEBUG_LAYERS})
    set(option_debug_layers "-o:a=enable_debug_layers=True;")
endif()

if (${GE_BUILD_TESTS})
    set(option_tests "-o:a=build_tests=True;")
endif()

set(CONAN_INSTALL_ARGS
    "--build=missing;"
    "${settings_stdlib}"
    "${option_lto}"
    "${option_debug_layers}"
    "${option_tests}"
    "-s:a=compiler.cppstd=20"
)

find_package(glm REQUIRED)
find_package(VulkanHeaders REQUIRED)
find_package(Vulkan REQUIRED)
find_package(stb REQUIRED)
find_package(tinyobjloader REQUIRED)

if (${GE_ENABLE_DEBUG_LAYERS})
    find_package(vulkan-validationlayers REQUIRED)
endif()

if (${GE_BUILD_TESTS})
    find_package(GTest REQUIRED)
endif()
