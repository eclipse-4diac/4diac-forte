#*******************************************************************************
# Copyright (c) 2022-2023 HR Agrartechnik GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#     Franz Hoepfinger - initial API and implementation and/or initial documentation
# *******************************************************************************/ 

#############################################################################
# FORTE_MODULE_ESP32  integration.
#############################################################################

# option to enable OPC UA with cmake
forte_add_module(ESP32 OFF "Interacting with GPIOs of esp32 devices via gpio")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (FORTE_MODULE_ESP32)
  forte_add_custom_configuration("#cmakedefine FORTE_MODULE_ESP32")

  set(ESP32_SDK_CONFIG_DIR "" CACHE PATH "Path to the sdkconfig file to be used for this FORTE build")

  if(NOT IS_DIRECTORY ${ESP32_SDK_CONFIG_DIR})
    message(FATAL_ERROR "The ESP32_SDK_CONFIG_DIR (" ${ESP32_SDK_CONFIG_DIR} ") does not exist or is not set")
  endif()
  forte_add_include_directories(${ESP32_SDK_CONFIG_DIR})


endif (FORTE_MODULE_ESP32)