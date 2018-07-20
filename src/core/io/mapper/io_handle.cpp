/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#include "io_handle.h"
#include "io_observer.h"

#include <io/device/io_controller.h>

using namespace forte::core::IO;

IOHandle::IOHandle(IODeviceController *controller, IOMapper::Direction direction, CIEC_ANY::EDataTypeID type) :
    controller(controller), observer(0), type(type), direction(
        direction) {

}

IOHandle::~IOHandle() {
  IOMapper::getInstance().deregisterHandle(this);
}

void IOHandle::onObserver(IOObserver *observer) {
  this->observer = observer;
}

void IOHandle::dropObserver() {
  this->observer = 0;
}

void IOHandle::onChange() {
  if (observer != 0) {
    if (observer->onChange()) {
      controller->fireIndicationEvent(observer);
    }
  }
}

