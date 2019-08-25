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
#include "FB_CTD.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FB_CTD_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FB_CTD, g_nStringIdFB_CTD)

const CStringDictionary::TStringId FB_CTD::scm_anDataInputNames[] = {g_nStringIdCD, g_nStringIdLD, g_nStringIdPV};

const CStringDictionary::TStringId FB_CTD::scm_anDataOutputNames[] = {g_nStringIdQ, g_nStringIdCV};
const CStringDictionary::TStringId FB_CTD::scm_aunDIDataTypeIds[] = {g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdINT};
const CStringDictionary::TStringId FB_CTD::scm_aunDODataTypeIds[] = {g_nStringIdBOOL, g_nStringIdINT};

const TForteInt16 FB_CTD::scm_anEIWithIndexes[] = {0};
const TDataIOID FB_CTD::scm_anEIWith[] = {0, 2, 1, 255};
const CStringDictionary::TStringId FB_CTD::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FB_CTD::scm_anEOWith[] = {0, 1, 255};
const TForteInt16 FB_CTD::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FB_CTD::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FB_CTD::scm_stFBInterfaceSpec = {
  1,
  scm_anEventInputNames,
  scm_anEIWith,
  scm_anEIWithIndexes,
  1,
  scm_anEventOutputNames,
  scm_anEOWith,
  scm_anEOWithIndexes,
  3,
  scm_anDataInputNames, scm_aunDIDataTypeIds,
  2,
  scm_anDataOutputNames, scm_aunDODataTypeIds,
  0,
  0
};

void FB_CTD::executeEvent(int pa_nEIID){
  if(pa_nEIID == scm_nEventREQID){
      if(true == LD()){
        CV() = PV();
      }
      else if( (true == CD()) && (CV() > CIEC_INT::scm_nMinVal)){
        CV() = static_cast<TForteInt16>(CV() - 1);
      }
      Q() = (CV() <= 0);
      sendOutputEvent(scm_nEventCNFID);
  }
}

