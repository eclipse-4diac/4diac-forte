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

#include "stdfblib/ita/multi/utils.h"
#include "stdfblib/ita/debug_device/DebugDevice.h"
#include "stdfblib/ita/OPCUA_DEV.h"
#include "deviceFactory.h"
#include "device.h"

namespace forte::ita::multi::utils {

OPCUA_MGR* getOpcuaMgr(CDevice& paDevice){
  if(auto currentDevice = DeviceFactory::getCurrentDeviceToCreate(); 
      currentDevice == "DebugDevice"){
    return &static_cast<DebugDevice*>(&paDevice)->mOpcuaMgr;
  }else if (currentDevice == "OPCUA_DEV"){
    return &static_cast<OPCUA_DEV*>(&paDevice)->mOPCUAMgr;
  } 
  return nullptr;
}


void setFactoriesSettings(FactoriesSettings paFactoriesSettings){
  EcetFactory::setEcetToCreate(paFactoriesSettings.mEcet);
  TimerHandlerFactory::setTimeHandlerNameToCreate(paFactoriesSettings.mTimer);
}

}