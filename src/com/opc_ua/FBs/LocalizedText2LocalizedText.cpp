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

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_LocalizedText2LocalizedText, g_nStringIdLocalizedText2LocalizedText)

const CStringDictionary::TStringId FORTE_LocalizedText2LocalizedText::scmDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId FORTE_LocalizedText2LocalizedText::scmDataInputTypeIds[] = {g_nStringIdLocalizedText};

const CStringDictionary::TStringId FORTE_LocalizedText2LocalizedText::scmDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_LocalizedText2LocalizedText::scmDataOutputTypeIds[] = {g_nStringIdLocalizedText};

const TForteInt16 FORTE_LocalizedText2LocalizedText::scmEIWithIndexes[] = {0};
const TDataIOID FORTE_LocalizedText2LocalizedText::scmEIWith[] = {0, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_LocalizedText2LocalizedText::scmEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_LocalizedText2LocalizedText::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_LocalizedText2LocalizedText::scmEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_LocalizedText2LocalizedText::scmEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_LocalizedText2LocalizedText::scmFBInterfaceSpec = {
  1,  scmEventInputNames,  scmEIWith,  scmEIWithIndexes,
  1,  scmEventOutputNames,  scmEOWith, scmEOWithIndexes,  1,  scmDataInputNames, scmDataInputTypeIds,
  1,  scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr
};


void FORTE_LocalizedText2LocalizedText::executeEvent(TEventID paEIID){
  if(scmEventREQID == paEIID) {
    st_OUT() = st_IN();
    sendOutputEvent(scmEventCNFID);
  }
}

void FORTE_LocalizedText2LocalizedText::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, *mDIs[0], mDIConns[0]);
      break;
    }
    default:
      break;
  }
}

void FORTE_LocalizedText2LocalizedText::writeOutputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      RES_DATA_CON_CRITICAL_REGION();
      writeData(0, *mDOs[0], mDOConns[0]);
      break;
    }
    default:
      break;
  }
}

