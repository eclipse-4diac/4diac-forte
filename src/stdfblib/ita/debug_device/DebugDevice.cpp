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

DebugDevice::DebugDevice(const std::string &paMGRID) : 
  RMT_DEV(paMGRID), mOpcuaMgr(*this), mDebugMgr(*this, mOpcuaMgr) {
}

int DebugDevice::startDevice() {
  RMT_DEV::startDevice();
  if (!mDebugMgr.initialize()) {
    return -1;
  }

  if(mOpcuaMgr.initialize() != EMGMResponse::Ready){
    return -2;
  }
  return 0;
}
