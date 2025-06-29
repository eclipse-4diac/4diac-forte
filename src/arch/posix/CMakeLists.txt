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

forte_register_architecture(Posix)

# TODO: should be replaced by better arch-independent mechanism
target_include_directories(forte-global INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/)

forte_register_timerhandler(CPCTimerHandler pctimeha.h)
forte_set_default_timerhandler(CPCTimerHandler)

target_sources(forte PRIVATE ../main.cpp)
add_library(forte-arch-posix OBJECT
    forte_architecture_time.cpp
    ../forte_standard_time.cpp
    forte_thread.cpp forte_thread.h
    forte_sync.cpp forte_sync.h
    forte_sem.cpp forte_sem.h
    pctimeha.cpp pctimeha.h
    ../genforte_printer.cpp
    ../genforte_fileio.cpp
    ../utils/timespec_utils.cpp
    ../common/forte_specific_architecture.cpp
    $<$<BOOL:${FORTE_COM_ETH}>:../fdselecthand.cpp>
    $<$<BOOL:${FORTE_COM_ETH}>:../fdselecthand.h>
    $<$<BOOL:${FORTE_COM_ETH}>:../bsdsocketinterf.cpp>
    $<$<BOOL:${FORTE_COM_ETH}>:../bsdsocketinterf.h>
    $<$<BOOL:${FORTE_COM_SER}>:posixsercommlayer.cpp>
    $<$<BOOL:${FORTE_COM_SER}>:posixsercommlayer.h>
)

target_link_libraries(forte PRIVATE forte-arch-posix)

target_link_libraries(forte-arch-posix PUBLIC
    pthread
    $<$<NOT:$<BOOL:${APPLE}>>:rt>
)

set(FORTE_POSIX_GENERATE_MAP_FILE FALSE CACHE BOOL "Enable the generation of map files")
mark_as_advanced(FORTE_POSIX_GENERATE_MAP_FILE)
target_link_options(forte-arch-posix PUBLIC $<$<BOOL:${FORTE_POSIX_GENERATE_MAP_FILE}>:-Wl,-M,-Map,forte.map,-cref>)

if (FORTE_COM_ETH)
   forte_register_handler(CFDSelectHandler sockhand.h)
endif()

if (NOT APPLE)
    forte_register_com_layer(SER OFF "ser" CPosixSerCommLayer posixsercommlayer.h "Enable Forte serial line communication")
endif()
