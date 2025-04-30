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

#include "F_ROL_fbt.h"

USE_STRING_ID(ANY_BIT);
USE_STRING_ID(ANY_INT);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_ROL);
USE_STRING_ID(IN);
USE_STRING_ID(N);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);


DEFINE_FIRMWARE_FB(FORTE_F_ROL, STRID(F_ROL))

const CStringDictionary::TStringId FORTE_F_ROL::scmDataInputNames[] = {STRID(IN), STRID(N)};

const CStringDictionary::TStringId FORTE_F_ROL::scmDataInputTypeIds[] = {STRID(ANY_BIT), STRID(ANY_INT)};

const CStringDictionary::TStringId FORTE_F_ROL::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_F_ROL::scmDataOutputTypeIds[] = {STRID(ANY_BIT)};

const TDataIOID FORTE_F_ROL::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_F_ROL::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_ROL::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_F_ROL::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_F_ROL::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_ROL::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_ROL::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_F_ROL::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_F_ROL::scmFBInterfaceSpec = {1,
                                                          scmEventInputNames,
                                                          scmEventInputTypeIds,
                                                          scmEIWith,
                                                          scmEIWithIndexes,
                                                          1,
                                                          scmEventOutputNames,
                                                          scmEventOutputTypeIds,
                                                          scmEOWith,
                                                          scmEOWithIndexes,
                                                          2,
                                                          scmDataInputNames,
                                                          scmDataInputTypeIds,
                                                          1,
                                                          scmDataOutputNames,
                                                          scmDataOutputTypeIds,
                                                          0,
                                                          nullptr,
                                                          0,
                                                          nullptr};

FORTE_F_ROL::FORTE_F_ROL(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_IN(CIEC_ANY_BIT_VARIANT()),
    var_N(CIEC_ANY_INT_VARIANT()),
    var_OUT(CIEC_ANY_BIT_VARIANT()),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_N(nullptr),
    conn_OUT(*this, 0, var_OUT) {};

void FORTE_F_ROL::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID:
      var_OUT = std::visit(
          [](auto &&paIN, auto &&paN) -> CIEC_ANY_BIT_VARIANT {
            using T = std::decay_t<decltype(paIN)>;
            if constexpr (!std::is_same<T, CIEC_BOOL>::value) {
              return func_ROL(paIN, paN);
            }
            DEVLOG_ERROR("Rotating left incompatible types %s and %s\n", CStringDictionary::get(paIN.getTypeNameID()),
                         CStringDictionary::get(paN.getTypeNameID()));
            return CIEC_ANY_BIT_VARIANT();
          },
          static_cast<CIEC_ANY_BIT_VARIANT::variant &>(var_IN), static_cast<CIEC_ANY_INT_VARIANT::variant &>(var_N));
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_F_ROL::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      readData(1, var_N, conn_N);
      break;
    }
    default: break;
  }
}

void FORTE_F_ROL::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_ROL::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN;
    case 1: return &var_N;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_ROL::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_ROL::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_ROL::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
    case 1: return &conn_N;
  }
  return nullptr;
}

CDataConnection *FORTE_F_ROL::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}
