/*******************************************************************************
 * Copyright (c) 2010-2015 fortiss, TU Wien ACIN and Profactor GmbH.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl - initial implementation, rework communication infrastructure and bug fixes
 *    Michael Hofmann - layer based communication infrastructure
 *******************************************************************************/
#ifndef IPCOMLAYER_H_
#define IPCOMLAYER_H_

#include <sockhand.h>
#include <forte_config.h>
#include "comlayer.h"


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
        CIPComSocketHandler::TSocketDescriptor mSocketID;
        CIPComSocketHandler::TUDPDestAddr mDestAddr;

      private:
        void closeSocket(CIPComSocketHandler::TSocketDescriptor *paSocketID);

        EComResponse openConnection(char *paLayerParameter);
        void closeConnection();
        void handledConnectedDataRecv();
        void handleConnectionAttemptInConnected() const;

        CIPComSocketHandler::TSocketDescriptor mListeningID; //!> to be used by server type connections. there the m_nSocketID will be used for the accepted connection.
        EComResponse mInterruptResp;
        char mRecvBuffer[cg_unIPLayerRecvBufferSize];
        unsigned int mBufFillSize;
    };

  }

}

#endif /* IPCOMLAYER_H_ */
