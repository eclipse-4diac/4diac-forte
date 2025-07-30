# *******************************************************************************
# Copyright (c) 2010, 2024 ACIN, Johannes Kepler University Linz This program
# and the accompanying materials are made available under the terms of the
# Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Alois Zoitl *    - initial API and implementation and/or
# initial documentation Markus Meingast *    - Add OPCUA_DEV and OPCUA_MGR to
# build configuration
# *******************************************************************************/
# ############################################################################
# Ita FB
# ############################################################################

if (FORTE_COM_OPC_UA)
    option(FORTE_ENABLE_OPCUA_DEV "Compile the OPCUA Device" TRUE)
endif ()

if (FORTE_ENABLE_OPCUA_DEV)
    target_compile_definitions(forte-global INTERFACE FORTE_OPCUA_DEVICE)
endif ()

option(FORTE_SUPPORT_BOOT_FILE "Enable 4diac FORTE boot file" ON)
mark_as_advanced(FORTE_SUPPORT_BOOT_FILE)
forte_generate_config_flag_include(FORTE_SUPPORT_BOOT_FILE)

set(FORTE_BOOT_FILE_LOCATION
    "forte.fboot"
    CACHE STRING "Path to boot file"
)
mark_as_advanced(FORTE_BOOT_FILE_LOCATION)
forte_generate_config_string_include(FORTE_BOOT_FILE_LOCATION)
add_subdirectory(debug_device)
add_subdirectory(multi)
add_subdirectory(replay)
add_library(forte-stdfblib-ita OBJECT
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
            $<$<BOOL:${FORTE_COM_OPC_UA}>:Config_EMB_RES.h>
            $<$<BOOL:${FORTE_COM_OPC_UA}>:Config_EMB_RES.cpp>
            $<$<BOOL:${FORTE_ENABLE_OPCUA_DEV}>:OPCUA_DEV.h>
            $<$<BOOL:${FORTE_ENABLE_OPCUA_DEV}>:OPCUA_DEV.cpp>
            $<$<BOOL:${FORTE_ENABLE_OPCUA_DEV}>:OPCUA_MGR.h>
            $<$<BOOL:${FORTE_ENABLE_OPCUA_DEV}>:OPCUA_MGR.cpp>
            $<$<BOOL:${FORTE_FAKE_TIME}>:FakeTimeDev.h>
            $<$<BOOL:${FORTE_FAKE_TIME}>:FakeTimeDev.cpp>
            $<$<BOOL:${FORTE_SUPPORT_BOOT_FILE}>:ForteBootFileLoader.h>
            $<$<BOOL:${FORTE_SUPPORT_BOOT_FILE}>:ForteBootFileLoader.cpp>
)
target_link_libraries(forte PRIVATE forte-stdfblib-ita)
