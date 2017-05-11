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

#include "bit.h"

namespace EmBrick {
namespace Handles {

BitSlaveHandle::BitSlaveHandle(Mapper::Direction direction, uint8_t offset,
    uint8_t position, Handlers::Slave *slave) :
    SlaveHandle(direction, offset, slave), mask((uint8_t) (1 << position)) {
  type = CIEC_ANY::e_BOOL;
}

void BitSlaveHandle::set(const CIEC_ANY &state) {
  updateMutex->lock();

  if (static_cast<const CIEC_BOOL&>(state))
    *(buffer + offset) = (uint8_t) (*(buffer + offset) | mask);
  else
    *(buffer + offset) = (uint8_t) (*(buffer + offset) & ~mask);

  updateMutex->unlock();

  SlaveHandle::set(state);
}

void BitSlaveHandle::get(CIEC_ANY &state) {
  updateMutex->lock();

  static_cast<CIEC_BOOL&>(state) = (*(buffer + offset) & mask) != 0;

  updateMutex->unlock();
}

bool BitSlaveHandle::equal(unsigned char* oldBuffer) {
  return (*(buffer + offset) & mask) == (*(oldBuffer + offset) & mask);
}

} /* namespace Handles */
} /* namespace EmBrick */
