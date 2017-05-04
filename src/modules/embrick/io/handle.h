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

#ifndef SRC_MODULES_EMBRICK_IO_HANDLE_H_
#define SRC_MODULES_EMBRICK_IO_HANDLE_H_

#include <cstddef>
#include <forte_any.h>
#include <forte_bool.h>

#include "mapper.h"

namespace IO {

class Handle {
  friend class Mapper;

public:
  Handle(Mapper::Direction direction);
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

protected:
  virtual void onObserver(Observer *observer);
  virtual void dropObserver();

  Observer *observer;
  CIEC_ANY::EDataTypeID type;
  Mapper::Direction direction;
};

} /* namespace IO */

#endif /* SRC_MODULES_EMBRICK_IO_HANDLE_H_ */
