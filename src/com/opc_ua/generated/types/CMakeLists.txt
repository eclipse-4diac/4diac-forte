function(forte_opcua_register_type source forte_typename ua_namespace)
    file(REAL_PATH ${source}.h source_h BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set_property(TARGET forte APPEND PROPERTY forte_opcua_type "${source_h}://:${forte_typename}://:${ua_namespace}")
endfunction()

function(_forte_generate_opcua_types)
    get_property(handlers TARGET forte PROPERTY forte_opcua_type)

    set(status "")
    set(includes "")
    set(entries "")
    foreach (handler IN LISTS handlers)
        string(REGEX REPLACE "(.*)://:(.*)://:(.*)" "\\1" source_h "${handler}")
        string(REGEX REPLACE "(.*)://:(.*)://:(.*)" "\\2" forte_typename "${handler}")
        string(REGEX REPLACE "(.*)://:(.*)://:(.*)" "\\3" ua_namespace "${handler}")

        string(APPEND status " ${forte_typename}")

        string(APPEND includes "USE_STRING_ID(${forte_typename})\n")
        string(APPEND includes "#include \"${source_h}\"\n")

        string(REGEX REPLACE "\\.h$" ".cpp" source_cpp "${source_h}")
        target_sources(forte-com-opc_ua PRIVATE "${source_cpp}")

        string(TOUPPER "${forte_typename}" ua_typename)
        string(TOUPPER "${ua_namespace}" ua_namespace)
        if (ua_namespace STREQUAL "UA")
	        string(APPEND entries "ENTRY(UA_TYPES,${ua_typename},${forte_typename})\n")

        else()
	        string(APPEND entries "ENTRY(UA_UA_TYPES_${ua_namespace},${ua_typename},${forte_typename})\n")
            string(TOLOWER "${ua_namespace}" ua_namespace_header)
	        string(APPEND includes "#include \"ua_types_${ua_namespace_header}_generated.h\"\n")
	    endif()
    endforeach()

    message(STATUS "OPC UA Types:${status}")
    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/opcua_types.h.in ${FORTE_GENERATED_FILES_DIR}/opcua_types.h)
endfunction()

cmake_language(DEFER DIRECTORY ../../../.. CALL _forte_generate_opcua_types)
