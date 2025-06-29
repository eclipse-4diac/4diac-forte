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

option(FORTE_TRACE_CTF "FORTE Common Trace Format (CTF) tracing" FALSE)

if (NOT FORTE_TRACE_CTF)
    return()
endif()

SET(FORTE_TRACE_CTF_BUFFER_SIZE 4096 CACHE STRING "FORTE Common Trace Format (CTF) trace buffer size")
option(FORTE_TRACE_CTF_REPLAY_DEBUGGING "FORTE Common Trace Format (CTF) tracing for replay debugging." OFF)

forte_add_definition("-DFORTE_TRACE_CTF")
forte_add_definition("-DFORTE_TRACE_CTF_BUFFER_SIZE=${FORTE_TRACE_CTF_BUFFER_SIZE}")

if (FORTE_TRACE_CTF_REPLAY_DEBUGGING)
    forte_add_definition("-DFORTE_TRACE_CTF_REPLAY_DEBUGGING")
    set(FORTE_TRACE_CONFIG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/replay_debugging/config.yaml)
    set(FORTE_TRACE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/replay_debugging/generated)
    forte_add_sourcefile_cpp(replay_debugging/trace_event_output.cpp)
else()
    set(FORTE_TRACE_CONFIG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/classic/config.yaml)
    set(FORTE_TRACE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/classic/generated)
    forte_add_sourcefile_cpp(classic/trace_event_output.cpp)
endif()
target_include_directories(forte-global INTERFACE ${FORTE_TRACE_FOLDER})

find_program(barectf_PROGRAM barectf)
add_custom_target(regenerate-barectf COMMAND ${barectf_PROGRAM} -c ${FORTE_TRACE_FOLDER} -H ${FORTE_TRACE_FOLDER} -m ${FORTE_TRACE_FOLDER} ${FORTE_TRACE_CONFIG_FILE})

option(FORTE_TRACE_CTF_ALWAYS_REGENERATE "Always regenerate CTF trace source and header files" OFF)
if (FORTE_TRACE_CTF_ALWAYS_REGENERATE)
    add_dependencies(forte-global regenerate-barectf)
endif()

forte_add_sourcefile_hc(${FORTE_TRACE_FOLDER}/barectf)
forte_add_sourcefile_h(${FORTE_TRACE_FOLDER}/barectf-bitfield.h)

forte_add_include_directories(${FORTE_TRACE_FOLDER})
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (FORTE_TRACE_CTF_TEST OR FORTE_REPLAY_DEVICE)
  add_subdirectory(internal)
  add_subdirectory(reader)
  forte_add_definition("-DFORTE_TRACER_INTERNAL")
endif()

forte_add_sourcefile_hcpp(barectf_platform_forte)
