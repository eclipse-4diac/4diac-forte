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
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: SPLIT_BYTE_INTO_QUARTERS
 *** Description: this Function extracts the 4 QUARTER BYTE from a byte
 *** Version:
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 ***     1.1: 2024-07-24/Moritz Ortmeier - HR Agrartechnik - rename Function and change Output Variables
 *************************************************************************/

#include "forte/eclipse4diac/utils/splitting/SPLIT_BYTE_INTO_QUARTERS_fct.h"

using namespace forte::literals;

#include "forte/datatypes/forte_any_int_variant.h"
#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_any_bit_variant.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"
#include "forte/eclipse4diac/utils/const/quarterconst_gcf.h"

namespace forte::eclipse4diac::utils::splitting {
  namespace {
    const auto cDataInputNames = std::array{"IN"_STRID};
    const auto cDataOutputNames =
        std::array{"QUARTER_BYTE_00"_STRID, "QUARTER_BYTE_01"_STRID, "QUARTER_BYTE_02"_STRID, "QUARTER_BYTE_03"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_SPLIT_BYTE_INTO_QUARTERS, "eclipse4diac::utils::splitting::SPLIT_BYTE_INTO_QUARTERS"_STRID)

  FORTE_SPLIT_BYTE_INTO_QUARTERS::FORTE_SPLIT_BYTE_INTO_QUARTERS(const forte::StringId paInstanceNameId,
                                                                 forte::CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_QUARTER_BYTE_00(*this, 0, var_QUARTER_BYTE_00),
      conn_QUARTER_BYTE_01(*this, 1, var_QUARTER_BYTE_01),
      conn_QUARTER_BYTE_02(*this, 2, var_QUARTER_BYTE_02),
      conn_QUARTER_BYTE_03(*this, 3, var_QUARTER_BYTE_03) {
  }

  void FORTE_SPLIT_BYTE_INTO_QUARTERS::setInitialValues() {
    var_IN = 0_BYTE;
    var_QUARTER_BYTE_00 = 0_BYTE;
    var_QUARTER_BYTE_01 = 0_BYTE;
    var_QUARTER_BYTE_02 = 0_BYTE;
    var_QUARTER_BYTE_03 = 0_BYTE;
  }

  void FORTE_SPLIT_BYTE_INTO_QUARTERS::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_IN, conn_IN);
        break;
      }
      default: break;
    }
  }

  void FORTE_SPLIT_BYTE_INTO_QUARTERS::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QUARTER_BYTE_00, conn_QUARTER_BYTE_00);
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_QUARTER_BYTE_01, conn_QUARTER_BYTE_01);
        writeData(cFBInterfaceSpec.getNumDIs() + 2, var_QUARTER_BYTE_02, conn_QUARTER_BYTE_02);
        writeData(cFBInterfaceSpec.getNumDIs() + 3, var_QUARTER_BYTE_03, conn_QUARTER_BYTE_03);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_SPLIT_BYTE_INTO_QUARTERS::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_IN;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_SPLIT_BYTE_INTO_QUARTERS::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QUARTER_BYTE_00;
      case 1: return &var_QUARTER_BYTE_01;
      case 2: return &var_QUARTER_BYTE_02;
      case 3: return &var_QUARTER_BYTE_03;
    }
    return nullptr;
  }

  CEventConnection *FORTE_SPLIT_BYTE_INTO_QUARTERS::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_SPLIT_BYTE_INTO_QUARTERS::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_IN;
    }
    return nullptr;
  }

  CDataConnection *FORTE_SPLIT_BYTE_INTO_QUARTERS::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QUARTER_BYTE_00;
      case 1: return &conn_QUARTER_BYTE_01;
      case 2: return &conn_QUARTER_BYTE_02;
      case 3: return &conn_QUARTER_BYTE_03;
    }
    return nullptr;
  }

  void FORTE_SPLIT_BYTE_INTO_QUARTERS::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    func_SPLIT_BYTE_INTO_QUARTERS(var_IN, var_QUARTER_BYTE_00, var_QUARTER_BYTE_01, var_QUARTER_BYTE_02,
                                  var_QUARTER_BYTE_03);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void func_SPLIT_BYTE_INTO_QUARTERS(CIEC_BYTE st_lv_IN,
                                     CIEC_BYTE &st_lv_QUARTER_BYTE_00,
                                     CIEC_BYTE &st_lv_QUARTER_BYTE_01,
                                     CIEC_BYTE &st_lv_QUARTER_BYTE_02,
                                     CIEC_BYTE &st_lv_QUARTER_BYTE_03) {
    st_lv_QUARTER_BYTE_00 = 0_BYTE;
    st_lv_QUARTER_BYTE_01 = 0_BYTE;
    st_lv_QUARTER_BYTE_02 = 0_BYTE;
    st_lv_QUARTER_BYTE_03 = 0_BYTE;

#line 15 "SPLIT_BYTE_INTO_QUARTERS.fct"
    st_lv_QUARTER_BYTE_00 = func_SHR(
        func_AND<CIEC_BYTE>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_BYTE_QUARTER_00),
        forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_00);
#line 16 "SPLIT_BYTE_INTO_QUARTERS.fct"
    st_lv_QUARTER_BYTE_01 = func_SHR(
        func_AND<CIEC_BYTE>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_BYTE_QUARTER_01),
        forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_01);
#line 17 "SPLIT_BYTE_INTO_QUARTERS.fct"
    st_lv_QUARTER_BYTE_02 = func_SHR(
        func_AND<CIEC_BYTE>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_BYTE_QUARTER_02),
        forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_02);
#line 18 "SPLIT_BYTE_INTO_QUARTERS.fct"
    st_lv_QUARTER_BYTE_03 = func_SHR(
        func_AND<CIEC_BYTE>(st_lv_IN, forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_BYTE_QUARTER_03),
        forte::eclipse4diac::utils::const_::FORTE_quarterconst::var_SHIFT_QUARTER_03);
  }
} // namespace forte::eclipse4diac::utils::splitting
