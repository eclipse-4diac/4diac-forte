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

#include "F_LEN_fbt.h"

USE_STRING_ID(ANY_INT);
USE_STRING_ID(ANY_STRING);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_LEN);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);


DEFINE_FIRMWARE_FB(FORTE_F_LEN, STRID(F_LEN))

const CStringDictionary::TStringId FORTE_F_LEN::scmDataInputNames[] = {STRID(IN)};

const CStringDictionary::TStringId FORTE_F_LEN::scmDataInputTypeIds[] = {STRID(ANY_STRING)};

const CStringDictionary::TStringId FORTE_F_LEN::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_F_LEN::scmDataOutputTypeIds[] = {STRID(ANY_INT)};

const TDataIOID FORTE_F_LEN::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_LEN::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_LEN::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_F_LEN::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_F_LEN::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_LEN::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_LEN::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_F_LEN::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_F_LEN::scmFBInterfaceSpec = {1,
                                                          scmEventInputNames,
                                                          scmEventInputTypeIds,
                                                          scmEIWith,
                                                          scmEIWithIndexes,
                                                          1,
                                                          scmEventOutputNames,
                                                          scmEventOutputTypeIds,
                                                          scmEOWith,
                                                          scmEOWithIndexes,
                                                          1,
                                                          scmDataInputNames,
                                                          scmDataInputTypeIds,
                                                          1,
                                                          scmDataOutputNames,
                                                          scmDataOutputTypeIds,
                                                          0,
                                                          nullptr,
                                                          0,
                                                          nullptr};

FORTE_F_LEN::FORTE_F_LEN(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_IN(CIEC_ANY_STRING_VARIANT()),
    var_OUT(CIEC_ANY_INT_VARIANT()),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {};

void FORTE_F_LEN::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID:
      std::visit(
          [](auto &&paIN, auto &&paOUT) -> void { paOUT = func_LEN<std::remove_reference_t<decltype(paOUT)>>(paIN); },
          static_cast<CIEC_ANY_STRING_VARIANT::variant &>(var_IN),
          static_cast<CIEC_ANY_INT_VARIANT::variant &>(var_OUT));
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_F_LEN::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default: break;
  }
}

void FORTE_F_LEN::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_LEN::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_LEN::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_LEN::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_LEN::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_LEN::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}
