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

#include "analog.h"

namespace EmBrick {
namespace Handles {

AnalogSlaveHandle::AnalogSlaveHandle(Mapper::Direction direction,
    uint8_t offset, Handlers::Slave *slave) :
    SlaveHandle(direction, offset, slave) {
  type = CIEC_ANY::e_DWORD;
}

void AnalogSlaveHandle::set(const CIEC_ANY &value) {
  updateMutex->lock();

  *(buffer + offset + 1) = static_cast<const CIEC_DWORD&>(value) % 256;
  *(buffer + offset) = (unsigned char) (static_cast<const CIEC_DWORD&>(value)
      / 256);

  updateMutex->unlock();

  SlaveHandle::set(value);
}

void AnalogSlaveHandle::get(CIEC_ANY &value) {
  updateMutex->lock();

  static_cast<CIEC_DWORD&>(value) = getValue(buffer);

  updateMutex->unlock();
}

bool AnalogSlaveHandle::equal(unsigned char* oldBuffer) {
  return getValue(buffer) == getValue(oldBuffer);
}

const CIEC_DWORD AnalogSlaveHandle::getValue(const unsigned char* buffer) {
  return *(buffer + offset) * 256 + *(buffer + offset + 1);
}

} /* namespace Handles */
} /* namespace EmBrick */
