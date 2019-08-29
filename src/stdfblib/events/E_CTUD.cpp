/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 *               2019 Johannes Kepler University Linz
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *   Alois Zoitl - fixed bug in down counting
 *******************************************************************************/
#include "E_CTUD.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_CTUD_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_E_CTUD, g_nStringIdE_CTUD)

const CStringDictionary::TStringId FORTE_E_CTUD::scm_anDataInputNames[] = {g_nStringIdPV};

const CStringDictionary::TStringId FORTE_E_CTUD::scm_anDataInputTypeIds[] = {g_nStringIdUINT};

const CStringDictionary::TStringId FORTE_E_CTUD::scm_anDataOutputNames[] = {g_nStringIdQU, g_nStringIdQD, g_nStringIdCV};

const CStringDictionary::TStringId FORTE_E_CTUD::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdUINT};

const TForteInt16 FORTE_E_CTUD::scm_anEIWithIndexes[] = {0, -1, -1, 2};
const TDataIOID FORTE_E_CTUD::scm_anEIWith[] = {0, 255, 0, 255};
const CStringDictionary::TStringId FORTE_E_CTUD::scm_anEventInputNames[] = {g_nStringIdCU, g_nStringIdCD, g_nStringIdR, g_nStringIdLD};

const TDataIOID FORTE_E_CTUD::scm_anEOWith[] = {0, 2, 1, 255, 0, 2, 1, 255, 0, 1, 2, 255};
const TForteInt16 FORTE_E_CTUD::scm_anEOWithIndexes[] = {0, 4, 8, -1};
const CStringDictionary::TStringId FORTE_E_CTUD::scm_anEventOutputNames[] = {g_nStringIdCO, g_nStringIdRO, g_nStringIdLDO};

const SFBInterfaceSpec FORTE_E_CTUD::scm_stFBInterfaceSpec = {
  4,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  3,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  3,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

void FORTE_E_CTUD::alg_CountUp(void){
CV() = static_cast<TForteUInt16>(CV() + 1);


}

void FORTE_E_CTUD::alg_Reset(void){
CV() = 0;

}

void FORTE_E_CTUD::alg_Load(void){
CV() = PV();
}

void FORTE_E_CTUD::alg_UpdateQUQD(void){
QU() = ((CV() >= PV()));
QD() = ((CV() == 0));
}

void FORTE_E_CTUD::alg_CountDown(void){
CV() = static_cast<TForteUInt16>(CV() - 1);
}


void FORTE_E_CTUD::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void FORTE_E_CTUD::enterStateCU(void){
  m_nECCState = scm_nStateCU;
  alg_CountUp();
  alg_UpdateQUQD();
  sendOutputEvent( scm_nEventCOID);
}

void FORTE_E_CTUD::enterStateR(void){
  m_nECCState = scm_nStateR;
  alg_Reset();
  alg_UpdateQUQD();
  sendOutputEvent( scm_nEventROID);
}

void FORTE_E_CTUD::enterStateCD(void){
  m_nECCState = scm_nStateCD;
  alg_CountDown();
  alg_UpdateQUQD();
  sendOutputEvent( scm_nEventCOID);
}

void FORTE_E_CTUD::enterStateLD(void){
  m_nECCState = scm_nStateLD;
  alg_Load();
  alg_UpdateQUQD();
  sendOutputEvent( scm_nEventLDOID);
}

void FORTE_E_CTUD::executeEvent(int pa_nEIID){
  bool bTransitionCleared;
  do{
    bTransitionCleared = true;
    switch(m_nECCState){
      case scm_nStateSTART:
        if((scm_nEventCUID == pa_nEIID) && (((CV() < 65535))))
          enterStateCU();
        else
        if(scm_nEventRID == pa_nEIID)
          enterStateR();
        else
        if((scm_nEventCDID == pa_nEIID) && (((((CV() > 0))))))
          enterStateCD();
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
      case scm_nStateR:
        if(1)
          enterStateSTART();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateCD:
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
      DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 4.", m_nECCState.operator TForteUInt16 ());
        m_nECCState = 0; //0 is always the initial state
        break;
    }
    pa_nEIID = cg_nInvalidEventID;  // we have to clear the event after the first check in order to ensure correct behavior
  }while(bTransitionCleared);
}


