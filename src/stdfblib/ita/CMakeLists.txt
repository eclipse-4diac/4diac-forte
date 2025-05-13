#*******************************************************************************
# Copyright (c) 2010, 2024 ACIN, Johannes Kepler University Linz
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Alois Zoitl
# *    - initial API and implementation and/or initial documentation
#    Markus Meingast
# *    - Add OPCUA_DEV and OPCUA_MGR to build configuration
# *******************************************************************************/

set(FORTE_DEVICE "RMT_DEV" CACHE STRING "Device to be used in this 4diac FORTE instance")
mark_as_advanced(FORTE_DEVICE)

add_library(forte-stdfblib-ita OBJECT
    CommandParser.cpp
    DEV_MGR.cpp
    EMB_RES.cpp
    RMT_DEV.cpp
    RMT_RES.cpp
    ${FORTE_DEVICE}.cpp
    $<$<BOOL:${FORTE_SUPPORT_BOOT_FILE}>:ForteBootFileLoader.cpp>
    $<$<BOOL:${FORTE_COM_OPC_UA}>:Config_EMB_RES.cpp>
)

target_link_libraries(forte PRIVATE forte-stdfblib-ita)

forte_register_device(RMT_DEV)

if (FORTE_COM_OPC_UA)
  forte_register_device(OPCUA_DEV)
endif()

if (FORTE_FAKE_TIME)
  forte_register_device(FakeTimeDev)
endif()
