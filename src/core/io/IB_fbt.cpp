/*************************************************************************
 * Copyright (c) 2025 TU Wien/ACIN, Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik-Merkumians - adds intial implememtation
 *   Alois Zoitl - copied to core/io and adjusted to core/io process
 *                 interface needs
 *************************************************************************/

#include "IB_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(BYTE);
USE_STRING_ID(CNF);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(IB);
USE_STRING_ID(IN);
USE_STRING_ID(IND);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(PARAMS);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(REQ);
USE_STRING_ID(STATUS);
USE_STRING_ID(STRING);


#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include "criticalregion.h"
#include "resource.h"

using namespace forte::core::io;

DEFINE_FIRMWARE_FB(FORTE_IB, STRID(IB))

const CStringDictionary::TStringId FORTE_IB::scmDataInputNames[] = {STRID(QI), STRID(PARAMS)};
const CStringDictionary::TStringId FORTE_IB::scmDataInputTypeIds[] = {STRID(BOOL), STRID(STRING)};
const CStringDictionary::TStringId FORTE_IB::scmDataOutputNames[] = {STRID(QO), STRID(STATUS), STRID(IN)};
const CStringDictionary::TStringId FORTE_IB::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(STRING), STRID(BYTE)};
const TDataIOID FORTE_IB::scmEIWith[] = {0, 1, scmWithListDelimiter, 0, scmWithListDelimiter};
const TForteInt16 FORTE_IB::scmEIWithIndexes[] = {0, 3};
const CStringDictionary::TStringId FORTE_IB::scmEventInputNames[] = {STRID(INIT), STRID(REQ)};
const CStringDictionary::TStringId FORTE_IB::scmEventInputTypeIds[] = {STRID(EInit), STRID(Event)};
const TDataIOID FORTE_IB::scmEOWith[] = {0, 1, scmWithListDelimiter, 0, 1, 2, scmWithListDelimiter, 0, 1, 2, scmWithListDelimiter};
const TForteInt16 FORTE_IB::scmEOWithIndexes[] = {0, 3, 7};
const CStringDictionary::TStringId FORTE_IB::scmEventOutputNames[] = {STRID(INITO), STRID(CNF), STRID(IND)};
const CStringDictionary::TStringId FORTE_IB::scmEventOutputTypeIds[] = {STRID(EInit), STRID(Event), STRID(Event)};
const SFBInterfaceSpec FORTE_IB::scmFBInterfaceSpec = {
  2, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  3, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  2, scmDataInputNames, scmDataInputTypeIds,
  3, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_IB::FORTE_IB(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CProcessInterfaceFB(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_IN(0_BYTE),
    var_conn_IN(var_IN),
    conn_IND(this, 2),
    conn_IN(this, 2, &var_conn_IN) {
};

void FORTE_IB::setInitialValues() {
  CProcessInterfaceFB::setInitialValues();
  var_IN = 0_BYTE;
}

void FORTE_IB::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case cgExternalEventID:
      sendOutputEvent(scmEventINDID, paECET);
      break;
    case scmEventINITID:
      if (var_QI) {
        var_QO = CIEC_BOOL(CProcessInterfaceFB::initialise(true, paECET)); //initialise as input
      } else {
        var_QO = CIEC_BOOL(CProcessInterfaceFB::deinitialise());
      }
      sendOutputEvent(scmEventINITOID, paECET);
      break;
    case scmEventREQID:
      if (var_QI) {
        var_QO = CProcessInterfaceFB::read(var_IN);
      } else {
        var_QO = false_BOOL;
      }
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}


void FORTE_IB::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_QO, conn_QO);
      writeData(1, var_STATUS, conn_STATUS);
      writeData(2, var_IN, conn_IN);
      break;
    }
    case scmEventINDID: {
      writeData(0, var_QO, conn_QO);
      writeData(1, var_STATUS, conn_STATUS);
      writeData(2, var_IN, conn_IN);
      break;
    }
    default:
      CProcessInterfaceFB::writeOutputData(paEIID);
      break;
  }
}

CIEC_ANY *FORTE_IB::getDO(const size_t paIndex) {
  if(paIndex == 2){
    return &var_IN;
  }
  return CProcessInterfaceFB::getDO(paIndex);
}

CEventConnection *FORTE_IB::getEOConUnchecked(const TPortId paIndex) {
  if(paIndex == 2) {
    return &conn_IND;
  }
  return CProcessInterfaceFB::getEOConUnchecked(paIndex);
}

CDataConnection *FORTE_IB::getDOConUnchecked(const TPortId paIndex) {
  if(paIndex == 2) {
    return &conn_IN;
  }
  return CProcessInterfaceFB::getDOConUnchecked(paIndex);
}
