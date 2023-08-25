/*******************************************************************************
 * Copyright (c) 2005, 2023 ACIN, Profactor GmbH, fortiss GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gunnar Grabmair, Ingo Hegny, GErhard Ebenhofer
 *    - initial API and implementation and/or initial documentation
 *   Martin Jobst - add generic readInputData and writeOutputData
 *******************************************************************************/
#include "timedfb.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "timedfb_gen.cpp"
#endif

#include "resource.h"
#include "criticalregion.h"

const CStringDictionary::TStringId CTimedFB::scmEINameIds[] = {g_nStringIdSTART, g_nStringIdSTOP};
const CStringDictionary::TStringId CTimedFB::scmEONameIds[] = {g_nStringIdEO};

const CStringDictionary::TStringId CTimedFB::scmDINameIds[] = {g_nStringIdDT};
const CStringDictionary::TStringId CTimedFB::scmDIDataTypeNameIds[] = {g_nStringIdTIME};

const SFBInterfaceSpec CTimedFB::scmFBInterfaceSpec = {
  2, scmEINameIds, nullptr, nullptr,
  1, scmEONameIds, nullptr, nullptr,
  1, scmDINameIds, scmDIDataTypeNameIds,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};

CTimedFB::CTimedFB(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes, ETimerActivationType paType) :
      CEventSourceFB( paSrcRes, &scmFBInterfaceSpec, paInstanceNameId){
  setEventChainExecutor(paSrcRes->getResourceEventExecution());
  mActive = false;
  mTimeListEntry.mTimeOut = 0;
  mTimeListEntry.mInterval = 0;
  mTimeListEntry.mNext = nullptr;
  mTimeListEntry.mType = paType;
  mTimeListEntry.mTimedFB = this;
}

void CTimedFB::executeEvent(TEventID paEIID, CEventChainExecutionThread * const ){
  switch(paEIID){
    case cgExternalEventID:
      sendOutputEvent(csmEOID, getEventChainExecutor());
      break;
    case csmEventSTOPID:
      if(mActive){
        getTimer().unregisterTimedFB(this);
        mActive = false;
      }
      break;
    case csmEventSTARTID:
      if(!mActive){
        getTimer().registerTimedFB( &mTimeListEntry, DT());
        mActive = true;
      }
      break;
    default:
      break;
  }
}

void CTimedFB::readInputData(TEventID) {
  RES_DATA_CON_CRITICAL_REGION();
  readData(0, *mDIs[0], mDIConns[0]);
}

void CTimedFB::writeOutputData(TEventID) {
}

EMGMResponse CTimedFB::changeFBExecutionState(EMGMCommandType paCommand){
  EMGMResponse eRetVal = CFunctionBlock::changeFBExecutionState(paCommand);
  if((EMGMResponse::Ready == eRetVal) && ((EMGMCommandType::Stop == paCommand) || (EMGMCommandType::Kill == paCommand)) && mActive) {
    getTimer().unregisterTimedFB(this);
    mActive = false;
  }
  return eRetVal;
}
