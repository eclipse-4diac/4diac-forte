/*******************************************************************************
 * Copyright (c) 2014, 2025 fortiss GmbH, HR Agrartechnik GmbH,
 *                          Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Waldemar Eisenmenger, Monika Wenger
 *                    - initial API and implementation and/or initial documentation
 *   Franz Hoepfinger - copied over IX to IE, and removed the IN
 *   Alois Zoitl      - copied to core/io and adjusted to core/io process
 *                      interface needs
 *******************************************************************************/

#include "core/io/IE_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(IE);
USE_STRING_ID(IND);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(PARAMS);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(REQ);
USE_STRING_ID(STATUS);
USE_STRING_ID(STRING);

using namespace forte::core::io;

namespace {
  const auto cEventInputNames = std::array{STRID(INIT), STRID(REQ)};
  const auto cEventInputTypeIds = std::array{STRID(EInit), STRID(Event)};
  const auto cEventOutputNames = std::array{STRID(INITO), STRID(CNF), STRID(IND)};
  const auto cEventOutputTypeIds = std::array{STRID(EInit), STRID(Event), STRID(Event)};
  const auto cDataInputNames = std::array{STRID(QI), STRID(PARAMS)};
  const auto cDataOutputNames = std::array{STRID(QO), STRID(STATUS)};

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

DEFINE_FIRMWARE_FB(FORTE_IE, STRID(IE))

FORTE_IE::FORTE_IE(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CProcessInterfaceFB(paContainer, cFBInterfaceSpec, paInstanceNameId),
    conn_IND(*this, 2) {};

void FORTE_IE::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case cgExternalEventID: sendOutputEvent(scmEventINDID, paECET); break;
    case scmEventREQID:
      var_QO = var_QI;
      sendOutputEvent(scmEventCNFID, paECET);
      break;
    default: CProcessInterfaceFB::executeEvent(paEIID, paECET); break;
  }
}

void FORTE_IE::writeOutputData(const TEventID paEIID) {
  if (paEIID == scmEventINDID) {
    writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
    writeData(cFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
  } else {
    CProcessInterfaceFB::writeOutputData(paEIID);
  }
}

CEventConnection *FORTE_IE::getEOConUnchecked(const TPortId paIndex) {
  if (paIndex == 2) {
    return &conn_IND;
  }
  return CProcessInterfaceFB::getEOConUnchecked(paIndex);
}
