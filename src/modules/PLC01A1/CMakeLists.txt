# *******************************************************************************
# Copyright (c) 2019 fortiss GmbH This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Jose Cabral - initial API and implementation and/or initial
# documentation
# *******************************************************************************/

if (NOT FORTE_ARCHITECTURE STREQUAL "Posix")
    return()
endif ()

forte_register_io(PLC01A1 OFF "Support for X-NUCLEO-PLC01A")

add_library(forte-modules-PLC01A1 OBJECT
            plc01a1_controller.h plc01a1_controller.cpp plc01a1_config_fb.h
            plc01a1_config_fb.cpp
)
target_link_libraries(forte-modules-PLC01A1 PUBLIC forte-core)
target_link_libraries(forte PUBLIC forte-modules-PLC01A1)
