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

#include <io/mapper/io_handle.h>

class EmbrickSlaveHandler;

class EmbrickSlaveHandle: public forte::core::IO::IOHandle {
public:
  EmbrickSlaveHandle(forte::core::IO::IODeviceController *controller, forte::core::IO::IOMapper::Direction direction,
      CIEC_ANY::EDataTypeID type, uint8_t offset, EmbrickSlaveHandler *slave);
  virtual ~EmbrickSlaveHandle();

  virtual void set(const CIEC_ANY &);
  virtual bool equal(unsigned char*) = 0;

protected:
  virtual void reset() {

  }

  virtual void onObserver(forte::core::IO::IOObserver *observer);
  virtual void dropObserver();

  unsigned char* buffer;
  const uint8_t offset;
  EmbrickSlaveHandler* slave;
  CSyncObject *updateMutex;
};

#endif /* SRC_MODULES_EMBRICK_SLAVE_HANDLE_H_ */
