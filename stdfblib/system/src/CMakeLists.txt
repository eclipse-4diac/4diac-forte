#############################################################################
# Copyright (c) 2010, 2025 ACIN, Johannes Kepler University Linz
#                          Martin Erich Jobst
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Alois Zoitl
#      - initial API and implementation and/or initial documentation
#    Markus Meingast
#      - Add OPCUA_DEV and OPCUA_MGR to build configuration
#    Martin Erich Jobst
#      - rework source file structure
#############################################################################

configure_file(ForteBootFileLoader_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/ForteBootFileLoader_config.h)

target_sources(forte-system PRIVATE
        DEV_MGR.h
        DEV_MGR.cpp
        EMB_RES.h
        EMB_RES.cpp
        RMT_DEV.h
        RMT_DEV.cpp
        RMT_RES.h
        RMT_RES.cpp
        CommandParser.h
        CommandParser.cpp
        ForteBootFileLoader.h
        $<$<BOOL:${FORTE_COM_OPC_UA}>:${CMAKE_CURRENT_SOURCE_DIR}/Config_EMB_RES.h>
        $<$<BOOL:${FORTE_COM_OPC_UA}>:${CMAKE_CURRENT_SOURCE_DIR}/Config_EMB_RES.cpp>
        $<$<BOOL:${FORTE_ENABLE_OPCUA_DEV}>:${CMAKE_CURRENT_SOURCE_DIR}/OPCUA_DEV.h>
        $<$<BOOL:${FORTE_ENABLE_OPCUA_DEV}>:${CMAKE_CURRENT_SOURCE_DIR}/OPCUA_DEV.cpp>
        $<$<BOOL:${FORTE_ENABLE_OPCUA_DEV}>:${CMAKE_CURRENT_SOURCE_DIR}/OPCUA_MGR.h>
        $<$<BOOL:${FORTE_ENABLE_OPCUA_DEV}>:${CMAKE_CURRENT_SOURCE_DIR}/OPCUA_MGR.cpp>
        $<$<BOOL:${FORTE_FAKE_TIME}>:${CMAKE_CURRENT_SOURCE_DIR}/FakeTimeDev.h>
        $<$<BOOL:${FORTE_FAKE_TIME}>:${CMAKE_CURRENT_SOURCE_DIR}/FakeTimeDev.cpp>
        $<$<BOOL:${FORTE_SUPPORT_BOOT_FILE}>:${CMAKE_CURRENT_SOURCE_DIR}/ForteBootFileLoader.h>
        $<$<BOOL:${FORTE_SUPPORT_BOOT_FILE}>:${CMAKE_CURRENT_SOURCE_DIR}/ForteBootFileLoader.cpp>
        ${CMAKE_CURRENT_BINARY_DIR}/ForteBootFileLoader_config.h
)

target_include_directories(forte-system PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(debug_device)
add_subdirectory(multi)
add_subdirectory(replay)
