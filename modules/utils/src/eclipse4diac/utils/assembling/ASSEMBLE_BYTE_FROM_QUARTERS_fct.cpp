/*******************************************************************************
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
 *** Name: ASSEMBLE_BYTE_FROM_QUARTERS
 *** Description: this Function combines the 4 QUARTER BYTES to a BYTE
 *** Version:
 ***     3.0.1: 2026-09-29/Martin Jobst -  - fix ST package declaration
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.1: 2024-07-28/Moritz Ortmeier - HR Agrartechnik - rename Function and change Input/Output Variables
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#include "forte/eclipse4diac/utils/assembling/ASSEMBLE_BYTE_FROM_QUARTERS_fct.h"

#include "forte/datatypes/forte_byte.h"
#include "forte/eclipse4diac/utils/assembling/ASSEMBLE_BYTE_FROM_QUARTERS_fct.h"
#include "forte/eclipse4diac/utils/const/quarterconst_gcf.h"
#include "forte/iec61131_functions/func_OR.h"
#include "forte/iec61131_functions/func_SHL.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::utils::assembling {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames =
        std::array{"QUARTER_BYTE_00"_STRID, "QUARTER_BYTE_01"_STRID, "QUARTER_BYTE_02"_STRID, "QUARTER_BYTE_03"_STRID};
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

    CIEC_BYTE func_ASSEMBLE_BYTE_FROM_QUARTERS__impl(const CIEC_BYTE &st_lv_QUARTER_BYTE_00,
                                                     const CIEC_BYTE &st_lv_QUARTER_BYTE_01,
                                                     const CIEC_BYTE &st_lv_QUARTER_BYTE_02,
                                                     const CIEC_BYTE &st_lv_QUARTER_BYTE_03);
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_ASSEMBLE_BYTE_FROM_QUARTERS,
                     "eclipse4diac::utils::assembling::ASSEMBLE_BYTE_FROM_QUARTERS"_STRID,
                     TypeHash)

  FORTE_ASSEMBLE_BYTE_FROM_QUARTERS::FORTE_ASSEMBLE_BYTE_FROM_QUARTERS(const StringId paInstanceNameId,
                                                                       CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_QUARTER_BYTE_00(0_BYTE),
      var_QUARTER_BYTE_01(0_BYTE),
      var_QUARTER_BYTE_02(0_BYTE),
      var_QUARTER_BYTE_03(0_BYTE),
      var_(0_BYTE),
      conn_CNF(*this, 0),
      conn_QUARTER_BYTE_00(nullptr),
      conn_QUARTER_BYTE_01(nullptr),
      conn_QUARTER_BYTE_02(nullptr),
      conn_QUARTER_BYTE_03(nullptr),
      conn_(*this, 0, var_) {
  }

  void FORTE_ASSEMBLE_BYTE_FROM_QUARTERS::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_QUARTER_BYTE_00 = 0_BYTE;
    var_QUARTER_BYTE_01 = 0_BYTE;
    var_QUARTER_BYTE_02 = 0_BYTE;
    var_QUARTER_BYTE_03 = 0_BYTE;
    var_ = 0_BYTE;
  }

  void FORTE_ASSEMBLE_BYTE_FROM_QUARTERS::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_QUARTER_BYTE_00, conn_QUARTER_BYTE_00);
        readData(1, var_QUARTER_BYTE_01, conn_QUARTER_BYTE_01);
        readData(2, var_QUARTER_BYTE_02, conn_QUARTER_BYTE_02);
        readData(3, var_QUARTER_BYTE_03, conn_QUARTER_BYTE_03);
        break;
      }
      default: break;
    }
  }

  void FORTE_ASSEMBLE_BYTE_FROM_QUARTERS::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(4, var_, conn_);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_ASSEMBLE_BYTE_FROM_QUARTERS::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QUARTER_BYTE_00;
      case 1: return &var_QUARTER_BYTE_01;
      case 2: return &var_QUARTER_BYTE_02;
      case 3: return &var_QUARTER_BYTE_03;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_ASSEMBLE_BYTE_FROM_QUARTERS::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_;
    }
    return nullptr;
  }

  CEventConnection *FORTE_ASSEMBLE_BYTE_FROM_QUARTERS::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_ASSEMBLE_BYTE_FROM_QUARTERS::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QUARTER_BYTE_00;
      case 1: return &conn_QUARTER_BYTE_01;
      case 2: return &conn_QUARTER_BYTE_02;
      case 3: return &conn_QUARTER_BYTE_03;
    }
    return nullptr;
  }

  CDataConnection *FORTE_ASSEMBLE_BYTE_FROM_QUARTERS::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_;
    }
    return nullptr;
  }

  void FORTE_ASSEMBLE_BYTE_FROM_QUARTERS::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    var_ = func_ASSEMBLE_BYTE_FROM_QUARTERS__impl(var_QUARTER_BYTE_00, var_QUARTER_BYTE_01, var_QUARTER_BYTE_02,
                                                  var_QUARTER_BYTE_03);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  CIEC_BYTE func_ASSEMBLE_BYTE_FROM_QUARTERS(const CIEC_BYTE &st_lv_QUARTER_BYTE_00,
                                             const CIEC_BYTE &st_lv_QUARTER_BYTE_01,
                                             const CIEC_BYTE &st_lv_QUARTER_BYTE_02,
                                             const CIEC_BYTE &st_lv_QUARTER_BYTE_03) {
    return func_ASSEMBLE_BYTE_FROM_QUARTERS__impl(st_lv_QUARTER_BYTE_00, st_lv_QUARTER_BYTE_01, st_lv_QUARTER_BYTE_02,
                                                  st_lv_QUARTER_BYTE_03);
  }

  namespace {
    CIEC_BYTE func_ASSEMBLE_BYTE_FROM_QUARTERS__impl(const CIEC_BYTE &st_lv_QUARTER_BYTE_00,
                                                     const CIEC_BYTE &st_lv_QUARTER_BYTE_01,
                                                     const CIEC_BYTE &st_lv_QUARTER_BYTE_02,
                                                     const CIEC_BYTE &st_lv_QUARTER_BYTE_03) {
      CIEC_BYTE st_ret_val = 0_BYTE;

#line 15 "ASSEMBLE_BYTE_FROM_QUARTERS.fct"
      st_ret_val = forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_BYTE_QUARTER_ZZ;
#line 16 "ASSEMBLE_BYTE_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_BYTE>(
          st_ret_val, func_SHL(st_lv_QUARTER_BYTE_03,
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_03));
#line 17 "ASSEMBLE_BYTE_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_BYTE>(
          st_ret_val, func_SHL(st_lv_QUARTER_BYTE_02,
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_02));
#line 18 "ASSEMBLE_BYTE_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_BYTE>(
          st_ret_val, func_SHL(st_lv_QUARTER_BYTE_01,
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_01));
#line 19 "ASSEMBLE_BYTE_FROM_QUARTERS.fct"
      st_ret_val = func_OR<CIEC_BYTE>(
          st_ret_val, func_SHL(st_lv_QUARTER_BYTE_00,
                               forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_00));

      return st_ret_val;
    }
  } // namespace
} // namespace forte::eclipse4diac::utils::assembling
