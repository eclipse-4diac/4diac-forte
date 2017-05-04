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

#include "mapper.h"

namespace IO {

class Observer {
  friend class Mapper;

public:
  Observer();
  virtual ~Observer();

  virtual bool onChange() = 0;

protected:
  virtual void onHandle(Handle *handle);
  virtual void dropHandle();

  Handle* handle;
  CIEC_ANY::EDataTypeID type;
  Mapper::Direction direction;
};

} /* namespace IO */

#endif /* SRC_MODULES_EMBRICK_IO_OBSERVER_H_ */
