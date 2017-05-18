/*******************************************************************************
 * Copyright (c) 2016 Johannes Messmer (messmer@fortiss.org)
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "io_controller.h"

namespace IO {
namespace ConfigurationFB {

int Controller::MaxErrors = 5;

const char * const Controller::scmOK = "OK";
const char * const Controller::scmInitializing =
    "Waiting for initialization..";
const char * const Controller::scmFailedToInit =
    "Failed to initialize controller.";
const char * const Controller::scmStopped = "Stopped";

Controller::Controller(CResource *pa_poSrcRes,
    const SFBInterfaceSpec *pa_pstInterfaceSpec,
    const CStringDictionary::TStringId pa_nInstanceNameId,
    TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData) :
    Base(pa_poSrcRes, pa_pstInterfaceSpec, pa_nInstanceNameId, pa_acFBConnData,
        pa_acFBVarsData), starting(false), errorCounter(0), controller(0) {

}

Controller::~Controller() {
  deInit();
}

void Controller::executeEvent(int pa_nEIID) {
  switch (pa_nEIID) {

  case cg_nExternalEventID:
    if (!(controller->notificationHandled = handleNotification(
        controller->notificationType, controller->notificationAttachment))) {
      DEVLOG_ERROR(
          "[IO:ConfigFB:Controller] Notification of type %d has not been handled.\n",
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

bool Controller::handleNotification(Device::Controller::NotificationType type,
    const void* attachment) {

  switch (type) {

  case Device::Controller::Error:
    if (starting) {
      if (attachment == 0)
        attachment = scmFailedToInit;

      DEVLOG_ERROR(
          "[IO:ConfigFB:Controller] Failed to initialize controller. Reason: %s\n",
          STATUS().getValue());
    } else {
      DEVLOG_ERROR("[IO:ConfigFB:Controller] Runtime error. Reason: %s\n",
          STATUS().getValue());
    }

    STATUS() = (const char*) attachment;

    onError();
    return true;

  case Device::Controller::Success:
    if (starting) {
      onStartup();
      return true;
    }
    return false;

  default:
    return false;
  }
}

void Controller::onError(bool isFatal) {
  // TODO Add delay

  errorCounter++;
  bool retry = errorCounter < MaxErrors;

  // ReInit Controller
  if (retry) {
    deInit();
    init();
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

bool Controller::init() {
  if (controller != 0) {
    DEVLOG_ERROR(
        "[IO:ConfigFB:Controller] Controller has already been initialized.\n");
    return false;
  }

  controller = createDeviceController();
  if (controller == 0) {
    DEVLOG_ERROR(
        "[IO:ConfigFB:Controller] Failed to create controller.\n");
    return false;
  }

  starting = true;
  STATUS() = scmInitializing;

  setConfig();

  setEventChainExecutor(m_poInvokingExecEnv);
  controller->start();

  return true;
}

void Controller::onStartup() {
  started();
}

void Controller::started(const char* error) {
  if (error != 0) {
    STATUS() = error;
    DEVLOG_ERROR("[IO:ConfigFB:Controller] Failed to start. Reason: %s\n",
        STATUS().getValue());
    return onError(false);
  }

  errorCounter = 0;
  starting = false;

  QO() = true;
  STATUS() = scmOK;
  sendOutputEvent(scm_nEventINITOID);
}

bool Controller::deInit() {
  if (controller == 0) {
    DEVLOG_ERROR(
        "[IO:ConfigFB:Controller] Controller has already been stopped.\n");
    return false;
  }

  if (controller->isAlive())
    controller->end();
  controller = 0;
  starting = false;

  STATUS() = scmStopped;

  return false;
}

} /* namespace ConfigurationFB */
} /* namespace IO */
