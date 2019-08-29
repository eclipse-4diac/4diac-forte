/*******************************************************************************
 * Copyright (c) 2014 ACIN
 *               2019 Johannes Kepler University Linz
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger - initial API and implementation and/or initial documentation
 *   Alois Zoitl - fixed bug in down counting
 *******************************************************************************/
#include "E_CTD.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_CTD_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_E_CTD, g_nStringIdE_CTD)

const CStringDictionary::TStringId FORTE_E_CTD::scm_anDataInputNames[] = {g_nStringIdPV};

const CStringDictionary::TStringId FORTE_E_CTD::scm_anDataInputTypeIds[] = {g_nStringIdUINT};

const CStringDictionary::TStringId FORTE_E_CTD::scm_anDataOutputNames[] = {g_nStringIdQ, g_nStringIdCV};

const CStringDictionary::TStringId FORTE_E_CTD::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdUINT};

const TForteInt16 FORTE_E_CTD::scm_anEIWithIndexes[] = {-1, 0};
const TDataIOID FORTE_E_CTD::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId FORTE_E_CTD::scm_anEventInputNames[] = {g_nStringIdCD, g_nStringIdLD};

const TDataIOID FORTE_E_CTD::scm_anEOWith[] = {0, 1, 255, 0, 1, 255};
const TForteInt16 FORTE_E_CTD::scm_anEOWithIndexes[] = {0, 3, -1};
const CStringDictionary::TStringId FORTE_E_CTD::scm_anEventOutputNames[] = {g_nStringIdCDO, g_nStringIdLDO};

const SFBInterfaceSpec FORTE_E_CTD::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

void FORTE_E_CTD::alg_CD(void){
CV() = static_cast<TForteUInt16>(CV()-1);
Q() = ((CV() == 0));
}

void FORTE_E_CTD::alg_LD(void){
CV() = PV();
Q() = ((CV() == 0));
}


void FORTE_E_CTD::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void FORTE_E_CTD::enterStateCU(void){
  m_nECCState = scm_nStateCU;
  alg_CD();
  sendOutputEvent( scm_nEventCDOID);
}

void FORTE_E_CTD::enterStateLD(void){
  m_nECCState = scm_nStateLD;
  alg_LD();
  sendOutputEvent( scm_nEventLDOID);
}

void FORTE_E_CTD::executeEvent(int pa_nEIID){
  bool bTransitionCleared;
  do{
    bTransitionCleared = true;
    switch(m_nECCState){
      case scm_nStateSTART:
        if((scm_nEventCDID == pa_nEIID) && (((CV() > 0))))
          enterStateCU();
        else
        if(scm_nEventLDID == pa_nEIID)
          enterStateLD();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateCU:
        if(1)
          enterStateSTART();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateLD:
        if(1)
          enterStateSTART();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      default:
      DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 2.", m_nECCState.operator TForteUInt16 ());
        m_nECCState = 0; //0 is always the initial state
        break;
    }
    pa_nEIID = cg_nInvalidEventID;  // we have to clear the event after the first check in order to ensure correct behavior
  }while(bTransitionCleared);
}


