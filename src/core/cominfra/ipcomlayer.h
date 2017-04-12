/*******************************************************************************
  * Copyright (c) 2010-2015 fortiss, TU Wien ACIN and Profactor GmbH.
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Alois Zoitl - initial implementation, rework communication infrastructure and bug fixes
  *    Michael Hofmann - layer based communication infrastructure
  *******************************************************************************/
#ifndef IPCOMLAYER_H_
#define IPCOMLAYER_H_

#include <forte_config.h>
#include "comlayer.h"
#include <sockhand.h>

namespace forte {

  namespace com_infra {

    class CIPComLayer : public CComLayer{
      public:
        CIPComLayer(CComLayer* pa_poUpperLayer, CCommFB* pa_poComFB);
        virtual ~CIPComLayer();

        EComResponse sendData(void *pa_pvData, unsigned int pa_unSize); // top interface, called from top
        EComResponse recvData(const void *pa_pvData, unsigned int pa_unSize);

        EComResponse processInterrupt();

      protected:

      private:
        static void closeSocket(CIPComSocketHandler::TSocketDescriptor *pa_nSocketID);

        EComResponse openConnection(char *pa_acLayerParameter);
        void closeConnection();
        void handledConnectedDataRecv();
        void handleConnectionAttemptInConnected();

        CIPComSocketHandler::TSocketDescriptor m_nSocketID;
        CIPComSocketHandler::TSocketDescriptor m_nListeningID; //!> to be used by server type connections. there the m_nSocketID will be used for the accepted connection.
        CIPComSocketHandler::TUDPDestAddr m_tDestAddr;
        EComResponse m_eInterruptResp;
        char m_acRecvBuffer[cg_unIPLayerRecvBufferSize];
        unsigned int m_unBufFillSize;
    };

  }

}

#endif /* IPCOMLAYER_H_ */
