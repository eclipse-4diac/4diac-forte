/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#include "LocalizedText2LocalizedText.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "LocalizedText2LocalizedText_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_LocalizedText2LocalizedText, g_nStringIdLocalizedText2LocalizedText)

const CStringDictionary::TStringId FORTE_LocalizedText2LocalizedText::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId FORTE_LocalizedText2LocalizedText::scm_anDataInputTypeIds[] = {g_nStringIdLocalizedText};

const CStringDictionary::TStringId FORTE_LocalizedText2LocalizedText::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_LocalizedText2LocalizedText::scm_anDataOutputTypeIds[] = {g_nStringIdLocalizedText};

const TForteInt16 FORTE_LocalizedText2LocalizedText::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_LocalizedText2LocalizedText::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId FORTE_LocalizedText2LocalizedText::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_LocalizedText2LocalizedText::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_LocalizedText2LocalizedText::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_LocalizedText2LocalizedText::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_LocalizedText2LocalizedText::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


void FORTE_LocalizedText2LocalizedText::executeEvent(int pa_nEIID){
  if(scm_nEventREQID == pa_nEIID) {
    st_OUT() = st_IN();
    sendOutputEvent(scm_nEventCNFID);
  }
}



