function(forte_register_handler class header)
    file(REAL_PATH ${header} header BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set_property(TARGET forte APPEND PROPERTY forte_handler "${header}://:${class}")
endfunction()

function(_forte_generate_core_handlers)
    get_property(registered TARGET forte PROPERTY forte_handler)

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

    message(STATUS "Event Handlers: CTimerHandler;${items}")
    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/deviceexecutionhandlers.cpp.in ${FORTE_GENERATED_FILES_DIR}/deviceexecutionhandlers.cpp)
    target_sources(forte-core PRIVATE ${FORTE_GENERATED_FILES_DIR}/deviceexecutionhandlers.cpp)
endfunction()

cmake_language(DEFER DIRECTORY ../../.. CALL _forte_generate_core_handlers)
