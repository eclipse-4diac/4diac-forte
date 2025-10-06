# ############################################################################
# Copyright (c) 2010-2014 fortiss, TU Wien ACIN and Profactor GmbH.
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Alois Zoitl, Michael Hofmann - initial implementation and
# rework communication infrastructure Martin Melik-Merkumians, Monika Wenger -
# add raw communication layer for ONE STRING input and ONE STRING output
# ############################################################################

# ############################################################################
# Core
# ############################################################################

option(FORTE_COM_ETH "Enable Forte Com Ethernet" ON)
option(FORTE_COM_FBDK "Enable Forte Com FBDK" ON)
option(FORTE_COM_LOCAL "Enable Forte local communication" ON)
option(FORTE_COM_RAW "Enable Forte raw communication" ON)
option(FORTE_COM_STRUCT_MEMBER "Local communication layer writing to a single element in a struct" OFF)

target_sources(forte-core PRIVATE
        basecommfb.cpp
        commfb.cpp
        comlayer.cpp
        comlayersmanager.cpp
        $<$<BOOL:${FORTE_COM_ETH}>:${CMAKE_CURRENT_SOURCE_DIR}/ipcomlayer.cpp>
        $<$<BOOL:${FORTE_COM_ETH}>:${CMAKE_CURRENT_SOURCE_DIR}/ipcomlayer.h>
        $<$<BOOL:${FORTE_COM_FBDK}>:${CMAKE_CURRENT_SOURCE_DIR}/fbdkasn1layer.cpp>
        $<$<BOOL:${FORTE_COM_FBDK}>:${CMAKE_CURRENT_SOURCE_DIR}/fbdkasn1layer.h>
        $<$<BOOL:${FORTE_COM_LOCAL}>:${CMAKE_CURRENT_SOURCE_DIR}/localcomlayer.cpp>
        $<$<BOOL:${FORTE_COM_LOCAL}>:${CMAKE_CURRENT_SOURCE_DIR}/localcomlayer.h>
        $<$<BOOL:${FORTE_COM_RAW}>:${CMAKE_CURRENT_SOURCE_DIR}/rawdatacomlayer.cpp>
        $<$<BOOL:${FORTE_COM_RAW}>:${CMAKE_CURRENT_SOURCE_DIR}/rawdatacomlayer.h>
        $<$<BOOL:${FORTE_COM_STRUCT_MEMBER}>:${CMAKE_CURRENT_SOURCE_DIR}/structmembercomlayer.cpp>
        $<$<BOOL:${FORTE_COM_STRUCT_MEMBER}>:${CMAKE_CURRENT_SOURCE_DIR}/structmembercomlayer.h>
)
