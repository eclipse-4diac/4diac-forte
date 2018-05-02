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
#include "criticalregion.h"

EmbrickBitSlaveHandle::EmbrickBitSlaveHandle(IODeviceController *controller,
    IOMapper::Direction direction, uint8_t offset,
    uint8_t position, EmbrickSlaveHandler *slave) :
    EmbrickSlaveHandle(controller, direction, type = CIEC_ANY::e_BOOL, offset, slave), mask(
        (uint8_t) (1 << position)) {

}

void EmbrickBitSlaveHandle::set(const CIEC_ANY &state) {
  CCriticalRegion criticalRegion(*updateMutex);

  if (static_cast<const CIEC_BOOL&>(state))
    *(buffer + offset) = (uint8_t) (*(buffer + offset) | mask);
  else
    *(buffer + offset) = (uint8_t) (*(buffer + offset) & ~mask);

  EmbrickSlaveHandle::set(state);
}

void EmbrickBitSlaveHandle::get(CIEC_ANY &state) {
  CCriticalRegion criticalRegion(*updateMutex);
  static_cast<CIEC_BOOL&>(state) = (*(buffer + offset) & mask) != 0;
}

bool EmbrickBitSlaveHandle::equal(unsigned char* oldBuffer) {
  return (*(buffer + offset) & mask) == (*(oldBuffer + offset) & mask);
}

