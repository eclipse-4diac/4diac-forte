/*******************************************************************************
 * Copyright (c) 2025 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Erich Jobst - initial implementation
 *******************************************************************************/

#include "GEN_SEND_RECV_fbt.h"

USE_STRING_ID(CNF);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(IND);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(REQ);
USE_STRING_ID(RSP);
USE_STRING_ID(GEN_SEND_RECV);

namespace {
  const auto cEventInputNames = std::array{STRID(INIT), STRID(REQ), STRID(RSP)};
  const auto cEventInputTypeIds = std::array{STRID(EInit), STRID(Event), STRID(Event)};
  const auto cEventOutputNames = std::array{STRID(INITO), STRID(CNF), STRID(IND)};
  const auto cEventOutputTypeIds = std::array{STRID(EInit), STRID(Event), STRID(Event)};

  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = cEventInputTypeIds,
      .mEONames = cEventOutputNames,
      .mEOTypeNames = cEventOutputTypeIds,
      .mDINames = {},
      .mDONames = {},
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = {},
  };
} // namespace

DEFINE_GENERIC_FIRMWARE_FB(GEN_SEND_RECV, STRID(GEN_SEND_RECV))

GEN_SEND_RECV::GEN_SEND_RECV(const CStringDictionary::TStringId paInstanceNameId,
                             forte::core::CFBContainer &paContainer) :
    CommunicationFB(paContainer, cFBInterfaceSpec, paInstanceNameId),
    conn_CNF(*this, 1),
    conn_IND(*this, 2) {};

void GEN_SEND_RECV::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID:
      if (send(mGenDIs) != forte::com::ComResult::Async) {
        sendOutputEvent(scmEventCNFID, paECET);
      }
      break;
    case scmEventRSPID:
      if (poll() != forte::com::ComResult::Async) {
        sendOutputEvent(scmEventINDID, paECET);
      }
      break;
    case scmEventSentID: sendOutputEvent(scmEventCNFID, paECET); break;
    case scmEventReceiveID: sendOutputEvent(scmEventINDID, paECET); break;
    default: CommunicationFB::executeEvent(paEIID, paECET); break;
  }
}

bool GEN_SEND_RECV::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
  auto [numGenDIs, numGenDOs] = parseConfigString(paConfigString);
  if (numGenDIs == cgInvalidPortId || numGenDOs == cgInvalidPortId) {
    return false;
  }

  configureDIs(numGenDIs, paInterfaceSpec);
  configureDOs(numGenDOs, paInterfaceSpec);
  return true;
}

CEventConnection *GEN_SEND_RECV::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 1: return &conn_CNF;
    case 2: return &conn_IND;
    default: return CommunicationFB::getEOConUnchecked(paIndex);
  }
}
