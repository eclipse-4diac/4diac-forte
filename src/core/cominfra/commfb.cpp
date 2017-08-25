/*******************************************************************************
  * Copyright (c) 2006-2014 ACIN, Profactor GmbH, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Rene Smodic, Alois Zoitl, Michael Hofmann, Martin Melik Merkumians,
  *    Patrick Smejkal
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#include <fortenew.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "commfb.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "commfb_gen.cpp"
#endif
#include "../resource.h"
#include "comlayer.h"
#include "comlayersmanager.h"

using namespace forte::com_infra;

const CStringDictionary::TStringId CCommFB::scm_aunRequesterEventInputNameIds[2] = { g_nStringIdINIT, g_nStringIdREQ };
const CStringDictionary::TStringId CCommFB::scm_aunRequesterEventOutputNameIds[2] = { g_nStringIdINITO, g_nStringIdCNF };

const CStringDictionary::TStringId CCommFB::scm_aunResponderEventInputNameIds[2] = { g_nStringIdINIT, g_nStringIdRSP };
const CStringDictionary::TStringId CCommFB::scm_aunResponderEventOutputNameIds[2] = { g_nStringIdINITO, g_nStringIdIND };

const TForteInt16 CCommFB::scm_anEIWithIndexes[] = { 0, 3 };
const TForteInt16 CCommFB::scm_anEOWithIndexes[] = { 0, 3, -1 };

const char * const CCommFB::scm_sResponseTexts[] = { "OK", "INVALID_ID", "TERMINATED", "INVALID_OBJECT", "DATA_TYPE_ERROR", "INHIBITED", "NO_SOCKET", "SEND_FAILED", "RECV_FAILED" };

const char * const CCommFB::scmDefaultIDPrefix = "fbdk[].ip[";
const char * const CCommFB::scmDefaultIDSuffix = "]";

CCommFB::CCommFB(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes, forte::com_infra::EComServiceType pa_eCommServiceType) :
    CEventSourceFB(pa_poSrcRes, 0, pa_nInstanceNameId, 0, 0), m_eCommServiceType(pa_eCommServiceType), m_poTopOfComStack(0){
  memset(m_apoInterruptQueue, 0, sizeof(m_apoInterruptQueue)); //TODO change this to  m_apoInterruptQueue{0} in the extended list when fully switching to C++11
  setEventChainExecutor(getResource().getResourceEventExecution());
  m_unComInterruptQueueCount = 0;
  m_nConfiguredFBTypeNameId = CStringDictionary::scm_nInvalidStringId;
}

CCommFB::CCommFB(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes, const SFBInterfaceSpec *pa_pstInterfaceSpec,
    TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData, forte::com_infra::EComServiceType pa_eCommServiceType) :
    CEventSourceFB(pa_poSrcRes, pa_pstInterfaceSpec, pa_nInstanceNameId, pa_acFBConnData, pa_acFBVarsData), m_eCommServiceType(pa_eCommServiceType), m_poTopOfComStack(0){
  memset(m_apoInterruptQueue, 0, sizeof(m_apoInterruptQueue)); //TODO change this to  m_apoInterruptQueue{0} in the extended list when fully switching to C++11
  setEventChainExecutor(getResource().getResourceEventExecution());
  m_unComInterruptQueueCount = 0;
  m_nConfiguredFBTypeNameId = CStringDictionary::scm_nInvalidStringId;
}

CCommFB::~CCommFB(){
  closeConnection();

  if((getManagesFBData()) && (0 != m_pstInterfaceSpec)){
    //Free the memory allocated for the interface, only do this if we dynamically created it (i.e., getManagesFBData is true)
    delete[] (m_pstInterfaceSpec->m_anEIWith);
    delete[] (m_pstInterfaceSpec->m_anEOWith);
    delete[] (m_pstInterfaceSpec->m_aunDINames);
    delete[] (m_pstInterfaceSpec->m_aunDIDataTypeNames);
    delete[] (m_pstInterfaceSpec->m_aunDONames);
    delete[] (m_pstInterfaceSpec->m_aunDODataTypeNames);
  }
}

EMGMResponse CCommFB::changeFBExecutionState(EMGMCommandType pa_unCommand){
  EMGMResponse retVal = CEventSourceFB::changeFBExecutionState(pa_unCommand);
  if((e_RDY == retVal) && (cg_nMGM_CMD_Kill == pa_unCommand)){
    //when we are killed we'll close the connection so that it can safely be opened again after an reset
    closeConnection();
  }
  return retVal;
}

void CCommFB::executeEvent(int pa_nEIID){
  EComResponse resp = e_Nothing;

  switch (pa_nEIID){
    case scm_nEventINITID:
      if(true == QI()){
        resp = openConnection();
      }
      else{
        closeConnection();
        resp = e_InitTerminated;
      }
      break;
    case scm_nSendNotificationEventID:
      resp = sendData();
      break;
    case cg_nExternalEventID:
      resp = receiveData();
      break;
    default:
      break;
  }

  if(resp & e_Terminated){
    if(m_eCommServiceType == e_Server){
      //servers will not send information on client termination and should silently start to listen again
      resp = e_Nothing;
    }
    else{
      //subscribers and clients will close the connection and inform the user
      closeConnection();
    }
  }

  if(e_Nothing != resp){
    STATUS() = scm_sResponseTexts[resp & 0xF];
    QO() = !(resp & scg_unComNegative);

    if(scg_unINIT & resp){
      sendOutputEvent(scm_nEventINITOID);
    }
    else{
      sendOutputEvent(scm_nReceiveNotificationEventID);
    }
  }
}

EComResponse CCommFB::sendData(){
  EComResponse resp = e_Nothing;
  if(true == QI()){
    if(m_eCommServiceType != e_Subscriber){
      if(0 != m_poTopOfComStack){
	    resp = m_poTopOfComStack->sendData(static_cast<void*>(getSDs()), m_pstInterfaceSpec->m_nNumDIs - 2);
	    if((resp == e_ProcessDataOk) && (m_eCommServiceType != e_Publisher)){
	      // client and server will not directly send a cnf/ind event
	      resp = e_Nothing;
	    }
      }else{
	    resp = e_ProcessDataNoSocket;
      }
    }
  }else{
    resp = e_ProcessDataInhibited; // we are not allowed to send data
  }
  return resp;
}

bool CCommFB::configureFB(const char *pa_acConfigString){
  TIdentifier tempstring;
  char *sParamA = 0;
  char *sParamB = 0;
  size_t inlength;
  unsigned int i;

  TForteUInt8 nNumDIs;
  CStringDictionary::TStringId* paun_DIDataTypeNames;
  CStringDictionary::TStringId* paun_DINames;
  TDataIOID* pan_EIWith;

  TForteUInt8 nNumDOs;
  CStringDictionary::TStringId* paun_DODataTypeNames;
  CStringDictionary::TStringId* paun_DONames;
  TDataIOID* pan_EOWith;

  CStringDictionary::TStringId* paun_EINames;
  CStringDictionary::TStringId* paun_EONames;

  TForteUInt8 nNumEIs = 2;
  TForteUInt8 nNumEOs = 2;

  m_nConfiguredFBTypeNameId = CStringDictionary::getInstance().insert(pa_acConfigString);

  memcpy(tempstring, pa_acConfigString, cg_nIdentifierLength);
  tempstring[cg_nIdentifierLength] = '\0';
  inlength = strlen(tempstring);
  for(i = 0; i < inlength - 1; i++){ // search first underscore
    if(tempstring[i] == '_'){
      sParamA = sParamB = &(tempstring[i + 1]);
      break;
    }
  }
  if(sParamA != 0) // search for 2nd underscore
    for(i = i + 1; i < inlength - 1; i++){
      if(tempstring[i] == '_'){
        tempstring[i] = '\0';
        sParamB = &(tempstring[i + 1]);
        break;
      }
    }

  if(sParamB == 0) // no underscore found
    return false;

  nNumDIs = 2;
  nNumDOs = 2;

  if(forte::com_infra::e_DataInputs == (forte::com_infra::e_DataInputs & m_eCommServiceType)){
    //TODO: Check range of sParamA
    nNumDIs = static_cast<TForteUInt8>(nNumDIs + forte::core::util::strtol(sParamA, 0, 10));
    paun_DIDataTypeNames = new CStringDictionary::TStringId[nNumDIs];
    paun_DINames = new CStringDictionary::TStringId[nNumDIs];
    pan_EIWith = new TDataIOID[nNumDIs - 2 + 6];

    generateGenericDataPointArrays("SD_", &(paun_DIDataTypeNames[2]), &(paun_DINames[2]), nNumDIs - 2);
  }
  else{
    paun_DIDataTypeNames = new CStringDictionary::TStringId[nNumDIs];
    paun_DINames = new CStringDictionary::TStringId[nNumDIs];
    pan_EIWith = new TDataIOID[6]; // = m_pstInterfaceSpec->m_nNumDIs - 2 + 6
  }

  pan_EIWith[0] = 0;
  pan_EIWith[1] = 1;
  pan_EIWith[2] = 255;
  pan_EIWith[3] = 0;
  pan_EIWith[4] = 1;

  for(i = 0; i < nNumDIs - 2U; i++){
    pan_EIWith[i + 5U] = static_cast<TForteUInt8>(i + 2U);
  }
  pan_EIWith[i + 5] = 255;

  paun_DIDataTypeNames[0] = g_nStringIdBOOL;
  paun_DINames[0] = g_nStringIdQI;
#ifdef FORTE_USE_WSTRING_DATATYPE
  paun_DIDataTypeNames[1] = g_nStringIdWSTRING;
#else
  paun_DIDataTypeNames[1] = g_nStringIdSTRING;
#endif
  paun_DINames[1] = g_nStringIdID;

  if(forte::com_infra::e_DataOutputs == (forte::com_infra::e_DataOutputs & m_eCommServiceType)){
    //TODO: Check range of sParamA
    nNumDOs = static_cast<TForteUInt8>(nNumDOs + forte::core::util::strtol(sParamB, 0, 10));
    paun_DODataTypeNames = new CStringDictionary::TStringId[nNumDOs];
    paun_DONames = new CStringDictionary::TStringId[nNumDOs];
    pan_EOWith = new TDataIOID[nNumDOs - 2 + 6];

    generateGenericDataPointArrays("RD_", &(paun_DODataTypeNames[2]), &(paun_DONames[2]), nNumDOs - 2);
  }
  else{
    paun_DODataTypeNames = new CStringDictionary::TStringId[nNumDOs];
    paun_DONames = new CStringDictionary::TStringId[nNumDOs];
    pan_EOWith = new TDataIOID[6];
  }

  pan_EOWith[0] = 0;
  pan_EOWith[1] = 1;
  pan_EOWith[2] = 255;
  pan_EOWith[3] = 0;
  pan_EOWith[4] = 1;

  for(i = 0; i < nNumDOs - 2U; i++){
    pan_EOWith[i + 5] = static_cast<TForteUInt8>(i + 2);
  }
  pan_EOWith[i + 5] = 255;

  paun_DODataTypeNames[0] = g_nStringIdBOOL;
  paun_DONames[0] = g_nStringIdQO;
#ifdef FORTE_USE_WSTRING_DATATYPE
  paun_DODataTypeNames[1] = g_nStringIdWSTRING;
#else
  paun_DODataTypeNames[1] = g_nStringIdSTRING;
#endif
  paun_DONames[1] = g_nStringIdSTATUS;

  if(forte::com_infra::e_Requester == (forte::com_infra::e_Requester & m_eCommServiceType)){
    paun_EINames = const_cast<CStringDictionary::TStringId*>(scm_aunRequesterEventInputNameIds);
    paun_EONames = const_cast<CStringDictionary::TStringId*>(scm_aunRequesterEventOutputNameIds);
  }
  else{
    if(forte::com_infra::e_Responder == (forte::com_infra::e_Responder & m_eCommServiceType)){
      paun_EINames = const_cast<CStringDictionary::TStringId*>(scm_aunResponderEventInputNameIds);
      paun_EONames = const_cast<CStringDictionary::TStringId*>(scm_aunResponderEventOutputNameIds);
    }
  }

  setupFBInterface(new SFBInterfaceSpecforGenerics(nNumEIs, const_cast<const CStringDictionary::TStringId * const >(paun_EINames), const_cast<const TDataIOID * const >(pan_EIWith), scm_anEIWithIndexes, nNumEOs, const_cast<const CStringDictionary::TStringId * const >(paun_EONames), const_cast<const TDataIOID * const >(pan_EOWith), scm_anEOWithIndexes, nNumDIs, const_cast<const CStringDictionary::TStringId * const >(paun_DINames), const_cast<const CStringDictionary::TStringId * const >(paun_DIDataTypeNames), nNumDOs, const_cast<const CStringDictionary::TStringId * const >(paun_DONames), const_cast<const CStringDictionary::TStringId * const >(paun_DODataTypeNames)), new TForteByte[genFBConnDataSize(nNumEOs, nNumDIs, nNumDOs)], new TForteByte[genFBVarsDataSize(nNumDIs, nNumDOs)], true //we want that the FB will delete the two data arrays
  );
  return true;
}

EComResponse CCommFB::openConnection(){
  EComResponse retVal;
  if(0 == m_poTopOfComStack){
    // Get the ID
    char *commID;
    if(0 == strchr(ID().getValue(), ']')){
      commID = getDefaultIDString(ID().getValue());
    }
    else{
      commID = new char[strlen(ID().getValue()) + 1];
      strcpy(commID, ID().getValue());
    }

    // If the ID is empty return an error
    if('\0' == *commID){
      retVal = e_InitInvalidId;
    }
    else {
    	retVal = createComstack(commID);
      // If any error is going to be returned, delete the layers that were created
      if(e_InitOk != retVal){
        closeConnection();
      }
    }
    delete [] commID;
  }
  else{
    // If the connection was already opened return ok
    retVal = e_InitOk;
  }
  return retVal;
}

EComResponse CCommFB::createComstack(char *commID){
  EComResponse retVal;
  CComLayer *newLayer;
  CComLayer *previousLayer = 0; // Reference to the previous layer as it needs to set the bottom layer
  char *layerParams;
  // Loop until reaching the end of the ID
  while('\0' != *commID){
    // Get the next layer's ID and parameters
    char * layerID = extractLayerIdAndParams(&commID, &layerParams);
    // If not well formated ID return an error
    if((0 == commID) && ('\0' != *layerID)){
      retVal = e_InitInvalidId;
      break;
    }

    // Create the new layer
    newLayer = CComLayersManager::createCommunicationLayer(layerID, previousLayer, this);
    if(0 == newLayer){
      // If the layer can't be created return an error
      retVal = e_InitInvalidId;
      break;
    }
    else if(0 == m_poTopOfComStack){
      // Assign the newly created layer to the FB
      m_poTopOfComStack = newLayer;
    }

    // Update the previous layer reference
    previousLayer = newLayer;

    // Open the layer connection
    retVal = newLayer->openConnection(layerParams);
    if(e_InitOk != retVal){
      // If it was not opened correctly return the error
      break;
    }
  }
  return retVal;
}


void CCommFB::closeConnection(){
  if(m_poTopOfComStack != 0){
    m_poTopOfComStack->closeConnection();
    delete m_poTopOfComStack; // this will close the whole communication stack
    m_poTopOfComStack = 0;
  }
}

EComResponse CCommFB::receiveData(){
  EComResponse eResp;
  EComResponse eRetVal = e_Nothing;

  unsigned int ComInterruptQueueCountCopy = m_unComInterruptQueueCount;
  for(unsigned int i = 0; i < ComInterruptQueueCountCopy; ++i){
    eResp = m_apoInterruptQueue[i]->processInterrupt();
    if(eResp > eRetVal){
      eRetVal = eResp;
    }
  }
  m_unComInterruptQueueCount -= ComInterruptQueueCountCopy;
  for(unsigned int i = 0; i < m_unComInterruptQueueCount; ++i){
	m_apoInterruptQueue[i] = m_apoInterruptQueue[i + ComInterruptQueueCountCopy];
  }

  return eRetVal;
}

void CCommFB::interruptCommFB(CComLayer *pa_poComLayer){
  if(cg_unCommunicationInterruptQueueSize > m_unComInterruptQueueCount){
    m_apoInterruptQueue[m_unComInterruptQueueCount] = pa_poComLayer;
    m_unComInterruptQueueCount++;
  }
  else{
    //TODO to many interrupts received issue error msg
  }
}

char *CCommFB::extractLayerIdAndParams(char **paRemainingID, char **paLayerParams){
  char *LayerID = *paRemainingID;
  if('\0' != **paRemainingID){
    *paRemainingID = strchr(*paRemainingID, '[');
    if(0 != *paRemainingID){
      **paRemainingID = '\0';
      ++*paRemainingID;
      *paLayerParams = *paRemainingID;
      *paRemainingID = strchr(*paRemainingID, ']');
      if(0 != *paRemainingID){
        **paRemainingID = '\0';
        ++*paRemainingID;
        if('\0' != **paRemainingID){
          ++*paRemainingID;
        }
      }
    }
  }
  return LayerID;
}

char *CCommFB::buildIDString(const char *paPrefix, const char *paIDRoot, const char *paSuffix){
  char * RetVal = new char[strlen(paPrefix) + strlen(paIDRoot) + strlen(paSuffix) + 1];
  strcpy(RetVal, paPrefix);
  strcat(RetVal, paIDRoot);
  strcat(RetVal, paSuffix);
  return RetVal;
}

char * CCommFB::getDefaultIDString(const char *paID){
  return buildIDString(scmDefaultIDPrefix, paID, scmDefaultIDSuffix);
}
