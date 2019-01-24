cmake_minimum_required(VERSION 3.9)

project(graphics_engine)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    option(GE_USE_LIBC++ "Use libc++?" ON)
    if (${GE_USE_LIBC++})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
        set(CONAN_STD_OPTION "CLANG_STD")
    endif()
endif()

include("third_party/cmake-conan/conan.cmake")

conan_cmake_run(CONANFILE conanfile.txt
                ${CONAN_STD_OPTION}
                BASIC_SETUP
                CMAKE_TARGETS
                BUILD missing)

option(GE_BUILD_TESTS "Build internal tests?" ON)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Install path prefix" FORCE)
endif()


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/ge)

if (${GE_BUILD_TESTS})
    enable_testing()
    add_subdirectory(test)
    add_subdirectory(examples)
endif()

# TODO: install targets (disable install by conan)
