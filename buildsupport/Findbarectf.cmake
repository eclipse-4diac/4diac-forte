#############################################################################
# Copyright (c) 2022 Martin Erich Jobst
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Martin Jobst
#      - initial API and implementation and/or initial documentation
#############################################################################

include(FindPackageHandleStandardArgs)

find_program(barectf_PROGRAM barectf)

find_package_handle_standard_args(barectf
    DEFAULT_MSG
    barectf_PROGRAM
)

if(barectf_FOUND)
    add_executable(barectf IMPORTED)
    set_target_properties(barectf PROPERTIES IMPORTED_LOCATION ${barectf_PROGRAM})
    set(barectf_PROGRAMS ${barectf_LIBRARY})
endif()

mark_as_advanced(barectf_PROGRAM)
