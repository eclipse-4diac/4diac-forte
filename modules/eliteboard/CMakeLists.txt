# *******************************************************************************
# Copyright (c) 2021, 2022 Jonathan Lainer This program and the accompanying
# materials are made available under the terms of the Eclipse Public License
# 2.0 which is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Jonathan Lainer - Initial implementation. Maximilian Scharf -
# Adaptations for event-triggered IOs, ADCs and DACs.
# *******************************************************************************/

if (NOT FORTE_ARCHITECTURE STREQUAL "FreeRTOSLwIP")
    return()
endif ()

forte_register_io(ELITEBOARD OFF
                  "Support for the eLITe-Board development board"
)

add_library(forte-eliteboard OBJECT)
target_link_libraries(forte-eliteboard PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<LINK_LIBRARY:WHOLE_ARCHIVE,forte-eliteboard>)

# This should be part of architecture code
# forte_add_definition("-ffunction-sections -fdata-sections
# -fno-threadsafe-statics -fno-rtti -fno-exceptions -Wl,--gc-sections")

add_subdirectory(handle)
add_subdirectory(handler)
add_subdirectory(types)
