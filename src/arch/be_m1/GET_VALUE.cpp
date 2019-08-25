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

DEFINE_FIRMWARE_FB(FORTE_GET_VALUE, g_nStringIdGET_VALUE)

const CStringDictionary::TStringId FORTE_GET_VALUE::scm_anDataInputNames[] = {g_nStringIdMOD_NB, g_nStringIdCHAN_NB};

const CStringDictionary::TStringId FORTE_GET_VALUE::scm_anDataInputTypeIds[] = {g_nStringIdINT, g_nStringIdINT};

const CStringDictionary::TStringId FORTE_GET_VALUE::scm_anDataOutputNames[] = {g_nStringIdRET_CODE, g_nStringIdVALUE};

const CStringDictionary::TStringId FORTE_GET_VALUE::scm_anDataOutputTypeIds[] = {g_nStringIdINT, g_nStringIdSINT};

const TForteInt16 FORTE_GET_VALUE::scm_anEIWithIndexes[] = {0, 2};
const TDataIOID FORTE_GET_VALUE::scm_anEIWith[] = {0, 255, 1, 255};
const CStringDictionary::TStringId FORTE_GET_VALUE::scm_anEventInputNames[] = {g_nStringIdINIT, g_nStringIdREQ};

const TDataIOID FORTE_GET_VALUE::scm_anEOWith[] = {0, 255, 0, 1, 255};
const TForteInt16 FORTE_GET_VALUE::scm_anEOWithIndexes[] = {0, 2, -1};
const CStringDictionary::TStringId FORTE_GET_VALUE::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdCNF};

const SFBInterfaceSpec FORTE_GET_VALUE::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  2,  scm_anDataInputNames, scm_anDataInputTypeIds,
  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


void FORTE_GET_VALUE::executeEvent(int pa_nEIID){
  switch(pa_nEIID){
    case scm_nEventINITID:
    if (init()) {
      sendOutputEvent(scm_nEventINITOID);
    }
    break;
  case scm_nEventREQID:
    RET_CODE() = read();
    sendOutputEvent(scm_nEventCNFID);
    break;
  }
}

bool FORTE_GET_VALUE::init() {
  driverId = mio_GetDrv(static_cast<unsigned int>(MOD_NB()));
  return driverId != NULL;
}

SINT32 FORTE_GET_VALUE::read() {
  SINT32 myValue;
  SINT32 retCode = mio_GetValue(driverId, CHAN_NB(), &myValue);
  VALUE() = myValue;
  return retCode;
}


