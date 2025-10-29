# *******************************************************************************
# Copyright (c) 2018, fortiss GmbH This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Jose Cabral - initial implementation
# *******************************************************************************

# ############################################################################
# Arrowhead specific functional blocks
# ############################################################################

option(FORTE_MODULE_Arrowhead "Arrowhead Function Blocks" OFF)

if (NOT FORTE_MODULE_Arrowhead)
    return()
endif ()

message(FATAL_ERROR "This module has not yet been ported to 4diac FORTE 3.0")

add_library(forte-arrowhead)
target_link_libraries(forte-arrowhead PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-arrowhead,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-arrowhead>>)

add_subdirectory(common)
add_subdirectory(orchestrator)
add_subdirectory(serviceRegistry)
add_subdirectory(eventHandler)

install(TARGETS forte-arrowhead EXPORT forte-export FILE_SET HEADERS)
