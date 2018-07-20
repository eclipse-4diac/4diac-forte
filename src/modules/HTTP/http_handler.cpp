/*******************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "http_handler.h"

#include "../../core/devexec.h"
#include "../../core/iec61131_functions.h"
#include "../../core/cominfra/basecommfb.h"
#include <criticalregion.h>
#include "httpparser.h"
#include <forte_printer.h>
#include "comlayer.h"

using namespace forte::com_infra;

CIPComSocketHandler::TSocketDescriptor CHTTP_Handler::smServerListeningSocket = CIPComSocketHandler::scmInvalidSocketDescriptor;

char CHTTP_Handler::sRecvBuffer[];
unsigned int  CHTTP_Handler::sBufFillSize = 0;
const unsigned int CHTTP_Handler::scmSendTimeout = 20;
const unsigned int CHTTP_Handler::scmAcceptedTimeout = 5;

DEFINE_HANDLER(CHTTP_Handler);

CHTTP_Handler::CHTTP_Handler(CDeviceExecution& pa_poDeviceExecution) : CExternalEventHandler(pa_poDeviceExecution){
  memset(sRecvBuffer, 0, cg_unIPLayerRecvBufferSize);
}

CHTTP_Handler::~CHTTP_Handler(){
  disableHandler();
}

void CHTTP_Handler::enableHandler(void){
  startTimeoutThread();
}

void CHTTP_Handler::disableHandler(void) {

  setAlive(false);
  resumeSelfsuspend();
  end();

  closeHTTPServer();
  {
    CCriticalRegion criticalRegion(mServerMutex);
    for(CSinglyLinkedList<HTTPServerWaiting *>::Iterator iter = mServerLayers.begin(); iter != mServerLayers.end(); ++iter){
      for(CSinglyLinkedList<CIPComSocketHandler::TSocketDescriptor*>::Iterator iter1 = (*iter)->mSockets.begin(); iter1 != (*iter)->mSockets.end(); ++iter1){
        closeSocket(*(*iter1));
      }
      delete (*iter);
    }
    mServerLayers.clearAll();
  }

  {
    CCriticalRegion criticalRegion(mClientMutex);
    for(CSinglyLinkedList<HTTPClientWaiting *>::Iterator iter = mClientLayers.begin(); iter != mClientLayers.end(); ++iter){
      closeSocket((*iter)->mSocket);
      delete (*iter);
    }
    mClientLayers.clearAll();
  }

  {
    CCriticalRegion criticalRegion(mAcceptedMutex);
    for(CSinglyLinkedList<HTTPAcceptedSockets *>::Iterator iter = mAcceptedSockets.begin(); iter != mAcceptedSockets.end(); ++iter){
      closeSocket((*iter)->mSocket);
      delete (*iter);
    }
    mAcceptedSockets.clearAll();
  }
}

void CHTTP_Handler::setPriority(int) {
  //currently we are doing nothing here.
}

int CHTTP_Handler::getPriority(void) const {
  //the same as for setPriority
  return 0;
}

forte::com_infra::EComResponse CHTTP_Handler::recvData(const void* paData, unsigned int){ //TODO: do something with the size parameter of the received data?
  CIPComSocketHandler::TSocketDescriptor socket = *(static_cast<const CIPComSocketHandler::TSocketDescriptor*>(paData));

  if(socket == smServerListeningSocket){
    CIPComSocketHandler::TSocketDescriptor newConnection = CIPComSocketHandler::acceptTCPConnection(socket);
    if(CIPComSocketHandler::scmInvalidSocketDescriptor != newConnection){
      CCriticalRegion criticalRegion(mAcceptedMutex);
      HTTPAcceptedSockets* accepted = new HTTPAcceptedSockets();
      accepted->mSocket = newConnection;
      accepted->mStartTime.setCurrentTime();
      mAcceptedSockets.push_back(accepted);
      GET_HANDLER_FROM_HANDLER(CIPComSocketHandler)->addComCallback(newConnection, this);
    }else{
      DEVLOG_INFO("[HTTP Handler] Couldn't accept new HTTP connection\n");
    }
  }else{
    { //remove socket from accepted
      CCriticalRegion criticalRegion(mAcceptedMutex);
      HTTPAcceptedSockets *toDelete = 0;
      for(CSinglyLinkedList<HTTPAcceptedSockets *>::Iterator iter = mAcceptedSockets.begin(); iter != mAcceptedSockets.end(); ++iter){
        if((*iter)->mSocket == socket){
          toDelete = *iter;
          break;
        }
      }
      if(0 != toDelete){
        mAcceptedSockets.erase(toDelete);
      }
    }

    int recv = CIPComSocketHandler::receiveDataFromTCP(socket, &sRecvBuffer[sBufFillSize], cg_unIPLayerRecvBufferSize - sBufFillSize);
    if(0 == recv){
      closeSocket(socket);
    }else if(-1 == recv){
      DEVLOG_ERROR("[HTTP handler] Error receiving packet\n");
      closeSocket(socket);
    }else{
      bool found = false;
      { //check clients
        CCriticalRegion criticalRegion(mClientMutex);
        if(!mClientLayers.isEmpty()){
          HTTPClientWaiting * toDelete = 0;
          for(CSinglyLinkedList<HTTPClientWaiting *>::Iterator iter = mClientLayers.begin(); iter != mClientLayers.end(); ++iter){
            if((*iter)->mSocket == socket){
              if(e_ProcessDataOk == (*iter)->mLayer->recvData(sRecvBuffer, recv)){
                startNewEventChain((*iter)->mLayer->getCommFB());
              }
              closeSocket(socket);
              found = true;
              toDelete = *iter;
              break;
            }
          }

          if(0 != toDelete){
            delete toDelete;
            mClientLayers.erase(toDelete);
          }
        }
      }

      if(!found){ //check paths
        CCriticalRegion criticalRegion(mServerMutex);
        if(!mServerLayers.isEmpty()){
          CIEC_STRING path;
          CSinglyLinkedList<CIEC_STRING> parameterNames;
          CSinglyLinkedList<CIEC_STRING> parameterValues;
          if(CHttpParser::parseGetRequest(path, parameterNames, parameterValues, sRecvBuffer)){
            for(CSinglyLinkedList<HTTPServerWaiting *>::Iterator iter = mServerLayers.begin(); iter != mServerLayers.end(); ++iter){
              if((*iter)->mPath == path){
                (*iter)->mSockets.push_back(&socket);
                if(e_ProcessDataOk == (*iter)->mLayer->recvServerData(parameterNames, parameterValues)){
                  startNewEventChain((*iter)->mLayer->getCommFB());
                }
                found = true;
                break;
              }
            }
          }else{
            DEVLOG_ERROR("[HTTP Handler] Wrong HTTP Get request\n");
          }
          if(!found){
            DEVLOG_ERROR("[HTTP Handler] Path %s has no FB registered\n", path.getValue());
          }
        }
      }
      if(!found){
        DEVLOG_WARNING("[HTTP Handler]: A packet arrived to the wrong place\n");
      }
    }
  }

  return e_Nothing;
}

bool CHTTP_Handler::sendClientData(forte::com_infra::CHttpComLayer* paLayer, CIEC_STRING& paToSend){
  CIPComSocketHandler::TSocketDescriptor newSocket = CIPComSocketHandler::openTCPClientConnection(paLayer->getHost().getValue(), paLayer->getPort());
  if(CIPComSocketHandler::scmInvalidSocketDescriptor != newSocket){
    if(paToSend.length() == CIPComSocketHandler::sendDataOnTCP(newSocket, paToSend.getValue(), paToSend.length())){
      CCriticalRegion criticalRegion(mClientMutex);
      HTTPClientWaiting* toAdd = new HTTPClientWaiting();
      toAdd->mLayer = paLayer;
      toAdd->mSocket = newSocket;
      toAdd->mStartTime.setCurrentTime();
      startTimeoutThread();
      mClientLayers.push_back(toAdd);
      GET_HANDLER_FROM_HANDLER(CIPComSocketHandler)->addComCallback(newSocket, this);
      resumeSelfsuspend();
      return true;
    }else{
      DEVLOG_ERROR("[HTTP Handler]: Couldn't send data to client %s:%u\n", paLayer->getHost().getValue(), paLayer->getPort());
      closeSocket(newSocket);
    }
  }else{
    DEVLOG_ERROR("[HTTP Handler]: Couldn't open client connection for %s:%u\n", paLayer->getHost().getValue(), paLayer->getPort());
  }
  return false;
}

void CHTTP_Handler::addServerPath(forte::com_infra::CHttpComLayer* paLayer, CIEC_STRING& paPath){
  openHTTPServer();
  CCriticalRegion criticalRegion(mServerMutex);
  HTTPServerWaiting* toAdd = new HTTPServerWaiting();
  toAdd->mLayer = paLayer;
  toAdd->mPath = paPath;
  mServerLayers.push_back(toAdd);
  DEVLOG_INFO("[HTTP Handler]: The listening  path \"%s\" was added to the http server\n", paPath.getValue());
}

void CHTTP_Handler::removeServerPath(CIEC_STRING& paPath){
  CCriticalRegion criticalRegion(mServerMutex);
  HTTPServerWaiting * toDelete = 0;
  for(CSinglyLinkedList<HTTPServerWaiting *>::Iterator iter = mServerLayers.begin(); iter != mServerLayers.end(); ++iter){
    if((*iter)->mPath == paPath){
      for(CSinglyLinkedList<CIPComSocketHandler::TSocketDescriptor *>::Iterator iter_ = (*iter)->mSockets.begin(); iter_ != (*iter)->mSockets.end(); ++iter_){
        if(0 != (*iter_)){
          closeSocket(*(*iter_));
        }
      }
      delete (*iter);
      toDelete = *iter;
      break;
    }
  }

  if(0 != toDelete){
    mServerLayers.erase(toDelete);
  }

  if(mServerLayers.isEmpty()){
    closeHTTPServer();
  }
}

void CHTTP_Handler::sendServerAnswer(forte::com_infra::CHttpComLayer* paLayer, CIEC_STRING& paAnswer){
  sendServerAnswerHelper(paLayer, paAnswer, false);
}

void CHTTP_Handler::sendServerAnswerFromRecv(forte::com_infra::CHttpComLayer* paLayer, CIEC_STRING& paAnswer){
  sendServerAnswerHelper(paLayer, paAnswer, true);
}

void CHTTP_Handler::forceClose(forte::com_infra::CHttpComLayer* paLayer){
  forceCloseHelper(paLayer, false);
}

void CHTTP_Handler::forceCloseFromRecv(forte::com_infra::CHttpComLayer* paLayer){
  forceCloseHelper(paLayer, true);
}

void CHTTP_Handler::run() {
  DEVLOG_INFO("[HTTP Handler]: Starting HTTP timeout thread\n");

  mThreadStarted.inc();
  while(isAlive()){
    if(mClientLayers.isEmpty() && mAcceptedSockets.isEmpty()){
      selfSuspend();
    }
    if(!isAlive()){
      break;
    }

    {
      CCriticalRegion criticalRegion(mClientMutex);
      if(!mClientLayers.isEmpty()){
        CSinglyLinkedList<HTTPClientWaiting *> clientsToDelete;
        for(CSinglyLinkedList<HTTPClientWaiting *>::Iterator iter = mClientLayers.begin(); iter != mClientLayers.end(); ++iter){
          // wait until result is ready
          CIEC_DATE_AND_TIME currentTime;
          currentTime.setCurrentTime();
          if(currentTime.getMilliSeconds() - (*iter)->mStartTime.getMilliSeconds() > scmSendTimeout * 1000){
            DEVLOG_ERROR("[HTTP Handler]: Timeout at client %s:%u \n", (*iter)->mLayer->getHost().getValue(), (*iter)->mLayer->getPort());
            closeSocket((*iter)->mSocket);
            clientsToDelete.push_back(*iter);
            (*iter)->mLayer->recvData(0, 0); //indicates timeout
          }
        }
        for(CSinglyLinkedList<HTTPClientWaiting *>::Iterator iter = clientsToDelete.begin(); iter != clientsToDelete.end(); ++iter){
          for(CSinglyLinkedList<HTTPClientWaiting *>::Iterator iter_ = mClientLayers.begin(); iter_ != mClientLayers.end(); ++iter_){
            if(*iter_ == *iter){
              mClientLayers.erase(*iter_);
              break;
            }
          }
        }
      }
    }

    {
      CCriticalRegion criticalRegion(mAcceptedMutex);
      if(!mAcceptedSockets.isEmpty()){
        CSinglyLinkedList<HTTPAcceptedSockets *> acceptedToDelete;
        for(CSinglyLinkedList<HTTPAcceptedSockets *>::Iterator iter = mAcceptedSockets.begin(); iter != mAcceptedSockets.end(); ++iter){
          // wait until result is ready
          CIEC_DATE_AND_TIME currentTime;
          currentTime.setCurrentTime();
          if(currentTime.getMilliSeconds() - (*iter)->mStartTime.getMilliSeconds() > scmAcceptedTimeout * 1000){
            DEVLOG_ERROR("[HTTP Handler]: Timeout at accepted socket\n");
            closeSocket((*iter)->mSocket);
            acceptedToDelete.push_back(*iter);
          }
        }

        for(CSinglyLinkedList<HTTPAcceptedSockets *>::Iterator iter = acceptedToDelete.begin(); iter != acceptedToDelete.end(); ++iter){
          for(CSinglyLinkedList<HTTPAcceptedSockets *>::Iterator iter_ = mAcceptedSockets.begin(); iter_ != mAcceptedSockets.end(); ++iter_){
            if(*iter_ == *iter){
              mAcceptedSockets.erase(*iter_);
              break;
            }
          }
        }
      }
    }

    sleepThread(100);

  }
}

void CHTTP_Handler::startTimeoutThread(){
  if(!isAlive()){
    start();
    mThreadStarted.waitIndefinitely();
    mThreadStarted.inc();
  }
}

void CHTTP_Handler::openHTTPServer(){
  if(CIPComSocketHandler::scmInvalidSocketDescriptor == smServerListeningSocket){
    char address[] = "127.0.0.1";
    smServerListeningSocket = CIPComSocketHandler::openTCPServerConnection(address, 80);//TODO: make port configurable
    if(CIPComSocketHandler::scmInvalidSocketDescriptor != smServerListeningSocket){
      GET_HANDLER_FROM_HANDLER(CIPComSocketHandler)->addComCallback(smServerListeningSocket, this);
      DEVLOG_INFO("[HTTP Handler] HTTP server listening on port 80\n");//TODO: make port configurable
    }
    else{
      DEVLOG_ERROR("[HTTP Handler] Couldn't start HTTP server\n");
    }
  }
}

void CHTTP_Handler::closeHTTPServer(){
  if(CIPComSocketHandler::scmInvalidSocketDescriptor != smServerListeningSocket){
    CIPComSocketHandler::closeSocket(smServerListeningSocket);
    smServerListeningSocket = CIPComSocketHandler::scmInvalidSocketDescriptor;
  }
}

void CHTTP_Handler::closeSocket(CIPComSocketHandler::TSocketDescriptor paSocket){
  GET_HANDLER_FROM_HANDLER(CIPComSocketHandler)->removeComCallback(paSocket);
  GET_HANDLER_FROM_HANDLER(CIPComSocketHandler)->closeSocket(paSocket);
}

void CHTTP_Handler::resumeSelfsuspend(){
  mSuspendSemaphore.inc();
}

void CHTTP_Handler::selfSuspend(){
  mSuspendSemaphore.waitIndefinitely();
}

void CHTTP_Handler::sendServerAnswerHelper(forte::com_infra::CHttpComLayer* paLayer, CIEC_STRING& paAnswer, bool paFromRecv){
  if(!paFromRecv){
    mServerMutex.lock();
  }

  if(!mServerLayers.isEmpty()){
    for(CSinglyLinkedList<HTTPServerWaiting *>::Iterator iter = mServerLayers.begin(); iter != mServerLayers.end(); ++iter){
      if((*iter)->mLayer == paLayer){
        CIPComSocketHandler::TSocketDescriptor socket = *(*iter)->mSockets.peek_front();
        if(paAnswer.length() != CIPComSocketHandler::sendDataOnTCP(socket, paAnswer.getValue(), paAnswer.length())){
          DEVLOG_ERROR("[HTTP Handler]: Error sending back the answer %s \n", paAnswer.getValue());
        }
        closeSocket(socket);
        (*iter)->mSockets.pop_front();
        break;
      }
    }
  }

  if(!paFromRecv){
    mServerMutex.unlock();
  }
}

void CHTTP_Handler::forceCloseHelper(forte::com_infra::CHttpComLayer* paLayer, bool paFromRecv){
  if(!paFromRecv){
    mServerMutex.lock();
  }

  bool found = false;

  if(!mServerLayers.isEmpty()){
    for(CSinglyLinkedList<HTTPServerWaiting *>::Iterator iter = mServerLayers.begin(); iter != mServerLayers.end(); ++iter){
      if((*iter)->mLayer == paLayer){
        if(!(*iter)->mSockets.isEmpty()){
          CSinglyLinkedList<CIPComSocketHandler::TSocketDescriptor*>::Iterator itSocket = (*iter)->mSockets.begin();
          closeSocket(**itSocket);
          (*iter)->mSockets.pop_front();
        }
        found = true;
        break;
      }
    }
  }

  if(!paFromRecv){
    mServerMutex.unlock();
    mClientMutex.lock();
  }

  if(!found && !mClientLayers.isEmpty()){
     for(CSinglyLinkedList<HTTPClientWaiting *>::Iterator iter = mClientLayers.begin(); iter != mClientLayers.end(); ++iter){
       if((*iter)->mLayer == paLayer){
         closeSocket((*iter)->mSocket);
       }
     }
   }

  if(!paFromRecv){
    mClientMutex.unlock();
   }
}
