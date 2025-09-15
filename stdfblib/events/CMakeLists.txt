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

add_library(forte-events)
target_link_libraries(forte-events PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-events,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-events>>)

add_subdirectory(include)
add_subdirectory(src)

install(TARGETS forte-events EXPORT forte-export FILE_SET HEADERS)
