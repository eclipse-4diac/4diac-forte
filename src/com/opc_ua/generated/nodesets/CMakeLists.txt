function(forte_opcua_register_nodeset ua_namespace source_dir)
    file(REAL_PATH "${source_dir}" source BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set_property(TARGET forte APPEND PROPERTY forte_opcua_nodeset "${source}://:${ua_namespace}")
endfunction()

function(_forte_generate_opcua_nodesets)
    get_property(handlers TARGET forte PROPERTY forte_opcua_nodeset)

    set(status "")
    set(includes "")
    set(entries "")
    foreach (handler IN LISTS handlers)
        string(REGEX REPLACE "(.*)://:(.*)" "\\1" source "${handler}")
        string(REGEX REPLACE "(.*)://:(.*)" "\\2" ua_namespace "${handler}")

        if (EXISTS "${source}/ua_types_${ua_namespace}_generated.h")
            string(APPEND includes "#include \"${source}/ua_types_${ua_namespace}_generated.h\"\n")
            target_sources(forte-com-opc_ua PRIVATE "${source}/ua_types_${ua_namespace}_generated.c")
        endif()

        string(APPEND includes "#include \"${source}/ua_namespace_${ua_namespace}.h\"\n")
        target_sources(forte-com-opc_ua PRIVATE "${source}/ua_namespace_${ua_namespace}.c")

        string(APPEND entries "ENTRY(${ua_namespace})\n")
    endforeach()

    message(STATUS "OPC UA Nodesets:${status}")
    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/opcua_nodesets.cpp.in ${FORTE_GENERATED_FILES_DIR}/opcua_nodesets.cpp)
    target_sources(forte-com-opc_ua PRIVATE ${FORTE_GENERATED_FILES_DIR}/opcua_nodesets.cpp)
endfunction()

cmake_language(DEFER DIRECTORY ../../../.. CALL _forte_generate_opcua_nodesets)
