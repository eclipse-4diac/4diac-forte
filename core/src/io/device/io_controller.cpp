/*******************************************************************************
 * Copyright (c) 2017, 2025 fortiss GmbH, Johannes Kepler University Linz
 *
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

#include "forte/io/device/io_controller.h"

#include "forte/io/processinterfacefb.h"
#include "forte/io/configFB/io_configFB_controller.h"
#include "forte/util/criticalregion.h"

namespace forte::io {
  IODeviceController::IODeviceController(CDeviceExecution &paDeviceExecution) :
      CExternalEventHandler(paDeviceExecution),
      mNotificationType(NotificationType::UnknownNotificationType),
      mNotificationAttachment(nullptr),
      mNotificationHandled(true),
      mError(nullptr),
      mDelegate(nullptr),
      mInitDelay(0) {
  }

  void IODeviceController::run() {
    // Delay initialization
    if (mInitDelay > 0) {
      sleepThread(mInitDelay * 1000);
    }

    mError = init();

    if (!hasError()) {
      notifyConfigFB(NotificationType::Success);

      runLoop();

      if (hasError()) {
        notifyConfigFB(NotificationType::Error, mError);
      }
    } else {
      notifyConfigFB(NotificationType::Error, mError);
    }

    dropHandles();
    deInit();

    while (isAlive()) {
      sleepThread(10);
    }
  }

  void IODeviceController::addHandle(HandleDescriptor &paHandleDescriptor) {
    std::unique_ptr<IOHandle> handle = std::unique_ptr<IOHandle>(createIOHandle(paHandleDescriptor));

    if (!handle) {
      DEVLOG_WARNING("[IODeviceController] Failed to initialize handle '%s'. Check initHandle method.\n",
                     paHandleDescriptor.mId.c_str());
      return;
    }
    addHandle(paHandleDescriptor.mId, std::move(handle));
  }

  void IODeviceController::addHandle(std::string const &paId, std::unique_ptr<IOHandle> paHandle) {
    switch (paHandle->getDirection()) {
      case IOMapper::In: addHandle(mInputHandles, paId, std::move(paHandle)); break;
      case IOMapper::Out: addHandle(mOutputHandles, paId, std::move(paHandle)); break;
      case IOMapper::InOut: addHandle(mDiverseHandles, paId, std::move(paHandle)); break;
      case IOMapper::UnknownDirection: addHandle(mDiverseHandles, paId, std::move(paHandle)); break;
      default: break;
    }
  }

  void IODeviceController::removeHandle(std::string const &paId) {
    IOMapper &mapper = IOMapper::getInstance();
    if (IOHandle *rawHandle = mapper.getHandle(paId); rawHandle) {
      switch (rawHandle->getDirection()) {
        case IOMapper::In: removeHandle(mInputHandles, rawHandle); return;
        case IOMapper::Out: removeHandle(mOutputHandles, rawHandle); return;
        case IOMapper::InOut: removeHandle(mDiverseHandles, rawHandle); return;
        case IOMapper::UnknownDirection: removeHandle(mDiverseHandles, rawHandle); return;
        default: break;
      }
    }
    DEVLOG_WARNING("[IODeviceController] Handle with ID '%s' not found in IOMapper for removal. Cannot remove from "
                   "controller lists.\n",
                   paId.c_str());
  }

  void IODeviceController::removeHandle(THandleList &paList, IOHandle *paRawHandle) {
    util::CCriticalRegion criticalRegion(mHandleMutex);

    IOMapper &mapper = IOMapper::getInstance();

    auto new_end = std::remove_if(paList.begin(), paList.end(), [&](const std::unique_ptr<IOHandle> &handle) {
      if (handle.get() == paRawHandle) {
        mapper.deregisterHandle(*handle);
        return true; // remove
      }
      return false;
    });

    paList.erase(new_end, paList.end());
  }

  void IODeviceController::updateHandleList(std::string const &paId, IOHandle *paHandle) {
    auto it = std::find_if(mDiverseHandles.begin(), mDiverseHandles.end(),
                           [&](const std::unique_ptr<IOHandle> &handle) { return handle.get() == paHandle; });

    if (it == mDiverseHandles.end()) {
      DEVLOG_INFO("[updateHandleList] %s's is no member of mDiverseHandles.\r\n", paId.c_str());
      return;
    }
    auto handleDirection = (*it)->getDirection();
    switch (handleDirection) {
      case IOMapper::In: {
        util::CCriticalRegion criticalRegion(mHandleMutex);
        mInputHandles.push_back(std::move(*it));
        mDiverseHandles.erase(it);
        break;
      }
      case IOMapper::Out: {
        util::CCriticalRegion criticalRegion(mHandleMutex);
        mOutputHandles.push_back(std::move(*it));
        mDiverseHandles.erase(it);
        break;
      }
      default: DEVLOG_ERROR("[updateHandleList] %s's direction is neither `In` nor `Out`.\r\n", paId.c_str()); break;
    }
    DEVLOG_INFO("[updateHandleList] %s's is no member of mDiverseHandles.\r\n", paId.c_str());
  }

  void IODeviceController::fireIndicationEvent(IOObserver *paObserver) {
    startNewEventChain(static_cast<CProcessInterfaceFB *>(paObserver));
  }

  void IODeviceController::handleChangeEvent(IOHandle *) {
    // EMPTY - Override
  }

  bool IODeviceController::hasError() const {
    return mError != nullptr;
  }

  void IODeviceController::notifyConfigFB(NotificationType paType, const void *paAttachment) {
    if (nullptr == mDelegate) {
      DEVLOG_WARNING("[IODeviceController] No receiver for notification is available. Notification is dropped.\n");
      return;
    }

    if (!mNotificationHandled) {
      DEVLOG_WARNING("[IODeviceController] Notification has not yet been handled by the configuration fb. Notification "
                     "is dropped.\n");
      return;
    }

    this->mNotificationType = paType;
    this->mNotificationAttachment = paAttachment;

    mNotificationHandled = false;
    startNewEventChain(mDelegate);
  }

  void IODeviceController::checkForInputChanges() {
    util::CCriticalRegion criticalRegion(mHandleMutex);

    // Iterate over input handles and check for changes
    for (auto &inputHandle : mInputHandles) {
      if (inputHandle->hasObserver() && !isHandleValueEqual(*inputHandle)) {
        // Inform Process Interface about change
        inputHandle->onChange();
      }
    }
  }

  void IODeviceController::setInitDelay(int paDelay) {
    mInitDelay = paDelay;
  }

  void IODeviceController::dropHandles() {
    util::CCriticalRegion criticalRegion(mHandleMutex);

    IOMapper &mapper = IOMapper::getInstance();

    for (auto &inputHandle : mInputHandles) {
      mapper.deregisterHandle(*inputHandle);
    }

    for (auto &outputHandle : mOutputHandles) {
      mapper.deregisterHandle(*outputHandle);
    }

    for (auto &diverseHandle : mDiverseHandles) {
      mapper.deregisterHandle(*diverseHandle);
    }

    mInputHandles.clear();
    mOutputHandles.clear();
    mDiverseHandles.clear();
  }

  bool IODeviceController::isHandleValueEqual(IOHandle &) {
    return true;
  }

  void IODeviceController::addHandle(THandleList &paList, std::string const &paId, std::unique_ptr<IOHandle> paHandle) {
    if (!paId.empty() && IOMapper::getInstance().registerHandle(paId, *paHandle)) {
      util::CCriticalRegion criticalRegion(mHandleMutex);
      paList.push_back(std::move(paHandle));
    }
  }
} // namespace forte::io
