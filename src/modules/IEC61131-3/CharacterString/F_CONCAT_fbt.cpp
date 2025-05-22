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

#include "F_CONCAT_fbt.h"

USE_STRING_ID(ANY_STRING);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_CONCAT);
USE_STRING_ID(IN1);
USE_STRING_ID(IN2);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);

DEFINE_FIRMWARE_FB(FORTE_F_CONCAT, STRID(F_CONCAT))

const CStringDictionary::TStringId FORTE_F_CONCAT::scmDataInputNames[] = {STRID(IN1), STRID(IN2)};

const CStringDictionary::TStringId FORTE_F_CONCAT::scmDataInputTypeIds[] = {STRID(ANY_STRING), STRID(ANY_STRING)};

const CStringDictionary::TStringId FORTE_F_CONCAT::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_F_CONCAT::scmDataOutputTypeIds[] = {STRID(ANY_STRING)};

const TDataIOID FORTE_F_CONCAT::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_F_CONCAT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_CONCAT::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_F_CONCAT::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_F_CONCAT::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_CONCAT::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_CONCAT::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_F_CONCAT::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_F_CONCAT::scmFBInterfaceSpec = {1,
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

FORTE_F_CONCAT::FORTE_F_CONCAT(const CStringDictionary::TStringId paInstanceNameId,
                               forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_IN1(CIEC_ANY_STRING_VARIANT()),
    var_IN2(CIEC_ANY_STRING_VARIANT()),
    var_OUT(CIEC_ANY_STRING_VARIANT()),
    conn_CNF(*this, 0),
    conn_IN1(nullptr),
    conn_IN2(nullptr),
    conn_OUT(*this, 0, var_OUT) {};

void FORTE_F_CONCAT::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID:
      var_OUT = std::visit(
          [](auto &&paIN1, auto &&paIN2) -> CIEC_ANY_STRING_VARIANT {
            using T = std::decay_t<decltype(paIN1)>;
            using U = std::decay_t<decltype(paIN2)>;
            if constexpr (std::is_same_v<T, U>) {
              return func_CONCAT(paIN1, paIN2);
            }
            DEVLOG_ERROR("Concatenating incompatible types %s and %s\n", CStringDictionary::get(paIN1.getTypeNameID()),
                         CStringDictionary::get(paIN2.getTypeNameID()));
            return CIEC_ANY_STRING_VARIANT();
          },
          static_cast<CIEC_ANY_STRING_VARIANT::variant &>(var_IN1),
          static_cast<CIEC_ANY_STRING_VARIANT::variant &>(var_IN2));
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_F_CONCAT::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN1, conn_IN1);
      readData(1, var_IN2, conn_IN2);
      break;
    }
    default: break;
  }
}

void FORTE_F_CONCAT::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_CONCAT::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN1;
    case 1: return &var_IN2;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_CONCAT::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_CONCAT::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_CONCAT::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN1;
    case 1: return &conn_IN2;
  }
  return nullptr;
}

CDataConnection *FORTE_F_CONCAT::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

void FORTE_F_CONCAT::setInitialValues() {
  var_IN1.reset();
  var_IN2.reset();
  var_OUT.reset();
}
