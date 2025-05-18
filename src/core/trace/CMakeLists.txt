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

set(FORTE_TRACE_CONFIG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/classic/config.yaml)
set(FORTE_TRACE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/classic/generated)

SET(FORTE_GENERATE_TRACE_FILES FALSE CACHE BOOL "Generate the barectf trace files again for the selected trace configuration")
mark_as_advanced(FORTE_GENERATE_TRACE_FILES)

if(FORTE_GENERATE_TRACE_FILES)

  find_package(barectf REQUIRED)

  add_custom_command(
      OUTPUT ${FORTE_TRACE_FOLDER}/barectf.h ${FORTE_TRACE_FOLDER}/barectf.c ${FORTE_TRACE_FOLDER}/barectf-bitfield.h ${FORTE_TRACE_FOLDER}/metadata
      COMMAND barectf -c ${FORTE_TRACE_FOLDER} -H ${FORTE_TRACE_FOLDER} -m ${FORTE_TRACE_FOLDER} ${FORTE_TRACE_CONFIG_FILE}
      MAIN_DEPENDENCY ${FORTE_TRACE_CONFIG_FILE}
      DEPENDS barectf
      COMMENT "Generating tracer for Common Trace Format (CTF) with barectf"
  )

endif()

add_library(forte_trace STATIC
        ${FORTE_TRACE_FOLDER}/barectf.h
        ${FORTE_TRACE_FOLDER}/barectf.c
        ${FORTE_TRACE_FOLDER}/barectf-bitfield.h
)
target_include_directories(forte_trace
    PUBLIC
        $<BUILD_INTERFACE:${FORTE_TRACE_FOLDER}>
)

forte_add_link_library(forte_trace)
forte_add_include_directories(${FORTE_TRACE_FOLDER})
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

forte_add_sourcefile_hcpp(barectf_platform_forte)

if(FORTE_TRACE_CTF_TEST)
  forte_add_sourcefile_hcpp(EventMessage)
  forte_add_sourcefile_hcpp(internalTracer)
  forte_add_sourcefile_hcpp(flexibleTracer)

  set(FORTE_TRACER_INCLUDE flexibleTracer.h)
  set(FORTE_TRACER CFlexibleTracer)

else(FORTE_TRACE_CTF_TEST)
  set(FORTE_TRACER_INCLUDE barectf_platform_forte.h)
  set(FORTE_TRACER BarectfPlatformFORTE)
endif(FORTE_TRACE_CTF_TEST)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tracerConfig.h.in ${CMAKE_BINARY_DIR}/tracerConfig.h)
