# *******************************************************************************
# Copyright (c) 2025 Jose Cabral This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Jose Cabral *    - initial API and implementation and/or
# initial documentation
# *******************************************************************************/
if (NOT FORTE_COM_OPC_UA)
    return()
endif ()

option(FORTE_DEBUG_DEVICE "Compile the debug Device" FALSE)
if (NOT FORTE_DEBUG_DEVICE)
    return()
endif ()

target_compile_definitions(forte-global INTERFACE FORTE_DEBUG_DEVICE)
forte_register_device(DebugDevice)

add_library(forte-stdfblib-ita-debug_device OBJECT
            DebugMGR.h DebugMGR.cpp DebugDevice.h DebugDevice.cpp
)
target_link_libraries(forte PRIVATE forte-stdfblib-ita-debug_device)
