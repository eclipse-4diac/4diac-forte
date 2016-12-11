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
#include <sync.h>
#include <forte_bool.h>
#include <forte_dword.h>

#include <io/handle.h>

namespace EmBrick {

class SlaveHandle: public IOHandle {
public:
  SlaveHandle(unsigned char* buffer, uint8_t offset, CSyncObject *syncMutex);
  virtual ~SlaveHandle();

  virtual bool equal(unsigned char*) = 0;

protected:
  unsigned char* buffer;
  const uint8_t offset;CSyncObject *syncMutex;
};

class BitSlaveHandle: public SlaveHandle, public IOHandleWrapper<CIEC_BOOL> {
public:
  BitSlaveHandle(unsigned char* buffer, uint8_t offset, uint8_t position,
  CSyncObject *syncMutex);

  void set(CIEC_BOOL state);
  CIEC_BOOL get();

  virtual bool equal(unsigned char* oldBuffer);

protected:
  const uint8_t mask;
};

class Analog10SlaveHandle: public SlaveHandle,
    public IOHandleWrapper<CIEC_DWORD> {
public:
  Analog10SlaveHandle(unsigned char* buffer, uint8_t offset,
      CSyncObject *syncMutex);

  void set(CIEC_DWORD value);
  CIEC_DWORD get();

  virtual bool equal(unsigned char* oldBuffer);

protected:
  CIEC_DWORD getValue(unsigned char* buffer);
};

class AnalogSlaveHandle: public SlaveHandle,
    public IOHandleWrapper<CIEC_DWORD> {
public:
  AnalogSlaveHandle(unsigned char* buffer, uint8_t offset,
      CSyncObject *syncMutex);

  void set(CIEC_DWORD value);
  CIEC_DWORD get();

  virtual bool equal(unsigned char* oldBuffer);

protected:
  CIEC_DWORD getValue(unsigned char* buffer);
};

} /* namespace EmBrick */

#endif /* SRC_MODULES_EMBRICK_SLAVE_HANDLE_H_ */
