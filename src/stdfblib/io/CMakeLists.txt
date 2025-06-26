#*******************************************************************************
# Copyright (c) 2024, 2025 Martin Erich Jobst, Jörg Walter
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Martin Jobst - initial API and implementation and/or initial documentation
# *******************************************************************************/

# these FBs are only built on-demand, if a module implements the old-style process interface
function(forte_set_process_interface name)
    if (ARGN)
        add_library(stdfblib-io OBJECT)
        foreach(fb ${ARGN})
            target_sources(stdfblib-io PRIVATE ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/${fb}_fbt.cpp)
        endforeach()

        target_include_directories(stdfblib-io PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
        target_include_directories(forte-global INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
        target_link_libraries(forte PRIVATE stdfblib-io)
    endif()
    message(STATUS "Process interface: ${name}")
endfunction()
