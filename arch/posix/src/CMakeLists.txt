#*******************************************************************************
# Copyright (c) 2010 - 2024 ACIN, Profactor GmbH, fortiss GmbH, Jose Cabral
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#   Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny, Martin Melik Merkumians
# *      - initial API and implementation and/or initial documentation
#   Christoph Binder - add possibility to configure simulated time
# *******************************************************************************/

if (NOT APPLE)
    option(FORTE_COM_SER "Enable Forte serial line communication" OFF)
endif ()

target_sources(forte-arch PRIVATE
        forte_architecture_time.cpp
        forte_sem.cpp
        forte_sync.cpp
        forte_thread.cpp
        pctimeha.cpp
        pctimeha.h
        posixsercommlayer.cpp
        posixsercommlayer.h
        ../../common/src/forte_specific_architecture.cpp
        ../../common/src/forte_standard_time.cpp
        ../../common/src/genforte_printer.cpp
        ../../common/src/genforte_fileio.cpp
        ../../common/src/utils/timespec_utils.cpp
        $<$<BOOL:${FORTE_COM_SER}>:${CMAKE_CURRENT_SOURCE_DIR}/posixsercommlayer.cpp>
        $<$<BOOL:${FORTE_COM_SER}>:${CMAKE_CURRENT_SOURCE_DIR}/posixsercommlayer.h>
        $<$<BOOL:${FORTE_COM_ETH}>:${CMAKE_CURRENT_SOURCE_DIR}/../../common/src/fdselecthand.cpp>
        $<$<BOOL:${FORTE_COM_ETH}>:${CMAKE_CURRENT_SOURCE_DIR}/../../common/src/bsdsocketinterf.cpp>
)

add_subdirectory(com)
