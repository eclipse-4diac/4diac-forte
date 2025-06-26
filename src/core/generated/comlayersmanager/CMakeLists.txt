function(forte_register_com_layer varname defaultvalue paramname class header docstring)
    option(FORTE_COM_${varname} "${docstring}" ${defaultvalue})

    file(REAL_PATH ${header} header BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    if (FORTE_COM_${varname})
        set_property(TARGET forte APPEND PROPERTY forte_com_handler "${header}://:${class}://:${paramname}")
        target_compile_definitions(forte-global INTERFACE FORTE_COM_${varname})
    endif()
endfunction()

function(_forte_generate_core_cominfra)
    get_property(handlers TARGET forte PROPERTY forte_com_handler)

    set(status "")
    set(includes "")
    set(instances "")
    foreach (handler IN LISTS handlers)
        string(REGEX REPLACE "(.*)://:(.*)://:(.*)" "\\1" header "${handler}")
        string(REGEX REPLACE "(.*)://:(.*)://:(.*)" "\\2" class "${handler}")
        string(REGEX REPLACE "(.*)://:(.*)://:(.*)" "\\3" name "${handler}")

        string(APPEND status " ${name}")
        string(APPEND includes "#include \"${header}\"\n")
        string(APPEND instances "  else if (!strcmp(\"${name}\", paLayerIdentifier)) mNewLayer = new ${class}(paUpperLayer, paComFB);\n")
    endforeach()

    message(STATUS "COM Layers:${status}")
    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/comlayersmanager.cpp.in ${FORTE_GENERATED_FILES_DIR}/comlayersmanager.cpp)
    target_sources(forte-core PRIVATE ${FORTE_GENERATED_FILES_DIR}/comlayersmanager.cpp)
endfunction()

cmake_language(DEFER DIRECTORY ../../.. CALL _forte_generate_core_cominfra)
