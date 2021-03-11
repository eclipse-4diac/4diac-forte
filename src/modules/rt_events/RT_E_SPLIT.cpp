/*******************************************************************************
 * Copyright (c) 2006 - 2015 ACIN, Profactor GmbH, fortiss GmbH
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
#include "RT_E_SPLIT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "RT_E_SPLIT_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_RT_E_SPLIT, g_nStringIdRT_E_SPLIT)

const CStringDictionary::TStringId FORTE_RT_E_SPLIT::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdTmin, g_nStringIdDeadline_EO1, g_nStringIdWCET_EO1, g_nStringIdDeadline_EO2, g_nStringIdWCET_EO2};

const CStringDictionary::TStringId FORTE_RT_E_SPLIT::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME};

const CStringDictionary::TStringId FORTE_RT_E_SPLIT::scm_anDataOutputNames[] = {g_nStringIdQO};

const CStringDictionary::TStringId FORTE_RT_E_SPLIT::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 FORTE_RT_E_SPLIT::scm_anEIWithIndexes[] = {0, -1};
const TDataIOID FORTE_RT_E_SPLIT::scm_anEIWith[] = {0, 1, 2, 3, 4, 5, 255};
const CStringDictionary::TStringId FORTE_RT_E_SPLIT::scm_anEventInputNames[] = {g_nStringIdINIT, g_nStringIdEI};

const TDataIOID FORTE_RT_E_SPLIT::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_RT_E_SPLIT::scm_anEOWithIndexes[] = {0, -1, -1, -1};
const CStringDictionary::TStringId FORTE_RT_E_SPLIT::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdEO1, g_nStringIdEO2};

const SFBInterfaceSpec FORTE_RT_E_SPLIT::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  3,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  6,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


void FORTE_RT_E_SPLIT::executeEvent(int pa_nEIID){
  if(scm_nEventEIID == pa_nEIID){
    if(m_bInitialized){
      CEventConnection *eoCon;
      eoCon = getEOConUnchecked(scm_nEventEO1ID);
      if(eoCon->isConnected()){
        eoCon->triggerEvent(&m_oECEO1);
        m_oECEO1.resumeSelfSuspend();
      }
      eoCon = getEOConUnchecked(scm_nEventEO2ID);
      if(eoCon->isConnected()){
        eoCon->triggerEvent(&m_oECEO2);
        m_oECEO2.resumeSelfSuspend();
      }
    }
  }
  else
    if(scm_nEventINITID == pa_nEIID){
      if(QI() == true){
        if(!m_bInitialized){
          m_oECEO1.changeExecutionState(cg_nMGM_CMD_Start);
          m_oECEO2.changeExecutionState(cg_nMGM_CMD_Start);
          m_bInitialized = true;
        }
        m_oECEO1.setDeadline(Deadline_EO1());
        m_oECEO2.setDeadline(Deadline_EO2());
      }
      else{
        m_oECEO1.changeExecutionState(cg_nMGM_CMD_Stop);
        m_oECEO2.changeExecutionState(cg_nMGM_CMD_Stop);
        m_bInitialized = false;
      }
      QO() = QI();
      sendOutputEvent(scm_nEventINITOID);
    }
}



