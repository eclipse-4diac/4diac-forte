/*******************************************************************************
 * Copyright (c) 2014, 2025 fortiss GmbH, Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Waldemar Eisenmenger
 *               - initial API and implementation and/or initial documentation
 *   Alois Zoitl - copied to core/io and adjusted to core/io process
 *                 interface needs
 *******************************************************************************/
#include "QX_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(OUT);
USE_STRING_ID(PARAMS);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(QX);
USE_STRING_ID(REQ);
USE_STRING_ID(STATUS);
USE_STRING_ID(STRING);


#include "criticalregion.h"
#include "resource.h"

using namespace forte::core::io;

DEFINE_FIRMWARE_FB(FORTE_QX, STRID(QX))

const CStringDictionary::TStringId FORTE_QX::scmDataInputNames[] = {STRID(QI), STRID(PARAMS), STRID(OUT)};
const CStringDictionary::TStringId FORTE_QX::scmDataInputTypeIds[] = {STRID(BOOL), STRID(STRING), STRID(BOOL)};
const CStringDictionary::TStringId FORTE_QX::scmDataOutputNames[] = {STRID(QO), STRID(STATUS)};
const CStringDictionary::TStringId FORTE_QX::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(STRING)};
const TDataIOID FORTE_QX::scmEIWith[] = {0, 1, scmWithListDelimiter, 0, 2, scmWithListDelimiter};
const TForteInt16 FORTE_QX::scmEIWithIndexes[] = {0, 3};
const CStringDictionary::TStringId FORTE_QX::scmEventInputNames[] = {STRID(INIT), STRID(REQ)};
const CStringDictionary::TStringId FORTE_QX::scmEventInputTypeIds[] = {STRID(EInit), STRID(Event)};
const TDataIOID FORTE_QX::scmEOWith[] = {0, 1, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_QX::scmEOWithIndexes[] = {0, 3};
const CStringDictionary::TStringId FORTE_QX::scmEventOutputNames[] = {STRID(INITO), STRID(CNF)};
const CStringDictionary::TStringId FORTE_QX::scmEventOutputTypeIds[] = {STRID(Event), STRID(Event)};
const SFBInterfaceSpec FORTE_QX::scmFBInterfaceSpec = {
  2, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  2, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  3, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_QX::FORTE_QX(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CProcessInterfaceFB(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_OUT(0_BOOL),
    conn_OUT(nullptr) {
};

void FORTE_QX::setInitialValues() {
  CProcessInterfaceFB::setInitialValues();
  var_OUT = 0_BOOL;
}

void FORTE_QX::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventINITID:
      if (var_QI) {
        var_QO = CIEC_BOOL(CProcessInterfaceFB::initialise(false, paECET)); //initialise as output
      } else {
        var_QO = CIEC_BOOL(CProcessInterfaceFB::deinitialise());
      }
      sendOutputEvent(scmEventINITOID, paECET);
      break;
    case scmEventREQID:
      if (var_QI) {
        var_QO = CProcessInterfaceFB::write(var_OUT);
      } else {
        var_QO = false_BOOL;
      }
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_QX::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_QI, conn_QI);
      readData(2, var_OUT, conn_OUT);
      break;
    }
    default:
      CProcessInterfaceFB::readInputData(paEIID);
      break;
  }
}

CIEC_ANY *FORTE_QX::getDI(size_t paIndex) {
  if(paIndex == 2) {
    return &var_OUT;
  }
  return CProcessInterfaceFB::getDI(paIndex);
}

CDataConnection **FORTE_QX::getDIConUnchecked(TPortId paIndex) {
  if(paIndex == 2) {
    return &conn_OUT;
  }
  return CProcessInterfaceFB::getDIConUnchecked(paIndex);
}
