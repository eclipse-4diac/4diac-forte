function(forte_register_timerhandler class header) 
    file(REAL_PATH ${header} header BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set_property(TARGET forte-arch APPEND PROPERTY forte_timerhandler "${header}://:${class}")
endfunction()

function(forte_set_default_timerhandler name)
    set_property(TARGET forte-arch PROPERTY forte_timerhandler_default "${name}")
endfunction()

function(generate_arch_timerhandlers)
    get_property(registered TARGET forte-arch PROPERTY forte_timerhandler)
    get_property(default TARGET forte-arch PROPERTY forte_timerhandler_default)

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
        message(FATAL_ERROR "${default} is not a valid timer handler. This is a developer error. Available handlers: ${items}")
    endif()

    message(STATUS "Timer Handler: ${default}, available: ${items}")
    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/timerHandlerFactory.h.in ${FORTE_GENERATED_FILES_DIR}/timerHandlerFactory.h)
    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/timerHandlerFactory.cpp.in ${FORTE_GENERATED_FILES_DIR}/timerHandlerFactory.cpp)
endfunction()

cmake_language(DEFER DIRECTORY ../.. CALL generate_arch_timerhandlers)
