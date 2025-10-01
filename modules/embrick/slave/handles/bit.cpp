/*******************************************************************************
 * Copyright (c) 2016, 2025 Johannes Messmer (admin@jomess.com), fortiss GmbH
 *
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
#include "forte/util/criticalregion.h"

namespace forte::eclipse4diac::io::embrick {

  EmbrickBitSlaveHandle::EmbrickBitSlaveHandle(forte::io::IODeviceController *paController,
                                               forte::io::IOMapper::Direction paDirection,
                                               uint8_t paOffset,
                                               uint8_t paPosition,
                                               EmbrickSlaveHandler *paSlave) :
      EmbrickSlaveHandle(paController, paDirection, CIEC_ANY::e_BOOL, paOffset, paSlave),
      mMask((uint8_t) (1 << paPosition)) {
  }

  void EmbrickBitSlaveHandle::set(const CIEC_ANY &paState) {
    util::CCriticalRegion criticalRegion(*mUpdateMutex);

    if (static_cast<const CIEC_BOOL &>(paState)) {
      *(mBuffer + mOffset) = (uint8_t) (*(mBuffer + mOffset) | mMask);
    } else {
      *(mBuffer + mOffset) = (uint8_t) (*(mBuffer + mOffset) & ~mMask);
    }

    EmbrickSlaveHandle::set(paState);
  }

  void EmbrickBitSlaveHandle::get(CIEC_ANY &paState) {
    util::CCriticalRegion criticalRegion(*mUpdateMutex);
    static_cast<CIEC_BOOL &>(paState) = CIEC_BOOL((*(mBuffer + mOffset) & mMask) != 0);
  }

  bool EmbrickBitSlaveHandle::equal(unsigned char *paOldBuffer) {
    return (*(mBuffer + mOffset) & mMask) == (*(paOldBuffer + mOffset) & mMask);
  }

} // namespace forte::eclipse4diac::io::embrick
