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

#include "slaveHandler.h"
#include "deviceController.h"
#include "forte/io/mapper/io_mapper.h"

using namespace forte::literals;

const TForteUInt8 PLCnextSlaveHandler::scmSlaveConfigurationIO[] = {};
const TForteUInt8 PLCnextSlaveHandler::scmSlaveConfigurationIONum = 0;

PLCnextSlaveHandler::PLCnextSlaveHandler(int paType,
                                         CFBContainer &paContainer,
                                         const SFBInterfaceSpec *paInterfaceSpec,
                                         const forte::StringId paInstanceNameId) :
    IOConfigFBMultiSlave(
        scmSlaveConfigurationIO, scmSlaveConfigurationIONum, paType, paContainer, paInterfaceSpec, paInstanceNameId),
    slaveType(SlaveType(paType)),
    var_BusAdapterOut("BusAdapterOut"_STRID, *this, 0),
    var_BusAdapterIn("BusAdapterIn"_STRID, *this, 0) {
}

PLCnextSlaveHandler::~PLCnextSlaveHandler() {
  delete imageBuffer;
}

int PLCnextSlaveHandler::update() {
  if (slaveType == Input) {
    plcNextDevice.read(this->imageBuffer, this->imageSize);
  } else if (slaveType == Output) {
    plcNextDevice.write(this->imageBuffer, this->imageSize);
  } else if (slaveType == NoUsage) {
    // PLCnext contains a "placeholder" module
    // for this module, nothing should happen
  } else {
    DEVLOG_ERROR("[PLCnextSlaveHandler] SlaveHander not correct initialized.\n");
  }
}

void PLCnextSlaveHandler::dropHandles() {
  CCriticalRegion criticalRegion(handleMutex);
  for (PLCnextSlaveHandle *it : mDeviceHandles) {
    delete it;
  }
  mDeviceHandles.clear();
}

void PLCnextSlaveHandler::initBufferImage(size_t size) {
  this->imageSize = size;
  this->imageBuffer = new char[imageSize];

  memset(this->imageBuffer, 0, size);
}

void PLCnextSlaveHandler::addHandle(PLCnextSlaveHandle *paHandle) {
  CCriticalRegion criticalRegion(handleMutex);
  mDeviceHandles.push_back(paHandle);
}

PLCnextSlaveHandle *PLCnextSlaveHandler::getHandle(size_t paIndex) {
  if (mDeviceHandles.size() <= paIndex) {
    return nullptr;
  }

  return mDeviceHandles[paIndex];
}

forte::IPlugPin *PLCnextSlaveHandler::getPlugPinUnchecked(size_t paIndex) {
  return (paIndex == 0) ? &var_BusAdapterOut : nullptr;
}

forte::ISocketPin *PLCnextSlaveHandler::getSocketPinUnchecked(size_t paIndex) {
  return (paIndex == 0) ? &var_BusAdapterIn : nullptr;
}
