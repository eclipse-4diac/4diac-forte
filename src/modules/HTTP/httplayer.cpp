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

using namespace forte::com_infra;

#define SEND_TIMEOUT 20

CHttpComLayer::CHttpComLayer(CComLayer* paUpperLayer, CBaseCommFB* paComFB) :
    CComLayer(paUpperLayer, paComFB), mInterruptResp(e_Nothing), mRequestType(e_NOTSET), mBufFillSize(0), mSocketID(CIPComSocketHandler::scm_nInvalidSocketDescriptor), hasOutputResponse(false){
  memset(mRecvBuffer, 0, sizeof(mRecvBuffer));
}

CHttpComLayer::~CHttpComLayer(){
}

EComResponse CHttpComLayer::openConnection(char *paLayerParameter){
  EComResponse eRetVal = e_InitInvalidId;
  switch (m_poFb->getComServiceType()){
    case e_Server:
      // TODO: Currently not implemented.
      eRetVal = e_ProcessDataInvalidObject;
      break;
    case e_Client: {
      char* helperChar = strchr(paLayerParameter, ';'); //look for expected response code
      if(0 != helperChar){
        *helperChar++ = '\0';
        mExpectedRspCode = helperChar;
      }else{
        mExpectedRspCode = "HTTP/1.1 200 OK";
      }
      // Copy params for later use by HTTP parser ()
      helperChar = strchr(paLayerParameter, '/'); //look for path
      if(0 != helperChar){
        *helperChar++ = '\0';
        mPath = helperChar;
        helperChar = strchr(paLayerParameter, ':'); //look for port
        if(0 != helperChar){
          *helperChar++ = '\0';
          mPort = static_cast<TForteUInt16>(forte::core::util::strtoul(helperChar, 0, 10));
          mHost = paLayerParameter;
          unsigned int noOfSD = m_poFb->getNumSD();
          unsigned int noOfRD = m_poFb->getNumRD();

          // Determine request type according to function block data inputs. If the function block has a data input, it sends a PUT request. Otherwise, it sends a GET request.
          if(noOfSD > 0){ //PUT REQUEST
            if(1 == noOfSD){
              mRequestType = e_PUT;
              CHttpParser::createPutRequest(mRequest, mHost, mPath, mReqData);
              if(1 < noOfRD){
                DEVLOG_ERROR("[HTTP Layer] A PUT request with more than one output\n");
              }else{
                if(1 == noOfRD){ //can have 0 RD, so response is ignored
                  hasOutputResponse = true;
                }
                eRetVal = e_InitOk;
                DEVLOG_INFO("[HTTP Layer] FB with PUT request initialized. Host: %s, Path: %s\n", mHost.getValue(), mPath.getValue());
              }
            }
            else{
              DEVLOG_ERROR("[HTTP Layer] Only one SD is possible in HTTP which is used as input for a PUT request\n");
            }
          }
          else{ //GET REQUEST
            if(1 == noOfRD){
              mRequestType = e_GET;
              CHttpParser::createGetRequest(mRequest, mHost, mPath);
              eRetVal = e_InitOk;
              hasOutputResponse = true;
              DEVLOG_INFO("[HTTP Layer] FB with GET request initialized. Host: %s, Path: %s\n", mHost.getValue(), mPath.getValue());
            }else if(0 == noOfRD){
              DEVLOG_ERROR("[HTTP Layer] A GET request without and output doesn't make sense\n");
            }else{
              DEVLOG_ERROR("[HTTP Layer] A GET request with more than one output\n");
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

      m_eConnectionState = e_Disconnected;
      break;
    }
    case e_Publisher:
    case e_Subscriber:
      // HTTP does not use UDP
      eRetVal = e_ProcessDataInvalidObject;
      break;
  }
  return eRetVal;
}

EComResponse CHttpComLayer::sendData(void *paData, unsigned int){
  EComResponse eRetVal = e_ProcessDataOk;
  switch (m_poFb->getComServiceType()){
    case e_Server:
      // TODO: Currently not implemented.
      eRetVal = e_Nothing;
      break;
    case e_Client: {
      if(CIPComSocketHandler::scm_nInvalidSocketDescriptor == mSocketID){
        if(e_PUT == mRequestType){
          TConstIEC_ANYPtr apoSDs = static_cast<TConstIEC_ANYPtr>(paData);
          if(!serializeData(apoSDs[0])){
            eRetVal = e_Nothing; // Serializing failed
            break;
          }
          if(!CHttpParser::changePutData(mRequest, mReqData)){
            DEVLOG_DEBUG("[HTTP Layer] Wrong PUT request when changing the data\n");
          }
        }

        mInterruptResp = e_Nothing; //TODO: Is this needed here?
        if(e_InitOk != openHTTPConnection()){
          mInterruptResp = e_ProcessDataSendFailed;
          DEVLOG_ERROR("[HTTP Layer] Opening HTTP connection failed\n");
        }
        else{
          DEVLOG_DEBUG("[HTTP Layer] Sending request on TCP\n");
          if(0 >= CIPComSocketHandler::sendDataOnTCP(mSocketID, mRequest.getValue(), mRequest.length())){
            mInterruptResp = e_ProcessDataSendFailed;
            DEVLOG_ERROR("[HTTP Layer] Sending request on TCP failed\n");
          }
          else{
            // wait until result is ready
            time_t start = time(0); // for timeout
            time_t endWait = start + SEND_TIMEOUT;
            // TODO update to timeout sempahore when ready
            // TODO: Implement detection of content-length and/or chunk detection in httplayer to break out of loop early
            // TODO: wait for server closing or just receiving?
            // TODO: Support chunked messaged?
            while(e_Nothing == mInterruptResp && start < endWait){
              start = time(0);
              CThread::sleepThread(1);
            }

            if (start > endWait) {
              DEVLOG_ERROR("[HTTP Layer] Response Timeout exceeded\n");
              mInterruptResp = e_ProcessDataRecvFaild;
            }else{
              if(e_ProcessDataRecvFaild != mInterruptResp){
                mInterruptResp = handleHTTPResponse(mRecvBuffer);
                eRetVal = e_ProcessDataOk;
                // Ensure event output is sent (sendData does not trigger e_ProcessDataOk events for clients/servers/subscribers)
                DEVLOG_DEBUG("[HTTP Layer] Interrupting CommFB\n");
                m_poFb->interruptCommFB(this);
                DEVLOG_DEBUG("[HTTP Layer] Forcing CommFB CNF event\n");
                CEventChainExecutionThread *poEventChainExecutor = m_poFb->getEventChainExecutor();
                m_poFb->receiveInputEvent(cg_nExternalEventID, *poEventChainExecutor);
                DEVLOG_DEBUG("[HTTP Layer]  CommFB CNF event executed\n");
              }else{
                DEVLOG_ERROR("[HTTP Layer] Error receiving response\n");
              }
            }

          }
          closeConnection(); // This sets m_eInterruptedResp to e_InitTerminated
        }
      }
      else{
        DEVLOG_ERROR("[HTTP Layer] Socket is still open, probably last called didn't finish yet\n");
      }
      break;
    }
    case e_Publisher:
    case e_Subscriber:
      // HTTP does not use UDP
      eRetVal = e_Nothing;
      break;
  }

  return eRetVal;
}

EComResponse CHttpComLayer::handleHTTPResponse(char *paData){
  DEVLOG_DEBUG("[HTTP Layer] Handling received HTTP response\n");
  EComResponse eRetVal = e_ProcessDataRecvFaild;
  if(0 != strstr(paData, "\r\n\r\n")){ // Verify that at least a body has been received
    if(m_poFb != 0){
      CIEC_ANY* apoRDs = m_poFb->getRDs();
      switch (m_poFb->getComServiceType()){
        case e_Server:
          // TODO: Currently not implemented.
          break;
        case e_Client: {
          // Interpret HTTP response and set output status according to success/failure.
          CIEC_STRING output;
          bool success = true;
          if(e_GET == mRequestType){
            success = CHttpParser::parseGetResponse(output, paData, mExpectedRspCode);
          }
          else if(e_PUT == mRequestType){
            success = CHttpParser::parsePutResponse(output, paData, mExpectedRspCode);
          }
          success ? eRetVal = e_ProcessDataOk : eRetVal = e_ProcessDataRecvFaild;
          // Set data output if possible
          if(hasOutputResponse){
            apoRDs->fromString(output.getValue());
          }
          break;
        }
        case e_Publisher:
        case e_Subscriber:
          // HTTP does not use UDP
          break;
      }
    }
    else{
      DEVLOG_INFO("[HTTP Layer] No FB defined\n");
    }
  }
  else{
    DEVLOG_INFO("[HTTP Layer] Invalid or incomplete HTTP response\n");
  }
  return eRetVal;
}

EComResponse CHttpComLayer::recvData(const void *paData, unsigned int){
  mInterruptResp = e_Nothing;
  switch (m_eConnectionState){
    case e_Listening:
      //TODO Server not yet implemented
      break;
    case e_Connected:
      if(mSocketID == *(static_cast<const CIPComSocketHandler::TSocketDescriptor *>(paData))){
        handledRecvData();
      }
      break;
    case e_ConnectedAndListening:
    case e_Disconnected:
    default:
      break;
  }
  return mInterruptResp;
}

void CHttpComLayer::handledRecvData(){
  if((cg_unIPLayerRecvBufferSize - mBufFillSize) <= 0){
    // If buffer is full, clear and return
    mBufFillSize = 0;
    mInterruptResp = e_ProcessDataRecvFaild;
    DEVLOG_ERROR("[HTTP Layer] Receive buffer full\n");
    return;
  }
  if(CIPComSocketHandler::scm_nInvalidSocketDescriptor != mSocketID){
    int nRetVal = -1;
    switch (m_poFb->getComServiceType()){
      case e_Server:
      case e_Client:
        DEVLOG_DEBUG("[HTTP Layer] Attempting to receive data from TCP\n");
        nRetVal = CIPComSocketHandler::receiveDataFromTCP(mSocketID, &mRecvBuffer[mBufFillSize], cg_unIPLayerRecvBufferSize - mBufFillSize);
        break;
      case e_Publisher:
      case e_Subscriber:
        // Do nothing because HTTP does not use UDP
        break;
    }
    switch (nRetVal){
      case 0:
        DEVLOG_DEBUG("HTTP Layer] Connection closed by peer\n");
        closeConnection();
        mInterruptResp = e_InitTerminated;
        //if(e_Server == m_poFb->getComServiceType()){
          //Move server into listening mode again
         // m_eConnectionState = e_Listening;
        //}
        break;
      case -1:
        mInterruptResp = e_ProcessDataRecvFaild;
        DEVLOG_ERROR("[HTTP Layer] Failed to receive data from TCP\n");
        break;
      default:
        //we successfully received data
        DEVLOG_DEBUG("HTTP Layer] Successfully received data from TCP\n");
        mBufFillSize += nRetVal;
        mInterruptResp = e_ProcessDataOk;
        break;
    }
    //m_poFb->interruptCommFB(this);
  }
}

EComResponse CHttpComLayer::processInterrupt(){
  return mInterruptResp;
}

void CHttpComLayer::closeConnection(){
  closeSocket(&mSocketID);
  mBufFillSize = 0;
}

void CHttpComLayer::closeSocket(CIPComSocketHandler::TSocketDescriptor *paSocketID){
  if(CIPComSocketHandler::scm_nInvalidSocketDescriptor != *paSocketID){
    GET_HANDLER_FROM_COMM_LAYER(CIPComSocketHandler)->removeComCallback(*paSocketID);
    CIPComSocketHandler::closeSocket(*paSocketID);
    *paSocketID = CIPComSocketHandler::scm_nInvalidSocketDescriptor;
    m_eConnectionState = e_Disconnected;
  }
}

EComResponse CHttpComLayer::openHTTPConnection(){
  EComResponse eRetVal = e_InitInvalidId;
  CIPComSocketHandler::TSocketDescriptor nSockDes = CIPComSocketHandler::scm_nInvalidSocketDescriptor;
  m_eConnectionState = e_Connected;

  switch (m_poFb->getComServiceType()){
    case e_Server:
      // TODO: HTTP server has not yet been implemented
      break;
    case e_Client:
      nSockDes = mSocketID = CIPComSocketHandler::openTCPClientConnection(mHost.getValue(), mPort);
      break;
    case e_Publisher:
    case e_Subscriber:
      break;
  }

  if(CIPComSocketHandler::scm_nInvalidSocketDescriptor != nSockDes){
    if(e_Publisher != m_poFb->getComServiceType()){ //Publishers should not be registered for receiving data
      GET_HANDLER_FROM_COMM_LAYER(CIPComSocketHandler)->addComCallback(nSockDes, this);
    }
    eRetVal = e_InitOk;
  }
  else{
    m_eConnectionState = e_Disconnected;
  }
  return eRetVal;
}

bool CHttpComLayer::serializeData(const CIEC_ANY& paCIECData){
  CIEC_STRING string;
  CIEC_ANY::EDataTypeID eDataType = paCIECData.getDataTypeID();
  switch (eDataType){
    case CIEC_ANY::e_BOOL:
      string = static_cast<CIEC_STRING>(BOOL_TO_STRING(static_cast<const CIEC_BOOL&>(paCIECData)));
      break;
    case CIEC_ANY::e_SINT:
      string = static_cast<CIEC_STRING>(SINT_TO_STRING(static_cast<const CIEC_SINT&>(paCIECData)));
      break;
    case CIEC_ANY::e_INT:
      string = static_cast<CIEC_STRING>(INT_TO_STRING(static_cast<const CIEC_INT&>(paCIECData)));
      break;
    case CIEC_ANY::e_DINT:
      string = static_cast<CIEC_STRING>(DINT_TO_STRING(static_cast<const CIEC_DINT&>(paCIECData)));
      break;
    case CIEC_ANY::e_LINT:
      string = static_cast<CIEC_STRING>(LINT_TO_STRING(static_cast<const CIEC_LINT&>(paCIECData)));
      break;
    case CIEC_ANY::e_USINT:
      string = static_cast<CIEC_STRING>(USINT_TO_STRING(static_cast<const CIEC_USINT&>(paCIECData)));
      break;
    case CIEC_ANY::e_UINT:
      string = static_cast<CIEC_STRING>(UINT_TO_STRING(static_cast<const CIEC_UINT&>(paCIECData)));
      break;
    case CIEC_ANY::e_UDINT:
      string = static_cast<CIEC_STRING>(UDINT_TO_STRING(static_cast<const CIEC_UDINT&>(paCIECData)));
      break;
    case CIEC_ANY::e_ULINT:
      string = static_cast<CIEC_STRING>(ULINT_TO_STRING(static_cast<const CIEC_ULINT&>(paCIECData)));
      break;
    case CIEC_ANY::e_BYTE:
      string = static_cast<CIEC_STRING>(BYTE_TO_STRING(static_cast<const CIEC_BYTE&>(paCIECData)));
      break;
    case CIEC_ANY::e_WORD:
      string = static_cast<CIEC_STRING>(WORD_TO_STRING(static_cast<const CIEC_WORD&>(paCIECData)));
      break;
    case CIEC_ANY::e_DWORD:
      string = static_cast<CIEC_STRING>(DWORD_TO_STRING(static_cast<const CIEC_DWORD&>(paCIECData)));
      break;
    case CIEC_ANY::e_LWORD:
      string = static_cast<CIEC_STRING>(LWORD_TO_STRING(static_cast<const CIEC_LWORD&>(paCIECData)));
      break;
    case CIEC_ANY::e_TIME:
      string = static_cast<CIEC_STRING>(TIME_TO_STRING(static_cast<const CIEC_TIME&>(paCIECData)));
      break;
    case CIEC_ANY::e_REAL:
      string = static_cast<CIEC_STRING>(REAL_TO_STRING(static_cast<const CIEC_REAL&>(paCIECData)));
      break;
    case CIEC_ANY::e_LREAL:
      string = static_cast<CIEC_STRING>(LREAL_TO_STRING(static_cast<const CIEC_LREAL&>(paCIECData)));
      break;
    case CIEC_ANY::e_WSTRING:
      string = static_cast<CIEC_STRING>(WSTRING_TO_STRING(static_cast<const CIEC_WSTRING&>(paCIECData)));
      break;
    case CIEC_ANY::e_STRING:
      string = static_cast<const CIEC_STRING&>(paCIECData);
      break;
    default:
      return false;
  }
  mReqData = string;
  return true;
}
