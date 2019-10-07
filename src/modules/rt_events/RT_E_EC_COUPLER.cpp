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
#include "RT_E_EC_COUPLER.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "RT_E_EC_COUPLER_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_RT_E_EC_COUPLER, g_nStringIdRT_E_EC_COUPLER)

const CStringDictionary::TStringId FORTE_RT_E_EC_COUPLER::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdTmin, g_nStringIdDeadline};

const CStringDictionary::TStringId FORTE_RT_E_EC_COUPLER::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdTIME, g_nStringIdTIME};

const CStringDictionary::TStringId FORTE_RT_E_EC_COUPLER::scm_anDataOutputNames[] = {g_nStringIdQO};

const CStringDictionary::TStringId FORTE_RT_E_EC_COUPLER::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 FORTE_RT_E_EC_COUPLER::scm_anEIWithIndexes[] = {0, -1};
const TDataIOID FORTE_RT_E_EC_COUPLER::scm_anEIWith[] = {0, 1, 2, 255};
const CStringDictionary::TStringId FORTE_RT_E_EC_COUPLER::scm_anEventInputNames[] = {g_nStringIdINIT, g_nStringIdEI};

const TDataIOID FORTE_RT_E_EC_COUPLER::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_RT_E_EC_COUPLER::scm_anEOWithIndexes[] = {0, -1, -1};
const CStringDictionary::TStringId FORTE_RT_E_EC_COUPLER::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdEO};

const SFBInterfaceSpec FORTE_RT_E_EC_COUPLER::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  3,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


FORTE_RT_E_EC_COUPLER::FORTE_RT_E_EC_COUPLER(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
       CRTEventSingle(
           pa_poSrcRes,
           &scm_stFBInterfaceSpec, pa_nInstanceNameId,
           m_anFBConnData, m_anFBVarsData) {
}



