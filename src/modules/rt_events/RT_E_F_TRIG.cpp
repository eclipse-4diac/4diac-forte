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
#include "RT_E_F_TRIG.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "RT_E_F_TRIG_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_RT_E_F_TRIG, g_nStringIdRT_E_F_TRIG)

const CStringDictionary::TStringId FORTE_RT_E_F_TRIG::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdTmin, g_nStringIdDeadline, g_nStringIdWCET};

const CStringDictionary::TStringId FORTE_RT_E_F_TRIG::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME};

const CStringDictionary::TStringId FORTE_RT_E_F_TRIG::scm_anDataOutputNames[] = {g_nStringIdQO};

const CStringDictionary::TStringId FORTE_RT_E_F_TRIG::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 FORTE_RT_E_F_TRIG::scm_anEIWithIndexes[] = {0, 5};
const TDataIOID FORTE_RT_E_F_TRIG::scm_anEIWith[] = {0, 1, 2, 3, 255, 0, 255};
const CStringDictionary::TStringId FORTE_RT_E_F_TRIG::scm_anEventInputNames[] = {g_nStringIdINIT, g_nStringIdEI};

const TDataIOID FORTE_RT_E_F_TRIG::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_RT_E_F_TRIG::scm_anEOWithIndexes[] = {0, -1, -1};
const CStringDictionary::TStringId FORTE_RT_E_F_TRIG::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdEO};

const SFBInterfaceSpec FORTE_RT_E_F_TRIG::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  4,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

FORTE_RT_E_F_TRIG::FORTE_RT_E_F_TRIG(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
       CRTEventSingle(
           pa_poSrcRes,
           &scm_stFBInterfaceSpec, pa_nInstanceNameId,
           m_anFBConnData, m_anFBVarsData){
    m_bWasHigh = false;
}

bool FORTE_RT_E_F_TRIG::checkActivation(int ){
  bool bRetval = false;

  if((QI() == false)&&(m_bWasHigh)){
    bRetval = true;
  }
  m_bWasHigh = QI();
  return bRetval;
}


