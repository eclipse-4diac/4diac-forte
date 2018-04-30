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

#include "../processinterface.h"
#include "../configFB/io_configFB_controller.h"
#include "criticalregion.h"

using namespace forte::core::IO;

IODeviceController::IODeviceController(CDeviceExecution& paDeviceExecution) : CExternalEventHandler(paDeviceExecution),
    notificationType(UnknownNotificationType), notificationAttachment(0), notificationHandled(
        true), delegate(0), error(0), initDelay(0) {
}

void IODeviceController::run() {
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

void IODeviceController::addHandle(HandleDescriptor *handleDescriptor) {
  IOHandle* handle = initHandle(handleDescriptor);

  if (handle == 0) {
    DEVLOG_WARNING(
        "[IODeviceController] Failed to initialize handle '%s'. Check initHandle method.\n",
        handleDescriptor->id.getValue());
    return;
  }

  if (handle->is(IOMapper::In))
    addHandle(&inputHandles, handleDescriptor->id, handle);
  else if (handle->is(IOMapper::Out))
    addHandle(&outputHandles, handleDescriptor->id, handle);
}

void IODeviceController::fireIndicationEvent(IOObserver* observer) {
  startNewEventChain((CProcessInterface*) observer);
}

void IODeviceController::handleChangeEvent(IOHandle*) {
  // EMPTY - Override
}

bool IODeviceController::hasError() {
  return error != 0;
}

void IODeviceController::notifyConfigFB(NotificationType type, const void* attachment) {
  if (delegate == 0) {
    DEVLOG_WARNING(
        "[IODeviceController] No receiver for notification is available. Notification is dropped.\n");
    return;
  }

  if (!notificationHandled) {
    DEVLOG_WARNING(
        "[IODeviceController] Notification has not yet been handled by the configuration fb. Notification is dropped.\n");
    return;
  }

  this->notificationType = type;
  this->notificationAttachment = attachment;

  notificationHandled = false;
  startNewEventChain(delegate);
}

void IODeviceController::checkForInputChanges() {
  CCriticalRegion criticalRegion(handleMutex);

  // Iterate over input handles and check for changes
  THandleList::Iterator itEnd = inputHandles.end();
  for (THandleList::Iterator it = inputHandles.begin(); it != itEnd; ++it)
    if ((*it)->hasObserver() && !isHandleValueEqual(*it)) {
      // Inform Process Interface about change
      (*it)->onChange();
    }
}

void IODeviceController::setInitDelay(int delay) {
  initDelay = delay;
}

void IODeviceController::dropHandles() {
  CCriticalRegion criticalRegion(handleMutex);

  IOMapper& mapper = IOMapper::getInstance();

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

}

bool IODeviceController::isHandleValueEqual(IOHandle*) {
  return true;
}

void IODeviceController::addHandle(THandleList* list, CIEC_WSTRING const &id,
    IOHandle* handle) {
  if (id != "" && IOMapper::getInstance().registerHandle(id, handle)) {
    CCriticalRegion criticalRegion(handleMutex);
    list->push_back(handle);
  } else {
    delete handle;
  }
}

