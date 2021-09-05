/*******************************************************************************
 * Copyright (c) 2013 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "F_DT_TO_TOD.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_DT_TO_TOD_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_F_DT_TO_TOD, g_nStringIdF_DT_TO_TOD)

const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scm_anDataInputTypeIds[] = {g_nStringIdDATE_AND_TIME};

const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scm_anDataOutputTypeIds[] = {g_nStringIdTIME_OF_DAY};

const TForteInt16 FORTE_F_DT_TO_TOD::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_F_DT_TO_TOD::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_DT_TO_TOD::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_F_DT_TO_TOD::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_F_DT_TO_TOD::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


void FORTE_F_DT_TO_TOD::executeEvent(int pa_nEIID){
  if(scm_nEventREQID == pa_nEIID){
    st_OUT() = DT_TO_TOD(st_IN());
    sendOutputEvent(scm_nEventCNFID);
  }
}
