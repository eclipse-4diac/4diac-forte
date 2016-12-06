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

#ifndef SRC_MODULES_EMBRICK_SLAVE_HANDLE_H_
#define SRC_MODULES_EMBRICK_SLAVE_HANDLE_H_

#include <stdint.h>
#include <cstring>

namespace EmBrick {

class ProcessInterface;

class SlaveHandle {
public:
  SlaveHandle(unsigned char* buffer, uint8_t length);
  virtual ~SlaveHandle();

  void prepare();
  virtual bool equal(unsigned char*) = 0;

protected:
  unsigned char* buffer;
  const uint8_t length;
  ProcessInterface *observer;
};

class BitSlaveHandle: public SlaveHandle {
public:
  BitSlaveHandle(unsigned char* buffer, uint8_t position);

  void set(bool state);
  bool get();

  virtual bool equal(unsigned char* oldBuffer);

protected:
  const uint8_t mask;
};

} /* namespace EmBrick */

#endif /* SRC_MODULES_EMBRICK_SLAVE_HANDLE_H_ */
