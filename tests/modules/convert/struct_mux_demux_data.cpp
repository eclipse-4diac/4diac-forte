/*******************************************************************************
 * Copyright (c) 2020 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial tests
 *******************************************************************************/
#include "struct_mux_demux_data.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "struct_mux_demux_data_gen.cpp"
#endif

DEFINE_FIRMWARE_DATATYPE(Struct_Muxer_Test_Struct_1, g_nStringIdStruct_Muxer_Test_Struct_1)

CIEC_Struct_Muxer_Test_Struct_1::CIEC_Struct_Muxer_Test_Struct_1() :
    CIEC_STRUCT(g_nStringIdStruct_Muxer_Test_Struct_1, 3, scm_unElementTypes, scm_unElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_Struct_Muxer_Test_Struct_1::scm_unElementTypes[] = { g_nStringIdINT, g_nStringIdINT, g_nStringIdSTRING };
const CStringDictionary::TStringId CIEC_Struct_Muxer_Test_Struct_1::scm_unElementNames[] = { g_nStringIdVar1, g_nStringIdVar2, g_nStringIdVar3 };


DEFINE_FIRMWARE_DATATYPE(Struct_Muxer_Test_Struct_2, g_nStringIdStruct_Muxer_Test_Struct_2)

CIEC_Struct_Muxer_Test_Struct_2::CIEC_Struct_Muxer_Test_Struct_2() :
    CIEC_STRUCT(g_nStringIdStruct_Muxer_Test_Struct_2, 3, scm_unElementTypes, scm_unElementNames, e_APPLICATION + e_CONSTRUCTED + 1) {
}

const CStringDictionary::TStringId CIEC_Struct_Muxer_Test_Struct_2::scm_unElementTypes[] = { g_nStringIdINT, g_nStringIdSTRING, g_nStringIdINT};
const CStringDictionary::TStringId CIEC_Struct_Muxer_Test_Struct_2::scm_unElementNames[] = { g_nStringIdVar1, g_nStringIdVar2, g_nStringIdVar3 };


