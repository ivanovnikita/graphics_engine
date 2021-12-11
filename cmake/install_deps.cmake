if (${GE_ENABLE_LTO})
    list(APPEND conan_options enable_lto=True)
endif()

if (${GE_ENABLE_DEBUG_LAYERS})
    list(APPEND conan_options enable_debug_layers=True)
endif()

if (${GE_BUILD_TESTS})
    list(APPEND conan_options build_tests=True)
endif()

if(NOT CONAN_EXPORTED)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/third_party/cmake-conan")
    include(conan)
    conan_cmake_autodetect(settings)
    list(APPEND settings compiler.cppstd=20)
    conan_cmake_install(
        PATH_OR_REFERENCE ${CMAKE_SOURCE_DIR}
        BUILD missing
        SETTINGS ${settings}
        OPTIONS ${conan_options}
    )
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)
