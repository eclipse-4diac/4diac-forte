#*******************************************************************************
# Copyright (c) 2025 Jörg Walter
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Jörg Walter - initial API and implementation and/or initial documentation
# *******************************************************************************/

function(forte_set_process_interface name)
    add_library(stdfblib-io OBJECT)
    foreach(fb ${ARGN})
        target_sources(stdfblib-io PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/${fb}_fbt.cpp)
    endforeach()

    target_include_directories(stdfblib-io PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
    target_include_directories(forte-core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
    target_link_libraries(forte PRIVATE stdfblib-io)
endfunction()

function(report_process_interface)
    message(STATUS "Process interface: ${FORTE_PROCESS_INTERFACE}")
endfunction()

cmake_language(DEFER DIRECTORY ../.. CALL report_process_interface)
