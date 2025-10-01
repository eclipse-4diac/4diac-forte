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

#include "DEV_MGR.h"
#include "ForteBootFileLoader_config.h"
#include "forte/device.h"
#include "ForteBootFileLoader.h"

using namespace forte::literals;

namespace forte::iec61499::system {
  namespace {
    const auto cDataInputNames = std::array{"QI"_STRID, "ID"_STRID, "RESP"_STRID};
    const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID, "DST"_STRID, "RQST"_STRID};
    const auto cEventInputNames = std::array{"INIT"_STRID, "REQ"_STRID};
    const auto cEventInputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
    const auto cEventOutputNames = std::array{"INITO"_STRID, "CNF"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID, "Event"_STRID};

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

  DEFINE_FIRMWARE_FB(DEV_MGR, "iec61499::system::DEV_MGR"_STRID)

  void DEV_MGR::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if (scmEventINITID == paEIID) {
      if constexpr (cgSupportBootFile) {
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
      }
      CCommFB::executeEvent(paEIID, paECET); // initialize the underlying server FB
    } else {
      if (cgExternalEventID == paEIID && // we received a message on the network let the server correctly handle it
          com_infra::e_ProcessDataOk == CCommFB::receiveData()) { // the message was correctly received
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

  DEV_MGR::DEV_MGR(StringId paInstanceNameId, CFBContainer &paContainer) :
      CCommFB(paInstanceNameId, paContainer, com_infra::e_Server),
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
} // namespace forte::iec61499::system
