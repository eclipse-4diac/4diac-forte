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

CIPComSocketHandler::TSocketDescriptor CHTTP_Handler::smServerListeningSocket =
    CIPComSocketHandler::scmInvalidSocketDescriptor;

char CHTTP_Handler::sRecvBuffer[];
unsigned int CHTTP_Handler::sBufFillSize = 0;
const unsigned int CHTTP_Handler::scmSendTimeout = 20;
const unsigned int CHTTP_Handler::scmAcceptedTimeout = 5;

DEFINE_HANDLER(CHTTP_Handler);

CHTTP_Handler::CHTTP_Handler(CDeviceExecution &paDeviceExecution) : CExternalEventHandler(paDeviceExecution) {
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
  for (auto &serverLayer : mServerLayers) {
    for (auto socket : serverLayer.mSockets) {
      removeAndCloseSocket(socket);
    }
  }
  mServerLayers.clear();
}

void CHTTP_Handler::clearClientLayers() {
  CCriticalRegion criticalRegion(mClientMutex);
  for (auto &clientLayer : mClientLayers) {
    removeAndCloseSocket(clientLayer.mSocket);
  }
  mClientLayers.clear();
}

void CHTTP_Handler::clearAcceptedSockets() {
  CCriticalRegion criticalRegion(mAcceptedMutex);
  for (auto &acceptedSocket : mAcceptedSockets) {
    removeAndCloseSocket(acceptedSocket.mSocket);
  }
  mAcceptedSockets.clear();
}

void CHTTP_Handler::setPriority(int) {
  // currently we are doing nothing here.
}

int CHTTP_Handler::getPriority() const {
  // the same as for setPriority
  return 0;
}

forte::com_infra::EComResponse
CHTTP_Handler::recvData(const void *paData,
                        unsigned int) { // TODO: do something with the size parameter of the received data?
  CIPComSocketHandler::TSocketDescriptor socket =
      *(static_cast<const CIPComSocketHandler::TSocketDescriptor *>(paData));

  if (socket == smServerListeningSocket) {
    CIPComSocketHandler::TSocketDescriptor newConnection = CIPComSocketHandler::acceptTCPConnection(socket);
    if (CIPComSocketHandler::scmInvalidSocketDescriptor != newConnection) {
      CCriticalRegion criticalRegion(mAcceptedMutex);
      HTTPAcceptedSockets accepted;
      accepted.mSocket = newConnection;
      accepted.mStartTime = func_NOW_MONOTONIC();
      mAcceptedSockets.emplace_back(std::move(accepted));
      getExtEvHandler<CIPComSocketHandler>().addComCallback(newConnection, this);
      resumeSelfsuspend();
    } else {
      DEVLOG_ERROR("[HTTP Handler] Couldn't accept new HTTP connection\n");
    }
  } else {
    int recvLen = CIPComSocketHandler::receiveDataFromTCP(socket, &sRecvBuffer[sBufFillSize],
                                                          cgIPLayerRecvBufferSize - sBufFillSize);
    if (0 == recvLen) {
      removeAndCloseSocket(socket);
      removeHTTPLayerFromClientList(socket);
    } else if (-1 == recvLen) {
      removeAndCloseSocket(socket);
      removeHTTPLayerFromClientList(socket);
      DEVLOG_ERROR("[HTTP handler] Error receiving packet\n");
    } else {
      if (!recvClients(socket, recvLen) && !recvServers(socket)) {
        DEVLOG_WARNING("[HTTP Handler]: A packet arrived to the wrong place\n");
      }
    }
  }

  return e_Nothing;
}

bool CHTTP_Handler::recvClients(const CIPComSocketHandler::TSocketDescriptor paSocket,
                                const int paRecvLength) { // check clients
  CCriticalRegion criticalRegion(mClientMutex);
  for (auto clientLayer = mClientLayers.begin(); clientLayer != mClientLayers.end(); clientLayer++) {
    if (clientLayer->mSocket == paSocket) {
      if (e_ProcessDataOk == clientLayer->mLayer->recvData(sRecvBuffer, static_cast<unsigned int>(paRecvLength))) {
        startNewEventChain(clientLayer->mLayer->getCommFB());
      }
      removeAndCloseSocket(paSocket);
      mClientLayers.erase(clientLayer);
      return true;
    }
  }

  return false;
}

bool CHTTP_Handler::removeHTTPLayerFromClientList(const CIPComSocketHandler::TSocketDescriptor paSocket) {
  CCriticalRegion criticalRegion(mClientMutex);
  for (auto clientLayer = mClientLayers.begin(); clientLayer != mClientLayers.end(); clientLayer++) {
    if (clientLayer->mSocket == paSocket) {
      // removeAndCloseSocket(paSocket);
      mClientLayers.erase(clientLayer);
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
  if (!mServerLayers.empty()) {
    std::string path;
    std::vector<std::string> parameterNames;
    std::vector<std::string> parameterValues;
    bool noParsingError = false;
    switch (CHttpParser::getTypeOfRequest(sRecvBuffer)) {
      case CHttpComLayer::e_GET:
        noParsingError = CHttpParser::parseGetRequest(path, parameterNames, parameterValues, sRecvBuffer);
        break;
      case CHttpComLayer::e_POST:
      case CHttpComLayer::e_PUT: {
        std::string content;
        noParsingError = CHttpParser::parsePutPostRequest(path, content, sRecvBuffer);
        parameterValues.push_back(content);
        break;
      }
      default: break;
    }

    if (noParsingError) {
      for (auto &serverLayer : mServerLayers) {
        if (serverLayer.mPath == path) {
          serverLayer.mSockets.push_back(paSocket);
          if (e_ProcessDataOk == serverLayer.mLayer->recvServerData(parameterNames, parameterValues)) {
            startNewEventChain(serverLayer.mLayer->getCommFB());
          }
          found = true;
          break;
        }
      }
    } else {
      DEVLOG_ERROR("[HTTP Handler] Wrong HTTP request\n");
    }

    if (!found) {
      handlerReceivedWrongPath(paSocket, path);
    }
  }

  return found;
}

void CHTTP_Handler::removeSocketFromAccepted(const CIPComSocketHandler::TSocketDescriptor paSocket) {
  CCriticalRegion criticalRegion(mAcceptedMutex);
  for (auto acceptedSocket = mAcceptedSockets.begin(); acceptedSocket != mAcceptedSockets.end(); acceptedSocket++) {
    if (acceptedSocket->mSocket == paSocket) {
      mAcceptedSockets.erase(acceptedSocket);
      break;
    }
  }
}

void CHTTP_Handler::handlerReceivedWrongPath(const CIPComSocketHandler::TSocketDescriptor paSocket,
                                             const std::string &paPath) {
  DEVLOG_ERROR("[HTTP Handler] Path %s has no FB registered\n", paPath.c_str());

  std::string toSend;
  std::string result("HTTP/1.1 404 Not Found"s);
  std::string mContentType("text/html"s);
  std::string mReqData;
  CHttpParser::createResponse(toSend, result, mContentType, mReqData);
  if (static_cast<int>(toSend.length()) !=
      CIPComSocketHandler::sendDataOnTCP(paSocket, toSend.c_str(), static_cast<unsigned int>(toSend.length()))) {
    DEVLOG_ERROR("[HTTP Handler]: Error sending back the answer %s \n", toSend.c_str());
  }
  removeAndCloseSocket(paSocket);
}

bool CHTTP_Handler::sendClientData(forte::com_infra::CHttpComLayer *paLayer, const std::string &paToSend) {
  CIPComSocketHandler::TSocketDescriptor newSocket =
      CIPComSocketHandler::openTCPClientConnection(paLayer->getHost().data(), paLayer->getPort());
  if (CIPComSocketHandler::scmInvalidSocketDescriptor != newSocket) {
    if (static_cast<int>(paToSend.length()) ==
        CIPComSocketHandler::sendDataOnTCP(newSocket, paToSend.c_str(), static_cast<unsigned int>(paToSend.length()))) {
      CCriticalRegion criticalRegion(mClientMutex);
      HTTPClientWaiting toAdd;
      toAdd.mLayer = paLayer;
      toAdd.mSocket = newSocket;
      toAdd.mStartTime = func_NOW_MONOTONIC();
      startTimeoutThread();
      mClientLayers.emplace_back(std::move(toAdd));
      getExtEvHandler<CIPComSocketHandler>().addComCallback(newSocket, this);
      resumeSelfsuspend();
      return true;
    } else {
      DEVLOG_ERROR("[HTTP Handler]: Couldn't send data to client %s:%u\n", paLayer->getHost().c_str(),
                   paLayer->getPort());
      removeAndCloseSocket(newSocket);
    }
  } else {
    DEVLOG_ERROR("[HTTP Handler]: Couldn't open client connection for %s:%u\n", paLayer->getHost().c_str(),
                 paLayer->getPort());
  }
  return false;
}

bool CHTTP_Handler::addServerPath(forte::com_infra::CHttpComLayer *paLayer, const std::string &paPath) {
  CCriticalRegion criticalRegion(mServerMutex);

  for (auto &serverLayer : mServerLayers) {
    if (serverLayer.mPath == paPath) {
      DEVLOG_ERROR(
          "[HTTP Handler]: The listening path \"%s\" was already added to the http server. Cannot add it again\n",
          paPath.c_str());
      return false;
    }
  }

  openHTTPServer();
  HTTPServerWaiting toAdd;
  toAdd.mLayer = paLayer;
  toAdd.mPath = paPath;
  mServerLayers.emplace_back(std::move(toAdd));
  DEVLOG_INFO("[HTTP Handler]: The listening path \"%s\" was added to the http server\n", paPath.c_str());
  return true;
}

void CHTTP_Handler::removeServerPath(const std::string &paPath) {
  CCriticalRegion criticalRegion(mServerMutex);

  for (auto serverLayer = mServerLayers.begin(); serverLayer != mServerLayers.end(); serverLayer++) {
    if (serverLayer->mPath == paPath) {
      for (auto socket : serverLayer->mSockets) {
        removeAndCloseSocket(socket);
      }
      mServerLayers.erase(serverLayer);
      break;
    }
  }

  if (mServerLayers.empty()) {
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
  while (isAlive()) {
    if (mClientLayers.empty() && mAcceptedSockets.empty()) {
      selfSuspend();
    }
    if (!isAlive()) {
      break;
    }

    checkClientLayers();
    checkAcceptedSockets();
    sleepThread(100);
  }
}

void CHTTP_Handler::checkClientLayers() {
  CCriticalRegion criticalRegion(mClientMutex);
  if (!mClientLayers.empty()) {
    std::vector<HTTPClientWaiting *> clientsToDelete;
    for (auto clientLayer = mClientLayers.begin(); clientLayer != mClientLayers.end();) {
      // wait until result is ready
      if (func_NOW_MONOTONIC().getInMilliSeconds() >
          clientLayer->mStartTime.getInMilliSeconds() + scmSendTimeout * 1000) {
        DEVLOG_ERROR("[HTTP Handler]: Timeout at client %s:%u \n", clientLayer->mLayer->getHost().c_str(),
                     clientLayer->mLayer->getPort());
        removeAndCloseSocket(clientLayer->mSocket);
        clientLayer->mLayer->recvData(nullptr, 0); // indicates timeout
        clientLayer = mClientLayers.erase(clientLayer);
      } else {
        clientLayer++;
      }
    }
  }
}

void CHTTP_Handler::checkAcceptedSockets() {
  CCriticalRegion criticalRegion(mAcceptedMutex);
  if (!mAcceptedSockets.empty()) {
    std::vector<HTTPAcceptedSockets *> acceptedToDelete;
    for (auto acceptedSocket = mAcceptedSockets.begin(); acceptedSocket != mAcceptedSockets.end();) {
      // wait until result is ready
      if (func_NOW_MONOTONIC().getInMilliSeconds() >
          acceptedSocket->mStartTime.getInMilliSeconds() + scmAcceptedTimeout * 1000) {
        DEVLOG_ERROR("[HTTP Handler]: Timeout at accepted socket\n");
        removeAndCloseSocket(acceptedSocket->mSocket);
        acceptedSocket = mAcceptedSockets.erase(acceptedSocket);
      } else {
        acceptedSocket++;
      }
    }
  }
}

void CHTTP_Handler::startTimeoutThread() {
  if (!isAlive()) {
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
  if (CIPComSocketHandler::scmInvalidSocketDescriptor == smServerListeningSocket) {
    char address[] = "127.0.0.1";
    smServerListeningSocket = CIPComSocketHandler::openTCPServerConnection(address, gHTTPServerPort);
    if (CIPComSocketHandler::scmInvalidSocketDescriptor != smServerListeningSocket) {
      getExtEvHandler<CIPComSocketHandler>().addComCallback(smServerListeningSocket, this);
      DEVLOG_INFO("[HTTP Handler] HTTP server listening on port %u\n", gHTTPServerPort);
    } else {
      DEVLOG_ERROR("[HTTP Handler] Couldn't start HTTP server on port %u\n", gHTTPServerPort);
    }
  }
}

void CHTTP_Handler::closeHTTPServer() {
  if (CIPComSocketHandler::scmInvalidSocketDescriptor != smServerListeningSocket) {
    removeAndCloseSocket(smServerListeningSocket);
    smServerListeningSocket = CIPComSocketHandler::scmInvalidSocketDescriptor;
  }
}

void CHTTP_Handler::removeAndCloseSocket(const CIPComSocketHandler::TSocketDescriptor paSocket) {
  // normally, when the device is being killed the CIPComSocketHandler is already dead, so calls to it must be avoided.
  if (mDeviceExecution.isExtEvHandlerValid(CIPComSocketHandler::mHandlerIdentifier)) {
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

void CHTTP_Handler::sendServerAnswerHelper(forte::com_infra::CHttpComLayer *paLayer,
                                           const std::string &paAnswer,
                                           bool paFromRecv) {
  if (!paFromRecv) {
    mServerMutex.lock();
  }

  for (auto &serverLayer : mServerLayers) {
    if (serverLayer.mLayer == paLayer) {
      auto socket = serverLayer.mSockets.begin();
      if (static_cast<int>(paAnswer.length()) !=
          CIPComSocketHandler::sendDataOnTCP(*socket, paAnswer.c_str(), static_cast<unsigned int>(paAnswer.length()))) {
        DEVLOG_ERROR("[HTTP Handler]: Error sending back the answer %s \n", paAnswer.c_str());
      }
      removeAndCloseSocket(*socket);
      serverLayer.mSockets.erase(serverLayer.mSockets.begin());
      break;
    }
  }

  if (!paFromRecv) {
    mServerMutex.unlock();
  }
}

void CHTTP_Handler::forceCloseHelper(forte::com_infra::CHttpComLayer *paLayer, bool paFromRecv) {
  if (!paFromRecv) {
    mServerMutex.lock();
  }

  bool found = false;

  for (auto &serverLayer : mServerLayers) {
    if (serverLayer.mLayer == paLayer) {
      if (!serverLayer.mSockets.empty()) {
        auto itSocket = serverLayer.mSockets.begin();
        removeAndCloseSocket(*itSocket);
        serverLayer.mSockets.erase(serverLayer.mSockets.begin());
      }
      found = true;
      break;
    }
  }

  if (!paFromRecv) {
    mServerMutex.unlock();
    mClientMutex.lock();
  }

  if (!found && !mClientLayers.empty()) {
    for (auto &clientLayer : mClientLayers) {
      if (clientLayer.mLayer == paLayer) {
        removeAndCloseSocket(clientLayer.mSocket);
      }
    }
  }

  if (!paFromRecv) {
    mClientMutex.unlock();
  }
}
