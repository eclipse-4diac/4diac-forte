/*******************************************************************************
 * Copyright (c) 2012 -2021 AIT, ACIN, fortiss GmbH, Hit robot group
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Patrick Smejkal, Alois Zoitl, Martin Melik-Merkumians - initial API and implementation and/or initial documentation
 *   ys guo - Fix opc module compilation errors and deadlock bug
 *******************************************************************************/
#include "opccomlayer.h"
#include "../../arch/devlog.h"
#include "commfb.h"

#include "opcconnection.h"
#include "opcconnectionhandler.h"
#include "Variant.h"
#include <criticalregion.h>

using namespace forte::com_infra;

COpcComLayer::COpcComLayer(CComLayer* paUpperLayer, CBaseCommFB* paComFB) :
CComLayer(paUpperLayer, paComFB), mHost(0), mServerName(0), mUpdateRate(0), mDeadBand(0), mLayerParamsOK(false), mOpcConnection(0), mInterruptResp(e_Nothing){
  mOpcGroupName = mFb->getInstanceName();
}

COpcComLayer::~COpcComLayer() = default;

EComResponse COpcComLayer::sendData(void *paData, unsigned int paSize){
  EComResponse eRetVal = e_ProcessDataOk;

  if((0 != mFb)){
    if(mOpcConnection->isConnected()){
      switch (mFb->getComServiceType()){
      case e_Server:
        //TODO
        break;
      case e_Client: {
        convertInputData(paData, paSize);

        TOpcProcessVarList::Iterator itEnd = mFBInputVars.end();
        for(TOpcProcessVarList::Iterator it = mFBInputVars.begin(); it != itEnd; ++it){
            if(it->getIsActive()) {
              mOpcConnection->send_sendItemData((*it));
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
  }
  return eRetVal;
}

EComResponse COpcComLayer::processInterrupt(){
  EComResponse eRet = e_Nothing;
  EComResponse currentComResponse;

  {
    CCriticalRegion criticalRegion(mSync);
    TComResponseList::Iterator comIt(mComResponses.begin());
    currentComResponse = *comIt;
    mComResponses.popFront();
  }

  switch (currentComResponse){
    case e_ProcessDataOk:
      switch (mConnectionState){
      case e_Connected: {
        CIEC_ANY **apoRDs = mFb->getRDs();
        unsigned int nrRDs = mFb->getNumRD();

        TOpcProcessVarList::Iterator itEnd = mFBOutputVars.end();
        TOpcProcessVarList::Iterator it = mFBOutputVars.begin();
        for(unsigned int i = 0; i < nrRDs && it != itEnd; i++, ++it){
          setOutputValue(apoRDs[i], &it->updateValue());
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
      switch (mOpcConnection->getConnectionState()){
        case COpcConnection::e_Connected:
          // Successfully connected --> adding OPC items
          if(addOpcItems() < 0){
            //TODO
          } else {
            mConnectionState = e_Connected;
          }
          break;
        case COpcConnection::e_ConnectionFailed:
          {
            CCriticalRegion criticalRegion(mSync);
            mComResponses.pushBack(e_InitTerminated);
          }
          mFb->interruptCommFB(this);
          eRet = e_InitTerminated;
          break;
        case COpcConnection::e_ItemAddedOk:
          //do nothing
          break;
        case COpcConnection::e_ItemAddedFailed:
          {
            CCriticalRegion criticalRegion(mSync);
            mComResponses.pushBack(e_InitTerminated);
          }
          mFb->interruptCommFB(this);
          eRet = e_InitTerminated;
          break;
        case COpcConnection::e_DataReceived: {
          int nRetVal = mOpcConnection->receiveData(mOpcGroupName, &mFBOutputVars);
          if (nRetVal > 0) {
            //we successfully received data
            CCriticalRegion criticalRegion(mSync);
            mComResponses.pushBack(e_ProcessDataOk);
          }
          mFb->interruptCommFB(this);
          eRet = e_ProcessDataOk;
          break;
        }
        case COpcConnection::e_Disconnected:
          //TODO error during connection try
          break;
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
        mOpcConnection = COpcConnectionHandler::getInstance().getOpcConnection(mHost, mServerName, mOpcGroupName, mUpdateRate, mDeadBand, this);

        mOpcConnection->send_connect();

        COpcConnection::EOpcConnectionEvents connState = mOpcConnection->getConnectionState();
        if (connState == COpcConnection::e_Connected) {
          mConnectionState = e_Connected;
          addOpcItems();
        }
        else if(connState == COpcConnection::e_ConnectionFailed) {
          eRetVal = e_InitTerminated;
        }
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
    DEVLOG_DEBUG("COpcComLayer::closeConnection() \n");
    COpcConnectionHandler::getInstance().removeOpcConnection(mOpcConnection->getHost(), mOpcConnection->getServerName(), mOpcGroupName);

    mConnectionState = e_Disconnected;
  }

  int COpcComLayer::addOpcItems(){
    int result = 0;

    // Add input items
    TOpcProcessVarList::Iterator itEnd = mFBInputVars.end();
    for(TOpcProcessVarList::Iterator it = mFBInputVars.begin(); it != itEnd; ++it){
      result = mOpcConnection->send_addItem(*it);
    }

    // Add output items
    itEnd = mFBOutputVars.end();
    for(TOpcProcessVarList::Iterator it = mFBOutputVars.begin(); it != itEnd; ++it){
      result = mOpcConnection->send_addItem(*it);
    }

    return result;
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

    char *chrStorage;
    char *chrHost;
    char *chrServer;
    char *temp;

    // Get Host
    chrStorage = strchr(paLayerParams, ':');
  if(chrStorage == 0) {
      return;
  }
    *chrStorage = '\0';
    ++chrStorage;
    chrHost = (char*) malloc(strlen(paLayerParams) + 1);
    strcpy(chrHost, paLayerParams);
  if(strcmp(chrHost, "127.0.0.1") == 0 || strcmp(chrHost, "localhost") == 0) {
      mHost = "";
  } else {
    mHost = chrHost;
  }

    // Get server name
    temp = chrStorage;
    chrStorage = strchr(chrStorage, ':');
    if(chrStorage == 0){
      if (chrHost){
        free(chrHost);
      }
      return;
    }
    *chrStorage = '\0';
    ++chrStorage;
    chrServer = (char*) malloc(strlen(temp) + 1);
    strcpy(chrServer, temp);
    mServerName = chrServer;

    // Get update rate
    mUpdateRate = atol(chrStorage);
    chrStorage = strchr(chrStorage, ':');
    if(chrStorage == 0){
      if (chrHost){
        free(chrHost);
      }
      return;
    }
    *chrStorage = '\0';
    ++chrStorage;

    // Get dead band
    mDeadBand = (float) atof(chrStorage);
    chrStorage = strchr(chrStorage, ':');
    if(chrStorage == 0){
      if (chrHost){
        free(chrHost);
      }
      return;
    }

    *chrStorage = '\0';
    ++chrStorage;

    // Get FB input items
    char * inputItems = chrStorage;
    chrStorage = strchr(chrStorage, ':');
    if(chrStorage == 0){
      if (chrHost){
        free(chrHost);
      }
      return;
    }
    *chrStorage = '\0';
    ++chrStorage;
    int nrItems = 0;
    char *pch;
    pch = strtok(inputItems, ",");
    while(pch != nullptr){
      char *itemName = (char*) malloc(strlen(pch) + 1);
      strcpy(itemName, pch);
      mFBInputVars.pushBack(new COpcProcessVar(mOpcGroupName, itemName, COpcProcessVar::e_FBInput));
      nrItems++;
      pch = strtok(nullptr, ",");
    }

    // Get FB output items
    pch = strtok(chrStorage, ",");
    while(pch != nullptr){
      char *itemName = (char*) malloc(strlen(pch) + 1);
      strcpy(itemName, pch);
      mFBOutputVars.pushBack(new COpcProcessVar(mOpcGroupName, itemName, COpcProcessVar::e_FBOutput));
      nrItems++;

      pch = strtok(nullptr, ",");
    }

  if(nrItems > 0) {
    mLayerParamsOK = true;
  }

  }

  void COpcComLayer::convertInputData(void *paData, unsigned int paSize){
    CIEC_ANY *apoSDs =  static_cast<CIEC_ANY*>(paData);
    unsigned int nrSDs = paSize;
    unsigned int sdIndex = 0;

    TOpcProcessVarList::Iterator it_var = mFBInputVars.begin();
    TOpcProcessVarList::Iterator itEnd = mFBInputVars.end();

    while(sdIndex < nrSDs && it_var != itEnd){
      CIEC_ANY *dataIn = &apoSDs[sdIndex];
      Variant newVariant;

      getInputValueSize(dataIn, &newVariant);

      it_var->setNewValue(newVariant);

      ++it_var;
      ++sdIndex;
    }

  }

  template<typename T>
  void COpcComLayer::getInputValue(void * paData, Variant * paNewValue){
    T* vData = (T*) paData;
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
      paNewValue->set<CHAR>((CHAR) *(dynamic_cast<CIEC_SINT*>(paData)));
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
