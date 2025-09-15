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

# only add C interface when 'forte' is not an executable
get_target_property(FORTE_TARGET_TYPE forte TYPE)
if (FORTE_TARGET_TYPE STREQUAL "EXECUTABLE")
    return()
endif ()

add_subdirectory(include)
add_subdirectory(src)
