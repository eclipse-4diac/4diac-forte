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

#include "F_EXPT_fbt.h"

USE_STRING_ID(ANY_REAL);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_EXPT);
USE_STRING_ID(IN1);
USE_STRING_ID(IN2);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);


#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_F_EXPT, STRID(F_EXPT))

const CStringDictionary::TStringId FORTE_F_EXPT::scmDataInputNames[] = {STRID(IN1), STRID(IN2)};

const CStringDictionary::TStringId FORTE_F_EXPT::scmDataInputTypeIds[] = {STRID(ANY_REAL), STRID(ANY_REAL)};

const CStringDictionary::TStringId FORTE_F_EXPT::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_F_EXPT::scmDataOutputTypeIds[] = {STRID(ANY_REAL)};

const TDataIOID FORTE_F_EXPT::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_F_EXPT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_EXPT::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_F_EXPT::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_F_EXPT::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_EXPT::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_EXPT::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_F_EXPT::scmEventOutputTypeIds[] = {STRID(Event)};


const SFBInterfaceSpec FORTE_F_EXPT::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  2, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_F_EXPT::FORTE_F_EXPT(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_IN1(CIEC_ANY_REAL_VARIANT()),
    var_IN2(CIEC_ANY_REAL_VARIANT()),
    var_OUT(CIEC_ANY_REAL_VARIANT()),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN1(nullptr),
    conn_IN2(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
};

void FORTE_F_EXPT::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      var_OUT = std::visit([](auto &&paIN1, auto&&paIN2) -> CIEC_ANY_REAL_VARIANT {
        return func_EXPT(paIN1, paIN2);
      }, static_cast<CIEC_ANY_REAL_VARIANT::variant&>(var_IN1), 
         static_cast<CIEC_ANY_REAL_VARIANT::variant&>(var_IN2));
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_F_EXPT::readInputData(TEventID paEIID) {
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

void FORTE_F_EXPT::writeOutputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_F_EXPT::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN1;
    case 1: return &var_IN2;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_EXPT::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_EXPT::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_EXPT::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN1;
    case 1: return &conn_IN2;
  }
  return nullptr;
}

CDataConnection *FORTE_F_EXPT::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}


