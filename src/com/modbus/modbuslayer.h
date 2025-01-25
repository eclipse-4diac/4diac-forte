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
#ifndef MODBUSCOMLAYER_H_
#define MODBUSCOMLAYER_H_

#include <vector>
#include <forte_config.h>
#include "modbusioblock.h"
#include "modbusenums.h"
#include "comlayer.h"
#include <stdint.h>

class CModbusConnection;
class CIEC_ANY;

namespace forte {

  namespace com_infra {

    class CModbusComLayer : public CComLayer{
      public:
        CModbusComLayer(CComLayer* paUpperLayer, CBaseCommFB* paComFB);
        ~CModbusComLayer() override;

        EComResponse sendData(void *paData, unsigned int paSize) override; // top interface, called from top
        EComResponse recvData(const void *paData, unsigned int paSize) override;

        EComResponse processInterrupt() override;

      private:
        struct STcpParams {
          char mIp[16];
          unsigned int mPort;
        };
        struct SRtuParams {
          char mDevice[256];
          int mBaud;
          char mParity;
          int mDataBit;
          int mStopBit;
          EModbusFlowControl mFlowControl;
        };
        struct SAddrRange {
          EModbusFunction mFunction;
          unsigned int mStartAddress;
          unsigned int mNrAddresses;
        };
        struct SCommonParams {
          unsigned int mNrPolls;
          unsigned int mNrSends;
          long mPollFrequency;
          unsigned int mSlaveId;
          SAddrRange mRead[100];
          SAddrRange mSend[100];
          unsigned int mResponseTimeout;
          unsigned int mByteTimeout;
        };
        struct SConnection {
          char mIdString[256];
          unsigned int mUseCount;
          CModbusConnection *mConnection;
        };

        template<typename T>
        T convertFBOutput(TForteByte *paDataArray, unsigned int paDataSize);

        unsigned int convertDataInput(void *paInData, unsigned int paDataSize, void *paConvertedData);

        EComResponse openConnection(char *paLayerParameter) override;
        void closeConnection() override;

        EModbusFunction decodeFunction(const char* paParam, int *strIndex, EModbusFunction paDefaultFunction=eHoldingRegister);
        int processClientParams(const char* paLayerParams, STcpParams* paTcpParams, SRtuParams* paRtuParams, SCommonParams* paCommonParams, char* paIdString);
        int findNextStartAddress(const char* paString, int paStartIndex);
        int findNextStopAddress(const char* paString, int paStartIndex);
        bool isIp(const char* paIp);

        CModbusConnection* getClientConnection(const char* paIdString);
        void putConnection(CModbusConnection *paModbusConn);

        EComResponse mInterruptResp;

        CModbusConnection *mModbusConnection;

        TForteByte mRecvBuffer[cgIPLayerRecvBufferSize];
        unsigned int mBufFillSize;

        CModbusIOBlock m_IOBlock;

        static std::vector<SConnection> smConnections;
    };

  }

}

#endif /* MODBUSCOMLAYER_H_ */
