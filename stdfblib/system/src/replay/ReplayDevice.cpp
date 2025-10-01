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
#include "forte/devicefactory.h"
#include "forte/timerhandlerfactory.h"
#include "forte/ecetfactory.h"
#include "../../../../core/src/trace/internal/flexibleTracer.h"

using namespace forte::literals;

namespace forte::iec61499::system {
  namespace {
    [[maybe_unused]] const DeviceFactory::EntryImpl<ReplayDevice> entry("Replay"_STRID);
  }

  ReplayDevice::ReplayDevice(const std::string_view paMGRID) :
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

  EMGMResponse ReplayDevice::executeMGMCommand(SManagementCMD &paCommand) {
    // the kill command is the only one that we let through before
    // the replay algorithm starts controlling the device
    // this is meant to not receive the Start command from the IDE which should be
    // handle only after the deviceReplayer was created
    if (paCommand.mCMD == EMGMCommandType::Start && !mAlreadyControlled) {
      return EMGMResponse::Ready;
    }
    return CDevice::executeMGMCommand(paCommand);
  }

  const std::string_view ReplayDevice::setInitialState(const std::string_view paMGRID) {
    TimerHandlerFactory::setDefaultImpl("FakeTime"_STRID);
    EcetFactory::setDefaultImpl("Fake"_STRID);
    trace::CFlexibleTracer::setTracer(trace::CFlexibleTracer::AvailableTracers::Internal);
    return paMGRID;
  }
} // namespace forte::iec61499::system
