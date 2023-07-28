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

    virtual int readData(CModbusIOBlock* pa_pIOBlock, void* pa_pData, unsigned int pa_nMaxDataSize) = 0;
    int writeData(CModbusIOBlock* pa_pIOBlock, const void* pa_pData, unsigned int pa_nDataSize);
    virtual void writeDataRange(EModbusFunction pa_eFunction, unsigned int pa_nStartAddress, unsigned int pa_nNrAddresses, const void *pa_pData) = 0;
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

    void setIPAddress(const char* pa_poIPAddress);
    void setPort(unsigned int pa_nPort);

    void setDevice(const char* pa_chDevice);
    void setBaud(int pa_nBaud);
    void setParity(char pa_cParity);
    void setDataBit(int pa_nDataBit);
    void setStopBit(int pa_nStopBit);
    void setFlowControl(EModbusFlowControl pa_enFlowControl);
    void setResponseTimeout(unsigned int pa_nResponseTimeout);
    void setByteTimeout(unsigned int pa_nByteTimeout);

  protected:
    modbus_t* m_pModbusConn;
    CModbusHandler* m_pModbusHandler;

    bool m_bConnected;

    const char* getDevice() const { return m_chDevice; }
    EModbusFlowControl getFlowControl() const { return m_enFlowControl; }

  private:
    const char* m_paIPAddress;
    unsigned int m_nPort;

    char m_chDevice[256];
    int m_nBaud;
    char m_cParity;
    int m_nDataBit;
    int m_nStopBit;
    EModbusFlowControl m_enFlowControl;

    unsigned int m_nResponseTimeout;
    unsigned int m_nByteTimeout;
};

#endif  // _MODBUSCONNECTION_H_
