######### Nodesets functions ####################################################    

#Don't use this function. This is just a private one
FUNCTION(forte_opcua_add_nodeset_general namespaceName directoryName)
  forte_add_sourcefile_with_path_hc(${directoryName}/ua_namespace_${namespaceName})
  set_property(GLOBAL APPEND PROPERTY FORTE_OPCUA_NODESETS_LIST ${namespaceName})
ENDFUNCTION()

#Use this function for nodesets which have a file called ua_types_NAMESPACE_generated.c file (DI for example). directoryName should point to the directory where the files are
FUNCTION(forte_opcua_add_nodeset namespaceName directoryName)
  forte_add_sourcefile_with_path_hc(${directoryName}/ua_types_${namespaceName}_generated)
  forte_opcua_add_nodeset_general(${namespaceName} ${directoryName})
ENDFUNCTION()
     
#Use this function for nodesets which DON'T have a file called ua_types_NAMESPACE_generated.c file (PLC for example). directoryName should point to the directory where the files are
FUNCTION(forte_opcua_add_nodeset_without_generated namespaceName directoryName)
  forte_opcua_add_nodeset_general(${namespaceName} ${directoryName})
  forte_add_include_directories(${directoryName})
ENDFUNCTION()

FUNCTION(forte_opcua_configure)
  forte_opcua_configure_nodesets()
  forte_opcua_configure_external_types()
ENDFUNCTION()

FUNCTION(forte_opcua_configure_nodesets)
  SET(FORTE_OPCUA_NODESETS_INCLUDE "")
  SET(FORTE_OPCUA_NODESETS_CODE "")

  GET_PROPERTY(FORTE_OPCUA_NODESETS GLOBAL PROPERTY FORTE_OPCUA_NODESETS_LIST)
  IF(NOT "${FORTE_OPCUA_NODESETS}" STREQUAL "")
    LIST(LENGTH FORTE_OPCUA_NODESETS FORTE_OPCUA_NODESETS_LEN)
    math(EXPR FORTE_OPCUA_NODESETS_LEN ${FORTE_OPCUA_NODESETS_LEN}-1)
    FOREACH(POS RANGE ${FORTE_OPCUA_NODESETS_LEN})
      LIST(GET FORTE_OPCUA_NODESETS ${POS} FORTE_OPCUA_NODESET)
      SET(FORTE_OPCUA_NODESETS_INCLUDE "${FORTE_OPCUA_NODESETS_INCLUDE} #include \"ua_namespace_${FORTE_OPCUA_NODESET}.h\"\n")
      SET(FORTE_OPCUA_NODESETS_CODE "${FORTE_OPCUA_NODESETS_CODE}  if(UA_STATUSCODE_GOOD != ua_namespace_${FORTE_OPCUA_NODESET}(&paUaServer)) return false;\n")
    ENDFOREACH(POS)
  ELSE()  
     SET(FORTE_OPCUA_NODESETS_CODE "(void)paUaServer;\n") #to silence warning
  ENDIF()
  
  set(FORTE_OPCUA_MODULE_DIR "src/com/opc_ua")
  
  CONFIGURE_FILE(${FORTE_OPCUA_MODULE_DIR}/opcua_nodesets.cpp.in ${CMAKE_BINARY_DIR}/com/opc_ua/opcua_nodesets_new.cpp)
  forte_replacefile_if_changed(${CMAKE_BINARY_DIR}/com/opc_ua/opcua_nodesets_new.cpp ${CMAKE_BINARY_DIR}/com/opc_ua/opcua_nodesets.cpp)
  file(REMOVE ${CMAKE_BINARY_DIR}/com/opc_ua/opcua_nodesets_new.cpp)
ENDFUNCTION()

FUNCTION(forte_opcua_configure_external_types)
  SET(FORTE_OPCUA_EXTERNAL_TYPES_INCLUDE "")
  SET(FORTE_OPCUA_EXTERNAL_TYPES_MAP_INSERT "")

   GET_PROPERTY(FORTE_OPCUA_EXTERNAL_TYPES_FILES GLOBAL PROPERTY FORTE_OPCUA_EXTERNAL_TYPES_FILES_LIST)
  
  IF(NOT "${FORTE_OPCUA_EXTERNAL_TYPES_FILES}" STREQUAL "")
    GET_PROPERTY(FORTE_OPCUA_EXTERNAL_TYPES_NAME GLOBAL PROPERTY FORTE_OPCUA_EXTERNAL_TYPES_NAME_LIST)
    GET_PROPERTY(FORTE_OPCUA_EXTERNAL_TYPES_OPCUA GLOBAL PROPERTY FORTE_OPCUA_EXTERNAL_TYPES_OPCUA_LIST)
    LIST(LENGTH FORTE_OPCUA_EXTERNAL_TYPES_FILES FORTE_OPCUA_TYPES_LEN)
    math(EXPR FORTE_OPCUA_TYPES_LEN ${FORTE_OPCUA_TYPES_LEN}-1)
    FOREACH(POS RANGE ${FORTE_OPCUA_TYPES_LEN})
      LIST(GET FORTE_OPCUA_EXTERNAL_TYPES_FILES ${POS} FORTE_OPCUA_EXTERNAL_TYPES_FILES_ITERATOR)
      LIST(GET FORTE_OPCUA_EXTERNAL_TYPES_NAME ${POS} FORTE_OPCUA_EXTERNAL_TYPES_NAME_ITERATOR)
      LIST(GET FORTE_OPCUA_EXTERNAL_TYPES_OPCUA ${POS} FORTE_OPCUA_EXTERNAL_TYPES_OPCUA_ITERATOR)

      SET(FORTE_OPCUA_EXTERNAL_TYPES_INCLUDE "${FORTE_OPCUA_EXTERNAL_TYPES_INCLUDE}#include \"${FORTE_OPCUA_EXTERNAL_TYPES_FILES_ITERATOR}.h\"\n")
      SET(FORTE_OPCUA_EXTERNAL_TYPES_MAP_INSERT "${FORTE_OPCUA_EXTERNAL_TYPES_MAP_INSERT}  UA_TypeConvert_external(&UA_TYPES[${FORTE_OPCUA_EXTERNAL_TYPES_OPCUA_ITERATOR}], g_nStringId${FORTE_OPCUA_EXTERNAL_TYPES_NAME_ITERATOR}),\n")
    ENDFOREACH(POS)
  ELSE()  
     #SET(FORTE_OPCUA_NODESETS_CODE "(void)paUaServer;\n") #to silence warning
  ENDIF()
  
  set(FORTE_OPCUA_MODULE_DIR "src/com/opc_ua")
  
  CONFIGURE_FILE(${FORTE_OPCUA_MODULE_DIR}/opcua_types.cpp.in ${CMAKE_BINARY_DIR}/com/opc_ua/opcua_types_new.cpp)
  forte_replacefile_if_changed(${CMAKE_BINARY_DIR}/com/opc_ua/opcua_types_new.cpp ${CMAKE_BINARY_DIR}/com/opc_ua/opcua_types.cpp)
  file(REMOVE ${CMAKE_BINARY_DIR}/com/opc_ua/opcua_types_new.cpp)
ENDFUNCTION()

FUNCTION(forte_opcua_add_type fileName forteName opcuaType)
  set_property(GLOBAL APPEND PROPERTY FORTE_OPCUA_EXTERNAL_TYPES_FILES_LIST ${fileName})
  set_property(GLOBAL APPEND PROPERTY FORTE_OPCUA_EXTERNAL_TYPES_NAME_LIST ${forteName})
  set_property(GLOBAL APPEND PROPERTY FORTE_OPCUA_EXTERNAL_TYPES_OPCUA_LIST ${opcuaType})
ENDFUNCTION()
  
#################################################################################