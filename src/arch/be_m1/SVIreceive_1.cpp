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
#include "SVIreceive_1.h"

DEFINE_FIRMWARE_FB(FORTE_SVIreceive_1, STRID(SVIreceive_1))

namespace {
  const auto cDataInputNames = std::array{STRID(QI), STRID(ID)};

  const auto cDataOutputNames = std::array{STRID(QO), STRID(RD_1)};

  const auto cEventInputNames = std::array{STRID(INIT), STRID(REQ)};
  const auto cEventInputTypeIds = std::array{STRID(EInit), STRID(Event)};

  const auto cEventOutputNames = std::array{STRID(INITO), STRID(CNF)};
  const auto cEventOutputTypeIds = std::array{STRID(Event), STRID(Event)};

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

void FORTE_SVIreceive_1::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventINITID:
      if (true == QI()) {
        QO() = registerSVIvar(ID(), RD_1(), SVI_F_IN);
      }
      sendOutputEvent(scmEventINITOID, paECET);
      break;

    case scmEventREQID: sendOutputEvent(scmEventCNFID, paECET); break;
    default: break;
  }
}
