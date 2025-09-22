/*******************************************************************************
 * Copyright (c) 2011, 2025 AIT, ACIN, Profactor GmbH, Martin Erich Jobst,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Alois Zoitl, Matthias Plasch
 *                - initial API and implementation and/or initial documentation
 *   Martin Jobst - refactor for ANY variant
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/

#include "forte/eclipse4diac/utils/OUT_ANY_CONSOLE_fbt.h"
#include "forte/datatypes/forte_bool.h"

using namespace forte::literals;

namespace forte::eclipse4diac::utils {
  namespace {
    const auto cDataInputNames = std::array{"QI"_STRID, "LABEL"_STRID, "IN"_STRID};
    const auto cDataOutputNames = std::array{"QO"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_OUT_ANY_CONSOLE, "eclipse4diac::utils::OUT_ANY_CONSOLE"_STRID)

  FORTE_OUT_ANY_CONSOLE::FORTE_OUT_ANY_CONSOLE(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_QI(false_BOOL),
      var_LABEL(""_STRING),
      var_IN(CIEC_ANY_VARIANT()),
      var_QO(false_BOOL),
      conn_CNF(*this, 0),
      conn_QI(nullptr),
      conn_LABEL(nullptr),
      conn_IN(nullptr),
      conn_QO(*this, 0, var_QO) {};

  void FORTE_OUT_ANY_CONSOLE::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID:
        var_QO = var_QI;
        if (var_QI) {
          DEVLOG_INFO(" %s = ", var_LABEL.getStorage().c_str());
          var_IN.toString(mDataOutPutBuffer);
          DEVLOG_INFO("%s\n", mDataOutPutBuffer.c_str());
          mDataOutPutBuffer.clear();
        }
        sendOutputEvent(scmEventCNFID, paECET);
        break;
    }
  }

  void FORTE_OUT_ANY_CONSOLE::readInputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_QI, conn_QI);
        readData(2, var_IN, conn_IN);
        readData(1, var_LABEL, conn_LABEL);
        break;
      }
      default: break;
    }
  }

  void FORTE_OUT_ANY_CONSOLE::writeOutputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_OUT_ANY_CONSOLE::getDI(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_LABEL;
      case 2: return &var_IN;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_OUT_ANY_CONSOLE::getDO(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
    }
    return nullptr;
  }

  CEventConnection *FORTE_OUT_ANY_CONSOLE::getEOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_OUT_ANY_CONSOLE::getDIConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_LABEL;
      case 2: return &conn_IN;
    }
    return nullptr;
  }

  CDataConnection *FORTE_OUT_ANY_CONSOLE::getDOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
    }
    return nullptr;
  }

  void FORTE_OUT_ANY_CONSOLE::setInitialValues() {
    var_QI = false_BOOL;
    var_LABEL = ""_STRING;
    var_IN.reset();
    var_QO = false_BOOL;
  }

} // namespace forte::eclipse4diac::utils
