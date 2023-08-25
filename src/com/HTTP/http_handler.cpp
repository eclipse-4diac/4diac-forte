/*******************************************************************************
 * Copyright (c) 2018, 2023 fortiss GmbH
 *               HIT robot group
 *               Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial API and implementation and/or initial documentation
 *    Tibalt Zhao - remove the client http layer when socket error in recv
 *    Martin Melik Merkumians - Change CIEC_STRING to std::string
 *******************************************************************************/

#include "http_handler.h"

#include "../../core/devexec.h"
#include "../../core/iec61131_functions.h"
#include "../../core/cominfra/basecommfb.h"
#include <criticalregion.h>
#include "httpparser.h"
#include <forte_printer.h>
#include "comlayer.h"
#include <forte_config.h>
#include <string>

using namespace forte::com_infra;
using namespace std::string_literals;

TForteUInt16 gHTTPServerPort = FORTE_COM_HTTP_LISTENING_PORT;

CIPComSocketHandler::TSocketDescriptor CHTTP_Handler::smServerListeningSocket = CIPComSocketHandler::scmInvalidSocketDescriptor;

char CHTTP_Handler::sRecvBuffer[];
unsigned int CHTTP_Handler::sBufFillSize = 0;
const unsigned int CHTTP_Handler::scmSendTimeout = 20;
const unsigned int CHTTP_Handler::scmAcceptedTimeout = 5;

DEFINE_HANDLER(CHTTP_Handler);

CHTTP_Handler::CHTTP_Handler(CDeviceExecution &paDeviceExecution) :
    CExternalEventHandler(paDeviceExecution) {
  memset(sRecvBuffer, 0, cgIPLayerRecvBufferSize);
}

CHTTP_Handler::~CHTTP_Handler() {
  stopTimeoutThread();
  closeHTTPServer();
  clearServerLayers();
  clearClientLayers();
  clearAcceptedSockets();
}

void CHTTP_Handler::enableHandler() {
  startTimeoutThread();
}

void CHTTP_Handler::disableHandler() {
  stopTimeoutThread();
}

void CHTTP_Handler::clearServerLayers() {
  CCriticalRegion criticalRegion(mServerMutex);
  for(CSinglyLinkedList<HTTPServerWaiting*>::Iterator iter = mServerLayers.begin(); iter != mServerLayers.end(); ++iter) {
    for(CSinglyLinkedList<CIPComSocketHandler::TSocketDescriptor>::Iterator iter1 = (*iter)->mSockets.begin(); iter1 != (*iter)->mSockets.end(); ++iter1) {
      removeAndCloseSocket(*iter1);
    }
    (*iter)->mSockets.clearAll();
    delete (*iter);
  }
  mServerLayers.clearAll();
}

void CHTTP_Handler::clearClientLayers() {
  CCriticalRegion criticalRegion(mClientMutex);
  for(CSinglyLinkedList<HTTPClientWaiting*>::Iterator iter = mClientLayers.begin(); iter != mClientLayers.end(); ++iter) {
    removeAndCloseSocket((*iter)->mSocket);
    delete (*iter);
  }
  mClientLayers.clearAll();
}

void CHTTP_Handler::clearAcceptedSockets() {
  CCriticalRegion criticalRegion(mAcceptedMutex);
  for(CSinglyLinkedList<HTTPAcceptedSockets*>::Iterator iter = mAcceptedSockets.begin(); iter != mAcceptedSockets.end(); ++iter) {
    removeAndCloseSocket((*iter)->mSocket);
    delete (*iter);
  }
  mAcceptedSockets.clearAll();
}

void CHTTP_Handler::setPriority(int) {
  //currently we are doing nothing here.
}

int CHTTP_Handler::getPriority() const {
  //the same as for setPriority
  return 0;
}

forte::com_infra::EComResponse CHTTP_Handler::recvData(const void *paData, unsigned int) { //TODO: do something with the size parameter of the received data?
  CIPComSocketHandler::TSocketDescriptor socket = *(static_cast<const CIPComSocketHandler::TSocketDescriptor*>(paData));

  if(socket == smServerListeningSocket) {
    CIPComSocketHandler::TSocketDescriptor newConnection = CIPComSocketHandler::acceptTCPConnection(socket);
    if(CIPComSocketHandler::scmInvalidSocketDescriptor != newConnection) {
      CCriticalRegion criticalRegion(mAcceptedMutex);
      HTTPAcceptedSockets *accepted = new HTTPAcceptedSockets();
      accepted->mSocket = newConnection;
      accepted->mStartTime = func_NOW();
      mAcceptedSockets.pushBack(accepted);
      getExtEvHandler<CIPComSocketHandler>().addComCallback(newConnection, this);
      resumeSelfsuspend();
    } else {
      DEVLOG_ERROR("[HTTP Handler] Couldn't accept new HTTP connection\n");
    }
  } else {
    int recvLen = CIPComSocketHandler::receiveDataFromTCP(socket, &sRecvBuffer[sBufFillSize], cgIPLayerRecvBufferSize - sBufFillSize);
    if(0 == recvLen) {
      removeAndCloseSocket(socket);
      removeHTTPLayerFromClientList(socket);
    } else if(-1 == recvLen) {
      removeAndCloseSocket(socket);
      removeHTTPLayerFromClientList(socket);
      DEVLOG_ERROR("[HTTP handler] Error receiving packet\n");
    } else {
      if(!recvClients(socket, recvLen) && !recvServers(socket)) {
        DEVLOG_WARNING("[HTTP Handler]: A packet arrived to the wrong place\n");
      }
    }
  }

  return e_Nothing;
}

bool CHTTP_Handler::recvClients(const CIPComSocketHandler::TSocketDescriptor paSocket, const int paRecvLength) { //check clients
  CCriticalRegion criticalRegion(mClientMutex);
  for(CSinglyLinkedList<HTTPClientWaiting*>::Iterator iter = mClientLayers.begin(); iter != mClientLayers.end(); ++iter) {
    if((*iter)->mSocket == paSocket) {
      if(e_ProcessDataOk == (*iter)->mLayer->recvData(sRecvBuffer, static_cast<unsigned int>(paRecvLength))) {
        startNewEventChain((*iter)->mLayer->getCommFB());
      }
      removeAndCloseSocket(paSocket);
      HTTPClientWaiting *toDelete = *iter;
      mClientLayers.erase(toDelete);
      delete toDelete;
      return true;
    }
  }

  return false;
}

bool CHTTP_Handler::removeHTTPLayerFromClientList(const CIPComSocketHandler::TSocketDescriptor paSocket) {
  CCriticalRegion criticalRegion(mClientMutex);
  for(CSinglyLinkedList<HTTPClientWaiting*>::Iterator iter = mClientLayers.begin(); iter != mClientLayers.end(); ++iter) {
    if((*iter)->mSocket == paSocket) {
      //removeAndCloseSocket(paSocket);
      HTTPClientWaiting *toDelete = *iter;
      mClientLayers.erase(toDelete);
      delete toDelete;
      return true;
    }
  }
  DEVLOG_ERROR("[HTTP Handler] Couldn't find layer to remove\n");
  return false;
}

bool CHTTP_Handler::recvServers(const CIPComSocketHandler::TSocketDescriptor paSocket) {
  CCriticalRegion criticalRegion(mServerMutex);
  removeSocketFromAccepted(paSocket);

  bool found = false;
  if(!mServerLayers.isEmpty()) {
    std::string path;
    CSinglyLinkedList<std::string> parameterNames;
    CSinglyLinkedList<std::string> parameterValues;
    bool noParsingError = false;
    switch(CHttpParser::getTypeOfRequest(sRecvBuffer)){
      case CHttpComLayer::e_GET:
        noParsingError = CHttpParser::parseGetRequest(path, parameterNames, parameterValues, sRecvBuffer);
        break;
      case CHttpComLayer::e_POST:
      case CHttpComLayer::e_PUT: {
        std::string content;
        noParsingError = CHttpParser::parsePutPostRequest(path, content, sRecvBuffer);
        parameterValues.pushBack(content);
        break;
      }
      default:
        break;
    }

    if(noParsingError) {
      for(CSinglyLinkedList<HTTPServerWaiting*>::Iterator iter = mServerLayers.begin(); iter != mServerLayers.end(); ++iter) {
        if((*iter)->mPath == path) {
          (*iter)->mSockets.pushBack(paSocket);
          if(e_ProcessDataOk == (*iter)->mLayer->recvServerData(parameterNames, parameterValues)) {
            startNewEventChain((*iter)->mLayer->getCommFB());
          }
          found = true;
          break;
        }
      }
    } else {
      DEVLOG_ERROR("[HTTP Handler] Wrong HTTP request\n");
    }

    if(!found) {
      handlerReceivedWrongPath(paSocket, path);
    }
  }

  return found;
}

void CHTTP_Handler::removeSocketFromAccepted(const CIPComSocketHandler::TSocketDescriptor paSocket) {
  CCriticalRegion criticalRegion(mAcceptedMutex);
  for(CSinglyLinkedList<HTTPAcceptedSockets*>::Iterator iter = mAcceptedSockets.begin(); iter != mAcceptedSockets.end(); ++iter) {
    if((*iter)->mSocket == paSocket) {
      HTTPAcceptedSockets *toDelete = *iter;
      mAcceptedSockets.erase(toDelete);
      delete toDelete;
      break;
    }
  }
}

void CHTTP_Handler::handlerReceivedWrongPath(const CIPComSocketHandler::TSocketDescriptor paSocket, const std::string &paPath) {
  DEVLOG_ERROR("[HTTP Handler] Path %s has no FB registered\n", paPath.c_str());

  std::string toSend;
  std::string result("HTTP/1.1 404 Not Found"s);
  std::string mContentType("text/html"s);
  std::string mReqData;
  CHttpParser::createResponse(toSend, result, mContentType, mReqData);
  if(static_cast<int>(toSend.length()) != CIPComSocketHandler::sendDataOnTCP(paSocket, toSend.c_str(), static_cast<unsigned int>(toSend.length()))) {
    DEVLOG_ERROR("[HTTP Handler]: Error sending back the answer %s \n", toSend.c_str());
  }
  removeAndCloseSocket(paSocket);
}

bool CHTTP_Handler::sendClientData(forte::com_infra::CHttpComLayer *paLayer, const std::string &paToSend) {
  CIPComSocketHandler::TSocketDescriptor newSocket = CIPComSocketHandler::openTCPClientConnection(paLayer->getHost().data(), paLayer->getPort());
  if(CIPComSocketHandler::scmInvalidSocketDescriptor != newSocket) {
    if(static_cast<int>(paToSend.length()) == CIPComSocketHandler::sendDataOnTCP(newSocket, paToSend.c_str(), static_cast<unsigned int>(paToSend.length()))) {
      CCriticalRegion criticalRegion(mClientMutex);
      HTTPClientWaiting *toAdd = new HTTPClientWaiting();
      toAdd->mLayer = paLayer;
      toAdd->mSocket = newSocket;
      toAdd->mStartTime = func_NOW();
      startTimeoutThread();
      mClientLayers.pushBack(toAdd);
      getExtEvHandler<CIPComSocketHandler>().addComCallback(newSocket, this);
      resumeSelfsuspend();
      return true;
    } else {
      DEVLOG_ERROR("[HTTP Handler]: Couldn't send data to client %s:%u\n", paLayer->getHost().c_str(), paLayer->getPort());
      removeAndCloseSocket(newSocket);
    }
  } else {
    DEVLOG_ERROR("[HTTP Handler]: Couldn't open client connection for %s:%u\n", paLayer->getHost().c_str(), paLayer->getPort());
  }
  return false;
}

bool CHTTP_Handler::addServerPath(forte::com_infra::CHttpComLayer *paLayer, const std::string &paPath) {
  CCriticalRegion criticalRegion(mServerMutex);

  for(CSinglyLinkedList<HTTPServerWaiting*>::Iterator iter = mServerLayers.begin(); iter != mServerLayers.end(); ++iter) {
    if((*iter)->mPath == paPath) {
      DEVLOG_ERROR("[HTTP Handler]: The listening path \"%s\" was already added to the http server. Cannot add it again\n", paPath.c_str());
      return false;
    }
  }

  openHTTPServer();
  HTTPServerWaiting *toAdd = new HTTPServerWaiting();
  toAdd->mLayer = paLayer;
  toAdd->mPath = paPath;
  mServerLayers.pushBack(toAdd);
  DEVLOG_INFO("[HTTP Handler]: The listening path \"%s\" was added to the http server\n", paPath.c_str());
  return true;
}

void CHTTP_Handler::removeServerPath(const std::string &paPath) {
  CCriticalRegion criticalRegion(mServerMutex);

  for(CSinglyLinkedList<HTTPServerWaiting*>::Iterator iter = mServerLayers.begin(); iter != mServerLayers.end(); ++iter) {
    if((*iter)->mPath == paPath) {
      for(CSinglyLinkedList<CIPComSocketHandler::TSocketDescriptor>::Iterator iter_ = (*iter)->mSockets.begin(); iter_ != (*iter)->mSockets.end(); ++iter_) {
        removeAndCloseSocket(*iter_);
      }
      HTTPServerWaiting *toDelete = *iter;
      mServerLayers.erase(toDelete);
      delete toDelete;
      break;
    }
  }

  if(mServerLayers.isEmpty()) {
    closeHTTPServer();
  }
}

void CHTTP_Handler::sendServerAnswer(forte::com_infra::CHttpComLayer *paLayer, const std::string &paAnswer) {
  sendServerAnswerHelper(paLayer, paAnswer, false);
}

void CHTTP_Handler::sendServerAnswerFromRecv(forte::com_infra::CHttpComLayer *paLayer, const std::string &paAnswer) {
  sendServerAnswerHelper(paLayer, paAnswer, true);
}

void CHTTP_Handler::forceClose(forte::com_infra::CHttpComLayer *paLayer) {
  forceCloseHelper(paLayer, false);
}

void CHTTP_Handler::forceCloseFromRecv(forte::com_infra::CHttpComLayer *paLayer) {
  forceCloseHelper(paLayer, true);
}

void CHTTP_Handler::run() {
  DEVLOG_INFO("[HTTP Handler]: Starting HTTP timeout thread\n");

  mThreadStarted.inc();
  while(isAlive()) {
    if(mClientLayers.isEmpty() && mAcceptedSockets.isEmpty()) {
      selfSuspend();
    }
    if(!isAlive()) {
      break;
    }

    checkClientLayers();
    checkAcceptedSockets();
    sleepThread(100);

  }
}

void CHTTP_Handler::checkClientLayers() {
  CCriticalRegion criticalRegion(mClientMutex);
  if(!mClientLayers.isEmpty()) {
    CSinglyLinkedList<HTTPClientWaiting*> clientsToDelete;
    for(CSinglyLinkedList<HTTPClientWaiting*>::Iterator iter = mClientLayers.begin(); iter != mClientLayers.end(); ++iter) {
      // wait until result is ready
      CIEC_DATE_AND_TIME currentTime(func_NOW());
      if(currentTime.getMilliSeconds() - (*iter)->mStartTime.getMilliSeconds() > scmSendTimeout * 1000) {
        DEVLOG_ERROR("[HTTP Handler]: Timeout at client %s:%u \n", (*iter)->mLayer->getHost().c_str(), (*iter)->mLayer->getPort());
        removeAndCloseSocket((*iter)->mSocket);
        clientsToDelete.pushBack(*iter);
        (*iter)->mLayer->recvData(nullptr, 0); //indicates timeout
      }
    }
    for(CSinglyLinkedList<HTTPClientWaiting*>::Iterator iter = clientsToDelete.begin(); iter != clientsToDelete.end(); ++iter) {
      for(CSinglyLinkedList<HTTPClientWaiting*>::Iterator iter_ = mClientLayers.begin(); iter_ != mClientLayers.end(); ++iter_) {
        if(*iter_ == *iter) {
          mClientLayers.erase(*iter_);
          delete (*iter);
          break;
        }
      }
    }
  }
}

void CHTTP_Handler::checkAcceptedSockets() {
  CCriticalRegion criticalRegion(mAcceptedMutex);
  if(!mAcceptedSockets.isEmpty()) {
    CSinglyLinkedList<HTTPAcceptedSockets*> acceptedToDelete;
    for(CSinglyLinkedList<HTTPAcceptedSockets*>::Iterator iter = mAcceptedSockets.begin(); iter != mAcceptedSockets.end(); ++iter) {
      // wait until result is ready
      CIEC_DATE_AND_TIME currentTime(func_NOW());
      if(currentTime.getMilliSeconds() - (*iter)->mStartTime.getMilliSeconds() > scmAcceptedTimeout * 1000) {
        DEVLOG_ERROR("[HTTP Handler]: Timeout at accepted socket\n");
        removeAndCloseSocket((*iter)->mSocket);
        acceptedToDelete.pushBack(*iter);
      }
    }

    for(CSinglyLinkedList<HTTPAcceptedSockets*>::Iterator iter = acceptedToDelete.begin(); iter != acceptedToDelete.end(); ++iter) {
      for(CSinglyLinkedList<HTTPAcceptedSockets*>::Iterator iter_ = mAcceptedSockets.begin(); iter_ != mAcceptedSockets.end(); ++iter_) {
        if(*iter_ == *iter) {
          mAcceptedSockets.erase(*iter_);
          delete (*iter);
          break;
        }
      }
    }
  }
}

void CHTTP_Handler::startTimeoutThread() {
  if(!isAlive()) {
    start();
    mThreadStarted.waitIndefinitely();
    mThreadStarted.inc();
  }
}

void CHTTP_Handler::stopTimeoutThread() {
  setAlive(false);
  resumeSelfsuspend();
  end();
}

void CHTTP_Handler::openHTTPServer() {
  if(CIPComSocketHandler::scmInvalidSocketDescriptor == smServerListeningSocket) {
    char address[] = "127.0.0.1";
    smServerListeningSocket = CIPComSocketHandler::openTCPServerConnection(address, gHTTPServerPort);
    if(CIPComSocketHandler::scmInvalidSocketDescriptor != smServerListeningSocket) {
      getExtEvHandler<CIPComSocketHandler>().addComCallback(smServerListeningSocket, this);
      DEVLOG_INFO("[HTTP Handler] HTTP server listening on port %u\n", gHTTPServerPort);
    } else {
      DEVLOG_ERROR("[HTTP Handler] Couldn't start HTTP server on port %u\n", gHTTPServerPort);
    }
  }
}

void CHTTP_Handler::closeHTTPServer() {
  if(CIPComSocketHandler::scmInvalidSocketDescriptor != smServerListeningSocket) {
    removeAndCloseSocket(smServerListeningSocket);
    smServerListeningSocket = CIPComSocketHandler::scmInvalidSocketDescriptor;
  }
}

void CHTTP_Handler::removeAndCloseSocket(const CIPComSocketHandler::TSocketDescriptor paSocket) {
  //normally, when the device is being killed the CIPComSocketHandler is already dead, so calls to it must be avoided.
  if(mDeviceExecution.isExtEvHandlerValid(CIPComSocketHandler::mHandlerIdentifier)) {
    getExtEvHandler<CIPComSocketHandler>().removeComCallback(paSocket);
    getExtEvHandler<CIPComSocketHandler>().closeSocket(paSocket);
  }
}

void CHTTP_Handler::resumeSelfsuspend() {
  mSuspendSemaphore.inc();
}

void CHTTP_Handler::selfSuspend() {
  mSuspendSemaphore.waitIndefinitely();
}

void CHTTP_Handler::sendServerAnswerHelper(forte::com_infra::CHttpComLayer *paLayer, const std::string &paAnswer, bool paFromRecv) {
  if(!paFromRecv) {
    mServerMutex.lock();
  }

  for(CSinglyLinkedList<HTTPServerWaiting*>::Iterator iter = mServerLayers.begin(); iter != mServerLayers.end(); ++iter) {
    if((*iter)->mLayer == paLayer) {
      CSinglyLinkedList<CIPComSocketHandler::TSocketDescriptor>::Iterator iterSocket = (*iter)->mSockets.begin();
      if(static_cast<int>(paAnswer.length()) != CIPComSocketHandler::sendDataOnTCP(*iterSocket, paAnswer.c_str(), static_cast<unsigned int>(paAnswer.length()))) {
        DEVLOG_ERROR("[HTTP Handler]: Error sending back the answer %s \n", paAnswer.c_str());
      }
      removeAndCloseSocket(*iterSocket);
      (*iter)->mSockets.popFront();
      break;
    }
  }

  if(!paFromRecv) {
    mServerMutex.unlock();
  }
}

void CHTTP_Handler::forceCloseHelper(forte::com_infra::CHttpComLayer *paLayer, bool paFromRecv) {
  if(!paFromRecv) {
    mServerMutex.lock();
  }

  bool found = false;

  for(CSinglyLinkedList<HTTPServerWaiting*>::Iterator iter = mServerLayers.begin(); iter != mServerLayers.end(); ++iter) {
    if((*iter)->mLayer == paLayer) {
      if(!(*iter)->mSockets.isEmpty()) {
        CSinglyLinkedList<CIPComSocketHandler::TSocketDescriptor>::Iterator itSocket = (*iter)->mSockets.begin();
        removeAndCloseSocket(*itSocket);
        (*iter)->mSockets.popFront();
      }
      found = true;
      break;
    }
  }

  if(!paFromRecv) {
    mServerMutex.unlock();
    mClientMutex.lock();
  }

  if(!found && !mClientLayers.isEmpty()) {
    for(CSinglyLinkedList<HTTPClientWaiting*>::Iterator iter = mClientLayers.begin(); iter != mClientLayers.end(); ++iter) {
      if((*iter)->mLayer == paLayer) {
        removeAndCloseSocket((*iter)->mSocket);
      }
    }
  }

  if(!paFromRecv) {
    mClientMutex.unlock();
  }
}
