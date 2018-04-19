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

#ifndef SRC_MODULES_EMBRICK_SLAVE_HANDLES_ANALOG10_H_
#define SRC_MODULES_EMBRICK_SLAVE_HANDLES_ANALOG10_H_

#include <slave/slave.h>
#include <slave/handle.h>

class EmbrickAnalog10SlaveHandle: public EmbrickSlaveHandle {
public:
  EmbrickAnalog10SlaveHandle(IODeviceController *controller,
      IOMapper::Direction direction, uint8_t offset,
      EmbrickSlaveHandler *slave);

  virtual void set(const CIEC_ANY &);
  void get(CIEC_ANY &);

  bool equal(unsigned char* oldBuffer);

protected:
  const CIEC_DWORD getValue(const unsigned char* buffer);
};


#endif /* SRC_MODULES_EMBRICK_SLAVE_HANDLES_ANALOG10_H_ */
