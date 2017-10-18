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
#include <io/pi/processinterface.h>

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

  error = init();

  if (!hasError()) {
    notifyConfigFB(Success);

    runLoop();

    if (hasError())
      notifyConfigFB(Error, error);
  } else {
    notifyConfigFB(Error, error);
  }

  dropHandles();
  deInit();

  // TODO Remove statement or add arch independent sleep method
  while (isAlive())
    CThread::sleepThread(10);
}

void Controller::addHandle(HandleDescriptor *handleDescriptor) {
  Handle* handle = initHandle(handleDescriptor);

  if (handle == 0) {
    DEVLOG_WARNING(
        "[IO:Device:Controller] Failed to initialize handle '%s'. Check initHandle method.\n",
        handleDescriptor->id.getValue());
    return;
  }

  if (handle->is(Mapper::In))
    addHandle(&inputHandles, handleDescriptor->id, handle);
  else if (handle->is(Mapper::Out))
    addHandle(&outputHandles, handleDescriptor->id, handle);
}

void Controller::fireIndicationEvent(Observer* observer) {
  startNewEventChain((CProcessInterface*) observer);
}

void Controller::handleChangeEvent(Handle*) {
  // EMPTY - Override
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

void Controller::checkForInputChanges() {
  handleMutex.lock();

  // Iterate over input handles and check for changes
  THandleList::Iterator itEnd = inputHandles.end();
  for (THandleList::Iterator it = inputHandles.begin(); it != itEnd; ++it)
    if ((*it)->hasObserver() && !isHandleValueEqual(*it)) {
      // Inform Process Interface about change
      (*it)->onChange();
    }

  handleMutex.unlock();
}

void Controller::setInitDelay(int delay) {
  initDelay = delay;
}

void Controller::dropHandles() {
  handleMutex.lock();

  Mapper& mapper = Mapper::getInstance();

  THandleList::Iterator itEnd = inputHandles.end();
  for (THandleList::Iterator it = inputHandles.begin(); it != itEnd; ++it) {
    mapper.deregisterHandle(*it);
    delete *it;
  }
  itEnd = outputHandles.end();
  for (THandleList::Iterator it = outputHandles.begin(); it != itEnd; ++it) {
    mapper.deregisterHandle(*it);
    delete *it;
  }

  inputHandles.clearAll();
  outputHandles.clearAll();

  handleMutex.unlock();
}

bool Controller::isHandleValueEqual(Handle*) {
  return true;
}

void Controller::addHandle(THandleList* list, CIEC_WSTRING const &id,
    Handle* handle) {
  if (id != "" && Mapper::getInstance().registerHandle(id, handle)) {
    handleMutex.lock();
    list->push_back(handle);
    handleMutex.unlock();
  } else {
    delete handle;
  }
}

} /* namespace Device */
} /* namespace IO */
