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

#include "rcXSocketInterface.h"
#include "bsdsocketinterf.h"
#include "devlog.h"
#include <string.h>
#include <TLR_Legacy.h>

DEFINE_SINGLETON(CrcXSocketInterface);

CrcXSocketInterface::CrcXSocketInterface(){

  RX_RESULT eRslt;

  initialized = true;
  /* forte Task identification */
  //TODO: Check if it is really necessary
  eRslt = TLR_TSK_IDENTIFY(0, 0, &forteResources.forteTask, 0, 0);
  if(eRslt != TLR_S_OK){
    DEVLOG_ERROR("Couldn't identify own task\n");
    initialized = false;
  }

  /* Create process queue */
  eRslt = TLR_QUE_CREATE(FORTE_QUEUE_NAME, FORTE_QUEUE_MAX_ELEMENT, &forteResources.forteQueueHandle);
  if(eRslt != TLR_S_OK){
    DEVLOG_ERROR("Couldn't create queue\n");
    initialized = false;
  }

  /* Create resource pool for queue packets */
  eRslt = TLR_POOL_CREATE(FORTE_POOL_NAME, FORTE_POOL_MAX_ELEMENT, sizeof(FORTE_TCP_PACKET_T), &forteResources.fortePoolHandle);
  if(eRslt != TLR_S_OK){
    DEVLOG_ERROR("Couldn't create pool\n");
    initialized = false;
  }

  /* Task TCP_UDP */
  //TODO: Check if it is really necessary
  eRslt = TLR_TSK_IDENTIFY(TCP_TASK_NAME, 0, &forteResources.tcpTaskHandle, 0, 0);
  if(eRslt != TLR_S_OK){
    DEVLOG_ERROR("Couldn't identify TCP/IP task\n");
    initialized = false;
  }

  eRslt = TLS_QUE_IDENTIFY(EN_TCPUDP_PROCESS_QUEUE_NAME, 0, &forteResources.tcpQueueHandle);
  if(eRslt != TLR_S_OK){
    DEVLOG_ERROR("Couldn't identify TCP/IP queue\n");
    initialized = false;
  }
}

bool CrcXSocketInterface::isInitialized(){
  return initialized;
}

CrcXSocketInterface::TSocketDescriptor CrcXSocketInterface::openTCPServerConnection(char *pa_acIPAddr, unsigned short pa_nPort){
  DEVLOG_INFO("CrcXSocketInterface: Opening TCP-Server connection at: %s:%d\n", pa_acIPAddr, pa_nPort);
  if(CrcXSocketInterface::getInstance().isInitialized()){
    return CrcXSocketInterface::getInstance().openConnection(pa_acIPAddr, pa_nPort, true, true, 0);
  }
  else{
    DEVLOG_ERROR("CrcXSocketInterface is not initialized\n");
    return -1;
  }
}

CrcXSocketInterface::TSocketDescriptor CrcXSocketInterface::openTCPClientConnection(char *pa_acIPAddr, unsigned short pa_nPort){
  DEVLOG_INFO("CrcXSocketInterface: Opening TCP-Server connection at: %s:%d\n", pa_acIPAddr, pa_nPort);
  if(CrcXSocketInterface::getInstance().isInitialized()){
    return CrcXSocketInterface::getInstance().openConnection(pa_acIPAddr, pa_nPort, true, false, 0);
  }
  else{
    DEVLOG_ERROR("CrcXSocketInterface is not initialized\n");
    return -1;
  }
}

CrcXSocketInterface::TSocketDescriptor CrcXSocketInterface::openUDPSendPort(char *pa_acIPAddr, unsigned short pa_nPort, TUDPDestAddr *m_ptDestAddr){
  DEVLOG_INFO("CrcXSocketInterface: Opening UDP sending connection at: %s:%d\n", pa_acIPAddr, pa_nPort);
  if(CrcXSocketInterface::getInstance().isInitialized()){
    return CrcXSocketInterface::getInstance().openConnection(pa_acIPAddr, pa_nPort, false, false, m_ptDestAddr);
  }
  else{
    DEVLOG_ERROR("CrcXSocketInterface is not initialized\n");
    return -1;
  }
}

CrcXSocketInterface::TSocketDescriptor CrcXSocketInterface::openUDPReceivePort(char *pa_acIPAddr, unsigned short pa_nPort){
  DEVLOG_INFO("CrcXSocketInterface: Opening UDP sending connection at: %s:%d\n", pa_acIPAddr, pa_nPort);
  if(CrcXSocketInterface::getInstance().isInitialized()){
    return CrcXSocketInterface::getInstance().openConnection(pa_acIPAddr, pa_nPort, false, true, 0);
  }
  else{
    DEVLOG_ERROR("CrcXSocketInterface is not initialized\n");
    return -1;
  }
}

void CrcXSocketInterface::closeSocket(TSocketDescriptor pa_nSockD){
  TCPIP_DATA_TCP_UDP_CMD_CLOSE_REQ_T tDataClose;
  tDataClose.ulTimeout = 0; /* Default timeout (13000 milliseconds). Must be zero for UDP */
  if(CrcXSocketInterface::getInstance().isInitialized()){
    CrcXSocketInterface::getInstance().sendPacketToTCP(pa_nSockD, TCPIP_DATA_TCP_UDP_CMD_CLOSE_REQ_SIZE, TCPIP_TCP_UDP_CMD_CLOSE_REQ, &tDataClose, sizeof(TCPIP_DATA_TCP_UDP_CMD_CLOSE_REQ_T));
    //TODO: When receiving the CNF it should be just deleted.
  }
  else{
    DEVLOG_ERROR("CrcXSocketInterface is not initialized\n");
  }
}

CrcXSocketInterface::TSocketDescriptor CrcXSocketInterface::openConnection(char *pa_acIPAddr, unsigned short pa_nPort, bool isTCP, bool isServer, TUDPDestAddr *m_ptDestAddr){
  TSocketDescriptor nRetVal = -1;

  TCPIP_DATA_TCP_UDP_CMD_OPEN_REQ_T tDataOpen;
  tDataOpen.ulIpAddr = 0; /* Bind socket to currently configured IP address */
  tDataOpen.ulPort = pa_nPort;
  tDataOpen.ulProtocol = (isTCP) ? TCP_PROTO_TCP : TCP_PROTO_UDP;

  if(sendPacketToTCP(0, TCPIP_DATA_TCP_UDP_CMD_OPEN_REQ_SIZE, TCPIP_TCP_UDP_CMD_OPEN_REQ, &tDataOpen, sizeof(TCPIP_DATA_TCP_UDP_CMD_OPEN_REQ_T))){
    TCPIP_PACKET_TCP_UDP_CMD_OPEN_CNF_T* cnfPacket = (TCPIP_PACKET_TCP_UDP_CMD_OPEN_CNF_T*) waitPacket(TCPIP_TCP_UDP_CMD_OPEN_CNF);
    if(0 != cnfPacket){
      if(RX_OK == cnfPacket->tHead.ulSta){
        if(!isTCP){ //Client UDP
          if(!isServer){
            m_ptDestAddr->destPort = cnfPacket->tData.ulPort;
            m_ptDestAddr->destAddress = cnfPacket->tData.ulIpAddr;
          }
          nRetVal = cnfPacket->tHead.ulDestId;
        }
        else if(isTCP){
          if(isServer){ //TCP server
            //call wait connection and return
            TCPIP_DATA_TCP_CMD_WAIT_CONNECT_REQ_T tDataWait;
            tDataWait.ulTimeoutSend = 0;
            tDataWait.ulTimeoutListen = 0; /*Wait until connection comes in*/

            if(sendPacketToTCP(cnfPacket->tHead.ulDestId, TCPIP_DATA_TCP_CMD_WAIT_CONNECT_REQ_SIZE, TCPIP_TCP_CMD_WAIT_CONNECT_REQ, &tDataWait, sizeof(TCPIP_DATA_TCP_CMD_WAIT_CONNECT_REQ_T))){
              nRetVal = cnfPacket->tHead.ulDestId;
            }else{
              //error sending wait command
            }
          }
          else{ //TCP CLient
            //call connect and wait for confirm
            TCPIP_DATA_TCP_CMD_CONNECT_REQ_T tDataConnect;
            tDataConnect.ulIpAddr = CrcXSocketInterface::getInstance().stringIpToInt(pa_acIPAddr); /* IP address of remote server to connect to */
            if(0 != tDataConnect.ulIpAddr){
              tDataConnect.ulPort = pa_nPort; /* Port number of remote server to connect to */
              tDataConnect.ulTimeoutSend = 0; /* IP address of remote server to connect to */
              tDataConnect.ulTimeoutConnect = 0; /* Default timeout (31000 milliseconds) */

              if(sendPacketToTCP(cnfPacket->tHead.ulDestId, TCPIP_DATA_TCP_CMD_CONNECT_REQ_SIZE, TCPIP_TCP_CMD_CONNECT_REQ, &tDataConnect, sizeof(TCPIP_DATA_TCP_CMD_CONNECT_REQ_T))){
                TCPIP_PACKET_TCP_CMD_CONNECT_CNF_T* connectPacket = (TCPIP_PACKET_TCP_CMD_CONNECT_CNF_T*) waitPacket(TCPIP_TCP_CMD_CONNECT_CNF);
                if(0 != cnfPacket){
                  if(RX_OK == cnfPacket->tHead.ulSta){
                    nRetVal = cnfPacket->tHead.ulDestId;
                  }
                  else{
                    //connect packet is not OK
                  }
                }
                else{
                  //connect packet didn't returned a valid packet
                }
                TLR_QUE_PACKETDONE(forteResources.fortePoolHandle, forteResources.forteQueueHandle, &connectPacket);
              }
            }
            else{
              //wrong IP
            }
          }
        }
        TLR_QUE_PACKETDONE(forteResources.fortePoolHandle, forteResources.forteQueueHandle, cnfPacket);
      }
      else{
        //returned packet has no OK status
      }
    }
    else{
      //waitPacket returned null
    }
    TLR_QUE_PACKETDONE(forteResources.fortePoolHandle, forteResources.forteQueueHandle, cnfPacket);
  }
  else{
    //error sending the packet
  }
  return nRetVal;
}

TForteUInt32 CrcXSocketInterface::stringIpToInt(char* pa_ipString){
  TForteUInt32 result = 0;
  char* currentCharacter;
  int currentNumber = 0;
  int ipNumberCounter = 0;
  currentCharacter = pa_ipString;

  while (1) {
    if('0' <= *currentCharacter && '9' >= *currentCharacter){
      currentNumber *= 10;
      currentNumber += *currentCharacter - '0';
    }else if('.' == *currentCharacter){
      if(256 <= currentNumber){
        return 0;
      }
      ipNumberCounter++;
      result <<= 8;
      result += currentNumber;
      currentNumber = 0;
    }else if('\0' == *currentCharacter){
      if(ipNumberCounter == 4){
            break;
      }else{
        return 0;
      }
    }else{
      return 0;
    }
    currentCharacter++;
  }
  return result;
}


bool CrcXSocketInterface::sendPacketToTCP(UINT32 pa_destId, UINT32 pa_ulLen, UINT32 pa_ulCmd, void* pa_tData, UINT32 pa_dataLength){
  FORTE_TCP_PACKET_Ttag* ptPck;
  if(TLR_S_OK == TLR_POOL_PACKET_GET(forteResources.fortePoolHandle, &ptPck)){

    TLS_QUE_LINK_SET_NEW_DESTID(forteResources.tcpQueueHandle, pa_destId);
    ptPck->tHead.ulSrc = (TLR_UINT32)forteResources.forteQueueHandle;
    ptPck->tHead.ulSrcId = (TLR_UINT32)this;
    ptPck->tHead.ulLen = pa_ulLen;
    ptPck->tHead.ulId = ++forteResources.sndId;
    ptPck->tHead.ulSta = 0;
    ptPck->tHead.ulCmd = pa_ulCmd;
    ptPck->tHead.ulExt = 0;
    ptPck->tHead.ulRout = 0;
    TLR_MEMCPY(pa_tData, &((TCPIP_PACKET_IP_CMD_SET_CONFIG_REQ_T*)&ptPck)->tData, pa_dataLength);

    if(TLS_QUE_SENDPACKET_FIFO(forteResources.tcpQueueHandle, ptPck, 100) == TLR_S_OK){
      return true;
    }else{
      TLR_POOL_PACKET_RELEASE(forteResources.fortePoolHandle, ptPck);
    }
  }

  return false;
}

CrcXSocketInterface::FORTE_TCP_PACKET_T* CrcXSocketInterface::waitPacket(UINT32 pa_command){
  FORTE_TCP_PACKET_T* ptPck = 0;
  do{
    if(RX_OK == TLR_QUE_WAITFORPACKET(forteResources.forteQueueHandle, &ptPck, RX_INFINITE)){
      if(pa_command == ptPck->tHead.ulCmd){
        break;
      }
      else{
        //what to do with packets that aren't waited yet. Maybe use rX_QueSendPriorityPacket()
      }
    }
    else{
      //error waiting packet?
    }
  } while(1);

  return ptPck;
}

int CrcXSocketInterface::sendData(TSocketDescriptor pa_nSockD, char* pa_pcData,
    unsigned int pa_unSize, bool pa_isTCP, TUDPDestAddr *pa_ptDestAddr, void* pa_PacketData){

  int nRetVal = 0;
  //Set not changing variables in the data
  if (pa_isTCP){
    ((TCPIP_DATA_TCP_CMD_SEND_REQ_T*)pa_PacketData)->ulOptions = 0; /* TCP_SEND_OPT_PUSH(0x00000001) Push flag: If set, the stack send the data immediate*/
  }else{
    ((TCPIP_DATA_UDP_CMD_SEND_REQ_T*)pa_PacketData)->ulIpAddr = pa_ptDestAddr->destAddress;
    ((TCPIP_DATA_UDP_CMD_SEND_REQ_T*)pa_PacketData)->ulPort = pa_ptDestAddr->destPort;
  }

  while(0 != pa_unSize){
    bool sendResult;
    if(pa_isTCP){
      nRetVal = (pa_unSize > 1460) ? 1460 : pa_unSize;
      TLR_MEMCPY(pa_pcData, &(((TCPIP_DATA_TCP_CMD_SEND_REQ_T*)pa_PacketData)->abData[0]), nRetVal);
      sendResult = sendPacketToTCP(pa_nSockD, TCPIP_DATA_TCP_CMD_SEND_REQ_SIZE + nRetVal, TCPIP_TCP_CMD_SEND_REQ, &pa_PacketData, sizeof(UINT32) + nRetVal);
    }
    else{
      nRetVal = (pa_unSize > 1472) ? 1472 : pa_unSize;
      TLR_MEMCPY(pa_pcData, &(((TCPIP_DATA_UDP_CMD_SEND_REQ_T*)pa_PacketData)->abData[0]), nRetVal);
      sendResult = sendPacketToTCP(pa_nSockD, TCPIP_DATA_UDP_CMD_SEND_REQ_SIZE + nRetVal, TCPIP_UDP_CMD_SEND_REQ, &pa_PacketData, sizeof(UINT32) * 3 + nRetVal);
    }

    if(sendResult){
      FORTE_TCP_PACKET_T* cnfPacket = (FORTE_TCP_PACKET_T*) waitPacket((pa_isTCP) ? TCPIP_TCP_CMD_SEND_CNF : TCPIP_UDP_CMD_SEND_CNF);
      if(0 != cnfPacket){
        if(RX_OK != cnfPacket->tHead.ulSta){
          //error sending packet
          nRetVal = 1;
        }
        else{

        }
        TLR_POOL_PACKET_RELEASE(forteResources.fortePoolHandle, cnfPacket);
      }
      else{
        //packet didn't arrive?
        nRetVal = -1;
      }
    }
    else{
      //error sending packet
      nRetVal = -1;
    }
    if(nRetVal <= 0){
      DEVLOG_ERROR("TCP-Socket Send failed: %s\n", strerror(errno));
      break;
    }
    pa_unSize -= nRetVal;
    pa_pcData += nRetVal;
  }

  return nRetVal;
}

int CrcXSocketInterface::sendDataOnTCP(TSocketDescriptor pa_nSockD, char* pa_pcData,
    unsigned int pa_unSize){
  if(CrcXSocketInterface::getInstance().isInitialized()){
    TCPIP_DATA_TCP_CMD_SEND_REQ_T tData;
    return CrcXSocketInterface::getInstance().sendData(pa_nSockD, pa_pcData, pa_unSize, true, 0, &tData);
  }
  else{
    DEVLOG_ERROR("CrcXSocketInterface is not initialized\n");
    return -1;
  }
}

int CrcXSocketInterface::sendDataOnUDP(TSocketDescriptor pa_nSockD, TUDPDestAddr *pa_ptDestAddr,
    char* pa_pcData, unsigned int pa_unSize){
  if(CrcXSocketInterface::getInstance().isInitialized()){
    TCPIP_DATA_UDP_CMD_SEND_REQ_T tData;
    return CrcXSocketInterface::getInstance().sendData(pa_nSockD, pa_pcData, pa_unSize, false, pa_ptDestAddr, &tData);
  }
  else{
    DEVLOG_ERROR("CrcXSocketInterface is not initialized\n");
    return -1;
  }
}

CrcXSocketInterface::TSocketDescriptor CrcXSocketInterface::acceptTCPConnection(
    TSocketDescriptor pa_nListeningSockD){
  //struct sockaddr client_addr;
  TSocketDescriptor nRetVal;
  //TODO: Implement
  return nRetVal;
}

int CrcXSocketInterface::receiveDataFromTCP(TSocketDescriptor pa_nSockD, char* pa_pcData,
    unsigned int pa_unBufSize){
  int nRetVal;
  do{
   // nRetVal = static_cast<int>(recv(pa_nSockD, pa_pcData, pa_unBufSize, 0));
  } while((-1 == nRetVal) /*&& (EINTR == errno)*/); // recv got interrupt / recieving again
  return nRetVal;
}

int CrcXSocketInterface::receiveDataFromUDP(TSocketDescriptor pa_nSockD, char* pa_pcData,
    unsigned int pa_unBufSize){
  int nRetVal;
  do{
    //nRetVal = static_cast<int>(recvfrom(pa_nSockD, pa_pcData, pa_unBufSize, 0, 0, 0));
  } while((-1 == nRetVal) /*&& (EINTR == errno)*/); // recv got interrupt / recieving again
  if(nRetVal == -1){ //
    DEVLOG_ERROR("CrcXSocketInterface: UDP-Socket recvfrom() failed: %s\n", strerror(errno));
  }
  return nRetVal;
}
