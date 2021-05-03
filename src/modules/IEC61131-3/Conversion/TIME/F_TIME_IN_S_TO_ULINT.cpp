/*******************************************************************************
 * Copyright (c) 2019 TU Wien/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "F_TIME_IN_S_TO_ULINT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_TIME_IN_S_TO_ULINT_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_F_TIME_IN_S_TO_ULINT, g_nStringIdF_TIME_IN_S_TO_ULINT)

const CStringDictionary::TStringId FORTE_F_TIME_IN_S_TO_ULINT::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId FORTE_F_TIME_IN_S_TO_ULINT::scm_anDataInputTypeIds[] = {g_nStringIdTIME};

const CStringDictionary::TStringId FORTE_F_TIME_IN_S_TO_ULINT::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_TIME_IN_S_TO_ULINT::scm_anDataOutputTypeIds[] = {g_nStringIdULINT};

const TForteInt16 FORTE_F_TIME_IN_S_TO_ULINT::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_F_TIME_IN_S_TO_ULINT::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId FORTE_F_TIME_IN_S_TO_ULINT::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_TIME_IN_S_TO_ULINT::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_F_TIME_IN_S_TO_ULINT::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_F_TIME_IN_S_TO_ULINT::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_F_TIME_IN_S_TO_ULINT::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

void FORTE_F_TIME_IN_S_TO_ULINT::alg_REQ(void){
  st_OUT() = TIME_IN_S_TO_ULINT((st_IN()));
}



