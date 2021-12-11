set(CMAKE_INSTALL_RPATH "$ORIGIN/../../lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)

if (${GE_ENABLE_LTO})
    include(CheckIPOSupported)
    check_ipo_supported()
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

    list(APPEND linker_flags "-fuse-ld=gold -fuse-linker-plugin")
endif()

list(APPEND CMAKE_SHARED_LINKER_FLAGS ${linker_flags})
list(APPEND CMAKE_EXE_LINKER_FLAGS ${linker_flags})

set(GE_TARGET_PROPERTIES
    LINKER_LANGUAGE CXX
    POSITION_INDEPENDENT_CODE ON
)
