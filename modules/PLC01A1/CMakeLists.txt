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

if (NOT LINUX)
    return()
endif ()

option(FORTE_MODULE_PLC01A1 "Support for X-NUCLEO-PLC01A" OFF)

if (NOT FORTE_MODULE_PLC01A1)
    return()
endif ()

add_library(forte-PLC01A1
            plc01a1_controller.h plc01a1_controller.cpp plc01a1_config_fb.h
            plc01a1_config_fb.cpp
)
target_link_libraries(forte-PLC01A1 PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-PLC01A1,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-PLC01A1>>)
install(TARGETS forte-PLC01A1 EXPORT forte-export FILE_SET HEADERS)
