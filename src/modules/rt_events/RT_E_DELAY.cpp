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
#include "RT_E_DELAY.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "RT_E_DELAY_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_RT_E_DELAY, g_nStringIdRT_E_DELAY)

const CStringDictionary::TStringId FORTE_RT_E_DELAY::scmDataInputNames[] = {g_nStringIdQI, g_nStringIdDT, g_nStringIdTmin, g_nStringIdDeadline, g_nStringIdWCET};
const CStringDictionary::TStringId FORTE_RT_E_DELAY::scmDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME, g_nStringIdTIME};
const CStringDictionary::TStringId FORTE_RT_E_DELAY::scmDataOutputNames[] = {g_nStringIdQO};
const CStringDictionary::TStringId FORTE_RT_E_DELAY::scmDataOutputTypeIds[] = {g_nStringIdBOOL};
const TDataIOID FORTE_RT_E_DELAY::scmEIWith[] = {0, 2, 3, 4, scmWithListDelimiter, 1, scmWithListDelimiter};
const TForteInt16 FORTE_RT_E_DELAY::scmEIWithIndexes[] = {0, 5, -1};
const CStringDictionary::TStringId FORTE_RT_E_DELAY::scmEventInputNames[] = {g_nStringIdINIT, g_nStringIdSTART, g_nStringIdSTOP};
const TDataIOID FORTE_RT_E_DELAY::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_RT_E_DELAY::scmEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_RT_E_DELAY::scmEventOutputNames[] = {g_nStringIdINITO, g_nStringIdEO};
const SFBInterfaceSpec FORTE_RT_E_DELAY::scmFBInterfaceSpec = {
  3, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  2, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  5, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_RT_E_DELAY::FORTE_RT_E_DELAY(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
        CEventSourceFB(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_conn_QO(var_QO),
    conn_INITO(this, 0),
    conn_EO(this, 1),
    conn_QI(nullptr),
    conn_DT(nullptr),
    conn_Tmin(nullptr),
    conn_Deadline(nullptr),
    conn_WCET(nullptr),
    conn_QO(this, 0, &var_conn_QO) {
  setEventChainExecutor(&mECEO);
};

void FORTE_RT_E_DELAY::setInitialValues() {
  var_QI = 0_BOOL;
  var_DT = 0_TIME;
  var_Tmin = 0_TIME;
  var_Deadline = 0_TIME;
  var_WCET = 0_TIME;
  var_QO = 0_BOOL;
}

void FORTE_RT_E_DELAY::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case cgExternalEventID:
      sendOutputEvent(scmEventEOID, paECET);
      mActive = false;
      break;
    case scmEventSTOPID:
      if(mActive){
        getTimer().unregisterTimedFB(this);
        mActive = false;
      }
      break;
    case scmEventSTARTID:
      if((!mActive)&&(mInitialized)){
        getTimer().registerOneShotTimedFB(this, var_DT);
        mActive = true;
      }
      break;
    case scmEventINITID:
      if(var_QI == true){
        if(!mInitialized){
          //mECEO.start();
          mInitialized = true;
        }
        mECEO.setDeadline(var_Deadline);
      }
      else{
        mInitialized = false;
        mECEO.setDeadline(CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0)));
      }
      var_QO = var_QI;
      sendOutputEvent(scmEventINITOID, paECET);
      break;
  }
}

EMGMResponse FORTE_RT_E_DELAY::changeExecutionState(EMGMCommandType paCommand){
  mECEO.changeExecutionState(paCommand);
  EMGMResponse eRetVal = CFunctionBlock::changeExecutionState(paCommand);
  if((EMGMResponse::Ready == eRetVal) && ((EMGMCommandType::Stop == paCommand) || (EMGMCommandType::Kill == paCommand)) && mActive) {
    getTimer().unregisterTimedFB(this);
    mActive = false;
  }
  return eRetVal;
}

void FORTE_RT_E_DELAY::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITID: {
      readData(0, var_QI, conn_QI);
      readData(2, var_Tmin, conn_Tmin);
      readData(3, var_Deadline, conn_Deadline);
      readData(4, var_WCET, conn_WCET);
      break;
    }
    case scmEventSTARTID: {
      readData(1, var_DT, conn_DT);
      break;
    }
    case scmEventSTOPID: {
      break;
    }
    default:
      break;
  }
}

void FORTE_RT_E_DELAY::writeOutputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITOID: {
      writeData(0, var_QO, conn_QO);
      break;
    }
    case scmEventEOID: {
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_RT_E_DELAY::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_DT;
    case 2: return &var_Tmin;
    case 3: return &var_Deadline;
    case 4: return &var_WCET;
  }
  return nullptr;
}

CIEC_ANY *FORTE_RT_E_DELAY::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
  }
  return nullptr;
}

CEventConnection *FORTE_RT_E_DELAY::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_EO;
  }
  return nullptr;
}

CDataConnection **FORTE_RT_E_DELAY::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_DT;
    case 2: return &conn_Tmin;
    case 3: return &conn_Deadline;
    case 4: return &conn_WCET;
  }
  return nullptr;
}

CDataConnection *FORTE_RT_E_DELAY::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
  }
  return nullptr;
}
