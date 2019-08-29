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

DEFINE_FIRMWARE_FB(FORTE_GET_CHAN_INFO, g_nStringIdGET_CHAN_INFO)

const CStringDictionary::TStringId FORTE_GET_CHAN_INFO::scm_anDataInputNames[] = { g_nStringIdSYMB_NAME };

const CStringDictionary::TStringId FORTE_GET_CHAN_INFO::scm_anDataInputTypeIds[] = { g_nStringIdSTRING };

const CStringDictionary::TStringId FORTE_GET_CHAN_INFO::scm_anDataOutputNames[] = { g_nStringIdRET_CODE, g_nStringIdMOD_NB, g_nStringIdCHAN_NB };

const CStringDictionary::TStringId FORTE_GET_CHAN_INFO::scm_anDataOutputTypeIds[] = { g_nStringIdINT, g_nStringIdINT, g_nStringIdINT };

const TForteInt16 FORTE_GET_CHAN_INFO::scm_anEIWithIndexes[] = { 0 };
const TDataIOID FORTE_GET_CHAN_INFO::scm_anEIWith[] = { 0, 255 };
const CStringDictionary::TStringId FORTE_GET_CHAN_INFO::scm_anEventInputNames[] = { g_nStringIdINIT };

const TDataIOID FORTE_GET_CHAN_INFO::scm_anEOWith[] = { 0, 1, 2, 255 };
const TForteInt16 FORTE_GET_CHAN_INFO::scm_anEOWithIndexes[] = { 0, -1 };
const CStringDictionary::TStringId FORTE_GET_CHAN_INFO::scm_anEventOutputNames[] = { g_nStringIdINITO };

const SFBInterfaceSpec FORTE_GET_CHAN_INFO::scm_stFBInterfaceSpec = { 1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 1, scm_anDataInputNames, scm_anDataInputTypeIds, 3, scm_anDataOutputNames, scm_anDataOutputTypeIds, 0, 0 };

void FORTE_GET_CHAN_INFO::executeEvent(int pa_nEIID){
  switch (pa_nEIID){
    case scm_nEventINITID:
      if(init()){
        sendOutputEvent(scm_nEventINITOID);
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
  SINT32 retCode = mio_GetDrvByName(symbolicName, &DrvId, &Type, &CardNb, &ChanNb, NULL);

  CHAN_NB() = ChanNb;
  MOD_NB() = CardNb;
  return retCode;
}
