/*******************************************************************************
 * Copyright (c) 2009 - 2013 ACIN, fortiss GmbH
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl, Ingo Hengy
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#include "F_AND_fbt.h"

USE_STRING_ID(ANY_BIT);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_AND);
USE_STRING_ID(IN1);
USE_STRING_ID(IN2);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);


#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_F_AND, STRID(F_AND))

const CStringDictionary::TStringId FORTE_F_AND::scmDataInputNames[] = {STRID(IN1), STRID(IN2)};

const CStringDictionary::TStringId FORTE_F_AND::scmDataInputTypeIds[] = {STRID(ANY_BIT), STRID(ANY_BIT)};

const CStringDictionary::TStringId FORTE_F_AND::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_F_AND::scmDataOutputTypeIds[] = {STRID(ANY_BIT)};

const TDataIOID FORTE_F_AND::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_F_AND::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_AND::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_F_AND::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_F_AND::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_AND::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_AND::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_F_AND::scmEventOutputTypeIds[] = {STRID(Event)};


const SFBInterfaceSpec FORTE_F_AND::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  2, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_F_AND::FORTE_F_AND(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_IN1(CIEC_ANY_BIT_VARIANT()),
    var_IN2(CIEC_ANY_BIT_VARIANT()),
    var_OUT(CIEC_ANY_BIT_VARIANT()),
    conn_CNF(this, 0),
    conn_IN1(nullptr),
    conn_IN2(nullptr),
    conn_OUT(this, 0, var_OUT) {
};

void FORTE_F_AND::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      var_OUT = std::visit([](auto &&paIN1, auto&&paIN2) -> CIEC_ANY_BIT_VARIANT {
        return func_AND(paIN1, paIN2);
      }, static_cast<CIEC_ANY_BIT_VARIANT::variant&>(var_IN1), 
         static_cast<CIEC_ANY_BIT_VARIANT::variant&>(var_IN2));
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_F_AND::readInputData(TEventID paEIID) {
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

void FORTE_F_AND::writeOutputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_F_AND::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN1;
    case 1: return &var_IN2;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_AND::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_AND::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_AND::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN1;
    case 1: return &conn_IN2;
  }
  return nullptr;
}

CDataConnection *FORTE_F_AND::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}


