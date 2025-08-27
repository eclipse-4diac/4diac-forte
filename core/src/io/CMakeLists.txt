# *******************************************************************************
# Copyright (c) 2017, 2025 fortiss GmbH, Johannes Kepler University Linz This
# program and the accompanying materials are made available under the terms of
# the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Johannes Messmer *   - initial API and implementation and/or
# initial documentation
# *******************************************************************************/

# ############################################################################
# IO
# ############################################################################

option(FORTE_IO "Enable IO Modules" OFF)

if (NOT FORTE_IO)
    return()
endif ()

message(STATUS "Process Interface: Modular IO")

target_sources(forte-core PRIVATE
        processinterfacefb.cpp
        IB_fbt.cpp
        ID_fbt.cpp
        IE_fbt.cpp
        IL_fbt.cpp
        IW_fbt.cpp
        IX_fbt.cpp
        QB_fbt.cpp
        QD_fbt.cpp
        QL_fbt.cpp
        QW_fbt.cpp
        QX_fbt.cpp
)

add_subdirectory(configFB)
add_subdirectory(device)
add_subdirectory(eIO)
add_subdirectory(mapper)
