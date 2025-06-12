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
#include "GET_VALUE.h"

#include <mtypes.h> /* M1 include files    */
#include <msys_e.h>
#include <mio.h>
#include <mio_e.h>

DEFINE_FIRMWARE_FB(FORTE_GET_VALUE, STRID(GET_VALUE))

namespace {
  const auto cDataInputNames = std::array{STRID(MOD_NB), STRID(CHAN_NB)};
  
  
  const auto cDataOutputNames = std::array{STRID(RET_CODE), STRID(VALUE)};
  
  
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
}


void FORTE_GET_VALUE::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventINITID:
      if (init()) {
        sendOutputEvent(scmEventINITOID, paECET);
      }
      break;
    case scmEventREQID:
      RET_CODE() = read();
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

bool FORTE_GET_VALUE::init() {
  driverId = mio_GetDrv(static_cast<unsigned int>(MOD_NB()));
  return driverId != nullptr;
}

SINT32 FORTE_GET_VALUE::read() {
  SINT32 myValue;
  SINT32 retCode = mio_GetValue(driverId, CHAN_NB(), &myValue);
  VALUE() = myValue;
  return retCode;
}
