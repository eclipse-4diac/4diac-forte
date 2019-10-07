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

#include "F_DIVTIME.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_DIVTIME_gen.cpp"
#endif
#include <anyhelper.h>

DEFINE_FIRMWARE_FB(FORTE_F_DIVTIME, g_nStringIdF_DIVTIME)

const CStringDictionary::TStringId FORTE_F_DIVTIME::scm_anDataInputNames[] = {g_nStringIdIN1, g_nStringIdIN2};

const CStringDictionary::TStringId FORTE_F_DIVTIME::scm_anDataInputTypeIds[] = {g_nStringIdTIME, g_nStringIdANY_NUM};

const CStringDictionary::TStringId FORTE_F_DIVTIME::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_DIVTIME::scm_anDataOutputTypeIds[] = {g_nStringIdTIME};

const TForteInt16 FORTE_F_DIVTIME::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_F_DIVTIME::scm_anEIWith[] = {0, 1, 255};
const CStringDictionary::TStringId FORTE_F_DIVTIME::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_DIVTIME::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_F_DIVTIME::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_F_DIVTIME::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_F_DIVTIME::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  2,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


void FORTE_F_DIVTIME::executeEvent(int pa_nEIID){
  if(scm_nEventREQID == pa_nEIID){
    anyIntFBHelper<FORTE_F_DIVTIME>(IN2().getDataTypeID(), *this);
    sendOutputEvent(scm_nEventCNFID);
  }
}
