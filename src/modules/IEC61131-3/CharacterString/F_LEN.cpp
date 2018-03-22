/*******************************************************************************
 * Copyright (c) 2013 ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "F_LEN.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_LEN_gen.cpp"
#endif
#include <anyhelper.h>

DEFINE_FIRMWARE_FB(FORTE_F_LEN, g_nStringIdF_LEN)

const CStringDictionary::TStringId FORTE_F_LEN::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId FORTE_F_LEN::scm_anDataInputTypeIds[] = {g_nStringIdANY_STRING};

const CStringDictionary::TStringId FORTE_F_LEN::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_LEN::scm_anDataOutputTypeIds[] = {g_nStringIdANY_INT};

const TForteInt16 FORTE_F_LEN::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_F_LEN::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId FORTE_F_LEN::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_LEN::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_F_LEN::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_F_LEN::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_F_LEN::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


void FORTE_F_LEN::executeEvent(int pa_nEIID){
  if (scm_nEventREQID == pa_nEIID) {
  anyElementaryFBHelper<FORTE_F_LEN>(IN().getDataTypeID(), *this);
  sendOutputEvent(scm_nEventCNFID);
  }
}



