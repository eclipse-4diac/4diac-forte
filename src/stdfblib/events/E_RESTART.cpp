/*******************************************************************************
 * Copyright (c) 2005 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Rene Smodic, Gerhard Ebenhofer,
 *   Martin Melik Merkumians, Ingo Hegny, Matthias Plasch
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "E_RESTART.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_RESTART_gen.cpp"
#endif
#include "../../core/device.h"

DEFINE_FIRMWARE_FB(E_RESTART, g_nStringIdE_RESTART)

const CStringDictionary::TStringId E_RESTART::scmEONameIds[] = {g_nStringIdCOLD, g_nStringIdWARM, g_nStringIdSTOP};

const TEventID E_RESTART::csmCOLDID;
const TEventID E_RESTART::csmWARMID;
const TEventID E_RESTART::csmSTOPID;

const SFBInterfaceSpec E_RESTART::scmFBInterfaceSpec = {
  0, nullptr, nullptr, nullptr,
  3, scmEONameIds, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};

void E_RESTART::executeEvent(TEventID paEIID) {
  if(cgExternalEventID == paEIID && cgInvalidEventID != mEventToSend) {
    sendOutputEvent(mEventToSend);
    if(csmSTOPID == mEventToSend) {
      //stop event is sent put the FB finally into the stopped state
      CFunctionBlock::changeFBExecutionState(EMGMCommandType::Stop);
      // release semaphore to indicate that the stop event was sent now
      mSuspendSemaphore.inc();
    }
  }
}

void E_RESTART::readInputData(TEventID) {
}

void E_RESTART::writeOutputData(TEventID) {
}

EMGMResponse E_RESTART::changeFBExecutionState(EMGMCommandType paCommand){
  EMGMResponse eRetVal = CFunctionBlock::changeFBExecutionState(paCommand);
  if(EMGMResponse::Ready == eRetVal){
    switch(paCommand){
      case EMGMCommandType::Start:
        mEventToSend = (csmSTOPID == mEventToSend) ? csmWARMID : csmCOLDID;
        getResource().getDevice().getDeviceExecution().startNewEventChain(this);
        break;
      case EMGMCommandType::Stop:
        mEventToSend = csmSTOPID;
        CFunctionBlock::changeFBExecutionState(EMGMCommandType::Start);   //keep FB in running state until stop event is delivered.
        getResource().getDevice().getDeviceExecution().startNewEventChain(this);
        // wait until semaphore is released, after STOP eventExecution was completed
        mSuspendSemaphore.waitIndefinitely();
        break;
      default:
        mEventToSend = cgInvalidEventID;
        break;
    }
  }
  return eRetVal;
}
