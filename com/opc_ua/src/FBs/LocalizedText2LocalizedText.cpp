/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#include "LocalizedText2LocalizedText.h"

using namespace forte::literals;

DEFINE_FIRMWARE_FB(FORTE_LocalizedText2LocalizedText, "LocalizedText2LocalizedText"_STRID)

namespace {
  const auto cDataInputNames = std::array{"IN"_STRID};

  const auto cDataOutputNames = std::array{"OUT"_STRID};

  const auto cEventInputNames = std::array{"REQ"_STRID};
  const auto cEventInputTypeIds = std::array{"Event"_STRID};

  const auto cEventOutputNames = std::array{"CNF"_STRID};
  const auto cEventOutputTypeIds = std::array{"Event"_STRID};

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

FORTE_LocalizedText2LocalizedText::FORTE_LocalizedText2LocalizedText(forte::StringId paInstanceNameId,
                                                                     CFBContainer &paContainer) :
    CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
    var_IN(CIEC_LocalizedText()),
    var_OUT(CIEC_LocalizedText()),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_LocalizedText2LocalizedText::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  if (scmEventREQID == paEIID) {
    var_OUT = var_IN;
    sendOutputEvent(scmEventCNFID, paECET);
  }
}

void FORTE_LocalizedText2LocalizedText::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default: break;
  }
}

void FORTE_LocalizedText2LocalizedText::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_LocalizedText2LocalizedText::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_LocalizedText2LocalizedText::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_LocalizedText2LocalizedText::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_LocalizedText2LocalizedText::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_LocalizedText2LocalizedText::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

void FORTE_LocalizedText2LocalizedText::setInitialValues() {
  var_IN = CIEC_LocalizedText();
  var_OUT = CIEC_LocalizedText();
}
