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
        CIPComLayer(CComLayer* paUpperLayer, CBaseCommFB* paComFB);
        virtual ~CIPComLayer();

        EComResponse sendData(void *paData, unsigned int paSize); // top interface, called from top
        EComResponse recvData(const void *paData, unsigned int paSize);

        EComResponse processInterrupt();

      protected:

      private:
        void closeSocket(CIPComSocketHandler::TSocketDescriptor *paSocketID);

        EComResponse openConnection(char *paLayerParameter);
        void closeConnection();
        void handledConnectedDataRecv();
        void handleConnectionAttemptInConnected();

        CIPComSocketHandler::TSocketDescriptor mSocketID;
        CIPComSocketHandler::TSocketDescriptor mListeningID; //!> to be used by server type connections. there the m_nSocketID will be used for the accepted connection.
        CIPComSocketHandler::TUDPDestAddr mDestAddr;
        EComResponse mInterruptResp;
        char mRecvBuffer[cg_unIPLayerRecvBufferSize];
        unsigned int mBufFillSize;
    };

  }

}

#endif /* IPCOMLAYER_H_ */
