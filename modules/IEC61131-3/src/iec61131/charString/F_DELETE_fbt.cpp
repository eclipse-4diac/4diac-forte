/*******************************************************************************
 * Copyright (c) 2013 ACIN
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

#include "forte/iec61131/charString/F_DELETE_fbt.h"

using namespace forte::literals;

namespace forte::iec61131::charString {
  namespace {
    const auto cDataInputNames = std::array{"IN"_STRID, "L"_STRID, "P"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_F_DELETE, "iec61131::charString::F_DELETE"_STRID)

  FORTE_F_DELETE::FORTE_F_DELETE(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_IN(CIEC_ANY_STRING_VARIANT()),
      var_L(CIEC_ANY_INT_VARIANT()),
      var_P(CIEC_ANY_INT_VARIANT()),
      var_OUT(CIEC_ANY_STRING_VARIANT()),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_L(nullptr),
      conn_P(nullptr),
      conn_OUT(*this, 0, var_OUT) {};

  void FORTE_F_DELETE::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID:
        var_OUT = std::visit(
            [](auto &&paIN, auto &&paL, auto &&paP) -> CIEC_ANY_STRING_VARIANT { return func_DELETE(paIN, paL, paP); },
            static_cast<CIEC_ANY_STRING_VARIANT::variant &>(var_IN),
            static_cast<CIEC_ANY_INT_VARIANT::variant &>(var_L), static_cast<CIEC_ANY_INT_VARIANT::variant &>(var_P));
        sendOutputEvent(scmEventCNFID, paECET);
        break;
    }
  }

  void FORTE_F_DELETE::readInputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_IN, conn_IN);
        readData(1, var_L, conn_L);
        readData(2, var_P, conn_P);
        break;
      }
      default: break;
    }
  }

  void FORTE_F_DELETE::writeOutputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_OUT, conn_OUT);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_F_DELETE::getDI(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_IN;
      case 1: return &var_L;
      case 2: return &var_P;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_F_DELETE::getDO(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_OUT;
    }
    return nullptr;
  }

  CEventConnection *FORTE_F_DELETE::getEOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_F_DELETE::getDIConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_IN;
      case 1: return &conn_L;
      case 2: return &conn_P;
    }
    return nullptr;
  }

  CDataConnection *FORTE_F_DELETE::getDOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_OUT;
    }
    return nullptr;
  }

  void FORTE_F_DELETE::setInitialValues() {
    var_IN.reset();
    var_L.reset();
    var_P.reset();
    var_OUT.reset();
  }

} // namespace forte::iec61131::charString
