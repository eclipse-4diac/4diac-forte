/*******************************************************************************
 * Copyright (c) 2011 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl, Gerhard Ebenhofer
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#include "F_LIMIT_fbt.h"

USE_STRING_ID(ANY_ELEMENTARY);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_LIMIT);
USE_STRING_ID(IN);
USE_STRING_ID(MN);
USE_STRING_ID(MX);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);

DEFINE_FIRMWARE_FB(FORTE_F_LIMIT, STRID(F_LIMIT))

const CStringDictionary::TStringId FORTE_F_LIMIT::scmDataInputNames[] = {STRID(MN), STRID(IN), STRID(MX)};

const CStringDictionary::TStringId FORTE_F_LIMIT::scmDataInputTypeIds[] = {STRID(ANY_ELEMENTARY), STRID(ANY_ELEMENTARY),
                                                                           STRID(ANY_ELEMENTARY)};

const CStringDictionary::TStringId FORTE_F_LIMIT::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_F_LIMIT::scmDataOutputTypeIds[] = {STRID(ANY_ELEMENTARY)};

const TDataIOID FORTE_F_LIMIT::scmEIWith[] = {0, 2, 1, scmWithListDelimiter};
const TForteInt16 FORTE_F_LIMIT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_LIMIT::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_F_LIMIT::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_F_LIMIT::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_LIMIT::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_LIMIT::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_F_LIMIT::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_F_LIMIT::scmFBInterfaceSpec = {1,
                                                            scmEventInputNames,
                                                            scmEventInputTypeIds,
                                                            scmEIWith,
                                                            scmEIWithIndexes,
                                                            1,
                                                            scmEventOutputNames,
                                                            scmEventOutputTypeIds,
                                                            scmEOWith,
                                                            scmEOWithIndexes,
                                                            3,
                                                            scmDataInputNames,
                                                            scmDataInputTypeIds,
                                                            1,
                                                            scmDataOutputNames,
                                                            scmDataOutputTypeIds,
                                                            0,
                                                            nullptr,
                                                            0,
                                                            nullptr};

FORTE_F_LIMIT::FORTE_F_LIMIT(const CStringDictionary::TStringId paInstanceNameId,
                             forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_MN(CIEC_ANY_ELEMENTARY_VARIANT()),
    var_IN(CIEC_ANY_ELEMENTARY_VARIANT()),
    var_MX(CIEC_ANY_ELEMENTARY_VARIANT()),
    var_OUT(CIEC_ANY_ELEMENTARY_VARIANT()),
    conn_CNF(*this, 0),
    conn_MN(nullptr),
    conn_IN(nullptr),
    conn_MX(nullptr),
    conn_OUT(*this, 0, var_OUT) {};

void FORTE_F_LIMIT::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID:
      var_OUT = var_IN <= var_MX ? (var_IN >= var_MN ? var_IN : var_MN) : var_MX;
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_F_LIMIT::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_MN, conn_MN);
      readData(2, var_MX, conn_MX);
      readData(1, var_IN, conn_IN);
      break;
    }
    default: break;
  }
}

void FORTE_F_LIMIT::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_LIMIT::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_MN;
    case 1: return &var_IN;
    case 2: return &var_MX;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_LIMIT::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_LIMIT::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_LIMIT::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_MN;
    case 1: return &conn_IN;
    case 2: return &conn_MX;
  }
  return nullptr;
}

CDataConnection *FORTE_F_LIMIT::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

void FORTE_F_LIMIT::setInitialValues() {
  var_MN.reset();
  var_IN.reset();
  var_MX.reset();
  var_OUT.reset();
}
