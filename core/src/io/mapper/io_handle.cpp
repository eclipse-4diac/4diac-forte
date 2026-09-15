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
      mDirection(paDirection) {
  }

  IOHandle::~IOHandle() {
    IOMapper::getInstance().deregisterHandle(*this);
  }

  void IOHandle::onObserver(IOObserver *paObserver) {
    if (paObserver) {
      this->mObservers.push_back(paObserver);
    }
  }

  void IOHandle::dropObserver() {
    this->mObservers.clear();
  }

  void IOHandle::dropObserver(IOObserver *paObserver) {
    if (paObserver) {
      auto iter = std::find(this->mObservers.begin(), this->mObservers.end(), paObserver);
      if (iter != this->mObservers.end()) {
        this->mObservers.erase(iter);
      }
    } else {
      this->mObservers.clear();
    }
  }

  void IOHandle::onChange() {
    auto itEnd(mObservers.end());
    for (auto itObserver = mObservers.begin(); itObserver != itEnd;) {
      if ((*itObserver)->onChange()) {
        mController->fireIndicationEvent((*itObserver));
      }
    }
  }
} // namespace forte::io
