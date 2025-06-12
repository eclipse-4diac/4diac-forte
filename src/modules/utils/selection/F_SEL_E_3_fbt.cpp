/*************************************************************************
 *** Copyright (c) 2012, 2022 TU Wien ACIN, HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: F_SEL_E_3
 *** Description: selection event-based selection)
 *** Version:
 ***     1.0: 2012-03-25/Monika Wenger - TU Wien ACIN -
 ***     1.1: 2022-08-04/Franz Höpfinger - HR Agrartechnik GmbH -
 *************************************************************************/

#include "F_SEL_E_3_fbt.h"

USE_STRING_ID(ANY);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_SEL_E_3);
USE_STRING_ID(IN0);
USE_STRING_ID(IN1);
USE_STRING_ID(IN2);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ0);
USE_STRING_ID(REQ1);
USE_STRING_ID(REQ2);

DEFINE_FIRMWARE_FB(FORTE_F_SEL_E_3, STRID(F_SEL_E_3))

namespace {
  const auto cDataInputNames = std::array{STRID(IN0), STRID(IN1), STRID(IN2)};
  const auto cDataOutputNames = std::array{STRID(OUT)};
  const auto cEventInputNames = std::array{STRID(REQ0), STRID(REQ1), STRID(REQ2)};
  const auto cEventInputTypeIds = std::array{STRID(Event), STRID(Event), STRID(Event)};
  const auto cEventOutputNames = std::array{STRID(CNF)};
  const auto cEventOutputTypeIds = std::array{STRID(Event)};
  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = cEventInputTypeIds,
      .mEONames = cEventOutputNames,
      .mEOTypeNames = cEventOutputTypeIds,
      .mDINames = cDataInputNames,
      .mDONames = cDataOutputNames,
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = {},
  };
} // namespace

FORTE_F_SEL_E_3::FORTE_F_SEL_E_3(const CStringDictionary::TStringId paInstanceNameId,
                                 forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
    conn_CNF(*this, 0),
    conn_IN0(nullptr),
    conn_IN1(nullptr),
    conn_IN2(nullptr),
    conn_OUT(*this, 0, var_OUT) {};

void FORTE_F_SEL_E_3::setInitialValues() {
  var_IN0.reset();
  var_IN1.reset();
  var_IN2.reset();
  var_OUT.reset();
}

void FORTE_F_SEL_E_3::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQ0ID:
      var_OUT = var_IN0;
      sendOutputEvent(scmEventCNFID, paECET);
      break;
    case scmEventREQ1ID:
      var_OUT = var_IN1;
      sendOutputEvent(scmEventCNFID, paECET);
      break;
    case scmEventREQ2ID:
      var_OUT = var_IN2;
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_F_SEL_E_3::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQ0ID: {

      readData(0, var_IN0, conn_IN0);
      break;
    }
    case scmEventREQ1ID: {

      readData(1, var_IN1, conn_IN1);
      break;
    }
    case scmEventREQ2ID: {

      readData(2, var_IN2, conn_IN2);
      break;
    }
    default: break;
  }
}

void FORTE_F_SEL_E_3::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {

      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_SEL_E_3::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN0;
    case 1: return &var_IN1;
    case 2: return &var_IN2;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_SEL_E_3::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_SEL_E_3::getDIO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_F_SEL_E_3::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_SEL_E_3::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN0;
    case 1: return &conn_IN1;
    case 2: return &conn_IN2;
  }
  return nullptr;
}

CDataConnection *FORTE_F_SEL_E_3::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CInOutDataConnection **FORTE_F_SEL_E_3::getDIOInConUnchecked(TPortId) {
  return nullptr;
}

CInOutDataConnection *FORTE_F_SEL_E_3::getDIOOutConUnchecked(TPortId) {
  return nullptr;
}
