/*******************************************************************************
 * Copyright (c) 2006, 2023 ACIN, Profactor GmbH, fortiss GmbH
 *                          Johannes Kepler University
 *                          Martin Erich Jobst
 *
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
 *    Martin Jobst - add generic readInputData and writeOutputData
 *******************************************************************************/
#include <fortenew.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "commfb.h"
#include "comlayer.h"
#include "comlayersmanager.h"
#include "criticalregion.h"
#include "string_utils.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(ID);
USE_STRING_ID(IND);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(REQ);
USE_STRING_ID(RSP);
USE_STRING_ID(STATUS);
USE_STRING_ID(STRING);
USE_STRING_ID(WSTRING);

using namespace forte::com_infra;

const CStringDictionary::TStringId CCommFB::scmRequesterEventInputNameIds[2] = {STRID(INIT), STRID(REQ)};
const CStringDictionary::TStringId CCommFB::scmRequesterEventOutputNameIds[2] = {STRID(INITO), STRID(CNF)};

const CStringDictionary::TStringId CCommFB::scmResponderEventInputNameIds[2] = {STRID(INIT), STRID(RSP)};
const CStringDictionary::TStringId CCommFB::scmResponderEventOutputNameIds[2] = {STRID(INITO), STRID(IND)};

const CStringDictionary::TStringId CCommFB::scmEventInputTypeIds[2] = {STRID(EInit), STRID(Event)};
const CStringDictionary::TStringId CCommFB::scmEventOutputTypeIds[2] = {STRID(Event), STRID(Event)};

CCommFB::CCommFB(const CStringDictionary::TStringId paInstanceNameId,
                 forte::core::CFBContainer &paContainer,
                 forte::com_infra::EComServiceType paCommServiceType) :
    CBaseCommFB(paInstanceNameId, paContainer, paCommServiceType) {
}

CCommFB::~CCommFB() = default;

EMGMResponse CCommFB::changeExecutionState(EMGMCommandType paCommand) {
  EMGMResponse retVal = CEventSourceFB::changeExecutionState(paCommand);
  if ((EMGMResponse::Ready == retVal) && (EMGMCommandType::Kill == paCommand)) {
    // when we are killed we'll close the connection so that it can safely be opened again after an reset
    closeConnection();
  }
  return retVal;
}

void CCommFB::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  EComResponse resp = e_Nothing;

  switch (paEIID) {
    case scmEventINITID:
      if (true == QI()) {
        resp = openConnection();
      } else {
        closeConnection();
        resp = e_InitTerminated;
      }
      break;
    case scmSendNotificationEventID: resp = sendData(); break;
    case cgExternalEventID: resp = receiveData(); break;
    default: break;
  }

  if (resp & +e_Terminated) {
    if (mCommServiceType == e_Server &&
        scmEventINITID != paEIID) { // if e_Terminated happened in INIT event, server shouldn't be silent
      // servers will not send information on client termination and should silently start to listen again
      resp = e_Nothing;
    } else {
      // subscribers and clients will close the connection and inform the user
      closeConnection();
    }
  }

  if (e_Nothing != resp) {
    STATUS() = CIEC_WSTRING(scmResponseTexts[resp & 0xF]);
    QO() = CIEC_BOOL(!(resp & scg_unComNegative));

    if (scg_unINIT & resp) {
      sendOutputEvent(scmEventINITOID, paECET);
    } else {
      sendOutputEvent(scmReceiveNotificationEventID, paECET);
    }
  }
}

void CCommFB::readInputData(TEventID paEI) {
  switch (paEI) {
    case scmEventINITID: {
      readData(0, *mDIs[0], mDIConns[0]);
      readData(1, *mDIs[1], mDIConns[1]);
      break;
    }
    case scmSendNotificationEventID: {
      for (TPortId i = 0; i < getFBInterfaceSpec().mNumDIs; ++i) {
        readData(i, *mDIs[i], mDIConns[i]);
      }
      break;
    }
    default: break;
  }
}

void CCommFB::writeOutputData(TEventID paEO) {
  size_t numDIs = getFBInterfaceSpec().mNumDIs;
  switch (paEO) {
    case scmEventINITOID: {
      writeData(numDIs + 0, *mDOs[0], mDOConns[0]);
      writeData(numDIs + 1, *mDOs[1], mDOConns[1]);
      break;
    }
    case scmReceiveNotificationEventID: {
      CCriticalRegion lock(getFBLock());
      for (TPortId i = 0; i < getFBInterfaceSpec().mNumDOs; ++i) {
        writeData(numDIs + i, *mDOs[i], mDOConns[i]);
      }
      break;
    }
    default: break;
  }
}

EComResponse CCommFB::sendData() {
  EComResponse resp = e_Nothing;
  if (true == QI()) {
    if (mCommServiceType != e_Subscriber) {
      if (nullptr != mTopOfComStack) {
        resp = mTopOfComStack->sendData(static_cast<void *>(getSDs()),
                                        static_cast<unsigned int>(getFBInterfaceSpec().mNumDIs - 2));
        if ((resp == e_ProcessDataOk) && (mCommServiceType != e_Publisher)) {
          // client and server will not directly send a cnf/ind event
          resp = e_Nothing;
        }
      } else {
        resp = e_ProcessDataNoSocket;
      }
    }
  } else {
    resp = e_ProcessDataInhibited; // we are not allowed to send data
  }
  return resp;
}

bool CCommFB::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
  TIdentifier tempstring;
  const char *sParamA = nullptr;
  const char *sParamB = nullptr;

  paInterfaceSpec.mNumEIs = 2;
  paInterfaceSpec.mNumEOs = 2;

  memcpy(tempstring, paConfigString,
         (strlen(paConfigString) > cgIdentifierLength) ? cgIdentifierLength
                                                       : strlen(paConfigString) + 1); // plus 1 for the null character
  tempstring[cgIdentifierLength] = '\0';

  size_t inlength = strlen(tempstring);

  size_t i;
  for (i = 0; i < inlength - 1; i++) { // search first underscore
    if (tempstring[i] == '_') {
      sParamA = sParamB = &(tempstring[i + 1]);
      break;
    }
  }
  if (nullptr != sParamA) { // search for 2nd underscore
    for (i = i + 1; i < inlength - 1; i++) {
      if (tempstring[i] == '_') {
        tempstring[i] = '\0';
        sParamB = &(tempstring[i + 1]);
        break;
      }
    }
  }
  if (nullptr == sParamB) { // no underscore found
    return false;
  }

  configureDIs(sParamA, paInterfaceSpec);
  configureDOs(sParamB, paInterfaceSpec);

  if (e_Requester == (+e_Requester & mCommServiceType)) {
    paInterfaceSpec.mEINames = scmRequesterEventInputNameIds;
    paInterfaceSpec.mEONames = scmRequesterEventOutputNameIds;
  } else {
    if (e_Responder == (+e_Responder & mCommServiceType)) {
      paInterfaceSpec.mEINames = scmResponderEventInputNameIds;
      paInterfaceSpec.mEONames = scmResponderEventOutputNameIds;
    }
  }
  paInterfaceSpec.mEITypeNames = scmEventInputTypeIds;
  paInterfaceSpec.mEOTypeNames = scmEventOutputTypeIds;

  return true;
}

void CCommFB::configureDIs(const char *paDIConfigString, SFBInterfaceSpec &paInterfaceSpec) {
  paInterfaceSpec.mNumDIs = 2;

  if (e_DataInputs == (+e_DataInputs & mCommServiceType)) {
    // TODO: Check range of sParamA
    paInterfaceSpec.mNumDIs =
        paInterfaceSpec.mNumDIs + static_cast<TPortId>(forte::core::util::strtol(paDIConfigString, nullptr, 10));
    mDiDataTypeNames = std::make_unique<CStringDictionary::TStringId[]>(paInterfaceSpec.mNumDIs);
    mDiNames = std::make_unique<CStringDictionary::TStringId[]>(paInterfaceSpec.mNumDIs);

    generateGenericDataPointArrays("SD_", &(mDiDataTypeNames[2]), &(mDiNames[2]), paInterfaceSpec.mNumDIs - 2);
  } else {
    mDiDataTypeNames = std::make_unique<CStringDictionary::TStringId[]>(paInterfaceSpec.mNumDIs);
    mDiNames = std::make_unique<CStringDictionary::TStringId[]>(paInterfaceSpec.mNumDIs);
  }
  paInterfaceSpec.mDIDataTypeNames = mDiDataTypeNames.get();
  paInterfaceSpec.mDINames = mDiNames.get();

  mDiDataTypeNames[0] = STRID(BOOL);
  mDiNames[0] = STRID(QI);
#ifdef FORTE_USE_WSTRING_DATATYPE
  mDiDataTypeNames[1] = STRID(WSTRING);
#else // FORTE_USE_WSTRING_DATATYPE
  mDiDataTypeNames[1] = STRID(STRING);
#endif // FORTE_USE_WSTRING_DATATYPE
  mDiNames[1] = STRID(ID);
}

void CCommFB::configureDOs(const char *paDOConfigString, SFBInterfaceSpec &paInterfaceSpec) {
  paInterfaceSpec.mNumDOs = 2;

  if (+e_DataOutputs == (+e_DataOutputs & mCommServiceType)) {
    // TODO: Check range of sParamA
    paInterfaceSpec.mNumDOs =
        paInterfaceSpec.mNumDOs + static_cast<TPortId>(forte::core::util::strtol(paDOConfigString, nullptr, 10));
    mDoDataTypeNames = std::make_unique<CStringDictionary::TStringId[]>(paInterfaceSpec.mNumDOs);
    mDoNames = std::make_unique<CStringDictionary::TStringId[]>(paInterfaceSpec.mNumDOs);

    generateGenericDataPointArrays("RD_", &(mDoDataTypeNames[2]), &(mDoNames[2]), paInterfaceSpec.mNumDOs - 2);
  } else {
    mDoDataTypeNames = std::make_unique<CStringDictionary::TStringId[]>(paInterfaceSpec.mNumDOs);
    mDoNames = std::make_unique<CStringDictionary::TStringId[]>(paInterfaceSpec.mNumDOs);
  }

  paInterfaceSpec.mDONames = mDoNames.get();
  paInterfaceSpec.mDODataTypeNames = mDoDataTypeNames.get();

  mDoDataTypeNames[0] = STRID(BOOL);
  mDoNames[0] = STRID(QO);
#ifdef FORTE_USE_WSTRING_DATATYPE
  mDoDataTypeNames[1] = STRID(WSTRING);
#else
  mDoDataTypeNames[1] = STRID(STRING);
#endif
  mDoNames[1] = STRID(STATUS);
}

EComResponse CCommFB::receiveData() {
  EComResponse eResp;
  EComResponse eRetVal = e_Nothing;

  const unsigned int comInterruptQueueCountCopy = mComInterruptQueueCount;
  for (size_t i = 0; i < comInterruptQueueCountCopy; ++i) {
    if (mInterruptQueue[i] == nullptr) {
      DEVLOG_ERROR("Attempt to process nullptr in CommFB::receiveData");
      eResp = e_Nothing;
    } else {
      eResp = mInterruptQueue[i]->processInterrupt();
    }
    if (eResp > eRetVal) {
      eRetVal = eResp;
    }
  }
  mComInterruptQueueCount -= comInterruptQueueCountCopy;
  for (unsigned int i = 0; i < mComInterruptQueueCount; ++i) {
    mInterruptQueue[i] = mInterruptQueue[i + comInterruptQueueCountCopy];
  }

  return eRetVal;
}

char *CCommFB::getDefaultIDString(const char *paID) {
  return buildIDString("fbdk[].ip[", paID, "]");
}
