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

#include "forte/iec61131/charString/F_INSERT_fbt.h"

using namespace forte::literals;

namespace forte::iec61131::charString {
  namespace {
    const auto cDataInputNames = std::array{"IN1"_STRID, "IN2"_STRID, "P"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_F_INSERT, "iec61131::charString::F_INSERT"_STRID)

  FORTE_F_INSERT::FORTE_F_INSERT(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_IN1(CIEC_ANY_STRING_VARIANT()),
      var_IN2(CIEC_ANY_STRING_VARIANT()),
      var_P(CIEC_ANY_INT_VARIANT()),
      var_OUT(CIEC_ANY_STRING_VARIANT()),
      conn_CNF(*this, 0),
      conn_IN1(nullptr),
      conn_IN2(nullptr),
      conn_P(nullptr),
      conn_OUT(*this, 0, var_OUT) {};

  void FORTE_F_INSERT::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID:
        var_OUT = std::visit(
            [](auto &&paIN1, auto &&paIN2, auto &&paP) -> CIEC_ANY_STRING_VARIANT {
              using T = std::decay_t<decltype(paIN1)>;
              using U = std::decay_t<decltype(paIN2)>;
              if constexpr (std::is_same_v<T, U>) {
                return func_INSERT(paIN1, paIN2, paP);
              }
              DEVLOG_ERROR("Inserting incompatible types %s and %s\n", paIN1.getTypeNameID().data(),
                           paIN2.getTypeNameID().data());
              return CIEC_ANY_STRING_VARIANT();
            },
            static_cast<CIEC_ANY_STRING_VARIANT::variant &>(var_IN1),
            static_cast<CIEC_ANY_STRING_VARIANT::variant &>(var_IN2),
            static_cast<CIEC_ANY_INT_VARIANT::variant &>(var_P));
        sendOutputEvent(scmEventCNFID, paECET);
        break;
    }
  }

  void FORTE_F_INSERT::readInputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_IN1, conn_IN1);
        readData(1, var_IN2, conn_IN2);
        readData(2, var_P, conn_P);
        break;
      }
      default: break;
    }
  }

  void FORTE_F_INSERT::writeOutputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_OUT, conn_OUT);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_F_INSERT::getDI(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_IN1;
      case 1: return &var_IN2;
      case 2: return &var_P;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_F_INSERT::getDO(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_OUT;
    }
    return nullptr;
  }

  CEventConnection *FORTE_F_INSERT::getEOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_F_INSERT::getDIConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_IN1;
      case 1: return &conn_IN2;
      case 2: return &conn_P;
    }
    return nullptr;
  }

  CDataConnection *FORTE_F_INSERT::getDOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_OUT;
    }
    return nullptr;
  }

  void FORTE_F_INSERT::setInitialValues() {
    var_IN1.reset();
    var_IN2.reset();
    var_P.reset();
    var_OUT.reset();
  }

} // namespace forte::iec61131::charString
