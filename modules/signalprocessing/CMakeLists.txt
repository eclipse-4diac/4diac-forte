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

option(FORTE_MODULE_SIGNALPROCESSING "SIGNALPROCESSING" OFF)

if (NOT FORTE_MODULE_SIGNALPROCESSING)
    return()
endif ()

add_library(forte-signalprocessing)
target_link_libraries(forte-signalprocessing PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-signalprocessing,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-signalprocessing>>)

add_subdirectory(include)
add_subdirectory(src)

install(TARGETS forte-signalprocessing EXPORT forte-export FILE_SET HEADERS)
