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

# option to enable ESP32 with cmake
option(FORTE_MODULE_ESP32 "Interacting with GPIOs of esp32 devices via gpio" OFF)

if (NOT FORTE_MODULE_ESP32)
    return()
endif ()

if (NOT IS_DIRECTORY ${CMAKE_SOURCE_DIR}/${ESP32_SDK_CONFIG_DIR})
    message(FATAL_ERROR "The ESP32_SDK_CONFIG_DIR (" ${CMAKE_SOURCE_DIR}/${ESP32_SDK_CONFIG_DIR}
                        ") does not exist or is not set"
    )
endif ()



add_library(forte-esp32
					${CMAKE_SOURCE_DIR}/${ESP32_SDK_CONFIG_DIR}/sdkconfig.h)
target_link_libraries(forte-esp32 PUBLIC forte-core)
target_link_libraries(forte PUBLIC forte-esp32)


target_sources(forte-esp32 PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_SOURCE_DIR}/${ESP32_SDK_CONFIG_DIR}
)

target_include_directories(
	forte-core
	PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/${ESP32_SDK_CONFIG_DIR}> 
	$<INSTALL_INTERFACE:include>
	)




install(TARGETS forte-esp32 EXPORT forte-export FILE_SET HEADERS)

