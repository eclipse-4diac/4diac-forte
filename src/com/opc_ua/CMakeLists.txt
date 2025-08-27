# *******************************************************************************************

# Copyright (c) 2015-2016 Florian Froschermeier
# <florian.froschermeier@tum.de>, fortiss GmbH This program and the
# accompanying materials are made available under the terms of the Eclipse
# Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Florian Froschermeier - initial integration of OPC Unified
# Architecture into FORTE Stefan Profanter - refactoring and adaption to new
# concept
# ******************************************************************************************

# ############################################################################
# OPC Unified Architecture (UA) integration. For integration the open-source
# OPC-UA communication stack/library from (open62541 - http://open62541.org/)
# was used.
# ############################################################################

# option to enable OPC UA with cmake
option(FORTE_COM_OPC_UA "Enable OPC UA communication layer" OFF)
if (NOT FORTE_COM_OPC_UA)
    return()
endif ()

set(FORTE_COM_OPC_UA_PORT
        4840
        CACHE STRING "Port where the OPC_UA server will listen"
)
set(FORTE_COM_OPC_UA_SERVER_PUB_INTERVAL
        100.0
        CACHE STRING "OPC UA Server publishing interval in ms"
)
set(FORTE_COM_OPC_UA_CLIENT_PUB_INTERVAL
        100.0
        CACHE STRING
        "OPC UA Client requested publishing interval via OPC UA data subscription in ms"
)
set(FORTE_COM_OPC_UA_SERVER_MAX_ITERATION_INTERVAL
        10
        CACHE STRING
        "The maximum time in OPC UA server will wait to iterate again"
)

option(FORTE_COM_OPC_UA_MULTICAST
        "Enable multicast support for OPC UA and registering with LDS" OFF
)
set(FORTE_COM_OPC_UA_INCLUDE_DIR
        ""
        CACHE PATH "ABSOLUTE path to open62541.h file in the OPC UA project"
)
set(FORTE_COM_OPC_UA_LIB_DIR
        ""
        CACHE PATH
        "ABSOLUTE path to OPC UA folder with object library FORTE_COM_OPC_UA_LIB"
)
set(FORTE_COM_OPC_UA_CUSTOM_HOSTNAME
        CACHE STRING
        "Custom hostname which is used for the OPC UA app name and app uri"
)
option(FORTE_COM_OPC_UA_ENCRYPTION
        "The open62541 lilbrary was compiled using encryption" OFF
)

set(FORTE_COM_OPC_UA_LIB
        "open62541"
        CACHE STRING
        "OPC_UA Library file in FORTE_COM_OPC_UA_DIR to be linked to forte (normally with name libopen62541 and a extension according to the type and OS)"
)
if (FORTE_ARCHITECTURE STREQUAL "Win32")
    target_link_libraries(forte PUBLIC iphlpapi)
endif ()
target_include_directories(forte-global
        INTERFACE ${FORTE_COM_OPC_UA_INCLUDE_DIR}
)
target_link_libraries(forte PUBLIC ${FORTE_COM_OPC_UA_LIB})

# OPC UA Event Layer
option(FORTE_COM_OPC_UA_Events "Enable OPC UA Event Support" OFF)
option(FORTE_COM_OPC_UA_AC "Enable OPC UA A&C Support" OFF)
configure_file(opcua_defaults.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/opcua_defaults.h
)

if (FORTE_COM_OPC_UA_ENCRYPTION)
    set(FORTE_COM_OPC_UA_ENCRYPTION_INCLUDE_DIR
            ""
            CACHE STRING "ABSOLUTE path to encryption headers"
    )
    set(FORTE_COM_OPC_UA_ENCRYPTION_LIB_DIR
            ""
            CACHE STRING "ABSOLUTE path to encryption libraries"
    )

    option(FORTE_COM_OPC_UA_ENCRYPTION_MBEDTLS
            "Was open62541 built against mbedtls instead of openssl/libressl?"
            TRUE
    )

    if (FORTE_COM_OPC_UA_ENCRYPTION_MBEDTLS)
        target_link_libraries(forte PUBLIC mbedcrypto)
        target_link_libraries(forte PUBLIC mbedtls)
        target_link_libraries(forte PUBLIC mbedx509)
        if (FORTE_ARCHITECTURE STREQUAL "Win32")
            target_link_libraries(forte PUBLIC bcrypt)
        endif ()
    else ()
        target_link_libraries(forte PUBLIC ssl)
        target_link_libraries(forte PUBLIC crypto)
    endif ()
endif ()

add_library(forte-com-opc_ua OBJECT
        opcua_types.h
        opcua_nodesets.h
        opcua_local_handler.h
        opcua_local_handler.cpp
        opcua_helper.h
        opcua_helper.cpp
        opcua_remote_handler.h
        opcua_remote_handler.cpp
        opcua_handler_abstract.h
        opcua_handler_abstract.cpp
        opcua_client_information.h
        opcua_client_information.cpp
        opcua_action_info.h
        opcua_action_info.cpp
        opcua_objectstruct_helper.h
        opcua_objectstruct_helper.cpp
        opcua_client_config_parser.h
        opcua_client_config_parser.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/opcua_defaults.h
        $<$<BOOL:${FORTE_COM_OPC_UA}>:opcua_layer.cpp>
        $<$<BOOL:${FORTE_COM_OPC_UA}>:opcua_layer.h>
        $<$<BOOL:${FORTE_COM_OPC_UA_Events}>:opcua_event_layer.cpp>
        $<$<BOOL:${FORTE_COM_OPC_UA_Events}>:opcua_event_layer.h>
        $<$<BOOL:${FORTE_COM_OPC_UA_AC}>:opcua_ac_layer.cpp>
        $<$<BOOL:${FORTE_COM_OPC_UA_AC}>:opcua_ac_layer.h>
)
target_link_libraries(forte-com-opc_ua PUBLIC forte-core)
target_link_libraries(forte PRIVATE forte-com-opc_ua)
target_include_directories(forte-com-opc_ua
        PUBLIC ${FORTE_COM_OPC_UA_ENCRYPTION_INCLUDE_DIR}
)
target_include_directories(forte-com-opc_ua PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_directories(forte-com-opc_ua PUBLIC ${FORTE_COM_OPC_UA_LIB_DIR})
target_link_directories(forte-com-opc_ua PUBLIC
        ${FORTE_COM_OPC_UA_ENCRYPTION_LIB_DIR}
)

add_subdirectory(FBs)
add_subdirectory(types)
add_subdirectory(detail)
