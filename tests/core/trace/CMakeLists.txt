#*******************************************************************************
# Copyright (c) 2024 Jose Cabral
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Jose Cabral  - initial API and implementation and/or initial documentation
# *******************************************************************************/

# an extra CMake variable is added since test tracing requires extra libraries which
# need to be compiled and might be too cumbersome
SET(FORTE_TRACE_CTF_TEST OFF CACHE BOOL "Include CTF trace tests")
mark_as_advanced(FORTE_TRACE_CTF_TEST)
if(FORTE_TRACE_CTF_TEST)
  SET(SOURCE_GROUP ${SOURCE_GROUP}\\trace)

  forte_test_add_sourcefile_cpp(common.cpp)

  if(FORTE_TRACE_CTF_REPLAY_DEBUGGING)
    forte_test_add_sourcefile_cpp(replayAlgorithmTest.cpp)
    SET(METADATA_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../../../core/src/trace/replay_debugging/generated/metadata)
  else()
    # currently the replay debugging traces a little bit differently from the classic one so the test does not apply
    forte_test_add_sourcefile_cpp(ctfTracerTest.cpp) 
    SET(METADATA_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../../../core/src/trace/classic/generated/metadata)
  endif()

  SET(CTF_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/ctfOutput)
  SET(REFERENCE_SYSTEMS_FILE ${CMAKE_CURRENT_SOURCE_DIR}/referenceSystem.fboot)

  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/trace_test_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/trace_test_config.h)
  forte_test_add_inc_directories(${CMAKE_CURRENT_BINARY_DIR})

  # reference systems tests requires the extra function blocks to be compiled
  set(FORTE_REPLAY_ALGORITHM_REFERENCE_TESTS FALSE CACHE BOOL "Compile the replay algorithm tests that uses the reference systems")
  if(FORTE_REPLAY_ALGORITHM_REFERENCE_TESTS)
    forte_add_definition("-DFORTE_REPLAY_ALGORITHM_REFERENCE_TESTS")
  endif(FORTE_REPLAY_ALGORITHM_REFERENCE_TESTS)

endif(FORTE_TRACE_CTF_TEST)
