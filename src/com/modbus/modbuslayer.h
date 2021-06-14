/*******************************************************************************
 * Copyright (c) 2012 -2015 AIT, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef MODBUSCOMLAYER_H_
#define MODBUSCOMLAYER_H_

#include <vector>
#include <forte_config.h>
#include "modbusioblock.h"
#include "modbusfunction.h"
#include "comlayer.h"
#include <stdint.h>

class CModbusConnection;
class CIEC_ANY;

namespace forte {

  namespace com_infra {

    class CModbusComLayer : public CComLayer{
      public:
        CModbusComLayer(CComLayer* pa_poUpperLayer, CBaseCommFB* pa_poComFB);
        ~CModbusComLayer() override;

        EComResponse sendData(void *pa_pvData, unsigned int pa_unSize) override; // top interface, called from top
        EComResponse recvData(const void *pa_pvData, unsigned int pa_unSize) override;

        EComResponse processInterrupt() override;

      private:
        struct STcpParams {
          char m_acIp[15];
          unsigned int m_nPort;
        };
        struct SRtuParams {
          char m_acDevice[256];
          int m_nBaud;
          char m_cParity;
          int m_nDataBit;
          int m_nStopBit;
        };
        struct SAddrRange {
          EModbusFunction m_eFunction;
          unsigned int m_nStartAddress;
          unsigned int m_nNrAddresses;
        };
        struct SCommonParams {
          unsigned int m_nNrPolls;
          unsigned int m_nNrSends;
          long m_nPollFrequency;
          unsigned int m_nSlaveId;
          SAddrRange m_stRead[100];
          SAddrRange m_stSend[100];
          unsigned int m_nResponseTimeout;
          unsigned int m_nByteTimeout;
        };
        struct SConnection {
          char m_acIdString[256];
          unsigned int m_nUseCount;
          CModbusConnection *m_pConnection;
        };

        template<typename T>
        T convertFBOutput(TForteByte *pa_acDataArray, unsigned int pa_nDataSize);

        unsigned int convertDataInput(void *pa_poInData, unsigned int pa_nDataSize, void *pa_poConvertedData);

        EComResponse openConnection(char *pa_acLayerParameter) override;
        void closeConnection() override;

        EModbusFunction decodeFunction(const char* pa_acParam, int *strIndex, EModbusFunction pa_eDefaultFunction=eHoldingRegister);
        int processClientParams(const char* pa_acLayerParams, STcpParams* pa_pTcpParams, SRtuParams* pa_pRtuParams, SCommonParams* pa_pCommonParams, char* pa_acIdString);
        int findNextStartAddress(const char* pa_acString, int pa_nStartIndex);
        int findNextStopAddress(const char* pa_acString, int pa_nStartIndex);
        bool isIp(const char* pa_acIp);

        CModbusConnection* getClientConnection(const char* pa_acIdString);
        void putConnection(CModbusConnection *pa_pModbusConn);

        EComResponse m_eInterruptResp;

        CModbusConnection *m_pModbusConnection;

        TForteByte m_acRecvBuffer[cg_unIPLayerRecvBufferSize];
        unsigned int m_unBufFillSize;

        CModbusIOBlock m_IOBlock;

        static std::vector<SConnection> sm_lstConnections;
    };

  }

}

#endif /* MODBUSCOMLAYER_H_ */
