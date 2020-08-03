/*******************************************************************************
 * Copyright (c) 2017 - 2018 fortiss GmbH
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

#include "io_handle_bit.h"

using namespace forte::core::io;

IOHandleBit::IOHandleBit(IODeviceController *paController, IOMapper::Direction paDirection, uint8_t paOffset, uint8_t paPosition, uint8_t* paImage) :
    IOHandle(paController, paDirection, CIEC_ANY::e_BOOL), mOffset(paOffset), mMask((uint8_t) (1 << paPosition)), mImage(paImage) {
}

void IOHandleBit::onObserver(IOObserver *paObserver) {
  IOHandle::onObserver(paObserver);

  if(mDirection == IOMapper::In) {
    CIEC_BOOL state;
    get(state);
    if(state) {
      mController->fireIndicationEvent(paObserver);
    }
  }
}

void IOHandleBit::dropObserver() {
  IOHandle::dropObserver();
  reset();
}

void IOHandleBit::set(const CIEC_ANY &paState) {
  if(static_cast<const CIEC_BOOL&>(paState)) {
    *(mImage + mOffset) = (uint8_t) (*(mImage + mOffset) | mMask);
  } else {
    *(mImage + mOffset) = (uint8_t) (*(mImage + mOffset) & ~mMask);
  }

  mController->handleChangeEvent(this);
}

void IOHandleBit::get(CIEC_ANY &paState) {
  static_cast<CIEC_BOOL&>(paState) = (*(mImage + mOffset) & mMask) != 0;
}

bool IOHandleBit::equal(uint8_t* paOldImage) const {
  return (*(mImage + mOffset) & mMask) == (*(paOldImage + mOffset) & mMask);
}

