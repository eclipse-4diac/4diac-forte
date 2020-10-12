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

#include <forte_config.h>
#include "comlayer.h"
#include <stdint.h>

class CModbusConnection;
class CIEC_ANY;

namespace forte {

  namespace com_infra {

    class CModbusComLayer : public CComLayer{
      public:
        CModbusComLayer(CComLayer* pa_poUpperLayer, CBaseCommFB* pa_poComFB);
        virtual ~CModbusComLayer();

        EComResponse sendData(void *pa_pvData, unsigned int pa_unSize); // top interface, called from top
        EComResponse recvData(const void *pa_pvData, unsigned int pa_unSize);

        EComResponse processInterrupt();

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
        struct SCommonParams {
          unsigned int m_nNrPolls;
          unsigned int m_nNrSends;
          long m_nPollFrequency;
          unsigned int m_nFuncCode;
          unsigned int m_nSlaveId;
          unsigned int m_nReadStartAddress[100];
          unsigned int m_nReadNrAddresses[100];
          unsigned int m_nSendStartAddress[100];
          unsigned int m_nSendNrAddresses[100];
          unsigned int m_nResponseTimeout;
          unsigned int m_nByteTimeout;
        };

        template<typename T>
        T convertFBOutput(TForteByte *pa_acDataArray, unsigned int pa_nDataSize);

        unsigned int convertDataInput(void *pa_poInData, unsigned int pa_nDataSize, TForteUInt16 *pa_poConvertedData);

        EComResponse openConnection(char *pa_acLayerParameter);
        void closeConnection();

        //int processClientParams(char* pa_acLayerParams, char* pa_acIp, unsigned int &pa_nPort, long &pa_nPollFrequency, unsigned int &pa_nFuncCode, unsigned int &pa_nSlaveId, unsigned int *pa_nStartAddress, unsigned int *pa_nNrAddresses);
        int processClientParams(char* pa_acLayerParams, STcpParams* pa_pTcpParams, SRtuParams* pa_pRtuParams, SCommonParams* pa_pCommonParams);
        int findNextStartAddress(const char* pa_acString, int pa_nStartIndex);
        int findNextStopAddress(const char* pa_acString, int pa_nStartIndex);
        bool isIp(const char* pa_acIp);

        EComResponse m_eInterruptResp;

        CModbusConnection *m_pModbusConnection;

        TForteByte m_acRecvBuffer[cg_unIPLayerRecvBufferSize];
        unsigned int m_unBufFillSize;
    };

  }

}

#endif /* MODBUSCOMLAYER_H_ */
