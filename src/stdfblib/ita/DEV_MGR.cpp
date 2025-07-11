/*******************************************************************************
 * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 *                      2018 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Rene Smodic, Thomas Strasser, Gerhard Ebenhofer,
 *   Oliver Hummer, Ingo Hegny, Michael Hofmann, Martin Melik Merkumians
 *    - initial API and implementation and/or initial documentation
 *   Monika Wenger
 *    - fix: apostrophes are deleted in parseWriteConnectionData
 *   Jens Reimann
 *    - Enhance bootfile loading behavior
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *******************************************************************************/
#include <string.h>
#include "stdfblib/ita/DEV_MGR.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(DEV_MGR);
USE_STRING_ID(DST);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(ID);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(REQ);
USE_STRING_ID(RESP);
USE_STRING_ID(RQST);
USE_STRING_ID(STATUS);
USE_STRING_ID(STRING);
USE_STRING_ID(WSTRING);

#include "core/device.h"
#include <stdio.h>
#include <stdlib.h>
#include "stdfblib/ita/ForteBootFileLoader.h"
#include "core/util/string_utils.h"

DEFINE_FIRMWARE_FB(DEV_MGR, STRID(DEV_MGR))

namespace {
  const auto cDataInputNames = std::array{STRID(QI), STRID(ID), STRID(RESP)};
  const auto cDataOutputNames = std::array{STRID(QO), STRID(STATUS), STRID(DST), STRID(RQST)};
  const auto cEventInputNames = std::array{STRID(INIT), STRID(REQ)};
  const auto cEventInputTypeIds = std::array{STRID(EInit), STRID(Event)};
  const auto cEventOutputNames = std::array{STRID(INITO), STRID(CNF)};
  const auto cEventOutputTypeIds = std::array{STRID(Event), STRID(Event)};

  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = cEventInputTypeIds,
      .mEONames = cEventOutputNames,
      .mEOTypeNames = cEventOutputTypeIds,
      .mDINames = cDataInputNames,
      .mDONames = cDataOutputNames,
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = {},
  };
} // namespace

void DEV_MGR::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  if (scmEventINITID == paEIID) {
#ifdef FORTE_SUPPORT_BOOT_FILE
    if ((true == QI()) && (false == QO())) {
      // this is the first time init is called try to load a boot file
      ForteBootFileLoader loader([this](const char *const paDest, char *paCommand) -> bool {
        return this->executeCommand(paDest, paCommand);
      });
      if (loader.needsExit()) {
        getDevice()->changeExecutionState(EMGMCommandType::Kill);
        return;
      }
      if (loader.isOpen() && LOAD_RESULT_OK == loader.loadBootFile()) {
        DEVLOG_INFO("Bootfile correctly loaded\n");
      }
    }
#endif
    CCommFB::executeEvent(paEIID, paECET); // initialize the underlying server FB
  } else {
    if (cgExternalEventID == paEIID && // we received a message on the network let the server correctly handle it
        forte::com_infra::e_ProcessDataOk == CCommFB::receiveData()) { // the message was correctly received
      executeRQST();
      // send response
      CCommFB::sendData();
    }
  }
}

void DEV_MGR::executeRQST() {
  char *request = new char[RQST().length() + 1];
  strcpy(request, RQST().getStorage().c_str());

  mCommandParser.parseAndExecuteMGMCommand(DST().getStorage().c_str(), request);
  mCommandParser.generateResponse(RESP());

  delete[] (request);
}

DEV_MGR::DEV_MGR(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CCommFB(paInstanceNameId, paContainer, forte::com_infra::e_Server),
    mDevice(*paContainer.getDevice()),
    mCommandParser(mDevice) {
  getGenInterfaceSpec() = cFBInterfaceSpec;
}

bool DEV_MGR::initialize() {
  setupFBInterface();
  if (!CCommFB::initialize()) {
    return false;
  }
  return true;
}

DEV_MGR::~DEV_MGR() = default;

bool DEV_MGR::executeCommand(const char *const paDest, char *paCommand) {
  EMGMResponse eResp = mCommandParser.parseAndExecuteMGMCommand(paDest, paCommand);
  if (eResp != EMGMResponse::Ready) {
    DEVLOG_ERROR("Boot file error. DEV_MGR says error is %s\n", forte::mgm_cmd::getResponseText(eResp).c_str());
  }
  return (eResp == EMGMResponse::Ready);
}

void DEV_MGR::createGenInputData() {
  size_t numGenDIs = getFBInterfaceSpec().getNumDIs() - 2;
  mGenDIs = std::unique_ptr<CIEC_ANY *[]>(new CIEC_ANY *[numGenDIs]);
  for (size_t i = 0; i < numGenDIs; ++i) {
    mGenDIs[i] = new CIEC_STRING();
  }
}

void DEV_MGR::createGenOutputData() {
  size_t numGenDOs = getFBInterfaceSpec().getNumDOs() - 2;
  mGenDOs = std::unique_ptr<CIEC_ANY *[]>(new CIEC_ANY *[numGenDOs]);
  for (size_t i = 0; i < numGenDOs; ++i) {
    mGenDOs[i] = new CIEC_STRING();
  }
}
