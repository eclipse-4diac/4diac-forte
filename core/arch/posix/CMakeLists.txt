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

forte_register_architecture(Posix Linux Darwin)

include(CheckLinkerFlag)

target_link_libraries(forte-core PUBLIC
        pthread
        $<$<NOT:$<BOOL:${APPLE}>>:rt>
        $<$<BOOL:${BUILD_SHARED_LIBS}>:dl>
)

set(FORTE_POSIX_GENERATE_MAP_FILE FALSE CACHE BOOL "Enable the generation of map files")
mark_as_advanced(FORTE_POSIX_GENERATE_MAP_FILE)
target_link_options(forte-core PUBLIC
        $<$<BOOL:${FORTE_POSIX_GENERATE_MAP_FILE}>:-Wl,-M,-Map,forte.map,-cref>
)

check_linker_flag(CXX LINKER:--no-as-needed has_no_as_needed)
if (has_no_as_needed)
    # Ubuntu/Debian have '--as-needed' as default. Restore default behavior of including "unused" shared library dependencies.
    target_link_options(forte-core PUBLIC LINKER:--no-as-needed)
endif ()

add_subdirectory(include)
add_subdirectory(src)
