#############################################################################
# OPC_UA Protocol Com Layer
#############################################################################

forte_add_definition(-std=c++11)
#include(CheckCXXCompilerFlag)
#CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
#CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
#message("Checking compiler options")
#if(COMPILER_SUPPORTS_CXX11)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#    message("Setting c++11 flag")
#elseif(COMPILER_SUPPORTS_CXX0X)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
#    message("Setting c++0x flag")
#else()
#        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
#endif()

# Macro definition
# MACRO(forte_add_network_layer NAME ONOFF CONFIGNAME CLASSNAME FILENAME DISCRIPTION)
forte_add_network_layer(OPC_UA OFF "opc_ua" COPC_UA_Layer opcua_layer "Enable OPC_UA Com Layer")

SET(FORTE_COM_OPC_UA_DIR "" CACHE PATH "Path to the OPC_UA (open62541 library) build folder (leave empty for installed source code)")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})



if(FORTE_COM_OPC_UA)
	forte_add_sourcefile_hcpp( opcua_layer opcuahandler )
	if(EXISTS ${FORTE_COM_OPC_UA_DIR})
	    if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
    		forte_add_include_directories( ${FORTE_COM_OPC_UA_DIR})
    		forte_add_link_directories( ${FORTE_COM_OPC_UA_DIR})
	       	forte_add_link_library(libopen62541.so)
	    elseif("${FORTE_ARCHITECTURE}" STREQUAL "Win32")
    		forte_add_include_directories( ${FORTE_COM_OPC_UA_DIR})
    		forte_add_link_directories( ${FORTE_COM_OPC_UA_DIR})
	    endif("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
	else(EXISTS ${FORTE_COM_OPC_UA_DIR})
		message("No OPC_UA directory given")
    endif(EXISTS ${FORTE_COM_OPC_UA_DIR})
endif(FORTE_COM_OPC_UA)
