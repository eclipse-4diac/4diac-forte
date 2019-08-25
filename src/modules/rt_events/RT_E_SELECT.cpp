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
#include "RT_E_SELECT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "RT_E_SELECT_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_RT_E_SELECT, g_nStringIdRT_E_SELECT)

const CStringDictionary::TStringId FORTE_RT_E_SELECT::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdG, g_nStringIdTmin, g_nStringIdDeadline, g_nStringIdWCET};

const CStringDictionary::TStringId FORTE_RT_E_SELECT::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME};

const CStringDictionary::TStringId FORTE_RT_E_SELECT::scm_anDataOutputNames[] = {g_nStringIdQO};

const CStringDictionary::TStringId FORTE_RT_E_SELECT::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 FORTE_RT_E_SELECT::scm_anEIWithIndexes[] = {0, 5, 7};
const TDataIOID FORTE_RT_E_SELECT::scm_anEIWith[] = {0, 2, 3, 4, 255, 1, 255, 1, 255};
const CStringDictionary::TStringId FORTE_RT_E_SELECT::scm_anEventInputNames[] = {g_nStringIdINIT, g_nStringIdEI0, g_nStringIdEI1};

const TDataIOID FORTE_RT_E_SELECT::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_RT_E_SELECT::scm_anEOWithIndexes[] = {0, -1, -1};
const CStringDictionary::TStringId FORTE_RT_E_SELECT::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdEO};

const SFBInterfaceSpec FORTE_RT_E_SELECT::scm_stFBInterfaceSpec = {
  3,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  5,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

FORTE_RT_E_SELECT::FORTE_RT_E_SELECT(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
       CRTEventSingle(
           pa_poSrcRes,
           &scm_stFBInterfaceSpec, pa_nInstanceNameId,
           m_anFBConnData, m_anFBVarsData) {
}

bool FORTE_RT_E_SELECT::checkActivation(int pa_nEIID){
  return (((scm_nEventEI0ID == pa_nEIID) && (G() == false))||
          ((scm_nEventEI1ID == pa_nEIID) && (G() == true)));
}

