/*******************************************************************************
 * Copyright (c) 2017 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_CORE_COMINFRA_SERIALCOMLAYERBASE_H_
#define SRC_CORE_COMINFRA_SERIALCOMLAYERBASE_H_

#include "comlayer.h"
#include "../datatypes/forte_string.h"

template <typename TSerialHandle, TSerialHandle nullHandle = static_cast<TSerialHandle>(0) >
class CSerialComLayerBase : public forte::com_infra::CComLayer{
  public:

    typedef TSerialHandle TSerialHandleType;

    CSerialComLayerBase(forte::com_infra::CComLayer* paUpperLayer, forte::com_infra::CBaseCommFB * paFB);
    virtual ~CSerialComLayerBase();

    virtual forte::com_infra::EComResponse processInterrupt();

    /*! \brief Perform send to serial interface
    *   \param paData Sendable payload
    *   \param paSize Payload size in bytes
    *
    *   \return ComLayer response
    */
    virtual forte::com_infra::EComResponse sendData(void *paData, unsigned int paSize) = 0;

    /*! \brief Perform reading from serial interface
     *
     * @return if not e_Nothing something was read and the FB should get an external event
     */
    virtual forte::com_infra::EComResponse recvData(const void *paData, unsigned int paSize) = 0;

    virtual void closeConnection() = 0;

  protected:

    enum EForteSerialBaudRate{
      e50 = 50,
      e75 = 75,
      e110 = 100,
      e134C5 = 134, //134.5
      e150 = 150,
      e200 = 200,
      e300 = 300,
      e600 = 600,
      e1200 = 1200,
      e1800 = 1800,
      e2400 = 2400,
      e4800 = 4800,
      e9600 = 9600,
      e14400 = 14400,
      e19200 = 19200,
      e38400 = 38400,
      e57600 = 57600,
      e115200 = 115200,
      e128000 = 128000,
      e256000 = 256000,
      e1000000 = 1000000,
    };

    enum EForteSerialByteSize{
      e5 = 5,
      e6 = 6,
      e7 = 7,
      e8 = 8,
      e9 = 9,
    };

    enum EForteSerialStopBits {
      eOneBit = 1,
      eTwoBits = 2,
      eOne5Bits
    };

    enum EForteSerialParity {
      eNoParity = 0,
      eODD = 1,
      eEven = 2,
      eMark,
      eSpace
    };

    struct SSerialParameters{
      CIEC_STRING interfaceName;
      EForteSerialBaudRate baudRate;
      EForteSerialByteSize byteSize;
      EForteSerialStopBits stopBits;
      EForteSerialParity parity;
    } ;

    char mTerminationSymbol[3]; //**< Space for CR, LF, or CR/LF + Terminating \0
    forte::com_infra::EComResponse openConnection(char *paLayerParameter);
    virtual forte::com_infra::EComResponse openSerialConnection(const SSerialParameters& paSerialParameters, TSerialHandle* paHandleResult) = 0;
    static const unsigned int mMaxRecvBuffer = 1000;

    forte::com_infra::EComResponse mInterruptResp;
    char mRecvBuffer[mMaxRecvBuffer];
    unsigned int mBufFillSize;
    CSyncObject mRecvLock;

    TSerialHandle mSerialHandle;
    TSerialHandle getSerialHandler(){
      return mSerialHandle;
    }

  private:

    enum EForteSerialCommunicationParameter{
      eInterface = 0,
      eBaudrate,
      eByteSize,
      eStopBits,
      eParity,
      eTerminationSymbol,
      eSerComParamterAmount
    };

    static const unsigned int mNoOfParameters = eSerComParamterAmount;

};

#include "serialcomlayerbase.tpp"

#endif /* SRC_CORE_COMINFRA_SERIALCOMLAYERBASE_H_ */
