/*******************************************************************************
 * Copyright (c) 2011 - 2013 ACIN, fortiss GmbH
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#include "F_DIV_fbt.h"

USE_STRING_ID(ANY_NUM);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_DIV);
USE_STRING_ID(IN1);
USE_STRING_ID(IN2);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);


DEFINE_FIRMWARE_FB(FORTE_F_DIV, STRID(F_DIV))

const CStringDictionary::TStringId FORTE_F_DIV::scmDataInputNames[] = {STRID(IN1), STRID(IN2)};

const CStringDictionary::TStringId FORTE_F_DIV::scmDataInputTypeIds[] = {STRID(ANY_NUM), STRID(ANY_NUM)};

const CStringDictionary::TStringId FORTE_F_DIV::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_F_DIV::scmDataOutputTypeIds[] = {STRID(ANY_NUM)};

const TDataIOID FORTE_F_DIV::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_F_DIV::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_DIV::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_F_DIV::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_F_DIV::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_DIV::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_DIV::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_F_DIV::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_F_DIV::scmFBInterfaceSpec = {1,
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

FORTE_F_DIV::FORTE_F_DIV(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_IN1(CIEC_ANY_NUM_VARIANT()),
    var_IN2(CIEC_ANY_NUM_VARIANT()),
    var_OUT(CIEC_ANY_NUM_VARIANT()),
    conn_CNF(*this, 0),
    conn_IN1(nullptr),
    conn_IN2(nullptr),
    conn_OUT(*this, 0, var_OUT) {};

void FORTE_F_DIV::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID:
      var_OUT = std::visit(
          [](auto &&paIN1, auto &&paIN2) -> CIEC_ANY_NUM_VARIANT {
            using T = std::decay_t<decltype(paIN1)>;
            using U = std::decay_t<decltype(paIN2)>;
            using deductedType = typename forte::core::mpl::get_div_operator_result_type<T, U>::type;
            if constexpr (!std::is_same<deductedType, forte::core::mpl::NullType>::value) {
              return func_DIV(paIN1, paIN2);
            }
            DEVLOG_ERROR("Dividing incompatible types %s and %s\n", CStringDictionary::get(paIN1.getTypeNameID()),
                         CStringDictionary::get(paIN2.getTypeNameID()));
            return CIEC_ANY_NUM_VARIANT();
          },
          static_cast<CIEC_ANY_NUM_VARIANT::variant &>(var_IN1), static_cast<CIEC_ANY_NUM_VARIANT::variant &>(var_IN2));
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_F_DIV::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN1, conn_IN1);
      readData(1, var_IN2, conn_IN2);
      break;
    }
    default: break;
  }
}

void FORTE_F_DIV::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_DIV::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN1;
    case 1: return &var_IN2;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_DIV::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_DIV::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_DIV::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN1;
    case 1: return &conn_IN2;
  }
  return nullptr;
}

CDataConnection *FORTE_F_DIV::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}
