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

#include "F_OR_fbt.h"

USE_STRING_ID(ANY_BIT);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_OR);
USE_STRING_ID(IN1);
USE_STRING_ID(IN2);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);

DEFINE_FIRMWARE_FB(FORTE_F_OR, STRID(F_OR))

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


FORTE_F_OR::FORTE_F_OR(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
    var_IN1(CIEC_ANY_BIT_VARIANT()),
    var_IN2(CIEC_ANY_BIT_VARIANT()),
    var_OUT(CIEC_ANY_BIT_VARIANT()),
    conn_CNF(*this, 0),
    conn_IN1(nullptr),
    conn_IN2(nullptr),
    conn_OUT(*this, 0, var_OUT) {};

void FORTE_F_OR::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID:
      var_OUT = std::visit([](auto &&paIN1, auto &&paIN2) -> CIEC_ANY_BIT_VARIANT { return func_OR(paIN1, paIN2); },
                           static_cast<CIEC_ANY_BIT_VARIANT::variant &>(var_IN1),
                           static_cast<CIEC_ANY_BIT_VARIANT::variant &>(var_IN2));
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_F_OR::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN1, conn_IN1);
      readData(1, var_IN2, conn_IN2);
      break;
    }
    default: break;
  }
}

void FORTE_F_OR::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_OR::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN1;
    case 1: return &var_IN2;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_OR::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_OR::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_OR::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN1;
    case 1: return &conn_IN2;
  }
  return nullptr;
}

CDataConnection *FORTE_F_OR::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

void FORTE_F_OR::setInitialValues() {
  var_IN1.reset();
  var_IN2.reset();
  var_OUT.reset();
}
