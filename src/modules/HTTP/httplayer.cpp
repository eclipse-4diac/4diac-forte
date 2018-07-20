/*******************************************************************************
 * Copyright (c) 2017-2018 Marc Jakobi, github.com/MrcJkb, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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
    CComLayer(paUpperLayer, paComFB), mInterruptResp(e_Nothing), mRequestType(e_NOTSET), mPort(0), mBufFillSize(0), hasOutputResponse(false), mCorrectlyInitialized(false), mHasParameter(false){
  memset(mRecvBuffer, 0, sizeof(mRecvBuffer));
}

CHttpComLayer::~CHttpComLayer(){
  closeConnection();
}

EComResponse CHttpComLayer::openConnection(char *paLayerParameter){
  EComResponse eRetVal = e_InitInvalidId;
  unsigned int noOfSD = m_poFb->getNumSD();
  unsigned int noOfRD = m_poFb->getNumRD();

  //TODO: Check that all connected types to SD and RD are string

  switch (m_poFb->getComServiceType()){
    case e_Server:
      if(1 == noOfSD){
        CIEC_STRING path = paLayerParameter;
        GET_HANDLER_FROM_COMM_LAYER(CHTTP_Handler)->addServerPath(this, path);
        eRetVal = e_InitOk;
      }
      else{
        DEVLOG_ERROR("[HTTP Layer] The FB with PARAM %s coudln't be initialized since only one SD is possible in HTTP Server which is for the response\n");
      }
      break;
    case e_Client: {
      bool error = false;
      bool defaultReponse = true;
      bool defaultContent = true;
      char* helperChar = strchr(paLayerParameter, ';'); //look for type of request
      if(0 != helperChar){
        *helperChar = '\0';
        helperChar++;
        char* startOfContentType = strchr(helperChar, ';'); //look for content type
        if(0 != startOfContentType){
          *startOfContentType++ = '\0';
          char* startOfResponse = strchr(startOfContentType, ';');
          if(0 != startOfResponse){
            *startOfResponse++ = '\0';
            mExpectedRspCode = startOfResponse;
            defaultReponse = false;
          }
          mContentType = startOfContentType;
          defaultContent = false;
        }
        if(0 == strcmp(helperChar, "POST")){
          mRequestType = e_POST;
        }else if(0 == strcmp(helperChar, "PUT")){
          mRequestType = e_PUT;
        }else if(0 == strcmp(helperChar, "GET")){
          mRequestType = e_GET;
        }else{
          error = true;
          DEVLOG_ERROR("[HTTP Layer] GET, PUT or POST must be defined, but %s was defined instead\n", helperChar);
        }
      }else{
        error = true;
        DEVLOG_ERROR("[HTTP Layer] GET, PUT or POST must be defined, but none of them was defined\n");
      }

      if(!error){
        if(defaultReponse){
          mExpectedRspCode = "HTTP/1.1 200 OK";
        }
        if(defaultContent){
          mContentType = "text/html";
        }

        helperChar = strchr(paLayerParameter, '?'); //look for parameters in path
        if(0 != helperChar && (e_PUT == mRequestType || e_POST == mRequestType)){
          *helperChar++ = '\0';
          mReqData = helperChar;
          mHasParameter = true;
          mContentType = "application/x-www-form-urlencoded";
        }

        helperChar = strchr(paLayerParameter, '/'); //look for path
        if(0 != helperChar){
          *helperChar++ = '\0';
          mPath = helperChar;
          helperChar = strchr(paLayerParameter, ':'); //look for port
          if(0 != helperChar){
            *helperChar++ = '\0';
            mPort = static_cast<TForteUInt16>(forte::core::util::strtoul(helperChar, 0, 10));
            mHost = paLayerParameter;

            switch (mRequestType){
              case e_PUT:
              case e_POST: {
                if("" != mReqData && 0 != noOfSD){
                  DEVLOG_WARNING("[HTTP Layer] Parameters in PARAMS are used for PUT/POST request data instead of the SDs\n");
                }else if("" == mReqData && 1 != noOfSD){
                  DEVLOG_ERROR("[HTTP Layer] You are using a POST/PUT FB but no data is defined as SD nor as parameters in PARAMS\n");
                  break;
                }

                if(1 < noOfRD){
                  DEVLOG_ERROR("[HTTP Layer] A PUT/POST request with more than one output\n");
                  break;
                }else{
                  if(1 == noOfRD){ //can have 0 RD, so response is ignored
                    hasOutputResponse = true;
                  }
                  CHttpParser::createPutPostRequest(mRequest, mHost, mPath, mReqData, mContentType, mRequestType);
                  eRetVal = e_InitOk;
                  DEVLOG_INFO("[HTTP Layer] FB with PUT/POST request initialized. Host: %s, Path: %s\n", mHost.getValue(), mPath.getValue());
                }
                break;
              }
              case e_GET: {
                if(1 == noOfRD){
                  CHttpParser::createGetRequest(mRequest, mHost, mPath);
                  eRetVal = e_InitOk;
                  hasOutputResponse = true;
                  DEVLOG_INFO("[HTTP Layer] FB with GET request initialized. Host: %s, Path: %s\n", mHost.getValue(), mPath.getValue());
                }else if(0 == noOfRD){
                  DEVLOG_ERROR("[HTTP Layer] A GET request without and output doesn't make sense\n");
                }
                else{
                  DEVLOG_ERROR("[HTTP Layer] A GET request with more than one output\n");
                }
              }
            }
          }
          else{
            DEVLOG_ERROR("[HTTP Layer] No port was found on the parameter\n");
          }
        }
        else{
          DEVLOG_ERROR("[HTTP Layer] No path was found on the parameter\n");
        }
      }else{
        DEVLOG_ERROR("[HTTP Layer] Wrong PARAM value\n");
      }

      m_eConnectionState = e_Disconnected;
      break;
    }
    case e_Publisher:
    case e_Subscriber:
      // HTTP does not use UDP
      eRetVal = e_ProcessDataInvalidObject;
      break;
  }
  mCorrectlyInitialized = (eRetVal == e_InitOk);
  return eRetVal;
}

EComResponse CHttpComLayer::sendData(void *paData, unsigned int){
  mInterruptResp = e_Nothing;
  if(mCorrectlyInitialized){
    bool error = false;
    switch (m_poFb->getComServiceType()){
      case e_Server:{
        TConstIEC_ANYPtr apoSDs = static_cast<TConstIEC_ANYPtr>(paData);
        if(!serializeData(apoSDs[0])){
          error = true;
        }else{
          if(!CHttpParser::createReponse(mRequest, "200 OK", mContentType, mReqData)){
            DEVLOG_DEBUG("[HTTP Layer] Wrong Response request when changing the data\n");
            error = true;
          }
        }
        if(error){
          GET_HANDLER_FROM_COMM_LAYER(CHTTP_Handler)->forceClose(this);
          mInterruptResp = e_ProcessDataDataTypeError;
        }else{
          GET_HANDLER_FROM_COMM_LAYER(CHTTP_Handler)->sendServerAnswer(this, mRequest);
          mInterruptResp = e_ProcessDataOk;
        }
        break;
      }
      case e_Client: {
        if(!mHasParameter && (e_PUT == mRequestType || e_POST == mRequestType)){
          TConstIEC_ANYPtr apoSDs = static_cast<TConstIEC_ANYPtr>(paData);
          if(!serializeData(apoSDs[0])){
            error = true;
            DEVLOG_ERROR("[HTTP Layer] Error in data serialization\n");
          }else{
            if(!CHttpParser::changePutPostData(mRequest, mReqData)){
              DEVLOG_ERROR("[HTTP Layer] Wrong PUT/POST request when changing the data\n");
              error = true;
            }
          }
        }

        if(!error){
          if(GET_HANDLER_FROM_COMM_LAYER(CHTTP_Handler)->sendClientData(this, mRequest)){
            mInterruptResp = e_ProcessDataOk;
          }else{
            mInterruptResp = e_ProcessDataSendFailed;
            DEVLOG_ERROR("[HTTP Layer] Sending request on TCP failed\n");
          }
        }else{
          mInterruptResp = e_ProcessDataDataTypeError;
        }
        break;
      }
      case e_Publisher:
      case e_Subscriber:
        // HTTP does not use UDP
        break;
    }
  }else{
    DEVLOG_ERROR("[HTTP Layer]The FB is not initialized\n");
  }

  return mInterruptResp;
}

EComResponse CHttpComLayer::recvData(const void *paData, unsigned int paSize){
  mInterruptResp = e_Nothing;
  if(mCorrectlyInitialized){
    memcpy(mRecvBuffer, paData, (paSize > cg_unIPLayerRecvBufferSize) ? cg_unIPLayerRecvBufferSize : paSize);
    switch (m_poFb->getComServiceType()){
      case e_Server:
        DEVLOG_ERROR("[HTTP Layer] Receiving raw data as a Serve? That's wrong, use the recvServerData function\n");
        break;
      case e_Client:
        if(0 == paData){ //timeout occurred
          mInterruptResp = e_ProcessDataRecvFaild;
        }else{
          if(e_ProcessDataOk != (mInterruptResp = handleHTTPResponse(mRecvBuffer))){
            DEVLOG_ERROR("[HTTP Layer] FB with host: %s:%u couldn't handle the HTTP response\n", mHost.getValue(), mPort);
          }else{
            //TODO Trigger event?
          }
        }
        break;
      case e_Publisher:
      case e_Subscriber:
      default:
        break;
    }
  }else{
    DEVLOG_ERROR("[HTTP Layer]The FB is not initialized\n");
  }
  if(e_ProcessDataOk == mInterruptResp){
    m_poFb->interruptCommFB(this);
  }
  return mInterruptResp;
}

EComResponse forte::com_infra::CHttpComLayer::recvServerData(CSinglyLinkedList<CIEC_STRING>& paParameterNames, CSinglyLinkedList<CIEC_STRING>& paParameterValues){
  mInterruptResp = e_Nothing;
  bool failed = false;
  if(0 < m_poFb->getNumSD()){
    unsigned int noOfParameters = 0;
    for(CSinglyLinkedList<CIEC_STRING>::Iterator iter = paParameterNames.begin(); iter != paParameterNames.end(); ++iter){
      noOfParameters++;
    }

    if(noOfParameters == m_poFb->getNumSD()){
      noOfParameters = 0;
      for(CSinglyLinkedList<CIEC_STRING>::Iterator iter = paParameterValues.begin(); iter != paParameterValues.end(); ++iter){
        m_poFb->getSDs()[noOfParameters].setValue((*iter));
      }
      //TODO: How do we handle the names? For now the parameters are put in the same order they arrived
    }else{
      DEVLOG_ERROR("[HTTP Layer] FB with path %s received a number of parameters of %u, while it has %u SDs\n", mPath.getValue(), static_cast<TForteUInt16>(noOfParameters), m_poFb->getNumSD());
      failed = true;
    }
  }

  if(failed){
    CIEC_STRING toSend;
    CIEC_STRING result = "400 Bad Request";
    mReqData = "";
    if(!CHttpParser::createReponse(toSend, result, mContentType, mReqData)){
      DEVLOG_DEBUG("[HTTP Layer] Wrong Response request when changing the data\n");
      GET_HANDLER_FROM_COMM_LAYER(CHTTP_Handler)->forceCloseFromRecv(this);
    }
    else{
      GET_HANDLER_FROM_COMM_LAYER(CHTTP_Handler)->sendServerAnswerFromRecv(this, toSend);
    }
    mInterruptResp = e_ProcessDataDataTypeError;
  }
  else{
    mInterruptResp = e_ProcessDataOk;
  }
  if(e_ProcessDataOk == mInterruptResp){
    m_poFb->interruptCommFB(this);
  }
  return mInterruptResp;
}

EComResponse CHttpComLayer::handleHTTPResponse(char *paData){
  DEVLOG_DEBUG("[HTTP Layer] Handling received HTTP response\n");
  EComResponse eRetVal = e_ProcessDataRecvFaild;
  if(0 != strstr(paData, "\r\n\r\n")){ // Verify that at least a body has been received
    if(m_poFb != 0){
      CIEC_ANY* apoRDs = m_poFb->getRDs();
      // Interpret HTTP response and set output status according to success/failure.
      CIEC_STRING output;
      bool success = true;
      if(e_GET == mRequestType){
        success = CHttpParser::parseGetResponse(output, paData, mExpectedRspCode);
      }
      else{
        success = CHttpParser::parsePutPostResponse(output, paData, mExpectedRspCode);
      }
      success ? eRetVal = e_ProcessDataOk : eRetVal = e_ProcessDataRecvFaild;
      // Set data output if possible
      if(success && hasOutputResponse){
        apoRDs->fromString(output.getValue());
      }
    }
    else{
      DEVLOG_ERROR("[HTTP Layer] No FB defined\n");
    }
  }
  else{
    DEVLOG_ERROR("[HTTP Layer] Invalid or incomplete HTTP response\n");
  }
  return eRetVal;
}

EComResponse CHttpComLayer::processInterrupt(){
  mInterruptResp = e_ProcessDataOk;
  return mInterruptResp;
}

void CHttpComLayer::closeConnection(){
  switch (m_poFb->getComServiceType()){
    case e_Server:
      GET_HANDLER_FROM_COMM_LAYER(CHTTP_Handler)->removeServerPath(mPath);
      break;
  }
  GET_HANDLER_FROM_COMM_LAYER(CHTTP_Handler)->forceClose(this);
}

bool CHttpComLayer::serializeData(const CIEC_ANY& paCIECData){
  int bufferSize = paCIECData.getToStringBufferSize();
  char acDataValue[bufferSize];
  int nConsumedBytes;
  switch (paCIECData.getDataTypeID()){
    case CIEC_ANY::e_WSTRING:
      nConsumedBytes = static_cast<const CIEC_WSTRING&>(paCIECData).toUTF8(acDataValue, bufferSize, false);
      break;
    case CIEC_ANY::e_STRING:
      nConsumedBytes = static_cast<const CIEC_STRING&>(paCIECData).toUTF8(acDataValue, bufferSize, false);
      break;
    default:
      nConsumedBytes = paCIECData.toString(acDataValue, bufferSize);
      break;
  }
  if(-1 != nConsumedBytes){
    acDataValue[nConsumedBytes] = '\0';
  }
  mReqData = acDataValue;
  return true;
}

CIEC_STRING& forte::com_infra::CHttpComLayer::getHost(){
  return mHost;
}

TForteUInt16 forte::com_infra::CHttpComLayer::getPort(){
  return mPort;
}

