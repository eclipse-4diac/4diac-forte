/*******************************************************************************
 * Copyright (c) 2006 - 2014 ACIN, Profactor GmbH, fortiss GmbH
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
#include "RT_E_CYCLE.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "RT_E_CYCLE_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_RT_E_CYCLE, g_nStringIdRT_E_CYCLE)

const CStringDictionary::TStringId FORTE_RT_E_CYCLE::scm_anDataInputNames[] = {g_nStringIdDT, g_nStringIdDeadline, g_nStringIdWCET};

const CStringDictionary::TStringId FORTE_RT_E_CYCLE::scm_anDataInputTypeIds[] = {g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME};

const CStringDictionary::TStringId FORTE_RT_E_CYCLE::scm_anDataOutputNames[] = {g_nStringIdQO};

const CStringDictionary::TStringId FORTE_RT_E_CYCLE::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 FORTE_RT_E_CYCLE::scm_anEIWithIndexes[] = {0, -1};
const TDataIOID FORTE_RT_E_CYCLE::scm_anEIWith[] = {0, 1, 2, 255};
const CStringDictionary::TStringId FORTE_RT_E_CYCLE::scm_anEventInputNames[] = {g_nStringIdSTART, g_nStringIdSTOP};

const TDataIOID FORTE_RT_E_CYCLE::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_RT_E_CYCLE::scm_anEOWithIndexes[] = {-1, 0};
const CStringDictionary::TStringId FORTE_RT_E_CYCLE::scm_anEventOutputNames[] = {g_nStringIdEO};

const SFBInterfaceSpec FORTE_RT_E_CYCLE::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  3,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

FORTE_RT_E_CYCLE::FORTE_RT_E_CYCLE(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
            CEventSourceFB( paSrcRes, &scm_stFBInterfaceSpec,  paInstanceNameId, m_anFBConnData, m_anFBVarsData){
  setEventChainExecutor(&mECEO);
  mActive = false;
  mTimeListEntry.mTimeOut = 0;
  mTimeListEntry.mInterval = 0;
  mTimeListEntry.mNext = 0;
  mTimeListEntry.mTimedFB = this;
  mTimeListEntry.mType = e_Periodic;
  mECEO.changeExecutionState(cg_nMGM_CMD_Start);
}


void FORTE_RT_E_CYCLE::executeEvent(int paEIID){
  switch(paEIID){
    case cg_nExternalEventID:
      sendOutputEvent(scm_nEventEOID);
      break;
    case scm_nEventSTOPID:
      if(mActive){
        mECEO.setDeadline(static_cast<CIEC_TIME::TValueType>(0));
        getTimer().unregisterTimedFB(this);
        mActive = false;
      }
      break;
    case scm_nEventSTARTID:
      if(!mActive){
        mECEO.setDeadline(Deadline());
        getTimer().registerTimedFB( &mTimeListEntry, DT());
        mActive = true;
      }
      break;
    default:
      break;
  }
}

