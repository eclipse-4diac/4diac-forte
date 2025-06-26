/*******************************************************************************
 * Copyright (c) 2025 Jose Cabral
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial implementation
 *******************************************************************************/

#include "ReplayDevice.h"

#include "generated/timerhandlerfactory.h"
#include "generated/ecetfactory.h"
#include "core/trace/internal/flexibleTracer.h"

ReplayDevice::ReplayDevice(const std::string &paMGRID) :
    RMT_DEV(setInitialState(paMGRID)),
    mOpcuaMgr(*this),
    mReplayMgr(*this, mOpcuaMgr) {
}

int ReplayDevice::startDevice() {
  RMT_DEV::startDevice();
  if (!mReplayMgr.initialize()) {
    return -1;
  }

  if (mOpcuaMgr.initialize() != EMGMResponse::Ready) {
    return -2;
  }
  return 0;
}

void ReplayDevice::startControlling() {
  mAlreadyControlled = true;
}

EMGMResponse ReplayDevice::executeMGMCommand(forte::core::SManagementCMD &paCommand) {
  // the kill command is the only one that we let through before
  // the replay algorithm starts controlling the device
  // this is meant to not receive the Start command from the IDE which should be
  // handle only after the deviceReplayer was created
  if (paCommand.mCMD == EMGMCommandType::Start && !mAlreadyControlled) {
    return EMGMResponse::Ready;
  }
  return CDevice::executeMGMCommand(paCommand);
}

const std::string &ReplayDevice::setInitialState(const std::string &paMGRID) {
  TimerHandlerFactory::setTimeHandlerNameToCreate(TimerHandlerFactory::AvailableTimers::fakeTimer);
  EcetFactory::setEcetToCreate(EcetFactory::AvailableEcets::fake);
  CFlexibleTracer::setTracer(CFlexibleTracer::AvailableTracers::Internal);
  return paMGRID;
}
