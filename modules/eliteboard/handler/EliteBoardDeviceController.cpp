/*******************************************************************************
 * Copyright (c) 2021, 2022 Jonathan Lainer (kontakt@lainer.co.at)
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jonathan Lainer - Initial implementation.
 *   Maximilian Scharf - Adaptations for ADC, DAC and event-triggered IOs.
 *******************************************************************************/

#include "EliteBoardDeviceController.h"
#include "handle/PinHandle.h"

#include "forte/util/devlog.h"
#include "forte/datatypes/forte_word.h"

#define POLL_FREQUENCY_Hz 1000
#define PA_POLL_INTERVAL_MS 1000 / POLL_FREQUENCY_Hz

EliteBoardDeviceController::EliteBoardDeviceController(CDeviceExecution &paDeviceExecution) :
    forte::io::IODevicePollController(paDeviceExecution, PA_POLL_INTERVAL_MS) {
}

EliteBoardDeviceController::~EliteBoardDeviceController() {
}

/* starts the loop thread (when it is not already running) which executes the `controllerPoll` function */
void EliteBoardDeviceController::startThread() {
  if (!isAlive()) {
    EliteBoardDeviceController::start();
    DEVLOG_DEBUG("[EliteBoardDeviceController] loop running at %3d Hz / %3d ms cycle time\r\n", POLL_FREQUENCY_Hz,
                 PA_POLL_INTERVAL_MS);
  }
}

EliteBoardDeviceController::IOHandle *EliteBoardDeviceController::createIOHandle(HandleDescriptor &paHandleDescriptor) {
  auto desc_base = static_cast<EliteBoardDescriptor &>(paHandleDescriptor);
  IOHandle *handle;

  switch (desc_base.mIOType) {
    case GPIO: {
      auto desc = static_cast<GPIODescriptor &>(paHandleDescriptor);
      handle = new IOHandleGPIO(this, desc);
      break;
    }
    default: break;
  }
  startThread();
  return static_cast<IOHandle *>(handle);
}

bool EliteBoardDeviceController::isHandleValueEqual(IOHandle &paHandle) {
  return false;
}

void EliteBoardDeviceController::poll() {
  checkForInputChanges();
}
