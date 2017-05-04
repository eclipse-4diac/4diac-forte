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

#include "analog10.h"

namespace EmBrick {
namespace Handles {

Analog10SlaveHandle::Analog10SlaveHandle(Mapper::Direction direction,
    uint8_t offset, Handlers::Slave *slave) :
    SlaveHandle(direction, offset, slave) {
  type = CIEC_ANY::e_DWORD;
}

void Analog10SlaveHandle::set(const CIEC_ANY &value) {
  slave->syncMutex.lock();

  *(buffer + offset + 1) = static_cast<const CIEC_DWORD&>(value) % 256;
  *(buffer + offset) = (static_cast<const CIEC_DWORD&>(value) / 256) & 0x03;

  slave->syncMutex.unlock();

  SlaveHandle::set(value);
}

void Analog10SlaveHandle::get(CIEC_ANY &value) {
  slave->syncMutex.lock();

  static_cast<CIEC_DWORD&>(value) = getValue(buffer);

  slave->syncMutex.unlock();
}

bool Analog10SlaveHandle::equal(unsigned char* oldBuffer) {
  return getValue(buffer) == getValue(oldBuffer);
}

const CIEC_DWORD Analog10SlaveHandle::getValue(const unsigned char* buffer) {
  return (*(buffer + offset) & 0x03) * 256 + *(buffer + offset + 1);
}

} /* namespace Handles */
} /* namespace EmBrick */
