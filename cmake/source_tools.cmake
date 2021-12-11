function(sources_in_dir_recurse directory result)
    file(GLOB_RECURSE output
        "${directory}/*.cpp"
        "${directory}/*.h"
        "${directory}/*.hpp"
    )
    set(${result} ${output} PARENT_SCOPE)
endfunction()

function(define_source_group target)
    get_target_property(src ${target} SOURCES)
    get_target_property(name ${target} NAME)
    source_group(${name} FILES ${src})
endfunction()
