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

#include "MultiDevice.h"

#include "stdfblib/ita/OPCUA_MGR.h"
#include "stdfblib/ita/multi/utils.h"
#include "deviceFactory.h"

#include "devlog.h"

const SFBInterfaceSpec MultiDevice::scmFBInterfaceSpec = {
  0, nullptr, nullptr, nullptr, nullptr,
  0, nullptr, nullptr, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};

MultiDevice::MultiDevice(const std::string &paMGRID) : 
  CDevice(scmFBInterfaceSpec, CStringDictionary::scmInvalidStringId), 
  mMGRID(paMGRID) {
  
  // avoid creating another MultiDevice in case it was set to it in cmake
  if(DeviceFactory::getCurrentDeviceToCreate() == scmMultiDeviceName){
    DeviceFactory::setDeviceToCreate(scmDefaultDeviceToCreate);
  }

  DEVLOG_INFO("Starting a %s device\n", scmMultiDeviceName.c_str());
}

void MultiDevice::awaitShutdown(){
  // wait for the kill signal from main to arrive
  mKillSignal.get_future().wait();
}

EMGMResponse MultiDevice::changeExecutionState(EMGMCommandType paCommand) {
  // handle the actual kill signal coming from main, ignore the rest
  if(EMGMCommandType::Kill == paCommand){
    killControlledDevice();
    mKillSignal.set_value();
  }
  return EMGMResponse::Ready;
}

int MultiDevice::startDevice() {
  // we don't want to do anything with this device, but rather with mControlledDevice
  CDevice::changeExecutionState(EMGMCommandType::Kill);

  return resetControlledDevice();
}

void MultiDevice::killControlledDevice(){
  if(mControlledDevice != nullptr){
    mControlledDevice->changeExecutionState(EMGMCommandType::Kill);
    mControlledDevice->awaitShutdown();
  }
}

void MultiDevice::requestResetControlledDevice(){
  mRestartSignalHandler = std::async(std::launch::async, [this](){
   resetControlledDevice();
  });
}

int MultiDevice::resetControlledDevice(){
  killControlledDevice();

  // reset all factories to the standard options
  // since each device should set them accordingly
  forte::ita::multi::utils::setFactoriesSettings(forte::ita::multi::utils::FactoriesSettings());

  DEVLOG_INFO("The controlled device is a %s\n", DeviceFactory::getCurrentDeviceToCreate().c_str());

  mControlledDevice = DeviceFactory::createDevice(mMGRID);

  // destroy before creating a new one
  mOpcuaMgr = nullptr; 
  mMultiMgr = nullptr;

  // if the controlled device has already an opcuaMgr we used that, otherwise
  // we create and own one
  if(auto opcuaMgr = forte::ita::multi::utils::getOpcuaMgr(*mControlledDevice); opcuaMgr != nullptr){
    mOpcuaMgr = opcuaMgr;
  } else {
    mOpcuaMgr = std::make_unique<OPCUA_MGR>(*mControlledDevice);
  }

  // initialize the opcua methods
  auto result = std::visit([this](auto&& paOpcuaMgr) -> int {
    // initialize the multi mgr commands and pass the opcua mgr object we have stored (own or borrowed from the device)
    mMultiMgr = std::make_unique<MultiMGR>(*this, *paOpcuaMgr);
    if(!mMultiMgr->initialize()){
      return -1;
    }

    using T = std::decay_t<decltype(paOpcuaMgr)>;
    if constexpr (std::is_same_v<T, std::unique_ptr<OPCUA_MGR>>) {
      // opcuaMgr belong to us, we need to initialize it
      if (paOpcuaMgr->initialize() != EMGMResponse::Ready) {
        return -1;
      }
    }
    return 0;
  }, mOpcuaMgr);

  if(result != 0){
    return -1;
  }
  mControlledDevice->initialize();
  return mControlledDevice->startDevice();
}

CIEC_ANY* MultiDevice::getDI(size_t) {
  return nullptr;
}

CDataConnection** MultiDevice::getDIConUnchecked(TPortId) {
  return nullptr;
}