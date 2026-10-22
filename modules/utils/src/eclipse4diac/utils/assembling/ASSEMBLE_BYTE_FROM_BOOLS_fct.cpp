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
 *** Name: ASSEMBLE_BYTE_FROM_BOOLS
 *** Description: this Function combines the 8 BOOLS to a BYTE
 *** Version:
 ***     3.0.1: 2026-09-29/Martin Jobst -  - fix ST package declaration
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.1: 2024-07-28/Moritz Ortmeier - HR Agrartechnik - rename Function and change Input/Output Variables
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#include "forte/eclipse4diac/utils/assembling/ASSEMBLE_BYTE_FROM_BOOLS_fct.h"

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_byte.h"
#include "forte/eclipse4diac/utils/assembling/ASSEMBLE_BYTE_FROM_BOOLS_fct.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::utils::assembling {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"BIT_00"_STRID, "BIT_01"_STRID, "BIT_02"_STRID, "BIT_03"_STRID,
                                            "BIT_04"_STRID, "BIT_05"_STRID, "BIT_06"_STRID, "BIT_07"_STRID};
    const auto cDataOutputNames = std::array{""_STRID};
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

    CIEC_BYTE func_ASSEMBLE_BYTE_FROM_BOOLS__impl(const CIEC_BOOL &st_lv_BIT_00,
                                                  const CIEC_BOOL &st_lv_BIT_01,
                                                  const CIEC_BOOL &st_lv_BIT_02,
                                                  const CIEC_BOOL &st_lv_BIT_03,
                                                  const CIEC_BOOL &st_lv_BIT_04,
                                                  const CIEC_BOOL &st_lv_BIT_05,
                                                  const CIEC_BOOL &st_lv_BIT_06,
                                                  const CIEC_BOOL &st_lv_BIT_07);
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_ASSEMBLE_BYTE_FROM_BOOLS,
                     "eclipse4diac::utils::assembling::ASSEMBLE_BYTE_FROM_BOOLS"_STRID,
                     TypeHash)

  FORTE_ASSEMBLE_BYTE_FROM_BOOLS::FORTE_ASSEMBLE_BYTE_FROM_BOOLS(const StringId paInstanceNameId,
                                                                 CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_BIT_00(0_BOOL),
      var_BIT_01(0_BOOL),
      var_BIT_02(0_BOOL),
      var_BIT_03(0_BOOL),
      var_BIT_04(0_BOOL),
      var_BIT_05(0_BOOL),
      var_BIT_06(0_BOOL),
      var_BIT_07(0_BOOL),
      var_(0_BYTE),
      conn_CNF(*this, 0),
      conn_BIT_00(nullptr),
      conn_BIT_01(nullptr),
      conn_BIT_02(nullptr),
      conn_BIT_03(nullptr),
      conn_BIT_04(nullptr),
      conn_BIT_05(nullptr),
      conn_BIT_06(nullptr),
      conn_BIT_07(nullptr),
      conn_(*this, 0, var_) {
  }

  void FORTE_ASSEMBLE_BYTE_FROM_BOOLS::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_BIT_00 = 0_BOOL;
    var_BIT_01 = 0_BOOL;
    var_BIT_02 = 0_BOOL;
    var_BIT_03 = 0_BOOL;
    var_BIT_04 = 0_BOOL;
    var_BIT_05 = 0_BOOL;
    var_BIT_06 = 0_BOOL;
    var_BIT_07 = 0_BOOL;
    var_ = 0_BYTE;
  }

  void FORTE_ASSEMBLE_BYTE_FROM_BOOLS::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_BIT_00, conn_BIT_00);
        readData(1, var_BIT_01, conn_BIT_01);
        readData(2, var_BIT_02, conn_BIT_02);
        readData(3, var_BIT_03, conn_BIT_03);
        readData(4, var_BIT_04, conn_BIT_04);
        readData(5, var_BIT_05, conn_BIT_05);
        readData(6, var_BIT_06, conn_BIT_06);
        readData(7, var_BIT_07, conn_BIT_07);
        break;
      }
      default: break;
    }
  }

  void FORTE_ASSEMBLE_BYTE_FROM_BOOLS::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(8, var_, conn_);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_ASSEMBLE_BYTE_FROM_BOOLS::getDI(const size_t paIndex) {
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

  CIEC_ANY *FORTE_ASSEMBLE_BYTE_FROM_BOOLS::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_;
    }
    return nullptr;
  }

  CEventConnection *FORTE_ASSEMBLE_BYTE_FROM_BOOLS::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_ASSEMBLE_BYTE_FROM_BOOLS::getDIConUnchecked(const TPortId paIndex) {
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

  CDataConnection *FORTE_ASSEMBLE_BYTE_FROM_BOOLS::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_;
    }
    return nullptr;
  }

  void FORTE_ASSEMBLE_BYTE_FROM_BOOLS::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    var_ = func_ASSEMBLE_BYTE_FROM_BOOLS__impl(var_BIT_00, var_BIT_01, var_BIT_02, var_BIT_03, var_BIT_04, var_BIT_05,
                                               var_BIT_06, var_BIT_07);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  CIEC_BYTE func_ASSEMBLE_BYTE_FROM_BOOLS(const CIEC_BOOL &st_lv_BIT_00,
                                          const CIEC_BOOL &st_lv_BIT_01,
                                          const CIEC_BOOL &st_lv_BIT_02,
                                          const CIEC_BOOL &st_lv_BIT_03,
                                          const CIEC_BOOL &st_lv_BIT_04,
                                          const CIEC_BOOL &st_lv_BIT_05,
                                          const CIEC_BOOL &st_lv_BIT_06,
                                          const CIEC_BOOL &st_lv_BIT_07) {
    return func_ASSEMBLE_BYTE_FROM_BOOLS__impl(st_lv_BIT_00, st_lv_BIT_01, st_lv_BIT_02, st_lv_BIT_03, st_lv_BIT_04,
                                               st_lv_BIT_05, st_lv_BIT_06, st_lv_BIT_07);
  }

  namespace {
    CIEC_BYTE func_ASSEMBLE_BYTE_FROM_BOOLS__impl(const CIEC_BOOL &st_lv_BIT_00,
                                                  const CIEC_BOOL &st_lv_BIT_01,
                                                  const CIEC_BOOL &st_lv_BIT_02,
                                                  const CIEC_BOOL &st_lv_BIT_03,
                                                  const CIEC_BOOL &st_lv_BIT_04,
                                                  const CIEC_BOOL &st_lv_BIT_05,
                                                  const CIEC_BOOL &st_lv_BIT_06,
                                                  const CIEC_BOOL &st_lv_BIT_07) {
      CIEC_BYTE st_ret_val = 0_BYTE;

#line 17 "ASSEMBLE_BYTE_FROM_BOOLS.fct"
      st_ret_val.partial<CIEC_BOOL>(0) = st_lv_BIT_00;
#line 18 "ASSEMBLE_BYTE_FROM_BOOLS.fct"
      st_ret_val.partial<CIEC_BOOL>(1) = st_lv_BIT_01;
#line 19 "ASSEMBLE_BYTE_FROM_BOOLS.fct"
      st_ret_val.partial<CIEC_BOOL>(2) = st_lv_BIT_02;
#line 20 "ASSEMBLE_BYTE_FROM_BOOLS.fct"
      st_ret_val.partial<CIEC_BOOL>(3) = st_lv_BIT_03;
#line 21 "ASSEMBLE_BYTE_FROM_BOOLS.fct"
      st_ret_val.partial<CIEC_BOOL>(4) = st_lv_BIT_04;
#line 22 "ASSEMBLE_BYTE_FROM_BOOLS.fct"
      st_ret_val.partial<CIEC_BOOL>(5) = st_lv_BIT_05;
#line 23 "ASSEMBLE_BYTE_FROM_BOOLS.fct"
      st_ret_val.partial<CIEC_BOOL>(6) = st_lv_BIT_06;
#line 24 "ASSEMBLE_BYTE_FROM_BOOLS.fct"
      st_ret_val.partial<CIEC_BOOL>(7) = st_lv_BIT_07;

      return st_ret_val;
    }
  } // namespace
} // namespace forte::eclipse4diac::utils::assembling
