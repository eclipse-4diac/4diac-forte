/*******************************************************************************
 * Copyright (c) 2016 - 2026 Johannes Messmer (admin@jomess.com), fortiss GmbH,
 *                           AVL List GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *   Thomas Oellinger - Add support for multiple observers per input.
 *******************************************************************************/

#include "forte/io/mapper/io_handle.h"
#include "forte/io/mapper/io_observer.h"

#include "forte/io/device/io_controller.h"

namespace forte::io {
  IOHandle::IOHandle(IODeviceController *paController, IOMapper::Direction paDirection, CIEC_ANY::EDataTypeID paType) :
      mController(paController),
      mType(paType),
      mDirection(paDirection),
      mObserver(nullptr) {
  }

  IOHandle::~IOHandle() {
    IOMapper::getInstance().deregisterHandle(*this);
  }

  void IOHandle::onObserver(IOObserver *paObserver) {
    if (IOMapper::In == mDirection) {
      if (paObserver) {
        this->mObservers.push_back(paObserver);
        this->mObserver = paObserver;
      }
    }
    else {
      this->mObserver = paObserver;
    }
  }

  void IOHandle::dropObserver() {
    this->mObserver = nullptr;
  }

  void IOHandle::dropObserver(IOObserver* paObserver) {
    if (paObserver) {
      auto iter = std::find(this->mObservers.begin(), this->mObservers.end(), paObserver);
      if (iter != this->mObservers.end()) {
        this->mObservers.erase(iter);
        if (this->mObservers.empty()) {
          this->mObserver = nullptr;
        }
        else {
          this->mObserver = this->mObservers.at(0);
        }
      }
    }
    else {
      this->mObserver = nullptr;
      this->mObservers.clear();
    }
  }

  void IOHandle::onChange() {
    if (mObserver != nullptr) {
      if (mObservers.empty()) { // Single observer for outputs:
        if (mObserver->onChange()) {
          mController->fireIndicationEvent(mObserver);
        }
      }
      else {  // Multiple observers for inputs:
        auto itEnd(mObservers.end());
        for (auto itObserver = mObservers.begin(); itObserver != itEnd;) {
          if ((*itObserver)->onChange()) {
            mController->fireIndicationEvent((*itObserver));
          }
        }
      }
    }
  }
} // namespace forte::io
