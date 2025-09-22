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

#include "forte/iec61131/charString/F_LEFT_fbt.h"

using namespace forte::literals;

namespace forte::iec61131::charString {
  namespace {
    const auto cDataInputNames = std::array{"IN"_STRID, "L"_STRID};

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

  DEFINE_FIRMWARE_FB(FORTE_F_LEFT, "iec61131::charString::F_LEFT"_STRID)

  FORTE_F_LEFT::FORTE_F_LEFT(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_IN(CIEC_ANY_STRING_VARIANT()),
      var_L(CIEC_ANY_INT_VARIANT()),
      var_OUT(CIEC_ANY_STRING_VARIANT()),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_L(nullptr),
      conn_OUT(*this, 0, var_OUT) {};

  void FORTE_F_LEFT::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID:
        var_OUT = std::visit([](auto &&paIN, auto &&paL) -> CIEC_ANY_STRING_VARIANT { return func_LEFT(paIN, paL); },
                             static_cast<CIEC_ANY_STRING_VARIANT::variant &>(var_IN),
                             static_cast<CIEC_ANY_INT_VARIANT::variant &>(var_L));
        sendOutputEvent(scmEventCNFID, paECET);
        break;
    }
  }

  void FORTE_F_LEFT::readInputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_IN, conn_IN);
        readData(1, var_L, conn_L);
        break;
      }
      default: break;
    }
  }

  void FORTE_F_LEFT::writeOutputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_OUT, conn_OUT);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_F_LEFT::getDI(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_IN;
      case 1: return &var_L;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_F_LEFT::getDO(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_OUT;
    }
    return nullptr;
  }

  CEventConnection *FORTE_F_LEFT::getEOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_F_LEFT::getDIConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_IN;
      case 1: return &conn_L;
    }
    return nullptr;
  }

  CDataConnection *FORTE_F_LEFT::getDOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_OUT;
    }
    return nullptr;
  }

  void FORTE_F_LEFT::setInitialValues() {
    var_IN.reset();
    var_L.reset();
    var_OUT.reset();
  }

} // namespace forte::iec61131::charString
