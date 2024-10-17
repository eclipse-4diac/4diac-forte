#/************************************************************************************
# Copyright (c) 2023 - 2024 KT Elektronik GmbH, Jose Cabral
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#  Dirk Kaar - initial API and implementation and/or initial documentation
# ************************************************************************************/

SET(SOURCE_GROUP ${SOURCE_GROUP}\\zephyr)

forte_add_architecture(Zephyr)

if("${FORTE_ARCHITECTURE}" STREQUAL "Zephyr")

  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

  set(FORTE_LOGLEVEL "NOLOG" CACHE STRING "Loglevel to use" FORCE)
  SET(FORTE_ZEPHYR_INCLUDES "" CACHE STRING "Folders to include from the Zephyr OS")

  forte_add_sourcefile_hcpp(forte_thread forte_sync forte_sem)

  forte_add_timerhandler(CZephyrTimerHandler zephyrtiha )
  forte_add_sourcefile_hcpp(zephyrtiha)

  forte_add_sourcefile_cpp(./forte_architecture_time.cpp)

  forte_add_sourcefile_cpp(../common/forte_specific_architecture.cpp)
  forte_add_sourcefile_cpp(../genforte_printer.cpp)
  forte_add_sourcefile_cpp(zephyrforte_fileio.cpp)
  forte_add_sourcefile_cpp(../genforte_realFunctions.cpp)

  forte_add_definition("-DFORTE_LITTLE_ENDIAN")

  forte_add_include_system_directories(${FORTE_ZEPHYR_INCLUDES})

  if(FORTE_COM_ETH)
    forte_add_handler(CFDSelectHandler sockhand)
    forte_add_sourcefile_hcpp( ../fdselecthand ../bsdsocketinterf)
    forte_add_sourcefile_h(../gensockhand.h)
    forte_add_sourcefile_hcpp(sockhand)
  endif(FORTE_COM_ETH)

endif()
