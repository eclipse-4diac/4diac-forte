/*******************************************************************************
 * Copyright (c) 2023 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "forte/eclipse4diac/utils/GetInstancePath_fbt.h"

using namespace forte::literals;

namespace forte::eclipse4diac::utils {
  namespace {
    const auto cDataInputNames = std::array{"Sep"_STRID};
    const auto cDataOutputNames = std::array{"Path"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_GetInstancePath, "eclipse4diac::utils::GetInstancePath"_STRID)

  FORTE_GetInstancePath::FORTE_GetInstancePath(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_Sep(0x2f_CHAR),
      conn_CNF(*this, 0),
      conn_Sep(nullptr),
      conn_Path(*this, 0, var_Path) {};

  void FORTE_GetInstancePath::setInitialValues() {
    var_Sep = 0x2f_CHAR;
    var_Path = ""_STRING;
  }

  void FORTE_GetInstancePath::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if (paEIID == scmEventREQID) {
      var_Path = CIEC_STRING(getFullQualifiedApplicationInstanceName(var_Sep.operator TForteChar()));
      sendOutputEvent(scmEventCNFID, paECET);
    }
  }

  void FORTE_GetInstancePath::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_Sep, conn_Sep);
        break;
      }
      default: break;
    }
  }

  void FORTE_GetInstancePath::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_Path, conn_Path);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_GetInstancePath::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_Sep;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_GetInstancePath::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_Path;
    }
    return nullptr;
  }

  CEventConnection *FORTE_GetInstancePath::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_GetInstancePath::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_Sep;
    }
    return nullptr;
  }

  CDataConnection *FORTE_GetInstancePath::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_Path;
    }
    return nullptr;
  }

} // namespace forte::eclipse4diac::utils
