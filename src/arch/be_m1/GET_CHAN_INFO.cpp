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
#include <mtypes.h>                     /* M1 include files    */
#include <msys_e.h>
#include <mio.h>
#include <mio_e.h>

DEFINE_FIRMWARE_FB(FORTE_GET_CHAN_INFO, STRID(GET_CHAN_INFO))

const CStringDictionary::TStringId FORTE_GET_CHAN_INFO::scmDataInputNames[] = { STRID(SYMB_NAME) };

const CStringDictionary::TStringId FORTE_GET_CHAN_INFO::scmDataInputTypeIds[] = { STRID(STRING) };

const CStringDictionary::TStringId FORTE_GET_CHAN_INFO::scmDataOutputNames[] = { STRID(RET_CODE), STRID(MOD_NB), STRID(CHAN_NB) };

const CStringDictionary::TStringId FORTE_GET_CHAN_INFO::scmDataOutputTypeIds[] = { STRID(INT), STRID(INT), STRID(INT) };

const TForteInt16 FORTE_GET_CHAN_INFO::scmEIWithIndexes[] = { 0 };
const TDataIOID FORTE_GET_CHAN_INFO::scmEIWith[] = { 0, scmWithListDelimiter };
const CStringDictionary::TStringId FORTE_GET_CHAN_INFO::scmEventInputNames[] = { STRID(INIT) };
const CStringDictionary::TStringId FORTE_GET_CHAN_INFO::scmEventInputTypeIds[] = {STRID(EInit)};

const TDataIOID FORTE_GET_CHAN_INFO::scmEOWith[] = { 0, 1, 2, scmWithListDelimiter };
const TForteInt16 FORTE_GET_CHAN_INFO::scmEOWithIndexes[] = { 0, -1 };
const CStringDictionary::TStringId FORTE_GET_CHAN_INFO::scmEventOutputNames[] = { STRID(INITO) };
const CStringDictionary::TStringId FORTE_GET_CHAN_INFO::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_GET_CHAN_INFO::scmFBInterfaceSpec = { 1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes, 1, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes, 1, scmDataInputNames, scmDataInputTypeIds, 3, scmDataOutputNames, scmDataOutputTypeIds, 0, 0 };

void FORTE_GET_CHAN_INFO::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID){
    case scmEventINITID:
      if(init()){
        sendOutputEvent(scmEventINITOID, paECET);
      }
      break;
  }
}

bool FORTE_GET_CHAN_INFO::init(){
  SINT32 retCode = getChannelInfo();
  return retCode == 0;
}

SINT32 FORTE_GET_CHAN_INFO::getChannelInfo(){
  VOID *DrvId;
  UINT32 Type;
  UINT32 CardNb;
  UINT32 ChanNb;

  CHAR* symbolicName = (CHAR*) (const char*) SYMB_NAME().getValue();
  SINT32 retCode = mio_GetDrvByName(symbolicName, &DrvId, &Type, &CardNb, &ChanNb, nullptr);

  CHAN_NB() = ChanNb;
  MOD_NB() = CardNb;
  return retCode;
}
