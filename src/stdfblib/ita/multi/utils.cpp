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
#include "deviceFactory.h"
#include "device.h"

#ifdef FORTE_DEBUG_DEVICE
#include "stdfblib/ita/debug_device/DebugDevice.h"
#endif // FORTE_DEBUG_DEVICE

#ifdef FORTE_OPCUA_DEVICE
#include "stdfblib/ita/OPCUA_DEV.h"
#endif // FORTE_OPCUA_DEVICE

namespace forte::ita::multi::utils {

OPCUA_MGR* getOpcuaMgr(CDevice& paDevice){
  auto currentDevice = DeviceFactory::getCurrentDeviceToCreate();
#ifdef FORTE_DEBUG_DEVICE
  if(currentDevice == "DebugDevice"){
    return &static_cast<DebugDevice*>(&paDevice)->mOpcuaMgr;
  }
#endif // FORTE_DEBUG_DEVICE
#ifdef FORTE_OPCUA_DEVICE
  if (currentDevice == "OPCUA_DEV"){
    return &static_cast<OPCUA_DEV*>(&paDevice)->mOPCUAMgr;
  } 
#endif // FORTE_OPCUA_DEVICE
  return nullptr;
}

void setFactoriesSettings(FactoriesSettings paFactoriesSettings){
  EcetFactory::setEcetToCreate(paFactoriesSettings.mEcet);
  TimerHandlerFactory::setTimeHandlerNameToCreate(paFactoriesSettings.mTimer);
}

}