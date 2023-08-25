/*******************************************************************************
 * Copyright (c) 2012 - 2023 AIT, fortiss GmbH, Davor Cihlar
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *   Davor Cihlar - multiple FBs sharing a single Modbus connection
 *******************************************************************************/
#ifndef _MODBUSCONNECTION_H_
#define _MODBUSCONNECTION_H_

#include <modbus.h>
#include <forte_thread.h>
#include "modbushandler.h"
#include "modbusioblock.h"
#include "modbusenums.h"
#include <errno.h>

namespace forte{
  namespace com_infra{
    class CModbusComLayer;
  }
}

class CModbusConnection : public CThread{
  public:
    explicit CModbusConnection(CModbusHandler* pa_modbusHandler);
    ~CModbusConnection() override;

    virtual int readData(CModbusIOBlock* paIOBlock, void* paData, unsigned int paMaxDataSize) = 0;
    int writeData(CModbusIOBlock* paIOBlock, const void* paData, unsigned int paDataSize);
    virtual void writeDataRange(EModbusFunction paFunction, unsigned int paStartAddress, unsigned int paNrAddresses, const void *paData) = 0;
    void run() override = 0;

    /*! \brief Initializes Modbus connection
     *
     *  Any classes derived from this class must call CModbusConnection::connect() in the beginning
     *  of their connect() function
     */
    virtual int connect();
    /*! \brief De-initializes Modbus connection
     *
     *  Any classes derived from this class should call CModbusConnection::connect() at the end
     *  of their disconnect() function
     */
    virtual void disconnect();

    void setIPAddress(const char* paIPAddress);
    void setPort(unsigned int paPort);

    void setDevice(const char* paDevice);
    void setBaud(int paBaud);
    void setParity(char paParity);
    void setDataBit(int paDataBit);
    void setStopBit(int paStopBit);
    void setFlowControl(EModbusFlowControl paFlowControl);
    void setResponseTimeout(unsigned int paResponseTimeout);
    void setByteTimeout(unsigned int paByteTimeout);

  protected:
    modbus_t* mModbusConn;
    CModbusHandler* mModbusHandler;

    bool mConnected;

    const char* getDevice() const { return mDevice; }
    EModbusFlowControl getFlowControl() const { return mFlowControl; }

  private:
    const char* mIPAddress;
    unsigned int mPort;

    char mDevice[256];
    int mBaud;
    char mParity;
    int mDataBit;
    int mStopBit;
    EModbusFlowControl mFlowControl;

    unsigned int mResponseTimeout;
    unsigned int mByteTimeout;
};

#endif  // _MODBUSCONNECTION_H_
