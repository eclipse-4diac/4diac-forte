/*******************************************************************************
 * Copyright (c) 2006 - 2014 ACIN, Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny, Monika Wenger,
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "RT_E_DELAY.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "RT_E_DELAY_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_RT_E_DELAY, g_nStringIdRT_E_DELAY)

const CStringDictionary::TStringId FORTE_RT_E_DELAY::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdDT, g_nStringIdTmin, g_nStringIdDeadline, g_nStringIdWCET};

const CStringDictionary::TStringId FORTE_RT_E_DELAY::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME};

const CStringDictionary::TStringId FORTE_RT_E_DELAY::scm_anDataOutputNames[] = {g_nStringIdQO};

const CStringDictionary::TStringId FORTE_RT_E_DELAY::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 FORTE_RT_E_DELAY::scm_anEIWithIndexes[] = {0, 5, -1};
const TDataIOID FORTE_RT_E_DELAY::scm_anEIWith[] = {0, 2, 3, 4, 255, 1, 255};
const CStringDictionary::TStringId FORTE_RT_E_DELAY::scm_anEventInputNames[] = {g_nStringIdINIT, g_nStringIdSTART, g_nStringIdSTOP};

const TDataIOID FORTE_RT_E_DELAY::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_RT_E_DELAY::scm_anEOWithIndexes[] = {0, -1, -1};
const CStringDictionary::TStringId FORTE_RT_E_DELAY::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdEO};

const SFBInterfaceSpec FORTE_RT_E_DELAY::scm_stFBInterfaceSpec = {
  3,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  5,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


FORTE_RT_E_DELAY::FORTE_RT_E_DELAY(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
            CEventSourceFB( pa_poSrcRes, &scm_stFBInterfaceSpec,  pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData){

  setEventChainExecutor(&m_oECEO);
  m_bActive = false;
  m_bInitialized = false;
  //Timeout value is correctly initialized by the constructor m_stTimeListEntry.m_nTimeOut = 0;
  m_stTimeListEntry.mInterval = 0;
  m_stTimeListEntry.mNext = 0;
  m_stTimeListEntry.mTimedFB = this;
  m_stTimeListEntry.mType = e_SingleShot;
  m_oECEO.changeExecutionState(cg_nMGM_CMD_Start);
}

void FORTE_RT_E_DELAY::executeEvent(int pa_nEIID){
  switch(pa_nEIID){
    case cg_nExternalEventID:
      sendOutputEvent(scm_nEventEOID);
      m_bActive = false;
      break;
    case scm_nEventSTOPID:
      if(m_bActive){
        getTimer().unregisterTimedFB(this);
        m_bActive = false;
      }
      break;
    case scm_nEventSTARTID:
      if((!m_bActive)&&(m_bInitialized)){
        getTimer().registerTimedFB( &m_stTimeListEntry, DT());
        m_bActive = true;
      }
      break;
    case scm_nEventINITID:
      if(QI() == true){
        if(!m_bInitialized){
          //m_oECEO.start();
          m_bInitialized = true;
        }
        m_oECEO.setDeadline(Deadline());
      }
      else{
       // m_oECEO.end();
        m_bInitialized = false;
        m_oECEO.setDeadline(static_cast<CIEC_TIME::TValueType>(0));
      }
      QO() = QI();
      sendOutputEvent(scm_nEventINITOID);
    default:
      break;
  }
}


