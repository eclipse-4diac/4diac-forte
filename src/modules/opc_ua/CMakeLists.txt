#*******************************************************************************
# * Copyright (c) 2015-2016 Florian Froschermeier <florian.froschermeier@tum.de>,
# *                         fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *    Florian Froschermeier
# *      - initial integration of OPC Unified Architecture into FORTE
# *    Stefan Profanter
# *      - refactoring and adaption to new concept
# ******************************************************************************/

#############################################################################
# OPC Unified Architecture (UA) integration.
# For integration the open-source OPC-UA communication stack/library
# from (open62541 - http://open62541.org/) was used.
#############################################################################

# option to enable OPC UA with cmake
forte_add_network_layer(OPC_UA OFF "opc_ua" COPC_UA_Layer opcua_layer "Enable OPC UA communication layer")

# path to the build folder of the open62541 stack or the shared-object library (.so)
SET(FORTE_COM_OPC_UA_DIR "" CACHE PATH "ABSOLUTE path to OPC UA build folder or shared object library")

# Version of open62541 stack
set(FORTE_COM_OPC_UA_VERSION "0.2" CACHE STRING "Version of the open62541 stack")
# The string must not contain spaces, valid chars: [a-Z], underscore, point
set_property(CACHE FORTE_COM_OPC_UA_VERSION PROPERTY STRINGS 0.2 master)

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (FORTE_COM_OPC_UA)
    # OPEN62541 library to be linked to forte
    IF ("${FORTE_ARCHITECTURE}" STREQUAL "Posix")
        SET(FORTE_COM_OPC_UA_LIB "libopen62541.so" CACHE STRING "OPC_UA Library file in FORTE_COM_OPC_UA_DIR to be linked to forte (normally with name libopen62541 and a extension according to the type and OS)")
    ELSEIF ("${FORTE_ARCHITECTURE}" STREQUAL "Win32")
        SET(FORTE_COM_OPC_UA_LIB "open62541.lib" CACHE STRING "OPC_UA Library file in FORTE_COM_OPC_UA_DIR to be linked to forte (normally with name libopen62541 and a extension according to the type and OS)")
    ELSE ()
        SET(FORTE_COM_OPC_UA_LIB "libopen62541.a" CACHE STRING "OPC_UA Library file in FORTE_COM_OPC_UA_DIR to be linked to forte (normally with name libopen62541 and a extension according to the type and OS)")
    ENDIF ("${FORTE_ARCHITECTURE}" STREQUAL "Posix")

    string(REPLACE "." "_" FORTE_COM_OPC_UA_VERSION_CLEAN ${FORTE_COM_OPC_UA_VERSION})
    forte_add_definition(-DFORTE_COM_OPC_UA_VERSION_${FORTE_COM_OPC_UA_VERSION_CLEAN})
    forte_add_sourcefile_hcpp(opcua_layer opcua_handler opcua_helper)
    if (EXISTS ${FORTE_COM_OPC_UA_DIR})
        forte_add_include_directories(${FORTE_COM_OPC_UA_DIR})
        forte_add_link_directories(${FORTE_COM_OPC_UA_DIR})
        forte_add_link_library(${FORTE_COM_OPC_UA_LIB})
		file(TO_CMAKE_PATH "${FORTE_COM_OPC_UA_DIR}/${FORTE_COM_OPC_UA_LIB}" FORTE_COM_OPC_UA_LIB_PATH)
		if (NOT EXISTS ${FORTE_COM_OPC_UA_LIB} AND NOT EXISTS ${FORTE_COM_OPC_UA_LIB_PATH})
			message(SEND_ERROR "OPC UA library not found '${FORTE_COM_OPC_UA_LIB}'. Check the value of FORTE_COM_OPC_UA_DIR and optionally define an absolute path for FORTE_COM_OPC_UA_LIB.")
		endif()
    else (EXISTS ${FORTE_COM_OPC_UA_DIR})
        message("No OPC UA directory given")
    endif (EXISTS ${FORTE_COM_OPC_UA_DIR})
endif (FORTE_COM_OPC_UA)


