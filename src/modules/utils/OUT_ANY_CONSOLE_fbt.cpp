/*******************************************************************************
 * Copyright (c) 2011, 2025 AIT, ACIN, Profactor GmbH, Martin Erich Jobst,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Alois Zoitl, Matthias Plasch
 *                - initial API and implementation and/or initial documentation
 *   Martin Jobst - refactor for ANY variant
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/

#include "OUT_ANY_CONSOLE_fbt.h"
#include "forte_bool.h"

USE_STRING_ID(ANY);
USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(IN);
USE_STRING_ID(LABEL);
USE_STRING_ID(OUT_ANY_CONSOLE);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(REQ);
USE_STRING_ID(STRING);

DEFINE_FIRMWARE_FB(FORTE_OUT_ANY_CONSOLE, STRID(OUT_ANY_CONSOLE))

namespace {
  const auto cDataInputNames = std::array{STRID(QI), STRID(LABEL), STRID(IN)};
  const auto cDataOutputNames = std::array{STRID(QO)};
  const auto cEventInputNames = std::array{STRID(REQ)};
  const auto cEventInputTypeIds = std::array{STRID(Event)};
  const auto cEventOutputNames = std::array{STRID(CNF)};
  const auto cEventOutputTypeIds = std::array{STRID(Event)};

  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = cEventInputTypeIds,
      .mEONames = cEventOutputNames,
      .mEOTypeNames = cEventOutputTypeIds,
      .mDINames = cDataInputNames,
      .mDONames = cDataOutputNames,
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = {},
  };
} // namespace

FORTE_OUT_ANY_CONSOLE::FORTE_OUT_ANY_CONSOLE(const CStringDictionary::TStringId paInstanceNameId,
                                             forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
    var_QI(false_BOOL),
    var_LABEL(""_STRING),
    var_IN(CIEC_ANY_VARIANT()),
    var_QO(false_BOOL),
    conn_CNF(*this, 0),
    conn_QI(nullptr),
    conn_LABEL(nullptr),
    conn_IN(nullptr),
    conn_QO(*this, 0, var_QO) {};

void FORTE_OUT_ANY_CONSOLE::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID:
      var_QO = var_QI;
      if (var_QI) {
        DEVLOG_INFO(" %s = ", var_LABEL.getStorage().c_str());
        var_IN.toString(mDataOutPutBuffer);
        DEVLOG_INFO("%s\n", mDataOutPutBuffer.c_str());
        mDataOutPutBuffer.clear();
      }
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_OUT_ANY_CONSOLE::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_QI, conn_QI);
      readData(2, var_IN, conn_IN);
      readData(1, var_LABEL, conn_LABEL);
      break;
    }
    default: break;
  }
}

void FORTE_OUT_ANY_CONSOLE::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_OUT_ANY_CONSOLE::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_LABEL;
    case 2: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_OUT_ANY_CONSOLE::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
  }
  return nullptr;
}

CEventConnection *FORTE_OUT_ANY_CONSOLE::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_OUT_ANY_CONSOLE::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_LABEL;
    case 2: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_OUT_ANY_CONSOLE::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
  }
  return nullptr;
}

void FORTE_OUT_ANY_CONSOLE::setInitialValues() {
  var_QI = false_BOOL;
  var_LABEL = ""_STRING;
  var_IN.reset();
  var_QO = false_BOOL;
}
