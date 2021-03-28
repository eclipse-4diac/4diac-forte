/*******************************************************************************
 * Copyright (c) 2009 - 2013 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl,  Ingo Hengy
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "FB_TP.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FB_TP_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FB_TP, g_nStringIdFB_TP)

const CStringDictionary::TStringId FB_TP::scm_anDataInputNames[] = {g_nStringIdIN, g_nStringIdPT};

const CStringDictionary::TStringId FB_TP::scm_anDataOutputNames[] = {g_nStringIdQ, g_nStringIdET};
const CStringDictionary::TStringId FB_TP::scm_aunDIDataTypeIds[] = {g_nStringIdBOOL, g_nStringIdTIME};
const CStringDictionary::TStringId FB_TP::scm_aunDODataTypeIds[] = {g_nStringIdBOOL, g_nStringIdTIME};

const TForteInt16 FB_TP::scm_anEIWithIndexes[] = {0};
const TDataIOID FB_TP::scm_anEIWith[] = {0, 1, 255};
const CStringDictionary::TStringId FB_TP::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FB_TP::scm_anEOWith[] = {0, 1, 255};
const TForteInt16 FB_TP::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FB_TP::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FB_TP::scm_stFBInterfaceSpec = {
    1,
    scm_anEventInputNames,
    scm_anEIWith,
    scm_anEIWithIndexes,
    1,
    scm_anEventOutputNames,
    scm_anEOWith,
    scm_anEOWithIndexes,
    2,
    scm_anDataInputNames, scm_aunDIDataTypeIds,
    2,
    scm_anDataOutputNames, scm_aunDODataTypeIds,
    0,
    0
};

void FB_TP::executeEvent(int pa_nEIID){
  if(pa_nEIID == scm_nEventREQID){
      if (edgeFlag) {
        if(ET() >= PT()){
          Q() = false;
          edgeFlag = false;
          DEVLOG_DEBUG("top\n");
        }else{
          ET() = NOW_MONOTONIC() - start;
          DEVLOG_DEBUG("rising\n");
        }
      }
      else {
        if(st_IN() == true && ET() == 0){
          Q() = true;
          edgeFlag = true;
          start = NOW_MONOTONIC();
          DEVLOG_DEBUG("start\n");
        }
        else
          if((false == st_IN()) && (ET()>0)) {
            ET() = 0;
            DEVLOG_DEBUG("reset\n");
          }
      }
      sendOutputEvent(scm_nEventCNFID);
  }
}
