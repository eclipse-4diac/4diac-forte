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
#include <forte_sync.h>
#include <forte_bool.h>
#include <forte_dword.h>

#include <io/handle.h>

namespace EmBrick {

class Slave;

class SlaveHandle: public IOHandle {
public:
  SlaveHandle(IOHandle::Direction direction, uint8_t offset, Slave *slave);
  virtual ~SlaveHandle();

  virtual void set(const CIEC_ANY &);
  virtual bool equal(unsigned char*) = 0;
  virtual void reset() {

  }

protected:
  virtual void onObserver(IOObserver *observer);
  virtual void dropObserver();

  unsigned char* buffer;
  const uint8_t offset;
  const int index;
  CSyncObject *syncMutex;
};

class BitSlaveHandle: public SlaveHandle {
public:
  BitSlaveHandle(IOHandle::Direction direction, uint8_t offset,
      uint8_t position, Slave *slave);

  virtual void set(const CIEC_ANY &);
  void get(CIEC_ANY &);

  virtual bool equal(unsigned char* oldBuffer);

  virtual void reset() {
    CIEC_BOOL s = false;
    set(s);
  }

protected:
  const uint8_t mask;
};

class Analog10SlaveHandle: public SlaveHandle {
public:
  Analog10SlaveHandle(IOHandle::Direction direction, uint8_t offset,
      Slave *slave);

  virtual void set(const CIEC_ANY &);
  void get(CIEC_ANY &);

  virtual bool equal(unsigned char* oldBuffer);

protected:
  const CIEC_DWORD getValue(const unsigned char* buffer);
};

class AnalogSlaveHandle: public SlaveHandle {
public:
  AnalogSlaveHandle(IOHandle::Direction direction, uint8_t offset,
      Slave *slave);

  virtual void set(const CIEC_ANY &);
  void get(CIEC_ANY &);

  virtual bool equal(unsigned char* oldBuffer);

protected:
  const CIEC_DWORD getValue(const unsigned char* buffer);
};

} /* namespace EmBrick */

#endif /* SRC_MODULES_EMBRICK_SLAVE_HANDLE_H_ */
