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
 *   Monika Wenger, Alois Zoitl Ingo Hengy
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#include "F_XOR_fbt.h"

USE_STRING_ID(ANY_BIT);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_XOR);
USE_STRING_ID(IN1);
USE_STRING_ID(IN2);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);

DEFINE_FIRMWARE_FB(FORTE_F_XOR, STRID(F_XOR))

const CStringDictionary::TStringId FORTE_F_XOR::scmDataInputNames[] = {STRID(IN1), STRID(IN2)};

const CStringDictionary::TStringId FORTE_F_XOR::scmDataInputTypeIds[] = {STRID(ANY_BIT), STRID(ANY_BIT)};

const CStringDictionary::TStringId FORTE_F_XOR::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_F_XOR::scmDataOutputTypeIds[] = {STRID(ANY_BIT)};

const TDataIOID FORTE_F_XOR::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_F_XOR::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_XOR::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_F_XOR::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_F_XOR::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_XOR::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_XOR::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_F_XOR::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_F_XOR::scmFBInterfaceSpec = {1,
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

FORTE_F_XOR::FORTE_F_XOR(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_IN1(CIEC_ANY_BIT_VARIANT()),
    var_IN2(CIEC_ANY_BIT_VARIANT()),
    var_OUT(CIEC_ANY_BIT_VARIANT()),
    conn_CNF(*this, 0),
    conn_IN1(nullptr),
    conn_IN2(nullptr),
    conn_OUT(*this, 0, var_OUT) {};

void FORTE_F_XOR::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID:
      var_OUT = std::visit([](auto &&paIN1, auto &&paIN2) -> CIEC_ANY_BIT_VARIANT { return func_XOR(paIN1, paIN2); },
                           static_cast<CIEC_ANY_BIT_VARIANT::variant &>(var_IN1),
                           static_cast<CIEC_ANY_BIT_VARIANT::variant &>(var_IN2));
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_F_XOR::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN1, conn_IN1);
      readData(1, var_IN2, conn_IN2);
      break;
    }
    default: break;
  }
}

void FORTE_F_XOR::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_XOR::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN1;
    case 1: return &var_IN2;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_XOR::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_XOR::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_XOR::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN1;
    case 1: return &conn_IN2;
  }
  return nullptr;
}

CDataConnection *FORTE_F_XOR::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

void FORTE_F_XOR::setInitialValues() {
  var_IN1 = CIEC_ANY_BIT_VARIANT();
  var_IN2 = CIEC_ANY_BIT_VARIANT();
  var_OUT = CIEC_ANY_BIT_VARIANT();
}
