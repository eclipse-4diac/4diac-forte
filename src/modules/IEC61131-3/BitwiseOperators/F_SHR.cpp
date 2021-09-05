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

#include "F_SHR.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_SHR_gen.cpp"
#endif
#include <anyhelper.h>

DEFINE_FIRMWARE_FB(FORTE_F_SHR, g_nStringIdF_SHR)

const CStringDictionary::TStringId FORTE_F_SHR::scm_anDataInputNames[] = {g_nStringIdIN, g_nStringIdN};

const CStringDictionary::TStringId FORTE_F_SHR::scm_anDataInputTypeIds[] = {g_nStringIdANY_BIT, g_nStringIdANY_INT};

const CStringDictionary::TStringId FORTE_F_SHR::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_SHR::scm_anDataOutputTypeIds[] = {g_nStringIdANY_BIT};

const TForteInt16 FORTE_F_SHR::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_F_SHR::scm_anEIWith[] = {0, 1, 255};
const CStringDictionary::TStringId FORTE_F_SHR::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_SHR::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_F_SHR::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_F_SHR::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_F_SHR::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  2,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


void FORTE_F_SHR::executeEvent(int pa_nEIID){
  if (scm_nEventREQID == pa_nEIID) {
  anyBitFBHelperWithoutBool<FORTE_F_SHR>(st_IN().getDataTypeID(), *this);
  sendOutputEvent(scm_nEventCNFID);
  }
}



