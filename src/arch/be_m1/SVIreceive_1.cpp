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

DEFINE_FIRMWARE_FB(FORTE_SVIreceive_1, g_nStringIdSVIreceive_1)

const CStringDictionary::TStringId FORTE_SVIreceive_1::scm_anDataInputNames[] = { g_nStringIdQI, g_nStringIdID };

const CStringDictionary::TStringId FORTE_SVIreceive_1::scm_anDataInputTypeIds[] = { g_nStringIdBOOL, g_nStringIdSTRING };

const CStringDictionary::TStringId FORTE_SVIreceive_1::scm_anDataOutputNames[] = { g_nStringIdQO, g_nStringIdRD_1 };

const CStringDictionary::TStringId FORTE_SVIreceive_1::scm_anDataOutputTypeIds[] = { g_nStringIdBOOL, g_nStringIdANY };

const TForteInt16 FORTE_SVIreceive_1::scm_anEIWithIndexes[] = { 0, 3 };
const TDataIOID FORTE_SVIreceive_1::scm_anEIWith[] = { 0, 1, 255, 0, 255 };
const CStringDictionary::TStringId FORTE_SVIreceive_1::scm_anEventInputNames[] = { g_nStringIdINIT, g_nStringIdREQ };

const TDataIOID FORTE_SVIreceive_1::scm_anEOWith[] = { 0, 255, 0, 1, 255 };
const TForteInt16 FORTE_SVIreceive_1::scm_anEOWithIndexes[] = { 0, 2, -1 };
const CStringDictionary::TStringId FORTE_SVIreceive_1::scm_anEventOutputNames[] = { g_nStringIdINITO, g_nStringIdCNF };

const SFBInterfaceSpec FORTE_SVIreceive_1::scm_stFBInterfaceSpec = { 2, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 2, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 2, scm_anDataInputNames, scm_anDataInputTypeIds, 2, scm_anDataOutputNames, scm_anDataOutputTypeIds, 0, 0 };

void FORTE_SVIreceive_1::executeEvent(int pa_nEIID){
  switch (pa_nEIID){
    case scm_nEventINITID:
      if(true == QI()){
        QO() = registerSVIvar(ID(), RD_1(), SVI_F_IN);
      }
      sendOutputEvent(scm_nEventINITOID);
      break;

    case scm_nEventREQID:
      sendOutputEvent(scm_nEventCNFID);
      break;
    default:
      break;
  }
}

