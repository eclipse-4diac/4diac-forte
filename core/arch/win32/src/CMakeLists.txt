#*******************************************************************************
# Copyright (c) 2010 - 2024 ACIN, Profactor GmbH, fortiss GmbH, Jose Cabral
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#   Alois Zoitl, Gerhard Ebenhofer, Martin Melik-Merkumians - initial API and implementation and/or initial documentation
#   Christoph Binder - add possibility to configure simulated time
# *******************************************************************************/

target_sources(forte-core PRIVATE
        forte_architecture_time.cpp
        forte_sem.cpp
        forte_specific_architecture.cpp
        forte_thread.cpp
        pctimeha.cpp
        pctimeha.h
        winforte_printer.cpp
        ../../common/src/forte_standard_time.cpp
        ../../common/src/genforte_fileio.cpp
        ../../common/src/utils/timespec_utils.cpp
        $<$<BOOL:${BUILD_SHARED_LIBS}>:${CMAKE_CURRENT_SOURCE_DIR}/load_option.cpp>
        $<$<BOOL:${FORTE_EXTERNAL_TIMEHA}>:${CMAKE_CURRENT_SOURCE_DIR}/externtimeha.cpp>
        $<$<BOOL:${FORTE_EXTERNAL_TIMEHA}>:${CMAKE_CURRENT_SOURCE_DIR}/externtimeha.h>
        $<$<NOT:$<BOOL:${FORTE_WINDOWS_XP_COMPAT}>>:${CMAKE_CURRENT_SOURCE_DIR}/forte_sync.cpp>
        $<$<BOOL:${FORTE_WINDOWS_XP_COMPAT}>:${CMAKE_CURRENT_SOURCE_DIR}/forte_sync_xp.cpp>
        $<$<BOOL:${FORTE_COM_ETH}>:${CMAKE_CURRENT_SOURCE_DIR}/win32socketinterf.cpp>
        $<$<BOOL:${FORTE_COM_ETH}>:${CMAKE_CURRENT_SOURCE_DIR}/../../common/src/fdselecthand.cpp>
)

add_subdirectory(com)
add_subdirectory(serlayer)
