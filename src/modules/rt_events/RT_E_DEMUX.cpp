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

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_RT_E_DEMUX, g_nStringIdRT_E_DEMUX)

const CStringDictionary::TStringId FORTE_RT_E_DEMUX::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdK, g_nStringIdTmin, g_nStringIdDeadline0, g_nStringIdWCET0, g_nStringIdDeadline1, g_nStringIdWCET1, g_nStringIdDeadline2, g_nStringIdWCET2, g_nStringIdDeadline3, g_nStringIdWCET3};
const CStringDictionary::TStringId FORTE_RT_E_DEMUX::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdUINT, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME};
const CStringDictionary::TStringId FORTE_RT_E_DEMUX::scm_anDataOutputNames[] = {g_nStringIdQO};
const CStringDictionary::TStringId FORTE_RT_E_DEMUX::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL};
const TDataIOID FORTE_RT_E_DEMUX::scm_anEIWith[] = {0, 2, 3, 4, 5, 6, 7, 8, 9, 10, scmWithListDelimiter, 1, scmWithListDelimiter};
const TForteInt16 FORTE_RT_E_DEMUX::scm_anEIWithIndexes[] = {0, 11};
const CStringDictionary::TStringId FORTE_RT_E_DEMUX::scm_anEventInputNames[] = {g_nStringIdINIT, g_nStringIdEI};
const TDataIOID FORTE_RT_E_DEMUX::scm_anEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_RT_E_DEMUX::scm_anEOWithIndexes[] = {0, -1, -1, -1, -1};
const CStringDictionary::TStringId FORTE_RT_E_DEMUX::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdEO0, g_nStringIdEO1, g_nStringIdEO2, g_nStringIdEO3};
const SFBInterfaceSpec FORTE_RT_E_DEMUX::scm_stFBInterfaceSpec = {
  2, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  5, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  11, scm_anDataInputNames, scm_anDataInputTypeIds,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_RT_E_DEMUX::FORTE_RT_E_DEMUX(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CFunctionBlock( pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId),
    var_conn_QO(var_QO),
    conn_INITO(this, 0),
    conn_EO0(this, 1),
    conn_EO1(this, 2),
    conn_EO2(this, 3),
    conn_EO3(this, 4),
    conn_QI(nullptr),
    conn_K(nullptr),
    conn_Tmin(nullptr),
    conn_Deadline0(nullptr),
    conn_WCET0(nullptr),
    conn_Deadline1(nullptr),
    conn_WCET1(nullptr),
    conn_Deadline2(nullptr),
    conn_WCET2(nullptr),
    conn_Deadline3(nullptr),
    conn_WCET3(nullptr),
    conn_QO(this, 0, &var_conn_QO) {
};

void FORTE_RT_E_DEMUX::setInitialValues() {
  var_QI = 0_BOOL;
  var_K = 0_UINT;
  var_Tmin = 0_TIME;
  var_Deadline0 = 0_TIME;
  var_WCET0 = 0_TIME;
  var_Deadline1 = 0_TIME;
  var_WCET1 = 0_TIME;
  var_Deadline2 = 0_TIME;
  var_WCET2 = 0_TIME;
  var_Deadline3 = 0_TIME;
  var_WCET3 = 0_TIME;
  var_QO = 0_BOOL;
}

void FORTE_RT_E_DEMUX::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scm_nEventEIID:
      if(m_bInitialized){
        CEventConnection *eoCon;
        switch(static_cast<CIEC_UINT::TValueType>(var_K)){
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
      break;
    case scm_nEventINITID:
      if(var_QI){
        if(!m_bInitialized){
          m_oECEO0.changeExecutionState(EMGMCommandType::Start);
          m_oECEO1.changeExecutionState(EMGMCommandType::Start);
          m_oECEO2.changeExecutionState(EMGMCommandType::Start);
          m_oECEO3.changeExecutionState(EMGMCommandType::Start);
          m_bInitialized = true;
        }
        m_oECEO0.setDeadline(var_Deadline0);
        m_oECEO1.setDeadline(var_Deadline1);
        m_oECEO2.setDeadline(var_Deadline2);
        m_oECEO3.setDeadline(var_Deadline3);
      }
      else{
        m_oECEO0.changeExecutionState(EMGMCommandType::Stop);
        m_oECEO1.changeExecutionState(EMGMCommandType::Stop);
        m_oECEO2.changeExecutionState(EMGMCommandType::Stop);
        m_oECEO3.changeExecutionState(EMGMCommandType::Stop);
        m_bInitialized = false;
      }
      var_QO = var_QI;
      sendOutputEvent(scm_nEventINITOID, paECET);
      break;
  }
}

void FORTE_RT_E_DEMUX::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scm_nEventINITID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, var_QI, conn_QI);
      readData(2, var_Tmin, conn_Tmin);
      readData(3, var_Deadline0, conn_Deadline0);
      readData(4, var_WCET0, conn_WCET0);
      readData(5, var_Deadline1, conn_Deadline1);
      readData(6, var_WCET1, conn_WCET1);
      readData(7, var_Deadline2, conn_Deadline2);
      readData(8, var_WCET2, conn_WCET2);
      readData(9, var_Deadline3, conn_Deadline3);
      readData(10, var_WCET3, conn_WCET3);
      break;
    }
    case scm_nEventEIID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(1, var_K, conn_K);
      break;
    }
    default:
      break;
  }
}

void FORTE_RT_E_DEMUX::writeOutputData(TEventID paEIID) {
  switch(paEIID) {
    case scm_nEventINITOID: {
      RES_DATA_CON_CRITICAL_REGION();
      writeData(0, var_QO, conn_QO);
      break;
    }
    case scm_nEventEO0ID: {
      RES_DATA_CON_CRITICAL_REGION();
      break;
    }
    case scm_nEventEO1ID: {
      RES_DATA_CON_CRITICAL_REGION();
      break;
    }
    case scm_nEventEO2ID: {
      RES_DATA_CON_CRITICAL_REGION();
      break;
    }
    case scm_nEventEO3ID: {
      RES_DATA_CON_CRITICAL_REGION();
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_RT_E_DEMUX::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_K;
    case 2: return &var_Tmin;
    case 3: return &var_Deadline0;
    case 4: return &var_WCET0;
    case 5: return &var_Deadline1;
    case 6: return &var_WCET1;
    case 7: return &var_Deadline2;
    case 8: return &var_WCET2;
    case 9: return &var_Deadline3;
    case 10: return &var_WCET3;
  }
  return nullptr;
}

CIEC_ANY *FORTE_RT_E_DEMUX::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
  }
  return nullptr;
}

CIEC_ANY *FORTE_RT_E_DEMUX::getDIO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_RT_E_DEMUX::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_EO0;
    case 2: return &conn_EO1;
    case 3: return &conn_EO2;
    case 4: return &conn_EO3;
  }
  return nullptr;
}

CDataConnection **FORTE_RT_E_DEMUX::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_K;
    case 2: return &conn_Tmin;
    case 3: return &conn_Deadline0;
    case 4: return &conn_WCET0;
    case 5: return &conn_Deadline1;
    case 6: return &conn_WCET1;
    case 7: return &conn_Deadline2;
    case 8: return &conn_WCET2;
    case 9: return &conn_Deadline3;
    case 10: return &conn_WCET3;
  }
  return nullptr;
}

CDataConnection *FORTE_RT_E_DEMUX::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
  }
  return nullptr;
}

CInOutDataConnection **FORTE_RT_E_DEMUX::getDIOInConUnchecked(TPortId) {
  return nullptr;
}

CInOutDataConnection *FORTE_RT_E_DEMUX::getDIOOutConUnchecked(TPortId) {
  return nullptr;
}


