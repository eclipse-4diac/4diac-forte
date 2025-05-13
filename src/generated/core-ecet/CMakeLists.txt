function(forte_register_ecet class header) 
    file(REAL_PATH ${header} header BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set_property(TARGET forte-core APPEND PROPERTY forte_ecet "${header}://:${class}")
endfunction()

function(forte_set_default_ecet name)
    set_property(TARGET forte-core PROPERTY forte_ecet_default "${name}")
endfunction()

function(generate_core_ecet)
    get_property(registered TARGET forte-core PROPERTY forte_ecet)
    get_property(default TARGET forte-core PROPERTY forte_ecet_default)

    set(items)
    set(includes "")
    set(entries "")
    foreach (reg IN LISTS registered)
        string(REGEX REPLACE "(.*)://:(.*)" "\\1" header "${reg}")
        string(REGEX REPLACE "(.*)://:(.*)" "\\2" class "${reg}")

        list(APPEND items ${class})
        string(APPEND entries "ENTRY(${class})\n")
        string(APPEND includes "#include \"${header}\"\n")
    endforeach()

    if (NOT "${default}" IN_LIST items)
        message(FATAL_ERROR "${default} is not a valid ECET. This is a developer error. Available classes: ${items}")
    endif()

    message(STATUS "ECET Classes: ${default}, available: ${items}")
    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/ecetFactory.h.in ${FORTE_GENERATED_FILES_DIR}/ecetFactory.h)
    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/ecetFactory.cpp.in ${FORTE_GENERATED_FILES_DIR}/ecetFactory.cpp)
endfunction()

cmake_language(DEFER DIRECTORY ../.. CALL generate_core_ecet)
