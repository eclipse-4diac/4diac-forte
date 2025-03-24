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


if(FORTE_TRACE_CTF)
  SET(SOURCE_GROUP ${SOURCE_GROUP}\\trace)

  forte_test_add_sourcefile_cpp(ctfTracerTest.cpp common.cpp replayAlgorithmTest.cpp)

  SET(CTF_OUTPUT_DIR ${CMAKE_BINARY_DIR}/ctfOutput)
  SET(METADATA_FILE ${CMAKE_BINARY_DIR}/src/core/trace/metadata)
  SET(REFERENCE_SYSTEMS_FILE ${CMAKE_CURRENT_SOURCE_DIR}/referenceSystem.fboot)

  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.cpp.in ${CMAKE_BINARY_DIR}/tests/core/trace/config.cpp)
  forte_test_add_sourcefile_with_path_cpp(${CMAKE_BINARY_DIR}/tests/core/trace/config.cpp) # created file

  # reference systems tests requires the extra function blocks to be compiled
  set(FORTE_REPLAY_ALGORITHM_REFERENCE_TESTS FALSE CACHE BOOL "Compile the replay algorithm tests that uses the reference systems")
  if(FORTE_REPLAY_ALGORITHM_REFERENCE_TESTS)
    forte_add_definition("-DFORTE_REPLAY_ALGORITHM_REFERENCE_TESTS")
  endif(FORTE_REPLAY_ALGORITHM_REFERENCE_TESTS)

endif(FORTE_TRACE_CTF)
