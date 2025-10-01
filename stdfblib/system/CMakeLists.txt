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

if (FORTE_COM_OPC_UA)
    option(FORTE_ENABLE_OPCUA_DEV "Compile the OPCUA Device" TRUE)
endif ()

option(FORTE_SUPPORT_BOOT_FILE "Enable 4diac FORTE boot file" ON)
mark_as_advanced(FORTE_SUPPORT_BOOT_FILE)

set(FORTE_BOOT_FILE_LOCATION
        "forte.fboot"
        CACHE STRING "Path to boot file"
)
mark_as_advanced(FORTE_BOOT_FILE_LOCATION)

add_library(forte-system)
target_compile_definitions(forte-system PRIVATE $<$<BOOL:${FORTE_ENABLE_OPCUA_DEV}>:FORTE_OPCUA_DEVICE>)
target_link_libraries(forte-system PUBLIC forte-core forte-events)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-system,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-system>>)

if (FORTE_COM_OPC_UA)
    target_link_libraries(forte-system PRIVATE forte-com-opc_ua)
endif ()

add_subdirectory(src)

install(TARGETS forte-system EXPORT forte-export FILE_SET HEADERS)
