# *******************************************************************************
# *******************************************************************************
# Copyright (c) 2025 Malte Grave, LIT CPS Johannes Kepler Universität,
#                                 github.com/gravemalte
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#     Malte Grave, Alexander Trojnin - initial API and implementation and/or initial documentation
# *******************************************************************************/
# *******************************************************************************/
#############################################################################
# CAN Com Layer (currently only usable on Linux)
#############################################################################

if (NOT LINUX)
    return()
endif ()

option(FORTE_COM_CAN "Enable the CAN Commuication Layer" OFF)

if(FORTE_COM_CAN)
  set(FORTE_COM_CAN_MODULE_CHECK ON CACHE BOOL "Checking for the presence of CAN Modules.")

  if(FORTE_COM_CAN_MODULE_CHECK)
    execute_process(
      COMMAND lsmod
      COMMAND grep -wE "^(can|can_raw|vcan|can_dev)"
      COMMAND awk "{print $1}"
      COMMAND tr "\n" " "
      OUTPUT_VARIABLE CAN_MODULES_FOUND
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    
    message(STATUS "Found CAN modules: ${CAN_MODULES_FOUND}")
    if(NOT "${CAN_MODULES_FOUND}" MATCHES "can")
        message(FATAL_ERROR "can module is missing!")
    endif(NOT "${CAN_MODULES_FOUND}" MATCHES "can")
    
    if(NOT "${CAN_MODULES_FOUND}" MATCHES "vcan")
        message(FATAL_ERROR "vcan module is missing!")
    endif(NOT "${CAN_MODULES_FOUND}" MATCHES "vcan")

    if(NOT "${CAN_MODULES_FOUND}" MATCHES "can_raw")
        message(FATAL_ERROR "can_raw module is missing!")
    endif(NOT "${CAN_MODULES_FOUND}" MATCHES "can_raw")

    if(NOT "${CAN_MODULES_FOUND}" MATCHES "can_dev")
        message(FATAL_ERROR "can_dev module is missing!")
    endif(NOT "${CAN_MODULES_FOUND}" MATCHES "can_dev")

  endif(FORTE_COM_CAN_MODULE_CHECK)


  add_library(forte-com-CAN
              CANLayer.cpp
              CANLayer.h
              CANHandler.cpp
              CANHandler.h
  )
  target_include_directories(forte-com-CAN PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
  target_link_libraries(forte-com-CAN PUBLIC forte-core)
  target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-com-CAN,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-com-CAN>>)

  install(TARGETS forte-com-CAN EXPORT forte-export FILE_SET HEADERS)
endif(FORTE_COM_CAN)
