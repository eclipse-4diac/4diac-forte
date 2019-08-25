/*******************************************************************************
 * Copyright (c) 2006 - 2013 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny, Monika Wenger,
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "RT_E_REND.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "RT_E_REND_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_RT_E_REND, g_nStringIdRT_E_REND)

const CStringDictionary::TStringId FORTE_RT_E_REND::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdTmin, g_nStringIdDeadline, g_nStringIdWCET};

const CStringDictionary::TStringId FORTE_RT_E_REND::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME};

const CStringDictionary::TStringId FORTE_RT_E_REND::scm_anDataOutputNames[] = {g_nStringIdQO};

const CStringDictionary::TStringId FORTE_RT_E_REND::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 FORTE_RT_E_REND::scm_anEIWithIndexes[] = {0, -1, -1, -1};
const TDataIOID FORTE_RT_E_REND::scm_anEIWith[] = {0, 1, 2, 3, 255};
const CStringDictionary::TStringId FORTE_RT_E_REND::scm_anEventInputNames[] = {g_nStringIdINIT, g_nStringIdEI1, g_nStringIdEI2, g_nStringIdR};

const TDataIOID FORTE_RT_E_REND::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_RT_E_REND::scm_anEOWithIndexes[] = {0, -1, -1};
const CStringDictionary::TStringId FORTE_RT_E_REND::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdEO};

const SFBInterfaceSpec FORTE_RT_E_REND::scm_stFBInterfaceSpec = {
  4,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  4,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


FORTE_RT_E_REND::FORTE_RT_E_REND(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
       CRTEventSingle(
           pa_poSrcRes,
           &scm_stFBInterfaceSpec, pa_nInstanceNameId,
           m_anFBConnData, m_anFBVarsData),
       m_nState(0) {
}

bool FORTE_RT_E_REND::checkActivation(int pa_nEIID){
  bool bRetval = false;

  if(scm_nEventEI1ID == pa_nEIID){
    if(m_nState == 2)
      bRetval = true;
    else
      m_nState = 1;
  }
  else
    if(scm_nEventEI2ID == pa_nEIID){
      if(m_nState == 1)
        bRetval = true;
      else
        m_nState = 2;
    }
    else
      if(scm_nEventRID == pa_nEIID)
        m_nState = 0;

  return bRetval;
}

