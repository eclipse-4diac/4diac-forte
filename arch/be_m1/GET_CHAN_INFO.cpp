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
#include "GET_CHAN_INFO.h"
#include <mtypes.h> /* M1 include files    */
#include <msys_e.h>
#include <mio.h>
#include <mio_e.h>

DEFINE_FIRMWARE_FB(FORTE_GET_CHAN_INFO, "GET_CHAN_INFO"_STRID)

namespace {
  const auto cDataInputNames = std::array{"SYMB_NAME"_STRID};

  const auto cDataOutputNames = std::array{"RET_CODE"_STRID, "MOD_NB"_STRID, "CHAN_NB"_STRID};

  const auto cEventInputNames = std::array{"INIT"_STRID};
  const auto cEventInputTypeIds = std::array{"EInit"_STRID};

  const auto cEventOutputNames = std::array{"INITO"_STRID};
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

void FORTE_GET_CHAN_INFO::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventINITID:
      if (init()) {
        sendOutputEvent(scmEventINITOID, paECET);
      }
      break;
  }
}

bool FORTE_GET_CHAN_INFO::init() {
  SINT32 retCode = getChannelInfo();
  return retCode == 0;
}

SINT32 FORTE_GET_CHAN_INFO::getChannelInfo() {
  VOID *DrvId;
  UINT32 Type;
  UINT32 CardNb;
  UINT32 ChanNb;

  CHAR *symbolicName = (CHAR *) (const char *) SYMB_NAME().getValue();
  SINT32 retCode = mio_GetDrvByName(symbolicName, &DrvId, &Type, &CardNb, &ChanNb, nullptr);

  CHAN_NB() = ChanNb;
  MOD_NB() = CardNb;
  return retCode;
}
