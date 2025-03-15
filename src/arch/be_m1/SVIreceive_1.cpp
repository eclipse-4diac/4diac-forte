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
#include "SVIreceive_1.h"

DEFINE_FIRMWARE_FB(FORTE_SVIreceive_1, STRID(SVIreceive_1))

const CStringDictionary::TStringId FORTE_SVIreceive_1::scmDataInputNames[] = { STRID(QI), STRID(ID) };

const CStringDictionary::TStringId FORTE_SVIreceive_1::scmDataInputTypeIds[] = { STRID(BOOL), STRID(STRING) };

const CStringDictionary::TStringId FORTE_SVIreceive_1::scmDataOutputNames[] = { STRID(QO), STRID(RD_1) };

const CStringDictionary::TStringId FORTE_SVIreceive_1::scmDataOutputTypeIds[] = { STRID(BOOL), STRID(ANY) };

const TForteInt16 FORTE_SVIreceive_1::scmEIWithIndexes[] = { 0, 3 };
const TDataIOID FORTE_SVIreceive_1::scmEIWith[] = { 0, 1, scmWithListDelimiter, 0, scmWithListDelimiter };
const CStringDictionary::TStringId FORTE_SVIreceive_1::scmEventInputNames[] = { STRID(INIT), STRID(REQ) };
const CStringDictionary::TStringId FORTE_SVIreceive_1::scmEventInputTypeIds[] = {STRID(EInit), STRID(Event)};

const TDataIOID FORTE_SVIreceive_1::scmEOWith[] = { 0, scmWithListDelimiter, 0, 1, scmWithListDelimiter };
const TForteInt16 FORTE_SVIreceive_1::scmEOWithIndexes[] = { 0, 2, -1 };
const CStringDictionary::TStringId FORTE_SVIreceive_1::scmEventOutputNames[] = { STRID(INITO), STRID(CNF) };
const CStringDictionary::TStringId FORTE_SVIreceive_1::scmEventOutputTypeIds[] = {STRID(Event), STRID(Event)};

const SFBInterfaceSpec FORTE_SVIreceive_1::scmFBInterfaceSpec = { 2, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes, 2, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes, 2, scmDataInputNames, scmDataInputTypeIds, 2, scmDataOutputNames, scmDataOutputTypeIds, 0, 0 };

void FORTE_SVIreceive_1::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID){
    case scmEventINITID:
      if(true == QI()){
        QO() = registerSVIvar(ID(), RD_1(), SVI_F_IN);
      }
      sendOutputEvent(scmEventINITOID, paECET);
      break;

    case scmEventREQID:
      sendOutputEvent(scmEventCNFID, paECET);
      break;
    default:
      break;
  }
}

