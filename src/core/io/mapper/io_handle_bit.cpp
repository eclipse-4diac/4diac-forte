/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "io_handle_bit.h"

using namespace forte::core::IO;

IOHandleBit::IOHandleBit(IODeviceController *controller, IOMapper::Direction direction,
    uint8_t offset, uint8_t position, uint8_t* image) :
    IOHandle(controller, direction, CIEC_ANY::e_BOOL), image(image), offset(
        offset), mask((uint8_t) (1 << position)) {
}

void IOHandleBit::onObserver(IOObserver *observer) {
  IOHandle::onObserver(observer);

  if (direction == IOMapper::In) {
    CIEC_BOOL state;
    get(state);
    if (state) {
      controller->fireIndicationEvent(observer);
    }
  }
}

void IOHandleBit::dropObserver() {
  IOHandle::dropObserver();

  reset();
}

void IOHandleBit::set(const CIEC_ANY &state) {
  if (static_cast<const CIEC_BOOL&>(state))
    *(image + offset) = (uint8_t) (*(image + offset) | mask);
  else
    *(image + offset) = (uint8_t) (*(image + offset) & ~mask);

  controller->handleChangeEvent(this);
}

void IOHandleBit::get(CIEC_ANY &state) {
  static_cast<CIEC_BOOL&>(state) = (*(image + offset) & mask) != 0;
}

bool IOHandleBit::equal(uint8_t* oldImage) {
  return (*(image + offset) & mask) == (*(oldImage + offset) & mask);
}

