#/************************************************************************************
# Copyright (c) 2016, 2024 fortiss GmbH, HR Agrartechnik GmbH, Jose Cabral
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#  Guru Chandrasekhara - initial API and implementation and/or initial documentation
# * Jose Cabral - Cleaning
#   Christoph Binder - add possibility to configure simulated time
#   Franz Hoepfinger - add memory allocation config.
#                    - add stack size config.
#############################################################################

forte_register_architecture(FreeRTOSLwIP)

set(FORTE_FREERTOS_INCLUDE_DIR "" CACHE PATH "Path to FreeRTOS include files (leave empty for installed source code)")
if (FORTE_FREERTOS_INCLUDE_DIR)
    target_include_directories(
            forte-core
            PUBLIC
            "${FORTE_FREERTOS_INCLUDE_DIR}"
    )
endif ()

option(FORTE_FREERTOS_PLUS_TCP "Use FreeRTOS-plus-TCP" OFF)
target_compile_definitions(forte-core PUBLIC $<$<BOOL:${FORTE_FREERTOS_PLUS_TCP}>:FORTE_FREERTOS_PLUS_TCP>)

add_subdirectory(include)
add_subdirectory(src)

