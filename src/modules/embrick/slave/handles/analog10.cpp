/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#include "analog10.h"
#include "criticalregion.h"

EmbrickAnalog10SlaveHandle::EmbrickAnalog10SlaveHandle(forte::core::IO::IODeviceController *controller,
    forte::core::IO::IOMapper::Direction direction, uint8_t offset, EmbrickSlaveHandler *slave) :
    EmbrickSlaveHandle(controller, direction, CIEC_ANY::e_DWORD, offset, slave) {

}

void EmbrickAnalog10SlaveHandle::set(const CIEC_ANY &value) {
  CCriticalRegion criticalRegion(*updateMutex);

  *(buffer + offset + 1) = static_cast<const CIEC_DWORD&>(value) % 256;
  *(buffer + offset) = (static_cast<const CIEC_DWORD&>(value) / 256) & 0x03;


  EmbrickSlaveHandle::set(value);
}

void EmbrickAnalog10SlaveHandle::get(CIEC_ANY &value) {
  CCriticalRegion criticalRegion(*updateMutex);
  static_cast<CIEC_DWORD&>(value) = getValue(buffer);
}

bool EmbrickAnalog10SlaveHandle::equal(unsigned char* oldBuffer) {
  return getValue(buffer) == getValue(oldBuffer);
}

const CIEC_DWORD EmbrickAnalog10SlaveHandle::getValue(const unsigned char* buffer) {
  return (*(buffer + offset) & 0x03) * 256 + *(buffer + offset + 1);
}



