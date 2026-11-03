/*************************************************************************
 *** Copyright (c) 2024 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.3.0.qualifier!
 ***
 *** Name: SPLIT_BYTE_INTO_BOOLS
 *** Description: this Function extracts the 8 BOOLS from a byte
 *** Version:
 ***     3.0.1: 2026-09-29/Martin Jobst -  - fix ST package declaration
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.1: 2024-07-28/Moritz Ortmeier - HR Agrartechnik - rename Function and change Output Variables
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#include "forte/eclipse4diac/utils/splitting/SPLIT_BYTE_INTO_BOOLS_fct.h"

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_byte.h"
#include "forte/forte_st_util.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::utils::splitting {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"IN"_STRID};
    const auto cDataOutputNames = std::array{"BIT_00"_STRID, "BIT_01"_STRID, "BIT_02"_STRID, "BIT_03"_STRID,
                                             "BIT_04"_STRID, "BIT_05"_STRID, "BIT_06"_STRID, "BIT_07"_STRID};
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

    void func_SPLIT_BYTE_INTO_BOOLS__impl(const CIEC_BYTE &st_lv_IN,
                                          CIEC_BOOL &st_lv_BIT_00,
                                          CIEC_BOOL &st_lv_BIT_01,
                                          CIEC_BOOL &st_lv_BIT_02,
                                          CIEC_BOOL &st_lv_BIT_03,
                                          CIEC_BOOL &st_lv_BIT_04,
                                          CIEC_BOOL &st_lv_BIT_05,
                                          CIEC_BOOL &st_lv_BIT_06,
                                          CIEC_BOOL &st_lv_BIT_07);
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_SPLIT_BYTE_INTO_BOOLS,
                     "eclipse4diac::utils::splitting::SPLIT_BYTE_INTO_BOOLS"_STRID,
                     TypeHash)

  FORTE_SPLIT_BYTE_INTO_BOOLS::FORTE_SPLIT_BYTE_INTO_BOOLS(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_IN(0_BYTE),
      var_BIT_00(0_BOOL),
      var_BIT_01(0_BOOL),
      var_BIT_02(0_BOOL),
      var_BIT_03(0_BOOL),
      var_BIT_04(0_BOOL),
      var_BIT_05(0_BOOL),
      var_BIT_06(0_BOOL),
      var_BIT_07(0_BOOL),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_BIT_00(*this, 0, var_BIT_00),
      conn_BIT_01(*this, 1, var_BIT_01),
      conn_BIT_02(*this, 2, var_BIT_02),
      conn_BIT_03(*this, 3, var_BIT_03),
      conn_BIT_04(*this, 4, var_BIT_04),
      conn_BIT_05(*this, 5, var_BIT_05),
      conn_BIT_06(*this, 6, var_BIT_06),
      conn_BIT_07(*this, 7, var_BIT_07) {
  }

  void FORTE_SPLIT_BYTE_INTO_BOOLS::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_IN = 0_BYTE;
    var_BIT_00 = 0_BOOL;
    var_BIT_01 = 0_BOOL;
    var_BIT_02 = 0_BOOL;
    var_BIT_03 = 0_BOOL;
    var_BIT_04 = 0_BOOL;
    var_BIT_05 = 0_BOOL;
    var_BIT_06 = 0_BOOL;
    var_BIT_07 = 0_BOOL;
  }

  void FORTE_SPLIT_BYTE_INTO_BOOLS::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_IN, conn_IN);
        break;
      }
      default: break;
    }
  }

  void FORTE_SPLIT_BYTE_INTO_BOOLS::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(1, var_BIT_00, conn_BIT_00);
        writeData(2, var_BIT_01, conn_BIT_01);
        writeData(3, var_BIT_02, conn_BIT_02);
        writeData(4, var_BIT_03, conn_BIT_03);
        writeData(5, var_BIT_04, conn_BIT_04);
        writeData(6, var_BIT_05, conn_BIT_05);
        writeData(7, var_BIT_06, conn_BIT_06);
        writeData(8, var_BIT_07, conn_BIT_07);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_SPLIT_BYTE_INTO_BOOLS::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_IN;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_SPLIT_BYTE_INTO_BOOLS::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_BIT_00;
      case 1: return &var_BIT_01;
      case 2: return &var_BIT_02;
      case 3: return &var_BIT_03;
      case 4: return &var_BIT_04;
      case 5: return &var_BIT_05;
      case 6: return &var_BIT_06;
      case 7: return &var_BIT_07;
    }
    return nullptr;
  }

  CEventConnection *FORTE_SPLIT_BYTE_INTO_BOOLS::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_SPLIT_BYTE_INTO_BOOLS::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_IN;
    }
    return nullptr;
  }

  CDataConnection *FORTE_SPLIT_BYTE_INTO_BOOLS::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_BIT_00;
      case 1: return &conn_BIT_01;
      case 2: return &conn_BIT_02;
      case 3: return &conn_BIT_03;
      case 4: return &conn_BIT_04;
      case 5: return &conn_BIT_05;
      case 6: return &conn_BIT_06;
      case 7: return &conn_BIT_07;
    }
    return nullptr;
  }

  void FORTE_SPLIT_BYTE_INTO_BOOLS::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    func_SPLIT_BYTE_INTO_BOOLS__impl(var_IN, var_BIT_00, var_BIT_01, var_BIT_02, var_BIT_03, var_BIT_04, var_BIT_05,
                                     var_BIT_06, var_BIT_07);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void func_SPLIT_BYTE_INTO_BOOLS(const CIEC_BYTE &st_lv_IN,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_00,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_01,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_02,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_03,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_04,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_05,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_06,
                                  CAnyBitOutputParameter<CIEC_BOOL> st_lv_BIT_07) {
    COutputGuard st_guard_BIT_00(st_lv_BIT_00);
    COutputGuard st_guard_BIT_01(st_lv_BIT_01);
    COutputGuard st_guard_BIT_02(st_lv_BIT_02);
    COutputGuard st_guard_BIT_03(st_lv_BIT_03);
    COutputGuard st_guard_BIT_04(st_lv_BIT_04);
    COutputGuard st_guard_BIT_05(st_lv_BIT_05);
    COutputGuard st_guard_BIT_06(st_lv_BIT_06);
    COutputGuard st_guard_BIT_07(st_lv_BIT_07);
    func_SPLIT_BYTE_INTO_BOOLS__impl(st_lv_IN, *st_lv_BIT_00, *st_lv_BIT_01, *st_lv_BIT_02, *st_lv_BIT_03,
                                     *st_lv_BIT_04, *st_lv_BIT_05, *st_lv_BIT_06, *st_lv_BIT_07);
  }

  namespace {
    void func_SPLIT_BYTE_INTO_BOOLS__impl(const CIEC_BYTE &st_lv_IN,
                                          CIEC_BOOL &st_lv_BIT_00,
                                          CIEC_BOOL &st_lv_BIT_01,
                                          CIEC_BOOL &st_lv_BIT_02,
                                          CIEC_BOOL &st_lv_BIT_03,
                                          CIEC_BOOL &st_lv_BIT_04,
                                          CIEC_BOOL &st_lv_BIT_05,
                                          CIEC_BOOL &st_lv_BIT_06,
                                          CIEC_BOOL &st_lv_BIT_07) {
      st_lv_BIT_00 = 0_BOOL;
      st_lv_BIT_01 = 0_BOOL;
      st_lv_BIT_02 = 0_BOOL;
      st_lv_BIT_03 = 0_BOOL;
      st_lv_BIT_04 = 0_BOOL;
      st_lv_BIT_05 = 0_BOOL;
      st_lv_BIT_06 = 0_BOOL;
      st_lv_BIT_07 = 0_BOOL;

#line 21 "SPLIT_BYTE_INTO_BOOLS.fct"
      st_lv_BIT_00 = st_lv_IN.cpartial<CIEC_BOOL>(0);
#line 22 "SPLIT_BYTE_INTO_BOOLS.fct"
      st_lv_BIT_01 = st_lv_IN.cpartial<CIEC_BOOL>(1);
#line 23 "SPLIT_BYTE_INTO_BOOLS.fct"
      st_lv_BIT_02 = st_lv_IN.cpartial<CIEC_BOOL>(2);
#line 24 "SPLIT_BYTE_INTO_BOOLS.fct"
      st_lv_BIT_03 = st_lv_IN.cpartial<CIEC_BOOL>(3);
#line 25 "SPLIT_BYTE_INTO_BOOLS.fct"
      st_lv_BIT_04 = st_lv_IN.cpartial<CIEC_BOOL>(4);
#line 26 "SPLIT_BYTE_INTO_BOOLS.fct"
      st_lv_BIT_05 = st_lv_IN.cpartial<CIEC_BOOL>(5);
#line 27 "SPLIT_BYTE_INTO_BOOLS.fct"
      st_lv_BIT_06 = st_lv_IN.cpartial<CIEC_BOOL>(6);
#line 28 "SPLIT_BYTE_INTO_BOOLS.fct"
      st_lv_BIT_07 = st_lv_IN.cpartial<CIEC_BOOL>(7);
    }
  } // namespace
} // namespace forte::eclipse4diac::utils::splitting
