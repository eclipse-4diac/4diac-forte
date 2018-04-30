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

#ifndef SRC_CORE_IO_MAPPER_HANDLE_H_
#define SRC_CORE_IO_MAPPER_HANDLE_H_

#include <forte_any.h>
#include <forte_bool.h>

#include "io_mapper.h"

namespace forte {
  namespace core {
    namespace IO {

class IODeviceController;

class IOHandle {
  friend class IOMapper;

public:
  IOHandle(IODeviceController *controller, IOMapper::Direction direction, CIEC_ANY::EDataTypeID type);
  virtual ~IOHandle();

  bool hasObserver() {
    return !!observer;
  }

  IOObserver* getObserver() {
    return observer;
  }

  bool is(CIEC_ANY::EDataTypeID type) {
    return this->type == type;
  }

  bool is(IOMapper::Direction direction) {
    return this->direction == direction;
  }

  virtual void set(const CIEC_ANY &) = 0;
  virtual void get(CIEC_ANY &) = 0;

  void onChange();

protected:
  IODeviceController *controller;

  virtual void onObserver(IOObserver *observer);
  virtual void dropObserver();

  IOObserver *observer;
  CIEC_ANY::EDataTypeID type;
  IOMapper::Direction direction;
};

    } //namespace IO
  } //namepsace core
} //namespace forte


#endif /* SRC_CORE_IO_MAPPER_HANDLE_H_ */
