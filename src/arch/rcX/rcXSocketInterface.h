/*******************************************************************************
 * Copyright (c) 2016 - 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_ARCH_RCX_RCXSOCKETINTERFACE_H_
#define SRC_ARCH_RCX_RCXSOCKETINTERFACE_H_

#include <rX_Includes.h>
#include <TcpipConfig.h>
#include <TcpipTcpTask_error.h>
#include <TcpipTcpTask_Functionlist.h>
#include <TcpipTcpTask_Public.h>
#include "../../core/fortelist.h"
#include "core/extevhan.h"
#include "core/cominfra/comlayer.h"
#include "core/util/singlet.h"
#include "forte_sync.h"
#include "forte_thread.h"


class CrcXSocketInterface : public CExternalEventHandler, private CThread{
    DECLARE_SINGLETON(CrcXSocketInterface)
  private:

    /* Task packet union.
     *
     * All the packets, which are used by the task to exchange data with another tasks,
     * are represented by the union.
     */
    union FORTE_TCP_PACKET_T /* Task packet union */
    {
        /* TLR packet header */
        TLR_PACKET_HEADER_T tHead;

        TCPIP_PACKET_TCP_UDP_CMD_OPEN_REQ_T tOpenReq;
        TCPIP_PACKET_TCP_UDP_CMD_OPEN_CNF_T tOpenCnf;

        TCPIP_PACKET_TCP_UDP_CMD_CLOSE_REQ_T tCloseReq;
        TCPIP_PACKET_TCP_UDP_CMD_CLOSE_CNF_T tCloseCnf;

        TCPIP_PACKET_TCP_CMD_WAIT_CONNECT_REQ_T tWaitConnectReq;
        TCPIP_PACKET_TCP_CMD_WAIT_CONNECT_CNF_T tWaitConnectCnf;

        TCPIP_PACKET_TCP_CMD_CONNECT_REQ_T tConnectReq;
        TCPIP_PACKET_TCP_CMD_CONNECT_CNF_T tConnectCnf;

        TCPIP_PACKET_TCP_CMD_SEND_REQ_T tSendTcpReq;
        TCPIP_PACKET_TCP_CMD_SEND_CNF_T tSendTcpCnf;

        TCPIP_PACKET_UDP_CMD_SEND_REQ_T tSendUdpReq;
        TCPIP_PACKET_UDP_CMD_SEND_CNF_T tSendUdpCnf;

        TCPIP_PACKET_TCP_UDP_CMD_RECEIVE_IND_T tRcvInd;

        TCPIP_PACKET_TCP_UDP_CMD_SHUTDOWN_IND_T tShutdownInd;
        TCPIP_PACKET_TCP_UDP_CMD_RECEIVE_STOP_IND_T tStopInd;

        TCPIP_PACKET_TCP_UDP_CMD_CLOSE_ALL_REQ_T tCloseAllReq;
        TCPIP_PACKET_TCP_UDP_CMD_CLOSE_ALL_CNF_T tCloseAllCnf;

        TCPIP_PACKET_TCP_UDP_CMD_SET_SOCK_OPTION_REQ_T tSetSocketOptionsReq;
        TCPIP_PACKET_TCP_UDP_CMD_SET_SOCK_OPTION_CNF_T tSetSocketOptionsCnf;
    };

    struct SSocketDescriptor{
      UINT32 socketNumber;
      bool accepted;
      bool deleteMe;
      UINT32 port;
      FORTE_TCP_PACKET_T* packetReceived;
    };

  public:

    struct TUDPDestAddr{
        UINT32 destPort;
        UINT32 destAddress;
    };

    typedef SSocketDescriptor* TSocketDescriptor;

    static const TSocketDescriptor scmInvalidSocketDescriptor;

    static void closeSocket(TSocketDescriptor paSockD);
    static TSocketDescriptor openTCPServerConnection(char *paIPAddr, unsigned short paPort);
    static TSocketDescriptor openTCPClientConnection(char *paIPAddr, unsigned short paPort);
    static TSocketDescriptor acceptTCPConnection(TSocketDescriptor paListeningSockD);
    static int sendDataOnTCP(TSocketDescriptor paSockD, const char* paData, unsigned int paSize);
    static int receiveDataFromTCP(TSocketDescriptor paSockD, char* paData, unsigned int paBufSize);

    static TSocketDescriptor openUDPSendPort(char *paIPAddr, unsigned short paPort, TUDPDestAddr *mDestAddr);
    static TSocketDescriptor openUDPReceivePort(char *paIPAddr, unsigned short paPort);
    static int sendDataOnUDP(TSocketDescriptor paSockD, TUDPDestAddr *paDestAddr, char* paData, unsigned int paSize);
    static int receiveDataFromUDP(TSocketDescriptor paSockD, char* paData, unsigned int paBufSize);

    /* Handler functions */

    void addComCallback(TSocketDescriptor paFD, forte::com_infra::CComLayer *paComLayer);
    void removeComCallback(TSocketDescriptor paFD);

    /* functions needed for the external event handler interface */
    void enableHandler() override {
      start();
    }

    void disableHandler() override {
      end();
    }

    void setPriority(int) override {
      //currently we are doing nothing here.
      //TODO We should adjust the thread priority.
    }

    int getPriority() const override {
      //the same as for setPriority
      return 0;
    }

  protected:
     virtual void run();

  private:

    struct TConnContType{
        TSocketDescriptor mSockDes;
        forte::com_infra::CComLayer * mCallee;
    };

    typedef CSinglyLinkedList<TConnContType> TConnectionContainer;

    TConnectionContainer mConnectionsList;
    CSyncObject mSync;

    /* End of Handler */

    static char * const scmForteQueueName;
    static char * const scmForteWaitingQueueName;
    static char * const scmFortePoolName;

    struct tcpResources{
        RX_HANDLE tcpTaskHandle;
        TLR_QUE_LINK_T tcpQueueLink;
        RX_HANDLE fortePoolHandle;
        RX_HANDLE forteQueueHandle;
        TLR_QUE_LINK_SOURCE_T forteLinkSource;
        RX_HANDLE forteTask;
        UINT32 sndId;
    };

    RX_RESULT openConnection(char *paIPAddr, unsigned short paPort, bool isTCP, bool isServer, TUDPDestAddr *mDestAddr, TSocketDescriptor& pa_destSocket);
    RX_RESULT sendData(TSocketDescriptor paSockD, char* paData, unsigned int paSize, bool paTCP, TUDPDestAddr *paDestAddr, void* pa_PacketData, int* pa_result);
    RX_RESULT close(TSocketDescriptor paSockD);
    RX_RESULT accept(TSocketDescriptor pa_listeningSocketDesc, TSocketDescriptor& pa_destSocket);
    RX_RESULT receiveData(TSocketDescriptor paSockD, bool isTcp, char* paData, unsigned int paBufSize, int* pa_receivedBytes);
    TForteUInt32 stringIpToInt(char* paString);
    RX_RESULT sendPacketToTCP(UINT32 pa_destId, UINT32 paLen, UINT32 paCmd, void* paData, UINT32 pa_dataLength);
    /*!\brief Waits for packets until timeout.
     *
     *\param pa_command Determine specific command to store in pa_packetResult
     *\param pa_packetResult where the packet received will be stored. If this parameter is null, the packet will be managed by default.
     *\param pa_timeout timeout to wait packets
     *
     */
    RX_RESULT waitPacket(UINT32 pa_command, FORTE_TCP_PACKET_T** pa_packetResult, UINT pa_timeout);
    void managePacketsDefault(FORTE_TCP_PACKET_T* pa_packetResult);
    UINT32 getSocketIDFromPacket(FORTE_TCP_PACKET_T* pa_packet);
    bool isInitialized();

    TSocketDescriptor socketDescriptorAlloc();
    void socketDescriptorDeAlloc(TSocketDescriptor pa_Socket);

    CSinglyLinkedList<FORTE_TCP_PACKET_T*> mWaitingList;
    UINT32 mPacketsWaiting;
    TSocketDescriptor mListeningSocketDescriptor;
    tcpResources mForteResources;
    bool mInitialized;
};



#endif /* SRC_ARCH_RCX_RCXSOCKETINTERFACE_H_ */
