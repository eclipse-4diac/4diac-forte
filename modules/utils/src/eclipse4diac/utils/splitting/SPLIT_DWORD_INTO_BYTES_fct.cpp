/*************************************************************************
 *** Copyright (c) 2025 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.0.0.202512201229!
 ***
 *** Name: SPLIT_DWORD_INTO_BYTES
 *** Description: this Function extracts the 4 BYTE from a dword
 *** Version:
 ***     1.0: 2025-12-21/Franz Höpfinger - HR Agrartechnik - initial Version
 *************************************************************************/

#include "forte/eclipse4diac/utils/splitting/SPLIT_DWORD_INTO_BYTES_fct.h"

#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/forte_st_util.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::utils::splitting {
  namespace {
    constexpr std::string_view TypeHash =""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"IN"_STRID};
    const auto cDataOutputNames = std::array{"BYTE_00"_STRID, "BYTE_01"_STRID, "BYTE_02"_STRID, "BYTE_03"_STRID};
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
  }

  DEFINE_FIRMWARE_FB(FORTE_SPLIT_DWORD_INTO_BYTES, "eclipse4diac::utils::splitting::SPLIT_DWORD_INTO_BYTES"_STRID, TypeHash)

  FORTE_SPLIT_DWORD_INTO_BYTES::FORTE_SPLIT_DWORD_INTO_BYTES(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_IN(0_DWORD),
      var_BYTE_00(0_BYTE),
      var_BYTE_01(0_BYTE),
      var_BYTE_02(0_BYTE),
      var_BYTE_03(0_BYTE),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_BYTE_00(*this, 0, var_BYTE_00),
      conn_BYTE_01(*this, 1, var_BYTE_01),
      conn_BYTE_02(*this, 2, var_BYTE_02),
      conn_BYTE_03(*this, 3, var_BYTE_03) {
  }

  void FORTE_SPLIT_DWORD_INTO_BYTES::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_IN = 0_DWORD;
    var_BYTE_00 = 0_BYTE;
    var_BYTE_01 = 0_BYTE;
    var_BYTE_02 = 0_BYTE;
    var_BYTE_03 = 0_BYTE;
  }

  void FORTE_SPLIT_DWORD_INTO_BYTES::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventREQID: {
        readData(0, var_IN, conn_IN);
        break;
      }
      default:
        break;
    }
  }

  void FORTE_SPLIT_DWORD_INTO_BYTES::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventCNFID: {
        writeData(1, var_BYTE_00, conn_BYTE_00);
        writeData(2, var_BYTE_01, conn_BYTE_01);
        writeData(3, var_BYTE_02, conn_BYTE_02);
        writeData(4, var_BYTE_03, conn_BYTE_03);
        break;
      }
      default:
        break;
    }
  }

  CIEC_ANY *FORTE_SPLIT_DWORD_INTO_BYTES::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_IN;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_SPLIT_DWORD_INTO_BYTES::getDO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_BYTE_00;
      case 1: return &var_BYTE_01;
      case 2: return &var_BYTE_02;
      case 3: return &var_BYTE_03;
    }
    return nullptr;
  }

  CEventConnection *FORTE_SPLIT_DWORD_INTO_BYTES::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_SPLIT_DWORD_INTO_BYTES::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_IN;
    }
    return nullptr;
  }

  CDataConnection *FORTE_SPLIT_DWORD_INTO_BYTES::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_BYTE_00;
      case 1: return &conn_BYTE_01;
      case 2: return &conn_BYTE_02;
      case 3: return &conn_BYTE_03;
    }
    return nullptr;
  }

  void FORTE_SPLIT_DWORD_INTO_BYTES::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    func_SPLIT_DWORD_INTO_BYTES(var_IN, var_BYTE_00, var_BYTE_01, var_BYTE_02, var_BYTE_03);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void func_SPLIT_DWORD_INTO_BYTES(const CIEC_DWORD &st_lv_IN, CAnyBitOutputParameter<CIEC_BYTE> st_lv_BYTE_00, CAnyBitOutputParameter<CIEC_BYTE> st_lv_BYTE_01, CAnyBitOutputParameter<CIEC_BYTE> st_lv_BYTE_02, CAnyBitOutputParameter<CIEC_BYTE> st_lv_BYTE_03) {
    COutputGuard st_guard_BYTE_00(st_lv_BYTE_00);
    COutputGuard st_guard_BYTE_01(st_lv_BYTE_01);
    COutputGuard st_guard_BYTE_02(st_lv_BYTE_02);
    COutputGuard st_guard_BYTE_03(st_lv_BYTE_03);
    (*st_lv_BYTE_00) = 0_BYTE;
    (*st_lv_BYTE_01) = 0_BYTE;
    (*st_lv_BYTE_02) = 0_BYTE;
    (*st_lv_BYTE_03) = 0_BYTE;

    #line 15 "SPLIT_DWORD_INTO_BYTES.fct"
    (*st_lv_BYTE_00) = st_lv_IN.cpartial<CIEC_BYTE>(0);
    #line 16 "SPLIT_DWORD_INTO_BYTES.fct"
    (*st_lv_BYTE_01) = st_lv_IN.cpartial<CIEC_BYTE>(1);
    #line 17 "SPLIT_DWORD_INTO_BYTES.fct"
    (*st_lv_BYTE_02) = st_lv_IN.cpartial<CIEC_BYTE>(2);
    #line 18 "SPLIT_DWORD_INTO_BYTES.fct"
    (*st_lv_BYTE_03) = st_lv_IN.cpartial<CIEC_BYTE>(3);

  }

}