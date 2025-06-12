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

#include "F_ROR_fbt.h"

USE_STRING_ID(ANY_BIT);
USE_STRING_ID(ANY_INT);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_ROR);
USE_STRING_ID(IN);
USE_STRING_ID(N);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);

DEFINE_FIRMWARE_FB(FORTE_F_ROR, STRID(F_ROR))

namespace {
  const auto cDataInputNames = std::array{STRID(IN), STRID(N)};
  
  
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


FORTE_F_ROR::FORTE_F_ROR(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
    var_IN(CIEC_ANY_BIT_VARIANT()),
    var_N(CIEC_ANY_INT_VARIANT()),
    var_OUT(CIEC_ANY_BIT_VARIANT()),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_N(nullptr),
    conn_OUT(*this, 0, var_OUT) {};

void FORTE_F_ROR::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID:
      var_OUT = std::visit(
          [](auto &&paIN, auto &&paN) -> CIEC_ANY_BIT_VARIANT {
            using T = std::decay_t<decltype(paIN)>;
            if constexpr (!std::is_same<T, CIEC_BOOL>::value) {
              return func_ROR(paIN, paN);
            }
            DEVLOG_ERROR("Rotating right incompatible types %s and %s\n", CStringDictionary::get(paIN.getTypeNameID()),
                         CStringDictionary::get(paN.getTypeNameID()));
            return CIEC_ANY_BIT_VARIANT();
          },
          static_cast<CIEC_ANY_BIT_VARIANT::variant &>(var_IN), static_cast<CIEC_ANY_INT_VARIANT::variant &>(var_N));
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_F_ROR::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      readData(1, var_N, conn_N);
      break;
    }
    default: break;
  }
}

void FORTE_F_ROR::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_ROR::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN;
    case 1: return &var_N;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_ROR::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_ROR::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_ROR::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
    case 1: return &conn_N;
  }
  return nullptr;
}

CDataConnection *FORTE_F_ROR::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

void FORTE_F_ROR::setInitialValues() {
  var_IN.reset();
  var_N.reset();
  var_OUT.reset();
}
