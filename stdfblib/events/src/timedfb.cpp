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
#include "forte/iec61499/events/timedfb.h"

using namespace forte::literals;

namespace forte::iec61499::events {
  namespace {
    const auto cEINameIds = std::array{"START"_STRID, "STOP"_STRID};
    const auto cEONameIds = std::array{"EO"_STRID};
    const auto cDINameIds = std::array{"DT"_STRID};

    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEINameIds,
        .mEITypeNames = {},
        .mEONames = cEONameIds,
        .mEOTypeNames = {},
        .mDINames = cDINameIds,
        .mDONames = {},
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  } // namespace

  CTimedFB::CTimedFB(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
      CEventSourceFB(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_DT(0_TIME),
      conn_DT(nullptr),
      conn_EO(*this, 0) {
    setEventChainExecutor(getResource()->getResourceEventExecution());
    mActive = false;
  }

  void CTimedFB::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case cgExternalEventID: sendOutputEvent(csmEOID, paECET); break;
      case csmEventSTOPID:
        if (mActive) {
          getTimer().unregisterTimedFB(this);
          mActive = false;
        }
        break;
      default: break;
    }
  }

  void CTimedFB::readInputData(TEventID) {
    readData(0, var_DT, conn_DT);
  }

  void CTimedFB::writeOutputData(TEventID) {
  }

  CIEC_ANY *CTimedFB::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_DT;
    }
    return nullptr;
  }

  CIEC_ANY *CTimedFB::getDO(const size_t) {
    return nullptr;
  }

  CEventConnection *CTimedFB::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_EO;
    }
    return nullptr;
  }

  CDataConnection **CTimedFB::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_DT;
    }
    return nullptr;
  }

  CDataConnection *CTimedFB::getDOConUnchecked(const TPortId) {
    return nullptr;
  }

  EMGMResponse CTimedFB::changeExecutionState(EMGMCommandType paCommand) {
    EMGMResponse eRetVal = CFunctionBlock::changeExecutionState(paCommand);
    if ((EMGMResponse::Ready == eRetVal) &&
        ((EMGMCommandType::Stop == paCommand) || (EMGMCommandType::Kill == paCommand)) && mActive) {
      getTimer().unregisterTimedFB(this);
      mActive = false;
    }
    return eRetVal;
  }

  void CTimedFB::setInitialValues() {
    var_DT = 0_TIME;
  }
} // namespace forte::iec61499::events
