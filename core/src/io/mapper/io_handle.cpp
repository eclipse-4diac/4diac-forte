/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
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
    this->mObserver = paObserver;
  }

  void IOHandle::dropObserver() {
    this->mObserver = nullptr;
  }

  void IOHandle::onChange() {
    if (mObserver != nullptr && mObserver->onChange()) {
      mController->fireIndicationEvent(mObserver);
    }
  }
} // namespace forte::io
