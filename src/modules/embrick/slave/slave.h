/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
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
        unsigned int mUpdateInterval; //!< Sets the default frequency for the data update cycle of slaves. The emBRICK slaves require at least 20 updates per minute. The default value is 25 Hz.
    };

    class Delegate {
      public:
        virtual void onSlaveStatus(SlaveStatus paStatus, SlaveStatus paOldStatus) = 0;
        virtual void onSlaveDestroy() = 0;
    };

    void setConfig(Config paConfig);
    Delegate* mDelegate;

    const unsigned int mAddress;
    unsigned int index() const {
      return mAddress - 1;
    }

    const SlaveType mType;

    int update();
    void forceUpdate();

    EmbrickSlaveHandle* getInputHandle(int paIndex) {
      return getHandle(&mInputs, paIndex);
    }
    EmbrickSlaveHandle* getOutputHandle(int paIndex) {
      return getHandle(&mOutputs, paIndex);
    }

    void addHandle(EmbrickSlaveHandle* paHandle) {
      if(paHandle->is(forte::core::io::IOMapper::In)) {
        addHandle(&mInputs, paHandle);
      } else if(paHandle->is(forte::core::io::IOMapper::Out)) {
        addHandle(&mOutputs, paHandle);
      }
    }

    void dropHandles();

    unsigned char *mUpdateSendImage;
    unsigned char *mUpdateReceiveImage;
    CSyncObject mUpdateMutex;

  protected:
    static EmbrickSlaveHandler* sendInit(EmbrickBusHandler* paBus, int paAddress);

    EmbrickSlaveHandler(EmbrickBusHandler* paBus, int paAddress, EmbrickSlaveInitPackage paInit);
    virtual ~EmbrickSlaveHandler();

    EmbrickBusHandler* mBus;

    Config mConfig;

    const uint8_t mDataSendLength;
    const uint8_t mDataReceiveLength;
    SlaveStatus mStatus;
    SlaveStatus mOldStatus;
    unsigned char *mUpdateReceiveImageOld;

    int mUpdateErrorCounter;
    static const int scmMaxUpdateErrors;

    CSyncObject mHandleMutex;
    typedef CSinglyLinkedList<EmbrickSlaveHandle *> TSlaveHandleList;
    TSlaveHandleList mInputs;
    TSlaveHandleList mOutputs;
    void addHandle(TSlaveHandleList* paList, EmbrickSlaveHandle* paHandle);
    EmbrickSlaveHandle* getHandle(TSlaveHandleList* list, int paIndex);

  private:
    //!declared but undefined copy constructor as we don't want Slaves to be directly copied.
    EmbrickSlaveHandler(const EmbrickSlaveHandler&);
};

#endif /* SRC_MODULES_EMBRICK_SLAVE_SLAVE_H_ */
