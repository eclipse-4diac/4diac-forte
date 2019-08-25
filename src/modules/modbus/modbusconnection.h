/*******************************************************************************
 * Copyright (c) 2012 -2014 AIT, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _MODBUSCONNECTION_H_
#define _MODBUSCONNECTION_H_

#include <modbus.h>
#include <forte_thread.h>
#include "modbushandler.h"
#include <errno.h>

namespace forte{
  namespace com_infra{
    class CModbusComLayer;
  }
}

class CModbusConnection : public CThread{
  public:
    explicit CModbusConnection(CModbusHandler* pa_modbusHandler);
    ~CModbusConnection();

    virtual int readData(uint8_t *pa_pData) = 0;
    virtual int writeData(uint16_t *pa_pData, unsigned int pa_nDataSize) = 0;
    virtual void run() = 0;

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
    void setResponseTimeout(unsigned int pa_nResponseTimeout);
    void setByteTimeout(unsigned int pa_nByteTimeout);

    void setComCallback(forte::com_infra::CModbusComLayer* pa_poModbusLayer);

  protected:
    modbus_t* m_pModbusConn;
    CModbusHandler* m_pModbusHandler;

    CModbusHandler::TCallbackDescriptor m_nComCallbackId;

    bool m_bConnected;

  private:
    const char* m_paIPAddress;
    unsigned int m_nPort;

    const char* m_chDevice;
    int m_nBaud;
    char m_cParity;
    int m_nDataBit;
    int m_nStopBit;

    unsigned int m_nResponseTimeout;
    unsigned int m_nByteTimeout;
};

#endif  // _MODBUSCONNECTION_H_
