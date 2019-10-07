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

#include "../../resource.h"
#include "../../device.h"
#include "io_configFB_controller.h"

using namespace forte::core::io;

int IOConfigFBController::smMaxErrors = 5;

const char * const IOConfigFBController::scmOK = "OK";
const char * const IOConfigFBController::scmInitializing = "Waiting for initialization..";
const char * const IOConfigFBController::scmFailedToInit = "Failed to initialize controller.";
const char * const IOConfigFBController::scmStopped = "Stopped";

IOConfigFBController::IOConfigFBController(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId,
    TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    IOConfigFBBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData), mStarting(false), mErrorCounter(0), mController(0),
        mPerformRestart(false) {
}

IOConfigFBController::~IOConfigFBController() {
  deInit(true);
}

void IOConfigFBController::executeEvent(int paEIID) {
  if(cg_nExternalEventID == paEIID) {
    if(!(mController->mNotificationHandled = handleNotification(mController->mNotificationType, mController->mNotificationAttachment))) {
      DEVLOG_ERROR("[IOConfigFBController] Notification of type %d has not been handled.\n", mController->mNotificationType);
    }
  } else if(scmEventINITID == paEIID) {
    if(true == QI()) {
      if(!init()) {
        QO() = false;
        sendOutputEvent(scmEventINITOID);
      }
    } else {
      QO() = deInit();
      sendOutputEvent(scmEventINITOID);
    }
  }
}

bool IOConfigFBController::handleNotification(IODeviceController::NotificationType paType, const void* paAttachment) {
  switch(paType){
    case IODeviceController::Error:
      onError();
      if(mStarting) {
        if(0 == paAttachment) {
          paAttachment = scmFailedToInit;
        }

        STATUS() = (const char*) paAttachment;
        DEVLOG_ERROR("[IOConfigFBController] Failed to initialize controller. Reason: %s\n", STATUS().getValue());
      } else {
        STATUS() = (const char*) paAttachment;

        DEVLOG_ERROR("[IOConfigFBController] Runtime error. Reason: %s\n", STATUS().getValue());
      }
      return true;
    case IODeviceController::Success:
      if(mStarting) {
        onStartup();
        return true;
      }
      return false;
    default:
      return false;
  }
}

void IOConfigFBController::onError(bool paIsFatal) {
  mErrorCounter++;
  mPerformRestart = mErrorCounter < smMaxErrors;

  // ReInit IOConfigFBController
  if(mPerformRestart) {
    deInit();
  } else {
    mStarting = false;
    QO() = false;
    sendOutputEvent(scmEventINITOID);

    if(paIsFatal) {
      deInit();
    } else {
      // Reset error counter if it was not a fatal error
      mErrorCounter = 0;
    }
  }
}

bool IOConfigFBController::init(int paDelay) {
  if(mController != 0) {
    DEVLOG_ERROR("[IOConfigFBController] IOConfigFBController has already been initialized.\n");
    return false;
  }

  mController = createDeviceController(getResource().getDevice().getDeviceExecution());
  if(mController == 0) {
    DEVLOG_ERROR("[IOConfigFBController] Failed to create controller.\n");
    return false;
  }

  mController->mDelegate = this;
  mController->setInitDelay(paDelay);

  mStarting = true;
  STATUS() = scmInitializing;

  setConfig();

  setEventChainExecutor(m_poInvokingExecEnv);
  mController->start();

  return true;
}

void IOConfigFBController::initHandle(IODeviceController::HandleDescriptor *paHandleDescriptor) {

  if(paHandleDescriptor->mId == "") {
    return;
  }

  mController->addHandle(paHandleDescriptor);
}

void IOConfigFBController::onStartup() {
  started();
}

void IOConfigFBController::started(const char* paError) {
  if(paError != 0) {
    STATUS() = paError;
    DEVLOG_ERROR("[IOConfigFBController] Failed to start. Reason: %s\n", STATUS().getValue());
    return onError(false);
  }

  mErrorCounter = 0;
  mStarting = false;

  QO() = true;
  STATUS() = scmOK;
  sendOutputEvent(scmEventINITOID);
}

void IOConfigFBController::onStop() {
  stopped();
}

bool IOConfigFBController::deInit(bool paIsDestructing) {
  if(0 == mController) {
    DEVLOG_ERROR("[IOConfigFBController] IOConfigFBController has already been stopped.\n");
    return false;
  }

  if(paIsDestructing) {
    mPerformRestart = false;
    stopped();
  } else {
    onStop();
  }

  return false;
}

void IOConfigFBController::stopped() {
  mController->mDelegate = 0;
  if(mController->isAlive()) {
    mController->end();
  }

  delete mController;
  mController = 0;
  mStarting = false;

  STATUS() = scmStopped;

  if(mPerformRestart) {
    mPerformRestart = false;

    init(mErrorCounter);
  }
}

