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

DEFINE_FIRMWARE_FB(FORTE_SVIsend_1, g_nStringIdSVIsend_1)

const CStringDictionary::TStringId FORTE_SVIsend_1::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdID, g_nStringIdSD_1};

const CStringDictionary::TStringId FORTE_SVIsend_1::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdANY};

const CStringDictionary::TStringId FORTE_SVIsend_1::scm_anDataOutputNames[] = {g_nStringIdQO};

const CStringDictionary::TStringId FORTE_SVIsend_1::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 FORTE_SVIsend_1::scm_anEIWithIndexes[] = {0, 4};
const TDataIOID FORTE_SVIsend_1::scm_anEIWith[] = {0, 1, 2, 255, 0, 1, 2, 255};
const CStringDictionary::TStringId FORTE_SVIsend_1::scm_anEventInputNames[] = {g_nStringIdINIT, g_nStringIdREQ};

const TDataIOID FORTE_SVIsend_1::scm_anEOWith[] = {0, 255, 0, 255};
const TForteInt16 FORTE_SVIsend_1::scm_anEOWithIndexes[] = {0, 2, -1};
const CStringDictionary::TStringId FORTE_SVIsend_1::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdCNF};

const SFBInterfaceSpec FORTE_SVIsend_1::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  3,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


void FORTE_SVIsend_1::executeEvent(int pa_nEIID){
  switch(pa_nEIID){
    case scm_nEventINITID:
      if(true == QI()){
        QO() = registerSVIvar(ID(),SD_1(),SVI_F_OUT);
      }
      sendOutputEvent(scm_nEventINITOID);
      if (true == QO()) {
        printf("Alles OK send\n");
      } else {
        printf("Alles FALSCH send\n");
      }
      break;

    case scm_nEventREQID:
      sendOutputEvent(scm_nEventCNFID);
      break;
    default:
      break;
  }
}



