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
#include "../../core/devexec.h"
#include "../core/cominfra/commfb.h"
#include <criticalregion.h>
#include <string.h>
#include <TLR_Legacy.h>
#include <forte_config_rcX.h>

DEFINE_SINGLETON(CrcXSocketInterface);

char * const CrcXSocketInterface::scmForteQueueName = "FOR_QUE";
char * const CrcXSocketInterface::scmForteWaitingQueueName = "FOR_WAIT_QUE";
char * const CrcXSocketInterface::scmFortePoolName = "FOR_POOL";

const CrcXSocketInterface::TSocketDescriptor CrcXSocketInterface::scm_nInvalidSocketDescriptor = 0;

CrcXSocketInterface::CrcXSocketInterface(){

  RX_RESULT eRslt;
  m_bInitialized = true;
  mListeningSocketDescriptor = 0;
  m_bConnectionListChanged = false;
  m_unPacketsWaiting = 0;
  /* forte Task identification */
  //TODO: Check if it is really necessary
  eRslt = TLR_TSK_IDENTIFY(0, 0, &mForteResources.forteTask, 0, 0);
  if(eRslt != TLR_S_OK){
    DEVLOG_ERROR("Couldn't identify own task\n");
    m_bInitialized = false;
  }

  /* Create process queue */
  eRslt = TLR_QUE_CREATE(scmForteQueueName, cg_forteQueueMax, &mForteResources.forteQueueHandle);
  if(eRslt != TLR_S_OK){
    DEVLOG_ERROR("Couldn't create queue\n");
    m_bInitialized = false;
  }

  /* Create resource pool for queue packets */
  eRslt = TLR_POOL_CREATE(scmFortePoolName, cg_fortePoolMax, sizeof(FORTE_TCP_PACKET_T), &mForteResources.fortePoolHandle);
  if(eRslt != TLR_S_OK){
    DEVLOG_ERROR("Couldn't create pool\n");
    m_bInitialized = false;
  }

  /* Task TCP_UDP */
  //TODO: Check if it is really necessary
  eRslt = TLR_TSK_IDENTIFY(TCP_TASK_NAME, 0, &mForteResources.tcpTaskHandle, 0, 0);
  if(eRslt != TLR_S_OK){
    DEVLOG_ERROR("Couldn't identify TCP/IP task\n");
    m_bInitialized = false;
  }

  eRslt = TLS_QUE_IDENTIFY(EN_TCPUDP_PROCESS_QUEUE_NAME, 0, &mForteResources.tcpQueueHandle);
  if(eRslt != TLR_S_OK){
    DEVLOG_ERROR("Couldn't identify TCP/IP queue\n");
    m_bInitialized = false;
  }
}

CrcXSocketInterface::~CrcXSocketInterface(){
  this->end();
}

void CrcXSocketInterface::run(void){

  while(isAlive()){
    if (mWaitingList.isEmpty()){
      waitPacket(0, 0, 1001000); //TODO: Check the actual value for the timeout
    }

    CSinglyLinkedList<FORTE_TCP_PACKET_T*>::Iterator itEndWaiting(mWaitingList.end());
    for(CSinglyLinkedList<FORTE_TCP_PACKET_T*>::Iterator itRunnerWaiting = mWaitingList.begin(); itRunnerWaiting != itEndWaiting;){
      m_oSync.lock();
      TConnectionContainer::Iterator itEndConnection(m_lstConnectionsList.end());
      for(TConnectionContainer::Iterator itRunnerConnection = m_lstConnectionsList.begin(); itRunnerConnection != itEndConnection;){
        // need to retrieve the callee as the iterator may get invalid in the recvDat function below in case of connection closing
        forte::com_infra::CComLayer *comLayer = itRunnerConnection->m_poCallee;
        TSocketDescriptor sockDes = itRunnerConnection->m_nSockDes;
        ++itRunnerConnection;

        if((*itRunnerWaiting)->tHead.ulSrcId == sockDes->socketNumber){
          ++itRunnerWaiting;
          if(0 != comLayer){
            if(forte::com_infra::e_Nothing != comLayer->recvData(&sockDes, 0)){
              startNewEventChain(comLayer->getCommFB());
            }
          }
          break;
        }else{
          ++itRunnerWaiting;
        }
      }
      m_oSync.unlock();
    }
  }
}

void CrcXSocketInterface::addComCallback(TSocketDescriptor pa_nFD, forte::com_infra::CComLayer *pa_poComLayer){
  {
    CCriticalRegion criticalRegion(m_oSync);
    TConnContType stNewNode = { pa_nFD, pa_poComLayer };
    m_lstConnectionsList.push_back(stNewNode);
    m_bConnectionListChanged = true;
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
        m_lstConnectionsList.pop_front();
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

void CrcXSocketInterface::createDesciptorList(CSinglyLinkedList<CrcXSocketInterface::TSocketDescriptor>* m_descriptorList){
  m_descriptorList->clearAll();
  TConnectionContainer::Iterator itEnd(m_lstConnectionsList.end());
  for(TConnectionContainer::Iterator itRunner = m_lstConnectionsList.begin(); itRunner != itEnd; ++itRunner){
    m_descriptorList->push_back(itRunner->m_nSockDes);
  }
  m_bConnectionListChanged = false;
}

bool CrcXSocketInterface::isSet(TSocketDescriptor pa_descriptor, CSinglyLinkedList<TSocketDescriptor>* m_descriptorList){
  bool retVal = false;
  CSinglyLinkedList<TSocketDescriptor>::Iterator itEnd(m_descriptorList->end());
  for(CSinglyLinkedList<TSocketDescriptor>::Iterator itRunner = m_descriptorList->begin(); itRunner != itEnd; ++itRunner){
    if (pa_descriptor == *itRunner){
      retVal = true;
      break;
    }
  }
  return retVal;
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
  return CrcXSocketInterface::scm_nInvalidSocketDescriptor;
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
  return CrcXSocketInterface::scm_nInvalidSocketDescriptor;
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
  return CrcXSocketInterface::scm_nInvalidSocketDescriptor;
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
  return CrcXSocketInterface::scm_nInvalidSocketDescriptor;
}

RX_RESULT CrcXSocketInterface::close(TSocketDescriptor pa_nSockD){
  if(scm_nInvalidSocketDescriptor != pa_nSockD){
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
  tDataOpen.ulIpAddr = 0; /* Bind socket to currently configured IP address */
  tDataOpen.ulPort = pa_nPort;
  tDataOpen.ulProtocol = (isTCP) ? TCP_PROTO_TCP : TCP_PROTO_UDP;

  retVal = sendPacketToTCP(0, TCPIP_DATA_TCP_UDP_CMD_OPEN_REQ_SIZE, TCPIP_TCP_UDP_CMD_OPEN_REQ, &tDataOpen, sizeof(TCPIP_DATA_TCP_UDP_CMD_OPEN_REQ_T)); //send packet to open a socket
  if(RX_OK == retVal){
    TCPIP_PACKET_TCP_UDP_CMD_OPEN_CNF_T* cnfPacket;
    retVal = waitPacket(TCPIP_TCP_UDP_CMD_OPEN_CNF, (FORTE_TCP_PACKET_T**) &cnfPacket, RX_INFINITE); //wait for CNF packet which indicates the result of the request
    if(RX_OK == retVal){
      if(RX_OK == cnfPacket->tHead.ulSta){
        if(!isTCP){
          if(!isServer){ //Client UDP
            if(0 == m_ptDestAddr){
              m_ptDestAddr->destPort = cnfPacket->tData.ulPort;
              m_ptDestAddr->destAddress = cnfPacket->tData.ulIpAddr;
            }
            else{
              retVal = RX_MEM_INVALID;
            }
          }
          socketNumber = cnfPacket->tHead.ulDestId;
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
                TLR_QUE_PACKETDONE(mForteResources.fortePoolHandle, mForteResources.forteQueueHandle, &connecCnftPacket);
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
    if(scm_nInvalidSocketDescriptor != pa_destSocket){
      pa_destSocket->socketNumber = socketNumber;
      pa_destSocket->accepted = false;
      pa_destSocket->port = pa_nPort;
      if(isServer && isTCP){
        mListeningSocketDescriptor = pa_destSocket;
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

RX_RESULT CrcXSocketInterface::sendPacketToTCP(UINT32 pa_destId, UINT32 pa_ulLen, UINT32 pa_ulCmd, void* pa_tData, UINT32 pa_dataLength){
  FORTE_TCP_PACKET_T* ptPck;
  RX_RESULT retVal;

  retVal = TLR_POOL_PACKET_GET(mForteResources.fortePoolHandle, &ptPck);
  if(TLR_S_OK == retVal){
    TLS_QUE_LINK_SET_NEW_DESTID(mForteResources.tcpQueueHandle, pa_destId);
    ptPck->tHead.ulSrc = (TLR_UINT32)mForteResources.forteQueueHandle;
    ptPck->tHead.ulSrcId = (TLR_UINT32)this;
    ptPck->tHead.ulLen = pa_ulLen;
    ptPck->tHead.ulId = ++mForteResources.sndId;
    ptPck->tHead.ulSta = 0;
    ptPck->tHead.ulCmd = pa_ulCmd;
    ptPck->tHead.ulExt = 0;
    ptPck->tHead.ulRout = 0;
    TLR_MEMCPY(pa_tData, &((TCPIP_PACKET_IP_CMD_SET_CONFIG_REQ_T*)&ptPck)->tData, pa_dataLength);

    retVal = TLS_QUE_SENDPACKET_FIFO(mForteResources.tcpQueueHandle, ptPck, 100);
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
    retVal = TLR_QUE_WAITFORPACKET(mForteResources.forteQueueHandle, &localPacket, pa_timeout);
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
  } while(RX_INFINITE != pa_timeout);
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
          TLS_QUE_RETURNPACKET(pa_packetResult);
        }
        else{
          mWaitingList.push_back(pa_packetResult);
          m_unPacketsWaiting++;
        }
      }
      case TCPIP_TCP_UDP_CMD_SHUTDOWN_IND: {
        //close all sockets. TODO: The caller will never get notice that the socket was closed. Maybe reopen the socket when the CNF arrives
        TCPIP_DATA_TCP_UDP_CMD_CLOSE_ALL_REQ_T tDataCloseAll;
        tDataCloseAll.ulTimeout = 0; /* Bind socket to currently configured IP address */
        sendPacketToTCP(0, TCPIP_DATA_TCP_UDP_CMD_CLOSE_ALL_REQ_SIZE, TCPIP_TCP_UDP_CMD_CLOSE_ALL_REQ, &tDataCloseAll, sizeof(TCPIP_DATA_TCP_UDP_CMD_CLOSE_ALL_REQ_T));
      }
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

RX_RESULT CrcXSocketInterface::sendData(TSocketDescriptor pa_nSockD, char* pa_pcData, unsigned int pa_unSize, bool pa_isTCP, TUDPDestAddr *pa_ptDestAddr, void* pa_PacketData, int* pa_result){

  RX_RESULT retVal = RX_OK;
  *pa_result = -1;

  if(scm_nInvalidSocketDescriptor != pa_nSockD){
    if(pa_nSockD->accepted){
      //Set not changing variables in the data
      if(pa_isTCP){
        ((TCPIP_DATA_TCP_CMD_SEND_REQ_T*) pa_PacketData)->ulOptions = 0; /* TCP_SEND_OPT_PUSH(0x00000001) Push flag: If set, the stack send the data immediate*/
      }
      else{
        ((TCPIP_DATA_UDP_CMD_SEND_REQ_T*) pa_PacketData)->ulIpAddr = pa_ptDestAddr->destAddress;
        ((TCPIP_DATA_UDP_CMD_SEND_REQ_T*) pa_PacketData)->ulPort = pa_ptDestAddr->destPort;
      }

      while(0 != pa_unSize){
        if(pa_isTCP){
          *pa_result = (pa_unSize > 1460) ? 1460 : pa_unSize;
          TLR_MEMCPY(pa_pcData, &(((TCPIP_DATA_TCP_CMD_SEND_REQ_T*) pa_PacketData)->abData[0]), *pa_result);
          retVal = sendPacketToTCP(pa_nSockD->socketNumber, TCPIP_DATA_TCP_CMD_SEND_REQ_SIZE + *pa_result, TCPIP_TCP_CMD_SEND_REQ, &pa_PacketData, sizeof(UINT32) + *pa_result);
        }
        else{
          *pa_result = (pa_unSize > 1472) ? 1472 : pa_unSize;
          TLR_MEMCPY(pa_pcData, &(((TCPIP_DATA_UDP_CMD_SEND_REQ_T*) pa_PacketData)->abData[0]), *pa_result);
          retVal = sendPacketToTCP(pa_nSockD->socketNumber, TCPIP_DATA_UDP_CMD_SEND_REQ_SIZE + *pa_result, TCPIP_UDP_CMD_SEND_REQ, &pa_PacketData, sizeof(UINT32) * 3 + *pa_result);
        }

        if(RX_OK == retVal){
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
        }
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
    if (scm_nInvalidSocketDescriptor != pa_destSocket){
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
  }
  return returnSocket;
}

void CrcXSocketInterface::socketDescriptorDeAlloc(TSocketDescriptor pa_Socket){
  if (scm_nInvalidSocketDescriptor != pa_Socket){
    delete pa_Socket;
  }
}

RX_RESULT CrcXSocketInterface::receiveData(TSocketDescriptor pa_nSockD, bool , char* pa_pcData, unsigned int pa_unBufSize, int* pa_receivedBytes){
  RX_RESULT retVal = RX_OK;
  *pa_receivedBytes = -1;
  if (scm_nInvalidSocketDescriptor != pa_nSockD){
    CSinglyLinkedList<FORTE_TCP_PACKET_T*>::Iterator prevIterator = 0;
    CSinglyLinkedList<FORTE_TCP_PACKET_T*>::Iterator itEndPacket(mWaitingList.end());
    for(CSinglyLinkedList<FORTE_TCP_PACKET_T*>::Iterator itRunnerPacket = mWaitingList.begin(); itRunnerPacket != itEndPacket; ){
      if((*itRunnerPacket)->tHead.ulSrcId == pa_nSockD->socketNumber){
        bool deletePacket = false;
        if(TCPIP_TCP_UDP_CMD_RECEIVE_STOP_IND == (*itRunnerPacket)->tHead.ulCmd){
          *pa_receivedBytes = 0;
          deletePacket = true;
        }
        else if(TCPIP_TCP_CMD_WAIT_CONNECT_CNF == (*itRunnerPacket)->tHead.ulCmd){
          deletePacket = true;
        }
        else if(TCPIP_TCP_UDP_CMD_RECEIVE_IND == (*itRunnerPacket)->tHead.ulCmd){
          if(pa_unBufSize >= ((*itRunnerPacket)->tHead.ulLen - TCPIP_DATA_TCP_UDP_CMD_RECEIVE_IND_SIZE)){
            TLR_MEMCPY(&(*itRunnerPacket)->tRcvInd.tData.abData[0], pa_pcData, ((*itRunnerPacket)->tHead.ulLen - TCPIP_DATA_TCP_UDP_CMD_RECEIVE_IND_SIZE));
            *pa_receivedBytes = ((*itRunnerPacket)->tHead.ulLen - TCPIP_DATA_TCP_UDP_CMD_RECEIVE_IND_SIZE);
            TLS_QUE_RETURNPACKET(*itRunnerPacket);
            deletePacket = true;

          }
          else{ //move the data not copied to the beginning of the abData field and keep the packet.
            UINT8 buffer[((*itRunnerPacket)->tHead.ulLen - TCPIP_DATA_TCP_UDP_CMD_RECEIVE_IND_SIZE) - pa_unBufSize];
            TLR_MEMCPY(&(*itRunnerPacket)->tRcvInd.tData.abData[0], pa_pcData, pa_unBufSize);
            TLR_MEMCPY(&(*itRunnerPacket)->tRcvInd.tData.abData[pa_unBufSize], &buffer[0], ((*itRunnerPacket)->tHead.ulLen - TCPIP_DATA_TCP_UDP_CMD_RECEIVE_IND_SIZE) - pa_unBufSize);
            TLR_MEMCPY(&buffer[0], &(*itRunnerPacket)->tRcvInd.tData.abData[0], ((*itRunnerPacket)->tHead.ulLen - TCPIP_DATA_TCP_UDP_CMD_RECEIVE_IND_SIZE) - pa_unBufSize);
            (*itRunnerPacket)->tHead.ulLen -= pa_unBufSize;
            *pa_receivedBytes = pa_unBufSize;
          }
        }
        if(deletePacket){
          if(mWaitingList.begin() == itRunnerPacket){
            ++itRunnerPacket;
            mWaitingList.pop_front();
          }
          else{
            itRunnerPacket = mWaitingList.eraseAfter(prevIterator);
          }
          m_unPacketsWaiting--;
        }
        break; //only check one message
      }
      else{
        prevIterator = itRunnerPacket;
        ++itRunnerPacket;
      }
    }
  }else{
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
