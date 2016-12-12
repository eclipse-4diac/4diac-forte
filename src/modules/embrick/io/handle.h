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

namespace EmBrick {

class IOMapper;
class IOObserver;

class IOHandle {
  friend class IOMapper;

public:
  IOHandle();
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

  virtual void set(const CIEC_ANY &) = 0;
  virtual void get(CIEC_ANY &) = 0;

protected:
  virtual void onObserver(IOObserver *observer);
  virtual void dropObserver();

  IOObserver *observer;
  CIEC_ANY::EDataTypeID type;

  // DISCUSS Should the handle differentiate between inputs and outputs
};

} /* namespace EmBrick */

#endif /* SRC_MODULES_EMBRICK_IO_HANDLE_H_ */
