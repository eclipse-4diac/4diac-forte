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

namespace {
  const auto cDataInputNames = std::array{STRID(MN), STRID(IN), STRID(MX)};
  const auto cDataOutputNames = std::array{STRID(OUT)};
  const auto cEventInputNames = std::array{STRID(REQ)};
  const auto cEventOutputNames = std::array{STRID(CNF)};

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
} // namespace

FORTE_F_LIMIT::FORTE_F_LIMIT(const CStringDictionary::TStringId paInstanceNameId,
                             forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
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
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_OUT, conn_OUT);
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
