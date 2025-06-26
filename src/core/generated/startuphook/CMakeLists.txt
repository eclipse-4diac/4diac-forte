function(forte_register_startup_hook funcname)
    file(REAL_PATH ${header} header BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set_property(TARGET forte APPEND PROPERTY forte_startuphook "${funcname}")
endfunction()

function(_forte_generate_startuphook)
    get_property(registered TARGET forte PROPERTY forte_startuphook)

    set(items)
    set(entries "")
    foreach (reg IN LISTS registered)
        string(REGEX REPLACE "(.*)://:(.*)" "\\1" header "${reg}")
        string(REGEX REPLACE "(.*)://:(.*)" "\\2" class "${reg}")

        list(APPEND items ${class})
        string(APPEND entries "ENTRY(${class})\n")
    endforeach()

    message(STATUS "Startup Hooks: ${items}")
    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/startuphook.cpp.in ${FORTE_GENERATED_FILES_DIR}/startuphook.cpp)
    target_sources(forte-core PRIVATE ${FORTE_GENERATED_FILES_DIR}/startuphook.cpp)
endfunction()

cmake_language(DEFER DIRECTORY ../../.. CALL _forte_generate_startuphook)
