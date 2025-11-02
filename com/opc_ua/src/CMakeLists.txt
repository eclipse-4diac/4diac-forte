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

configure_file(opcua_defaults.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/opcua_defaults.h
)

target_sources(forte-com-opc_ua PRIVATE
        opcua_local_handler.h
        opcua_local_handler.cpp
        opcua_nodesets.cpp
        opcua_helper.h
        opcua_helper.cpp
        opcua_remote_handler.h
        opcua_remote_handler.cpp
        opcua_types.cpp
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
        $<$<BOOL:${FORTE_COM_OPC_UA}>:${CMAKE_CURRENT_SOURCE_DIR}/opcua_layer.cpp>
        $<$<BOOL:${FORTE_COM_OPC_UA}>:${CMAKE_CURRENT_SOURCE_DIR}/opcua_layer.h>
        $<$<BOOL:${FORTE_COM_OPC_UA_Events}>:${CMAKE_CURRENT_SOURCE_DIR}/opcua_event_layer.cpp>
        $<$<BOOL:${FORTE_COM_OPC_UA_Events}>:${CMAKE_CURRENT_SOURCE_DIR}/opcua_event_layer.h>
        $<$<BOOL:${FORTE_COM_OPC_UA_AC}>:${CMAKE_CURRENT_SOURCE_DIR}/opcua_ac_layer.cpp>
        $<$<BOOL:${FORTE_COM_OPC_UA_AC}>:${CMAKE_CURRENT_SOURCE_DIR}/opcua_ac_layer.h>
)
target_include_directories(forte-com-opc_ua PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(types)
add_subdirectory(detail)
