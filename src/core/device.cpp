/*******************************************************************************
 * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Thomas Strasser, Gerhard Ebenhofer, Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/

#include "device.h"
#include "if2indco.h"
#include "ecet.h"
#include <string.h>

#include "forte_device_config.h"

std::unique_ptr<CDevice> CDevice::smActiveDevice;

CDevice* CDevice::createDev(const std::string &paMGRID) {
  return FORTE_DEVICE::createDev(paMGRID);
}

bool CDevice::startupNewDevice(const std::string &paMGRID){
  if(smActiveDevice){
    //we have a current active device stop it
    triggerDeviceShutdown();
    awaitDeviceShutdown();
  }
  smActiveDevice.reset(createDev(paMGRID));
  if(smActiveDevice){
    smActiveDevice->startDevice();
  }
  return smActiveDevice.operator bool();
}

void CDevice::triggerDeviceShutdown() {
  if(smActiveDevice) {
    smActiveDevice->changeFBExecutionState(EMGMCommandType::Kill);
  }
}

void CDevice::awaitDeviceShutdown() {
  if(smActiveDevice) {
    smActiveDevice->awaitShutdown();
  }
}

EMGMResponse CDevice::executeMGMCommand(forte::core::SManagementCMD &paCommand){
  EMGMResponse retval = EMGMResponse::InvalidDst;

  if(CStringDictionary::scmInvalidStringId == paCommand.mDestination){
    retval = CResource::executeMGMCommand(paCommand);
  }
  else{
    CResource *res = static_cast<CResource *>(CFBContainer::getFB(paCommand.mDestination));
    if(nullptr != res){
      paCommand.mDestination = CStringDictionary::scmInvalidStringId;
      retval = res->executeMGMCommand(paCommand);
    }
  }
  return retval;
}

EMGMResponse CDevice::changeFBExecutionState(EMGMCommandType paCommand){
  if(EMGMCommandType::Kill == paCommand){
    mDeviceExecution.disableHandlers();
  }
  return CResource::changeFBExecutionState(paCommand);
}
