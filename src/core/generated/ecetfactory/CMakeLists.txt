function(forte_register_ecet class header) 
    file(REAL_PATH ${header} header BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set_property(TARGET forte APPEND PROPERTY forte_ecet "${header}://:${class}")
endfunction()

function(forte_set_default_ecet name)
    set_property(TARGET forte PROPERTY forte_ecet_default "${name}")
endfunction()

function(_forte_generate_core_ecet)
    get_property(registered TARGET forte PROPERTY forte_ecet)
    get_property(default TARGET forte PROPERTY forte_ecet_default)

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

    if (NOT DEFINED default)
        list(GET items 0 default)
    endif()

    if (NOT "${default}" IN_LIST items)
        message(FATAL_ERROR "${default} is not a valid ECET. This is a developer error. Available classes: ${items}")
    endif()

    message(STATUS "ECET Classes: ${default}, available: ${items}")
    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/ecetfactory.h.in ${FORTE_GENERATED_FILES_DIR}/ecetfactory.h)
    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/ecetfactory.cpp.in ${FORTE_GENERATED_FILES_DIR}/ecetfactory.cpp)
    target_sources(forte-core PRIVATE ${FORTE_GENERATED_FILES_DIR}/ecetfactory.cpp)
endfunction()

cmake_language(DEFER DIRECTORY ../../.. CALL _forte_generate_core_ecet)
