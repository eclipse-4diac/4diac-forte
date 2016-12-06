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
#include <processinterface.h>

namespace EmBrick {

SlaveHandle::SlaveHandle(unsigned char* buffer, uint8_t length) :
    buffer(buffer), length(length), observer(NULL) {
}

SlaveHandle::~SlaveHandle() {
}

BitSlaveHandle::BitSlaveHandle(unsigned char* buffer, uint8_t position) :
    SlaveHandle(buffer, 1), mask((uint8_t) (1 << position)) {

}

void BitSlaveHandle::set(bool state) {
  // TODO Add global slave mutex
  if (state)
    *buffer = (uint8_t) (*buffer | mask);
  else
    *buffer = (uint8_t) (*buffer & ~mask);
}

bool BitSlaveHandle::get() {
  return (*buffer & mask) != 0;
}

bool BitSlaveHandle::equal(unsigned char* oldBuffer) {
  return (*buffer & mask) == (*oldBuffer & mask);
}

} /* namespace EmBrick */
