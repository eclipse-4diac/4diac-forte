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

#ifndef SRC_STDFBLIB_IO_MAPPER_HANDLE_H_
#define SRC_STDFBLIB_IO_MAPPER_HANDLE_H_

#include <forte_any.h>
#include <forte_bool.h>

#include "io_mapper.h"

namespace IO {

namespace Device {
class Controller;
}

class Handle {
  friend class Mapper;

public:
  Handle(Device::Controller *controller, Mapper::Direction direction);
  virtual ~Handle();

  bool hasObserver() {
    return !!observer;
  }

  Observer* getObserver() {
    return observer;
  }

  bool is(CIEC_ANY::EDataTypeID type) {
    return this->type == type;
  }

  bool is(Mapper::Direction direction) {
    return this->direction == direction;
  }

  virtual void set(const CIEC_ANY &) = 0;
  virtual void get(CIEC_ANY &) = 0;

  void onChange();

protected:
  Device::Controller *controller;

  virtual void onObserver(Observer *observer);
  virtual void dropObserver();

  Observer *observer;
  CIEC_ANY::EDataTypeID type;
  Mapper::Direction direction;
};

} /* namespace IO */

#endif /* SRC_STDFBLIB_IO_MAPPER_HANDLE_H_ */
