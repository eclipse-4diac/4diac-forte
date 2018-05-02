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
#include <forte_sync.h>
#include <forte_wstring.h>
#include <devlog.h>

#include <io/mapper/io_mapper.h>


class EmbrickBusHandler;

class EmbrickSlaveHandler {

public:
  friend class EmbrickBusHandler;

  enum SlaveStatus {
    NotInitialized = 0, //!< Slave requires initialization
    OK = 1, //!< Everything works as expected
    Slow = 200, //!< Update frequency is too low, some errors may occurred
    Interrupted = 201, //!< Slave received no master updates
    Error = 202, //!< Connection has errors. Check hardware
  };

  enum SlaveType {
    UnknownSlave = 0,
    G_8Di8Do = 2181, //!< 8x Digital-Input, 24V, p-switch, 1-wire & 8x Digital-Output, 24V, p-switch, 1-wire
    G_2RelNo4RelCo = 2301 //!< 2x Relay-Output, NO, potential free & 4x Relay-Output, CO, potential free
  };

  struct Config {
    unsigned int UpdateInterval; //!< Sets the default frequency for the data update cycle of slaves. The emBRICK slaves require at least 20 updates per minute. The default value is 25 Hz.
  };

  class Delegate {
  public:
    virtual void onSlaveStatus(SlaveStatus status, SlaveStatus oldStatus) = 0;
    virtual void onSlaveDestroy() = 0;
  };

  void setConfig(Config config);
  Delegate* delegate;

  const unsigned int address;
  unsigned int index() const {
    return address - 1;
  }

  const SlaveType type;

  int update();
  void forceUpdate();

  EmbrickSlaveHandle* getInputHandle(int index) {
    return getHandle(&inputs, index);
  }
  EmbrickSlaveHandle* getOutputHandle(int index) {
    return getHandle(&outputs, index);
  }

  void addHandle(EmbrickSlaveHandle* handle) {
    if (handle->is(IOMapper::In))
      addHandle(&inputs, handle);
    else if (handle->is(IOMapper::Out))
      addHandle(&outputs, handle);
  }

  void dropHandles();

  unsigned char *updateSendImage;
  unsigned char *updateReceiveImage;
  CSyncObject updateMutex;

protected:
  static EmbrickSlaveHandler* sendInit(EmbrickBusHandler* bus, int address);

  EmbrickSlaveHandler(EmbrickBusHandler* bus, int address, EmbrickSlaveInitPackage init);
  virtual ~EmbrickSlaveHandler();

  EmbrickBusHandler* bus;

  Config config;

  const uint8_t dataSendLength;
  const uint8_t dataReceiveLength;
  SlaveStatus status;
  SlaveStatus oldStatus;
  unsigned char *updateReceiveImageOld;

  int updateErrorCounter;
  static const int MaxUpdateErrors;

  CSyncObject handleMutex;
  typedef CSinglyLinkedList<EmbrickSlaveHandle *> TSlaveHandleList;
  TSlaveHandleList inputs;
  TSlaveHandleList outputs;
  void addHandle(TSlaveHandleList* list, EmbrickSlaveHandle* handle);
  EmbrickSlaveHandle* getHandle(TSlaveHandleList* list, int index);

private:
  //!declared but undefined copy constructor as we don't want Slaves to be directly copied.
  EmbrickSlaveHandler(const EmbrickSlaveHandler&);
};

#endif /* SRC_MODULES_EMBRICK_SLAVE_SLAVE_H_ */
