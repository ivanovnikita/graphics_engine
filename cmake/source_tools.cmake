function(sources_in_dir_recurse directory extension result)
    file(GLOB_RECURSE output
        "${directory}/*.${extension}"
    )
    set(${result} ${output} PARENT_SCOPE)
endfunction()

function(interfaces_in_dir_recurse directory result)
    sources_in_dir_recurse(${directory} "ixx" output)
    set(${result} ${output} PARENT_SCOPE)
endfunction()

function(implementations_in_dir_recurse directory result)
    sources_in_dir_recurse(${directory} "cxx" output)
    set(${result} ${output} PARENT_SCOPE)
endfunction()

function(headers_in_dir_recurse directory result)
    sources_in_dir_recurse(${directory} "h" output)
    set(${result} ${output} PARENT_SCOPE)
endfunction()

function(legacy_sources_in_dir_recurse directory result)
    sources_in_dir_recurse(${directory} "cpp" output)
    set(${result} ${output} PARENT_SCOPE)
endfunction()

function(define_source_group target)
    get_target_property(src ${target} SOURCES)
    get_target_property(name ${target} NAME)
    source_group(${name} FILES ${src})
endfunction()
