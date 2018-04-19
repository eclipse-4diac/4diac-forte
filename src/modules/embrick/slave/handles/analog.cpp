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
#include "criticalregion.h"

EmbrickAnalogSlaveHandle::EmbrickAnalogSlaveHandle(IODeviceController *controller,
    IOMapper::Direction direction, uint8_t offset, EmbrickSlaveHandler *slave) :
    EmbrickSlaveHandle(controller, direction, CIEC_ANY::e_DWORD, offset, slave) {

}

void EmbrickAnalogSlaveHandle::set(const CIEC_ANY &value) {
  CCriticalRegion criticalRegion(*updateMutex);

  *(buffer + offset + 1) = static_cast<const CIEC_DWORD&>(value) % 256;
  *(buffer + offset) = (unsigned char) (static_cast<const CIEC_DWORD&>(value)
      / 256);

  EmbrickSlaveHandle::set(value);
}

void EmbrickAnalogSlaveHandle::get(CIEC_ANY &value) {
  CCriticalRegion criticalRegion(*updateMutex);
  static_cast<CIEC_DWORD&>(value) = getValue(buffer);
}

bool EmbrickAnalogSlaveHandle::equal(unsigned char* oldBuffer) {
  return getValue(buffer) == getValue(oldBuffer);
}

const CIEC_DWORD EmbrickAnalogSlaveHandle::getValue(const unsigned char* buffer) {
  return *(buffer + offset) * 256 + *(buffer + offset + 1);
}
