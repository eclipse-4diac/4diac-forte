function(forte_register_handler class header)
    file(REAL_PATH ${header} header BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set_property(TARGET forte-core APPEND PROPERTY forte_handler "${header}://:${class}")
endfunction()

function(generate_core_handlers)
    get_property(registered TARGET forte-core PROPERTY forte_handler)

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
    get_property(handlers TARGET forte-core PROPERTY forte_handler)

    message(STATUS "Event Handlers: CTimerHandler;${items}")
    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/deviceExecutionHandlers.cpp.in ${FORTE_GENERATED_FILES_DIR}/deviceExecutionHandlers.cpp)
endfunction()

cmake_language(DEFER DIRECTORY ../.. CALL generate_core_handlers)
