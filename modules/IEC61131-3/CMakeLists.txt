#############################################################################
# Copyright (c) 2025 Martin Erich Jobst
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Martin Erich Jobst
#      - initial API and implementation and/or initial documentation
#############################################################################

option(FORTE_MODULE_IEC61131 "IEC61131-3 Function blocks" OFF)

if (NOT FORTE_MODULE_IEC61131)
    return()
endif ()

add_library(forte-iec61131-3)
target_link_libraries(forte-iec61131-3 PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-iec61131-3,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-iec61131-3>>)

add_subdirectory(include)
add_subdirectory(src)

install(TARGETS forte-iec61131-3 EXPORT forte-export FILE_SET HEADERS)
