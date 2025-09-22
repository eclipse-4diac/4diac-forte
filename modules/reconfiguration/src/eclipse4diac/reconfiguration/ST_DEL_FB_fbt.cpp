/*******************************************************************************
 * Copyright (c) 2014, 2015 Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch, Gerhard Ebenhofer, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "forte/eclipse4diac/reconfiguration/ST_DEL_FB_fbt.h"

#include "forte/device.h"
#include "forte/mgmcmdstruct.h"

using namespace forte::literals;

namespace forte::eclipse4diac::reconfiguration {
  namespace {
    const auto cDataInputNames = std::array{"QI"_STRID, "FB_NAME"_STRID, "DST"_STRID};
    const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID};
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventInputTypeIds = std::array{"Event"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_ST_DEL_FB, "eclipse4diac::reconfiguration::ST_DEL_FB"_STRID)

  FORTE_ST_DEL_FB::FORTE_ST_DEL_FB(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      conn_CNF(*this, 0),
      conn_QI(nullptr),
      conn_FB_NAME(nullptr),
      conn_DST(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS) {};

  void FORTE_ST_DEL_FB::setInitialValues() {
    var_QI = 0_BOOL;
    var_FB_NAME = u""_WSTRING;
    var_DST = u""_WSTRING;
    var_QO = 0_BOOL;
    var_STATUS = u""_WSTRING;
  }

  void FORTE_ST_DEL_FB::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID:
        var_QO = var_QI;
        if (var_QI) {
          executeRQST();
        } else {
          var_STATUS = u"Not Ready"_WSTRING;
        }
        sendOutputEvent(scmEventCNFID, paECET);
        break;
    }
  }

  void FORTE_ST_DEL_FB::executeRQST() {
    SManagementCMD theCommand;

    theCommand.mDestination = StringId::lookup(var_DST.getValue());
    theCommand.mFirstParam.push_back(StringId::lookup(var_FB_NAME.getValue()));
    theCommand.mCMD = EMGMCommandType::DeleteFBInstance;

    EMGMResponse resp = getDevice()->executeMGMCommand(theCommand);

    // calculate return value
    var_QO = CIEC_BOOL(resp == EMGMResponse::Ready);
    const std::string retVal(mgm_cmd::getResponseText(resp));
    DEVLOG_DEBUG("%s\n", retVal.c_str());
    var_STATUS = CIEC_WSTRING(retVal.c_str());
  }

  void FORTE_ST_DEL_FB::readInputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(1, var_FB_NAME, conn_FB_NAME);
        readData(2, var_DST, conn_DST);
        readData(0, var_QI, conn_QI);
        break;
      }
      default: break;
    }
  }

  void FORTE_ST_DEL_FB::writeOutputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_ST_DEL_FB::getDI(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_FB_NAME;
      case 2: return &var_DST;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_ST_DEL_FB::getDO(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
    }
    return nullptr;
  }

  CEventConnection *FORTE_ST_DEL_FB::getEOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_ST_DEL_FB::getDIConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_FB_NAME;
      case 2: return &conn_DST;
    }
    return nullptr;
  }

  CDataConnection *FORTE_ST_DEL_FB::getDOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
    }
    return nullptr;
  }

} // namespace forte::eclipse4diac::reconfiguration
