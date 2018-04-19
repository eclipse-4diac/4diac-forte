/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "io_controller_poll.h"

IODevicePollController::IODevicePollController(CDeviceExecution& paDeviceExecution, float PollInterval) : IODeviceController(paDeviceExecution),
    PollInterval(PollInterval), forcedLoop(false), loopActive(false) {

}

void IODevicePollController::handleChangeEvent(IOHandle*) {
  forcePoll();
}

void IODevicePollController::runLoop() {
  clock_gettime(CLOCK_MONOTONIC, &nextLoop);

  while (isAlive()) {
    if (!forcedLoop)
      loopSync.wait(nextLoop, (unsigned long) (1000000.0 / PollInterval));
    else
      forcedLoop = false;

    loopActive = true;
    loopSync.unlock();

    // Perform poll operation
    poll();

    if (hasError())
      break;

    loopSync.lock();
    loopActive = false;
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
  loopSync.lock();

  clock_gettime(CLOCK_MONOTONIC, &nextLoop);
  if (loopActive)
    forcedLoop = true;
  else
    loopSync.wakeUp();

  loopSync.unlock();
}
