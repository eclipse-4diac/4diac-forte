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

#include "../../resource.h"
#include "../../device.h"
#include "io_configFB_controller.h"

using namespace forte::core::IO;

int IOConfigFBController::MaxErrors = 5;

const char * const IOConfigFBController::scmOK = "OK";
const char * const IOConfigFBController::scmInitializing =
    "Waiting for initialization..";
const char * const IOConfigFBController::scmFailedToInit =
    "Failed to initialize controller.";
const char * const IOConfigFBController::scmStopped = "Stopped";

IOConfigFBController::IOConfigFBController(CResource *pa_poSrcRes,
    const SFBInterfaceSpec *pa_pstInterfaceSpec,
    const CStringDictionary::TStringId pa_nInstanceNameId,
    TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData) :
    IOConfigFBBase(pa_poSrcRes, pa_pstInterfaceSpec, pa_nInstanceNameId, pa_acFBConnData,
        pa_acFBVarsData), starting(false), errorCounter(0), controller(0), performRestart(
        false) {

}

IOConfigFBController::~IOConfigFBController() {
  deInit(true);
}

void IOConfigFBController::executeEvent(int pa_nEIID) {
  switch (pa_nEIID) {

  case cg_nExternalEventID:
    if (!(controller->notificationHandled = handleNotification(
        controller->notificationType, controller->notificationAttachment))) {
      DEVLOG_ERROR(
          "[IOConfigFBController] Notification of type %d has not been handled.\n",
          controller->notificationType);
    }
    break;

  case scm_nEventINITID:
    if (true == QI()) {
      if (!init()) {
        QO() = false;
        sendOutputEvent(scm_nEventINITOID);
      }
    } else {
      QO() = deInit();
      sendOutputEvent(scm_nEventINITOID);
    }
    break;
  }
}

bool IOConfigFBController::handleNotification(IODeviceController::NotificationType type,
    const void* attachment) {

  switch (type) {

  case IODeviceController::Error:
    onError();

    if (starting) {
      if (attachment == 0)
        attachment = scmFailedToInit;

      STATUS() = (const char*) attachment;
      DEVLOG_ERROR(
          "[IOConfigFBController] Failed to initialize controller. Reason: %s\n",
          STATUS().getValue());
    } else {
      STATUS() = (const char*) attachment;

      DEVLOG_ERROR("[IOConfigFBController] Runtime error. Reason: %s\n",
          STATUS().getValue());
    }
    return true;

  case IODeviceController::Success:
    if (starting) {
      onStartup();
      return true;
    }
    return false;

  default:
    return false;
  }
}

void IOConfigFBController::onError(bool isFatal) {
  errorCounter++;
  performRestart = errorCounter < MaxErrors;

  // ReInit IOConfigFBController
  if (performRestart) {
    deInit();
  } else {
    starting = false;
    QO() = false;
    sendOutputEvent(scm_nEventINITOID);

    if (isFatal) {
      deInit();
    } else {
      // Reset error counter if it was not a fatal error
      errorCounter = 0;
    }
  }
}

bool IOConfigFBController::init(int delay) {
  if (controller != 0) {
    DEVLOG_ERROR(
        "[IOConfigFBController] IOConfigFBController has already been initialized.\n");
    return false;
  }

  controller = createDeviceController(getResource().getDevice().getDeviceExecution());
  if (controller == 0) {
    DEVLOG_ERROR(
        "[IOConfigFBController] Failed to create controller.\n");
    return false;
  }

  controller->delegate = this;
  controller->setInitDelay(delay);

  starting = true;
  STATUS() = scmInitializing;

  setConfig();

  setEventChainExecutor(m_poInvokingExecEnv);
  controller->start();

  return true;
}

void IOConfigFBController::initHandle(
    IODeviceController::HandleDescriptor *handleDescriptor) {

  if (handleDescriptor->id == "")
    return;

  controller->addHandle(handleDescriptor);
}

void IOConfigFBController::onStartup() {
  started();
}

void IOConfigFBController::started(const char* error) {
  if (error != 0) {
    STATUS() = error;
    DEVLOG_ERROR("[IOConfigFBController] Failed to start. Reason: %s\n",
        STATUS().getValue());
    return onError(false);
  }

  errorCounter = 0;
  starting = false;

  QO() = true;
  STATUS() = scmOK;
  sendOutputEvent(scm_nEventINITOID);
}

void IOConfigFBController::onStop() {
  stopped();
}

bool IOConfigFBController::deInit(bool isDestructing) {
  if (controller == 0) {
    DEVLOG_ERROR(
        "[IOConfigFBController] IOConfigFBController has already been stopped.\n");
    return false;
  }

  if (isDestructing) {
    performRestart = false;

    stopped();
  } else {
    onStop();
  }

  return false;
}

void IOConfigFBController::stopped() {
  controller->delegate = 0;
  if (controller->isAlive())
    controller->end();

  delete controller;
  controller = 0;
  starting = false;

  STATUS() = scmStopped;

  if (performRestart) {
    performRestart = false;

    init(errorCounter);
  }
}

