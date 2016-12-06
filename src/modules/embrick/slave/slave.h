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

#ifndef SRC_MODULES_EMBRICK_SLAVE_SLAVE_H_
#define SRC_MODULES_EMBRICK_SLAVE_SLAVE_H_

#include <stdint.h>
#include <cstring>
#include "packages.h"
#include "handle.h"
#include <fortelist.h>
#include <stdint.h>

namespace EmBrick {

class BusHandler;

enum SlaveStatus {
  NotInitialized = 0, // Slave requires initialization
  OK = 1, // Everything works as expected
  Slow = 200, // Update frequency is too low, some errors may occurred
  Interrupted = 201, // Slave received no master updates
  Error = 202, // Connection has errors. Check hardware
};

enum SlaveType {
  G_8Di8Do = 2181, // 8x Digital-Input, 24V, p-switch, 1-wire & 8x Digital-Output, 24V, p-switch, 1-wire
  G_2RelNo4RelCo = 2301 // 2x Relay-Output, NO, potential free & 4x Relay-Output, CO, potential free
};

class Slave {
public:
  static Slave* sendInit(int address);
  virtual ~Slave();

  const unsigned int address;

  bool update();
  SlaveHandle* getInputHandle(int index) {
    return getHandle(&inputs, index);
  }
  SlaveHandle* getOutputHandle(int index) {
    return getHandle(&outputs, index);
  }

protected:
  Slave(int address, Packages::SlaveInit init);

  BusHandler *bus;

  const uint8_t deviceId;
  const uint8_t dataSendLength;
  const uint8_t dataReceiveLength;
  SlaveStatus status;

  unsigned char *updateSendImage;
  unsigned char *updateReceiveImage;
  unsigned char *updateReceiveImageOld;

  typedef CSinglyLinkedList<SlaveHandle *> TSlaveHandleList;
  TSlaveHandleList inputs;
  TSlaveHandleList outputs;
  SlaveHandle* getHandle(TSlaveHandleList* list, int index);
};

} /* namespace EmBrick */

#endif /* SRC_MODULES_EMBRICK_SLAVE_SLAVE_H_ */
