#*******************************************************************************
# Copyright (c) 2010 - 2024 ACIN, Profactor GmbH, fortiss GmbH, Jose Cabral
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#   Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny, Martin Melik Merkumians
# *      - initial API and implementation and/or initial documentation
#   Christoph Binder - add possibility to configure simulated time
# *******************************************************************************/

SET(SOURCE_GROUP ${SOURCE_GROUP}\\posix)

forte_add_architecture(Posix)

if("${FORTE_ARCHITECTURE}" STREQUAL "Posix")

  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
  forte_add_timerhandler(CPCTimerHandler pctimeha)
  forte_add_sourcefile_cpp(forte_architecture_time.cpp ../forte_standard_time.cpp)

  forte_add_sourcefile_hcpp(forte_thread forte_sync forte_sem pctimeha)
  forte_add_sourcefile_cpp(../genforte_printer.cpp)
  forte_add_sourcefile_cpp(../genforte_fileio.cpp)
  forte_add_sourcefile_h(../forte_architecture_time.h)
  forte_add_sourcefile_hcpp(../utils/timespec_utils)
  forte_add_sourcefile_cpp(../genforte_realFunctions.cpp)
  forte_add_sourcefile_cpp(../common/forte_specific_architecture.cpp)
  forte_add_to_executable_cpp(../main)

  if(FORTE_COM_ETH)
   forte_add_handler(CFDSelectHandler sockhand)
   forte_add_sourcefile_hcpp( ../fdselecthand ../bsdsocketinterf)
   forte_add_sourcefile_h(../gensockhand.h)
   forte_add_sourcefile_h(sockhand.h)
  endif(FORTE_COM_ETH)

  #forte_add_link_library(pthread)

  forte_add_definition("-W -Wall -Wextra -Woverloaded-virtual -Wconversion -Wshadow -DFORTE_LITTLE_ENDIAN -DFORTE_USE_DEFAULT_NEW_AND_DELETE")

  set(FORTE_POSIX_GENERATE_MAP_FILE FALSE CACHE BOOL "Enable the generation of map files")
  mark_as_advanced(FORTE_POSIX_GENERATE_MAP_FILE)
  IF(FORTE_POSIX_GENERATE_MAP_FILE)
    forte_add_link_flags("-Wl,-M -Wl,-Map -Wl,forte.map -Wl,-cref")
  ENDIF(FORTE_POSIX_GENERATE_MAP_FILE)  

  forte_add_link_library("-lpthread") 
  if (NOT APPLE)
    forte_add_link_library("-lrt") 
  endif()

  forte_add_network_layer(SER OFF "ser" CPosixSerCommLayer posixsercommlayer "Enable Forte serial line communication")

  set(FORTE_RTTI_AND_EXCEPTIONS FALSE CACHE BOOL "Enable RTTI and Exceptions")
  mark_as_advanced(FORTE_RTTI_AND_EXCEPTIONS)

  if(FORTE_TESTS AND FORTE_USE_TEST_CONFIG_IN_FORTE)
    if(FORTE_TEST_CODE_COVERAGE_ANALYSIS)
      forte_add_definition("-g -O0 --coverage -fno-inline")
      forte_add_link_library("--coverage")
    else()
      forte_add_definition("-g -O0 -fno-inline")
    endif()
    if(FORTE_TEST_SANITIZE)
      forte_add_definition("-fsanitize=address")
      forte_add_link_library("-fsanitize=address")
    endif()
  else()
    IF(NOT FORTE_RTTI_AND_EXCEPTIONS)
      forte_add_definition("-fno-rtti -fno-exceptions")
    ENDIF(NOT FORTE_RTTI_AND_EXCEPTIONS)
  endif(FORTE_TESTS AND FORTE_USE_TEST_CONFIG_IN_FORTE)
endif()
