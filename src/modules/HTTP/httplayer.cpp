/*******************************************************************************
*
********************************************************************************
*******************************************************************************
* Copyright (c) 2017-2018 Marc Jakobi, github.com/MrcJkb
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
*
* Contributors:
*    Marc Jakobi - initial implementation for HTTP clients
*******************************************************************************
******************************************************************************
*
*
********************************************************************************
* HTTP Com Layer
********************************************************************************/
#include "httplayer.h"
#include "httpiplayer.h"
#include "../../arch/devlog.h"
#include "commfb.h"
#include "../../core/datatypes/forte_dint.h"
#include <stdio.h>
#include <string.h>

using namespace forte::com_infra;

CHttpComLayer::CHttpComLayer(CComLayer* paUpperLayer, CBaseCommFB* paComFB) :
  CComLayer(paUpperLayer, paComFB),
  mHttpParser(CHttpParser()),
  mRequestType(e_NOTSET) {
}

CHttpComLayer::~CHttpComLayer(){
  delete m_poBottomLayer;
  m_poBottomLayer = 0;
}

void CHttpComLayer::closeConnection(){
  DEVLOG_INFO("Connection closed by FB\n");
  m_eConnectionState = e_Disconnected;
  if (0 != m_poBottomLayer) {
    m_poBottomLayer->closeConnection();
  }
}

EComResponse forte::com_infra::CHttpComLayer::openConnection(char *paLayerParameter){
  EComResponse eRetVal = e_InitInvalidId;
  if (0 != strchr(paLayerParameter, ';')) {
    strtok(paLayerParameter, ";");
    char* expectedRspCode = strtok(0, ";");
    mHttpParser.setExpectedRspCode(expectedRspCode);
  }
  switch (m_poFb->getComServiceType()) {
  case e_Server:
    // TODO: Currently not implemented.
    eRetVal = e_ProcessDataInvalidObject;
    break;
  case e_Client:
  {
    // Determine request type according to function block data inputs. If the function block has a data input,
    // it sends a PUT request. Otherwise, it sends a GET request.
    eRetVal = e_InitOk;
    if (m_poFb->getNumSD() > 0) {
      mRequestType = e_PUT;
    }
    else {
      mRequestType = e_GET;
    }
    if (m_poBottomLayer == 0) {
      m_poBottomLayer = new CHttpIPComLayer(this, m_poFb);
      if (m_poBottomLayer == 0) {
        eRetVal = e_InitInvalidId;
        return eRetVal;
      }
    }
    char ipParams[kAllocSize]; // address + port for CHttpIPComLayer
    // Copy params for later use by HTTP parser ()
    mParams[0] = 0;
    strncpy(mParams, paLayerParameter, strlen(paLayerParameter) + 1);
    sscanf(paLayerParameter, "%99[^/]", ipParams); // Extract address & port for IP com layer
    m_poBottomLayer->openConnection(ipParams);
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
    case e_Client:
    {
      // Send HTTP request
      char request[kAllocSize];
      if (e_GET == mRequestType) {
        mHttpParser.createGetRequest(request, mParams);
      }
      else if (e_PUT == mRequestType) {
        TConstIEC_ANYPtr apoSDs = static_cast<TConstIEC_ANYPtr > (paData);
        if (!serializeData(apoSDs[0])) {
          return e_Nothing; // Serializing failed
        }
        mHttpParser.createPutRequest(request, mParams, mReqData);
      }
      eRetVal = m_poBottomLayer->sendData(request, strlen(request) + 1);
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

EComResponse CHttpComLayer::recvData(const void *paData, unsigned int){
  DEVLOG_DEBUG("Handling received HTTP response\n");
  EComResponse eRetVal = e_ProcessDataSendFailed;
  char* recvData = (char*) paData;
  if (0 != strstr(recvData, "\r\n\r\n")) { // Verify that at least a body has been received
    if (m_poFb != 0) {
      CIEC_ANY* apoRDs = m_poFb->getRDs();
      switch (m_poFb->getComServiceType()) {
      case e_Server:
        // TODO: Currently not implemented.
        break;
      case e_Client:
      {
        // Interpret HTTP response and set output status according to success/failure.
        char output[cg_unIPLayerRecvBufferSize];
        bool success = true;
        if (e_GET == mRequestType) {
          success = mHttpParser.parseGetResponse(output, recvData);
        }
        else if (e_PUT == mRequestType) {
          success = mHttpParser.parsePutResponse(output, recvData);
        }
        if (success) {
          eRetVal = e_ProcessDataOk;
        }
        else {
          eRetVal = e_ProcessDataSendFailed;
        }
        // Set data output if possible
        if (m_poFb->getNumRD() > 0) {
          apoRDs->fromString(output);
        }
        break;
      }
      case e_Publisher:
      case e_Subscriber:
        // HTTP does not use UDP
        break;
      }
    }
  }
  else {
    DEVLOG_INFO("Invalid or incomplete HTTP response\n");
  }
  return eRetVal;
}

bool CHttpComLayer::serializeData(const CIEC_ANY& paCIECData) {
  CIEC_STRING string;
  CIEC_ANY::EDataTypeID eDataType = paCIECData.getDataTypeID();
  switch (eDataType) {
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
  default:
    return false;
  }
  const char* data = string.getValue();
  strncpy(mReqData, data, strlen(data) + 1);
  return true;
}
