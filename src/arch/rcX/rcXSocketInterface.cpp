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

#include "rcXSocketInterface.h"
#include <TLR_Includes.h>
#include "bsdsocketinterf.h"
#include "devlog.h"
#include "../../core/devexec.h"
#include "../core/cominfra/commfb.h"
#include <criticalregion.h>
#include <string.h>
#include <forte_config.h>

DEFINE_SINGLETON(CrcXSocketInterface);

char * const CrcXSocketInterface::scmForteQueueName = "FOR_QUE";
char * const CrcXSocketInterface::scmForteWaitingQueueName = "FOR_WAIT_QUE";
char * const CrcXSocketInterface::scmFortePoolName = "FOR_POOL";

const CrcXSocketInterface::TSocketDescriptor CrcXSocketInterface::scmInvalidSocketDescriptor = 0;

CrcXSocketInterface::CrcXSocketInterface(){

  RX_RESULT eRslt;
  m_bInitialized = true;
  mListeningSocketDescriptor = 0;
  m_unPacketsWaiting = 0;
  /* forte Task identification */
  //TODO: Check if it is really necessary
  eRslt = TLR_TSK_IDENTIFY(NULL, 0, &mForteResources.forteTask, 0, 0);
  if(eRslt != RX_OK){
    DEVLOG_ERROR("Couldn't identify own task\n");
    m_bInitialized = false;
  }

  /* Create process queue */
  eRslt = TLR_QUE_CREATE(scmForteQueueName, cg_forteQueueMax, &mForteResources.forteQueueHandle);
  if(eRslt != RX_OK){
    DEVLOG_ERROR("Couldn't create queue\n");
    m_bInitialized = false;
  }

  /* Create resource pool for queue packets */
  eRslt = TLR_POOL_CREATE(scmFortePoolName, cg_fortePoolMax, sizeof(FORTE_TCP_PACKET_T), &mForteResources.fortePoolHandle);
  if(eRslt != RX_OK){
    DEVLOG_ERROR("Couldn't create pool\n");
    m_bInitialized = false;
  }

  TLR_QUE_LINK_SOURCE_SET(mForteResources.forteLinkSource, mForteResources.forteQueueHandle, 1);
  mForteResources.sndId = 0;

  /* Task TCP_UDP */
  //TODO: Check if it is really necessary
  eRslt = TLR_TSK_IDENTIFY(TCP_TASK_NAME, 0, &mForteResources.tcpTaskHandle, 0, 0);
  if(eRslt != RX_OK){
    DEVLOG_ERROR("Couldn't identify TCP/IP task\n");
    m_bInitialized = false;
  }

  eRslt = TLR_QUE_IDENTIFY(EN_TCPUDP_PROCESS_QUEUE_NAME, 0, &mForteResources.tcpQueueLink);
  if(eRslt != RX_OK){
    DEVLOG_ERROR("Couldn't identify TCP/IP queue\n");
    m_bInitialized = false;
  }
}

CrcXSocketInterface::~CrcXSocketInterface(){
  TLR_QUE_DELETE_CHECK(mForteResources.forteQueueHandle);
  TLR_POOL_DELETE_CHECK(mForteResources.fortePoolHandle);
}

void CrcXSocketInterface::run(void){

  UINT32 ticks = 1000000 * 1 / rX_SysGetSystemCycletime(); //1 second
  while(isAlive()){
    if (mWaitingList.isEmpty()){
      waitPacket(0, 0, ticks); //TODO: Check the actual value for the timeout
    }

    while(!mWaitingList.isEmpty()){
      CCriticalRegion region(m_oSync);
      FORTE_TCP_PACKET_T* firstPacket = *(static_cast<CSinglyLinkedList<FORTE_TCP_PACKET_T*>::Iterator>(mWaitingList.begin()));
      TConnectionContainer::Iterator itEndConnection(m_lstConnectionsList.end());
      bool deletePacket = true;
      for(TConnectionContainer::Iterator itRunnerConnection = m_lstConnectionsList.begin(); itRunnerConnection != itEndConnection;){
        // need to retrieve the callee as the iterator may get invalid in the recvDat function below in case of connection closing
        forte::com_infra::CComLayer *comLayer = itRunnerConnection->m_poCallee;
        TSocketDescriptor sockDes = itRunnerConnection->m_nSockDes;
        ++itRunnerConnection;
        if(getSocketIDFromPacket(firstPacket) == sockDes->socketNumber){
          sockDes->packetReceived = firstPacket;
          if(0 != comLayer){
            if(forte::com_infra::e_Nothing != comLayer->recvData(&sockDes, 0)){
              startNewEventChain(comLayer->getCommFB());
            }
          }
          sockDes->packetReceived = 0;
          deletePacket = sockDes->deleteMe;
          break;
        }
      }
      if(deletePacket){
        TLR_QUE_PACKETDONE(mForteResources.fortePoolHandle, mForteResources.forteQueueHandle, firstPacket);
        mWaitingList.popFront();
        m_unPacketsWaiting--;
      }
    }
  }
}

void CrcXSocketInterface::addComCallback(TSocketDescriptor pa_nFD, forte::com_infra::CComLayer *pa_poComLayer){
  {
    CCriticalRegion criticalRegion(m_oSync);
    TConnContType stNewNode = { pa_nFD, pa_poComLayer };
    m_lstConnectionsList.pushBack(stNewNode);
  }
  if(!isAlive()){
    this->start();
  }
}

void CrcXSocketInterface::removeComCallback(TSocketDescriptor pa_nFD){
  CCriticalRegion criticalRegion(m_oSync);

  TConnectionContainer::Iterator itRunner(m_lstConnectionsList.begin());
  TConnectionContainer::Iterator itRefNode(m_lstConnectionsList.end());
  TConnectionContainer::Iterator itEnd(m_lstConnectionsList.end());

  while(itRunner != itEnd){
    if(itRunner->m_nSockDes == pa_nFD){
      if(itRefNode ==itEnd){
        m_lstConnectionsList.popFront();
      }
      else{
        m_lstConnectionsList.eraseAfter(itRefNode);
      }
      break;
    }
    itRefNode = itRunner;
    ++itRunner;
  }
}

bool CrcXSocketInterface::isInitialized(){
  return m_bInitialized;
}

CrcXSocketInterface::TSocketDescriptor CrcXSocketInterface::openTCPServerConnection(char *pa_acIPAddr, unsigned short pa_nPort){
  DEVLOG_INFO("CrcXSocketInterface: Opening TCP-Server connection at: %s:%d\n", pa_acIPAddr, pa_nPort);
  if(CrcXSocketInterface::getInstance().isInitialized()){
    TSocketDescriptor returnSocket = CrcXSocketInterface::getInstance().socketDescriptorAlloc();
    if(0 != returnSocket){
      RX_RESULT retVal = CrcXSocketInterface::getInstance().openConnection(pa_acIPAddr, pa_nPort, true, true, 0, returnSocket);
      if(RX_OK == retVal){
        return returnSocket;
      }
      else{
        DEVLOG_ERROR("Error %d opening socket\n", retVal);
        CrcXSocketInterface::getInstance().socketDescriptorDeAlloc(returnSocket);
      }
    }
    else{
      DEVLOG_ERROR("No enough memory to create a socket\n");
    }
  }
  else{
    DEVLOG_ERROR("CrcXSocketInterface is not initialized\n");
  }
  return CrcXSocketInterface::scmInvalidSocketDescriptor;
}

CrcXSocketInterface::TSocketDescriptor CrcXSocketInterface::openTCPClientConnection(char *pa_acIPAddr, unsigned short pa_nPort){
  DEVLOG_INFO("CrcXSocketInterface: Opening TCP-Server connection at: %s:%d\n", pa_acIPAddr, pa_nPort);
  if(CrcXSocketInterface::getInstance().isInitialized()){
    TSocketDescriptor returnSocket = CrcXSocketInterface::getInstance().socketDescriptorAlloc();
    if(0 != returnSocket){
      RX_RESULT retVal = CrcXSocketInterface::getInstance().openConnection(pa_acIPAddr, pa_nPort, true, false, 0, returnSocket);
      if(RX_OK == retVal){
        return returnSocket;
      }
      else{
        DEVLOG_ERROR("Error %d opening socket\n", retVal);
        CrcXSocketInterface::getInstance().socketDescriptorDeAlloc(returnSocket);
      }
    }
    else{
      DEVLOG_ERROR("No enough memory to create a socket\n");
    }
  }
  else{
    DEVLOG_ERROR("CrcXSocketInterface is not initialized\n");
  }
  return CrcXSocketInterface::scmInvalidSocketDescriptor;
}

CrcXSocketInterface::TSocketDescriptor CrcXSocketInterface::openUDPSendPort(char *pa_acIPAddr, unsigned short pa_nPort, TUDPDestAddr *m_ptDestAddr){
  DEVLOG_INFO("CrcXSocketInterface: Opening UDP sending connection at: %s:%d\n", pa_acIPAddr, pa_nPort);
  if(CrcXSocketInterface::getInstance().isInitialized()){
    TSocketDescriptor returnSocket = CrcXSocketInterface::getInstance().socketDescriptorAlloc();
    if(0 != returnSocket){
      RX_RESULT retVal = CrcXSocketInterface::getInstance().openConnection(pa_acIPAddr, pa_nPort, false, false, m_ptDestAddr, returnSocket);
      if(RX_OK == retVal){
        return returnSocket;
      }
      else{
        DEVLOG_ERROR("Error %d opening socket\n", retVal);
        CrcXSocketInterface::getInstance().socketDescriptorDeAlloc(returnSocket);
      }
    }
    else{
      DEVLOG_ERROR("No enough memory to create a socket\n");
    }
  }
  else{
    DEVLOG_ERROR("CrcXSocketInterface is not initialized\n");
  }
  return CrcXSocketInterface::scmInvalidSocketDescriptor;
}

CrcXSocketInterface::TSocketDescriptor CrcXSocketInterface::openUDPReceivePort(char *pa_acIPAddr, unsigned short pa_nPort){
  DEVLOG_INFO("CrcXSocketInterface: Opening UDP sending connection at: %s:%d\n", pa_acIPAddr, pa_nPort);
  if(CrcXSocketInterface::getInstance().isInitialized()){
    TSocketDescriptor returnSocket = CrcXSocketInterface::getInstance().socketDescriptorAlloc();
    if(0 != returnSocket){
      RX_RESULT retVal = CrcXSocketInterface::getInstance().openConnection(pa_acIPAddr, pa_nPort, false, true, 0, returnSocket);
      if(RX_OK == retVal){
        return returnSocket;
      }
      else{
        DEVLOG_ERROR("Error %d opening socket\n", retVal);
        CrcXSocketInterface::getInstance().socketDescriptorDeAlloc(returnSocket);
      }
    }
    else{
      DEVLOG_ERROR("No enough memory to create a socket\n");
    }
  }
  else{
    DEVLOG_ERROR("CrcXSocketInterface is not initialized\n");
  }
  return CrcXSocketInterface::scmInvalidSocketDescriptor;
}

RX_RESULT CrcXSocketInterface::close(TSocketDescriptor pa_nSockD){
  if(scmInvalidSocketDescriptor != pa_nSockD){
    TCPIP_DATA_TCP_UDP_CMD_CLOSE_REQ_T tDataClose;
    tDataClose.ulTimeout = 0; /* Default timeout (13000 milliseconds). Must be zero for UDP */
    return CrcXSocketInterface::getInstance().sendPacketToTCP(pa_nSockD->socketNumber, TCPIP_DATA_TCP_UDP_CMD_CLOSE_REQ_SIZE, TCPIP_TCP_UDP_CMD_CLOSE_REQ, &tDataClose, sizeof(TCPIP_DATA_TCP_UDP_CMD_CLOSE_REQ_T));
  }
  return RX_MEM_INVALID;
}

void CrcXSocketInterface::closeSocket(TSocketDescriptor pa_nSockD){
  if(CrcXSocketInterface::getInstance().isInitialized()){
    RX_RESULT retVal = CrcXSocketInterface::getInstance().close(pa_nSockD);
    if (RX_OK != retVal){
      DEVLOG_ERROR("Error %d when closing socket\n", retVal);
    }
    CrcXSocketInterface::getInstance().socketDescriptorDeAlloc(pa_nSockD);
  }
  else{
    DEVLOG_ERROR("CrcXSocketInterface is not initialized\n");
  }
}

RX_RESULT CrcXSocketInterface::openConnection(char *pa_acIPAddr, unsigned short pa_nPort, bool isTCP, bool isServer, TUDPDestAddr *m_ptDestAddr, TSocketDescriptor& pa_destSocket){
  RX_RESULT retVal = RX_OK;
  UINT32 socketNumber = 0;

  TCPIP_DATA_TCP_UDP_CMD_OPEN_REQ_T tDataOpen;
  tDataOpen.ulIpAddr = 0;
  tDataOpen.ulPort = pa_nPort;
  tDataOpen.ulProtocol = (isTCP) ? TCP_PROTO_TCP : TCP_PROTO_UDP;

  retVal = sendPacketToTCP(0, TCPIP_DATA_TCP_UDP_CMD_OPEN_REQ_SIZE, TCPIP_TCP_UDP_CMD_OPEN_REQ, &tDataOpen, sizeof(TCPIP_DATA_TCP_UDP_CMD_OPEN_REQ_T)); //send packet to open a socket
  if(RX_OK == retVal){
    TCPIP_PACKET_TCP_UDP_CMD_OPEN_CNF_T* cnfPacket;
    retVal = waitPacket(TCPIP_TCP_UDP_CMD_OPEN_CNF, (FORTE_TCP_PACKET_T**) &cnfPacket, RX_INFINITE); //wait for CNF packet which indicates the result of the request
    if(RX_OK == retVal){
      if(RX_OK == cnfPacket->tHead.ulSta){
        if(!isTCP){
          socketNumber = cnfPacket->tHead.ulDestId;
          UINT32 ipAddress = stringIpToInt(pa_acIPAddr);
          if(!isServer){ //Client UDP
            if(0 != m_ptDestAddr){
              m_ptDestAddr->destPort = pa_nPort;
              m_ptDestAddr->destAddress = ipAddress;
            }
            else{
              retVal = RX_MEM_INVALID;
            }
          }
          if(0xE0000000 <= ipAddress && 0xEFFFFFFF >= ipAddress){
            TCPIP_DATA_TCP_UDP_CMD_SET_SOCK_OPTION_REQ_T setOpt;
            setOpt.ulMode = TCP_SOCK_ADD_MEMBERSHIP;
            setOpt.unParam.ulMulticastGroup = ipAddress;
            retVal = sendPacketToTCP(socketNumber, TCPIP_DATA_TCP_UDP_CMD_SET_SOCK_OPTION_REQ_SIZE, TCPIP_TCP_UDP_CMD_SET_SOCK_OPTION_REQ, &setOpt, sizeof(TCPIP_DATA_TCP_UDP_CMD_SET_SOCK_OPTION_REQ_T)); //send packet to open a socket
            if(RX_OK == retVal){
              TCPIP_PACKET_TCP_UDP_CMD_SET_SOCK_OPTION_CNF_T* setOptCnf;
              retVal = waitPacket(TCPIP_TCP_UDP_CMD_SET_SOCK_OPTION_CNF, (FORTE_TCP_PACKET_T**) &setOptCnf, RX_INFINITE); //wait for CNF packet which indicates the result of the request
              if (RX_OK == retVal){
                retVal = setOptCnf->tHead.ulSta;
              }
              TLR_QUE_PACKETDONE(mForteResources.fortePoolHandle, mForteResources.forteQueueHandle, setOptCnf);
            }
          }

        }
        else { //TCP
          if(isServer){ //TCP server
            //call wait request but don't wait for CNF
            TCPIP_DATA_TCP_CMD_WAIT_CONNECT_REQ_T tDataWait;
            tDataWait.ulTimeoutSend = 0;
            tDataWait.ulTimeoutListen = 0; /*Wait until connection comes in*/
            retVal = sendPacketToTCP(cnfPacket->tHead.ulDestId, TCPIP_DATA_TCP_CMD_WAIT_CONNECT_REQ_SIZE, TCPIP_TCP_CMD_WAIT_CONNECT_REQ, &tDataWait, sizeof(TCPIP_DATA_TCP_CMD_WAIT_CONNECT_REQ_T));
            if(RX_OK == retVal){
              socketNumber = cnfPacket->tHead.ulDestId;
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

              retVal = sendPacketToTCP(cnfPacket->tHead.ulDestId, TCPIP_DATA_TCP_CMD_CONNECT_REQ_SIZE, TCPIP_TCP_CMD_CONNECT_REQ, &tDataConnect, sizeof(TCPIP_DATA_TCP_CMD_CONNECT_REQ_T));
              if(RX_OK == retVal){
                TCPIP_PACKET_TCP_CMD_CONNECT_CNF_T* connecCnftPacket;
                retVal = waitPacket(TCPIP_TCP_CMD_CONNECT_CNF, (FORTE_TCP_PACKET_T**) &connecCnftPacket, RX_INFINITE);
                if(RX_OK == retVal){
                  if (RX_OK == connecCnftPacket->tHead.ulSta){
                    socketNumber = cnfPacket->tHead.ulDestId;
                  }else{
                    //connect packet didn't returned a valid packet
                    retVal = connecCnftPacket->tHead.ulSta;
                  }
                }
                else{
                  //error in waiting (?)
                }
                TLR_QUE_PACKETDONE(mForteResources.fortePoolHandle, mForteResources.forteQueueHandle, connecCnftPacket);
              }
            }
            else{
              retVal = TLR_E_TCP_ERR_IP_ADDR_INVALID_TCP_UDP_CMD_OPEN;
              //wrong IP
            }
          }
        }
      }
      else{
        //returned packet has no OK status
      }
      TLR_QUE_PACKETDONE(mForteResources.fortePoolHandle, mForteResources.forteQueueHandle, cnfPacket);
    }
    else{
      //waitPacket returned null
    }
  }
  else{
    //error sending the packet
  }
  if(RX_OK == retVal){
    if(scmInvalidSocketDescriptor != pa_destSocket){
      pa_destSocket->socketNumber = socketNumber;
      pa_destSocket->accepted = isTCP ? false : true;
      pa_destSocket->port = pa_nPort;
      pa_destSocket->packetReceived = 0;
      pa_destSocket->deleteMe = true;
      if (isTCP && isServer){
        if(0 == mListeningSocketDescriptor){
          mListeningSocketDescriptor = pa_destSocket;
        }
      }
    }else{
      retVal = RX_MEM_INVALID;
    }
  }
  return retVal;
}

TForteUInt32 CrcXSocketInterface::stringIpToInt(char* pa_ipString){
  TForteUInt32 result = 0;
  char* currentCharacter;
  int currentNumber = 0;
  int ipNumberCounter = 0;
  currentCharacter = pa_ipString;
  if (0 == strcmp("localhost", pa_ipString)){
    return 0;
  }

  while (1) {
    if('0' <= *currentCharacter && '9' >= *currentCharacter){
      currentNumber *= 10;
      currentNumber += *currentCharacter - '0';
    }else if('.' == *currentCharacter || '\0' == *currentCharacter ){
      if(256 <= currentNumber){
        return 0;
      }
      ipNumberCounter++;
      result <<= 8;
      result += currentNumber;
      currentNumber = 0;
      if(ipNumberCounter == 4)
      {
        break;
      }else{
        if ('\0' == *currentCharacter){
          return 0;
        }
      }
    }else{
      return 0;
    }
    currentCharacter++;
  }
  return result;
}

RX_RESULT CrcXSocketInterface::sendPacketToTCP(UINT32 pa_destId, UINT32 pa_ulLen, UINT32 pa_ulCmd, void* pa_tData, UINT32 pa_dataLength){
  FORTE_TCP_PACKET_T* ptPck;
  RX_RESULT retVal;

  retVal = TLR_POOL_PACKET_GET(mForteResources.fortePoolHandle, &ptPck);
  if(RX_OK == retVal){
    TLR_QUE_LINK_SET_NEW_DESTID(mForteResources.tcpQueueLink, pa_destId);
    TLR_QUE_LINK_SET_PACKET_SRC(ptPck, mForteResources.forteLinkSource);

    ptPck->tHead.ulLen = pa_ulLen;
    ptPck->tHead.ulId = ++mForteResources.sndId;
    ptPck->tHead.ulSta = 0;
    ptPck->tHead.ulCmd = pa_ulCmd;
    ptPck->tHead.ulExt = 0;
    ptPck->tHead.ulRout = 0;
    TLR_MEMCPY(&((TCPIP_PACKET_TCP_UDP_CMD_OPEN_REQ_T*)ptPck)->tData, pa_tData, pa_dataLength);

    retVal = TLR_QUE_SENDPACKET_FIFO(mForteResources.tcpQueueLink, ptPck, 100); //rX_QueSendPacket((void*) mForteResources.tcpQueueLink.hQue, ptPck, 100);
    if (RX_OK != retVal){
      TLR_POOL_PACKET_RELEASE(mForteResources.fortePoolHandle, ptPck);
    }
  }
  return retVal;
}

RX_RESULT CrcXSocketInterface::waitPacket(UINT32 pa_command, FORTE_TCP_PACKET_T** pa_packetResult, UINT pa_timeout){
  RX_RESULT retVal;
  FORTE_TCP_PACKET_T* localPacket = 0;
  do{
    retVal = TLR_QUE_WAITFORPACKET(mForteResources.forteQueueHandle, &localPacket, pa_timeout); //pa_timeout);rX_QueWaitForPacket((void*)mForteResources.forteQueueHandle, (void**) &localPacket, 100); //
    if(0 != localPacket){
      if(pa_command == localPacket->tHead.ulCmd || 0 == pa_command){
        break;
      }
      else{
        managePacketsDefault(localPacket);
        localPacket = 0;
      }
    }
    else{ //error waiting packet, normally because timeout expired
      break;
    }
  } while(1);
  if (0 != pa_packetResult){
    *pa_packetResult = localPacket;
  }else{
    managePacketsDefault(localPacket);
  }
  return retVal;
}

void CrcXSocketInterface::managePacketsDefault(FORTE_TCP_PACKET_T* pa_packetResult){
  if(0 != pa_packetResult){
    switch (pa_packetResult->tHead.ulCmd){
      case TCPIP_TCP_UDP_CMD_RECEIVE_STOP_IND:
      case TCPIP_TCP_UDP_CMD_RECEIVE_IND:
      case TCPIP_TCP_CMD_WAIT_CONNECT_CNF: {
        if(cg_forteWaitingQueueMax <= m_unPacketsWaiting){
          TLR_QUE_RETURNPACKET(pa_packetResult);
        }
        else{
          mWaitingList.pushBack(pa_packetResult);
          m_unPacketsWaiting++;
        }
      }
      break;
      case TCPIP_TCP_UDP_CMD_SHUTDOWN_IND: {
        //close all sockets. TODO: The caller will never get notice that the socket was closed. Maybe reopen the socket when the CNF arrives
        TCPIP_DATA_TCP_UDP_CMD_CLOSE_ALL_REQ_T tDataCloseAll;
        tDataCloseAll.ulTimeout = 0; /* Bind socket to currently configured IP address */
        sendPacketToTCP(0, TCPIP_DATA_TCP_UDP_CMD_CLOSE_ALL_REQ_SIZE, TCPIP_TCP_UDP_CMD_CLOSE_ALL_REQ, &tDataCloseAll, sizeof(TCPIP_DATA_TCP_UDP_CMD_CLOSE_ALL_REQ_T));
        TLR_QUE_RETURNPACKET(pa_packetResult);//TODO: this shuld be sent back when all the sockets are closed
      }
      break;
        /*In the following cases, the packets shouldn't be there, so they are release or return to the sender*/
      case TCPIP_TCP_CMD_SEND_CNF:
      case TCPIP_UDP_CMD_SEND_CNF:
      case TCPIP_TCP_UDP_CMD_OPEN_CNF:
      case TCPIP_TCP_UDP_CMD_CLOSE_CNF:
      case TCPIP_TCP_CMD_CONNECT_CNF:
      case TCPIP_TCP_UDP_CMD_CLOSE_ALL_CNF:
      default: {
        TLR_QUE_PACKETDONE(mForteResources.fortePoolHandle, mForteResources.forteQueueHandle, pa_packetResult);
      }
    }
  }
}

UINT32 CrcXSocketInterface::getSocketIDFromPacket(FORTE_TCP_PACKET_T* pa_packet){
  if(pa_packet->tHead.ulSrc == (UINT32)mForteResources.forteQueueHandle){
    return pa_packet->tHead.ulDestId;
  }else{
    return pa_packet->tHead.ulSrcId;
  }
}

RX_RESULT CrcXSocketInterface::sendData(TSocketDescriptor pa_nSockD, char* pa_pcData, unsigned int pa_unSize, bool pa_isTCP, TUDPDestAddr *pa_ptDestAddr, void* pa_PacketData, int* pa_result){

  RX_RESULT retVal = RX_OK;
  *pa_result = -1;

  if(scmInvalidSocketDescriptor != pa_nSockD){
    if(pa_nSockD->accepted){
      //Set not changing variables in the data
      if(pa_isTCP){
        ((TCPIP_DATA_TCP_CMD_SEND_REQ_T*) pa_PacketData)->ulOptions = TCP_SEND_OPT_PUSH; /* TCP_SEND_OPT_PUSH(0x00000001) Push flag: If set, the stack send the data immediate*/
      }
      else{
        ((TCPIP_DATA_UDP_CMD_SEND_REQ_T*) pa_PacketData)->ulIpAddr = pa_ptDestAddr->destAddress;
        ((TCPIP_DATA_UDP_CMD_SEND_REQ_T*) pa_PacketData)->ulPort = pa_ptDestAddr->destPort;
      }

      while(0 != pa_unSize){
        if(pa_isTCP){
          *pa_result = (pa_unSize > 1460) ? 1460 : pa_unSize;
          TLR_MEMCPY(&(((TCPIP_DATA_TCP_CMD_SEND_REQ_T*) pa_PacketData)->abData[0]), pa_pcData, *pa_result);
          retVal = sendPacketToTCP(pa_nSockD->socketNumber, TCPIP_DATA_TCP_CMD_SEND_REQ_SIZE + *pa_result, TCPIP_TCP_CMD_SEND_REQ, pa_PacketData, sizeof(UINT32) + *pa_result);
        }
        else{
          *pa_result = (pa_unSize > 1472) ? 1472 : pa_unSize;
          TLR_MEMCPY(&(((TCPIP_DATA_UDP_CMD_SEND_REQ_T*) pa_PacketData)->abData[0]), pa_pcData, *pa_result);
          retVal = sendPacketToTCP(pa_nSockD->socketNumber, TCPIP_DATA_UDP_CMD_SEND_REQ_SIZE + *pa_result, TCPIP_UDP_CMD_SEND_REQ, pa_PacketData, sizeof(UINT32) * 3 + *pa_result);
        }

        if (RX_OK != retVal){
          break;
        }

        /*if(RX_OK == retVal){
          FORTE_TCP_PACKET_T* cnfPacket;
          retVal = waitPacket((pa_isTCP) ? TCPIP_TCP_CMD_SEND_CNF : TCPIP_UDP_CMD_SEND_CNF, &cnfPacket, RX_INFINITE);
          if(RX_OK == retVal){
            if(RX_OK != cnfPacket->tHead.ulSta){
              //error sending packet
              retVal = cnfPacket->tHead.ulSta;
            }
            else{

            }
            TLR_POOL_PACKET_RELEASE(mForteResources.fortePoolHandle, cnfPacket);
          }
          else{
            //packet didn't arrive?
          }
        }
        else{
          //error sending packet
        }
        if(RX_OK != retVal){
          *pa_result = -1;
          DEVLOG_ERROR("TCP-Socket Send failed\n");
          break;
        }*/
        pa_unSize -= *pa_result;
        pa_pcData += *pa_result;
      }
    }
    else{
      retVal = TLR_E_TCP_TASK_F_NOT_INITIALIZED;
      DEVLOG_ERROR("TCP-Socket Send failed because socket was not accepted yet\n");
    }
  }
  else{
    retVal = RX_MEM_INVALID;
  }

  return retVal;
}
//TODO: Check if it's possible to write, using TLR_QUE_GET_LOAD which should be less than 128
int CrcXSocketInterface::sendDataOnTCP(TSocketDescriptor pa_nSockD, char* pa_pcData, unsigned int pa_unSize){
  int bytesSent = -1;
  if(CrcXSocketInterface::getInstance().isInitialized()){
    TCPIP_DATA_TCP_CMD_SEND_REQ_T tData;
    CrcXSocketInterface::getInstance().sendData(pa_nSockD, pa_pcData, pa_unSize, true, 0, &tData, &bytesSent);
  }
  else{
    DEVLOG_ERROR("CrcXSocketInterface is not initialized\n");
  }
  return bytesSent;
}

int CrcXSocketInterface::sendDataOnUDP(TSocketDescriptor pa_nSockD, TUDPDestAddr *pa_ptDestAddr, char* pa_pcData, unsigned int pa_unSize){
  int bytesSent = -1;
  if(CrcXSocketInterface::getInstance().isInitialized()){
    TCPIP_DATA_UDP_CMD_SEND_REQ_T tData;
    CrcXSocketInterface::getInstance().sendData(pa_nSockD, pa_pcData, pa_unSize, false, pa_ptDestAddr, &tData, &bytesSent);
  }
  else{
    DEVLOG_ERROR("CrcXSocketInterface is not initialized\n");
  }
  return bytesSent;
}

RX_RESULT CrcXSocketInterface::accept(TSocketDescriptor pa_listeningSocketDesc, TSocketDescriptor& pa_destSocket){
  RX_RESULT retVal = RX_OK;
  if (mListeningSocketDescriptor == pa_listeningSocketDesc){
    if (scmInvalidSocketDescriptor != pa_destSocket){
      pa_destSocket->socketNumber = pa_listeningSocketDesc->socketNumber;
      pa_destSocket->accepted = true;
      pa_destSocket->port = pa_listeningSocketDesc->port;

      //Open a new listening socket
      retVal = CrcXSocketInterface::getInstance().openConnection(0, static_cast<unsigned short>(pa_listeningSocketDesc->port), true, true, 0, pa_listeningSocketDesc);
      if(RX_OK != retVal){
        DEVLOG_ERROR("Error %d opening socket\n", retVal);
      }
    }else{
      retVal = RX_MEM_INVALID;
    }
  }
  return retVal;
}

CrcXSocketInterface::TSocketDescriptor CrcXSocketInterface::acceptTCPConnection(TSocketDescriptor pa_nListeningSockD){
  if(CrcXSocketInterface::getInstance().isInitialized()){
    TSocketDescriptor returnSocket = CrcXSocketInterface::getInstance().socketDescriptorAlloc();
    if(0 != returnSocket){
      RX_RESULT retVal = CrcXSocketInterface::getInstance().accept(pa_nListeningSockD, returnSocket);
      if(RX_OK == retVal){
        return returnSocket;
      }
      else{
        DEVLOG_ERROR("Error %d opening socket\n", retVal);
        CrcXSocketInterface::getInstance().socketDescriptorDeAlloc(returnSocket);
      }
    }
    else{
      DEVLOG_ERROR("No enough memory to create a socket\n");
    }
  }
  else{
    DEVLOG_ERROR("CrcXSocketInterface is not initialized\n");
  }
  return 0;
}

CrcXSocketInterface::TSocketDescriptor CrcXSocketInterface::socketDescriptorAlloc(void){
  SSocketDescriptor* returnSocket = new SSocketDescriptor;
  if (0 != returnSocket){
    returnSocket->socketNumber = 0;
    returnSocket->accepted = false;
    returnSocket->port = 0;
    returnSocket->packetReceived = 0;
    returnSocket->deleteMe = true;
  }
  return returnSocket;
}

void CrcXSocketInterface::socketDescriptorDeAlloc(TSocketDescriptor pa_Socket){
  if (scmInvalidSocketDescriptor != pa_Socket){
    delete pa_Socket;
  }
}

RX_RESULT CrcXSocketInterface::receiveData(TSocketDescriptor pa_nSockD, bool , char* pa_pcData, unsigned int pa_unBufSize, int* pa_receivedBytes){
  RX_RESULT retVal = RX_OK;
  *pa_receivedBytes = -1;
  if(scmInvalidSocketDescriptor != pa_nSockD && 0 != pa_nSockD->packetReceived){
    pa_nSockD->deleteMe = true;
    if(TCPIP_TCP_UDP_CMD_RECEIVE_STOP_IND == pa_nSockD->packetReceived->tHead.ulCmd){
      *pa_receivedBytes = 0;
    }
    else if(TCPIP_TCP_CMD_WAIT_CONNECT_CNF == pa_nSockD->packetReceived->tHead.ulCmd){

    }
    else if(TCPIP_TCP_UDP_CMD_RECEIVE_IND == pa_nSockD->packetReceived->tHead.ulCmd){
      if(pa_unBufSize >= (pa_nSockD->packetReceived->tHead.ulLen - TCPIP_DATA_TCP_UDP_CMD_RECEIVE_IND_SIZE)){
        TLR_MEMCPY(pa_pcData, &pa_nSockD->packetReceived->tRcvInd.tData.abData[0], (pa_nSockD->packetReceived->tHead.ulLen - TCPIP_DATA_TCP_UDP_CMD_RECEIVE_IND_SIZE));
        *pa_receivedBytes = (pa_nSockD->packetReceived->tHead.ulLen - TCPIP_DATA_TCP_UDP_CMD_RECEIVE_IND_SIZE);
      }
      else{ //move the data not copied to the beginning of the abData field and keep the packet.
        UINT8 buffer[(pa_nSockD->packetReceived->tHead.ulLen - TCPIP_DATA_TCP_UDP_CMD_RECEIVE_IND_SIZE) - pa_unBufSize];
        TLR_MEMCPY(pa_pcData, &pa_nSockD->packetReceived->tRcvInd.tData.abData[0], pa_unBufSize);
        TLR_MEMCPY(&buffer[0], &pa_nSockD->packetReceived->tRcvInd.tData.abData[pa_unBufSize], (pa_nSockD->packetReceived->tHead.ulLen - TCPIP_DATA_TCP_UDP_CMD_RECEIVE_IND_SIZE) - pa_unBufSize);
        TLR_MEMCPY(&pa_nSockD->packetReceived->tRcvInd.tData.abData[0], &buffer[0], (pa_nSockD->packetReceived->tHead.ulLen - TCPIP_DATA_TCP_UDP_CMD_RECEIVE_IND_SIZE) - pa_unBufSize);
        pa_nSockD->packetReceived->tHead.ulLen -= pa_unBufSize;
        *pa_receivedBytes = pa_unBufSize;
        pa_nSockD->deleteMe = false;
      }
    }
  }
  else{
    retVal = RX_MEM_INVALID;
  }
  return retVal;
}

int CrcXSocketInterface::receiveDataFromTCP(TSocketDescriptor pa_nSockD, char* pa_pcData, unsigned int pa_unBufSize){
  int receivedBytes = -1;
  if(CrcXSocketInterface::getInstance().isInitialized()){
    CrcXSocketInterface::getInstance().receiveData(pa_nSockD, true, pa_pcData, pa_unBufSize, &receivedBytes);
  }
  else{
    DEVLOG_ERROR("CrcXSocketInterface is not initialized\n");
  }
  return receivedBytes;
}

int CrcXSocketInterface::receiveDataFromUDP(TSocketDescriptor pa_nSockD, char* pa_pcData, unsigned int pa_unBufSize){
  int receivedBytes = -1;
  if(CrcXSocketInterface::getInstance().isInitialized()){
    CrcXSocketInterface::getInstance().receiveData(pa_nSockD, false, pa_pcData, pa_unBufSize, &receivedBytes);
  }
  else{
    DEVLOG_ERROR("CrcXSocketInterface is not initialized\n");
  }
  return receivedBytes;
}
