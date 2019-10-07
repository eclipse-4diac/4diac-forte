/*******************************************************************************
 * Copyright (c) 2017 - 2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#include "io_controller_poll.h"

using namespace forte::core::io;

IODevicePollController::IODevicePollController(CDeviceExecution& paDeviceExecution, float paPollInterval) :
    IODeviceController(paDeviceExecution), mPollInterval(paPollInterval) {
}

void IODevicePollController::handleChangeEvent(IOHandle*) {
  forcePoll();
}

void IODevicePollController::runLoop() {
  while(isAlive()) {
    mForceLoop.timedWait(static_cast<const TForteUInt64>(mPollInterval * 1E6)); //If timeout occurred is a normal waiting, otherwise is a forced loop. Don't care about the return value

    // Perform poll operation
    poll();

    if(hasError()) {
      break;
    }

  }
}

void IODevicePollController::setPollInterval(float paPollInterval) {
  if(paPollInterval <= 0) {
    DEVLOG_WARNING("[IODevicePollController] Configured PollInterval is set to an invalid value '%d'. Set to 25.\n", paPollInterval);
    paPollInterval = 25;
  }

  this->mPollInterval = paPollInterval;
}

void IODevicePollController::forcePoll() {
  mForceLoop.inc();
}

