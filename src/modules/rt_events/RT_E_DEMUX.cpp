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
#include "RT_E_DEMUX.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "RT_E_DEMUX_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_RT_E_DEMUX, g_nStringIdRT_E_DEMUX)

const CStringDictionary::TStringId FORTE_RT_E_DEMUX::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdK, g_nStringIdTmin, g_nStringIdDeadline0, g_nStringIdWCET0, g_nStringIdDeadline1, g_nStringIdWCET1, g_nStringIdDeadline2, g_nStringIdWCET2, g_nStringIdDeadline3, g_nStringIdWCET3};

const CStringDictionary::TStringId FORTE_RT_E_DEMUX::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdUINT, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME};

const CStringDictionary::TStringId FORTE_RT_E_DEMUX::scm_anDataOutputNames[] = {g_nStringIdQO};

const CStringDictionary::TStringId FORTE_RT_E_DEMUX::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 FORTE_RT_E_DEMUX::scm_anEIWithIndexes[] = {0, 11};
const TDataIOID FORTE_RT_E_DEMUX::scm_anEIWith[] = {0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 255, 1, 255};
const CStringDictionary::TStringId FORTE_RT_E_DEMUX::scm_anEventInputNames[] = {g_nStringIdINIT, g_nStringIdEI};

const TDataIOID FORTE_RT_E_DEMUX::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_RT_E_DEMUX::scm_anEOWithIndexes[] = {0, -1, -1, -1, -1, -1};
const CStringDictionary::TStringId FORTE_RT_E_DEMUX::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdEO0, g_nStringIdEO1, g_nStringIdEO2, g_nStringIdEO3};

const SFBInterfaceSpec FORTE_RT_E_DEMUX::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  5,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  11,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

FORTE_RT_E_DEMUX::FORTE_RT_E_DEMUX(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
       CFunctionBlock(
           pa_poSrcRes,
           &scm_stFBInterfaceSpec, pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData) {
    m_bInitialized = false;
}

void FORTE_RT_E_DEMUX::executeEvent(int pa_nEIID){

   if(scm_nEventEIID == pa_nEIID){
    if(m_bInitialized){
      CEventConnection *eoCon;
      switch(K()){
        case 0:
          eoCon = getEOConUnchecked(scm_nEventEO0ID);
          if(eoCon->isConnected()){
            eoCon->triggerEvent(&m_oECEO0);
            m_oECEO0.resumeSelfSuspend();
          }
          break;
        case 1:
          eoCon = getEOConUnchecked(scm_nEventEO1ID);
          if(eoCon->isConnected()){
            eoCon->triggerEvent(&m_oECEO1);
            m_oECEO1.resumeSelfSuspend();
          }
          break;
        case 2:
          eoCon = getEOConUnchecked(scm_nEventEO2ID);
          if(eoCon->isConnected()){
            eoCon->triggerEvent(&m_oECEO2);
            m_oECEO2.resumeSelfSuspend();
          }
          break;
        case 3:
          eoCon = getEOConUnchecked(scm_nEventEO3ID);
          if(eoCon->isConnected()){
            eoCon->triggerEvent(&m_oECEO3);
            m_oECEO3.resumeSelfSuspend();
          }
          break;
        default:
          break;
      }
   }
  }
  else
    if(scm_nEventINITID == pa_nEIID){
      if(QI() == true){
        if(!m_bInitialized){
          m_oECEO0.changeExecutionState(cg_nMGM_CMD_Start);
          m_oECEO1.changeExecutionState(cg_nMGM_CMD_Start);
          m_oECEO2.changeExecutionState(cg_nMGM_CMD_Start);
          m_oECEO3.changeExecutionState(cg_nMGM_CMD_Start);
          m_bInitialized = true;
        }
        m_oECEO0.setDeadline(Deadline0());
        m_oECEO1.setDeadline(Deadline1());
        m_oECEO2.setDeadline(Deadline2());
        m_oECEO3.setDeadline(Deadline3());
      }
      else{
        m_oECEO0.changeExecutionState(cg_nMGM_CMD_Stop);
        m_oECEO1.changeExecutionState(cg_nMGM_CMD_Stop);
        m_oECEO2.changeExecutionState(cg_nMGM_CMD_Stop);
        m_oECEO3.changeExecutionState(cg_nMGM_CMD_Stop);
        m_bInitialized = false;
      }
    QO() = QI();
      sendOutputEvent(scm_nEventINITOID);
    }
}


