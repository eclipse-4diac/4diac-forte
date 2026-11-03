#############################################################################
# Copyright (c) 2026 Monika Wenger
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#############################################################################

option(FORTE_MODULE_OPCUA "OPC UA helper Function Blocks" OFF)

if (NOT FORTE_MODULE_OPCUA)
    return()
endif ()

add_library(forte-opcua)
target_link_libraries(forte-opcua PUBLIC forte-core forte-iec61131-3)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-opcua,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-opcua>>)

add_subdirectory(include)
add_subdirectory(src)

install(TARGETS forte-opcua EXPORT forte-export FILE_SET HEADERS)
