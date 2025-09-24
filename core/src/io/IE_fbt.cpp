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

#include "forte/io/IE_fbt.h"

using namespace forte::literals;

namespace forte::io {
  namespace {
    const auto cEventInputNames = std::array{"INIT"_STRID, "REQ"_STRID};
    const auto cEventInputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
    const auto cEventOutputNames = std::array{"INITO"_STRID, "CNF"_STRID, "IND"_STRID};
    const auto cEventOutputTypeIds = std::array{"EInit"_STRID, "Event"_STRID, "Event"_STRID};
    const auto cDataInputNames = std::array{"QI"_STRID, "PARAMS"_STRID};
    const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID};

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

  DEFINE_FIRMWARE_FB(FORTE_IE, "IE"_STRID)

  FORTE_IE::FORTE_IE(const StringId paInstanceNameId, CFBContainer &paContainer) :
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
} // namespace forte::io
