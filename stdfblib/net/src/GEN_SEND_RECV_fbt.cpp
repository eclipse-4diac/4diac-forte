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

#include "forte/iec61499/net/GEN_SEND_RECV_fbt.h"

using namespace forte::literals;

namespace forte::iec61499::net {
  namespace {
    const auto cEventInputNames = std::array{"INIT"_STRID, "REQ"_STRID, "RSP"_STRID};
    const auto cEventInputTypeIds = std::array{"EInit"_STRID, "Event"_STRID, "Event"_STRID};
    const auto cEventOutputNames = std::array{"INITO"_STRID, "CNF"_STRID, "IND"_STRID};
    const auto cEventOutputTypeIds = std::array{"EInit"_STRID, "Event"_STRID, "Event"_STRID};

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

  DEFINE_GENERIC_FIRMWARE_FB(GEN_SEND_RECV, "iec61499::net::GEN_SEND_RECV"_STRID)

  GEN_SEND_RECV::GEN_SEND_RECV(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CommunicationFB(paContainer, cFBInterfaceSpec, paInstanceNameId),
      conn_CNF(*this, 1),
      conn_IND(*this, 2) {};

  void GEN_SEND_RECV::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID:
        if (send(mGenDIs) != com::ComResult::Async) {
          sendOutputEvent(scmEventCNFID, paECET);
        }
        break;
      case scmEventRSPID:
        if (poll() != com::ComResult::Async) {
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
} // namespace forte::iec61499::net
