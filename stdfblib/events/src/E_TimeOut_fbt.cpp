/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "forte/iec61499/events/E_TimeOut_fbt.h"

using namespace forte::literals;

namespace forte::iec61499::events {
  namespace {
    const auto cSocketNameIds = std::array{"TimeOutSocket"_STRID};

    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = {},
        .mEITypeNames = {},
        .mEONames = {},
        .mEOTypeNames = {},
        .mDINames = {},
        .mDONames = {},
        .mDIONames = {},
        .mSocketNames = cSocketNameIds,
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_TimeOut, "iec61499::events::E_TimeOut"_STRID)

  FORTE_E_TimeOut::FORTE_E_TimeOut(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CEventSourceFB(paContainer, cFBInterfaceSpec, paInstanceNameId),
      mActive(false),
      var_TimeOutSocket("TimeOutSocket"_STRID, *this, 0) {};

  bool FORTE_E_TimeOut::initialize() {
    return CEventSourceFB::initialize();
  }

  void FORTE_E_TimeOut::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if (cgExternalEventID == paEIID) {
      mActive = false;
      sendAdapterEvent(*var_TimeOutSocket, FORTE_ATimeOut::scmEventTimeOutID, paECET);
    } else if (var_TimeOutSocket->evt_START() == paEIID) {
      if (!mActive) {
        setEventChainExecutor(
            paECET); // delay notification should be execute in the same thread on as from where it has been triggered.
        getTimer().registerOneShotTimedFB(this, var_TimeOutSocket->var_DT);
        mActive = true;
      }
    } else if (var_TimeOutSocket->evt_STOP() == paEIID) {
      if (mActive) {
        getTimer().unregisterTimedFB(this);
        mActive = false;
      }
    }
  }

  void FORTE_E_TimeOut::readInputData(TEventID) {
    // nothing to do
  }

  void FORTE_E_TimeOut::writeOutputData(TEventID) {
    // nothing to do
  }

  CIEC_ANY *FORTE_E_TimeOut::getDI(size_t) {
    return nullptr;
  }

  CIEC_ANY *FORTE_E_TimeOut::getDO(size_t) {
    return nullptr;
  }

  forte::ISocketPin *FORTE_E_TimeOut::getSocketPinUnchecked(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_TimeOutSocket;
    }
    return nullptr;
  }

  CEventConnection *FORTE_E_TimeOut::getEOConUnchecked(TPortId) {
    return nullptr;
  }

  CDataConnection **FORTE_E_TimeOut::getDIConUnchecked(TPortId) {
    return nullptr;
  }

  CDataConnection *FORTE_E_TimeOut::getDOConUnchecked(TPortId) {
    return nullptr;
  }

  EMGMResponse FORTE_E_TimeOut::changeExecutionState(EMGMCommandType paCommand) {
    EMGMResponse eRetVal = CFunctionBlock::changeExecutionState(paCommand);
    if ((EMGMResponse::Ready == eRetVal) &&
        ((EMGMCommandType::Stop == paCommand) || (EMGMCommandType::Kill == paCommand))) {
      if (mActive) {
        getTimer().unregisterTimedFB(this);
        mActive = false;
      }
    }
    return eRetVal;
  }

  void FORTE_E_TimeOut::setInitialValues() {
    // no data ports so nothing to do
  }

} // namespace forte::iec61499::events
