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

#include "F_REPLACE_fbt.h"

USE_STRING_ID(ANY_INT);
USE_STRING_ID(ANY_STRING);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_REPLACE);
USE_STRING_ID(IN1);
USE_STRING_ID(IN2);
USE_STRING_ID(L);
USE_STRING_ID(OUT);
USE_STRING_ID(P);
USE_STRING_ID(REQ);

DEFINE_FIRMWARE_FB(FORTE_F_REPLACE, STRID(F_REPLACE))

namespace {
  const auto cDataInputNames = std::array{STRID(IN1), STRID(IN2), STRID(L), STRID(P)};
  const auto cDataOutputNames = std::array{STRID(OUT)};
  const auto cEventInputNames = std::array{STRID(REQ)};
  const auto cEventOutputNames = std::array{STRID(CNF)};
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
} // namespace

FORTE_F_REPLACE::FORTE_F_REPLACE(const CStringDictionary::TStringId paInstanceNameId,
                                 forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
    var_IN1(CIEC_ANY_STRING_VARIANT()),
    var_IN2(CIEC_ANY_STRING_VARIANT()),
    var_L(CIEC_ANY_INT_VARIANT()),
    var_P(CIEC_ANY_INT_VARIANT()),
    var_OUT(CIEC_ANY_STRING_VARIANT()),
    conn_CNF(*this, 0),
    conn_IN1(nullptr),
    conn_IN2(nullptr),
    conn_L(nullptr),
    conn_P(nullptr),
    conn_OUT(*this, 0, var_OUT) {};

void FORTE_F_REPLACE::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID:
      var_OUT = std::visit(
          [](auto &&paIN1, auto &&paIN2, auto &&paP, auto &&paL) -> CIEC_ANY_STRING_VARIANT {
            using T = std::decay_t<decltype(paIN1)>;
            using U = std::decay_t<decltype(paIN2)>;
            if constexpr (std::is_same_v<T, U>) {
              return func_REPLACE(paIN1, paIN2, paP, paL);
            }
            DEVLOG_ERROR("Replacing incompatible types %s and %s\n", CStringDictionary::get(paIN1.getTypeNameID()),
                         CStringDictionary::get(paIN2.getTypeNameID()));
            return CIEC_ANY_STRING_VARIANT();
          },
          static_cast<CIEC_ANY_STRING_VARIANT::variant &>(var_IN1),
          static_cast<CIEC_ANY_STRING_VARIANT::variant &>(var_IN2), static_cast<CIEC_ANY_INT_VARIANT::variant &>(var_P),
          static_cast<CIEC_ANY_INT_VARIANT::variant &>(var_L));
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_F_REPLACE::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN1, conn_IN1);
      readData(1, var_IN2, conn_IN2);
      readData(2, var_L, conn_L);
      readData(3, var_P, conn_P);
      break;
    }
    default: break;
  }
}

void FORTE_F_REPLACE::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_REPLACE::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN1;
    case 1: return &var_IN2;
    case 2: return &var_L;
    case 3: return &var_P;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_REPLACE::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_REPLACE::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_REPLACE::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN1;
    case 1: return &conn_IN2;
    case 2: return &conn_L;
    case 3: return &conn_P;
  }
  return nullptr;
}

CDataConnection *FORTE_F_REPLACE::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

void FORTE_F_REPLACE::setInitialValues() {
  var_IN1.reset();
  var_IN2.reset();
  var_L.reset();
  var_P.reset();
  var_OUT.reset();
}
