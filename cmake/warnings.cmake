function(get_warnings result)

    list(APPEND warnings
        -Werror
        -Wall
        -Wextra
        -pedantic
        -pedantic-errors
    )

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

        if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "4.7")
            list(APPEND warnings
                -Wfloat-equal
                -Wpointer-arith
                -Wcast-qual
                -Wconversion
                -Wsign-conversion
                -Wlogical-op
                -Wmissing-declarations
                -Wredundant-decls
            )
        endif()

        if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "7.0")
            list(APPEND warnings
                -Wduplicated-branches
                -Wduplicated-cond
            )
        endif()

    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

        list(APPEND warnings
            -Weverything
            -Wno-c++98-compat
            -Wno-c++98-compat-pedantic
            -Wno-c++11-compat
            -Wno-c++14-compat
            -Wno-unreachable-code
            -Wno-padded

            -Wno-unsafe-buffer-usage # not usable for `int main(int argc, char** argv)`

            -Wno-double-promotion

            # for vulkan
            -Wno-switch-enum
            -Wno-covered-switch-default
            -Wno-cast-function-type-strict

            # Or it is better to not ignore?
            -Wno-exit-time-destructors

            # for gtest
            -Wno-gnu-zero-variadic-macro-arguments
            -Wno-global-constructors
            -Wno-weak-vtables
        )

    endif()

    set(${result} ${warnings} PARENT_SCOPE)
endfunction()
