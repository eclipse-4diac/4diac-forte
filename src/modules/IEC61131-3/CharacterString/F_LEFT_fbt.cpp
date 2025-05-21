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

#include "F_LEFT_fbt.h"

USE_STRING_ID(ANY_INT);
USE_STRING_ID(ANY_STRING);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_LEFT);
USE_STRING_ID(IN);
USE_STRING_ID(L);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);

DEFINE_FIRMWARE_FB(FORTE_F_LEFT, STRID(F_LEFT))

const CStringDictionary::TStringId FORTE_F_LEFT::scmDataInputNames[] = {STRID(IN), STRID(L)};

const CStringDictionary::TStringId FORTE_F_LEFT::scmDataInputTypeIds[] = {STRID(ANY_STRING), STRID(ANY_INT)};

const CStringDictionary::TStringId FORTE_F_LEFT::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_F_LEFT::scmDataOutputTypeIds[] = {STRID(ANY_STRING)};

const TDataIOID FORTE_F_LEFT::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_F_LEFT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_LEFT::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_F_LEFT::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_F_LEFT::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_LEFT::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_LEFT::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_F_LEFT::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_F_LEFT::scmFBInterfaceSpec = {1,
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

FORTE_F_LEFT::FORTE_F_LEFT(const CStringDictionary::TStringId paInstanceNameId,
                           forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_IN(CIEC_ANY_STRING_VARIANT()),
    var_L(CIEC_ANY_INT_VARIANT()),
    var_OUT(CIEC_ANY_STRING_VARIANT()),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_L(nullptr),
    conn_OUT(*this, 0, var_OUT) {};

void FORTE_F_LEFT::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID:
      var_OUT = std::visit([](auto &&paIN, auto &&paL) -> CIEC_ANY_STRING_VARIANT { return func_LEFT(paIN, paL); },
                           static_cast<CIEC_ANY_STRING_VARIANT::variant &>(var_IN),
                           static_cast<CIEC_ANY_INT_VARIANT::variant &>(var_L));
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_F_LEFT::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      readData(1, var_L, conn_L);
      break;
    }
    default: break;
  }
}

void FORTE_F_LEFT::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_LEFT::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN;
    case 1: return &var_L;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_LEFT::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_LEFT::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_LEFT::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
    case 1: return &conn_L;
  }
  return nullptr;
}

CDataConnection *FORTE_F_LEFT::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

void FORTE_F_LEFT::setInitialValues() {
  var_IN.reset();
  var_L.reset();
  var_OUT.reset();
}
