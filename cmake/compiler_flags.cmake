set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND cxx_flags -O3)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND cxx_flags -O0)
else()
    message(FATAL_ERROR "Unknown value of CMAKE_BUILD_TYPE!")
endif()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")

    list(APPEND cxx_flags -fconcepts-diagnostics-depth=2)
    list(APPEND std_libs -lstdc++fs)
    #list(APPEND cxx_flags -fanalyzer)

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")

    if (${GE_USE_LIBC++})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
        list(APPEND std_libs -lc++ -lc++abi -lc++experimental)
    endif()

endif()

include(warnings)

get_warnings(warnings)

set(GE_COMPILE_OPTIONS ${cxx_flags} ${warnings})
