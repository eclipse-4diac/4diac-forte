/*******************************************************************************
 * Copyright (c) 2021 Davor Cihlar
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Davor Cihlar - multiple FBs sharing a single Modbus connection
 *******************************************************************************/
#ifndef _MODBUSIOBLOCK_H_
#define _MODBUSIOBLOCK_H_

#include <vector>
#include "modbusenums.h"

namespace forte {
  namespace com_infra {
    class CModbusComLayer;
  }
}

class CModbusIOBlock {
  public:
    struct SModbusRange {
      EModbusFunction mFunction;
      unsigned int mStartAddress;
      unsigned int mNrAddresses;
    };
    typedef std::vector<SModbusRange> TModbusRangeList;

    CModbusIOBlock(forte::com_infra::CModbusComLayer* paParent);
    ~CModbusIOBlock();

    forte::com_infra::CModbusComLayer* getParent() const { return mParent; }

    void addNewRead(EModbusFunction paFunction, unsigned int paStartAddress, unsigned int paNrAddresses);
    void addNewSend(EModbusFunction paFunction, unsigned int paStartAddress, unsigned int paNrAddresses);

    void allocCache();
    void* getCache() { return mCache; }

    const TModbusRangeList& getReads() const { return mReads; }
    const TModbusRangeList& getSends() const { return mSends; }

    unsigned int getReadSize() const { return mReadSize; }
    unsigned int getSendSize() const { return mSendSize; }
    static unsigned int getRegisterSize(EModbusFunction paFunction);

  private:
    forte::com_infra::CModbusComLayer *const mParent;
    TModbusRangeList mReads;
    TModbusRangeList mSends;

    void *mCache;
    unsigned int mReadSize;
    unsigned int mSendSize;
};

#endif
