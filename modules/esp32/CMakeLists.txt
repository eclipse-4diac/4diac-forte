# *******************************************************************************
# Copyright (c) 2022-2023 HR Agrartechnik GmbH This program and the
# accompanying materials are made available under the terms of the Eclipse
# Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Franz Hoepfinger - initial API and implementation and/or
# initial documentation
# *******************************************************************************/

# ############################################################################
# FORTE_MODULE_ESP32  integration.
# ############################################################################

# option to enable OPC UA with cmake
option(FORTE_MODULE_ESP32 "Interacting with GPIOs of esp32 devices via gpio" OFF)

if (NOT FORTE_MODULE_ESP32)
    return()
endif ()

set(ESP32_SDK_CONFIG_DIR
    ""
    CACHE PATH "Path to the sdkconfig file to be used for this FORTE build"
)

if (NOT IS_DIRECTORY ${ESP32_SDK_CONFIG_DIR})
    message(FATAL_ERROR "The ESP32_SDK_CONFIG_DIR (" ${ESP32_SDK_CONFIG_DIR}
                        ") does not exist or is not set"
    )
endif ()

target_include_directories(forte-esp32 PUBLIC ${ESP32_SDK_CONFIG_DIR})
