/*******************************************************************************
 * Copyright (c) 2012 -2014 AIT, ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Patrick Smejkal, Alois Zoitl, Martin Melik-Merkumians - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "modbuslayer.h"
#include "commfb.h"
#include "modbusclientconnection.h"

using namespace forte::com_infra;

CModbusComLayer::CModbusComLayer(CComLayer* pa_poUpperLayer, CBaseCommFB* pa_poComFB) :
    CComLayer(pa_poUpperLayer, pa_poComFB), m_pModbusConnection(0), m_unBufFillSize(0){
  m_eConnectionState = e_Disconnected;
}

CModbusComLayer::~CModbusComLayer(){
}

EComResponse CModbusComLayer::sendData(void *pa_pvData, unsigned int pa_unSize){
  EComResponse eRetVal = e_ProcessDataOk;

  if(m_eConnectionState == e_Connected){
    switch (m_poFb->getComServiceType()){
      case e_Server:
        // todo
        break;
      case e_Client: {
        TForteUInt16 *convertedData = new TForteUInt16[pa_unSize * 4];
        unsigned int sendLength = convertDataInput(pa_pvData, pa_unSize, convertedData);
        if(sendLength > 0){
          m_pModbusConnection->writeData(convertedData, sendLength);
        }
        delete[] convertedData;
        break;
      }
      case e_Publisher:
        // todo
        break;
      case e_Subscriber:
        //do nothing as subscribers do not send data
        break;
    }
  }
  return eRetVal;
}

unsigned int CModbusComLayer::convertDataInput(void *pa_poInData, unsigned int pa_nDataSize, TForteUInt16 *pa_poConvertedData){
  unsigned int outLength = 0;

  CIEC_ANY *apoSDs = static_cast<CIEC_ANY*>(pa_poInData);
  unsigned int nrSDs = pa_nDataSize;

  for(unsigned int i = 0; i < nrSDs; i++){
    CIEC_ANY *anyVal = &apoSDs[i];
    switch (apoSDs[i].getDataTypeID()){
      case CIEC_ANY::e_BOOL: // 8bit data types
      {
        TForteUInt16 out = (bool) *(CIEC_BOOL*) anyVal;
        *(TForteUInt16*) (&pa_poConvertedData[outLength]) = out;
        outLength += sizeof(TForteUInt16) / 2;
        break;
      }
      case CIEC_ANY::e_SINT: {
        TForteInt16 out = (TForteInt8) *(CIEC_SINT*) anyVal;
        *(TForteInt16*) (&pa_poConvertedData[outLength]) = out;
        outLength += sizeof(TForteInt16) / 2;
        break;
      }
      case CIEC_ANY::e_USINT: {
        TForteUInt16 out = (TForteUInt8) *(CIEC_USINT*) anyVal;
        *(TForteUInt16*) (&pa_poConvertedData[outLength]) = out;
        outLength += sizeof(TForteUInt16) / 2;
        break;
      }
      case CIEC_ANY::e_BYTE: {
        TForteUInt16 out = (TForteByte) *(CIEC_BYTE*) anyVal;
        *(TForteUInt16*) (&pa_poConvertedData[outLength]) = out;
        outLength += sizeof(TForteUInt16) / 2;
        break;
      }
      case CIEC_ANY::e_INT: // 16bit data types
      {
        TForteInt16 out = (TForteInt16) *(CIEC_INT*) anyVal;
        *(TForteInt16*) (&pa_poConvertedData[outLength]) = out;
        outLength += sizeof(TForteInt16) / 2;
        break;
      }
      case CIEC_ANY::e_UINT: {
        TForteUInt16 out = (TForteUInt16) *(CIEC_UINT*) anyVal;
        *(TForteUInt16*) (&pa_poConvertedData[outLength]) = out;
        outLength += sizeof(TForteUInt16) / 2;
        break;
      }
      case CIEC_ANY::e_WORD: {
        TForteWord out = (TForteWord) *(CIEC_WORD*) anyVal;
        *(TForteWord*) (&pa_poConvertedData[outLength]) = out;
        outLength += sizeof(TForteWord) / 2;
        break;
      }
      case CIEC_ANY::e_DINT: // 32bit data types
      {
        TForteInt32 out = (TForteInt32) *static_cast<CIEC_DINT*>(anyVal);
        *(TForteInt32*) (&pa_poConvertedData[outLength]) = convertFBOutput<TForteInt32>((TForteByte*) &out, sizeof(TForteInt32));
        outLength += sizeof(TForteInt32) / 2;
        break;
      }
      case CIEC_ANY::e_UDINT: {
        TForteUInt32 out = (TForteUInt32) *(CIEC_UDINT*) anyVal;
        *(TForteUInt32*) (&pa_poConvertedData[outLength]) = convertFBOutput<TForteUInt32>((TForteByte*) &out, sizeof(TForteUInt32));
        outLength += sizeof(TForteUInt32) / 2;
        break;
      }
      case CIEC_ANY::e_DWORD: {
        TForteDWord out = (TForteDWord) *(CIEC_DWORD*) anyVal;
        *(TForteDWord*) (&pa_poConvertedData[outLength]) = convertFBOutput<TForteDWord>((TForteByte*) &out, sizeof(TForteDWord));
        outLength += sizeof(TForteDWord) / 2;
        break;
      }
      case CIEC_ANY::e_REAL: {
        TForteFloat out = (TForteFloat) *(CIEC_REAL*) anyVal;
        *(TForteFloat*) (&pa_poConvertedData[outLength]) = convertFBOutput<TForteFloat>((TForteByte*) &out, sizeof(TForteFloat));
        outLength += sizeof(TForteFloat) / 2;
        break;
      }
      case CIEC_ANY::e_LINT: // 64bit data types
      {
        TForteInt64 out = (TForteInt64) *(CIEC_LINT*) anyVal;
        *(TForteInt64*) (&pa_poConvertedData[outLength]) = convertFBOutput<TForteInt64>((TForteByte*) &out, sizeof(TForteInt64));
        outLength += sizeof(TForteInt64) / 2;
        break;
      }
      case CIEC_ANY::e_ULINT: {
        TForteUInt64 out = (TForteUInt64) *(CIEC_ULINT*) anyVal;
        *(TForteUInt64*) (&pa_poConvertedData[outLength]) = convertFBOutput<TForteUInt64>((TForteByte*) &out, sizeof(TForteUInt64));
        outLength += sizeof(TForteUInt64) / 2;
        break;
      }
      case CIEC_ANY::e_LWORD: {
        TForteLWord out = (TForteLWord) *(CIEC_LWORD*) anyVal;
        *(TForteLWord*) (&pa_poConvertedData[outLength]) = convertFBOutput<TForteLWord>((TForteByte*) &out, sizeof(TForteLWord));
        outLength += sizeof(TForteLWord) / 2;
        break;
      }
      case CIEC_ANY::e_LREAL: {
        TForteDFloat out = (TForteDFloat) *(CIEC_LREAL*) anyVal;
        *(TForteDFloat*) (&pa_poConvertedData[outLength]) = convertFBOutput<TForteDFloat>((TForteByte*) &out, sizeof(TForteDFloat));
        outLength += sizeof(TForteDFloat) / 2;
        break;
      }
      default:
        break;
    }
  }
  return outLength;
}

EComResponse CModbusComLayer::processInterrupt(){
  if(e_ProcessDataOk == m_eInterruptResp){
    switch (m_eConnectionState){
      case e_Connected: {
        CIEC_ANY *apoRDs = m_poFb->getRDs();
        unsigned int nrRDs = m_poFb->getNumRD();

        unsigned int dataIndex = 0;

        for(unsigned int i = 0; i < nrRDs; i++){
          switch (apoRDs[i].getDataTypeID()){
            case CIEC_ANY::e_BOOL:
              apoRDs[i].setValue(CIEC_BOOL(convertFBOutput<bool>(&m_acRecvBuffer[dataIndex], m_unBufFillSize - dataIndex)));
              dataIndex += sizeof(bool);
              break;
            case CIEC_ANY::e_SINT:
              apoRDs[i].setValue(CIEC_SINT(convertFBOutput<TForteInt8>(&m_acRecvBuffer[dataIndex], m_unBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteInt8);
              break;
            case CIEC_ANY::e_INT:
              apoRDs[i].setValue(CIEC_INT(convertFBOutput<TForteInt16>(&m_acRecvBuffer[dataIndex], m_unBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteInt16);
              break;
            case CIEC_ANY::e_DINT:
              apoRDs[i].setValue(CIEC_DINT(convertFBOutput<TForteInt32>(&m_acRecvBuffer[dataIndex], m_unBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteInt32);
              break;
            case CIEC_ANY::e_LINT:
              apoRDs[i].setValue(CIEC_LINT(convertFBOutput<TForteInt64>(&m_acRecvBuffer[dataIndex], m_unBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteInt64);
              break;
            case CIEC_ANY::e_USINT:
              apoRDs[i].setValue(CIEC_USINT(convertFBOutput<TForteUInt8>(&m_acRecvBuffer[dataIndex], m_unBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteUInt8);
              break;
            case CIEC_ANY::e_UINT:
              apoRDs[i].setValue(CIEC_UINT(convertFBOutput<TForteUInt16>(&m_acRecvBuffer[dataIndex], m_unBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteUInt16);
              break;
            case CIEC_ANY::e_UDINT:
              apoRDs[i].setValue(CIEC_UDINT(convertFBOutput<TForteUInt32>(&m_acRecvBuffer[dataIndex], m_unBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteUInt32);
              break;
            case CIEC_ANY::e_ULINT:
              apoRDs[i].setValue(CIEC_ULINT(convertFBOutput<TForteUInt64>(&m_acRecvBuffer[dataIndex], m_unBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteUInt64);
              break;
            case CIEC_ANY::e_BYTE:
              apoRDs[i].setValue(CIEC_BYTE(convertFBOutput<TForteByte>(&m_acRecvBuffer[dataIndex], m_unBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteByte);
              break;
            case CIEC_ANY::e_WORD:
              apoRDs[i].setValue(CIEC_WORD(convertFBOutput<TForteWord>(&m_acRecvBuffer[dataIndex], m_unBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteWord);
              break;
            case CIEC_ANY::e_DWORD:
              apoRDs[i].setValue(CIEC_DWORD(convertFBOutput<TForteDWord>(&m_acRecvBuffer[dataIndex], m_unBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteDWord);
              break;
            case CIEC_ANY::e_LWORD:
              apoRDs[i].setValue(CIEC_LWORD(convertFBOutput<TForteLWord>(&m_acRecvBuffer[dataIndex], m_unBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteLWord);
              break;
            case CIEC_ANY::e_REAL:
              apoRDs[i].setValue(CIEC_REAL(convertFBOutput<TForteFloat>(&m_acRecvBuffer[dataIndex], m_unBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteFloat);
              break;
            case CIEC_ANY::e_LREAL:
              apoRDs[i].setValue(CIEC_LREAL(convertFBOutput<TForteDFloat>(&m_acRecvBuffer[dataIndex], m_unBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteDFloat);
              break;
            default:
              // TODO
              break;
          }
        }
        break;
      }
      case e_Disconnected:
      case e_Listening:
      case e_ConnectedAndListening:
      default:
        break;
    }
  }
  else{
    if(e_InitTerminated == m_eInterruptResp){
      // todo: Move server into listening mode again, etc.
    }
  }
  return m_eInterruptResp;
}

EComResponse CModbusComLayer::recvData(const void *, unsigned int){
  m_eInterruptResp = e_Nothing;

  switch (m_eConnectionState){
    case e_Listening:

      //TODO accept incoming connection

      break;
    case e_Connected: {
      int nRetVal = 0;
      switch (m_poFb->getComServiceType()){
        case e_Server:
          //TODO
          break;
        case e_Client:
          //TODO check if errors occured during polling in ModbusConnection
          nRetVal = m_pModbusConnection->readData(&m_acRecvBuffer[0]);
          break;
        case e_Publisher:
          //do nothing as publisher cannot receive data
          break;
        case e_Subscriber:
          //do nothing since Modbus protocol cannot act as subscriber
          break;
      }
      switch (nRetVal){
        case 0:
          //TODO
          break;
        default:
          //we successfully received data
          m_unBufFillSize = nRetVal;
          m_eInterruptResp = e_ProcessDataOk;
          break;
      }
      m_poFb->interruptCommFB(this);
    }
      break;
    case e_ConnectedAndListening:
    case e_Disconnected:
    default:
      break;
  }
  return m_eInterruptResp;
}

template<typename T>
T CModbusComLayer::convertFBOutput(TForteByte *pa_acDataArray, unsigned int pa_nDataSize){
  T retVal;
  unsigned int currentDataSize = sizeof(T);

  if(currentDataSize <= pa_nDataSize){
    if(currentDataSize > 2){
      // A data type with size greater than 16 bits is requested =>
      // we need to swap order of each 16 bit data package

      unsigned int nrUint16s = currentDataSize / 2;
      TForteUInt16 *destAr = (TForteUInt16*)(&retVal);
      TForteUInt16 *sourceAr = (TForteUInt16*) pa_acDataArray;

      for(unsigned int i = 0; i < nrUint16s; i++) {
        destAr[i] = sourceAr[nrUint16s - 1 - i];
      }
    }
    else{
      TForteByte *tempAr = (TForteByte*)(&retVal);
      for(unsigned int j = 0; j < currentDataSize; j++) {
        tempAr[j] = pa_acDataArray[j];
      }
    }
  }
  else {
    retVal = 0;
  }

  return retVal;
}

EComResponse CModbusComLayer::openConnection(char *pa_acLayerParameter){
  EComResponse eRetVal = e_InitInvalidId;
  switch (m_poFb->getComServiceType()){
    case e_Server:
      //TODO open server connection
      m_eConnectionState = e_Listening;
      break;
    case e_Client: {
      STcpParams tcpParams;
      SRtuParams rtuParams;
      SCommonParams commonParams;
      memset(&tcpParams, 0, sizeof(tcpParams));
      memset(&rtuParams, 0, sizeof(rtuParams));
      memset(&commonParams, 0, sizeof(commonParams));

      int errCode = processClientParams(pa_acLayerParameter, &tcpParams, &rtuParams, &commonParams);
      if(errCode != 0){
        DEVLOG_ERROR("CModbusComLayer:: Invalid input parameters\n");
      }
      else{
        m_pModbusConnection = new CModbusClientConnection((CModbusHandler*)&getExtEvHandler<CModbusHandler>());
        if(strlen(tcpParams.m_acIp) > 0){
          m_pModbusConnection->setIPAddress(tcpParams.m_acIp);
          m_pModbusConnection->setPort(tcpParams.m_nPort);
        }
        else if(strlen(rtuParams.m_acDevice) > 0){
          m_pModbusConnection->setDevice(rtuParams.m_acDevice);
          m_pModbusConnection->setBaud(rtuParams.m_nBaud);
          m_pModbusConnection->setParity(rtuParams.m_cParity);
          m_pModbusConnection->setDataBit(rtuParams.m_nDataBit);
          m_pModbusConnection->setStopBit(rtuParams.m_nStopBit);
        }
        m_pModbusConnection->setComCallback(this);
        m_pModbusConnection->setResponseTimeout(commonParams.m_nResponseTimeout);
        m_pModbusConnection->setByteTimeout(commonParams.m_nByteTimeout);

        static_cast<CModbusClientConnection*>(m_pModbusConnection)->setSlaveId(commonParams.m_nSlaveId);

        for(unsigned int i = 0; i < commonParams.m_nNrPolls; i++){
          static_cast<CModbusClientConnection*>(m_pModbusConnection)->addNewPoll(commonParams.m_nPollFrequency, commonParams.m_nFuncCode, commonParams.m_nReadStartAddress[i], commonParams.m_nReadNrAddresses[i]);
        }
        for(unsigned int i = 0; i < commonParams.m_nNrSends; i++){
          static_cast<CModbusClientConnection*>(m_pModbusConnection)->addNewSend(commonParams.m_nSendStartAddress[i], commonParams.m_nSendNrAddresses[i]);
        }

        if(m_pModbusConnection->connect() < 0){
          return eRetVal;
        }
        m_eConnectionState = e_Connected;
        eRetVal = e_InitOk;
      }
    }
      break;
    case e_Publisher:
      //do nothing as modbus cannot be publisher
      break;
    case e_Subscriber:
      //do nothing as modbus cannot be subscriber
      break;
  }

  return eRetVal;
}

void CModbusComLayer::closeConnection(){
  //TODO
  DEVLOG_INFO("CModbusLayer::closeConnection()\n");

  if(m_pModbusConnection != NULL){
    m_pModbusConnection->disconnect();
    delete m_pModbusConnection;
  }
}

int CModbusComLayer::processClientParams(char* pa_acLayerParams, STcpParams* pa_pTcpParams, SRtuParams* pa_pRtuParams, SCommonParams* pa_pCommonParams){
  char *params = new char[strlen(pa_acLayerParams) + 1];
  char *paramsAddress =  params;
  strcpy(params, pa_acLayerParams);
  char *chrStorage;

  pa_pTcpParams->m_acIp[0] = '\0';
  pa_pRtuParams->m_acDevice[0] = '\0';

  chrStorage = strchr(params, ':');
  if(chrStorage == 0){
    delete[] paramsAddress;
    return -1;
  }
  *chrStorage = '\0';
  ++chrStorage;

  if(strcmp(params, "rtu") == 0 || strcmp(params, "RTU") == 0){
    // get rtu params
    params = chrStorage;
    chrStorage = strchr(chrStorage, ':');
    if(chrStorage == 0){
      delete[] paramsAddress;
      return -1;
    }
    *chrStorage = '\0';
    ++chrStorage;

    strcpy(pa_pRtuParams->m_acDevice, params);
    pa_pRtuParams->m_nBaud = (int) forte::core::util::strtol(chrStorage, 0, 10);

    chrStorage = strchr(chrStorage, ':');
    if(chrStorage == 0){
      delete[] paramsAddress;
      return -1;
    }
    *chrStorage = '\0';
    ++chrStorage;

    pa_pRtuParams->m_cParity = chrStorage[0];

    chrStorage = strchr(chrStorage, ':');
    if(chrStorage == 0){
      delete[] paramsAddress;
      return -1;
    }
    *chrStorage = '\0';
    ++chrStorage;

    pa_pRtuParams->m_nDataBit = (int) forte::core::util::strtol(chrStorage, 0, 10);

    chrStorage = strchr(chrStorage, ':');
    if(chrStorage == 0){
      delete[] paramsAddress;
      return -1;
    }
    *chrStorage = '\0';
    ++chrStorage;

    pa_pRtuParams->m_nStopBit = (int) forte::core::util::strtol(chrStorage, 0, 10);

    chrStorage = strchr(chrStorage, ':');
    if(chrStorage == 0){
      delete[] paramsAddress;
      return -1;
    }
    *chrStorage = '\0';
    ++chrStorage;
  }
  else{
    if(strcmp(params, "tcp") == 0 || strcmp(params, "TCP") == 0){
      params = chrStorage;

      chrStorage = strchr(chrStorage, ':');
      if(chrStorage == 0){
        delete[] paramsAddress;
        return -1;
      }
      *chrStorage = '\0';
      ++chrStorage;
    }
    if(isIp(params)){
      // TCP connection
      strcpy(pa_pTcpParams->m_acIp, params);
      pa_pTcpParams->m_nPort = (unsigned int) forte::core::util::strtoul(chrStorage, 0, 10);

      chrStorage = strchr(chrStorage, ':');
      if(chrStorage == 0){
        delete[] paramsAddress;
        return -1;
      }
      *chrStorage = '\0';
      ++chrStorage;
    }
    else{
      delete[] paramsAddress;
      return -1;
    }
  }
  // Get common parameters

  pa_pCommonParams->m_nPollFrequency = atol(chrStorage);

  chrStorage = strchr(chrStorage, ':');
  if(chrStorage == 0){
    delete[] paramsAddress;
    return -1;
  }
  *chrStorage = '\0';
  ++chrStorage;

  pa_pCommonParams->m_nFuncCode = (unsigned int) forte::core::util::strtoul(chrStorage, 0, 10);

  chrStorage = strchr(chrStorage, ':');
  if(chrStorage == 0){
    delete[] paramsAddress;
    return -1;
  }
  *chrStorage = '\0';
  ++chrStorage;

  // Search for optional parameter slave id
  char *chrSlave = strchr(chrStorage, ':');
  if(chrSlave != NULL){
    chrSlave++;
    if(strchr(chrSlave, ':') != NULL){
      pa_pCommonParams->m_nSlaveId = (unsigned int) forte::core::util::strtoul(chrStorage, 0, 10);

      chrStorage = chrSlave;
    }
    else{
      pa_pCommonParams->m_nSlaveId = 0xFF;
    }
  }
  else{
    delete[] paramsAddress;
    return -1;
  }

  char *readAddresses = chrStorage;
  chrStorage = strchr(chrStorage, ':');
  if(chrStorage == 0){
    delete[] paramsAddress;
    return -1;
  }
  *chrStorage = '\0';
  ++chrStorage;

  // Find read addresses
  int paramLen = strlen(readAddresses);
  int nrPolls = 0;
  int strIndex = -1;
  while(strIndex < paramLen - 1){
    strIndex = findNextStartAddress(readAddresses, ++strIndex);
    if(strIndex < 0){
      break;
    }
    pa_pCommonParams->m_nReadStartAddress[nrPolls] = (unsigned int) forte::core::util::strtoul(const_cast<char*>(&readAddresses[strIndex]), 0, 10);
    strIndex = findNextStopAddress(readAddresses, strIndex);
    pa_pCommonParams->m_nReadNrAddresses[nrPolls] = (unsigned int) forte::core::util::strtoul(const_cast<char*>(&readAddresses[strIndex]), 0, 10) - pa_pCommonParams->m_nReadStartAddress[nrPolls] + 1;
    nrPolls++;
  }
  pa_pCommonParams->m_nNrPolls = nrPolls;

  char *writeAddresses = chrStorage;
  chrStorage = strchr(chrStorage, ':');
  if(chrStorage != 0){
    *chrStorage = '\0';
    ++chrStorage;
  }

  // Find send addresses
  paramLen = strlen(writeAddresses);
  int nrSends = 0;
  strIndex = -1;
  while(strIndex < paramLen - 1){
    strIndex = findNextStartAddress(writeAddresses, ++strIndex);
    if(strIndex < 0){
      break;
    }
    pa_pCommonParams->m_nSendStartAddress[nrSends] = (unsigned int) forte::core::util::strtoul(const_cast<char*>(&writeAddresses[strIndex]), 0, 10);
    strIndex = findNextStopAddress(writeAddresses, strIndex);
    pa_pCommonParams->m_nSendNrAddresses[nrSends] = (unsigned int) forte::core::util::strtoul(const_cast<char*>(&writeAddresses[strIndex]), 0, 10) - pa_pCommonParams->m_nSendStartAddress[nrSends] + 1;
    nrSends++;
  }
  pa_pCommonParams->m_nNrSends = nrSends;

  // Find responseTimeout and byteTimeout
  do{
    if(chrStorage == 0){
      break;
    }
    pa_pCommonParams->m_nResponseTimeout = (unsigned int) forte::core::util::strtoul(chrStorage, 0, 10);

    chrStorage = strchr(chrStorage, ':');
    if(chrStorage == 0){
      break;
    }
    *chrStorage = '\0';
    ++chrStorage;

    pa_pCommonParams->m_nByteTimeout = (unsigned int) forte::core::util::strtoul(chrStorage, 0, 10);
  } while(0);

  if(nrPolls == 0 && nrSends == 0){
    delete[] paramsAddress;
    return -1;
  }

  delete[] paramsAddress;
  return 0;
}

int CModbusComLayer::findNextStartAddress(const char* pa_acParam, int pa_nStartIndex){
  if(pa_nStartIndex == 0){
    switch (pa_acParam[pa_nStartIndex]){
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        return pa_nStartIndex;
    }
  }

  int strLength = strlen(&pa_acParam[pa_nStartIndex]);
  const char *pch = strchr(&pa_acParam[pa_nStartIndex], ',');

  if(pch != NULL){
    if(pch - &pa_acParam[pa_nStartIndex] < strLength - 1) {
      return pch - &pa_acParam[0] + 1;
    }

  }

  return -1;
}

int CModbusComLayer::findNextStopAddress(const char* pa_acParam, int pa_nStartIndex){
  int strLength = strlen(&pa_acParam[pa_nStartIndex]);
  const char *pchComma = strchr(&pa_acParam[pa_nStartIndex], ',');
  const char *pchDot = strchr(&pa_acParam[pa_nStartIndex], '.');

  if(pchComma != NULL && pchDot != NULL) {
    if(pchDot < pchComma && (pchDot - &pa_acParam[pa_nStartIndex] < strLength - 2)) {
      return pchDot - &pa_acParam[0] + 2;
    }
  } else if(pchDot != NULL) {
    if(pchDot - &pa_acParam[pa_nStartIndex] < strLength - 2) {
      return pchDot - &pa_acParam[0] + 2;
    }
  }

  return pa_nStartIndex;
}

bool CModbusComLayer::isIp(const char* pa_acIp){
  char* str = new char[strlen(pa_acIp) + 1];
  strcpy(str, pa_acIp);
  char* pch;
  int nrPeriods = 0;

  pch = strtok(str, ".");
  while(pch != NULL){
    nrPeriods++;
    if(strlen(pch) > 3){
      delete[] str;
      return false;
    }
    for(unsigned int i = 0; i < strlen(pch); i++){
      if(!forte::core::util::isDigit(pch[i])){
        delete[] str;
        return false;
      }
    }

    pch = strtok(NULL, ".");
  }
  if(nrPeriods != 4){
    delete[] str;
    return false;
  }

  delete[] str;
  return true;
}
