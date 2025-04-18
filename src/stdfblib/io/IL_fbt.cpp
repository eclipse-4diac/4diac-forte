/*************************************************************************
 * Copyright (c) 2018 TU Wien/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik-Merkumians - adds intial implememtation
 *************************************************************************/

#include "IL_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(IL);
USE_STRING_ID(IN);
USE_STRING_ID(IND);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(LWORD);
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

DEFINE_FIRMWARE_FB(FORTE_IL, STRID(IL))

const CStringDictionary::TStringId FORTE_IL::scmDataInputNames[] = {STRID(QI), STRID(PARAMS)};
const CStringDictionary::TStringId FORTE_IL::scmDataInputTypeIds[] = {STRID(BOOL), STRID(STRING)};
const CStringDictionary::TStringId FORTE_IL::scmDataOutputNames[] = {STRID(QO), STRID(STATUS), STRID(IN)};
const CStringDictionary::TStringId FORTE_IL::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(STRING), STRID(LWORD)};
const TDataIOID FORTE_IL::scmEIWith[] = {0, 1, scmWithListDelimiter, 0, scmWithListDelimiter};
const TForteInt16 FORTE_IL::scmEIWithIndexes[] = {0, 3};
const CStringDictionary::TStringId FORTE_IL::scmEventInputNames[] = {STRID(INIT), STRID(REQ)};
const CStringDictionary::TStringId FORTE_IL::scmEventInputTypeIds[] = {STRID(EInit), STRID(Event)};
const TDataIOID FORTE_IL::scmEOWith[] = {0, 1, scmWithListDelimiter, 0, 1, 2, scmWithListDelimiter, 0,
                                         1, 2, scmWithListDelimiter};
const TForteInt16 FORTE_IL::scmEOWithIndexes[] = {0, 3, 7};
const CStringDictionary::TStringId FORTE_IL::scmEventOutputNames[] = {STRID(INITO), STRID(CNF), STRID(IND)};
const CStringDictionary::TStringId FORTE_IL::scmEventOutputTypeIds[] = {STRID(EInit), STRID(Event), STRID(Event)};
const SFBInterfaceSpec FORTE_IL::scmFBInterfaceSpec = {2,
                                                       scmEventInputNames,
                                                       scmEventInputTypeIds,
                                                       scmEIWith,
                                                       scmEIWithIndexes,
                                                       3,
                                                       scmEventOutputNames,
                                                       scmEventOutputTypeIds,
                                                       scmEOWith,
                                                       scmEOWithIndexes,
                                                       2,
                                                       scmDataInputNames,
                                                       scmDataInputTypeIds,
                                                       3,
                                                       scmDataOutputNames,
                                                       scmDataOutputTypeIds,
                                                       0,
                                                       nullptr,
                                                       0,
                                                       nullptr};

FORTE_IL::FORTE_IL(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CProcessInterface(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_QI(0_BOOL),
    var_PARAMS(""_STRING),
    var_QO(0_BOOL),
    var_STATUS(""_STRING),
    var_IN(0_LWORD),
    conn_INITO(*this, 0),
    conn_CNF(*this, 1),
    conn_IND(*this, 2),
    conn_QI(nullptr),
    conn_PARAMS(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_STATUS(*this, 1, var_STATUS),
    conn_IN(*this, 2, var_IN) {};

void FORTE_IL::setInitialValues() {
  var_QI = 0_BOOL;
  var_PARAMS = ""_STRING;
  var_QO = 0_BOOL;
  var_STATUS = ""_STRING;
  var_IN = 0_LWORD;
}

void FORTE_IL::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case cgExternalEventID: sendOutputEvent(scmEventINDID, paECET); break;
    case scmEventINITID:
      if (var_QI) {
        var_QO = CIEC_BOOL(CProcessInterface::initialise(true, paECET)); // initialise as input
      } else {
        var_QO = CIEC_BOOL(CProcessInterface::deinitialise());
      }
      sendOutputEvent(scmEventINITOID, paECET);
      break;
    case scmEventREQID:
      if (var_QI) {
        var_QO = CIEC_BOOL(CProcessInterface::read(var_IN));
      } else {
        var_QO = false_BOOL;
      }
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_IL::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_PARAMS, conn_PARAMS);
      break;
    }
    case scmEventREQID: {
      readData(0, var_QI, conn_QI);
      break;
    }
    default: break;
  }
}

void FORTE_IL::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITOID: {
      writeData(0, var_QO, conn_QO);
      writeData(1, var_STATUS, conn_STATUS);
      break;
    }
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
    default: break;
  }
}

CIEC_ANY *FORTE_IL::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_PARAMS;
  }
  return nullptr;
}

CIEC_ANY *FORTE_IL::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
    case 2: return &var_IN;
  }
  return nullptr;
}

CEventConnection *FORTE_IL::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_CNF;
    case 2: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_IL::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_PARAMS;
  }
  return nullptr;
}

CDataConnection *FORTE_IL::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
    case 2: return &conn_IN;
  }
  return nullptr;
}
