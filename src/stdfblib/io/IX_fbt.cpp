/*******************************************************************************
 * Copyright (c) 2014 - 2015 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Waldemar Eisenmenger, Monika Wenger - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "IX_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(IN);
USE_STRING_ID(IND);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(IX);
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

DEFINE_FIRMWARE_FB(FORTE_IX, STRID(IX))

const CStringDictionary::TStringId FORTE_IX::scmDataInputNames[] = {STRID(QI), STRID(PARAMS)};
const CStringDictionary::TStringId FORTE_IX::scmDataInputTypeIds[] = {STRID(BOOL), STRID(STRING)};
const CStringDictionary::TStringId FORTE_IX::scmDataOutputNames[] = {STRID(QO), STRID(STATUS), STRID(IN)};
const CStringDictionary::TStringId FORTE_IX::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(STRING), STRID(BOOL)};
const TDataIOID FORTE_IX::scmEIWith[] = {0, 1, scmWithListDelimiter, 0, scmWithListDelimiter};
const TForteInt16 FORTE_IX::scmEIWithIndexes[] = {0, 3};
const CStringDictionary::TStringId FORTE_IX::scmEventInputNames[] = {STRID(INIT), STRID(REQ)};
const CStringDictionary::TStringId FORTE_IX::scmEventInputTypeIds[] = {STRID(EInit), STRID(Event)};
const TDataIOID FORTE_IX::scmEOWith[] = {0, 1, scmWithListDelimiter, 0, 1, 2, scmWithListDelimiter, 0,
                                         1, 2, scmWithListDelimiter};
const TForteInt16 FORTE_IX::scmEOWithIndexes[] = {0, 3, 7};
const CStringDictionary::TStringId FORTE_IX::scmEventOutputNames[] = {STRID(INITO), STRID(CNF), STRID(IND)};
const CStringDictionary::TStringId FORTE_IX::scmEventOutputTypeIds[] = {STRID(EInit), STRID(Event), STRID(Event)};
const SFBInterfaceSpec FORTE_IX::scmFBInterfaceSpec = {2,
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

FORTE_IX::FORTE_IX(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CProcessInterface(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_QI(0_BOOL),
    var_PARAMS(""_STRING),
    var_QO(0_BOOL),
    var_STATUS(""_STRING),
    var_IN(0_BOOL),
    conn_INITO(*this, 0),
    conn_CNF(*this, 1),
    conn_IND(*this, 2),
    conn_QI(nullptr),
    conn_PARAMS(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_STATUS(*this, 1, var_STATUS),
    conn_IN(*this, 2, var_IN) {};

void FORTE_IX::setInitialValues() {
  var_QI = 0_BOOL;
  var_PARAMS = ""_STRING;
  var_QO = 0_BOOL;
  var_STATUS = ""_STRING;
  var_IN = 0_BOOL;
}

void FORTE_IX::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
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

void FORTE_IX::readInputData(const TEventID paEIID) {
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

void FORTE_IX::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITOID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_QO, conn_QO);
      writeData(scmFBInterfaceSpec.mNumDIs + 1, var_STATUS, conn_STATUS);
      break;
    }
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_QO, conn_QO);
      writeData(scmFBInterfaceSpec.mNumDIs + 1, var_STATUS, conn_STATUS);
      writeData(scmFBInterfaceSpec.mNumDIs + 2, var_IN, conn_IN);
      break;
    }
    case scmEventINDID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_QO, conn_QO);
      writeData(scmFBInterfaceSpec.mNumDIs + 1, var_STATUS, conn_STATUS);
      writeData(scmFBInterfaceSpec.mNumDIs + 2, var_IN, conn_IN);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_IX::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_PARAMS;
  }
  return nullptr;
}

CIEC_ANY *FORTE_IX::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
    case 2: return &var_IN;
  }
  return nullptr;
}

CEventConnection *FORTE_IX::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_CNF;
    case 2: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_IX::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_PARAMS;
  }
  return nullptr;
}

CDataConnection *FORTE_IX::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
    case 2: return &conn_IN;
  }
  return nullptr;
}
