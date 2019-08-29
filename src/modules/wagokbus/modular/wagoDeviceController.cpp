/*************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial API and implementation and/or initial documentation
 *************************************************************************/

#include "wagoDeviceController.h"
#include "wagoHandle.h"
#include "../../../core/utils/criticalregion.h"

const char * const WagoDeviceController::scmKBusDeviceName = "libpackbus";

const char * const WagoDeviceController::scmFailedToGetApplicationInterface = "Failed to get the application Interface";
const char * const WagoDeviceController::scmFailedToInitializeKBus =
  "Failed to initialize the KBus device. Probably there's another program that is using the bus";
const char * const WagoDeviceController::scmFailedToScanDevices = "Failed to scan devices on the KBus";
const char * const WagoDeviceController::scmFailedToOpenKBusDevice = "Failed to open KBus device.";

const char * const WagoDeviceController::scmFailedToCreateKBusInfo = "Call to ldkc_KbusInfo_Create() failed";
const char * const WagoDeviceController::scmFailedGetTerminalInfo = "Call to ldkc_KbusInfo_GetTerminalInfo() failed";
const char * const WagoDeviceController::scmFailedGetTerminalList = "Call to ldkc_KbusInfo_GetTerminalList() failed";

const char * const WagoDeviceController::scmFailedToGetDeviceList = "Failed to get device list";

WagoDeviceController::WagoDeviceController(CDeviceExecution& paDeviceExecution) :
    forte::core::io::IODeviceMultiController(paDeviceExecution), mAppDevInterface(0), mTaskId(0), mKBusDeviceId(scmInvalidDeviceId), mTerminalCount(0) {

  memset(mTerminalIds, 0, sizeof(mTerminalIds));
  memset(mTerminalInfos, 0, sizeof(mTerminalInfos));
  mConfig.updateInterval = 25;
}

WagoDeviceController::~WagoDeviceController() {
  //do nothing
}

void WagoDeviceController::setConfig(struct forte::core::io::IODeviceController::Config* paConfig) {
  this->mConfig = *static_cast<WagoConfig*>(paConfig);
}

const char* WagoDeviceController::init() {
  tDeviceInfo deviceList[scmNumberOfDevicesToScan]; // the list of devices given by the ADI

  mAppDevInterface = adi_GetApplicationInterface();
  if(!mAppDevInterface) {
    DEVLOG_ERROR("[WagoDeviceController] %s\n", scmFailedToGetApplicationInterface);
    return scmFailedToGetApplicationInterface;
  }

  if(DAL_SUCCESS != mAppDevInterface->Init()) {
    DEVLOG_ERROR("[WagoDeviceController] %s\n", scmFailedToInitializeKBus);
    return scmFailedToInitializeKBus;
  }

  if(DAL_SUCCESS != mAppDevInterface->ScanDevices()) {
    DEVLOG_ERROR("[WagoDeviceController] %s\n", scmFailedToScanDevices);
    return scmFailedToScanDevices;
  }

  mAppDevInterface->GetDeviceList(sizeof(deviceList), deviceList, &mTerminalCount); //always return DAL_SUCCESS

  for(size_t i = 0; i < mTerminalCount; ++i) { // find kbus device
    if(0 == strcmp(deviceList[i].DeviceName, scmKBusDeviceName)) {
      mKBusDeviceId = deviceList[i].DeviceId;
      if(0 != mAppDevInterface->OpenDevice(mKBusDeviceId)) {
        DEVLOG_ERROR("[WagoDeviceController] %s \n", scmFailedToOpenKBusDevice);
        return scmFailedToOpenKBusDevice;
      }

      return loadTerminalInformation();
    }
  }

  for(size_t i = 0; i < mTerminalCount; i++) {
    DEVLOG_INFO("[WagoDeviceController] Found device with ID: %d\n", mTerminalIds[i]);
  }

  return 0;
}

forte::core::io::IOHandle* WagoDeviceController::initHandle(forte::core::io::IODeviceController::HandleDescriptor *paHandleDescriptor) {
  WagoHandleDescriptor desc = *static_cast<WagoHandleDescriptor*>(paHandleDescriptor);

  TForteUInt32 outputOffset;
  TForteUInt32 inputOffset;
  switch(desc.mType){
    case CIEC_ANY::e_BOOL:
      outputOffset = mTerminalInfos[desc.mSlaveIndex].OffsetOutput_bits + desc.mChannel;
      inputOffset = mTerminalInfos[desc.mSlaveIndex].OffsetInput_bits + desc.mChannel;
      break;
    case CIEC_ANY::e_WORD:
      outputOffset = mTerminalInfos[desc.mSlaveIndex].OffsetOutput_bits + (2 * desc.mChannel);
      inputOffset = mTerminalInfos[desc.mSlaveIndex].OffsetInput_bits + (2 * desc.mChannel);
      break;
    default:
      return 0;
  }

  return new WagoHandle(this, desc.mType, desc.mDirection, mAppDevInterface, mTaskId, mKBusDeviceId, outputOffset, inputOffset);
}

void WagoDeviceController::deInit() {
  if(0 != mAppDevInterface) {
    if(0 != mAppDevInterface->CloseDevice(mKBusDeviceId)) {
      DEVLOG_ERROR("[WagoDeviceController] There was a problem closing the KBus device\n");
    }
    mAppDevInterface->Exit(); // disconnect ADI-Interface
    mKBusDeviceId = scmInvalidDeviceId;
    mAppDevInterface = 0;
  }
}

void WagoDeviceController::runLoop() {
  tApplicationStateChangedEvent stEvent;

  stEvent.State = ApplicationState_Running; // Set application state to "Running" to drive kbus by ourselves.
  if(DAL_SUCCESS == mAppDevInterface->ApplicationStateChanged(stEvent)) {
    while(isAlive()) {
      CThread::sleepThread(mConfig.updateInterval);
      if(!triggerKBusCycle()) {
        break; //we have severe problem exit KBus handling thread
      }

      checkForInputChanges();
    }
  } else {
    DEVLOG_ERROR("[WagoDeviceController] Set application state to 'Running' failed\n");
  }
}

void WagoDeviceController::addSlaveHandle(int, forte::core::io::IOHandle* paHandle) {
  CCriticalRegion criticalRegion(mHandleMutex);
  paHandle->is(forte::core::io::IOMapper::In) ? mInputHandles.pushBack(paHandle) : mOutputHandles.pushBack(paHandle);
}

void WagoDeviceController::dropSlaveHandles(int) {
  // Is handled by #dropHandles method
}

bool WagoDeviceController::isSlaveAvailable(int paIndex) {
  return paIndex < mTerminalCount;
}

bool WagoDeviceController::checkSlaveType(int paIndex, int paType) {
  return mTerminalIds[paIndex] == paType;
}

bool WagoDeviceController::isHandleValueEqual(forte::core::io::IOHandle* paHandle) {
  return !static_cast<WagoHandle*>(paHandle)->check();
}

const char* WagoDeviceController::loadTerminalInformation() {
  if(KbusInfo_Failed == ldkc_KbusInfo_Create()) {
    DEVLOG_ERROR("[WagoDeviceController] %s\n", scmFailedToCreateKBusInfo);
    return scmFailedToCreateKBusInfo;
  }

  if(KbusInfo_Failed == ldkc_KbusInfo_GetTerminalInfo(OS_ARRAY_SIZE(mTerminalInfos), mTerminalInfos, &mTerminalCount)) {
    DEVLOG_ERROR("[WagoDeviceController] %s\n", scmFailedGetTerminalInfo);
    ldkc_KbusInfo_Destroy();
    return scmFailedGetTerminalInfo;
  }

  if(KbusInfo_Failed == ldkc_KbusInfo_GetTerminalList(OS_ARRAY_SIZE(mTerminalIds), mTerminalIds, NULL)) {
    DEVLOG_ERROR("[WagoDeviceController] %s\n", scmFailedGetTerminalList);
    ldkc_KbusInfo_Destroy();
    return scmFailedGetTerminalList;
  }

  return 0;
}

bool WagoDeviceController::triggerKBusCycle() {
  uint32_t pushRetVal = 0;

  if(DAL_SUCCESS == mAppDevInterface->CallDeviceSpecificFunction("libpackbus_Push", &pushRetVal)) {
    if(DAL_SUCCESS == pushRetVal) {
      mAppDevInterface->WatchdogTrigger();
      return true;
    } else {
      DEVLOG_ERROR("[WagoDeviceController] Function 'libpackbus_Push' failed\n");
    }
  } else {
    DEVLOG_ERROR("[WagoDeviceController] CallDeviceSpecificFunction for 'libpackbus_Push' failed\n");
  }
  return false;
}
