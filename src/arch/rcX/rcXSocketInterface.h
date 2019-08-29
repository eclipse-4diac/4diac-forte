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
#include "../../core/extevhan.h"
#include "../../core/cominfra/comlayer.h"
#include "../../core/utils/singlet.h"
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

    static void closeSocket(TSocketDescriptor pa_nSockD);
    static TSocketDescriptor openTCPServerConnection(char *pa_acIPAddr, unsigned short pa_nPort);
    static TSocketDescriptor openTCPClientConnection(char *pa_acIPAddr, unsigned short pa_nPort);
    static TSocketDescriptor acceptTCPConnection(TSocketDescriptor pa_nListeningSockD);
    static int sendDataOnTCP(TSocketDescriptor pa_nSockD, char* pa_pcData, unsigned int pa_unSize);
    static int receiveDataFromTCP(TSocketDescriptor pa_nSockD, char* pa_pcData, unsigned int pa_unBufSize);

    static TSocketDescriptor openUDPSendPort(char *pa_acIPAddr, unsigned short pa_nPort, TUDPDestAddr *m_ptDestAddr);
    static TSocketDescriptor openUDPReceivePort(char *pa_acIPAddr, unsigned short pa_nPort);
    static int sendDataOnUDP(TSocketDescriptor pa_nSockD, TUDPDestAddr *pa_ptDestAddr, char* pa_pcData, unsigned int pa_unSize);
    static int receiveDataFromUDP(TSocketDescriptor pa_nSockD, char* pa_pcData, unsigned int pa_unBufSize);

    /* Handler functions */

    void addComCallback(TSocketDescriptor pa_nFD, forte::com_infra::CComLayer *pa_poComLayer);
    void removeComCallback(TSocketDescriptor pa_nFD);

    /* functions needed for the external event handler interface */
    void enableHandler(void){
      start();
    }

    void disableHandler(void){
      end();
    }

    void setPriority(int){
      //currently we are doing nothing here.
      //TODO We should adjust the thread priority.
    }

    int getPriority(void) const{
      //the same as for setPriority
      return 0;
    }

  protected:
     virtual void run(void);

  private:

    struct TConnContType{
        TSocketDescriptor m_nSockDes;
        forte::com_infra::CComLayer * m_poCallee;
    };

    typedef CSinglyLinkedList<TConnContType> TConnectionContainer;

    TConnectionContainer m_lstConnectionsList;
    CSyncObject m_oSync;

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

    RX_RESULT openConnection(char *pa_acIPAddr, unsigned short pa_nPort, bool isTCP, bool isServer, TUDPDestAddr *m_ptDestAddr, TSocketDescriptor& pa_destSocket);
    RX_RESULT sendData(TSocketDescriptor pa_nSockD, char* pa_pcData, unsigned int pa_unSize, bool pa_isTCP, TUDPDestAddr *pa_ptDestAddr, void* pa_PacketData, int* pa_result);
    RX_RESULT close(TSocketDescriptor pa_nSockD);
    RX_RESULT accept(TSocketDescriptor pa_listeningSocketDesc, TSocketDescriptor& pa_destSocket);
    RX_RESULT receiveData(TSocketDescriptor pa_nSockD, bool isTcp, char* pa_pcData, unsigned int pa_unBufSize, int* pa_receivedBytes);
    TForteUInt32 stringIpToInt(char* pa_ipString);
    RX_RESULT sendPacketToTCP(UINT32 pa_destId, UINT32 pa_ulLen, UINT32 pa_ulCmd, void* pa_tData, UINT32 pa_dataLength);
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
    bool isInitialized(void);

    TSocketDescriptor socketDescriptorAlloc(void);
    void socketDescriptorDeAlloc(TSocketDescriptor pa_Socket);

    CSinglyLinkedList<FORTE_TCP_PACKET_T*> mWaitingList;
    UINT32 m_unPacketsWaiting;
    TSocketDescriptor mListeningSocketDescriptor;
    tcpResources mForteResources;
    bool m_bInitialized;
};



#endif /* SRC_ARCH_RCX_RCXSOCKETINTERFACE_H_ */
