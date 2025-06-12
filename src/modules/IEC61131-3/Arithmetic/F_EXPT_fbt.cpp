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

DEFINE_FIRMWARE_FB(FORTE_F_EXPT, STRID(F_EXPT))

namespace {
  const auto cDataInputNames = std::array{STRID(IN1), STRID(IN2)};
  
  
  const auto cDataOutputNames = std::array{STRID(OUT)};
  
  
  const auto cEventInputNames = std::array{STRID(REQ)};
  const auto cEventInputTypeIds = std::array{STRID(Event)};
  
  const auto cEventOutputNames = std::array{STRID(CNF)};
  const auto cEventOutputTypeIds = std::array{STRID(Event)};
  
  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = {},
      .mEONames = cEventOutputNames,
      .mEOTypeNames = {},
      .mDINames = cDataInputNames,
      .mDONames = cDataOutputNames,
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = {},
  };
}


FORTE_F_EXPT::FORTE_F_EXPT(const CStringDictionary::TStringId paInstanceNameId,
                           forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
    var_IN1(CIEC_ANY_REAL_VARIANT()),
    var_IN2(CIEC_ANY_REAL_VARIANT()),
    var_OUT(CIEC_ANY_REAL_VARIANT()),
    conn_CNF(*this, 0),
    conn_IN1(nullptr),
    conn_IN2(nullptr),
    conn_OUT(*this, 0, var_OUT) {};

void FORTE_F_EXPT::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID:
      var_OUT = std::visit([](auto &&paIN1, auto &&paIN2) -> CIEC_ANY_REAL_VARIANT { return func_EXPT(paIN1, paIN2); },
                           static_cast<CIEC_ANY_REAL_VARIANT::variant &>(var_IN1),
                           static_cast<CIEC_ANY_REAL_VARIANT::variant &>(var_IN2));
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_F_EXPT::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN1, conn_IN1);
      readData(1, var_IN2, conn_IN2);
      break;
    }
    default: break;
  }
}

void FORTE_F_EXPT::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_EXPT::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN1;
    case 1: return &var_IN2;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_EXPT::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_EXPT::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_EXPT::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN1;
    case 1: return &conn_IN2;
  }
  return nullptr;
}

CDataConnection *FORTE_F_EXPT::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

void FORTE_F_EXPT::setInitialValues() {
  var_IN1.reset();
  var_IN2.reset();
  var_OUT.reset();
}
