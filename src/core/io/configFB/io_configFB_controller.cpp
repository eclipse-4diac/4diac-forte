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

const CIEC_WSTRING IOConfigFBController::scmOK("OK");
const CIEC_WSTRING  IOConfigFBController::scmInitializing("Waiting for initialization..");
const CIEC_WSTRING  IOConfigFBController::scmFailedToInit("Failed to initialize controller.");
const CIEC_WSTRING  IOConfigFBController::scmStopped("Stopped");

IOConfigFBController::IOConfigFBController(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId) :
    IOConfigFBBase(paSrcRes, paInterfaceSpec, paInstanceNameId), mStarting(false), mErrorCounter(0), mController(nullptr),
        mPerformRestart(false) {
}

IOConfigFBController::~IOConfigFBController() {
  deInit(nullptr, true);
}

void IOConfigFBController::executeEvent(TEventID paEIID, CEventChainExecutionThread * const paECET) {
  if(cgExternalEventID == paEIID) {
    if(!(mController->mNotificationHandled = handleNotification(mController->mNotificationType, mController->mNotificationAttachment, paECET))) {
      DEVLOG_ERROR("[IOConfigFBController] Notification of type %d has not been handled.\n", static_cast<int>(mController->mNotificationType));
    }
  } else if(scmEventINITID == paEIID) {
    if(true == QI()) {
      if(!init(paECET)) {
        QO() = CIEC_BOOL(false);
        sendOutputEvent(scmEventINITOID, paECET);
      }
    } else {
      QO() = CIEC_BOOL(deInit(paECET));
      sendOutputEvent(scmEventINITOID, paECET);
    }
  }
}

bool IOConfigFBController::handleNotification(IODeviceController::NotificationType paType, const void* paAttachment, CEventChainExecutionThread * const paECET) {
  switch(paType){
    case IODeviceController::NotificationType::Error:
      onError(paECET);
      if(mStarting) {
        if(nullptr == paAttachment) {
          STATUS() = scmFailedToInit;
        } else {
          STATUS() = CIEC_WSTRING((const char*) paAttachment);
        }

        DEVLOG_ERROR("[IOConfigFBController] Failed to initialize controller. Reason: %s\n", STATUS().getValue());
      } else {
        STATUS() = CIEC_WSTRING((const char*) paAttachment);

        DEVLOG_ERROR("[IOConfigFBController] Runtime error. Reason: %s\n", STATUS().getValue());
      }
      return true;
    case IODeviceController::NotificationType::Success:
      if(mStarting) {
        onStartup(paECET);
        return true;
      }
      return false;
    default:
      return false;
  }
}

void IOConfigFBController::onError(CEventChainExecutionThread * const paECET, bool paIsFatal) {
  mErrorCounter++;
  mPerformRestart = mErrorCounter < smMaxErrors;

  // ReInit IOConfigFBController
  if(mPerformRestart) {
    deInit(paECET);
  } else {
    mStarting = false;
    QO() = CIEC_BOOL(false);
    sendOutputEvent(scmEventINITOID, paECET);

    if(paIsFatal) {
      deInit(paECET);
    } else {
      // Reset error counter if it was not a fatal error
      mErrorCounter = 0;
    }
  }
}

bool IOConfigFBController::init(CEventChainExecutionThread * const paECET, int paDelay) {
  if(mController != nullptr) {
    DEVLOG_ERROR("[IOConfigFBController] IOConfigFBController has already been initialized.\n");
    return false;
  }

  mController = createDeviceController(getResource().getDevice().getDeviceExecution());
  if(mController == nullptr) {
    DEVLOG_ERROR("[IOConfigFBController] Failed to create controller.\n");
    return false;
  }

  mController->mDelegate = this;
  mController->setInitDelay(paDelay);

  mStarting = true;
  STATUS() = scmInitializing;

  setConfig();

  setEventChainExecutor(paECET);
  mController->start();

  return true;
}

void IOConfigFBController::initHandle(IODeviceController::HandleDescriptor *paHandleDescriptor) {

  if(paHandleDescriptor->mId.empty()) {
    return;
  }

  mController->addHandle(paHandleDescriptor);
}

void IOConfigFBController::onStartup(CEventChainExecutionThread * const paECET) {
  started(paECET);
}

void IOConfigFBController::started(CEventChainExecutionThread * const paECET, const char* paError) {
  if(paError != nullptr) {
    STATUS() = CIEC_WSTRING(paError);
    DEVLOG_ERROR("[IOConfigFBController] Failed to start. Reason: %s\n", STATUS().getValue());
    return onError(paECET, false);
  }

  mErrorCounter = 0;
  mStarting = false;

  QO() = CIEC_BOOL(true);
  STATUS() = CIEC_WSTRING(scmOK);
  sendOutputEvent(scmEventINITOID, paECET);
}

void IOConfigFBController::onStop(CEventChainExecutionThread * const paECET) {
  stopped(paECET);
}

bool IOConfigFBController::deInit(CEventChainExecutionThread * const paECET, bool paIsDestructing) {
  if(nullptr == mController) {
    DEVLOG_ERROR("[IOConfigFBController] IOConfigFBController has already been stopped.\n");
    return false;
  }

  if(paIsDestructing) {
    mPerformRestart = false;
    stopped(paECET);
  } else {
    onStop(paECET);
  }

  return false;
}

void IOConfigFBController::stopped(CEventChainExecutionThread * const paECET) {
  mController->mDelegate = nullptr;
  if(mController->isAlive()) {
    mController->end();
  }

  delete mController;
  mController = nullptr;
  mStarting = false;

  STATUS() = scmStopped;

  if(mPerformRestart) {
    mPerformRestart = false;
    init(paECET, mErrorCounter);
  }
}

