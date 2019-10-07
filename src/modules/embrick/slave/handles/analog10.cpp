/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#include "analog10.h"
#include "criticalregion.h"

EmbrickAnalog10SlaveHandle::EmbrickAnalog10SlaveHandle(forte::core::io::IODeviceController *paController, forte::core::io::IOMapper::Direction paDirection,
    uint8_t paOffset, EmbrickSlaveHandler *paSlave) :
    EmbrickSlaveHandle(paController, paDirection, CIEC_ANY::e_DWORD, paOffset, paSlave) {

}

void EmbrickAnalog10SlaveHandle::set(const CIEC_ANY &paValue) {
  CCriticalRegion criticalRegion(*mUpdateMutex);

  *(mBuffer + mOffset + 1) = static_cast<const CIEC_DWORD&>(paValue) % 256;
  *(mBuffer + mOffset) = (static_cast<const CIEC_DWORD&>(paValue) / 256) & 0x03;

  EmbrickSlaveHandle::set(paValue);
}

void EmbrickAnalog10SlaveHandle::get(CIEC_ANY &paValue) {
  CCriticalRegion criticalRegion(*mUpdateMutex);
  static_cast<CIEC_DWORD&>(paValue) = getValue(mBuffer);
}

bool EmbrickAnalog10SlaveHandle::equal(unsigned char* paOldBuffer) {
  return getValue(mBuffer) == getValue(paOldBuffer);
}

const CIEC_DWORD EmbrickAnalog10SlaveHandle::getValue(const unsigned char* paBuffer) {
  return (*(paBuffer + mOffset) & 0x03) * 256 + *(paBuffer + mOffset + 1);
}

