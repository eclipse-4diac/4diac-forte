/*******************************************************************************
 * Copyright (c) 2022 Peirlberger Juergen
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Peirlberger Juergen - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "deviceController.h"
#include "slaveHandle/bitSlaveHandle.h"
#include "slaveHandle/slaveHandle.h"

void PLCnextDeviceController::setConfig(struct Config *paConfig) {
  if (isAlive()) {
    DEVLOG_ERROR("PLCnext[PLCnextDeviceController]: Cannot change configuration while running.\n");
    return;
  }

  this->mConfig = *static_cast<PLCnextConfig *>(paConfig);
}

const char *PLCnextDeviceController::init() {
  return nullptr;
}

void PLCnextDeviceController::deInit() {
  for (PLCnextSlaveHandler *it : mDevices) {
    delete it;
  }
  mDevices.clear();
}

void PLCnextDeviceController::registerSlaveHandler(PLCnextSlaveHandler *slave) {
  mDevices.push_back(slave);
}

forte::io::IOHandle *PLCnextDeviceController::createIOHandle(IODeviceController::HandleDescriptor &paHandleDescriptor) {
  HandleDescriptor &desc = static_cast<HandleDescriptor &>(paHandleDescriptor);
  PLCnextSlaveHandler *device = getSlave(desc.mSlaveIndex);

  if (device == nullptr) {
    DEVLOG_ERROR("slave with index %d not found.\n", desc.mSlaveIndex);
    return nullptr;
  }

  switch (desc.mType) {
    case Bit: return new PLCnextBitSlaveHandle(this, desc.mDirection, desc.mPosition, device);
    case BitString16: return nullptr; // TODO: implement AnalogSlaveHandle!
  }

  return nullptr;
}

void PLCnextDeviceController::runLoop() {
  while (isAlive()) {
    for (PLCnextSlaveHandler *it : mDevices) {
      it->update();
    }

    sleepThread(mConfig.updateInterval);
  }
}

PLCnextSlaveHandler *PLCnextDeviceController::getSlave(size_t paIndex) {
  if (mDevices.size() <= paIndex) {
    return nullptr;
  }

  return mDevices[paIndex];
}

void PLCnextDeviceController::addSlaveHandle(size_t paIndex, std::unique_ptr<forte::io::IOHandle> paHandle) {
  PLCnextSlaveHandler *device = getSlave(paIndex);
  if (device == nullptr) {
    DEVLOG_ERROR("no devices registered.");
    return;
  }

  device->addHandle((PLCnextSlaveHandle *) paHandle.release());
}

void PLCnextDeviceController::dropSlaveHandles(size_t paIndex) {
  PLCnextSlaveHandler *device = getSlave(paIndex);
  if (device == nullptr) {
    DEVLOG_ERROR("no devices registered.");
    return;
  }

  device->dropHandles();
}

bool PLCnextDeviceController::isSlaveAvailable(size_t) {
  // (currently) don't check if index is really available at initialization;
  return true;
}

bool PLCnextDeviceController::checkSlaveType(size_t, int) {
  // (currently) don't check slaveType at initialization;
  return true;
}
