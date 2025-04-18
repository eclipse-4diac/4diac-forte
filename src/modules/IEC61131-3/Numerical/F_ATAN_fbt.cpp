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

#include "F_ATAN_fbt.h"

USE_STRING_ID(ANY_REAL);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_ATAN);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_F_ATAN, STRID(F_ATAN))

const CStringDictionary::TStringId FORTE_F_ATAN::scmDataInputNames[] = {STRID(IN)};

const CStringDictionary::TStringId FORTE_F_ATAN::scmDataInputTypeIds[] = {STRID(ANY_REAL)};

const CStringDictionary::TStringId FORTE_F_ATAN::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_F_ATAN::scmDataOutputTypeIds[] = {STRID(ANY_REAL)};

const TDataIOID FORTE_F_ATAN::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_ATAN::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_ATAN::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_F_ATAN::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_F_ATAN::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_ATAN::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_ATAN::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_F_ATAN::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_F_ATAN::scmFBInterfaceSpec = {1,
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

FORTE_F_ATAN::FORTE_F_ATAN(const CStringDictionary::TStringId paInstanceNameId,
                           forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_IN(CIEC_ANY_REAL_VARIANT()),
    var_OUT(CIEC_ANY_REAL_VARIANT()),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {};

void FORTE_F_ATAN::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID:
      var_OUT = std::visit([](auto &&paIN) -> CIEC_ANY_REAL_VARIANT { return func_ATAN(paIN); },
                           static_cast<CIEC_ANY_REAL_VARIANT::variant &>(var_IN));
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_F_ATAN::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default: break;
  }
}

void FORTE_F_ATAN::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_ATAN::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_ATAN::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_ATAN::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_ATAN::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_ATAN::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}
