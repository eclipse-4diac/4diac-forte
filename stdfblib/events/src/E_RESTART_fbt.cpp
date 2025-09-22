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
#include "forte/iec61499/events/E_RESTART_fbt.h"

using namespace forte::literals;

#include "forte/device.h"

namespace forte::iec61499::events {
  namespace {
    const auto cEventOutputNames = std::array{"COLD"_STRID, "WARM"_STRID, "STOP"_STRID};
    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = {},
        .mEITypeNames = {},
        .mEONames = cEventOutputNames,
        .mEOTypeNames = {},
        .mDINames = {},
        .mDONames = {},
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_RESTART, "iec61499::events::E_RESTART"_STRID)

  FORTE_E_RESTART::FORTE_E_RESTART(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CEventSourceFB(paContainer, cFBInterfaceSpec, paInstanceNameId),
      mEventToSend(cgInvalidEventID),
      conn_COLD(*this, 0),
      conn_WARM(*this, 1),
      conn_STOP(*this, 2) {
    setEventChainExecutor(getResource()->getResourceEventExecution());
  };

  void FORTE_E_RESTART::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if (cgExternalEventID == paEIID && cgInvalidEventID != mEventToSend) {
      sendOutputEvent(mEventToSend, paECET);
      if (scmEventSTOPID == mEventToSend) {
        // stop event is sent put the FB finally into the stopped state
        CFunctionBlock::changeExecutionState(EMGMCommandType::Stop);
        // release semaphore to indicate that the stop event was sent now
        mSuspendSemaphore.inc();
      }
    }
  }

  void FORTE_E_RESTART::readInputData(TEventID) {
    // nothing to do
  }

  void FORTE_E_RESTART::writeOutputData(TEventID) {
    // nothing to do
  }

  CIEC_ANY *FORTE_E_RESTART::getDI(size_t) {
    return nullptr;
  }

  CIEC_ANY *FORTE_E_RESTART::getDO(size_t) {
    return nullptr;
  }

  CEventConnection *FORTE_E_RESTART::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_COLD;
      case 1: return &conn_WARM;
      case 2: return &conn_STOP;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_RESTART::getDIConUnchecked(TPortId) {
    return nullptr;
  }

  CDataConnection *FORTE_E_RESTART::getDOConUnchecked(TPortId) {
    return nullptr;
  }

  EMGMResponse FORTE_E_RESTART::changeExecutionState(EMGMCommandType paCommand) {
    EMGMResponse eRetVal = CFunctionBlock::changeExecutionState(paCommand);
    if (EMGMResponse::Ready == eRetVal) {
      switch (paCommand) {
        case EMGMCommandType::Start:
          mEventToSend = (scmEventSTOPID == mEventToSend) ? scmEventWARMID : scmEventCOLDID;
          getDevice()->getDeviceExecution().startNewEventChain(this);
          break;
        case EMGMCommandType::Stop:
          mEventToSend = scmEventSTOPID;
          CFunctionBlock::changeExecutionState(
              EMGMCommandType::Start); // keep FB in running state until stop event is delivered.
          getDevice()->getDeviceExecution().startNewEventChain(this);
          // wait until semaphore is released, after STOP eventExecution was completed
          mSuspendSemaphore.waitIndefinitely();
          break;
        default: mEventToSend = cgInvalidEventID; break;
      }
    }
    return eRetVal;
  }

  void FORTE_E_RESTART::setInitialValues() {
    // no variables so nothing to do
  }
} // namespace forte::iec61499::events
