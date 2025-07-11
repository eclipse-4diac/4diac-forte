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

forte_register_com_layer(ETH ON "ip" CIPComLayer ipcomlayer.h
                         "Enable Forte Com Ethernet"
)
forte_register_com_layer(FBDK ON "fbdk" CFBDKASN1ComLayer fbdkasn1layer.h
                         "Enable Forte Com FBDK"
)
forte_register_com_layer(LOCAL ON "loc" CLocalComLayer localcomlayer.h
                         "Enable Forte local communication"
)
forte_register_com_layer(RAW ON "raw" CRawDataComLayer rawdatacomlayer.h
                         "Enable Forte raw communication"
)
forte_register_com_layer(STRUCT_MEMBER
                         OFF
                         "structmemb"
                         CStructMemberLocalComLayer
                         structmembercomlayer.h
                         "Local communication layer writing to a single element in a struct"
)
add_library(forte-core-cominfra OBJECT
            comtypes.h
            comlayersmanager.h
            basecommfb.h
            basecommfb.cpp
            commfb.h
            commfb.cpp
            comlayer.h
            comlayer.cpp
            $<$<BOOL:${FORTE_COM_ETH}>:ipcomlayer.cpp>
            $<$<BOOL:${FORTE_COM_ETH}>:ipcomlayer.h>
            $<$<BOOL:${FORTE_COM_FBDK}>:fbdkasn1layer.cpp>
            $<$<BOOL:${FORTE_COM_FBDK}>:fbdkasn1layer.h>
            $<$<BOOL:${FORTE_COM_LOCAL}>:localcomlayer.cpp>
            $<$<BOOL:${FORTE_COM_LOCAL}>:localcomlayer.h>
            $<$<BOOL:${FORTE_COM_RAW}>:rawdatacomlayer.cpp>
            $<$<BOOL:${FORTE_COM_RAW}>:rawdatacomlayer.h>
            $<$<BOOL:${FORTE_COM_STRUCT_MEMBER}>:structmembercomlayer.cpp>
            $<$<BOOL:${FORTE_COM_STRUCT_MEMBER}>:structmembercomlayer.h>
)
target_link_libraries(forte PRIVATE forte-core-cominfra)
