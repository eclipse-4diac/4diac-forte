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
#include <cstddef>
#include <memory>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "forte/datatypes/forte_any_variant.h"
#include "forte/cominfra/commfb.h"
#include "forte/cominfra/comlayer.h"
#include "forte/cominfra/comlayersmanager.h"
#include "forte/util/criticalregion.h"
#include "forte/util/string_utils.h"

using namespace forte::literals;

namespace forte::com_infra {
  namespace {
    const auto cRequesterEventInputNameIds = std::array{"INIT"_STRID, "REQ"_STRID};
    const auto cRequesterEventOutputNameIds = std::array{"INITO"_STRID, "CNF"_STRID};

    const auto cResponderEventInputNameIds = std::array{"INIT"_STRID, "RSP"_STRID};
    const auto cResponderEventOutputNameIds = std::array{"INITO"_STRID, "IND"_STRID};

    const auto cEventInputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID, "Event"_STRID};
  } // namespace

  CCommFB::CCommFB(const StringId paInstanceNameId, CFBContainer &paContainer, EComServiceType paCommServiceType) :
      CBaseCommFB(paInstanceNameId, paContainer, paCommServiceType),
      conn_INITO(*this, 0),
      conn_CNF_IND(*this, 1),
      conn_QI(nullptr),
      conn_ID(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS) {
  }

  CCommFB::~CCommFB() {
    for (size_t i = 0; i < getGenDINums(); ++i) {
      delete mGenDIs[i];
    }

    for (size_t i = 0; i < getGenDONums(); ++i) {
      delete mGenDOs[i];
    }
  }

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
        readData(0, var_QI, conn_QI);
        readData(1, var_ID, conn_ID);
        break;
      }
      case scmSendNotificationEventID: {
        readData(0, var_QI, conn_QI);
        readData(1, var_ID, conn_ID);
        for (size_t i = 0; i < getGenDINums(); ++i) {
          readData(i + 2, *mGenDIs[i], mGenDIConns[i]);
        }
        break;
      }
      default: break;
    }
  }

  void CCommFB::writeOutputData(TEventID paEO) {
    size_t numDIs = getFBInterfaceSpec().getNumDIs();
    switch (paEO) {
      case scmEventINITOID: {
        writeData(numDIs + 0, var_QO, conn_QO);
        writeData(numDIs + 1, var_STATUS, conn_STATUS);
        break;
      }
      case scmReceiveNotificationEventID: {
        writeData(numDIs + 0, var_QO, conn_QO);
        writeData(numDIs + 1, var_STATUS, conn_STATUS);
        util::CCriticalRegion lock(getFBLock());
        for (size_t i = 0; i < getGenDONums(); ++i) {
          writeData(numDIs + 2 + i, *mGenDOs[i], mGenDOConns[i]);
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
                                          static_cast<unsigned int>(getFBInterfaceSpec().getNumDIs() - 2));
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
      paInterfaceSpec.mEINames = cRequesterEventInputNameIds;
      paInterfaceSpec.mEONames = cRequesterEventOutputNameIds;
    } else {
      if (e_Responder == (+e_Responder & mCommServiceType)) {
        paInterfaceSpec.mEINames = cResponderEventInputNameIds;
        paInterfaceSpec.mEONames = cResponderEventOutputNameIds;
      }
    }
    paInterfaceSpec.mEITypeNames = cEventInputTypeIds;
    paInterfaceSpec.mEOTypeNames = cEventOutputTypeIds;

    return true;
  }

  void CCommFB::configureDIs(const char *paDIConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    mDiNames.emplace_back("QI"_STRID);
    mDiNames.emplace_back("ID"_STRID);
    if (e_DataInputs == (+e_DataInputs & mCommServiceType)) {
      // TODO: Check range of sParamA
      size_t numGenDIs = static_cast<TPortId>(util::strtol(paDIConfigString, nullptr, 10));
      generateGenericInterfacePointNameArray("SD_", mDiNames, numGenDIs);
    }
    paInterfaceSpec.mDINames = mDiNames;
  }

  void CCommFB::configureDOs(const char *paDOConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    mDoNames.emplace_back("QO"_STRID);
    mDoNames.emplace_back("STATUS"_STRID);

    if (+e_DataOutputs == (+e_DataOutputs & mCommServiceType)) {
      // TODO: Check range of sParamA
      size_t numGenDOs = static_cast<TPortId>(util::strtol(paDOConfigString, nullptr, 10));
      generateGenericInterfacePointNameArray("RD_", mDoNames, numGenDOs);
    }
    paInterfaceSpec.mDONames = mDoNames;
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

  void CCommFB::createGenInputData() {
    size_t numGenDIs = getGenDINums();
    mGenDIs = std::make_unique<CIEC_ANY *[]>(numGenDIs);
    for (size_t i = 0; i < numGenDIs; ++i) {
      mGenDIs[i] = new CIEC_ANY_VARIANT();
    }
  }

  void CCommFB::createGenOutputData() {
    size_t numGenDOs = getGenDONums();
    mGenDOs = std::make_unique<CIEC_ANY *[]>(numGenDOs);
    for (size_t i = 0; i < numGenDOs; ++i) {
      mGenDOs[i] = new CIEC_ANY_VARIANT();
    }
  }

  CIEC_ANY *CCommFB::getDI(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_ID;
      default: return mGenDIs[paIndex - getGenDIOffset()];
    }
    return nullptr;
  }

  CIEC_ANY *CCommFB::getDO(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
      default: return mGenDOs[paIndex - getGenDOOffset()];
    }
    return nullptr;
  }

  CEventConnection *CCommFB::getEOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_CNF_IND;
    }
    return nullptr;
  }

  CDataConnection **CCommFB::getDIConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_ID;
      default: return CBaseCommFB::getDIConUnchecked(paIndex);
    }
    return nullptr;
  }

  CDataConnection *CCommFB::getDOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
      default: return CBaseCommFB::getDOConUnchecked(paIndex);
    }
    return nullptr;
  }
} // namespace forte::com_infra
