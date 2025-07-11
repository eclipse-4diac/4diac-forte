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

add_subdirectory(configFB)
add_subdirectory(device)
add_subdirectory(eIO)
add_subdirectory(mapper)

message(STATUS "Process Interface: Modular IO")

add_library(forte-core-io OBJECT
            processinterfacefb.h
            processinterfacefb.cpp
            inputfb.h
            outputfb.h
            IB_fbt.h
            IB_fbt.cpp
            ID_fbt.h
            ID_fbt.cpp
            IE_fbt.h
            IE_fbt.cpp
            IL_fbt.h
            IL_fbt.cpp
            IW_fbt.h
            IW_fbt.cpp
            IX_fbt.h
            IX_fbt.cpp
            QB_fbt.h
            QB_fbt.cpp
            QD_fbt.h
            QD_fbt.cpp
            QL_fbt.h
            QL_fbt.cpp
            QW_fbt.h
            QW_fbt.cpp
            QX_fbt.h
            QX_fbt.cpp
)
target_link_libraries(forte PRIVATE forte-core-io)
