/*******************************************************************************
 * Copyright (c) 2011 - 2013 ACIN, fortiss GmbH
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#include "forte/iec61131/selection/F_SEL_fbt.h"

using namespace forte::literals;

namespace forte::iec61131::selection {
  namespace {
    const auto cDataInputNames = std::array{"G"_STRID, "IN0"_STRID, "IN1"_STRID};

    const auto cDataOutputNames = std::array{"OUT"_STRID};

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventInputTypeIds = std::array{"Event"_STRID};

    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID};

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

  DEFINE_FIRMWARE_FB(FORTE_F_SEL, "iec61131::selection::F_SEL"_STRID)

  FORTE_F_SEL::FORTE_F_SEL(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_G(false_BOOL),
      var_IN0(CIEC_ANY_VARIANT()),
      var_IN1(CIEC_ANY_VARIANT()),
      var_OUT(CIEC_ANY_VARIANT()),
      conn_CNF(*this, 0),
      conn_G(nullptr),
      conn_IN0(nullptr),
      conn_IN1(nullptr),
      conn_OUT(*this, 0, var_OUT) {};

  void FORTE_F_SEL::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID:
        var_OUT = var_G ? var_IN1 : var_IN0;
        sendOutputEvent(scmEventCNFID, paECET);
        break;
    }
  }

  void FORTE_F_SEL::readInputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_G, conn_G);
        readData(2, var_IN1, conn_IN1);
        readData(1, var_IN0, conn_IN0);
        break;
      }
      default: break;
    }
  }

  void FORTE_F_SEL::writeOutputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_OUT, conn_OUT);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_F_SEL::getDI(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_G;
      case 1: return &var_IN0;
      case 2: return &var_IN1;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_F_SEL::getDO(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_OUT;
    }
    return nullptr;
  }

  CEventConnection *FORTE_F_SEL::getEOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_F_SEL::getDIConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_G;
      case 1: return &conn_IN0;
      case 2: return &conn_IN1;
    }
    return nullptr;
  }

  CDataConnection *FORTE_F_SEL::getDOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_OUT;
    }
    return nullptr;
  }

  void FORTE_F_SEL::setInitialValues() {
    var_G = CIEC_BOOL(0);
    var_IN0.reset();
    var_IN1.reset();
    var_OUT.reset();
  }

} // namespace forte::iec61131::selection
