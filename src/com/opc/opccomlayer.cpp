/*******************************************************************************
 * Copyright (c) 2012, 2022 AIT, ACIN, fortiss GmbH, Hit robot group
 *               2024 Samator Indo Gas
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Patrick Smejkal, Alois Zoitl, Martin Melik-Merkumians - initial API and implementation and/or initial documentation
 *   ys guo - Fix opc module compilation errors and deadlock bug
 *   Tibalt Zhao -Merge additem into Connect and take advantage of detailed event info from opcconnection
 *   Ketut Kumajaya - Code refactoring from char* to std::string
 *                  - Proper memory usage and deallocation
 *******************************************************************************/
#include "opccomlayer.h"
#include "arch/devlog.h"
#include "commfb.h"
#include "core/util/parameterParser.h"

#include "opcconnection.h"
#include "opcconnectionhandler.h"
#include "Variant.h"
#include <criticalregion.h>
#include <boost/lexical_cast.hpp>

using namespace forte::com_infra;

COpcComLayer::COpcComLayer(CComLayer* paUpperLayer, CBaseCommFB* paComFB) :
    CComLayer(paUpperLayer, paComFB), mHost(), mServerName(), mUpdateRate(0), mDeadBand(0),
    mLayerParamsOK(false), mOpcConnection(0), mInterruptResp(e_Nothing), mOpcGroupName(mFb->getInstanceName()){
  DEVLOG_DEBUG("new OpcComLayer: [0x%lX][%s]\n", this, mOpcGroupName.c_str());
}

COpcComLayer::~COpcComLayer(){
  DEVLOG_DEBUG("delete OpcComLayer: [0x%lX][%s]\n", this, mOpcGroupName.c_str());
  for(auto &it :mFBInputVars){
    delete it;
  }
  //clear vector and remove it from memory
  TOpcProcessVarList().swap(mFBInputVars);
  for(auto &it :mFBOutputVars){
    delete it;
  }
  //clear vector and remove it from memory
  TOpcProcessVarList().swap(mFBOutputVars);
}

EComResponse COpcComLayer::sendData(void *paData, unsigned int paSize){
  EComResponse eRetVal = e_ProcessDataOk;

  if((0 != mFb)){
    if(mOpcConnection->getConnectionState() == COpcConnection::e_Connected || 
          mOpcConnection->getConnectionState() == COpcConnection::e_Connecting){
      switch (mFb->getComServiceType()){
      case e_Server:
        //TODO
        break;
      case e_Client: {
        convertInputData(paData, paSize);

        TOpcProcessVarList::iterator itEnd = mFBInputVars.end();
        for(TOpcProcessVarList::iterator it = mFBInputVars.begin(); it != itEnd; ++it){
            int rtn = mOpcConnection->send_sendItemData((*it));
            if(rtn != 0){
              // data send failed
              DEVLOG_DEBUG("send_sendItemData failed[%s]\n", mFb->getInstanceName());
              eRetVal = e_ProcessDataSendFailed;
              return eRetVal;
            }
        }
        break;
      }
      case e_Publisher:
        //do nothing as OPC cannot be publisher
        break;
      case e_Subscriber:
        //do nothing as subscribers do not send data
        break;
      }
    }
    else{
      DEVLOG_DEBUG("COpcComLayer::sendData():OPC connection is not connected[%s]\n", mFb->getInstanceName());
      eRetVal = e_ProcessDataSendFailed;
    }
  }
  return eRetVal;
}

EComResponse COpcComLayer::processInterrupt(){
  EComResponse eRet = e_Nothing;
  EComResponse currentComResponse;

  {
    CCriticalRegion criticalRegion(mSync);
    if(mComResponses.empty()){
      return eRet;
    }
    TComResponseList::iterator comIt(mComResponses.begin());
    currentComResponse = *comIt;
    mComResponses.pop_front();
  }

  switch (currentComResponse){
    case e_ProcessDataOk:
      switch (mConnectionState){
      case e_Connected: {
        CIEC_ANY **apoRDs = mFb->getRDs();
        size_t nrRDs = mFb->getNumRD();

        TOpcProcessVarList::iterator itEnd = mFBOutputVars.end();
        TOpcProcessVarList::iterator it = mFBOutputVars.begin();
        for(size_t i = 0; i < nrRDs && it != itEnd; i++, ++it){
          setOutputValue(&apoRDs[i]->unwrap(), &(*it)->updateValue());
        }

        break;
      }
      case e_Disconnected:
      case e_Listening:
      case e_ConnectedAndListening:
      default:
        break;
      }
      eRet = e_ProcessDataOk;
      break;
    case e_ProcessDataSendFailed:
      eRet = e_ProcessDataSendFailed;
      break;
    case e_InitInvalidId:
      eRet = e_InitInvalidId;
      break;
    case e_InitTerminated:
      eRet = e_InitTerminated;
      break;
  }

  return eRet;
}

EComResponse COpcComLayer::recvData(const void *, unsigned int){
  EComResponse eRet = e_Nothing;

  switch (mFb->getComServiceType()){
    case e_Server:
      //TODO
      break;
    case e_Client: {
      COpcConnection::EOpcConnectionEvents opcConnEvent = mOpcConnection->getLastEvent().mLastHappenedEvent;
      bool success = mOpcConnection->getLastEvent().mSuccess;
      switch (opcConnEvent){
        case COpcConnection::e_Disconnect:
          //TODO error during connection try
          break;
        case COpcConnection::e_Connect:
          if (success) {
              mConnectionState = e_Connected;
          }
          else {
              {
                CCriticalRegion criticalRegion(mSync);
                mComResponses.push_back(e_InitTerminated);
              }
              mFb->interruptCommFB(this);
              eRet = e_InitTerminated;
          }
          break;
          case COpcConnection::e_ItemSend:{
            if(!success){
              DEVLOG_DEBUG("opccomlayer:recvData():OPC Item Send Failed[%s]\n",mFb->getInstanceName());
              {
                CCriticalRegion criticalRegion(mSync);
                mComResponses.push_back(e_ProcessDataSendFailed);
              }
              mFb->interruptCommFB(this);
              eRet = e_ProcessDataSendFailed;
              break;
            }
          }
        break;
        case COpcConnection::e_DataReceive: {
          if(success){
            int nRetVal = mOpcConnection->receiveData(mOpcGroupName, &mFBOutputVars);
            if (nRetVal > 0) {
            //we successfully received data
              CCriticalRegion criticalRegion(mSync);
              mComResponses.push_back(e_ProcessDataOk);
            }
            mFb->interruptCommFB(this);
            eRet = e_ProcessDataOk;
          }
          break;
        }
        default:
          break;
        }
      break;
    }
    case e_Publisher:
      //do nothing as publisher cannot receive data
    case e_Subscriber:
      //do nothing as OPC cannot be subscribers
      break;
  }

  return eRet;
}

EComResponse COpcComLayer::openConnection(char *paLayerParameter){
  EComResponse eRetVal = e_InitInvalidId;
  mConnectionState = e_Disconnected;

  switch (mFb->getComServiceType()){
  case e_Server:
    //TODO
    break;
  case e_Client:
    processClientParams(paLayerParameter);
    if(mLayerParamsOK){
      eRetVal = e_InitOk;
      mOpcConnection = COpcConnectionHandler::getInstance().getOpcConnection(mHost, mServerName, this);
      TOpcProcessVarList lAllVars;
      lAllVars.insert(lAllVars.end(), mFBInputVars.begin(), mFBInputVars.end());
      lAllVars.insert(lAllVars.end(), mFBOutputVars.begin(), mFBOutputVars.end());
      mOpcConnection->send_connect(mOpcGroupName, mUpdateRate, mDeadBand, this, lAllVars);
      eRetVal = e_InitOk;
    }
    break;
  case e_Publisher:
    //do nothing, OPC cannot be publisher
    break;
  case e_Subscriber:
    //do nothing, OPC cannot be subscriber
    break;
  }

  return eRetVal;
}

void COpcComLayer::closeConnection(){
  DEVLOG_DEBUG("COpcComLayer::closeConnection()[%s] \n", mFb->getInstanceName());
  COpcConnectionHandler::getInstance().removeOpcConnection(mOpcConnection->getHost(), mOpcConnection->getServerName(), mOpcGroupName);
  mConnectionState = e_Disconnected;
}

void COpcComLayer::setOutputValue(CIEC_ANY *paDataOut, Variant * paValue){
  switch (paDataOut->getDataTypeID()){
  case CIEC_ANY::e_BOOL:
    paDataOut->setValue(CIEC_BOOL(paValue->get<bool>()));
    break;
  case CIEC_ANY::e_SINT:
    paDataOut->setValue(CIEC_SINT(paValue->get<TForteInt8>()));
    break;
  case CIEC_ANY::e_INT:
    paDataOut->setValue(CIEC_INT(paValue->get<TForteInt16>()));
    break;
  case CIEC_ANY::e_DINT:
    paDataOut->setValue(CIEC_DINT(paValue->get<TForteInt32>()));
    break;
  case CIEC_ANY::e_LINT:
    paDataOut->setValue(CIEC_LINT(paValue->get<TForteInt64>()));
    break;
  case CIEC_ANY::e_USINT:
    paDataOut->setValue(CIEC_USINT(paValue->get<TForteUInt8>()));
    break;
  case CIEC_ANY::e_UINT:
    paDataOut->setValue(CIEC_UINT(paValue->get<TForteUInt16>()));
    break;
  case CIEC_ANY::e_UDINT:
    paDataOut->setValue(CIEC_UDINT(paValue->get<TForteUInt32>()));
    break;
  case CIEC_ANY::e_ULINT:
    paDataOut->setValue(CIEC_ULINT(paValue->get<TForteUInt64>()));
    break;
  case CIEC_ANY::e_BYTE:
    paDataOut->setValue(CIEC_BYTE(paValue->get<TForteByte>()));
    break;
  case CIEC_ANY::e_WORD:
    paDataOut->setValue(CIEC_WORD(paValue->get<TForteWord>()));
    break;
  case CIEC_ANY::e_DWORD:
    paDataOut->setValue(CIEC_DWORD(paValue->get<TForteDWord>()));
    break;
  case CIEC_ANY::e_LWORD:
    paDataOut->setValue(CIEC_LWORD(paValue->get<TForteLWord>()));
    break;
  case CIEC_ANY::e_REAL:
    paDataOut->setValue(CIEC_REAL(paValue->get<TForteFloat>()));
    break;
  case CIEC_ANY::e_LREAL:
    paDataOut->setValue(CIEC_LREAL(paValue->get<TForteDFloat>()));
    break;
  default:
    //TODO
    break;
  }
}

void COpcComLayer::processClientParams(char* paLayerParams){
  CParameterParser parser(paLayerParams, ':', ParameterAmount);
  if (ParameterAmount == parser.parseParameters()) {
    mHost = parser[Host];
    if (mHost.empty()) {
      return;
    }
    if (mHost.compare("127.0.0.1") == 0 || mHost.compare("localhost") == 0) {
      mHost = "";
    }

    mServerName = parser[ServerName];
    if (mServerName.empty()) {
      return;
    }

    std::string val = parser[UpdateRate];
    if (val.empty()) {
      return;
    }
    else {
      mUpdateRate = boost::lexical_cast<long>(val);
    }

    val = parser[DeadBand];
    if (val.empty()) {
      return;
    }
    else {
      mDeadBand = boost::lexical_cast<float>(val);
    }

    unsigned int nrItems = 0;
    unsigned int nrItemAmount = 0;
    const char* inputItems = parser[FbInputItems];
    if (inputItems != nullptr && inputItems[0] != '\0') {
      CParameterParser inputParser(inputItems, ',');
      nrItems = static_cast<unsigned int>(inputParser.parseParameters());
      if (nrItems > 0) {
        for(unsigned int i = 0; i < nrItems; i++){
          mFBInputVars.push_back(new COpcProcessVar(mOpcGroupName, inputParser[i], COpcProcessVar::e_FBInput));
          nrItemAmount++;
        }
      }
    }

    const char* outputItems = parser[FbOutputItems];
    if (outputItems != nullptr && outputItems[0] != '\0') {
      CParameterParser outputParser(outputItems, ',');
      nrItems = static_cast<unsigned int>(outputParser.parseParameters());
      if (nrItems > 0) {
        for(unsigned int i = 0; i < nrItems; i++){
          mFBOutputVars.push_back(new COpcProcessVar(mOpcGroupName, outputParser[i], COpcProcessVar::e_FBOutput));
          nrItemAmount++;
        }
      }
    }

    if(nrItemAmount > 0) {
      mLayerParamsOK = true;
    }
  }
  else {
    return;
  }
}

void COpcComLayer::convertInputData(void *paData, unsigned int paSize){
  CIEC_ANY **sDs = static_cast<CIEC_ANY**>(paData);
  unsigned int nrSDs = paSize;
  unsigned int sdIndex = 0;

  TOpcProcessVarList::iterator itVar = mFBInputVars.begin();

  while(sdIndex < nrSDs && itVar != mFBInputVars.end()){
    CIEC_ANY &dataIn(sDs[sdIndex]->unwrap());
    Variant newVariant;

    getInputValueSize(&dataIn, &newVariant);

    (*itVar)->setNewValue(newVariant);

    ++itVar;
    ++sdIndex;
  }
}

template<typename T>
void COpcComLayer::getInputValue(void * paData, Variant * paNewValue){
  T* vData = static_cast<T*>(paData);
  T data = vData[0];

  paNewValue->set<T>(data);
}

unsigned int COpcComLayer::getInputValueSize(CIEC_ANY* paData, Variant * paNewValue){
  switch (paData->getDataTypeID()){
  case CIEC_ANY::e_BOOL:
  {
    paNewValue->set<bool>((bool) *(dynamic_cast<CIEC_BOOL*>(paData)));
    return sizeof(bool);
  }
  case CIEC_ANY::e_SINT:
  {
    paNewValue->set<TForteInt8>((TForteInt8) *(dynamic_cast<CIEC_SINT*>(paData)));
    return sizeof(TForteInt8);
  }
  case CIEC_ANY::e_INT:
  {
    CIEC_INT* tempInt = dynamic_cast<CIEC_INT*>(paData);
    TForteInt16 forteInt = (TForteInt16) (*tempInt);
    paNewValue->set<TForteInt16>(forteInt);
    return sizeof(TForteInt16);
  }
  case CIEC_ANY::e_DINT:
  {
    paNewValue->set<TForteInt32>((TForteInt32) *(dynamic_cast<CIEC_DINT*>(paData)));
    return sizeof(TForteInt32);
  }
  case CIEC_ANY::e_LINT:
  {
    paNewValue->set<TForteInt64>((TForteInt64) *(dynamic_cast<CIEC_LINT*>(paData)));
    return sizeof(TForteInt64);
  }
  case CIEC_ANY::e_USINT:
  {
    paNewValue->set<TForteUInt8>((TForteUInt8) *(dynamic_cast<CIEC_USINT*>(paData)));
    return sizeof(TForteUInt8);
  }
  case CIEC_ANY::e_UINT:
  {
    paNewValue->set<TForteUInt16>((TForteUInt16) *(dynamic_cast<CIEC_UINT*>(paData)));
    return sizeof(TForteUInt16);
  }
  case CIEC_ANY::e_UDINT:
  {
    paNewValue->set<TForteUInt32>((TForteUInt32) *(dynamic_cast<CIEC_UDINT*>(paData)));
    return sizeof(TForteUInt32);
  }
  case CIEC_ANY::e_ULINT:
  {
    paNewValue->set<TForteUInt64>((TForteUInt64) *(dynamic_cast<CIEC_ULINT*>(paData)));
    return sizeof(TForteUInt64);
  }
  case CIEC_ANY::e_BYTE:
  {
    paNewValue->set<TForteByte>((TForteByte) *(dynamic_cast<CIEC_BYTE*>(paData)));
    return sizeof(TForteByte);
  }
  case CIEC_ANY::e_WORD:
  {
    paNewValue->set<TForteWord>((TForteWord) *(dynamic_cast<CIEC_WORD*>(paData)));
    return sizeof(TForteWord);
  }
  case CIEC_ANY::e_DWORD:
  {
    paNewValue->set<TForteDWord>((TForteDWord) *(dynamic_cast<CIEC_DWORD*>(paData)));
    return sizeof(TForteDWord);
  }
  case CIEC_ANY::e_LWORD:
  {
    paNewValue->set<TForteLWord>((TForteLWord) *(dynamic_cast<CIEC_LWORD*>(paData)));
    return sizeof(TForteLWord);
  }
  case CIEC_ANY::e_REAL:
  {
    paNewValue->set<TForteFloat>((TForteFloat) *(dynamic_cast<CIEC_REAL*>(paData)));
    return sizeof(TForteFloat);
  }
  case CIEC_ANY::e_LREAL:
  {
    paNewValue->set<TForteDFloat>((TForteDFloat) *(dynamic_cast<CIEC_LREAL*>(paData)));
    return sizeof(TForteDFloat);
  }
  default:
    //TODO
    break;
  }
  return 0;
}
