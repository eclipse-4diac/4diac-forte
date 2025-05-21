/*******************************************************************************
 * Copyright (c) 2013 ACIN, fortiss GmbH
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

#include "F_SIN_fbt.h"

USE_STRING_ID(ANY_REAL);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_SIN);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);

DEFINE_FIRMWARE_FB(FORTE_F_SIN, STRID(F_SIN))

const CStringDictionary::TStringId FORTE_F_SIN::scmDataInputNames[] = {STRID(IN)};

const CStringDictionary::TStringId FORTE_F_SIN::scmDataInputTypeIds[] = {STRID(ANY_REAL)};

const CStringDictionary::TStringId FORTE_F_SIN::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_F_SIN::scmDataOutputTypeIds[] = {STRID(ANY_REAL)};

const TDataIOID FORTE_F_SIN::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_SIN::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_SIN::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_F_SIN::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_F_SIN::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_SIN::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_SIN::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_F_SIN::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_F_SIN::scmFBInterfaceSpec = {1,
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

FORTE_F_SIN::FORTE_F_SIN(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_IN(CIEC_ANY_REAL_VARIANT()),
    var_OUT(CIEC_ANY_REAL_VARIANT()),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {};

void FORTE_F_SIN::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID:
      var_OUT = std::visit([](auto &&paIN) -> CIEC_ANY_REAL_VARIANT { return func_SIN(paIN); },
                           static_cast<CIEC_ANY_REAL_VARIANT::variant &>(var_IN));
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_F_SIN::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default: break;
  }
}

void FORTE_F_SIN::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_SIN::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_SIN::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_SIN::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_SIN::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_SIN::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

void FORTE_F_SIN::setInitialValues() {
  var_IN.reset();
  var_OUT.reset();
}
