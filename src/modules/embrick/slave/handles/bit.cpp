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

#include "bit.h"
#include "criticalregion.h"

EmbrickBitSlaveHandle::EmbrickBitSlaveHandle(forte::core::io::IODeviceController *paController, forte::core::io::IOMapper::Direction paDirection,
    uint8_t paOffset, uint8_t paPosition, EmbrickSlaveHandler *paSlave) :
    EmbrickSlaveHandle(paController, paDirection, mType = CIEC_ANY::e_BOOL, paOffset, paSlave), mMask((uint8_t) (1 << paPosition)) {

}

void EmbrickBitSlaveHandle::set(const CIEC_ANY &paState) {
  CCriticalRegion criticalRegion(*mUpdateMutex);

  if(static_cast<const CIEC_BOOL&>(paState)) {
    *(mBuffer + mOffset) = (uint8_t) (*(mBuffer + mOffset) | mMask);
  } else {
    *(mBuffer + mOffset) = (uint8_t) (*(mBuffer + mOffset) & ~mMask);
  }

  EmbrickSlaveHandle::set(paState);
}

void EmbrickBitSlaveHandle::get(CIEC_ANY &paState) {
  CCriticalRegion criticalRegion(*mUpdateMutex);
  static_cast<CIEC_BOOL&>(paState) = (*(mBuffer + mOffset) & mMask) != 0;
}

bool EmbrickBitSlaveHandle::equal(unsigned char* paOldBuffer) {
  return (*(mBuffer + mOffset) & mMask) == (*(paOldBuffer + mOffset) & mMask);
}

