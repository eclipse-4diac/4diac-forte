/*******************************************************************************
 * Copyright (c) 2017, 2023, 2024 Marc Jakobi, github.com/MrcJkb
 *                          fortiss GmbH
 *                          Primetals Technologies Austria GmbH
 *                          OFFIS e.V.
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Marc Jakobi - initial implementation for HTTP clients
 *    Jose Cabral - Merge old HTTPIpLayer to this one and use CIEC_STRING
 *    Martin Melik Merkumians - change CIEC_STRING to std::string
 *    Malte Grave - fixed the handleHTTPResponse output
 ********************************************************************************/

#include "httplayer.h"
#include "httpparser.h"
#include "../../arch/devlog.h"
#include <string.h>
#include "basecommfb.h"
#include "http_handler.h"
#include "comtypes.h"

using namespace forte::com_infra;
using namespace std::string_literals;

CHttpComLayer::CHttpComLayer(CComLayer* paUpperLayer, CBaseCommFB* paComFB) :
    CComLayer(paUpperLayer, paComFB), mInterruptResp(e_Nothing), mRequestType(e_NOTSET), mPort(80), mBufFillSize(0), mCorrectlyInitialized(false) {
  memset(mRecvBuffer, 0, sizeof(mRecvBuffer));
}

CHttpComLayer::~CHttpComLayer() {
  closeConnection();
}

EComResponse CHttpComLayer::openConnection(char *paLayerParameter) {
  EComResponse eRetVal = e_InitInvalidId;
  if(checkSDsAndRDsType()) {
    switch(mFb->getComServiceType()){
      case e_Server:
        eRetVal = startServer(paLayerParameter);
        break;
      case e_Client:
        eRetVal = openClientConnection(paLayerParameter);
        break;
      default:
        // e_Publisher and e_Subscriber
        eRetVal = e_ProcessDataInvalidObject;
        break;
    }
  }
  mCorrectlyInitialized = (eRetVal == e_InitOk);
  return eRetVal;
}

EComResponse CHttpComLayer::startServer (char *paLayerParameter) {
  EComResponse eRetVal = e_InitInvalidId;
  if (1 == mFb->getNumSD()) {
    mPath = std::string(paLayerParameter);
    if (getExtEvHandler<CHTTP_Handler>().addServerPath(this, mPath)) {
      eRetVal = e_InitOk;
    }
  } else {
    DEVLOG_ERROR(
        "[HTTP Layer] The FB with PARAM %s coudln't be initialized since only one SD is possible in HTTP Server which is for the response\n",
        paLayerParameter);
  }
  return eRetVal;
}

EComResponse CHttpComLayer::openClientConnection(char* paLayerParameter) {
  EComResponse eRetVal = e_InitInvalidId;
  if(2 == mFb->getNumRD() && 3 == mFb->getNumSD()) {
    CParameterParser parser(paLayerParameter, ';', 3); //IP:PORT;POST|PUT|GET;[content-type]
    handleContentAndRequestType(parser, parser.parseParameters());
    handleAddress(parser[0]);
    eRetVal = e_InitOk;
  } else {
    DEVLOG_ERROR(
        "[HTTP Layer] A client should ALWAYS have exactly 2 RD, the first one for the response code, and the second for the response body, and ALWAYS exactly 3 SD one for authentication, one for parameters, and one for the body.\n");
  }
  mConnectionState = e_Disconnected;
  return eRetVal;
}

bool CHttpComLayer::checkSDsAndRDsType() const {
  for(size_t i = 2; i < mFb->getNumSD(); i++) {
    CIEC_ANY::EDataTypeID typeToCheck = mFb->getDI(static_cast<unsigned int>(i))->getDataTypeID();
    if(CIEC_ANY::e_ANY != typeToCheck && CIEC_ANY::e_STRING != typeToCheck && CIEC_ANY::e_WSTRING != typeToCheck) {
      DEVLOG_ERROR("[HTTP Layer] Client called %s has an invalid SD_%d\n", CStringDictionary::get(mFb->getInstanceNameId()), i);
      return false;
    }
  }
  for(size_t i = 2; i < mFb->getNumRD(); i++) {
    CIEC_ANY::EDataTypeID typeToCheck = mFb->getDO(static_cast<unsigned int>(i))->getDataTypeID();
    if(CIEC_ANY::e_ANY != typeToCheck && CIEC_ANY::e_STRING != typeToCheck && CIEC_ANY::e_WSTRING != typeToCheck) {
      DEVLOG_ERROR("[HTTP Layer] Client called %s has an invalid RD_%d\n", CStringDictionary::get(mFb->getInstanceNameId()), i);
      return false;
    }
  }
  return true;
}

bool CHttpComLayer::storeRequestType(const char* paType) {
  bool everythingOK = true;
  if(0 == strcmp(paType, "POST")) {
    mRequestType = e_POST;
  } else if(0 == strcmp(paType, "PUT")) {
    mRequestType = e_PUT;
  } else if(0 == strcmp(paType, "GET")) {
    mRequestType = e_GET;
  } else {
    DEVLOG_ERROR("[HTTP Layer] GET, PUT or POST must be defined, but %s was defined instead\n", paType);
    everythingOK = false;
  }
  return everythingOK;
}

bool CHttpComLayer::handleContentAndRequestType(CParameterParser &paParser, size_t paNoOfParameters) {
  bool everythingOK = true;
  if(1 < paNoOfParameters) {
    if(3 == paNoOfParameters) {
      mContentType = std::string(paParser[2]);
    } else {
      mContentType = "text/html"s;
    }
    everythingOK = storeRequestType(paParser[1]);
  } else {
    everythingOK = false;
    DEVLOG_ERROR("[HTTP Layer] Wrong parameters. It should be as IP:PORT;POST|PUT|GET;[content-type]\n");
  }
  return everythingOK;
}

bool CHttpComLayer::handleAddress(const char* paAddress) {
  bool everythingOK = true;
  //look for parameters
  std::string addressToParse(paAddress);
  char *firstSlash = strchr(addressToParse.data(), '/');
  if(firstSlash) {
    mPath = std::string(firstSlash);
    *firstSlash = '\0';
    CParameterParser portParser(addressToParse.c_str(), ':', 2);
    if(2 == portParser.parseParameters()) {
      mHost = std::string(portParser[0]);
      mPort = static_cast<TForteUInt16>(forte::core::util::strtoul(portParser[1], nullptr, 10));
    } else {
      mHost = addressToParse;
      DEVLOG_INFO("[HTTP Layer] No port was found on the parameter, using default 80\n");
    }
  } else {
    everythingOK = false;
    DEVLOG_ERROR("[HTTP Layer] No path was found on the parameter\n");
  }
  return everythingOK;
}

const CIEC_ANY& CHttpComLayer::getSDx(void *paData, int paSdNum){
  const CIEC_ANY **sds = static_cast<const CIEC_ANY **>(paData);
  return sds[paSdNum]->unwrap();
}

EComResponse CHttpComLayer::sendData(void *paData, unsigned int) {
  mInterruptResp = e_Nothing;
  if(mCorrectlyInitialized) {
    switch(mFb->getComServiceType()){
      case e_Server:
        sendDataAsServer(paData);
        break;
      case e_Client:
        sendDataAsClient(paData);
        break;
      default:
        // e_Publisher and e_Subscriber
        break;
    }
  } else {
    DEVLOG_ERROR("[HTTP Layer]The FB is not initialized\n");
  }

  return mInterruptResp;
}

void CHttpComLayer::createRequest() {
  switch (mRequestType) {
    case e_PUT:
    case e_POST:
      CHttpParser::createPutPostRequest(mRequest, mHost, mPort, mPath, mAuth, mParams, mBody, mContentType, mRequestType);
      DEVLOG_INFO(
          "[HTTP Layer] FB with PUT/POST request initialized. Host: %s:%d, Path: %s, Parameters: '%s'\n",
          mHost.c_str(), mPort, mPath.c_str(), mParams.c_str());
      break;
    case e_GET:
      CHttpParser::createGetRequest(mRequest, mHost, mPort, mPath, mAuth, mParams);
      DEVLOG_INFO("[HTTP Layer] FB with GET request initialized. Host: %s, Path: %s, Parameters: '%s'\n", mHost.c_str(), mPath.c_str(), mParams.c_str());
      break;
    default:
      break;
  }
}

void CHttpComLayer::sendDataAsServer(void *paData) {
  serializeData(getSDx(paData, 0), mBody);
  createRequest();
  CHttpParser::createResponse(mRequest, "HTTP/1.1 200 OK"s, "text/plain"s, mBody);
  getExtEvHandler<CHTTP_Handler>().sendServerAnswer(this, mRequest);
  mInterruptResp = e_ProcessDataOk;
}

void CHttpComLayer::sendDataAsClient(void *paData) {
  serializeData(getSDx(paData, 0), mAuth);
  serializeData(getSDx(paData, 1), mParams);
  serializeData(getSDx(paData, 2), mBody);
  createRequest();
  if(getExtEvHandler<CHTTP_Handler>().sendClientData(this, mRequest)) {
    mInterruptResp = e_ProcessDataOk;
  } else {
    mInterruptResp = e_ProcessDataDataTypeError;
    mInterruptResp = e_ProcessDataSendFailed;
    DEVLOG_ERROR("[HTTP Layer] Sending request on TCP failed\n");
  }
}

EComResponse CHttpComLayer::recvData(const void *paData, unsigned int paSize) {
  mInterruptResp = e_Nothing;
  if(mCorrectlyInitialized) {
    memcpy(mRecvBuffer, paData, (paSize > cgIPLayerRecvBufferSize) ? cgIPLayerRecvBufferSize : paSize);
    switch(mFb->getComServiceType()){
      case e_Server:
        DEVLOG_ERROR("[HTTP Layer] Receiving raw data as a Server? That's wrong, use the recvServerData function\n");
        break;
      case e_Client:
        if(nullptr == paData) { //timeout occurred
          mInterruptResp = e_ProcessDataRecvFaild;
        } else {
          if(e_ProcessDataOk != (mInterruptResp = handleHTTPResponse(mRecvBuffer))) {
            DEVLOG_ERROR("[HTTP Layer] FB with host: %s:%u couldn't handle the HTTP response\n", mHost.c_str(), mPort);
          } else {
            //TODO Trigger event?
          }
        }
        break;
      default:
        break;
    }
  } else {
    DEVLOG_ERROR("[HTTP Layer] The FB is not initialized\n");
  }
  if(e_ProcessDataOk == mInterruptResp) {
    mFb->interruptCommFB(this);
  }
  return mInterruptResp;
}

EComResponse forte::com_infra::CHttpComLayer::recvServerData(CSinglyLinkedList<std::string>&, CSinglyLinkedList<std::string>& paParameterValues) {
  //for now, the parameterNames are not taken in account, and the parameters are put in the same order they arrived

  mInterruptResp = e_Nothing;
  bool failed = false;
  if(0 < mFb->getNumSD()) {
    unsigned int noOfParameters = 0;
    for(CSinglyLinkedList<std::string>::Iterator iter = paParameterValues.begin(); iter != paParameterValues.end(); ++iter) {
      noOfParameters++;
    }

    if(noOfParameters == mFb->getNumRD()) {
      noOfParameters = 0;
      for(CSinglyLinkedList<std::string>::Iterator iter = paParameterValues.begin(); iter != paParameterValues.end(); ++iter) {
        mFb->getRDs()[noOfParameters++]->setValue(CIEC_STRING(*iter));
      }
    } else {
      DEVLOG_ERROR("[HTTP Layer] FB with path %s received a number of parameters of %u, while it has %u SDs\n", mPath.c_str(),
        static_cast<TForteUInt16>(noOfParameters), mFb->getNumRD());
      failed = true;
    }
  }

  if(failed) {
    std::string toSend;
    std::string result("HTTP/1.1 400 Bad Request"s);
    mBody.clear();
    CHttpParser::createResponse(toSend, result, mContentType, mBody);
    getExtEvHandler<CHTTP_Handler>().sendServerAnswerFromRecv(this, toSend);
    mInterruptResp = e_ProcessDataDataTypeError;
  } else {
    mInterruptResp = e_ProcessDataOk;
  }
  if(e_ProcessDataOk == mInterruptResp) {
    mFb->interruptCommFB(this);
  }
  return mInterruptResp;
}

EComResponse CHttpComLayer::handleHTTPResponse(char *paData) {
  DEVLOG_DEBUG("[HTTP Layer] Handling received HTTP response\n");
  EComResponse eRetVal = e_ProcessDataRecvFaild;
  if(mFb != nullptr) {
    CIEC_ANY** apoRDs = mFb->getRDs();
    // Interpret HTTP response and set output status according to success/failure.
    std::string responseCode;
    std::string output;
    CHttpParser::parseResponse(output, responseCode, paData) ? eRetVal = e_ProcessDataOk : eRetVal = e_ProcessDataRecvFaild;
    apoRDs[0]->setValue(CIEC_STRING(responseCode));
    apoRDs[1]->setValue(CIEC_STRING(output));
  } else {
    DEVLOG_ERROR("[HTTP Layer] No FB defined\n");
  }
  return eRetVal;
}

EComResponse CHttpComLayer::processInterrupt() {
  mInterruptResp = e_ProcessDataOk;
  return mInterruptResp;
}

void CHttpComLayer::closeConnection() {
  if(e_Server == mFb->getComServiceType()) {
    getExtEvHandler<CHTTP_Handler>().removeServerPath(mPath);
  }
  getExtEvHandler<CHTTP_Handler>().forceClose(this);
}

void CHttpComLayer::serializeData(const CIEC_ANY& paSDx, std::string& paMember) {
  if(CIEC_ANY::e_WSTRING == paSDx.getDataTypeID()){
      paMember = static_cast<const CIEC_WSTRING&>(paSDx).getValue();
  }else if(CIEC_ANY::e_STRING == paSDx.getDataTypeID()){
      paMember = static_cast<const CIEC_STRING&>(paSDx).getStorage();
  }
}

std::string& forte::com_infra::CHttpComLayer::getHost() {
  return mHost;
}

TForteUInt16 forte::com_infra::CHttpComLayer::getPort() const {
  return mPort;
}

