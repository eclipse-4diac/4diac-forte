#*******************************************************************************
# * Copyright (c) 2015-2016 Florian Froschermeier <florian.froschermeier@tum.de>
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *    Florian Froschermeier
# *      - initial integration of OPC Unified Architecture into FORTE
# ******************************************************************************/

#############################################################################
# OPC Unified Architecture (UA) integration.
# For integration the open-source OPC-UA communication stack/library
# from (open62541 - http://open62541.org/) was used.
#############################################################################

# option to enable OPC UA with cmake
forte_add_network_layer(OPC_UA OFF "opc_ua" COPC_UA_Layer opcua_layer "Enable OPC UA communication layer")
forte_add_definition(-std=c++11)

# path to the build folder of the open62541 stack or the shared-object library (.so)
SET(FORTE_COM_OPC_UA_DIR "" CACHE PATH "ABSOLUTE path to OPC UA build folder or shared object library")

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
		forte_add_link_library( libopen62541.dll )
	    endif("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
	else(EXISTS ${FORTE_COM_OPC_UA_DIR})
		message("No OPC_UA directory given")
    endif(EXISTS ${FORTE_COM_OPC_UA_DIR})
endif(FORTE_COM_OPC_UA)


# CMake for IEC 61499 based OPC-UA publish-subscribe function blocks
set(OPCUA_FB_PATH "61499_FBs")
forte_add_sourcefile_hcpp(${OPCUA_FB_PATH}/AOPCUANodeID ${OPCUA_FB_PATH}/GEN_SUBSCRIBEOPCUA ${OPCUA_FB_PATH}/GEN_PUBLISHOPCUA ${OPCUA_FB_PATH}/OPCUA_NodeID)

# Set if the open62541 OPC-UA server WAS built with an external INIT namespace (predefined nodes defined in xml file),
# see open62541 documentation.
set(FORTE_COM_OPC_UA_ENABLE_INIT_NAMESPACE ON CACHE BOOL "Include the external init namespace the open62541 server was built with")
mark_as_advanced(FORTE_COM_OPC_UA_ENABLE_INIT_NAMESPACE)

if(FORTE_COM_OPC_UA_ENABLE_INIT_NAMESPACE)
  forte_add_definition("-DFORTE_COM_OPC_UA_ENABLE_INIT_NAMESPACE")
endif(FORTE_COM_OPC_UA_ENABLE_INIT_NAMESPACE)


#get_directory_property( DirDefs COMPILE_DEFINITIONS )
#message( "COMPILE_DEFINITIONS = ${DirDefs}" )


