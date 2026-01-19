/*******************************************************************************
 * Copyright (c) 2026 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Erich Jobst - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "CFB_DELEGATING_TEST.h"

#include "forte/forte_st_util.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::test {
  DEFINE_FIRMWARE_DATATYPE(DelegatingTestStruct, "test::DelegatingTestStruct"_STRID);

  const StringId CIEC_DelegatingTestStruct::scmElementNames[] = {"VAR1"_STRID, "VAR2"_STRID, "VAR3"_STRID};

  CIEC_DelegatingTestStruct::CIEC_DelegatingTestStruct() :
      CIEC_STRUCT(),
      var_VAR1(0_BOOL),
      var_VAR2(0_BOOL),
      var_VAR3(0_BOOL) {
  }

  CIEC_DelegatingTestStruct::CIEC_DelegatingTestStruct(const CIEC_BOOL &paVAR1,
                                                       const CIEC_BOOL &paVAR2,
                                                       const CIEC_BOOL &paVAR3) :
      CIEC_STRUCT(),
      var_VAR1(paVAR1),
      var_VAR2(paVAR2),
      var_VAR3(paVAR3) {
  }

  StringId CIEC_DelegatingTestStruct::getStructTypeNameID() const {
    return "test::DelegatingTestStruct"_STRID;
  }

  void CIEC_DelegatingTestStruct::setValue(const CIEC_ANY &paValue) {
    if (paValue.getDataTypeID() == e_STRUCT) {
      auto &otherStruct = static_cast<const CIEC_STRUCT &>(paValue);
      if ("test::DelegatingTestStruct"_STRID == otherStruct.getStructTypeNameID()) {
        operator=(static_cast<const CIEC_DelegatingTestStruct &>(paValue));
      }
    }
  }

  CIEC_ANY *CIEC_DelegatingTestStruct::getMember(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_VAR1;
      case 1: return &var_VAR2;
      case 2: return &var_VAR3;
    }
    return nullptr;
  }

  const CIEC_ANY *CIEC_DelegatingTestStruct::getMember(const size_t paIndex) const {
    switch (paIndex) {
      case 0: return &var_VAR1;
      case 1: return &var_VAR2;
      case 2: return &var_VAR3;
    }
    return nullptr;
  }

  namespace {
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"QI"_STRID, "DI1"_STRID};
    const auto cDataOutputNames = std::array{"QO"_STRID, "DO1"_STRID};

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

    const auto cEventConnections = std::to_array<SCFB_FBConnectionData>({
        {"F_MOVE_2"_STRID, "CNF"_STRID, "F_MOVE_3"_STRID, "REQ"_STRID},
        {{}, "REQ"_STRID, "F_MOVE_2"_STRID, "REQ"_STRID},
        {"F_MOVE_3"_STRID, "CNF"_STRID, "F_MOVE"_STRID, "REQ"_STRID},
        {"F_MOVE"_STRID, "CNF"_STRID, "F_MOVE_1"_STRID, "REQ"_STRID},
        {"F_MOVE_1"_STRID, "CNF"_STRID, {}, "CNF"_STRID},
    });

    const auto ep_IN__VAR1 = std::array{"IN"_STRID, "VAR1"_STRID};
    const auto ep_IN__VAR2 = std::array{"IN"_STRID, "VAR2"_STRID};
    const auto ep_IN__VAR3 = std::array{"IN"_STRID, "VAR3"_STRID};
    const auto ep_OUT__NOT = std::array{"OUT"_STRID, "NOT"_STRID};
    const auto ep_OUT__VAR1 = std::array{"OUT"_STRID, "VAR1"_STRID};
    const auto ep_OUT__VAR2 = std::array{"OUT"_STRID, "VAR2"_STRID};
    const auto ep_OUT__VAR3 = std::array{"OUT"_STRID, "VAR3"_STRID};
    const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
        {"F_MOVE"_STRID, ep_OUT__VAR1, "F_MOVE_1"_STRID, ep_IN__VAR1},
        {"F_MOVE"_STRID, ep_OUT__VAR2, "F_MOVE_1"_STRID, ep_IN__VAR2},
        {"F_MOVE"_STRID, ep_OUT__VAR3, "F_MOVE_1"_STRID, ep_IN__VAR3},
        {"F_MOVE_2"_STRID, ep_OUT__NOT, "F_MOVE_3"_STRID, "IN"_STRID},
        {{}, "QI"_STRID, "F_MOVE_2"_STRID, "IN"_STRID},
        {"F_MOVE_3"_STRID, "OUT"_STRID, {}, "QO"_STRID},
        {{}, "DI1"_STRID, "F_MOVE"_STRID, "IN"_STRID},
        {"F_MOVE_1"_STRID, "OUT"_STRID, {}, "DO1"_STRID},
    });

    const SCFB_FBNData cFBNData = {
        .mEventConnections = cEventConnections,
        .mDataConnections = cDataConnections,
        .mAdapterConnections = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_CFB_DELEGATING_TEST, "test::CFB_DELEGATING_TEST"_STRID)

  FORTE_CFB_DELEGATING_TEST::FORTE_CFB_DELEGATING_TEST(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
      fb_F_MOVE("F_MOVE"_STRID, "iec61131::selection::F_MOVE_1test::DelegatingTestStruct", *this),
      fb_F_MOVE_1("F_MOVE_1"_STRID, "iec61131::selection::F_MOVE_1test::DelegatingTestStruct", *this),
      fb_F_MOVE_2("F_MOVE_2"_STRID, "iec61131::selection::F_MOVE_1BOOL", *this),
      fb_F_MOVE_3("F_MOVE_3"_STRID, "iec61131::selection::F_MOVE_1BOOL", *this),
      conn_CNF(*this, 0),
      conn_QI(nullptr),
      conn_DI1(nullptr),
      conn_QO(*this, 0, 0_BOOL),
      conn_DO1(*this, 1, forte::test::CIEC_DelegatingTestStruct()),
      conn_if2in_QI(*this, 0, 0_BOOL),
      conn_if2in_DI1(*this, 1, forte::test::CIEC_DelegatingTestStruct()) {};

  void FORTE_CFB_DELEGATING_TEST::setInitialValues() {
    CCompositeFB::setInitialValues();
    conn_if2in_QI.getValue() = 0_BOOL;
    conn_if2in_DI1.getValue() = forte::test::CIEC_DelegatingTestStruct();
    fb_F_MOVE_3->getDOConnection("OUT"_STRID)->getValue().setValue(0_BOOL);
    fb_F_MOVE_1->getDOConnection("OUT"_STRID)->getValue().setValue(forte::test::CIEC_DelegatingTestStruct());
  }

  void FORTE_CFB_DELEGATING_TEST::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, conn_if2in_QI.getValue(), conn_QI);
        readData(1, conn_if2in_DI1.getValue(), conn_DI1);
        break;
      }
      default: break;
    }
  }

  void FORTE_CFB_DELEGATING_TEST::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(2, fb_F_MOVE_3->getDOConnection("OUT"_STRID)->getValue(), conn_QO);
        writeData(3, fb_F_MOVE_1->getDOConnection("OUT"_STRID)->getValue(), conn_DO1);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_CFB_DELEGATING_TEST::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_QI.getValue();
      case 1: return &conn_if2in_DI1.getValue();
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_CFB_DELEGATING_TEST::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &fb_F_MOVE_3->getDOConnection("OUT"_STRID)->getValue();
      case 1: return &fb_F_MOVE_1->getDOConnection("OUT"_STRID)->getValue();
    }
    return nullptr;
  }

  CEventConnection *FORTE_CFB_DELEGATING_TEST::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_CFB_DELEGATING_TEST::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_DI1;
    }
    return nullptr;
  }

  CDataConnection *FORTE_CFB_DELEGATING_TEST::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_DO1;
    }
    return nullptr;
  }

  CDataConnection *FORTE_CFB_DELEGATING_TEST::getIf2InConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_if2in_QI;
      case 1: return &conn_if2in_DI1;
    }
    return nullptr;
  }

} // namespace forte::test
