/*************************************************************************
 * Copyright (c) 2018, 2025 fortiss GmbH, Monika Wenger
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial API and implementation and/or initial documentation
 *************************************************************************/

#include "WagoDeviceController.h"
#include "forte/util/criticalregion.h"
#include "WagoHandle.h"
#include "types/WagoRegComDevice.h"
#include "forte/iec61131_functions/func_USINT_TO_BYTE.h"

using namespace forte::io;

namespace forte::eclipse4diac::io::wago {

  const char *const WagoDeviceController::scmKBusDeviceName = "libpackbus";

  const char *const WagoDeviceController::scmFailedToGetApplicationInterface = "Failed to get the application Interface";
  const char *const WagoDeviceController::scmFailedToInitializeKBus =
      "Failed to initialize the KBus device. Probably there's another program that is using the bus";
  const char *const WagoDeviceController::scmFailedToScanDevices = "Failed to scan devices on the KBus";
  const char *const WagoDeviceController::scmFailedToOpenKBusDevice = "Failed to open KBus device.";

  const char *const WagoDeviceController::scmFailedToCreateKBusInfo = "Call to ldkc_KbusInfo_Create() failed";
  const char *const WagoDeviceController::scmFailedGetTerminalInfo = "Call to ldkc_KbusInfo_GetTerminalInfo() failed";
  const char *const WagoDeviceController::scmFailedGetTerminalList = "Call to ldkc_KbusInfo_GetTerminalList() failed";

  const char *const WagoDeviceController::scmFailedToGetDeviceList = "Failed to get device list";

  WagoDeviceController::WagoDeviceController(CDeviceExecution &paDeviceExecution) :
      IODeviceMultiController(paDeviceExecution),
      mAppDevInterface(0),
      mTaskId(0),
      mKBusDeviceId(scmInvalidDeviceId),
      mTerminalCount(0) {

    memset(mTerminalIds, 0, sizeof(mTerminalIds));
    memset(mTerminalInfos, 0, sizeof(mTerminalInfos));
    mConfig.updateInterval = 25;
  }

  WagoDeviceController::~WagoDeviceController() {
    // do nothing
  }

  void WagoDeviceController::setConfig(struct Config *paConfig) {
    this->mConfig = *static_cast<WagoConfig *>(paConfig);
  }

  const char *WagoDeviceController::init() {
    tDeviceInfo deviceList[scmNumberOfDevicesToScan]; // the list of devices given by the ADI

    mAppDevInterface = adi_GetApplicationInterface();
    if (!mAppDevInterface) {
      DEVLOG_ERROR("[WagoDeviceController] %s\n", scmFailedToGetApplicationInterface);
      return scmFailedToGetApplicationInterface;
    }

    if (DAL_SUCCESS != mAppDevInterface->Init()) {
      DEVLOG_ERROR("[WagoDeviceController] %s\n", scmFailedToInitializeKBus);
      return scmFailedToInitializeKBus;
    }

    if (DAL_SUCCESS != mAppDevInterface->ScanDevices()) {
      DEVLOG_ERROR("[WagoDeviceController] %s\n", scmFailedToScanDevices);
      return scmFailedToScanDevices;
    }

    mAppDevInterface->GetDeviceList(sizeof(deviceList), deviceList, &mTerminalCount); // always return DAL_SUCCESS

    for (size_t i = 0; i < mTerminalCount; ++i) { // find kbus device
      if (0 == strcmp(deviceList[i].DeviceName, scmKBusDeviceName)) {
        mKBusDeviceId = deviceList[i].DeviceId;
        if (DAL_SUCCESS != mAppDevInterface->OpenDevice(mKBusDeviceId)) {
          DEVLOG_ERROR("[WagoDeviceController] %s \n", scmFailedToOpenKBusDevice);
          return scmFailedToOpenKBusDevice;
        } else {
          loadTerminalInformation();
          break;
        }
      }
    }

    for (size_t i = 0; i < mTerminalCount; i++) {
      DEVLOG_INFO("[WagoDeviceController] Found device with ID: %d\n", mTerminalIds[i]);
    }

    return 0;
  }

  IOHandle *WagoDeviceController::createIOHandle(IODeviceController::HandleDescriptor &paHandleDescriptor) {
    WagoHandleDescriptor &desc(static_cast<WagoHandleDescriptor &>(paHandleDescriptor));
    TForteUInt32 outputOffset;
    TForteUInt32 inputOffset;
    switch (desc.mType) {
      case CIEC_ANY::e_BOOL:
        outputOffset = mTerminalInfos[desc.mSlaveIndex].OffsetOutput_bits + desc.mChannel;
        inputOffset = mTerminalInfos[desc.mSlaveIndex].OffsetInput_bits + desc.mChannel;
        break;
      case CIEC_ANY::e_WORD:
        outputOffset = mTerminalInfos[desc.mSlaveIndex].OffsetOutput_bits + (2 * desc.mChannel);
        inputOffset = mTerminalInfos[desc.mSlaveIndex].OffsetInput_bits + (2 * desc.mChannel);
        break;
      case CIEC_ANY::e_DWORD:
        outputOffset = (mTerminalInfos[desc.mSlaveIndex].OffsetOutput_bits + desc.mChannel) / 8;
        inputOffset = (mTerminalInfos[desc.mSlaveIndex].OffsetInput_bits + desc.mChannel) / 8;
        break;
      default: return 0;
    }
    return new WagoHandle(this, desc.mType, desc.mDirection, mAppDevInterface, mTaskId, mKBusDeviceId, outputOffset,
                          inputOffset);
  }

  void WagoDeviceController::deInit() {
    if (0 != mAppDevInterface) {
      if (0 != mAppDevInterface->CloseDevice(mKBusDeviceId)) {
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
    if (DAL_SUCCESS == mAppDevInterface->ApplicationStateChanged(stEvent)) {
      while (isAlive()) {
        sleepThread(mConfig.updateInterval);
        if (!triggerKBusCycle()) {
          break; // we have severe problem exit KBus handling thread
        }
        if(mRegComDevice){
          checkForRegComChanges();
        }
        checkForInputChanges();
      }
    } else {
      DEVLOG_ERROR("[WagoDeviceController] Set application state to 'Running' failed\n");
    }
  }

  void WagoDeviceController::addSlaveHandle(size_t, std::unique_ptr<IOHandle> paHandle) {
    forte::util::CCriticalRegion criticalRegion(mHandleMutex);
    paHandle->isInput() ? mInputHandles.push_back(std::move(paHandle)) : mOutputHandles.push_back(std::move(paHandle));
  }

  void WagoDeviceController::dropSlaveHandles(size_t) {
    // Is handled by #dropHandles method
  }

  bool WagoDeviceController::isSlaveAvailable(size_t paIndex) {
    return paIndex < mTerminalCount;
  }

  bool WagoDeviceController::checkSlaveType(size_t paIndex, int paType) {
    return mTerminalIds[paIndex] == paType;
  }

  bool WagoDeviceController::isHandleValueEqual(IOHandle &paHandle) {
    return !static_cast<WagoHandle &>(paHandle).check();
  }

  const char *WagoDeviceController::loadTerminalInformation() {
    if (KbusInfo_Failed == ldkc_KbusInfo_Create()) {
      DEVLOG_ERROR("[WagoDeviceController] %s\n", scmFailedToCreateKBusInfo);
      return scmFailedToCreateKBusInfo;
    }

    if (KbusInfo_Failed ==
        ldkc_KbusInfo_GetTerminalInfo(OS_ARRAY_SIZE(mTerminalInfos), mTerminalInfos, &mTerminalCount)) {
      DEVLOG_ERROR("[WagoDeviceController] %s\n", scmFailedGetTerminalInfo);
      ldkc_KbusInfo_Destroy();
      return scmFailedGetTerminalInfo;
    }

    if (KbusInfo_Failed == ldkc_KbusInfo_GetTerminalList(OS_ARRAY_SIZE(mTerminalIds), mTerminalIds, nullptr)) {
      DEVLOG_ERROR("[WagoDeviceController] %s\n", scmFailedGetTerminalList);
      ldkc_KbusInfo_Destroy();
      return scmFailedGetTerminalList;
    }

    return 0;
  }

  bool WagoDeviceController::triggerKBusCycle() {
    uint32_t pushRetVal = 0;

    if (DAL_SUCCESS == mAppDevInterface->CallDeviceSpecificFunction("libpackbus_Push", &pushRetVal)) {
      if (DAL_SUCCESS == pushRetVal) {
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

  void WagoDeviceController::checkForRegComChanges() {
    mAppDevInterface->ReadStart(mKBusDeviceId, mTaskId);
    if(DAL_SUCCESS == mAppDevInterface->ReadBytes(mKBusDeviceId, mTaskId, mRegComDevice->getOffset_REG_S0() / 8, 1, &mREG_S)) {
      if (!isRegComOn && ( (mREG_S & 0x80) == 0x80)) {
        isRegComOn = true;
        DEVLOG_DEBUG("[WagoDeviceController] Register communication on.\n");
        startNewEventChain(mRegComDevice);
      } else if (isRegComOn && ((mREG_S & 0xBF) == (mREG_C & 0xBF)) ) {
        mREG_C = 0x00; // one change approve
        DEVLOG_DEBUG("[WagoDeviceController] Register communication change.\n");
        startNewEventChain(mRegComDevice);
      } else if (isRegComOn && ((mREG_S & 0x80) == 0x00)) {
        isRegComOn = false;
        DEVLOG_DEBUG("[WagoDeviceController] Register communication off.\n");
        startNewEventChain(mRegComDevice);
        mRegComDevice = nullptr;
    }
  }
  mAppDevInterface->ReadEnd(mKBusDeviceId, mTaskId);
}

  void WagoDeviceController::enableRegCom(WagoRegComDevice *paECStartFB) {
    mRegComDevice = paECStartFB;
    mAppDevInterface->WriteStart(mKBusDeviceId, mTaskId);
    if(DAL_SUCCESS == mAppDevInterface->WriteBool(mKBusDeviceId, mTaskId, mRegComDevice->getOffset_REG_C7(), true) ) {
      DEVLOG_DEBUG("[WagoDeviceController] Register communication enabling.\n");
    }
    mAppDevInterface->WriteEnd(mKBusDeviceId, mTaskId);
  }

  void WagoDeviceController::disableRegCom() {
    mAppDevInterface->WriteStart(mKBusDeviceId, mTaskId);
    if(DAL_SUCCESS == mAppDevInterface->WriteBool(mKBusDeviceId, mTaskId, mRegComDevice->getOffset_REG_C7(), false) ) {
      DEVLOG_DEBUG("[WagoDeviceController] Register communication disabling.\n");
    }
    mAppDevInterface->WriteEnd(mKBusDeviceId, mTaskId);
  }

  void WagoDeviceController::writeRegComRequest(const CIEC_RegComCmd &paCmd) {
    if (isRegComOn){
      mAppDevInterface->WriteStart(mKBusDeviceId, mTaskId);
      TForteByte outData[3];
      outData[0] = mREG_C = 0xC0 | (func_USINT_TO_BYTE(paCmd.var_RegNr) & 0x3F); // Bit7 = 1, Bit6 = write = 1, Bit0-5= regNr
      outData[1] = paCmd.var_REG_D0; // data
      outData[2] = paCmd.var_REG_D1; // data
      if(DAL_SUCCESS == mAppDevInterface->WriteBytes(mKBusDeviceId, mTaskId, mRegComDevice->getOffset_REG_C0() / 8, 3, outData) ) {
        DEVLOG_DEBUG("[WagoDeviceController] Register communication write.\n");
      }
      mAppDevInterface->WriteEnd(mKBusDeviceId, mTaskId);
    } else {
      DEVLOG_ERROR("[WagoDeviceController] Register communication is off, write request not executable.\n");
    }
  }

  void WagoDeviceController::readRegComRequest(const CIEC_RegComCmd &paCmd) {
      if (isRegComOn){
        mAppDevInterface->WriteStart(mKBusDeviceId, mTaskId);
        mREG_C = 0x80 | func_USINT_TO_BYTE(paCmd.var_RegNr); // Bit7 = 1, Bit6 = read = 0, Bit0-5= regNr
        if(DAL_SUCCESS == mAppDevInterface->WriteBytes(mKBusDeviceId, mTaskId, mRegComDevice->getOffset_REG_C0() / 8, 1, &mREG_C) ) {
          DEVLOG_DEBUG("[WagoDeviceController] Register communication read request.\n");
        }
        mAppDevInterface->WriteEnd(mKBusDeviceId, mTaskId);
      } else{
        DEVLOG_ERROR("[WagoDeviceController] Register communication is off, read request not executable.\n");
      }
    }

  void WagoDeviceController::readRegComResult(CIEC_BYTE &paD0, CIEC_BYTE &paD1) {
    if (isRegComOn){
      TForteByte inDataWord[2];
      mAppDevInterface->ReadStart(mKBusDeviceId, mTaskId);
      if(DAL_SUCCESS == mAppDevInterface->ReadBytes(mKBusDeviceId, mTaskId, mRegComDevice->getOffset_rREG_D0() / 8, 2, inDataWord) ) {
        paD0 = CIEC_BYTE(inDataWord[0]);
        paD1 = CIEC_BYTE(inDataWord[1]);
        DEVLOG_DEBUG("[WagoDeviceController] Register communication read.\n");
      }
      mAppDevInterface->ReadEnd(mKBusDeviceId, mTaskId);
      startNewEventChain(mRegComDevice);
    } else{
      DEVLOG_ERROR("[WagoDeviceController] Register communication is off, register result cannot be read.\n");
    }
  }

  void WagoDeviceController::initRegComOffsets(WagoRegComDevice *paECStartFB){
    size_t slaveIndex = paECStartFB->getSlaveIndex();
    TForteUInt32 startOffsetIn = mTerminalInfos[slaveIndex].OffsetInput_bits;
    paECStartFB->setOffset_REG_S0( startOffsetIn ); // register communication mirrored request
    paECStartFB->setOffset_REG_S7( startOffsetIn + 7 ); // register communication enable response
    paECStartFB->setOffset_rREG_D0( startOffsetIn + 8 ); // register communication response data
    TForteUInt32 startOffsetOut = mTerminalInfos[slaveIndex].OffsetOutput_bits;
    paECStartFB->setOffset_REG_C0( startOffsetOut ); // register communication request data
    paECStartFB->setOffset_REG_C7( startOffsetOut + 7 ); // register communication enable
    paECStartFB->setOffset_wREG_D0( startOffsetOut + 8 ); // register communication set data
  }

} // namespace forte::eclipse4diac::io::wago
