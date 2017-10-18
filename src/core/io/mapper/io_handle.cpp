/*******************************************************************************
 * Copyright (c) 2016 Johannes Messmer (admin@jomess.com)
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "io_handle.h"
#include "io_observer.h"

#include <io/device/io_controller.h>

namespace IO {

Handle::Handle(Device::Controller *controller, Mapper::Direction direction, CIEC_ANY::EDataTypeID type) :
    controller(controller), observer(0), type(type), direction(
        direction) {

}

Handle::~Handle() {
  Mapper::getInstance().deregisterHandle(this);
}

void Handle::onObserver(Observer *observer) {
  this->observer = observer;
}

void Handle::dropObserver() {
  this->observer = 0;
}

void Handle::onChange() {
  if (observer != 0) {
    if (observer->onChange()) {
      controller->fireIndicationEvent(observer);
    }
  }
}

} /* namespace IO */
