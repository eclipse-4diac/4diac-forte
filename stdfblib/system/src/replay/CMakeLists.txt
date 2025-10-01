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

if (NOT FORTE_TRACE_CTF_REPLAY_DEBUGGING)
    return()
endif ()

option(FORTE_REPLAY_DEVICE "Compile the replay Device" FALSE)

if (NOT FORTE_REPLAY_DEVICE)
    return()
endif ()

target_compile_definitions(forte-system PRIVATE FORTE_REPLAY_DEVICE)
target_link_libraries(forte-system PRIVATE ${FORTE_TRACE_READER_LIB})

target_sources(forte-system PRIVATE
        deviceReplayer.h
        deviceReplayer.cpp
        resourceReplayer.h
        resourceReplayer.cpp
        ReplayDevice.h
        ReplayDevice.cpp
        ReplayMGR.h
        ReplayMGR.cpp
)
