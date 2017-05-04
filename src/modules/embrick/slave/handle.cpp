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

#include "handle.h"
#include "slave.h"
#include <handler/bus.h>
#include <io/mapper.h>
#include <devlog.h>
namespace EmBrick {

SlaveHandle::SlaveHandle(Mapper::Direction direction, uint8_t offset,
    Handlers::Slave *slave) :
    Handle(direction), offset(offset), slave(slave) {
  if (direction == Mapper::In)
    buffer = slave->updateReceiveImage;
  else if (direction == Mapper::Out)
    buffer = slave->updateSendImage;
}

SlaveHandle::~SlaveHandle() {
}

void SlaveHandle::set(const CIEC_ANY &) {
  slave->forceUpdate();
}

void SlaveHandle::onObserver(Observer *observer) {
  reset();

  Handle::onObserver(observer);
}

void SlaveHandle::dropObserver() {
  Handle::dropObserver();

  reset();
}

} /* namespace EmBrick */
