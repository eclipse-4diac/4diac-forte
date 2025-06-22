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

forte_add_network_layer(CAN OFF "can" CCANComLayer CANLayer "Enable the CAN Commuication Layer")
set(FORTE_COM_CAN_MODULE_CHECK OFF CACHE BOOL "Build Tests")

if(FORTE_COM_CAN)
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

  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})  
  forte_add_sourcefile_hcpp(
    CANHandler
  )

  forte_add_handler(CCANHandler CANHandler)

endif(FORTE_COM_CAN)
