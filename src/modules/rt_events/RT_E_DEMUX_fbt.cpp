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
#include "RT_E_DEMUX_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(Deadline0);
USE_STRING_ID(Deadline1);
USE_STRING_ID(Deadline2);
USE_STRING_ID(Deadline3);
USE_STRING_ID(EI);
USE_STRING_ID(EInit);
USE_STRING_ID(EO0);
USE_STRING_ID(EO1);
USE_STRING_ID(EO2);
USE_STRING_ID(EO3);
USE_STRING_ID(Event);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(K);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(RT_E_DEMUX);
USE_STRING_ID(TIME);
USE_STRING_ID(Tmin);
USE_STRING_ID(UINT);
USE_STRING_ID(WCET0);
USE_STRING_ID(WCET1);
USE_STRING_ID(WCET2);
USE_STRING_ID(WCET3);


#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_RT_E_DEMUX, STRID(RT_E_DEMUX))

const CStringDictionary::TStringId FORTE_RT_E_DEMUX::scmDataInputNames[] = {STRID(QI), STRID(K), STRID(Tmin), STRID(Deadline0), STRID(WCET0), STRID(Deadline1), STRID(WCET1), STRID(Deadline2), STRID(WCET2), STRID(Deadline3), STRID(WCET3)};
const CStringDictionary::TStringId FORTE_RT_E_DEMUX::scmDataInputTypeIds[] = {STRID(BOOL), STRID(UINT), STRID(TIME), STRID(TIME), STRID(TIME), STRID(TIME), STRID(TIME), STRID(TIME), STRID(TIME), STRID(TIME), STRID(TIME)};
const CStringDictionary::TStringId FORTE_RT_E_DEMUX::scmDataOutputNames[] = {STRID(QO)};
const CStringDictionary::TStringId FORTE_RT_E_DEMUX::scmDataOutputTypeIds[] = {STRID(BOOL)};
const TDataIOID FORTE_RT_E_DEMUX::scmEIWith[] = {0, 2, 3, 4, 5, 6, 7, 8, 9, 10, scmWithListDelimiter, 1, scmWithListDelimiter};
const TForteInt16 FORTE_RT_E_DEMUX::scmEIWithIndexes[] = {0, 11};
const CStringDictionary::TStringId FORTE_RT_E_DEMUX::scmEventInputNames[] = {STRID(INIT), STRID(EI)};
const CStringDictionary::TStringId FORTE_RT_E_DEMUX::scmEventInputTypeIds[] = {STRID(EInit), STRID(Event)};
const TDataIOID FORTE_RT_E_DEMUX::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_RT_E_DEMUX::scmEOWithIndexes[] = {0, -1, -1, -1, -1};
const CStringDictionary::TStringId FORTE_RT_E_DEMUX::scmEventOutputNames[] = {STRID(INITO), STRID(EO0), STRID(EO1), STRID(EO2), STRID(EO3)};
const CStringDictionary::TStringId FORTE_RT_E_DEMUX::scmEventOutputTypeIds[] = {STRID(Event), STRID(Event), STRID(Event), STRID(Event), STRID(Event)};
const SFBInterfaceSpec FORTE_RT_E_DEMUX::scmFBInterfaceSpec = {
  2, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  5, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  11, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_RT_E_DEMUX::FORTE_RT_E_DEMUX(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
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
    case scmEventEIID:
      if(mInitialized){
        CEventConnection *eoCon;
        switch(static_cast<CIEC_UINT::TValueType>(var_K)){
          case 0:
            eoCon = getEOConUnchecked(scmEventEO0ID);
            if(eoCon->isConnected()){
              eoCon->triggerEvent(&mECEO0);
              mECEO0.resumeSelfSuspend();
            }
            break;
          case 1:
            eoCon = getEOConUnchecked(scmEventEO1ID);
            if(eoCon->isConnected()){
              eoCon->triggerEvent(&mECEO1);
              mECEO1.resumeSelfSuspend();
            }
            break;
          case 2:
            eoCon = getEOConUnchecked(scmEventEO2ID);
            if(eoCon->isConnected()){
              eoCon->triggerEvent(&mECEO2);
              mECEO2.resumeSelfSuspend();
            }
            break;
          case 3:
            eoCon = getEOConUnchecked(scmEventEO3ID);
            if(eoCon->isConnected()){
              eoCon->triggerEvent(&mECEO3);
              mECEO3.resumeSelfSuspend();
            }
            break;
          default:
            break;
        }
      }
      break;
    case scmEventINITID:
      if(var_QI){
        if(!mInitialized){
          mECEO0.changeExecutionState(EMGMCommandType::Start);
          mECEO1.changeExecutionState(EMGMCommandType::Start);
          mECEO2.changeExecutionState(EMGMCommandType::Start);
          mECEO3.changeExecutionState(EMGMCommandType::Start);
          mInitialized = true;
        }
        mECEO0.setDeadline(var_Deadline0);
        mECEO1.setDeadline(var_Deadline1);
        mECEO2.setDeadline(var_Deadline2);
        mECEO3.setDeadline(var_Deadline3);
      }
      else{
        mECEO0.changeExecutionState(EMGMCommandType::Stop);
        mECEO1.changeExecutionState(EMGMCommandType::Stop);
        mECEO2.changeExecutionState(EMGMCommandType::Stop);
        mECEO3.changeExecutionState(EMGMCommandType::Stop);
        mInitialized = false;
      }
      var_QO = var_QI;
      sendOutputEvent(scmEventINITOID, paECET);
      break;
  }
}

void FORTE_RT_E_DEMUX::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITID: {
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
    case scmEventEIID: {
      readData(1, var_K, conn_K);
      break;
    }
    default:
      break;
  }
}

void FORTE_RT_E_DEMUX::writeOutputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITOID: {
      writeData(0, var_QO, conn_QO);
      break;
    }
    case scmEventEO0ID: {
      break;
    }
    case scmEventEO1ID: {
      break;
    }
    case scmEventEO2ID: {
      break;
    }
    case scmEventEO3ID: {
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


