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

#include "DebugDevice.h"

const SFBInterfaceSpec DebugDevice::scmFBInterfaceSpec = {
  0, nullptr, nullptr, nullptr, nullptr,
  0, nullptr, nullptr, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};

DebugDevice::DebugDevice(const std::string&) : 
  CDevice(scmFBInterfaceSpec, CStringDictionary::scmInvalidStringId), 
  mOpcuaMgr(*this), mDebugMgr(*this, mOpcuaMgr) {
}

int DebugDevice::startDevice() {
  CDevice::startDevice();
  if (!mDebugMgr.initialize()) {
    return -1;
  }

  if(mOpcuaMgr.initialize() != EMGMResponse::Ready){
    return -2;
  }
  return 0;
}

EMGMResponse DebugDevice::changeExecutionState(EMGMCommandType paCommand){
  EMGMResponse eRetVal = CDevice::changeExecutionState(paCommand);
  if(EMGMCommandType::Kill == paCommand){
    mKillSignal.set_value();
  }
  return eRetVal;
}

void DebugDevice::awaitShutdown() {
  // wait for the kill signal to arrive
  mKillSignal.get_future().wait();
}

CIEC_ANY* DebugDevice::getDI(size_t) {
  return nullptr;
}

CDataConnection** DebugDevice::getDIConUnchecked(TPortId) {
  return nullptr;
}


