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

#include <mtypes.h>                     /* M1 include files    */
#include <msys_e.h>
#include <mio.h>
#include <mio_e.h>

DEFINE_FIRMWARE_FB(FORTE_GET_VALUE, STRID(GET_VALUE))

const CStringDictionary::TStringId FORTE_GET_VALUE::scmDataInputNames[] = {STRID(MOD_NB), STRID(CHAN_NB)};

const CStringDictionary::TStringId FORTE_GET_VALUE::scmDataInputTypeIds[] = {STRID(INT), STRID(INT)};

const CStringDictionary::TStringId FORTE_GET_VALUE::scmDataOutputNames[] = {STRID(RET_CODE), STRID(VALUE)};

const CStringDictionary::TStringId FORTE_GET_VALUE::scmDataOutputTypeIds[] = {STRID(INT), STRID(SINT)};

const TForteInt16 FORTE_GET_VALUE::scmEIWithIndexes[] = {0, 2};
const TDataIOID FORTE_GET_VALUE::scmEIWith[] = {0, scmWithListDelimiter, 1, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_GET_VALUE::scmEventInputNames[] = {STRID(INIT), STRID(REQ)};
const CStringDictionary::TStringId FORTE_GET_VALUE::scmEventInputTypeIds[] = {STRID(EInit), STRID(Event)};

const TDataIOID FORTE_GET_VALUE::scmEOWith[] = {0, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_GET_VALUE::scmEOWithIndexes[] = {0, 2, -1};
const CStringDictionary::TStringId FORTE_GET_VALUE::scmEventOutputNames[] = {STRID(INITO), STRID(CNF)};
const CStringDictionary::TStringId FORTE_GET_VALUE::scmEventOutputTypeIds[] = {STRID(Event), STRID(Event)};

const SFBInterfaceSpec FORTE_GET_VALUE::scmFBInterfaceSpec = {
  2,  scmEventInputNames, scmEventInputTypeIds,  scmEIWith,  scmEIWithIndexes,
  2,  scmEventOutputNames, scmEventOutputTypeIds,  scmEOWith, scmEOWithIndexes,  2,  scmDataInputNames, scmDataInputTypeIds,
  2,  scmDataOutputNames, scmDataOutputTypeIds,
  0, 0
};


void FORTE_GET_VALUE::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID){
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


