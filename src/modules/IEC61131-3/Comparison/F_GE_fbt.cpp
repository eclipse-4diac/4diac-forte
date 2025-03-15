/*******************************************************************************
 * Copyright (c) 2013 ACIN
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#include "F_GE_fbt.h"

USE_STRING_ID(ANY_ELEMENTARY);
USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_GE);
USE_STRING_ID(IN1);
USE_STRING_ID(IN2);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);


#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_F_GE, STRID(F_GE))

const CStringDictionary::TStringId FORTE_F_GE::scmDataInputNames[] = {STRID(IN1), STRID(IN2)};

const CStringDictionary::TStringId FORTE_F_GE::scmDataInputTypeIds[] = {STRID(ANY_ELEMENTARY), STRID(ANY_ELEMENTARY)};

const CStringDictionary::TStringId FORTE_F_GE::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_F_GE::scmDataOutputTypeIds[] = {STRID(BOOL)};

const TDataIOID FORTE_F_GE::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_F_GE::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_GE::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_F_GE::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_F_GE::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_GE::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_GE::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_F_GE::scmEventOutputTypeIds[] = {STRID(Event)};


const SFBInterfaceSpec FORTE_F_GE::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  2, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_F_GE::FORTE_F_GE(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_IN1(CIEC_ANY_ELEMENTARY_VARIANT()),
    var_IN2(CIEC_ANY_ELEMENTARY_VARIANT()),
    var_OUT(CIEC_BOOL(0)),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN1(nullptr),
    conn_IN2(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
};

void FORTE_F_GE::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      var_OUT = CIEC_BOOL(var_IN1 >= var_IN2);
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_F_GE::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN1, conn_IN1);
      readData(1, var_IN2, conn_IN2);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_GE::writeOutputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_F_GE::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN1;
    case 1: return &var_IN2;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_GE::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_GE::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_GE::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN1;
    case 1: return &conn_IN2;
  }
  return nullptr;
}

CDataConnection *FORTE_F_GE::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}


