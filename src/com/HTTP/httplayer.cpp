/*******************************************************************************
 * Copyright (c) 2017-2018 Marc Jakobi, github.com/MrcJkb, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Marc Jakobi - initial implementation for HTTP clients
 *    Jose Cabral - Merge old HTTPIpLayer to this one and use CIEC_STRING
 ********************************************************************************/

#include "httplayer.h"
#include "httpparser.h"
#include "../../arch/devlog.h"
#include <string.h>
#include "basecommfb.h"
#include "http_handler.h"
#include "comtypes.h"

using namespace forte::com_infra;

CHttpComLayer::CHttpComLayer(CComLayer* paUpperLayer, CBaseCommFB* paComFB) :
    CComLayer(paUpperLayer, paComFB), mInterruptResp(e_Nothing), mRequestType(e_NOTSET), mPort(80), mBufFillSize(0), mCorrectlyInitialized(false),
        mHasParameterInSD(false) {
  memset(mRecvBuffer, 0, sizeof(mRecvBuffer));
}

CHttpComLayer::~CHttpComLayer() {
  closeConnection();
}

EComResponse CHttpComLayer::openConnection(char *paLayerParameter) {
  EComResponse eRetVal = e_InitInvalidId;

  if(checkSDsAndRDsType()) {
    switch(m_poFb->getComServiceType()){
      case e_Server:
        if(1 == m_poFb->getNumSD()) {
          mPath = paLayerParameter;
          if(getExtEvHandler<CHTTP_Handler>().addServerPath(this, mPath)) {
            eRetVal = e_InitOk;
          }
        } else {
          DEVLOG_ERROR("[HTTP Layer] The FB with PARAM %s coudln't be initialized since only one SD is possible in HTTP Server which is for the response\n",
            paLayerParameter);
        }
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

EComResponse CHttpComLayer::openClientConnection(char* paLayerParameter) {
  EComResponse eRetVal = e_InitInvalidId;
  unsigned int numberOfSD = m_poFb->getNumSD();

  if(2 == m_poFb->getNumRD()) {
    CParameterParser parser(paLayerParameter, ';', 3); //IP:PORT;POST|PUT|GET;[content-type]

    if(handleContentAndRequestType(parser, parser.parseParameters()) && handleAddress(parser[0], numberOfSD)) {

      switch(mRequestType){
        case e_PUT:
        case e_POST:
          if(checkSDInPOSTAndPUT(numberOfSD)) {
            CHttpParser::createPutPostRequest(mRequest, mHost, mPath, mReqData, mContentType, mRequestType);
            eRetVal = e_InitOk;
            DEVLOG_INFO("[HTTP Layer] FB with PUT/POST request initialized. Host: %s, Path: %s\n", mHost.getValue(), mPath.getValue());
          }
          break;
        case e_GET:
          if(0 != numberOfSD) {
            DEVLOG_WARNING("[HTTP Layer] FB with GET request has SD which are ignore. If you want to have parameters, put them in the path in the PARAMS\n");
          }
          CHttpParser::createGetRequest(mRequest, mHost, mPath);
          eRetVal = e_InitOk;
          DEVLOG_INFO("[HTTP Layer] FB with GET request initialized. Host: %s, Path: %s\n", mHost.getValue(), mPath.getValue());
          break;
        default:
          break;
      }
    }
  } else {
    DEVLOG_ERROR(
      "[HTTP Layer] A client should ALWAYS have exactly 2 RD, the first one for the response code, and the second for the response body, even for PUT and POST\n");
  }

  m_eConnectionState = e_Disconnected;
  return eRetVal;
}

bool CHttpComLayer::checkSDInPOSTAndPUT(size_t paNoOfSD) {
  bool evetythingOK = true;

  if(0 == paNoOfSD) {
    if("" == mReqData) {
      DEVLOG_ERROR("[HTTP Layer] You are using a POST/PUT FB but no data is defined as SD nor as parameters in PARAMS\n");
      evetythingOK = false;
    }
  } else if(1 == paNoOfSD) {
    if("" != mReqData) {
      DEVLOG_WARNING("[HTTP Layer] Parameters in PARAMS are ignored for PUT/POST request data and SDs are sent instead\n");
    }
    mHasParameterInSD = true;
  } else {
    DEVLOG_ERROR("[HTTP Layer] You can use maximum 1 SD\n");
    evetythingOK = false;
  }

  return evetythingOK;
}

bool CHttpComLayer::checkSDsAndRDsType() const {

  for(size_t i = 2; i < m_poFb->getNumSD(); i++) {
    CIEC_ANY::EDataTypeID typeToCheck = m_poFb->getDI(static_cast<unsigned int>(i))->getDataTypeID();
    if(CIEC_ANY::e_ANY != typeToCheck && CIEC_ANY::e_STRING != typeToCheck && CIEC_ANY::e_WSTRING != typeToCheck) {
      DEVLOG_ERROR("[HTTP Layer] Client called %s has an invalid SD_%d\n", CStringDictionary::getInstance().get(m_poFb->getInstanceNameId()), i);
      return false;
    }
  }

  for(size_t i = 2; i < m_poFb->getNumRD(); i++) {
    CIEC_ANY::EDataTypeID typeToCheck = m_poFb->getDO(static_cast<unsigned int>(i))->getDataTypeID();
    if(CIEC_ANY::e_ANY != typeToCheck && CIEC_ANY::e_STRING != typeToCheck && CIEC_ANY::e_WSTRING != typeToCheck) {
      DEVLOG_ERROR("[HTTP Layer] Client called %s has an invalid RD_%d\n", CStringDictionary::getInstance().get(m_poFb->getInstanceNameId()), i);
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
      mContentType = paParser[2];
    } else {
      mContentType = "text/html";
    }
    everythingOK = storeRequestType(paParser[1]);
  } else {
    everythingOK = false;
    DEVLOG_ERROR("[HTTP Layer] Wrong parameters. It should be as IP:PORT;POST|PUT|GET;[content-type]\n");
  }
  return everythingOK;
}

bool CHttpComLayer::handleAddress(const char* paAddress, size_t paNoOfSDs) {
  bool everythingOK = true;

  //look for parameters
  CParameterParser parser(paAddress, '?', 2); //IP:PORT/PATH?PARAMETERS
  CIEC_STRING addressToParse = paAddress;
  if(2 == parser.parseParameters() && (e_PUT == mRequestType || e_POST == mRequestType)) {
    if(0 == paNoOfSDs) { //if SDs are present, the parameters in PARAMS are ignored
      mReqData = parser[1];
      mContentType = "application/x-www-form-urlencoded";
    }
    addressToParse = parser[0];
  }

  char* firstSlash = strchr(addressToParse.getValue(), '/');

  if(firstSlash) {
    mPath = firstSlash;
    *firstSlash = '\0';
    CParameterParser portParser(addressToParse.getValue(), ':', 2);
    if(2 == portParser.parseParameters()) {
      mHost = portParser[0];
      mPort = static_cast<TForteUInt16>(forte::core::util::strtoul(portParser[1], 0, 10));
    } else {
      mHost = addressToParse.getValue();
      DEVLOG_INFO("[HTTP Layer] No port was found on the parameter, using default 80\n");
    }
  } else {
    everythingOK = false;
    DEVLOG_ERROR("[HTTP Layer] No path was found on the parameter\n");
  }
  return everythingOK;
}

EComResponse CHttpComLayer::sendData(void *paData, unsigned int) {
  mInterruptResp = e_Nothing;
  if(mCorrectlyInitialized) {
    switch(m_poFb->getComServiceType()){
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

void CHttpComLayer::sendDataAsServer(const void *paData) {
  bool error = false;
  TConstIEC_ANYPtr apoSDs = static_cast<TConstIEC_ANYPtr>(paData);
  if(!serializeData(apoSDs[0])) {
    error = true;
  } else {
    CHttpParser::createResponse(mRequest, "HTTP/1.1 200 OK", mContentType, mReqData);
  }
  if(error) {
    getExtEvHandler<CHTTP_Handler>().forceClose(this);
    mInterruptResp = e_ProcessDataDataTypeError;
  } else {
    getExtEvHandler<CHTTP_Handler>().sendServerAnswer(this, mRequest);
    mInterruptResp = e_ProcessDataOk;
  }
}

void CHttpComLayer::sendDataAsClient(const void *paData) {
  bool error = false;
  if(mHasParameterInSD) {
    TConstIEC_ANYPtr apoSDs = static_cast<TConstIEC_ANYPtr>(paData);
    if(!serializeData(apoSDs[0])) {
      error = true;
      DEVLOG_ERROR("[HTTP Layer] Error in data serialization\n");
    } else {
      if(!CHttpParser::changePutPostData(mRequest, mReqData)) {
        DEVLOG_ERROR("[HTTP Layer] Wrong PUT/POST request when changing the data\n");
        error = true;
      }
    }
  }

  if(!error) {
    if(getExtEvHandler<CHTTP_Handler>().sendClientData(this, mRequest)) {
      mInterruptResp = e_ProcessDataOk;
    } else {
      mInterruptResp = e_ProcessDataSendFailed;
      DEVLOG_ERROR("[HTTP Layer] Sending request on TCP failed\n");
    }
  } else {
    mInterruptResp = e_ProcessDataDataTypeError;
  }
}

EComResponse CHttpComLayer::recvData(const void *paData, unsigned int paSize) {
  mInterruptResp = e_Nothing;
  if(mCorrectlyInitialized) {
    memcpy(mRecvBuffer, paData, (paSize > cg_unIPLayerRecvBufferSize) ? cg_unIPLayerRecvBufferSize : paSize);
    switch(m_poFb->getComServiceType()){
      case e_Server:
        DEVLOG_ERROR("[HTTP Layer] Receiving raw data as a Server? That's wrong, use the recvServerData function\n");
        break;
      case e_Client:
        if(0 == paData) { //timeout occurred
          mInterruptResp = e_ProcessDataRecvFaild;
        } else {
          if(e_ProcessDataOk != (mInterruptResp = handleHTTPResponse(mRecvBuffer))) {
            DEVLOG_ERROR("[HTTP Layer] FB with host: %s:%u couldn't handle the HTTP response\n", mHost.getValue(), mPort);
          } else {
            //TODO Trigger event?
          }
        }
        break;
      default:
        break;
    }
  } else {
    DEVLOG_ERROR("[HTTP Layer]The FB is not initialized\n");
  }
  if(e_ProcessDataOk == mInterruptResp) {
    m_poFb->interruptCommFB(this);
  }
  return mInterruptResp;
}

EComResponse forte::com_infra::CHttpComLayer::recvServerData(CSinglyLinkedList<CIEC_STRING>&, CSinglyLinkedList<CIEC_STRING>& paParameterValues) {
  //for now, the parameterNames are not taken in account, and the parameters are put in the same order they arrived

  mInterruptResp = e_Nothing;
  bool failed = false;
  if(0 < m_poFb->getNumSD()) {
    unsigned int noOfParameters = 0;
    for(CSinglyLinkedList<CIEC_STRING>::Iterator iter = paParameterValues.begin(); iter != paParameterValues.end(); ++iter) {
      noOfParameters++;
    }

    if(noOfParameters == m_poFb->getNumRD()) {
      noOfParameters = 0;
      for(CSinglyLinkedList<CIEC_STRING>::Iterator iter = paParameterValues.begin(); iter != paParameterValues.end(); ++iter) {
        m_poFb->getRDs()[noOfParameters++].setValue(*iter);
      }
    } else {
      DEVLOG_ERROR("[HTTP Layer] FB with path %s received a number of parameters of %u, while it has %u SDs\n", mPath.getValue(),
        static_cast<TForteUInt16>(noOfParameters), m_poFb->getNumRD());
      failed = true;
    }
  }

  if(failed) {
    CIEC_STRING toSend;
    CIEC_STRING result = "HTTP/1.1 400 Bad Request";
    mReqData = "";
    CHttpParser::createResponse(toSend, result, mContentType, mReqData);
    getExtEvHandler<CHTTP_Handler>().sendServerAnswerFromRecv(this, toSend);
    mInterruptResp = e_ProcessDataDataTypeError;
  } else {
    mInterruptResp = e_ProcessDataOk;
  }
  if(e_ProcessDataOk == mInterruptResp) {
    m_poFb->interruptCommFB(this);
  }
  return mInterruptResp;
}

EComResponse CHttpComLayer::handleHTTPResponse(char *paData) {
  DEVLOG_DEBUG("[HTTP Layer] Handling received HTTP response\n");
  EComResponse eRetVal = e_ProcessDataRecvFaild;
  if(m_poFb != 0) {
    CIEC_ANY* apoRDs = m_poFb->getRDs();
    // Interpret HTTP response and set output status according to success/failure.
    CIEC_STRING responseCode;
    CIEC_STRING output;
    CHttpParser::parseResponse(output, responseCode, paData) ? eRetVal = e_ProcessDataOk : eRetVal = e_ProcessDataRecvFaild;
    apoRDs[0].fromString(responseCode.getValue());
    apoRDs[1].fromString(output.getValue());
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
  if(e_Server == m_poFb->getComServiceType()) {
    getExtEvHandler<CHTTP_Handler>().removeServerPath(mPath);
  }
  getExtEvHandler<CHTTP_Handler>().forceClose(this);
}

bool CHttpComLayer::serializeData(const CIEC_ANY& paCIECData) {
  size_t bufferSize = paCIECData.getToStringBufferSize();
  mReqData.reserve(static_cast<TForteUInt16>(bufferSize));
  char *buffer = mReqData.getValue();
  int nWrittenBytes;
  switch(paCIECData.getDataTypeID()){
    case CIEC_ANY::e_WSTRING:
      nWrittenBytes = static_cast<const CIEC_WSTRING&>(paCIECData).toUTF8(buffer, bufferSize, false);
      break;
    case CIEC_ANY::e_STRING:
      nWrittenBytes = static_cast<const CIEC_STRING&>(paCIECData).toUTF8(buffer, bufferSize, false);
      break;
    default:
      nWrittenBytes = paCIECData.toString(buffer, bufferSize);
  }

  nWrittenBytes = nWrittenBytes > -1 ? nWrittenBytes : 0;
  mReqData.assign(buffer, static_cast<TForteUInt16>(nWrittenBytes));
  return true;
}

CIEC_STRING& forte::com_infra::CHttpComLayer::getHost() {
  return mHost;
}

TForteUInt16 forte::com_infra::CHttpComLayer::getPort() const {
  return mPort;
}

