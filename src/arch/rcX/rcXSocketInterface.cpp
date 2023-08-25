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
  mInitialized = true;
  mListeningSocketDescriptor = 0;
  mPacketsWaiting = 0;
  /* forte Task identification */
  //TODO: Check if it is really necessary
  eRslt = TLR_TSK_IDENTIFY(nullptr, 0, &mForteResources.forteTask, 0, 0);
  if(eRslt != RX_OK){
    DEVLOG_ERROR("Couldn't identify own task\n");
    mInitialized = false;
  }

  /* Create process queue */
  eRslt = TLR_QUE_CREATE(scmForteQueueName, cg_forteQueueMax, &mForteResources.forteQueueHandle);
  if(eRslt != RX_OK){
    DEVLOG_ERROR("Couldn't create queue\n");
    mInitialized = false;
  }

  /* Create resource pool for queue packets */
  eRslt = TLR_POOL_CREATE(scmFortePoolName, cg_fortePoolMax, sizeof(FORTE_TCP_PACKET_T), &mForteResources.fortePoolHandle);
  if(eRslt != RX_OK){
    DEVLOG_ERROR("Couldn't create pool\n");
    mInitialized = false;
  }

  TLR_QUE_LINK_SOURCE_SET(mForteResources.forteLinkSource, mForteResources.forteQueueHandle, 1);
  mForteResources.sndId = 0;

  /* Task TCP_UDP */
  //TODO: Check if it is really necessary
  eRslt = TLR_TSK_IDENTIFY(TCP_TASK_NAME, 0, &mForteResources.tcpTaskHandle, 0, 0);
  if(eRslt != RX_OK){
    DEVLOG_ERROR("Couldn't identify TCP/IP task\n");
    mInitialized = false;
  }

  eRslt = TLR_QUE_IDENTIFY(EN_TCPUDP_PROCESS_QUEUE_NAME, 0, &mForteResources.tcpQueueLink);
  if(eRslt != RX_OK){
    DEVLOG_ERROR("Couldn't identify TCP/IP queue\n");
    mInitialized = false;
  }
}

CrcXSocketInterface::~CrcXSocketInterface(){
  TLR_QUE_DELETE_CHECK(mForteResources.forteQueueHandle);
  TLR_POOL_DELETE_CHECK(mForteResources.fortePoolHandle);
}

void CrcXSocketInterface::run(){

  UINT32 ticks = 1000000 * 1 / rX_SysGetSystemCycletime(); //1 second
  while(isAlive()){
    if (mWaitingList.isEmpty()){
      waitPacket(0, 0, ticks); //TODO: Check the actual value for the timeout
    }

    while(!mWaitingList.isEmpty()){
      CCriticalRegion region(mSync);
      FORTE_TCP_PACKET_T* firstPacket = *(static_cast<CSinglyLinkedList<FORTE_TCP_PACKET_T*>::Iterator>(mWaitingList.begin()));
      TConnectionContainer::Iterator itEndConnection(mConnectionsList.end());
      bool deletePacket = true;
      for(TConnectionContainer::Iterator itRunnerConnection = mConnectionsList.begin(); itRunnerConnection != itEndConnection;){
        // need to retrieve the callee as the iterator may get invalid in the recvDat function below in case of connection closing
        forte::com_infra::CComLayer *comLayer = itRunnerConnection->mCallee;
        TSocketDescriptor sockDes = itRunnerConnection->mSockDes;
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
        mPacketsWaiting--;
      }
    }
  }
}

void CrcXSocketInterface::addComCallback(TSocketDescriptor paFD, forte::com_infra::CComLayer *paComLayer){
  {
    CCriticalRegion criticalRegion(mSync);
    TConnContType stNewNode = { paFD, paComLayer };
    mConnectionsList.pushBack(stNewNode);
  }
  if(!isAlive()){
    this->start();
  }
}

void CrcXSocketInterface::removeComCallback(TSocketDescriptor paFD){
  CCriticalRegion criticalRegion(mSync);

  TConnectionContainer::Iterator itRunner(mConnectionsList.begin());
  TConnectionContainer::Iterator itRefNode(mConnectionsList.end());
  TConnectionContainer::Iterator itEnd(mConnectionsList.end());

  while(itRunner != itEnd){
    if(itRunner->mSockDes == paFD){
      if(itRefNode ==itEnd){
        mConnectionsList.popFront();
      }
      else{
        mConnectionsList.eraseAfter(itRefNode);
      }
      break;
    }
    itRefNode = itRunner;
    ++itRunner;
  }
}

bool CrcXSocketInterface::isInitialized(){
  return mInitialized;
}

CrcXSocketInterface::TSocketDescriptor CrcXSocketInterface::openTCPServerConnection(char *paIPAddr, unsigned short paPort){
  DEVLOG_INFO("CrcXSocketInterface: Opening TCP-Server connection at: %s:%d\n", paIPAddr, paPort);
  if(CrcXSocketInterface::getInstance().isInitialized()){
    TSocketDescriptor returnSocket = CrcXSocketInterface::getInstance().socketDescriptorAlloc();
    if(0 != returnSocket){
      RX_RESULT retVal = CrcXSocketInterface::getInstance().openConnection(paIPAddr, paPort, true, true, 0, returnSocket);
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

CrcXSocketInterface::TSocketDescriptor CrcXSocketInterface::openTCPClientConnection(char *paIPAddr, unsigned short paPort){
  DEVLOG_INFO("CrcXSocketInterface: Opening TCP-Server connection at: %s:%d\n", paIPAddr, paPort);
  if(CrcXSocketInterface::getInstance().isInitialized()){
    TSocketDescriptor returnSocket = CrcXSocketInterface::getInstance().socketDescriptorAlloc();
    if(0 != returnSocket){
      RX_RESULT retVal = CrcXSocketInterface::getInstance().openConnection(paIPAddr, paPort, true, false, 0, returnSocket);
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

CrcXSocketInterface::TSocketDescriptor CrcXSocketInterface::openUDPSendPort(char *paIPAddr, unsigned short paPort, TUDPDestAddr *mDestAddr){
  DEVLOG_INFO("CrcXSocketInterface: Opening UDP sending connection at: %s:%d\n", paIPAddr, paPort);
  if(CrcXSocketInterface::getInstance().isInitialized()){
    TSocketDescriptor returnSocket = CrcXSocketInterface::getInstance().socketDescriptorAlloc();
    if(0 != returnSocket){
      RX_RESULT retVal = CrcXSocketInterface::getInstance().openConnection(paIPAddr, paPort, false, false, mDestAddr, returnSocket);
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

CrcXSocketInterface::TSocketDescriptor CrcXSocketInterface::openUDPReceivePort(char *paIPAddr, unsigned short paPort){
  DEVLOG_INFO("CrcXSocketInterface: Opening UDP sending connection at: %s:%d\n", paIPAddr, paPort);
  if(CrcXSocketInterface::getInstance().isInitialized()){
    TSocketDescriptor returnSocket = CrcXSocketInterface::getInstance().socketDescriptorAlloc();
    if(0 != returnSocket){
      RX_RESULT retVal = CrcXSocketInterface::getInstance().openConnection(paIPAddr, paPort, false, true, 0, returnSocket);
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

RX_RESULT CrcXSocketInterface::close(TSocketDescriptor paSockD){
  if(scmInvalidSocketDescriptor != paSockD){
    TCPIP_DATA_TCP_UDP_CMD_CLOSE_REQ_T tDataClose;
    tDataClose.ulTimeout = 0; /* Default timeout (13000 milliseconds). Must be zero for UDP */
    return CrcXSocketInterface::getInstance().sendPacketToTCP(paSockD->socketNumber, TCPIP_DATA_TCP_UDP_CMD_CLOSE_REQ_SIZE, TCPIP_TCP_UDP_CMD_CLOSE_REQ, &tDataClose, sizeof(TCPIP_DATA_TCP_UDP_CMD_CLOSE_REQ_T));
  }
  return RX_MEM_INVALID;
}

void CrcXSocketInterface::closeSocket(TSocketDescriptor paSockD){
  if(CrcXSocketInterface::getInstance().isInitialized()){
    RX_RESULT retVal = CrcXSocketInterface::getInstance().close(paSockD);
    if (RX_OK != retVal){
      DEVLOG_ERROR("Error %d when closing socket\n", retVal);
    }
    CrcXSocketInterface::getInstance().socketDescriptorDeAlloc(paSockD);
  }
  else{
    DEVLOG_ERROR("CrcXSocketInterface is not initialized\n");
  }
}

RX_RESULT CrcXSocketInterface::openConnection(char *paIPAddr, unsigned short paPort, bool isTCP, bool isServer, TUDPDestAddr *mDestAddr, TSocketDescriptor& pa_destSocket){
  RX_RESULT retVal = RX_OK;
  UINT32 socketNumber = 0;

  TCPIP_DATA_TCP_UDP_CMD_OPEN_REQ_T tDataOpen;
  tDataOpen.ulIpAddr = 0;
  tDataOpen.ulPort = paPort;
  tDataOpen.ulProtocol = (isTCP) ? TCP_PROTO_TCP : TCP_PROTO_UDP;

  retVal = sendPacketToTCP(0, TCPIP_DATA_TCP_UDP_CMD_OPEN_REQ_SIZE, TCPIP_TCP_UDP_CMD_OPEN_REQ, &tDataOpen, sizeof(TCPIP_DATA_TCP_UDP_CMD_OPEN_REQ_T)); //send packet to open a socket
  if(RX_OK == retVal){
    TCPIP_PACKET_TCP_UDP_CMD_OPEN_CNF_T* cnfPacket;
    retVal = waitPacket(TCPIP_TCP_UDP_CMD_OPEN_CNF, (FORTE_TCP_PACKET_T**) &cnfPacket, RX_INFINITE); //wait for CNF packet which indicates the result of the request
    if(RX_OK == retVal){
      if(RX_OK == cnfPacket->tHead.ulSta){
        if(!isTCP){
          socketNumber = cnfPacket->tHead.ulDestId;
          UINT32 ipAddress = stringIpToInt(paIPAddr);
          if(!isServer){ //Client UDP
            if(0 != mDestAddr){
              mDestAddr->destPort = paPort;
              mDestAddr->destAddress = ipAddress;
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
            tDataConnect.ulIpAddr = CrcXSocketInterface::getInstance().stringIpToInt(paIPAddr); /* IP address of remote server to connect to */
            if(0 != tDataConnect.ulIpAddr){
              tDataConnect.ulPort = paPort; /* Port number of remote server to connect to */
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
      pa_destSocket->port = paPort;
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

TForteUInt32 CrcXSocketInterface::stringIpToInt(char* paString){
  TForteUInt32 result = 0;
  char* currentCharacter;
  int currentNumber = 0;
  int ipNumberCounter = 0;
  currentCharacter = paString;
  if (0 == strcmp("localhost", paString)){
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

RX_RESULT CrcXSocketInterface::sendPacketToTCP(UINT32 pa_destId, UINT32 paLen, UINT32 paCmd, void* paData, UINT32 pa_dataLength){
  FORTE_TCP_PACKET_T* ptPck;
  RX_RESULT retVal;

  retVal = TLR_POOL_PACKET_GET(mForteResources.fortePoolHandle, &ptPck);
  if(RX_OK == retVal){
    TLR_QUE_LINK_SET_NEW_DESTID(mForteResources.tcpQueueLink, pa_destId);
    TLR_QUE_LINK_SET_PACKET_SRC(ptPck, mForteResources.forteLinkSource);

    ptPck->tHead.ulLen = paLen;
    ptPck->tHead.ulId = ++mForteResources.sndId;
    ptPck->tHead.ulSta = 0;
    ptPck->tHead.ulCmd = paCmd;
    ptPck->tHead.ulExt = 0;
    ptPck->tHead.ulRout = 0;
    TLR_MEMCPY(&((TCPIP_PACKET_TCP_UDP_CMD_OPEN_REQ_T*)ptPck)->tData, paData, pa_dataLength);

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
        if(cg_forteWaitingQueueMax <= mPacketsWaiting){
          TLR_QUE_RETURNPACKET(pa_packetResult);
        }
        else{
          mWaitingList.pushBack(pa_packetResult);
          mPacketsWaiting++;
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

RX_RESULT CrcXSocketInterface::sendData(TSocketDescriptor paSockD, char* paData, unsigned int paSize, bool paTCP, TUDPDestAddr *paDestAddr, void* pa_PacketData, int* pa_result){

  RX_RESULT retVal = RX_OK;
  *pa_result = -1;

  if(scmInvalidSocketDescriptor != paSockD){
    if(paSockD->accepted){
      //Set not changing variables in the data
      if(paTCP){
        ((TCPIP_DATA_TCP_CMD_SEND_REQ_T*) pa_PacketData)->ulOptions = TCP_SEND_OPT_PUSH; /* TCP_SEND_OPT_PUSH(0x00000001) Push flag: If set, the stack send the data immediate*/
      }
      else{
        ((TCPIP_DATA_UDP_CMD_SEND_REQ_T*) pa_PacketData)->ulIpAddr = paDestAddr->destAddress;
        ((TCPIP_DATA_UDP_CMD_SEND_REQ_T*) pa_PacketData)->ulPort = paDestAddr->destPort;
      }

      while(0 != paSize){
        if(paTCP){
          *pa_result = (paSize > 1460) ? 1460 : paSize;
          TLR_MEMCPY(&(((TCPIP_DATA_TCP_CMD_SEND_REQ_T*) pa_PacketData)->abData[0]), paData, *pa_result);
          retVal = sendPacketToTCP(paSockD->socketNumber, TCPIP_DATA_TCP_CMD_SEND_REQ_SIZE + *pa_result, TCPIP_TCP_CMD_SEND_REQ, pa_PacketData, sizeof(UINT32) + *pa_result);
        }
        else{
          *pa_result = (paSize > 1472) ? 1472 : paSize;
          TLR_MEMCPY(&(((TCPIP_DATA_UDP_CMD_SEND_REQ_T*) pa_PacketData)->abData[0]), paData, *pa_result);
          retVal = sendPacketToTCP(paSockD->socketNumber, TCPIP_DATA_UDP_CMD_SEND_REQ_SIZE + *pa_result, TCPIP_UDP_CMD_SEND_REQ, pa_PacketData, sizeof(UINT32) * 3 + *pa_result);
        }

        if (RX_OK != retVal){
          break;
        }

        /*if(RX_OK == retVal){
          FORTE_TCP_PACKET_T* cnfPacket;
          retVal = waitPacket((paTCP) ? TCPIP_TCP_CMD_SEND_CNF : TCPIP_UDP_CMD_SEND_CNF, &cnfPacket, RX_INFINITE);
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
        paSize -= *pa_result;
        paData += *pa_result;
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
int CrcXSocketInterface::sendDataOnTCP(TSocketDescriptor paSockD, const char* paData, unsigned int paSize){
  int bytesSent = -1;
  if(CrcXSocketInterface::getInstance().isInitialized()){
    TCPIP_DATA_TCP_CMD_SEND_REQ_T tData;
    CrcXSocketInterface::getInstance().sendData(paSockD, paData, paSize, true, 0, &tData, &bytesSent);
  }
  else{
    DEVLOG_ERROR("CrcXSocketInterface is not initialized\n");
  }
  return bytesSent;
}

int CrcXSocketInterface::sendDataOnUDP(TSocketDescriptor paSockD, TUDPDestAddr *paDestAddr, char* paData, unsigned int paSize){
  int bytesSent = -1;
  if(CrcXSocketInterface::getInstance().isInitialized()){
    TCPIP_DATA_UDP_CMD_SEND_REQ_T tData;
    CrcXSocketInterface::getInstance().sendData(paSockD, paData, paSize, false, paDestAddr, &tData, &bytesSent);
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

CrcXSocketInterface::TSocketDescriptor CrcXSocketInterface::acceptTCPConnection(TSocketDescriptor paListeningSockD){
  if(CrcXSocketInterface::getInstance().isInitialized()){
    TSocketDescriptor returnSocket = CrcXSocketInterface::getInstance().socketDescriptorAlloc();
    if(0 != returnSocket){
      RX_RESULT retVal = CrcXSocketInterface::getInstance().accept(paListeningSockD, returnSocket);
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

CrcXSocketInterface::TSocketDescriptor CrcXSocketInterface::socketDescriptorAlloc(){
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

RX_RESULT CrcXSocketInterface::receiveData(TSocketDescriptor paSockD, bool , char* paData, unsigned int paBufSize, int* pa_receivedBytes){
  RX_RESULT retVal = RX_OK;
  *pa_receivedBytes = -1;
  if(scmInvalidSocketDescriptor != paSockD && 0 != paSockD->packetReceived){
    paSockD->deleteMe = true;
    if(TCPIP_TCP_UDP_CMD_RECEIVE_STOP_IND == paSockD->packetReceived->tHead.ulCmd){
      *pa_receivedBytes = 0;
    }
    else if(TCPIP_TCP_CMD_WAIT_CONNECT_CNF == paSockD->packetReceived->tHead.ulCmd){

    }
    else if(TCPIP_TCP_UDP_CMD_RECEIVE_IND == paSockD->packetReceived->tHead.ulCmd){
      if(paBufSize >= (paSockD->packetReceived->tHead.ulLen - TCPIP_DATA_TCP_UDP_CMD_RECEIVE_IND_SIZE)){
        TLR_MEMCPY(paData, &paSockD->packetReceived->tRcvInd.tData.abData[0], (paSockD->packetReceived->tHead.ulLen - TCPIP_DATA_TCP_UDP_CMD_RECEIVE_IND_SIZE));
        *pa_receivedBytes = (paSockD->packetReceived->tHead.ulLen - TCPIP_DATA_TCP_UDP_CMD_RECEIVE_IND_SIZE);
      }
      else{ //move the data not copied to the beginning of the abData field and keep the packet.
        UINT8 buffer[(paSockD->packetReceived->tHead.ulLen - TCPIP_DATA_TCP_UDP_CMD_RECEIVE_IND_SIZE) - paBufSize];
        TLR_MEMCPY(paData, &paSockD->packetReceived->tRcvInd.tData.abData[0], paBufSize);
        TLR_MEMCPY(&buffer[0], &paSockD->packetReceived->tRcvInd.tData.abData[paBufSize], (paSockD->packetReceived->tHead.ulLen - TCPIP_DATA_TCP_UDP_CMD_RECEIVE_IND_SIZE) - paBufSize);
        TLR_MEMCPY(&paSockD->packetReceived->tRcvInd.tData.abData[0], &buffer[0], (paSockD->packetReceived->tHead.ulLen - TCPIP_DATA_TCP_UDP_CMD_RECEIVE_IND_SIZE) - paBufSize);
        paSockD->packetReceived->tHead.ulLen -= paBufSize;
        *pa_receivedBytes = paBufSize;
        paSockD->deleteMe = false;
      }
    }
  }
  else{
    retVal = RX_MEM_INVALID;
  }
  return retVal;
}

int CrcXSocketInterface::receiveDataFromTCP(TSocketDescriptor paSockD, char* paData, unsigned int paBufSize){
  int receivedBytes = -1;
  if(CrcXSocketInterface::getInstance().isInitialized()){
    CrcXSocketInterface::getInstance().receiveData(paSockD, true, paData, paBufSize, &receivedBytes);
  }
  else{
    DEVLOG_ERROR("CrcXSocketInterface is not initialized\n");
  }
  return receivedBytes;
}

int CrcXSocketInterface::receiveDataFromUDP(TSocketDescriptor paSockD, char* paData, unsigned int paBufSize){
  int receivedBytes = -1;
  if(CrcXSocketInterface::getInstance().isInitialized()){
    CrcXSocketInterface::getInstance().receiveData(paSockD, false, paData, paBufSize, &receivedBytes);
  }
  else{
    DEVLOG_ERROR("CrcXSocketInterface is not initialized\n");
  }
  return receivedBytes;
}
