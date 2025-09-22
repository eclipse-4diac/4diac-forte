/*******************************************************************************
 * Copyright (c) 2011 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl, Gerhard Ebenhofer
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#include "forte/iec61131/selection/F_LIMIT_fbt.h"

using namespace forte::literals;

namespace forte::iec61131::selection {
  namespace {
    const auto cDataInputNames = std::array{"MN"_STRID, "IN"_STRID, "MX"_STRID};
    const auto cDataOutputNames = std::array{"OUT"_STRID};
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};

    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = {},
        .mEONames = cEventOutputNames,
        .mEOTypeNames = {},
        .mDINames = cDataInputNames,
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_F_LIMIT, "iec61131::selection::F_LIMIT"_STRID)

  FORTE_F_LIMIT::FORTE_F_LIMIT(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_MN(CIEC_ANY_ELEMENTARY_VARIANT()),
      var_IN(CIEC_ANY_ELEMENTARY_VARIANT()),
      var_MX(CIEC_ANY_ELEMENTARY_VARIANT()),
      var_OUT(CIEC_ANY_ELEMENTARY_VARIANT()),
      conn_CNF(*this, 0),
      conn_MN(nullptr),
      conn_IN(nullptr),
      conn_MX(nullptr),
      conn_OUT(*this, 0, var_OUT) {};

  void FORTE_F_LIMIT::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID:
        var_OUT = var_IN <= var_MX ? (var_IN >= var_MN ? var_IN : var_MN) : var_MX;
        sendOutputEvent(scmEventCNFID, paECET);
        break;
    }
  }

  void FORTE_F_LIMIT::readInputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_MN, conn_MN);
        readData(2, var_MX, conn_MX);
        readData(1, var_IN, conn_IN);
        break;
      }
      default: break;
    }
  }

  void FORTE_F_LIMIT::writeOutputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_OUT, conn_OUT);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_F_LIMIT::getDI(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_MN;
      case 1: return &var_IN;
      case 2: return &var_MX;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_F_LIMIT::getDO(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_OUT;
    }
    return nullptr;
  }

  CEventConnection *FORTE_F_LIMIT::getEOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_F_LIMIT::getDIConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_MN;
      case 1: return &conn_IN;
      case 2: return &conn_MX;
    }
    return nullptr;
  }

  CDataConnection *FORTE_F_LIMIT::getDOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_OUT;
    }
    return nullptr;
  }

  void FORTE_F_LIMIT::setInitialValues() {
    var_MN.reset();
    var_IN.reset();
    var_MX.reset();
    var_OUT.reset();
  }

} // namespace forte::iec61131::selection
