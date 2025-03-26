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

#include "F_TRUNC_fbt.h"

USE_STRING_ID(ANY_INT);
USE_STRING_ID(ANY_REAL);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_TRUNC);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);


#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_F_TRUNC, STRID(F_TRUNC))

const CStringDictionary::TStringId FORTE_F_TRUNC::scmDataInputNames[] = {STRID(IN)};

const CStringDictionary::TStringId FORTE_F_TRUNC::scmDataInputTypeIds[] = {STRID(ANY_REAL)};

const CStringDictionary::TStringId FORTE_F_TRUNC::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_F_TRUNC::scmDataOutputTypeIds[] = {STRID(ANY_INT)};

const TDataIOID FORTE_F_TRUNC::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_TRUNC::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_TRUNC::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_F_TRUNC::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_F_TRUNC::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_TRUNC::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_TRUNC::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_F_TRUNC::scmEventOutputTypeIds[] = {STRID(Event)};


const SFBInterfaceSpec FORTE_F_TRUNC::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_F_TRUNC::FORTE_F_TRUNC(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_IN(CIEC_ANY_REAL_VARIANT()),
    var_OUT(CIEC_ANY_INT_VARIANT()),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
};

void FORTE_F_TRUNC::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      std::visit([](auto &&paIN, auto &&paOUT) -> void {
        using T = std::decay_t<decltype(paOUT)>;
        paOUT = func_TRUNC<T>(paIN);
      }, static_cast<CIEC_ANY_REAL_VARIANT::variant&>(var_IN),
         static_cast<CIEC_ANY_INT_VARIANT::variant&>(var_OUT));
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_F_TRUNC::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_TRUNC::writeOutputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_F_TRUNC::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_TRUNC::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_TRUNC::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_TRUNC::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_TRUNC::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}


