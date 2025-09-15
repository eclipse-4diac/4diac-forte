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

if (NOT FORTE_IO)
    return()
endif ()

# these FBs are only built on-demand, if a module implements the old-style process interface
function(forte_set_process_interface name)
    add_library(forte-io)
    target_link_libraries(forte-io PUBLIC forte-core)
    target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-io,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-io>>)
    target_sources(forte-io PUBLIC FILE_SET HEADERS FILES ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/include/forte/eclipse4diac/io/processinterfacebase.h)
    foreach (fb ${ARGN})
        target_sources(forte-io PRIVATE ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/src/${fb}_fbt.cpp)
        target_sources(forte-io PUBLIC FILE_SET HEADERS FILES ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/include/forte/eclipse4diac/io/${fb}_fbt.h)
    endforeach ()
    install(TARGETS forte-io EXPORT forte-export FILE_SET HEADERS)
    message(STATUS "Process interface: ${name}")
endfunction()
