/*******************************************************************************
 * Copyright (c) 2024 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Ernst Blecha - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "forte/eclipse4diac/utils/GetInstancePathAndName_fbt.h"

using namespace forte::literals;

#include "forte/datatypes/forte_char.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::utils {
  namespace {
    const auto cDataInputNames = std::array{"Sep"_STRID};
    const auto cDataOutputNames = std::array{"Path"_STRID, "Name"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_GetInstancePathAndName, "eclipse4diac::utils::GetInstancePathAndName"_STRID)

  FORTE_GetInstancePathAndName::FORTE_GetInstancePathAndName(const StringId paInstanceNameId,
                                                             CFBContainer &paContainer) :
      CSimpleFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      var_Sep(0x2f_CHAR),
      conn_CNF(*this, 0),
      conn_Sep(nullptr),
      conn_Path(*this, 0, var_Path),
      conn_Name(*this, 1, var_Name) {
  }

  void FORTE_GetInstancePathAndName::setInitialValues() {
    var_Sep = 0x2f_CHAR;
    var_Path = ""_STRING;
    var_Name = ""_STRING;
  }

  void FORTE_GetInstancePathAndName::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID: alg_REQ(); break;
      default: break;
    }
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_GetInstancePathAndName::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_Sep, conn_Sep);
        break;
      }
      default: break;
    }
  }

  void FORTE_GetInstancePathAndName::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_Path, conn_Path);
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_Name, conn_Name);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_GetInstancePathAndName::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_Sep;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_GetInstancePathAndName::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_Path;
      case 1: return &var_Name;
    }
    return nullptr;
  }

  CEventConnection *FORTE_GetInstancePathAndName::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_GetInstancePathAndName::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_Sep;
    }
    return nullptr;
  }

  CDataConnection *FORTE_GetInstancePathAndName::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_Path;
      case 1: return &conn_Name;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_GetInstancePathAndName::getVarInternal(size_t) {
    return nullptr;
  }

  void FORTE_GetInstancePathAndName::alg_REQ(void) {
    var_Path = CIEC_STRING(getParent().getFullQualifiedApplicationInstanceName(var_Sep.operator TForteChar()));
    var_Name = CIEC_STRING(getInstanceName(), strlen(getInstanceName()));
  }

} // namespace forte::eclipse4diac::utils
