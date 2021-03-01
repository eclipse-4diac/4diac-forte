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

COpcComLayer::COpcComLayer(CComLayer* pa_poUpperLayer, CBaseCommFB* pa_poComFB) :
CComLayer(pa_poUpperLayer, pa_poComFB), m_acHost(0), m_acServerName(0), m_nUpdateRate(0), m_nDeadBand(0), m_bLayerParamsOK(false), m_pOpcConnection(0), m_eInterruptResp(e_Nothing){
  m_acOpcGroupName = m_poFb->getInstanceName();
}

COpcComLayer::~COpcComLayer(){
}

EComResponse COpcComLayer::sendData(void *pa_pvData, unsigned int pa_unSize){
  EComResponse eRetVal = e_ProcessDataOk;

  if((0 != m_poFb)){
    if(m_pOpcConnection->isConnected()){
      switch (m_poFb->getComServiceType()){
      case e_Server:
        //TODO
        break;
      case e_Client: {
        convertInputData(pa_pvData, pa_unSize);

        TOpcProcessVarList::Iterator itEnd = m_lFBInputVars.end();
        for(TOpcProcessVarList::Iterator it = m_lFBInputVars.begin(); it != itEnd; ++it){
            if(it->getIsActive()) {
              m_pOpcConnection->send_sendItemData((*it));
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
    CCriticalRegion criticalRegion(m_oSync);
    TComResponseList::Iterator comIt(m_lComResponses.begin());
    currentComResponse = *comIt;
    m_lComResponses.popFront();
  }

  switch (currentComResponse){
    case e_ProcessDataOk:
      switch (m_eConnectionState){
      case e_Connected: {
        CIEC_ANY *apoRDs = m_poFb->getRDs();
        unsigned int nrRDs = m_poFb->getNumRD();

        TOpcProcessVarList::Iterator itEnd = m_lFBOutputVars.end();
        TOpcProcessVarList::Iterator it = m_lFBOutputVars.begin();
        for(unsigned int i = 0; i < nrRDs && it != itEnd; i++, ++it){
          setOutputValue(&apoRDs[i], &it->updateValue());
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

  switch (m_poFb->getComServiceType()){
    case e_Server:
      //TODO
      break;
    case e_Client: {
      switch (m_pOpcConnection->getConnectionState()){
        case COpcConnection::e_Connected:
          // Successfully connected --> adding OPC items
          if(addOpcItems() < 0){
            //TODO
          } else {
            m_eConnectionState = e_Connected;
          }
          break;
        case COpcConnection::e_ConnectionFailed:
          {
            CCriticalRegion criticalRegion(m_oSync);
            m_lComResponses.pushBack(e_InitTerminated);
          }
          m_poFb->interruptCommFB(this);
          eRet = e_InitTerminated;
          break;
        case COpcConnection::e_ItemAddedOk:
          //do nothing
          break;
        case COpcConnection::e_ItemAddedFailed:
          {
            CCriticalRegion criticalRegion(m_oSync);
            m_lComResponses.pushBack(e_InitTerminated);
          }
          m_poFb->interruptCommFB(this);
          eRet = e_InitTerminated;
          break;
        case COpcConnection::e_DataReceived: {
          int nRetVal = m_pOpcConnection->receiveData(m_acOpcGroupName, &m_lFBOutputVars);
          if (nRetVal > 0) {
            //we successfully received data
            CCriticalRegion criticalRegion(m_oSync);
            m_lComResponses.pushBack(e_ProcessDataOk);
          }
          m_poFb->interruptCommFB(this);
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

  EComResponse COpcComLayer::openConnection(char *pa_acLayerParameter){
    EComResponse eRetVal = e_InitInvalidId;
    m_eConnectionState = e_Disconnected;

    switch (m_poFb->getComServiceType()){
    case e_Server:
      //TODO
      break;
    case e_Client:
      processClientParams(pa_acLayerParameter);
      if(m_bLayerParamsOK){
        eRetVal = e_InitOk;
        m_pOpcConnection = COpcConnectionHandler::getInstance().getOpcConnection(m_acHost, m_acServerName, m_acOpcGroupName, m_nUpdateRate, m_nDeadBand, this);

        m_pOpcConnection->send_connect();

        COpcConnection::EOpcConnectionEvents connState = m_pOpcConnection->getConnectionState();
        if (connState == COpcConnection::e_Connected) {
          m_eConnectionState = e_Connected;
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
    COpcConnectionHandler::getInstance().removeOpcConnection(m_pOpcConnection->getHost(), m_pOpcConnection->getServerName(), m_acOpcGroupName);

    m_eConnectionState = e_Disconnected;
  }

  int COpcComLayer::addOpcItems(){
    int result = 0;

    // Add input items
    TOpcProcessVarList::Iterator itEnd = m_lFBInputVars.end();
    for(TOpcProcessVarList::Iterator it = m_lFBInputVars.begin(); it != itEnd; ++it){
      result = m_pOpcConnection->send_addItem(*it);
    }

    // Add output items
    itEnd = m_lFBOutputVars.end();
    for(TOpcProcessVarList::Iterator it = m_lFBOutputVars.begin(); it != itEnd; ++it){
      result = m_pOpcConnection->send_addItem(*it);
    }

    return result;
  }

  void COpcComLayer::setOutputValue(CIEC_ANY *pa_pDataOut, Variant * pa_pValue){
    switch (pa_pDataOut->getDataTypeID()){
    case CIEC_ANY::e_BOOL:
      pa_pDataOut->setValue(CIEC_BOOL(pa_pValue->get<bool>()));
      break;
    case CIEC_ANY::e_SINT:
      pa_pDataOut->setValue(CIEC_SINT(pa_pValue->get<TForteInt8>()));
      break;
    case CIEC_ANY::e_INT:
      pa_pDataOut->setValue(CIEC_INT(pa_pValue->get<TForteInt16>()));
      break;
    case CIEC_ANY::e_DINT:
      pa_pDataOut->setValue(CIEC_DINT(pa_pValue->get<TForteInt32>()));
      break;
    case CIEC_ANY::e_LINT:
      pa_pDataOut->setValue(CIEC_LINT(pa_pValue->get<TForteInt64>()));
      break;
    case CIEC_ANY::e_USINT:
      pa_pDataOut->setValue(CIEC_USINT(pa_pValue->get<TForteUInt8>()));
      break;
    case CIEC_ANY::e_UINT:
      pa_pDataOut->setValue(CIEC_UINT(pa_pValue->get<TForteUInt16>()));
      break;
    case CIEC_ANY::e_UDINT:
      pa_pDataOut->setValue(CIEC_UDINT(pa_pValue->get<TForteUInt32>()));
      break;
    case CIEC_ANY::e_ULINT:
      pa_pDataOut->setValue(CIEC_ULINT(pa_pValue->get<TForteUInt64>()));
      break;
    case CIEC_ANY::e_BYTE:
      pa_pDataOut->setValue(CIEC_BYTE(pa_pValue->get<TForteByte>()));
      break;
    case CIEC_ANY::e_WORD:
      pa_pDataOut->setValue(CIEC_WORD(pa_pValue->get<TForteWord>()));
      break;
    case CIEC_ANY::e_DWORD:
      pa_pDataOut->setValue(CIEC_DWORD(pa_pValue->get<TForteDWord>()));
      break;
    case CIEC_ANY::e_LWORD:
      pa_pDataOut->setValue(CIEC_LWORD(pa_pValue->get<TForteLWord>()));
      break;
    case CIEC_ANY::e_REAL:
      pa_pDataOut->setValue(CIEC_REAL(pa_pValue->get<TForteFloat>()));
      break;
    case CIEC_ANY::e_LREAL:
      pa_pDataOut->setValue(CIEC_LREAL(pa_pValue->get<TForteDFloat>()));
      break;
    default:
      //TODO
      break;
    }
  }

  void COpcComLayer::processClientParams(char* pa_acLayerParams){

    char *chrStorage;
    char *chrHost;
    char *chrServer;
    char *temp;

    // Get Host
    chrStorage = strchr(pa_acLayerParams, ':');
  if(chrStorage == 0) {
      return;
  }
    *chrStorage = '\0';
    ++chrStorage;
    chrHost = (char*) malloc(strlen(pa_acLayerParams) + 1);
    strcpy(chrHost, pa_acLayerParams);
  if(strcmp(chrHost, "127.0.0.1") == 0 || strcmp(chrHost, "localhost") == 0) {
      m_acHost = "";
  } else {
    m_acHost = chrHost;
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
    m_acServerName = chrServer;

    // Get update rate
    m_nUpdateRate = atol(chrStorage);
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
    m_nDeadBand = (float) atof(chrStorage);
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
    while(pch != NULL){
      char *itemName = (char*) malloc(strlen(pch) + 1);
      strcpy(itemName, pch);
      m_lFBInputVars.pushBack(new COpcProcessVar(m_acOpcGroupName, itemName, COpcProcessVar::e_FBInput));
      nrItems++;
      pch = strtok(NULL, ",");
    }

    // Get FB output items
    pch = strtok(chrStorage, ",");
    while(pch != NULL){
      char *itemName = (char*) malloc(strlen(pch) + 1);
      strcpy(itemName, pch);
      m_lFBOutputVars.pushBack(new COpcProcessVar(m_acOpcGroupName, itemName, COpcProcessVar::e_FBOutput));
      nrItems++;

      pch = strtok(NULL, ",");
    }

  if(nrItems > 0) {
    m_bLayerParamsOK = true;
  }

  }

  void COpcComLayer::convertInputData(void *pa_pData, unsigned int pa_nSize){
    CIEC_ANY *apoSDs =  static_cast<CIEC_ANY*>(pa_pData);
    unsigned int nrSDs = pa_nSize;
    unsigned int sdIndex = 0;

    TOpcProcessVarList::Iterator it_var = m_lFBInputVars.begin();
    TOpcProcessVarList::Iterator itEnd = m_lFBInputVars.end();

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
  void COpcComLayer::getInputValue(void * pa_pData, Variant * pa_pNewValue){
    T* vData = (T*) pa_pData;
    T data = vData[0];

    pa_pNewValue->set<T>(data);
  }

  unsigned int COpcComLayer::getInputValueSize(CIEC_ANY* pa_pData, Variant * pa_pNewValue){
    switch (pa_pData->getDataTypeID()){
    case CIEC_ANY::e_BOOL:
    {
      pa_pNewValue->set<bool>((bool) *(dynamic_cast<CIEC_BOOL*>(pa_pData)));
      return sizeof(bool);
    }
    case CIEC_ANY::e_SINT:
    {
      pa_pNewValue->set<CHAR>((CHAR) *(dynamic_cast<CIEC_SINT*>(pa_pData)));
      return sizeof(TForteInt8);
    }
    case CIEC_ANY::e_INT:
    {
      CIEC_INT* tempInt = dynamic_cast<CIEC_INT*>(pa_pData);
      TForteInt16 forteInt = (TForteInt16) (*tempInt);
      pa_pNewValue->set<TForteInt16>(forteInt);
      return sizeof(TForteInt16);
    }
    case CIEC_ANY::e_DINT:
    {
      pa_pNewValue->set<TForteInt32>((TForteInt32) *(dynamic_cast<CIEC_DINT*>(pa_pData)));
      return sizeof(TForteInt32);
    }
    case CIEC_ANY::e_LINT:
    {
      pa_pNewValue->set<TForteInt64>((TForteInt64) *(dynamic_cast<CIEC_LINT*>(pa_pData)));
      return sizeof(TForteInt64);
    }
    case CIEC_ANY::e_USINT:
    {
      pa_pNewValue->set<TForteUInt8>((TForteUInt8) *(dynamic_cast<CIEC_USINT*>(pa_pData)));
      return sizeof(TForteUInt8);
    }
    case CIEC_ANY::e_UINT:
    {
      pa_pNewValue->set<TForteUInt16>((TForteUInt16) *(dynamic_cast<CIEC_UINT*>(pa_pData)));
      return sizeof(TForteUInt16);
    }
    case CIEC_ANY::e_UDINT:
    {
      pa_pNewValue->set<TForteUInt32>((TForteUInt32) *(dynamic_cast<CIEC_UDINT*>(pa_pData)));
      return sizeof(TForteUInt32);
    }
    case CIEC_ANY::e_ULINT:
    {
      pa_pNewValue->set<TForteUInt64>((TForteUInt64) *(dynamic_cast<CIEC_ULINT*>(pa_pData)));
      return sizeof(TForteUInt64);
    }
    case CIEC_ANY::e_BYTE:
    {
      pa_pNewValue->set<TForteByte>((TForteByte) *(dynamic_cast<CIEC_BYTE*>(pa_pData)));
      return sizeof(TForteByte);
    }
    case CIEC_ANY::e_WORD:
    {
      pa_pNewValue->set<TForteWord>((TForteWord) *(dynamic_cast<CIEC_WORD*>(pa_pData)));
      return sizeof(TForteWord);
    }
    case CIEC_ANY::e_DWORD:
    {
      pa_pNewValue->set<TForteDWord>((TForteDWord) *(dynamic_cast<CIEC_DWORD*>(pa_pData)));
      return sizeof(TForteDWord);
    }
    case CIEC_ANY::e_LWORD:
    {
      pa_pNewValue->set<TForteLWord>((TForteLWord) *(dynamic_cast<CIEC_LWORD*>(pa_pData)));
      return sizeof(TForteLWord);
    }
    case CIEC_ANY::e_REAL:
    {
      pa_pNewValue->set<TForteFloat>((TForteFloat) *(dynamic_cast<CIEC_REAL*>(pa_pData)));
      return sizeof(TForteFloat);
    }
    case CIEC_ANY::e_LREAL:
    {
      pa_pNewValue->set<TForteDFloat>((TForteDFloat) *(dynamic_cast<CIEC_LREAL*>(pa_pData)));
      return sizeof(TForteDFloat);
    }
    default:
      //TODO
      break;
    }

    return 0;
  }
