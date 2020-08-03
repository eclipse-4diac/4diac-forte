/*******************************************************************************
 * Copyright (c) 2006-2014 ACIN, Profactor GmbH, fortiss GmbH
 *                      2018 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Rene Smodic, Alois Zoitl, Michael Hofmann, Martin Melik Merkumians,
 *    Patrick Smejkal
 *      - initial implementation and rework communication infrastructure
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
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

CCommFB::CCommFB(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes, forte::com_infra::EComServiceType pa_eCommServiceType) :
  CBaseCommFB(pa_nInstanceNameId, pa_poSrcRes, pa_eCommServiceType) {
}

CCommFB::~CCommFB() {

}

EMGMResponse CCommFB::changeFBExecutionState(EMGMCommandType pa_unCommand) {
  EMGMResponse retVal = CEventSourceFB::changeFBExecutionState(pa_unCommand);
  if ((e_RDY == retVal) && (cg_nMGM_CMD_Kill == pa_unCommand)) {
    //when we are killed we'll close the connection so that it can safely be opened again after an reset
    closeConnection();
  }
  return retVal;
}

void CCommFB::executeEvent(int paEIID) {
  EComResponse resp = e_Nothing;

  switch (paEIID) {
  case scm_nEventINITID:
    if (true == QI()) {
      resp = openConnection();
    }
    else {
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

  if(resp & e_Terminated) {
    if(m_eCommServiceType == e_Server && scm_nEventINITID != paEIID) { //if e_Terminated happened in INIT event, server shouldn't be silent
      //servers will not send information on client termination and should silently start to listen again
      resp = e_Nothing;
    } else {
      //subscribers and clients will close the connection and inform the user
      closeConnection();
    }
  }

  if (e_Nothing != resp) {
    STATUS() = scm_sResponseTexts[resp & 0xF];
    QO() = !(resp & scg_unComNegative);

    if (scg_unINIT & resp) {
      sendOutputEvent(scm_nEventINITOID);
    }
    else {
      sendOutputEvent(scm_nReceiveNotificationEventID);
    }
  }
}

EComResponse CCommFB::sendData() {
  EComResponse resp = e_Nothing;
  if (true == QI()) {
    if (m_eCommServiceType != e_Subscriber) {
      if (0 != m_poTopOfComStack) {
        resp = m_poTopOfComStack->sendData(static_cast<void*>(getSDs()), m_pstInterfaceSpec->m_nNumDIs - 2);
        if ((resp == e_ProcessDataOk) && (m_eCommServiceType != e_Publisher)) {
          // client and server will not directly send a cnf/ind event
          resp = e_Nothing;
        }
      }
      else {
        resp = e_ProcessDataNoSocket;
      }
    }
  }
  else {
    resp = e_ProcessDataInhibited; // we are not allowed to send data
  }
  return resp;
}

bool CCommFB::createInterfaceSpec(const char* paConfigString, SFBInterfaceSpec& paInterfaceSpec) {
  TIdentifier tempstring;
  const char *sParamA = 0;
  const char *sParamB = 0;

  paInterfaceSpec.m_nNumEIs = 2;
  paInterfaceSpec.m_nNumEOs = 2;

  memcpy(tempstring, paConfigString, (strlen(paConfigString) > cg_nIdentifierLength) ? cg_nIdentifierLength : strlen(paConfigString) + 1); //plus 1 for the null character
  tempstring[cg_nIdentifierLength] = '\0';

  size_t inlength = strlen(tempstring);

  size_t i;
  for (i = 0; i < inlength - 1; i++) { // search first underscore
    if (tempstring[i] == '_') {
      sParamA = sParamB = &(tempstring[i + 1]);
      break;
    }
  }
  if(0 != sParamA) { // search for 2nd underscore
    for (i = i + 1; i < inlength - 1; i++) {
      if (tempstring[i] == '_') {
        tempstring[i] = '\0';
        sParamB = &(tempstring[i + 1]);
        break;
      }
    }
  }
  if (0 == sParamB){ // no underscore found
    return false;
  }

  configureDIs(sParamA, paInterfaceSpec);
  configureDOs(sParamB, paInterfaceSpec);

  if (forte::com_infra::e_Requester == (forte::com_infra::e_Requester & m_eCommServiceType)) {
    paInterfaceSpec.m_aunEINames = scm_aunRequesterEventInputNameIds;
    paInterfaceSpec.m_aunEONames = scm_aunRequesterEventOutputNameIds;
  }
  else {
    if (forte::com_infra::e_Responder == (forte::com_infra::e_Responder & m_eCommServiceType)) {
      paInterfaceSpec.m_aunEINames = scm_aunResponderEventInputNameIds;
      paInterfaceSpec.m_aunEONames = scm_aunResponderEventOutputNameIds;
    }
  }
  paInterfaceSpec.m_anEIWithIndexes = scm_anEIWithIndexes;
  paInterfaceSpec.m_anEOWithIndexes = scm_anEOWithIndexes;

  return true;
}

void CCommFB::configureDIs(const char* paDIConfigString, SFBInterfaceSpec& paInterfaceSpec) const {
  CStringDictionary::TStringId* diDataTypeNames;
  CStringDictionary::TStringId* diNames;
  TDataIOID* eiWith;

  paInterfaceSpec.m_nNumDIs = 2;

  if (forte::com_infra::e_DataInputs == (forte::com_infra::e_DataInputs & m_eCommServiceType)) {
      //TODO: Check range of sParamA
      paInterfaceSpec.m_nNumDIs = static_cast<TForteUInt8>(paInterfaceSpec.m_nNumDIs + forte::core::util::strtol(paDIConfigString, 0, 10));
      diDataTypeNames = new CStringDictionary::TStringId[paInterfaceSpec.m_nNumDIs];
      diNames = new CStringDictionary::TStringId[paInterfaceSpec.m_nNumDIs];
      eiWith = new TDataIOID[paInterfaceSpec.m_nNumDIs - 2 + scmMinWithLength];

      generateGenericDataPointArrays("SD_", &(diDataTypeNames[2]), &(diNames[2]), paInterfaceSpec.m_nNumDIs - 2);
    }
    else {
      diDataTypeNames = new CStringDictionary::TStringId[paInterfaceSpec.m_nNumDIs];
      diNames = new CStringDictionary::TStringId[paInterfaceSpec.m_nNumDIs];
      eiWith = new TDataIOID[scmMinWithLength];
    }
    paInterfaceSpec.m_aunDIDataTypeNames = diDataTypeNames;
    paInterfaceSpec.m_aunDINames = diNames;
    paInterfaceSpec.m_anEIWith = eiWith;

    eiWith[0] = 0;
    eiWith[1] = 1;
    eiWith[2] = scmWithListDelimiter;
    eiWith[3] = 0;
    eiWith[4] = 1;

    size_t i;
    for (i = 0; i < paInterfaceSpec.m_nNumDIs - 2U; i++) {
      eiWith[i + 5U] = static_cast<TForteUInt8>(i + 2U);
    }
    eiWith[i + 5U] = scmWithListDelimiter;

    diDataTypeNames[0] = g_nStringIdBOOL;
    diNames[0] = g_nStringIdQI;
#ifdef FORTE_USE_WSTRING_DATATYPE
    diDataTypeNames[1] = g_nStringIdWSTRING;
#else //FORTE_USE_WSTRING_DATATYPE
    diDataTypeNames[1] = g_nStringIdSTRING;
#endif //FORTE_USE_WSTRING_DATATYPE
    diNames[1] = g_nStringIdID;
}

void CCommFB::configureDOs(const char* paDOConfigString, SFBInterfaceSpec& paInterfaceSpec) const {
  CStringDictionary::TStringId* doDataTypeNames;
  CStringDictionary::TStringId* doNames;
  TDataIOID* eoWith;

  paInterfaceSpec.m_nNumDOs = 2;

  if(forte::com_infra::e_DataOutputs == (forte::com_infra::e_DataOutputs & m_eCommServiceType)){
    //TODO: Check range of sParamA
    paInterfaceSpec.m_nNumDOs = static_cast<TForteUInt8>(paInterfaceSpec.m_nNumDOs + forte::core::util::strtol(paDOConfigString, 0, 10));
    doDataTypeNames  = new CStringDictionary::TStringId[paInterfaceSpec.m_nNumDOs];
    doNames = new CStringDictionary::TStringId[paInterfaceSpec.m_nNumDOs];
    eoWith = new TDataIOID[paInterfaceSpec.m_nNumDOs - 2 + scmMinWithLength];

    generateGenericDataPointArrays("RD_", &(doDataTypeNames[2]), &(doNames[2]), paInterfaceSpec.m_nNumDOs - 2);
  }
  else{
    doDataTypeNames = new CStringDictionary::TStringId[paInterfaceSpec.m_nNumDOs];
    doNames = new CStringDictionary::TStringId[paInterfaceSpec.m_nNumDOs];
    eoWith = new TDataIOID[scmMinWithLength];
  }

  paInterfaceSpec.m_aunDONames = doNames;
  paInterfaceSpec.m_aunDODataTypeNames = doDataTypeNames;
  paInterfaceSpec.m_anEOWith = eoWith;

  eoWith[0] = 0;
  eoWith[1] = 1;
  eoWith[2] = scmWithListDelimiter;
  eoWith[3] = 0;
  eoWith[4] = 1;

  size_t i;
  for(i = 0; i < paInterfaceSpec.m_nNumDOs - 2U; i++){
    eoWith[i + 5U] = static_cast<TForteUInt8>(i + 2U);
  }
  eoWith[i + 5U] = scmWithListDelimiter;

  doDataTypeNames[0] = g_nStringIdBOOL;
  doNames[0] = g_nStringIdQO;
#ifdef FORTE_USE_WSTRING_DATATYPE
  doDataTypeNames[1] = g_nStringIdWSTRING;
#else
  doDataTypeNames[1] = g_nStringIdSTRING;
#endif
  doNames[1] = g_nStringIdSTATUS;

}

EComResponse CCommFB::receiveData() {
  EComResponse eResp;
  EComResponse eRetVal = e_Nothing;

  const unsigned int comInterruptQueueCountCopy = m_unComInterruptQueueCount;
  for (size_t i = 0; i < comInterruptQueueCountCopy; ++i) {
    eResp = m_apoInterruptQueue[i]->processInterrupt();
    if (eResp > eRetVal) {
      eRetVal = eResp;
    }
  }
  m_unComInterruptQueueCount -= comInterruptQueueCountCopy;
  for (unsigned int i = 0; i < m_unComInterruptQueueCount; ++i) {
    m_apoInterruptQueue[i] = m_apoInterruptQueue[i + comInterruptQueueCountCopy];
  }

  return eRetVal;
}

char *CCommFB::getDefaultIDString(const char *paID) {
  return buildIDString("fbdk[].ip[", paID, "]");
}
