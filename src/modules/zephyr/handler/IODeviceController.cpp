/************************************************************************************
 Copyright (c) 2024 KT Elektronik GmbH
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0
 
 Contributors:
  Dirk Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#include "IODeviceController.h"
#include <handle/IOHandleGPIO.h>
#include <handle/IOHandleADC.h>
#include <handle/IOHandlePWM.h>

DEFINE_HANDLER(IODeviceController);

IODeviceController::IODeviceController(CDeviceExecution& paDeviceExecution) 
: forte::core::io::IODeviceController(paDeviceExecution) {
}

IODeviceController::~IODeviceController() {
  DEVLOG_INFO("IODeviceController dtor\n");
}

IODeviceController::IOHandle *
IODeviceController::createIOHandle(HandleDescriptor& paHandleDescriptor) {
  auto desc = static_cast<IOHandleDescriptor&>(paHandleDescriptor);
  IOHandle *handle;
  switch (desc.mType) {
  case IOHandleDescriptor::Gpio: {
    auto gpioDesc = static_cast<IOHandleGPIODescriptor&>(paHandleDescriptor);
    handle = new IOHandleGPIO(this, gpioDesc.mDirection, gpioDesc.getGpioSpec(), gpioDesc.getFlags());
    break;
  }
  case IOHandleDescriptor::Adc: {
    auto adcDesc = static_cast<IOHandleADCDescriptor&>(paHandleDescriptor);
    handle = new IOHandleADC(this, adcDesc.getAdcSpec());
    break;
  }
  case IOHandleDescriptor::Pwm: {
    auto pwmDesc = static_cast<IOHandlePWMDescriptor&>(paHandleDescriptor);
    handle = new IOHandlePWM(this, pwmDesc.mId, pwmDesc.getPwmSpec());
    break;
  }
  default:
    DEVLOG_ERROR("IODeviceController::createIOHandle: unsupported HandleType.\n");
    handle = nullptr;
    break;
  }
  return handle;
}

void IODeviceController::setConfig(struct forte::core::io::IODeviceController::Config* paConfig) {
  DEVLOG_INFO("IODeviceController::setConfig\n");
  if (isAlive()) {
    DEVLOG_ERROR("IODeviceController::setConfig: Cannot change configuration while running.\n");
    return;
  }
  mConfig = *static_cast<Config*>(paConfig);
}

void IODeviceController::handleChangeEvent(IOHandle* paHandle) {
  (void)paHandle;
  mUpdateSema.inc();
}

void IODeviceController::runLoop() {
  DEVLOG_INFO("IODeviceController::runLoop\n");
  while (isAlive()) {
    if (mConfig.updateInterval > 0) {
      const auto updateIntvl_ns = mConfig.updateInterval * 1000000ULL; // update interval ms to ns
      mUpdateSema.timedWait(updateIntvl_ns);
    } else {
      mUpdateSema.waitIndefinitely();
    }

    checkForInputChanges();

    if (hasError()) {
      break;
    }
  }
  DEVLOG_INFO("IODeviceController::runLoop done\n");
}

bool IODeviceController::isHandleValueEqual(IOHandle* paHandle) {
  switch(paHandle->getIOHandleDataType()) {
  case CIEC_ANY::e_BOOL:
    return static_cast<IOHandleGPIO*>(paHandle)->equal();
    break;
  case CIEC_ANY::e_DWORD:
    return paHandle->isInput() && static_cast<IOHandleADC*>(paHandle)->equal();
    break;
  default:
    return false;
    break;
  }
}

