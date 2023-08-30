/*******************************************************************************
 * Copyright (c) 2012 - 2023 AIT, ACIN, fortiss GmbH, Davor Cihlar
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Patrick Smejkal, Alois Zoitl, Martin Melik-Merkumians - initial API and implementation and/or initial documentation
 *   Davor Cihlar - multiple FBs sharing a single Modbus connection
 *******************************************************************************/
#include <algorithm>
#include "modbuslayer.h"
#include "commfb.h"
#include "modbusclientconnection.h"

using namespace forte::com_infra;

std::vector<CModbusComLayer::SConnection> CModbusComLayer::smConnections;

CModbusComLayer::CModbusComLayer(CComLayer* paUpperLayer, CBaseCommFB* paComFB) :
    CComLayer(paUpperLayer, paComFB), mModbusConnection(nullptr), mBufFillSize(0),m_IOBlock(this){
  mConnectionState = e_Disconnected;
}

CModbusComLayer::~CModbusComLayer() = default;

EComResponse CModbusComLayer::sendData(void *paData, unsigned int paSize){
  EComResponse eRetVal = e_ProcessDataOk;

  if(mConnectionState == e_Connected){
    switch (mFb->getComServiceType()){
      case e_Server:
        // todo
        break;
      case e_Client: {
        TForteUInt8 *convertedData = new TForteUInt8[paSize * 8];
        unsigned int sendLength = convertDataInput(paData, paSize, convertedData);
        if(sendLength > 0){
          mModbusConnection->writeData(&m_IOBlock, convertedData, sendLength);
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

unsigned int CModbusComLayer::convertDataInput(void *paInData, unsigned int paDataSize, void *paConvertedData){
  TForteUInt8 *convertedData = (TForteUInt8*)paConvertedData;
  unsigned int outLength = 0;

  CIEC_ANY **apoSDs = static_cast<CIEC_ANY**>(paInData);
  unsigned int nrSDs = paDataSize;

  for(unsigned int i = 0; i < nrSDs; i++){
    CIEC_ANY *anyVal = apoSDs[i];
    switch (anyVal->getDataTypeID()) {
      case CIEC_ANY::e_BOOL: // 1bit data type
      {
        TForteUInt8 out = (bool) *(CIEC_BOOL*) anyVal;
        *(TForteUInt8*) (&convertedData[outLength]) = out;
        outLength += sizeof(TForteUInt8);
        break;
      }
      case CIEC_ANY::e_SINT: // 8bit data types
      {
        TForteInt16 out = (TForteInt8) *(CIEC_SINT*) anyVal;
        *(TForteInt16*) (&convertedData[outLength]) = out;
        outLength += sizeof(TForteInt16);
        break;
      }
      case CIEC_ANY::e_USINT: {
        TForteUInt16 out = (TForteUInt8) *(CIEC_USINT*) anyVal;
        *(TForteUInt16*) (&convertedData[outLength]) = out;
        outLength += sizeof(TForteUInt16);
        break;
      }
      case CIEC_ANY::e_BYTE: {
        TForteUInt16 out = (TForteByte) *(CIEC_BYTE*) anyVal;
        *(TForteUInt16*) (&convertedData[outLength]) = out;
        outLength += sizeof(TForteUInt16);
        break;
      }
      case CIEC_ANY::e_INT: // 16bit data types
      {
        TForteInt16 out = (TForteInt16) *(CIEC_INT*) anyVal;
        *(TForteInt16*) (&convertedData[outLength]) = out;
        outLength += sizeof(TForteInt16);
        break;
      }
      case CIEC_ANY::e_UINT: {
        TForteUInt16 out = (TForteUInt16) *(CIEC_UINT*) anyVal;
        *(TForteUInt16*) (&convertedData[outLength]) = out;
        outLength += sizeof(TForteUInt16);
        break;
      }
      case CIEC_ANY::e_WORD: {
        TForteWord out = (TForteWord) *(CIEC_WORD*) anyVal;
        *(TForteWord*) (&convertedData[outLength]) = out;
        outLength += sizeof(TForteWord);
        break;
      }
      case CIEC_ANY::e_DINT: // 32bit data types
      {
        TForteInt32 out = (TForteInt32) *static_cast<CIEC_DINT*>(anyVal);
        *(TForteInt32*) (&convertedData[outLength]) = convertFBOutput<TForteInt32>((TForteByte*) &out, sizeof(TForteInt32));
        outLength += sizeof(TForteInt32);
        break;
      }
      case CIEC_ANY::e_UDINT: {
        TForteUInt32 out = (TForteUInt32) *(CIEC_UDINT*) anyVal;
        *(TForteUInt32*) (&convertedData[outLength]) = convertFBOutput<TForteUInt32>((TForteByte*) &out, sizeof(TForteUInt32));
        outLength += sizeof(TForteUInt32);
        break;
      }
      case CIEC_ANY::e_DWORD: {
        TForteDWord out = (TForteDWord) *(CIEC_DWORD*) anyVal;
        *(TForteDWord*) (&convertedData[outLength]) = convertFBOutput<TForteDWord>((TForteByte*) &out, sizeof(TForteDWord));
        outLength += sizeof(TForteDWord);
        break;
      }
      case CIEC_ANY::e_REAL: {
        TForteFloat out = (TForteFloat) *(CIEC_REAL*) anyVal;
        *(TForteFloat*) (&convertedData[outLength]) = convertFBOutput<TForteFloat>((TForteByte*) &out, sizeof(TForteFloat));
        outLength += sizeof(TForteFloat);
        break;
      }
      case CIEC_ANY::e_LINT: // 64bit data types
      {
        TForteInt64 out = (TForteInt64) *(CIEC_LINT*) anyVal;
        *(TForteInt64*) (&convertedData[outLength]) = convertFBOutput<TForteInt64>((TForteByte*) &out, sizeof(TForteInt64));
        outLength += sizeof(TForteInt64);
        break;
      }
      case CIEC_ANY::e_ULINT: {
        TForteUInt64 out = (TForteUInt64) *(CIEC_ULINT*) anyVal;
        *(TForteUInt64*) (&convertedData[outLength]) = convertFBOutput<TForteUInt64>((TForteByte*) &out, sizeof(TForteUInt64));
        outLength += sizeof(TForteUInt64);
        break;
      }
      case CIEC_ANY::e_LWORD: {
        TForteLWord out = (TForteLWord) *(CIEC_LWORD*) anyVal;
        *(TForteLWord*) (&convertedData[outLength]) = convertFBOutput<TForteLWord>((TForteByte*) &out, sizeof(TForteLWord));
        outLength += sizeof(TForteLWord);
        break;
      }
      case CIEC_ANY::e_LREAL: {
        TForteDFloat out = (TForteDFloat) *(CIEC_LREAL*) anyVal;
        *(TForteDFloat*) (&convertedData[outLength]) = convertFBOutput<TForteDFloat>((TForteByte*) &out, sizeof(TForteDFloat));
        outLength += sizeof(TForteDFloat);
        break;
      }
      default:
        break;
    }
  }
  return outLength;
}

EComResponse CModbusComLayer::processInterrupt(){
  if(e_ProcessDataOk == mInterruptResp){
    switch (mConnectionState){
      case e_Connected: {
        CIEC_ANY **apoRDs = mFb->getRDs();
        unsigned int nrRDs = mFb->getNumRD();

        unsigned int dataIndex = 0;

        for(unsigned int i = 0; i < nrRDs; i++){
          switch (apoRDs[i]->getDataTypeID()){
            case CIEC_ANY::e_BOOL:
              apoRDs[i]->setValue(CIEC_BOOL(convertFBOutput<bool>(&mRecvBuffer[dataIndex], mBufFillSize - dataIndex)));
              dataIndex += sizeof(bool);
              break;
            case CIEC_ANY::e_SINT:
              apoRDs[i]->setValue(CIEC_SINT(convertFBOutput<TForteInt8>(&mRecvBuffer[dataIndex], mBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteInt8);
              break;
            case CIEC_ANY::e_INT:
              apoRDs[i]->setValue(CIEC_INT(convertFBOutput<TForteInt16>(&mRecvBuffer[dataIndex], mBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteInt16);
              break;
            case CIEC_ANY::e_DINT:
              apoRDs[i]->setValue(CIEC_DINT(convertFBOutput<TForteInt32>(&mRecvBuffer[dataIndex], mBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteInt32);
              break;
            case CIEC_ANY::e_LINT:
              apoRDs[i]->setValue(CIEC_LINT(convertFBOutput<TForteInt64>(&mRecvBuffer[dataIndex], mBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteInt64);
              break;
            case CIEC_ANY::e_USINT:
              apoRDs[i]->setValue(CIEC_USINT(convertFBOutput<TForteUInt8>(&mRecvBuffer[dataIndex], mBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteUInt8);
              break;
            case CIEC_ANY::e_UINT:
              apoRDs[i]->setValue(CIEC_UINT(convertFBOutput<TForteUInt16>(&mRecvBuffer[dataIndex], mBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteUInt16);
              break;
            case CIEC_ANY::e_UDINT:
              apoRDs[i]->setValue(CIEC_UDINT(convertFBOutput<TForteUInt32>(&mRecvBuffer[dataIndex], mBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteUInt32);
              break;
            case CIEC_ANY::e_ULINT:
              apoRDs[i]->setValue(CIEC_ULINT(convertFBOutput<TForteUInt64>(&mRecvBuffer[dataIndex], mBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteUInt64);
              break;
            case CIEC_ANY::e_BYTE:
              apoRDs[i]->setValue(CIEC_BYTE(convertFBOutput<TForteByte>(&mRecvBuffer[dataIndex], mBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteByte);
              break;
            case CIEC_ANY::e_WORD:
              apoRDs[i]->setValue(CIEC_WORD(convertFBOutput<TForteWord>(&mRecvBuffer[dataIndex], mBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteWord);
              break;
            case CIEC_ANY::e_DWORD:
              apoRDs[i]->setValue(CIEC_DWORD(convertFBOutput<TForteDWord>(&mRecvBuffer[dataIndex], mBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteDWord);
              break;
            case CIEC_ANY::e_LWORD:
              apoRDs[i]->setValue(CIEC_LWORD(convertFBOutput<TForteLWord>(&mRecvBuffer[dataIndex], mBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteLWord);
              break;
            case CIEC_ANY::e_REAL:
              apoRDs[i]->setValue(CIEC_REAL(convertFBOutput<TForteFloat>(&mRecvBuffer[dataIndex], mBufFillSize - dataIndex)));
              dataIndex += sizeof(TForteFloat);
              break;
            case CIEC_ANY::e_LREAL:
              apoRDs[i]->setValue(CIEC_LREAL(convertFBOutput<TForteDFloat>(&mRecvBuffer[dataIndex], mBufFillSize - dataIndex)));
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
    if(e_InitTerminated == mInterruptResp){
      // todo: Move server into listening mode again, etc.
    }
  }
  return mInterruptResp;
}

EComResponse CModbusComLayer::recvData(const void *, unsigned int){
  mInterruptResp = e_Nothing;

  switch (mConnectionState){
    case e_Listening:

      //TODO accept incoming connection

      break;
    case e_Connected: {
      int nRetVal = 0;
      switch (mFb->getComServiceType()){
        case e_Server:
          //TODO
          break;
        case e_Client:
          //TODO check if errors occured during polling in ModbusConnection
          nRetVal = mModbusConnection->readData(&m_IOBlock, &mRecvBuffer[0], sizeof(mRecvBuffer));
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
          mBufFillSize = nRetVal;
          mInterruptResp = e_ProcessDataOk;
          break;
      }
      mFb->interruptCommFB(this);
    }
      break;
    case e_ConnectedAndListening:
    case e_Disconnected:
    default:
      break;
  }
  return mInterruptResp;
}

template<typename T>
T CModbusComLayer::convertFBOutput(TForteByte *paDataArray, unsigned int paDataSize){
  T retVal;
  unsigned int currentDataSize = sizeof(T);

  if(currentDataSize <= paDataSize){
    if(currentDataSize > 2){
      // A data type with size greater than 16 bits is requested =>
      // we need to swap order of each 16 bit data package

      unsigned int nrUint16s = currentDataSize / 2;
      TForteUInt16 *destAr = (TForteUInt16*)(&retVal);
      TForteUInt16 *sourceAr = (TForteUInt16*) paDataArray;

      for(unsigned int i = 0; i < nrUint16s; i++) {
        destAr[i] = sourceAr[nrUint16s - 1 - i];
      }
    }
    else{
      TForteByte *tempAr = (TForteByte*)(&retVal);
      for(unsigned int j = 0; j < currentDataSize; j++) {
        tempAr[j] = paDataArray[j];
      }
    }
  }
  else {
    retVal = 0;
  }

  return retVal;
}

EComResponse CModbusComLayer::openConnection(char *paLayerParameter){
  EComResponse eRetVal = e_InitInvalidId;
  switch (mFb->getComServiceType()){
    case e_Server:
      //TODO open server connection
      mConnectionState = e_Listening;
      break;
    case e_Client: {
      STcpParams tcpParams;
      SRtuParams rtuParams;
      SCommonParams commonParams;
      char idString[256] = {0};
      memset(&tcpParams, 0, sizeof(tcpParams));
      memset(&rtuParams, 0, sizeof(rtuParams));
      memset(&commonParams, 0, sizeof(commonParams));

      int errCode = processClientParams(paLayerParameter, &tcpParams, &rtuParams, &commonParams, idString);
      if(errCode != 0){
        DEVLOG_ERROR("CModbusComLayer:: Invalid input parameters\n");
      }
      else{
        bool reuseConnection = false;
        mModbusConnection = getClientConnection(idString);
        if(strlen(tcpParams.mIp) > 0){
          mModbusConnection->setIPAddress(tcpParams.mIp);
          mModbusConnection->setPort(tcpParams.mPort);
        }
        else if(strlen(rtuParams.mDevice) > 0){
          mModbusConnection->setDevice(rtuParams.mDevice);
          mModbusConnection->setBaud(rtuParams.mBaud);
          mModbusConnection->setParity(rtuParams.mParity);
          mModbusConnection->setDataBit(rtuParams.mDataBit);
          mModbusConnection->setStopBit(rtuParams.mStopBit);
          mModbusConnection->setFlowControl(rtuParams.mFlowControl);
        }
        else{
          reuseConnection = true;
        }
        mModbusConnection->setResponseTimeout(commonParams.mResponseTimeout);
        mModbusConnection->setByteTimeout(commonParams.mByteTimeout);

        static_cast<CModbusClientConnection*>(mModbusConnection)->setSlaveId(commonParams.mSlaveId);

        for(unsigned int i = 0; i < commonParams.mNrPolls; i++){
          const SAddrRange *const readParams = commonParams.mRead;
          m_IOBlock.addNewRead(readParams[i].mFunction, readParams[i].mStartAddress, readParams[i].mNrAddresses);
        }
        for(unsigned int i = 0; i < commonParams.mNrSends; i++){
          const SAddrRange *const sendParams = commonParams.mSend;
          m_IOBlock.addNewSend(sendParams[i].mFunction, sendParams[i].mStartAddress, sendParams[i].mNrAddresses);
        }
        static_cast<CModbusClientConnection*>(mModbusConnection)->addNewPoll(commonParams.mPollFrequency, &m_IOBlock);

        if(!reuseConnection && mModbusConnection->connect() < 0){
          return eRetVal;
        }
        mConnectionState = e_Connected;
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

  putConnection(mModbusConnection);
}

EModbusFunction CModbusComLayer::decodeFunction(const char* paParam, int *strIndex, EModbusFunction paDefaultFunction){
  switch(paParam[*strIndex]){
    case 'd':
    case 'D':
      ++*strIndex;
      return eDiscreteInput;
    case 'c':
    case 'C':
      ++*strIndex;
      return eCoil;
    case 'i':
    case 'I':
      ++*strIndex;
      return eInputRegister;
    case 'h':
    case 'H':
      ++*strIndex;
      return eHoldingRegister;
    default:
      break;
  }
  return paDefaultFunction;
}

int CModbusComLayer::processClientParams(const char* paLayerParams, STcpParams* paTcpParams, SRtuParams* paRtuParams, SCommonParams* paCommonParams, char* paIdString){
  char *params = new char[strlen(paLayerParams) + 1];
  char *paramsAddress =  params;
  strcpy(params, paLayerParams);
  char *chrStorage;
  unsigned int defaultSlaveId;
  bool reuseConnection = false;

  paTcpParams->mIp[0] = '\0';
  paRtuParams->mDevice[0] = '\0';

  chrStorage = strchr(params, ':');
  if(chrStorage == nullptr){
    delete[] paramsAddress;
    return -1;
  }
  *chrStorage = '\0';
  ++chrStorage;

  if(strcmp(params, "rtu") == 0 || strcmp(params, "RTU") == 0){
    defaultSlaveId = 1;

    // get rtu params
    params = chrStorage;
    chrStorage = strchr(chrStorage, ':');
    if(chrStorage == nullptr){
      delete[] paramsAddress;
      return -1;
    }
    *chrStorage = '\0';
    ++chrStorage;

    strcpy(paIdString, "rtu:");
    strcat(paIdString, params);
    strcpy(paRtuParams->mDevice, params);
    paRtuParams->mBaud = (int) forte::core::util::strtol(chrStorage, nullptr, 10);

    chrStorage = strchr(chrStorage, ':');
    if(chrStorage == nullptr){
      delete[] paramsAddress;
      return -1;
    }
    *chrStorage = '\0';
    ++chrStorage;

    paRtuParams->mParity = chrStorage[0];
    reuseConnection |= chrStorage[0] == ':';

    chrStorage = strchr(chrStorage, ':');
    if(chrStorage == nullptr){
      delete[] paramsAddress;
      return -1;
    }
    *chrStorage = '\0';
    ++chrStorage;

    paRtuParams->mDataBit = (int) forte::core::util::strtol(chrStorage, nullptr, 10);
    reuseConnection |= !chrStorage[0];

    chrStorage = strchr(chrStorage, ':');
    if(chrStorage == nullptr){
      delete[] paramsAddress;
      return -1;
    }
    *chrStorage = '\0';
    ++chrStorage;

    paRtuParams->mStopBit = (int) forte::core::util::strtol(chrStorage, nullptr, 10);
    reuseConnection |= !chrStorage[0];

    chrStorage = strchr(chrStorage, ':');
    if(chrStorage == nullptr){
      delete[] paramsAddress;
      return -1;
    }
    *chrStorage = '\0';
    ++chrStorage;

    const char* flow = chrStorage;
    chrStorage = strchr(chrStorage, ':');
    if(chrStorage == 0){
      delete[] paramsAddress;
      return -1;
    }
    *chrStorage = '\0';
    ++chrStorage;

    if (!strcmp(flow, "arduino")) {
      paRtuParams->mFlowControl = eFlowArduino;
    } else if (!strcmp(flow, "delay")) {
      paRtuParams->mFlowControl = eFlowDelay;
    } else if (!strcmp(flow, "longdelay") || !strcmp(flow, "delay5")) {
      paRtuParams->mFlowControl = eFlowLongDelay;
    } else {
      paRtuParams->mFlowControl = eFlowNone;
    }

    if (reuseConnection) {
      paRtuParams->mDevice[0] = '\0';
    }
  }
  else{
    if(strcmp(params, "tcp") == 0 || strcmp(params, "TCP") == 0){
      defaultSlaveId = 0xFF;

      params = chrStorage;

      chrStorage = strchr(chrStorage, ':');
      if(chrStorage == nullptr){
        delete[] paramsAddress;
        return -1;
      }
      *chrStorage = '\0';
      ++chrStorage;
    }
    if(isIp(params)){
      // TCP connection
      strcpy(paIdString, "tcp:");
      strcat(paIdString, params);
      strcat(paIdString, ":");
      strcpy(paTcpParams->mIp, params);
      paTcpParams->mPort = (unsigned int) forte::core::util::strtoul(chrStorage, nullptr, 10);
      reuseConnection |= !chrStorage[0];

      params = chrStorage;
      chrStorage = strchr(chrStorage, ':');
      if(chrStorage == nullptr){
        delete[] paramsAddress;
        return -1;
      }
      *chrStorage = '\0';
      ++chrStorage;
      strcat(paIdString, params);
    }
    else{
      delete[] paramsAddress;
      return -1;
    }

    if (reuseConnection) {
      paTcpParams->mIp[0] = '\0';
    }
  }
  // Get common parameters

  char *chrSlave = chrStorage;
  chrStorage = strchr(chrStorage, ':');
  if(chrStorage == nullptr){
    delete[] paramsAddress;
    return -1;
  }
  *chrStorage = '\0';
  ++chrStorage;

  // Search for optional parameter slave id
  if(*chrSlave){
    paCommonParams->mSlaveId = (unsigned int) forte::core::util::strtoul(chrSlave, nullptr, 10);
  }
  else{
    paCommonParams->mSlaveId = defaultSlaveId;
  }

  char *pollFrequency = chrStorage;
  chrStorage = strchr(chrStorage, ':');
  if(chrStorage == 0){
    delete[] paramsAddress;
    return -1;
  }
  *chrStorage = '\0';
  ++chrStorage;

  // Find poll frequency
  paCommonParams->mPollFrequency = atol(pollFrequency);

  char *readAddresses = chrStorage;
  chrStorage = strchr(chrStorage, ':');
  if(chrStorage == nullptr){
    delete[] paramsAddress;
    return -1;
  }
  *chrStorage = '\0';
  ++chrStorage;

  // Find read addresses
  int paramLen = (int)strlen(readAddresses);
  int nrPolls = 0;
  int strIndex = -1;
  while(strIndex < paramLen - 1){
    strIndex = findNextStartAddress(readAddresses, ++strIndex);
    if(strIndex < 0){
      break;
    }
    SAddrRange *const curRead = &paCommonParams->mRead[nrPolls];
    curRead->mFunction = decodeFunction(readAddresses, &strIndex);
    curRead->mStartAddress = (unsigned int) forte::core::util::strtoul(const_cast<char*>(&readAddresses[strIndex]), nullptr, 10);
    strIndex = findNextStopAddress(readAddresses, strIndex);
    curRead->mNrAddresses = (unsigned int) forte::core::util::strtoul(const_cast<char*>(&readAddresses[strIndex]), nullptr, 10) - curRead->mStartAddress + 1;
    nrPolls++;
  }
  paCommonParams->mNrPolls = nrPolls;

  char *writeAddresses = chrStorage;
  chrStorage = strchr(chrStorage, ':');
  if(chrStorage != nullptr){
    *chrStorage = '\0';
    ++chrStorage;
  }

  // Find send addresses
  paramLen = (int)strlen(writeAddresses);
  int nrSends = 0;
  strIndex = -1;
  while(strIndex < paramLen - 1){
    strIndex = findNextStartAddress(writeAddresses, ++strIndex);
    if(strIndex < 0){
      break;
    }
    SAddrRange *const curSend = &paCommonParams->mSend[nrSends];
    curSend->mFunction = decodeFunction(writeAddresses, &strIndex);
    curSend->mStartAddress = (unsigned int) forte::core::util::strtoul(const_cast<char*>(&writeAddresses[strIndex]), nullptr, 10);
    strIndex = findNextStopAddress(writeAddresses, strIndex);
    curSend->mNrAddresses = (unsigned int) forte::core::util::strtoul(const_cast<char*>(&writeAddresses[strIndex]), nullptr, 10) - curSend->mStartAddress + 1;
    nrSends++;
  }
  paCommonParams->mNrSends = nrSends;

  // Find responseTimeout and byteTimeout
  do{
    if(chrStorage == nullptr){
      break;
    }
    paCommonParams->mResponseTimeout = (unsigned int) forte::core::util::strtoul(chrStorage, nullptr, 10);

    chrStorage = strchr(chrStorage, ':');
    if(chrStorage == nullptr){
      break;
    }
    *chrStorage = '\0';
    ++chrStorage;

    paCommonParams->mByteTimeout = (unsigned int) forte::core::util::strtoul(chrStorage, nullptr, 10);
  } while(false);

  if(nrPolls == 0 && nrSends == 0){
    delete[] paramsAddress;
    return -1;
  }

  delete[] paramsAddress;
  return 0;
}

int CModbusComLayer::findNextStartAddress(const char* paParam, int paStartIndex){
  if(paStartIndex == 0){
    switch (paParam[paStartIndex]){
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
      case 'c': // coil
      case 'C':
      case 'd': // discrete
      case 'D':
      case 'h': // holding
      case 'H':
      case 'i': // input
      case 'I':
        return paStartIndex;
    }
  }

  int strLength = (int)strlen(&paParam[paStartIndex]);
  const char *pch = strchr(&paParam[paStartIndex], ',');

  if(pch != nullptr){
    if(pch - &paParam[paStartIndex] < strLength - 1) {
      return (int)(pch - &paParam[0]) + 1;
    }

  }

  return -1;
}

int CModbusComLayer::findNextStopAddress(const char* paParam, int paStartIndex){
  int strLength = (int)strlen(&paParam[paStartIndex]);
  const char *pchComma = strchr(&paParam[paStartIndex], ',');
  const char *pchDot = strchr(&paParam[paStartIndex], '.');

  if(pchComma != nullptr && pchDot != nullptr) {
    if(pchDot < pchComma && (pchDot - &paParam[paStartIndex] < strLength - 2)) {
      return (int)(pchDot - &paParam[0]) + 2;
    }
  } else if(pchDot != nullptr) {
    if(pchDot - &paParam[paStartIndex] < strLength - 2) {
      return (int)(pchDot - &paParam[0]) + 2;
    }
  }

  return paStartIndex;
}

bool CModbusComLayer::isIp(const char* paIp){
  char* str = new char[strlen(paIp) + 1];
  strcpy(str, paIp);
  char* pch;
  int nrPeriods = 0;

  pch = strtok(str, ".");
  while(pch != nullptr){
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

    pch = strtok(nullptr, ".");
  }
  if(nrPeriods != 4){
    delete[] str;
    return false;
  }

  delete[] str;
  return true;
}

CModbusConnection* CModbusComLayer::getClientConnection(const char* paIdString) {
  auto itConn = std::find_if(
          smConnections.begin(),
          smConnections.end(),
          [paIdString](const SConnection &sc) {
            return !strcmp(sc.mIdString, paIdString);
          });
  if (itConn != smConnections.end()) {
    ++itConn->mUseCount;
    return itConn->mConnection;
  }

  CModbusConnection *modbusConnection = new CModbusClientConnection((CModbusHandler*)&getExtEvHandler<CModbusHandler>());
  SConnection connInfo = {{0}, 1, modbusConnection};
  strcpy(connInfo.mIdString, paIdString);
  smConnections.push_back(connInfo);
  return modbusConnection;
}

void CModbusComLayer::putConnection(CModbusConnection *paModbusConn) {
  auto itConn = std::find_if(
          smConnections.begin(),
          smConnections.end(),
          [paModbusConn](const SConnection &sc) {
            return sc.mConnection == paModbusConn;
          });
  if (itConn != smConnections.end()) {
    if (!--itConn->mUseCount) {
      itConn->mConnection->disconnect();
      delete itConn->mConnection;
      smConnections.erase(itConn);
    }
  }
}
