/*******************************************************************************
 * Copyright (c) 2011 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "SVIsend_1.h"

DEFINE_FIRMWARE_FB(FORTE_SVIsend_1, STRID(SVIsend_1))

const CStringDictionary::TStringId FORTE_SVIsend_1::scmDataInputNames[] = {STRID(QI), STRID(ID), STRID(SD_1)};

const CStringDictionary::TStringId FORTE_SVIsend_1::scmDataInputTypeIds[] = {STRID(BOOL), STRID(STRING), STRID(ANY)};

const CStringDictionary::TStringId FORTE_SVIsend_1::scmDataOutputNames[] = {STRID(QO)};

const CStringDictionary::TStringId FORTE_SVIsend_1::scmDataOutputTypeIds[] = {STRID(BOOL)};

const TForteInt16 FORTE_SVIsend_1::scmEIWithIndexes[] = {0, 4};
const TDataIOID FORTE_SVIsend_1::scmEIWith[] = {0, 1, 2, scmWithListDelimiter, 0, 1, 2, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_SVIsend_1::scmEventInputNames[] = {STRID(INIT), STRID(REQ)};
const CStringDictionary::TStringId FORTE_SVIsend_1::scmEventInputTypeIds[] = {STRID(EInit), STRID(Event)};

const TDataIOID FORTE_SVIsend_1::scmEOWith[] = {0, scmWithListDelimiter, 0, scmWithListDelimiter};
const TForteInt16 FORTE_SVIsend_1::scmEOWithIndexes[] = {0, 2, -1};
const CStringDictionary::TStringId FORTE_SVIsend_1::scmEventOutputNames[] = {STRID(INITO), STRID(CNF)};
const CStringDictionary::TStringId FORTE_SVIsend_1::scmEventOutputTypeIds[] = {STRID(Event), STRID(Event)};

const SFBInterfaceSpec FORTE_SVIsend_1::scmFBInterfaceSpec = {
  2,  scmEventInputNames, scmEventInputTypeIds,  scmEIWith,  scmEIWithIndexes,
  2,  scmEventOutputNames, scmEventOutputTypeIds,  scmEOWith, scmEOWithIndexes,  3,  scmDataInputNames, scmDataInputTypeIds,
  1,  scmDataOutputNames, scmDataOutputTypeIds,
  0, 0
};


void FORTE_SVIsend_1::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID){
    case scmEventINITID:
      if(true == QI()){
        QO() = registerSVIvar(ID(),SD_1(),SVI_F_OUT);
      }
      sendOutputEvent(scmEventINITOID, paECET);
      if (true == QO()) {
        printf("Alles OK send\n");
      } else {
        printf("Alles FALSCH send\n");
      }
      break;

    case scmEventREQID:
      sendOutputEvent(scmEventCNFID, paECET);
      break;
    default:
      break;
  }
}



