/*******************************************************************************
 * Copyright (c) 2011 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "SVIsend_1.h"

DEFINE_FIRMWARE_FB(FORTE_SVIsend_1, "SVIsend_1"_STRID)

namespace {
  const auto cDataInputNames = std::array{"QI"_STRID, "ID"_STRID, "SD_1"_STRID};

  const auto cDataOutputNames = std::array{"QO"_STRID};

  const auto cEventInputNames = std::array{"INIT"_STRID, "REQ"_STRID};
  const auto cEventInputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};

  const auto cEventOutputNames = std::array{"INITO"_STRID, "CNF"_STRID};
  const auto cEventOutputTypeIds = std::array{"Event"_STRID, "Event"_STRID};

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

void FORTE_SVIsend_1::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventINITID:
      if (true == QI()) {
        QO() = registerSVIvar(ID(), SD_1(), SVI_F_OUT);
      }
      sendOutputEvent(scmEventINITOID, paECET);
      if (true == QO()) {
        printf("Alles OK send\n");
      } else {
        printf("Alles FALSCH send\n");
      }
      break;

    case scmEventREQID: sendOutputEvent(scmEventCNFID, paECET); break;
    default: break;
  }
}
