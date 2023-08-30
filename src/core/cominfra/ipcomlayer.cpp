/*******************************************************************************
 * Copyright (c) 2010-2014 fortiss, TU Wien ACIN and others.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl - initial implementation, rework communication infrastructure and bug fixes
 *    Ingo Hegny - changed datatype for IP-communication
 *    Martin Melik-Merkumians -  fix typo in forte::com_infra::EComConnectionState, serial communication support for WIN32
 *    Michael Hofmann - fix for fragmented packets
 *    Patrik Smejkal - rename interrupt in interruptCCommFB
 *******************************************************************************/
#include "ipcomlayer.h"
#include "../../arch/devlog.h"
#include "commfb.h"
#include <forte_thread.h>

using namespace forte::com_infra;

CIPComLayer::CIPComLayer(CComLayer* paUpperLayer, CBaseCommFB* paComFB) :
        CComLayer(paUpperLayer, paComFB),
        mSocketID(CIPComSocketHandler::scmInvalidSocketDescriptor),
        mListeningID(CIPComSocketHandler::scmInvalidSocketDescriptor),
        mInterruptResp(e_Nothing),
        mBufFillSize(0){
  memset(mRecvBuffer, 0, sizeof(mRecvBuffer)); //TODO change this to  mRecvBuffer{0} in the extended list when fully switching to C++11
  memset(&mDestAddr, 0, sizeof(mDestAddr));
}

CIPComLayer::~CIPComLayer() = default;

EComResponse CIPComLayer::sendData(void *paData, unsigned int paSize){
  EComResponse eRetVal = e_ProcessDataOk;

  if((nullptr != mFb) && (CIPComSocketHandler::scmInvalidSocketDescriptor != mSocketID)){
    switch (mFb->getComServiceType()){
      case e_Server:
        if(0
            >= CIPComSocketHandler::sendDataOnTCP(mSocketID, static_cast<char*>(paData), paSize)){
          closeSocket(&mSocketID);
          mConnectionState = e_Listening;
          eRetVal = e_InitTerminated;
        }
        break;
      case e_Client:
        if(0
            >= CIPComSocketHandler::sendDataOnTCP(mSocketID, static_cast<char*>(paData), paSize)){
          eRetVal = e_InitTerminated;
        }
        break;
      case e_Publisher:
        if(0
            >= CIPComSocketHandler::sendDataOnUDP(mSocketID, &mDestAddr, static_cast<char*>(paData), paSize)){
          eRetVal = e_InitTerminated;
        }
        break;
      case e_Subscriber:
        //do nothing as subscribers do not send data
        break;
    }
  }
  return eRetVal;
}

EComResponse CIPComLayer::processInterrupt(){
  if(e_ProcessDataOk == mInterruptResp){
    switch (mConnectionState){
      case e_Connected:
        if((0 < mBufFillSize) && (nullptr != mTopLayer)){
          mInterruptResp = mTopLayer->recvData(mRecvBuffer, mBufFillSize);
          mBufFillSize = 0;
        }
        break;
      case e_Disconnected:
        case e_Listening:
        case e_ConnectedAndListening:
        default:
        break;
    }
  }
  return mInterruptResp;
}

EComResponse CIPComLayer::recvData(const void *paData, unsigned int){
  mInterruptResp = e_Nothing;
  switch (mConnectionState){
    case e_Listening:
      //TODO move this to the processInterrupt()
      mSocketID = CIPComSocketHandler::acceptTCPConnection(mListeningID);
      if(CIPComSocketHandler::scmInvalidSocketDescriptor != mSocketID){
        DEVLOG_INFO("Connection established by client\n");
        getExtEvHandler<CIPComSocketHandler>().addComCallback(mSocketID, this);
        mConnectionState = e_Connected;
      }
      break;
    case e_Connected:
      if(mSocketID == *(static_cast<const CIPComSocketHandler::TSocketDescriptor *>(paData))){
        handledConnectedDataRecv();
      }
      else if(mListeningID
          == *(static_cast<const CIPComSocketHandler::TSocketDescriptor *>(paData))){
        handleConnectionAttemptInConnected();
      }
      break;
    case e_ConnectedAndListening:
      case e_Disconnected:
      default:
      break;
  }
  return mInterruptResp;
}

EComResponse CIPComLayer::openConnection(char *paLayerParameter){
  EComResponse eRetVal = e_InitInvalidId;
  char *acPort = strchr(paLayerParameter, ':');
  if(nullptr != acPort){
    *acPort = '\0';
    ++acPort;

    TForteUInt16 nPort = static_cast<TForteUInt16>(forte::core::util::strtoul(acPort, nullptr, 10));

    CIPComSocketHandler::TSocketDescriptor nSockDes =
        CIPComSocketHandler::scmInvalidSocketDescriptor;
    mConnectionState = e_Connected;

    switch (mFb->getComServiceType()){
      case e_Server:
        nSockDes = mListeningID =
            CIPComSocketHandler::openTCPServerConnection(paLayerParameter, nPort);
        mConnectionState = e_Listening;
        break;
      case e_Client:
        nSockDes = mSocketID =
            CIPComSocketHandler::openTCPClientConnection(paLayerParameter, nPort);
        break;
      case e_Publisher:
        nSockDes = mSocketID =
            CIPComSocketHandler::openUDPSendPort(paLayerParameter, nPort, &mDestAddr);
        break;
      case e_Subscriber:
        nSockDes = mSocketID =
            CIPComSocketHandler::openUDPReceivePort(paLayerParameter, nPort);
        break;
    }

    if(CIPComSocketHandler::scmInvalidSocketDescriptor != nSockDes){
      if(e_Publisher != mFb->getComServiceType()){
        //Publishers should not be registered for receiving data
        getExtEvHandler<CIPComSocketHandler>().addComCallback(nSockDes, this);
      }
      eRetVal = e_InitOk;
    }
    else{
      mConnectionState = e_Disconnected;
    }
  }
  return eRetVal;
}

void CIPComLayer::closeConnection(){
  DEVLOG_DEBUG("CSocketBaseLayer::closeConnection() \n");
  closeSocket(&mSocketID);
  closeSocket(&mListeningID);

  mConnectionState = e_Disconnected;
}

void CIPComLayer::closeSocket(CIPComSocketHandler::TSocketDescriptor *paSocketID){
  if(CIPComSocketHandler::scmInvalidSocketDescriptor != *paSocketID){
    getExtEvHandler<CIPComSocketHandler>().removeComCallback(*paSocketID);
    CIPComSocketHandler::closeSocket(*paSocketID);
    *paSocketID = CIPComSocketHandler::scmInvalidSocketDescriptor;
  }
}

void CIPComLayer::handledConnectedDataRecv(){
  // in case of fragmented packets, it can occur that the buffer is full,
  // to avoid calling receiveDataFromTCP with a buffer size of 0 wait until buffer is larger 0
  while((cgIPLayerRecvBufferSize - mBufFillSize) <= 0){
    CThread::sleepThread(0);
  }
  if(CIPComSocketHandler::scmInvalidSocketDescriptor != mSocketID){
    // TODO: sync buffer and bufFillSize
    int nRetVal = 0;
    switch (mFb->getComServiceType()){
      case e_Server:
        case e_Client:
        nRetVal =
            CIPComSocketHandler::receiveDataFromTCP(mSocketID, &mRecvBuffer[mBufFillSize], cgIPLayerRecvBufferSize
                - mBufFillSize);
        break;
      case e_Publisher:
        //do nothing as subscribers cannot receive data
        break;
      case e_Subscriber:
        nRetVal =
            CIPComSocketHandler::receiveDataFromUDP(mSocketID, &mRecvBuffer[mBufFillSize], cgIPLayerRecvBufferSize
                - mBufFillSize);
        break;
    }
    switch (nRetVal){
      case 0:
        DEVLOG_INFO("Connection closed by peer\n");
        mInterruptResp = e_InitTerminated;
        closeSocket (&mSocketID);
        if(e_Server == mFb->getComServiceType()){
          //Move server into listening mode again
          mConnectionState = e_Listening;
        }
        break;
      case -1:
        mInterruptResp = e_ProcessDataRecvFaild;
        break;
      default:
        //we successfully received data
        mBufFillSize += nRetVal;
        mInterruptResp = e_ProcessDataOk;
        break;
    }
    mFb->interruptCommFB(this);
  }
}

void CIPComLayer::handleConnectionAttemptInConnected() const {
  //accept and immediately close the connection to tell the client that we are not available
  //so far the best option I've found for handling single connection servers
  CIPComSocketHandler::TSocketDescriptor socketID = CIPComSocketHandler::acceptTCPConnection(mListeningID);
  if(CIPComSocketHandler::scmInvalidSocketDescriptor != socketID){
    CIPComSocketHandler::closeSocket(socketID);
  }
}
