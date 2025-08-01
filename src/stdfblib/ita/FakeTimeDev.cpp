/*******************************************************************************
 * Copyright (c) 2005, 2024 ACIN, Profactor GmbH, fortiss GmbH
 *                                Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Rene Smodic, Ingo Hegny
 *               - initial API and implementation and/or initial documentation
 *   Alois Zoitl - Copied and modified from CFakeTimeDev.h
 *******************************************************************************/
#include "stdfblib/ita/FakeTimeDev.h"

using namespace forte::core::literals;

#include "core/stringid.h"
#include "../../arch/fake_time/faketimerha.h"

#include "generated/timerhandlerfactory.h"

namespace {
  const auto cDataInputNames = std::array{"MGR_ID"_STRID, "FakeTime"_STRID};

  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = {},
      .mEITypeNames = {},
      .mEONames = {},
      .mEOTypeNames = {},
      .mDINames = cDataInputNames,
      .mDONames = {},
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = {},
  };
} // namespace

FakeTimeDev::FakeTimeDev(const std::string &paMGR_ID) :
    CDevice(cFBInterfaceSpec, initializeTimer()),
    conn_MGR_ID(*this, 0, u""_WSTRING),
    conn_FakeTime(*this, 0, 0_TIME),
    MGR("MGR"_STRID, *this) {
  conn_MGR_ID.getValue().fromString(paMGR_ID.c_str());
}

bool FakeTimeDev::initialize() {
  if (!CDevice::initialize()) {
    return false;
  }

  if (!MGR.initialize()) {
    return false;
  }

  // we need to manually crate this connection as the MGR is not managed by device
  conn_MGR_ID.connect(MGR, std::array{"MGR_ID"_STRID});
  return true;
}

FakeTimeDev::~FakeTimeDev() = default;

int FakeTimeDev::startDevice() {
  CDevice::startDevice();
  MGR.changeExecutionState(EMGMCommandType::Start);
  return 0;
}

void FakeTimeDev::awaitShutdown() {
  MGR.joinResourceThread();
}

EMGMResponse FakeTimeDev::changeExecutionState(EMGMCommandType paCommand) {
  EMGMResponse eRetVal = CDevice::changeExecutionState(paCommand);
  if ((EMGMResponse::Ready == eRetVal) && (EMGMCommandType::Kill == paCommand)) {
    MGR.changeExecutionState(EMGMCommandType::Kill);
  }
  return eRetVal;
}

CIEC_ANY *FakeTimeDev::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &conn_MGR_ID.getValue();
    case 1: return &conn_FakeTime.getValue();
  }
  return nullptr;
}

CConnection *FakeTimeDev::getResIf2InConnectionUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_MGR_ID;
    case 1: return &conn_FakeTime;
  }
  return nullptr;
}

EMGMResponse
FakeTimeDev::writeValue(forte::core::TNameIdentifier &paNameList, const std::string &paValue, bool paForce) {
  // parent writeValue is modifying the name list so we need to get the name as backup here
  forte::core::StringId portName = paNameList.back();
  EMGMResponse eRetVal = CDevice::writeValue(paNameList, paValue, paForce);
  if ((EMGMResponse::Ready == eRetVal) && ("FakeTime"_STRID == portName)) {
    // fake time was written, update CFakeTimerHandler
    static_cast<CFakeTimerHandler &>(getTimer()).sleepToTime(conn_FakeTime.getValue());
  }
  return eRetVal;
}

forte::core::StringId FakeTimeDev::initializeTimer() {
  TimerHandlerFactory::setTimeHandlerNameToCreate(TimerHandlerFactory::AvailableTimers::CFakeTimerHandler);
  return {};
}
