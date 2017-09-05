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

#ifndef SRC_CORE_IO_HANDLES_IO_HANDLE_BIT_H_
#define SRC_CORE_IO_HANDLES_IO_HANDLE_BIT_H_

#include <io/mapper/io_handle.h>
#include <io/device/io_controller.h>

namespace IO {
namespace Handles {

class Bit: public IO::Handle {
public:
  Bit(Device::Controller *controller, IO::Mapper::Direction direction,
      uint8_t offset,
      uint8_t position, uint8_t* image);

  virtual void set(const CIEC_ANY &);
  void get(CIEC_ANY &);

  bool equal(unsigned char* oldImage);

protected:
  virtual void onObserver(IO::Observer *observer);

  virtual void dropObserver();

  virtual void reset() {
    CIEC_BOOL s = false;
    set(s);
  }

  uint8_t* image;
  const uint8_t offset;
  const uint8_t mask;
};

} /* namespace Handles */
} /* namespace IO */

#endif /* SRC_CORE_IO_HANDLES_IO_HANDLE_BIT_H_ */
