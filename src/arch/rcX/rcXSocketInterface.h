/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_ARCH_RCX_RCXSOCKETINTERFACE_H_
#define SRC_ARCH_RCX_RCXSOCKETINTERFACE_H_

#include "../datatype.h"
#include <singlet.h>
#include <rX_Includes.h>
#include <TcpipConfig.h>
#include <TcpipTcpTask_error.h>
#include <TcpipTcpTask_Functionlist.h>
#include <TcpipTcpTask_Public.h>

#define FORTE_QUEUE_NAME "FOR_QUE"
#define FORTE_QUEUE_MAX_ELEMENT 32

#define FORTE_POOL_NAME "FOR_POOL"
#define FORTE_POOL_MAX_ELEMENT 32

class CrcXSocketInterface{
  DECLARE_SINGLETON(CrcXSocketInterface);
  public:
    typedef UINT32 TSocketDescriptor;
    typedef struct udpAdressStruct{
    	TSocketDescriptor socketHandler;
        UINT32  destPort;
        UINT32  destAddress;
    }TUDPDestAddr;

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

  private:

    typedef struct tcpResourcesType{
        RX_HANDLE tcpTaskHandle;
        TLR_QUE_LINK_T tcpQueueHandle;
        RX_HANDLE fortePoolHandle;
        RX_HANDLE forteQueueHandle;
        RX_HANDLE forteTask;
        UINT32 sndId;
    } tcpResources;

    /* Task packet union.
     *
     * All the packets, which are used by the task to exchange data with another tasks,
     * are represented by the union.
     */
    typedef union FORTE_TCP_PACKET_Ttag                                                            /* Task packet union */
    {
      /* TLR packet header */
      TLR_PACKET_HEADER_T                           tHead;
      TCPIP_PACKET_IP_CMD_SET_CONFIG_REQ_T          tIpSetCfgReq;
      TCPIP_PACKET_IP_CMD_SET_CONFIG_CNF_T          tIpSetCfgCnf;
      TCPIP_PACKET_TCP_UDP_CMD_OPEN_REQ_T           tTcpUdpCmdOpenReq;
      TCPIP_PACKET_TCP_UDP_CMD_OPEN_CNF_T           tTcpUdpCmdOpenCnf;
      TCPIP_PACKET_TCP_UDP_CMD_RECEIVE_IND_T        tTcpUdpCmdRcvInd;
      TCPIP_PACKET_UDP_CMD_SEND_REQ_T               tTcpUdpCmdSndReq;
      TCPIP_PACKET_UDP_CMD_SEND_CNF_T               tTcpUdpCmdSndCnf;
    }FORTE_TCP_PACKET_T;

    TSocketDescriptor openConnection(char *pa_acIPAddr, unsigned short pa_nPort, bool isTCP, bool isServer, TUDPDestAddr *m_ptDestAddr);
    TForteUInt32 stringIpToInt(char* pa_ipString);
    bool sendPacketToTCP(UINT32 pa_destId, UINT32 pa_ulLen, UINT32 pa_ulCmd, void* pa_tData, UINT32 pa_dataLength);
    FORTE_TCP_PACKET_T* waitPacket(UINT32 pa_command);
    bool isInitialized(void);
    tcpResources forteResources;
    bool initialized;
};



#endif /* SRC_ARCH_RCX_RCXSOCKETINTERFACE_H_ */
