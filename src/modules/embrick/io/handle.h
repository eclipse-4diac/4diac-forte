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

protected:
  virtual void onObserver(IOObserver *observer);
  virtual void dropObserver();

  IOObserver *observer;
};

} /* namespace EmBrick */

#endif /* SRC_MODULES_EMBRICK_IO_HANDLE_H_ */
