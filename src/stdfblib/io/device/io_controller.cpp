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

#include "io_controller.h"
#include <io/configFB/io_controller.h>

namespace IO {
namespace Device {

Controller::Controller() :
    notificationType(UnknownNotificationType), notificationAttachment(0), notificationHandled(
        true), delegate(0), error(0), initDelay(0) {
}

void Controller::run() {
  // Delay initialization
  if (initDelay > 0)
    CThread::sleepThread(initDelay * 1000);

  if (init()) {
    notifyConfigFB(Success);

    runLoop();
  } else {
    if (!hasError())
      notifyConfigFB(Error);
  }

  if (hasError())
    notifyConfigFB(Error, error);

  deInit();

  // TODO Remove statement or add arch independent sleep method
  while (isAlive())
    ;
}

bool Controller::hasError() {
  return error != 0;
}

void Controller::notifyConfigFB(NotificationType type, const void* attachment) {
  if (delegate == 0) {
    DEVLOG_WARNING(
        "[IO:Device:Controller] No receiver for notification is available. Notification is dropped.\n");
    return;
  }

  if (!notificationHandled) {
    DEVLOG_WARNING(
        "[IO:Device:Controller] Notification has not yet been handled by the configuration fb. Notification is dropped.\n");
    return;
  }

  this->notificationType = type;
  this->notificationAttachment = attachment;

  notificationHandled = false;
  startNewEventChain(delegate);
}

void Controller::setInitDelay(int delay) {
  initDelay = delay;
}

} /* namespace Device */
} /* namespace IO */
