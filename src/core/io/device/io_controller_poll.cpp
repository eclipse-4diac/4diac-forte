/*******************************************************************************
 * Copyright (c) 2017 - 2018 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#include "io_controller_poll.h"

using namespace forte::core::IO;

IODevicePollController::IODevicePollController(CDeviceExecution& paDeviceExecution, float PollInterval) : IODeviceController(paDeviceExecution),
    PollInterval(PollInterval){
}

void IODevicePollController::handleChangeEvent(IOHandle*) {
  forcePoll();
}

void IODevicePollController::runLoop() {
  while (isAlive()) {
    forceLoop.timedWait(PollInterval * 1E6); //If timeout occurred is a normal waiting, otherwise is a forced loop. Don't care about the return value

    // Perform poll operation
    poll();

    if (hasError())
      break;

  }
}

void IODevicePollController::setPollInterval(float PollInterval) {
  if (PollInterval <= 0) {
    DEVLOG_WARNING(
        "[IODevicePollController] Configured PollInterval is set to an invalid value '%d'. Set to 25.\n",
        PollInterval);
    PollInterval = 25;
  }

  this->PollInterval = PollInterval;
}

void IODevicePollController::forcePoll() {
  forceLoop.inc();
}

