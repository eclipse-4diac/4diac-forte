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

option(FORTE_MODULE_RT_Events "Real-Time Event Funktion blocks" OFF)

if (NOT FORTE_MODULE_RT_Events)
    return()
endif ()

add_library(forte-rtevents)
target_link_libraries(forte-rtevents PUBLIC forte-core forte-events)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-rtevents,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-rtevents>>)

add_subdirectory(include)
add_subdirectory(src)

install(TARGETS forte-rtevents EXPORT forte-export FILE_SET HEADERS)
