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

namespace EmBrick {

SlaveHandle::SlaveHandle(unsigned char* buffer, uint8_t offset,
    CSyncObject *syncMutex) :
    IOHandle(), buffer(buffer), offset(offset), syncMutex(syncMutex) {
}

SlaveHandle::~SlaveHandle() {
}

BitSlaveHandle::BitSlaveHandle(unsigned char* buffer, uint8_t offset,
    uint8_t position, CSyncObject *syncMutex) :
    SlaveHandle(buffer, offset, syncMutex), mask((uint8_t) (1 << position)) {

}

void BitSlaveHandle::set(bool state) {
  syncMutex->lock();

  if (state)
    *(buffer + offset) = (uint8_t) (*(buffer + offset) | mask);
  else
    *(buffer + offset) = (uint8_t) (*(buffer + offset) & ~mask);

  syncMutex->unlock();
}

bool BitSlaveHandle::get() {
  syncMutex->lock();

  bool res = (*(buffer + offset) & mask) != 0;

  syncMutex->unlock();
  return res;
}

bool BitSlaveHandle::equal(unsigned char* oldBuffer) {
  return (*(buffer + offset) & mask) == (*(oldBuffer + offset) & mask);
}

} /* namespace EmBrick */
