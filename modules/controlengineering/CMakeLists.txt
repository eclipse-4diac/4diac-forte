###############################################################################
 # Copyright (c) 2026 Thomas Mayr
 # This program and the accompanying materials are made available under the
 # terms of the Eclipse Public License 2.0 which is available at
 # http://www.eclipse.org/legal/epl-2.0.
 #
 # SPDX-License-Identifier: EPL-2.0
 #
 # Contributors:
 #   Thomas Mayr - Initial implementation.
###############################################################################

option(FORTE_MODULE_CONTROL "Control Engineering Function Blocks" OFF)

if (NOT FORTE_MODULE_CONTROL)
    return()
endif ()

add_library(forte-controlengineering)
target_link_libraries(forte-controlengineering PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-controlengineering,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-controlengineering>>)

add_subdirectory(include)
add_subdirectory(src)

install(TARGETS forte-controlengineering EXPORT forte-export FILE_SET HEADERS)
