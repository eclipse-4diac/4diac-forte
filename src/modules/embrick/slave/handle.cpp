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

SlaveHandle::SlaveHandle(unsigned char* buffer, uint8_t length,
    CSyncObject *syncMutex) :
    buffer(buffer), length(length), observer(NULL), syncMutex(syncMutex) {
}

SlaveHandle::~SlaveHandle() {
}

BitSlaveHandle::BitSlaveHandle(unsigned char* buffer, uint8_t position,
    CSyncObject *syncMutex) :
    SlaveHandle(buffer, 1, syncMutex), mask((uint8_t) (1 << position)) {

}

void BitSlaveHandle::set(bool state) {
  syncMutex->lock();

  if (state)
    *buffer = (uint8_t) (*buffer | mask);
  else
    *buffer = (uint8_t) (*buffer & ~mask);

  syncMutex->unlock();
}

bool BitSlaveHandle::get() {
  syncMutex->lock();

  bool res = (*buffer & mask) != 0;

  syncMutex->unlock();
  return res;
}

bool BitSlaveHandle::equal(unsigned char* oldBuffer) {
  return (*buffer & mask) == (*oldBuffer & mask);
}

} /* namespace EmBrick */
