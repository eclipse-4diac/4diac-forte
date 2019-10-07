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
#include "FB_R_TRIG.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FB_R_TRIG_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FB_R_TRIG, g_nStringIdFB_R_TRIG)

const CStringDictionary::TStringId FB_R_TRIG::scm_anDataInputNames[] = {g_nStringIdCLK};

const CStringDictionary::TStringId FB_R_TRIG::scm_anDataOutputNames[] = {g_nStringIdQ};
const CStringDictionary::TStringId FB_R_TRIG::scm_aunDIDataTypeIds[] = {g_nStringIdBOOL};
const CStringDictionary::TStringId FB_R_TRIG::scm_aunDODataTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 FB_R_TRIG::scm_anEIWithIndexes[] = {0};
const TDataIOID FB_R_TRIG::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId FB_R_TRIG::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FB_R_TRIG::scm_anEOWith[] = {0, 255};
const TForteInt16 FB_R_TRIG::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FB_R_TRIG::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FB_R_TRIG::scm_stFBInterfaceSpec = {
  1,
  scm_anEventInputNames,
  scm_anEIWith,
  scm_anEIWithIndexes,
  1,
  scm_anEventOutputNames,
  scm_anEOWith,
  scm_anEOWithIndexes,
  1,
  scm_anDataInputNames, scm_aunDIDataTypeIds,
  1,
  scm_anDataOutputNames, scm_aunDODataTypeIds,
  0,
  0
};

void FB_R_TRIG::executeEvent(int pa_nEIID){
  switch(pa_nEIID){
    case scm_nEventREQID:
        Q() = CLK() && !M;
        M = CLK();
        sendOutputEvent(scm_nEventCNFID);
      break;
  }
}


