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

add_library(forte-core)
target_compile_features(forte-core PUBLIC cxx_std_20)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-core,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-core>>)

add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(arch)

install(TARGETS forte-core EXPORT forte-export FILE_SET HEADERS)
