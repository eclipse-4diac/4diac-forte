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
#include <io/mapper/io_mapper.h>
#include <devlog.h>

EmbrickSlaveHandle::EmbrickSlaveHandle(forte::core::IO::IODeviceController *controller,
    forte::core::IO::IOMapper::Direction direction, CIEC_ANY::EDataTypeID type, uint8_t offset,
    EmbrickSlaveHandler *slave) :
    forte::core::IO::IOHandle(controller, direction, type), offset(offset), slave(slave), updateMutex(
        &slave->updateMutex) {
  if (direction == forte::core::IO::IOMapper::In)
    buffer = slave->updateReceiveImage;
  else if (direction == forte::core::IO::IOMapper::Out)
    buffer = slave->updateSendImage;
}

EmbrickSlaveHandle::~EmbrickSlaveHandle() {
}

void EmbrickSlaveHandle::set(const CIEC_ANY &) {
  slave->forceUpdate();
}

void EmbrickSlaveHandle::onObserver(forte::core::IO::IOObserver *observer) {
  reset();

  forte::core::IO::IOHandle::onObserver(observer);
}

void EmbrickSlaveHandle::dropObserver() {
  forte::core::IO::IOHandle::dropObserver();

  reset();
}


