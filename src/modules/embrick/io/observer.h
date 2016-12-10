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

#ifndef SRC_MODULES_EMBRICK_IO_OBSERVER_H_
#define SRC_MODULES_EMBRICK_IO_OBSERVER_H_

#include "handle.h"

namespace EmBrick {

class IOMapper;

class IOObserver {
  friend class IOMapper;

public:
  IOObserver();
  virtual ~IOObserver();

  virtual bool onChange() = 0;

protected:
  virtual void onHandle(IOHandle *handle);
  virtual void dropHandle();

  IOHandle* handle;
};

} /* namespace EmBrick */

#endif /* SRC_MODULES_EMBRICK_IO_OBSERVER_H_ */
