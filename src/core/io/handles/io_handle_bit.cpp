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

namespace IO {
namespace Handles {

Bit::Bit(Device::Controller *controller, IO::Mapper::Direction direction,
    uint8_t offset, uint8_t position, uint8_t* image) :
    Handle(controller, direction, CIEC_ANY::e_BOOL), image(image), offset(
        offset), mask((uint8_t) (1 << position)) {
}

void Bit::onObserver(IO::Observer *observer) {
  IO::Handle::onObserver(observer);

  if (direction == IO::Mapper::In) {
    CIEC_BOOL state;
    get(state);
    if (state) {
      controller->fireIndicationEvent(observer);
    }
  }
}

void Bit::dropObserver() {
  IO::Handle::dropObserver();

  reset();
}

void Bit::set(const CIEC_ANY &state) {
  if (static_cast<const CIEC_BOOL&>(state))
    *(image + offset) = (uint8_t) (*(image + offset) | mask);
  else
    *(image + offset) = (uint8_t) (*(image + offset) & ~mask);

  controller->handleChangeEvent(this);
}

void Bit::get(CIEC_ANY &state) {
  static_cast<CIEC_BOOL&>(state) = (*(image + offset) & mask) != 0;
}

bool Bit::equal(uint8_t* oldImage) {
  return (*(image + offset) & mask) == (*(oldImage + offset) & mask);
}

} /* namespace Handles */
} /* namespace IO */
