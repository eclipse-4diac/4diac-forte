/*******************************************************************************
 * Copyright (c) 2009 - 2013 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl, Ingo Hengy
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "FB_TOF.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FB_TOF_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FB_TOF, g_nStringIdFB_TOF)

const CStringDictionary::TStringId FB_TOF::scm_anDataInputNames[] = {g_nStringIdIN, g_nStringIdPT};

const CStringDictionary::TStringId FB_TOF::scm_anDataOutputNames[] = {g_nStringIdQ, g_nStringIdET};
const CStringDictionary::TStringId FB_TOF::scm_aunDIDataTypeIds[] = {g_nStringIdBOOL, g_nStringIdTIME};
const CStringDictionary::TStringId FB_TOF::scm_aunDODataTypeIds[] = {g_nStringIdBOOL, g_nStringIdTIME};

const TForteInt16 FB_TOF::scm_anEIWithIndexes[] = {0};
const TDataIOID FB_TOF::scm_anEIWith[] = {0, 1, 255};
const CStringDictionary::TStringId FB_TOF::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FB_TOF::scm_anEOWith[] = {0, 1, 255};
const TForteInt16 FB_TOF::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FB_TOF::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FB_TOF::scm_stFBInterfaceSpec = {
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

void FB_TOF::executeEvent(int pa_nEIID){
  if(scm_nEventREQID == pa_nEIID){
    if(st_IN() == true){
      Q() = true;
      ET() = 0;
      fallingEdge = false;
      notFirstRisingEdge = true;
      start = 0;
    }
    else{
      if(true == notFirstRisingEdge){
        if(fallingEdge == false){
          fallingEdge = true;
          start = NOW_MONOTONIC();
        }
        else{
          count = NOW_MONOTONIC() - start;
          if(PT() <= count){
            Q() = false;
            ET() = PT();
          }else{
            ET() = count;
          }
        }
      }
    }
    sendOutputEvent(scm_nEventCNFID);
  }
}
