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
#include "modbusfunction.h"

namespace forte {
  namespace com_infra {
    class CModbusComLayer;
  }
}

class CModbusIOBlock {
  public:
    struct SModbusRange {
      EModbusFunction m_eFunction;
      unsigned int m_nStartAddress;
      unsigned int m_nNrAddresses;
    };
    typedef std::vector<SModbusRange> TModbusRangeList;

    CModbusIOBlock(forte::com_infra::CModbusComLayer* pa_pParent);
    ~CModbusIOBlock();

    forte::com_infra::CModbusComLayer* getParent() const { return m_pParent; }

    void addNewRead(EModbusFunction pa_eFunction, unsigned int pa_nStartAddress, unsigned int pa_nNrAddresses);
    void addNewSend(EModbusFunction pa_eFunction, unsigned int pa_nStartAddress, unsigned int pa_nNrAddresses);

    void allocCache();
    void* getCache() { return m_pCache; }

    const TModbusRangeList& getReads() const { return m_lReads; }
    const TModbusRangeList& getSends() const { return m_lSends; }

    unsigned int getReadSize() const { return m_nReadSize; }
    unsigned int getSendSize() const { return m_nSendSize; }
    static unsigned int getRegisterSize(EModbusFunction pa_eFunction);

  private:
    forte::com_infra::CModbusComLayer *const m_pParent;
    TModbusRangeList m_lReads;
    TModbusRangeList m_lSends;

    void *m_pCache;
    unsigned int m_nReadSize;
    unsigned int m_nSendSize;
};

#endif
