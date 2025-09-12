# ############################################################################
# Copyright (c) 2022 Martin Erich Jobst
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Martin Jobst - initial API and implementation and/or initial
# documentation
# ############################################################################

option(FORTE_TRACE_CTF "FORTE Common Trace Format (CTF) tracing" FALSE)

if (NOT FORTE_TRACE_CTF)
    return()
endif ()

set(FORTE_TRACE_CTF_BUFFER_SIZE
        4096
        CACHE STRING "FORTE Common Trace Format (CTF) trace buffer size"
)
option(FORTE_TRACE_CTF_REPLAY_DEBUGGING
        "FORTE Common Trace Format (CTF) tracing for replay debugging." OFF
)

target_compile_definitions(forte-core PUBLIC FORTE_TRACE_CTF)
target_compile_definitions(forte-core PUBLIC
        FORTE_TRACE_CTF_BUFFER_SIZE=${FORTE_TRACE_CTF_BUFFER_SIZE}
)

if (FORTE_TRACE_CTF_REPLAY_DEBUGGING)
    target_compile_definitions(forte-core PUBLIC
            FORTE_TRACE_CTF_REPLAY_DEBUGGING
    )
    set(FORTE_TRACE_CONFIG_FILE
            ${CMAKE_CURRENT_SOURCE_DIR}/replay_debugging/config.yaml
    )
    set(FORTE_TRACE_FOLDER
            ${CMAKE_CURRENT_SOURCE_DIR}/replay_debugging/generated
    )
else ()
    set(FORTE_TRACE_CONFIG_FILE
            ${CMAKE_CURRENT_SOURCE_DIR}/classic/config.yaml
    )
    set(FORTE_TRACE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/classic/generated)
endif ()
target_include_directories(forte-core PRIVATE ${FORTE_TRACE_FOLDER})

find_program(barectf_PROGRAM barectf)
add_custom_target(regenerate-barectf
        COMMAND ${barectf_PROGRAM} -c ${FORTE_TRACE_FOLDER} -H
        ${FORTE_TRACE_FOLDER} -m ${FORTE_TRACE_FOLDER}
        ${FORTE_TRACE_CONFIG_FILE}
)

option(FORTE_TRACE_CTF_ALWAYS_REGENERATE
        "Always regenerate CTF trace source and header files" OFF
)
if (FORTE_TRACE_CTF_ALWAYS_REGENERATE)
    add_dependencies(forte-core regenerate-barectf)
endif ()

if (FORTE_TRACE_CTF_TEST OR FORTE_REPLAY_DEVICE)
    add_subdirectory(internal)
    add_subdirectory(reader)
    target_compile_definitions(forte-core PUBLIC FORTE_TRACER_INTERNAL)
endif ()
target_sources(forte-core PRIVATE
            $<$<BOOL:${FORTE_TRACE_CTF_REPLAY_DEBUGGING}>:${CMAKE_CURRENT_SOURCE_DIR}/replay_debugging/trace_event_output.cpp>
            $<$<NOT:$<BOOL:${FORTE_TRACE_CTF_REPLAY_DEBUGGING}>>:${CMAKE_CURRENT_SOURCE_DIR}/classic/trace_event_output.cpp>
            ${FORTE_TRACE_FOLDER}/barectf.h
            ${FORTE_TRACE_FOLDER}/barectf.c
            ${FORTE_TRACE_FOLDER}/barectf-bitfield.h
            barectf_platform_forte.h
            barectf_platform_forte.cpp
)
target_include_directories(forte-core PUBLIC $<BUILD_LOCAL_INTERFACE:${FORTE_TRACE_FOLDER}>)
