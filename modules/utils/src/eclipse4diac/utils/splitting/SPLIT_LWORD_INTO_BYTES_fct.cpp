/*************************************************************************
 *** Copyright (c) 2026 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.1.100.202605112003!
 ***
 *** Name: SPLIT_LWORD_INTO_BYTES
 *** Description: this Function extracts the 8 BYTES from a lword
 *** Version:
 ***     1.0: 2026-05-16/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#include "forte/eclipse4diac/utils/splitting/SPLIT_LWORD_INTO_BYTES_fct.h"

#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_lword.h"
#include "forte/forte_st_util.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::utils::splitting {
  namespace {
    constexpr std::string_view TypeHash =""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"IN"_STRID};
    const auto cDataOutputNames = std::array{"BYTE_00"_STRID, "BYTE_01"_STRID, "BYTE_02"_STRID, "BYTE_03"_STRID, "BYTE_04"_STRID, "BYTE_05"_STRID, "BYTE_06"_STRID, "BYTE_07"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_SPLIT_LWORD_INTO_BYTES, "eclipse4diac::utils::splitting::SPLIT_LWORD_INTO_BYTES"_STRID, TypeHash)

  FORTE_SPLIT_LWORD_INTO_BYTES::FORTE_SPLIT_LWORD_INTO_BYTES(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_IN(0_LWORD),
      var_BYTE_00(0_BYTE),
      var_BYTE_01(0_BYTE),
      var_BYTE_02(0_BYTE),
      var_BYTE_03(0_BYTE),
      var_BYTE_04(0_BYTE),
      var_BYTE_05(0_BYTE),
      var_BYTE_06(0_BYTE),
      var_BYTE_07(0_BYTE),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_BYTE_00(*this, 0, var_BYTE_00),
      conn_BYTE_01(*this, 1, var_BYTE_01),
      conn_BYTE_02(*this, 2, var_BYTE_02),
      conn_BYTE_03(*this, 3, var_BYTE_03),
      conn_BYTE_04(*this, 4, var_BYTE_04),
      conn_BYTE_05(*this, 5, var_BYTE_05),
      conn_BYTE_06(*this, 6, var_BYTE_06),
      conn_BYTE_07(*this, 7, var_BYTE_07) {
  }

  void FORTE_SPLIT_LWORD_INTO_BYTES::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_IN = 0_LWORD;
    var_BYTE_00 = 0_BYTE;
    var_BYTE_01 = 0_BYTE;
    var_BYTE_02 = 0_BYTE;
    var_BYTE_03 = 0_BYTE;
    var_BYTE_04 = 0_BYTE;
    var_BYTE_05 = 0_BYTE;
    var_BYTE_06 = 0_BYTE;
    var_BYTE_07 = 0_BYTE;
  }

  void FORTE_SPLIT_LWORD_INTO_BYTES::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventREQID: {
        readData(0, var_IN, conn_IN);
        break;
      }
      default:
        break;
    }
  }

  void FORTE_SPLIT_LWORD_INTO_BYTES::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventCNFID: {
        writeData(1, var_BYTE_00, conn_BYTE_00);
        writeData(2, var_BYTE_01, conn_BYTE_01);
        writeData(3, var_BYTE_02, conn_BYTE_02);
        writeData(4, var_BYTE_03, conn_BYTE_03);
        writeData(5, var_BYTE_04, conn_BYTE_04);
        writeData(6, var_BYTE_05, conn_BYTE_05);
        writeData(7, var_BYTE_06, conn_BYTE_06);
        writeData(8, var_BYTE_07, conn_BYTE_07);
        break;
      }
      default:
        break;
    }
  }

  CIEC_ANY *FORTE_SPLIT_LWORD_INTO_BYTES::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_IN;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_SPLIT_LWORD_INTO_BYTES::getDO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_BYTE_00;
      case 1: return &var_BYTE_01;
      case 2: return &var_BYTE_02;
      case 3: return &var_BYTE_03;
      case 4: return &var_BYTE_04;
      case 5: return &var_BYTE_05;
      case 6: return &var_BYTE_06;
      case 7: return &var_BYTE_07;
    }
    return nullptr;
  }

  CEventConnection *FORTE_SPLIT_LWORD_INTO_BYTES::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_SPLIT_LWORD_INTO_BYTES::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_IN;
    }
    return nullptr;
  }

  CDataConnection *FORTE_SPLIT_LWORD_INTO_BYTES::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_BYTE_00;
      case 1: return &conn_BYTE_01;
      case 2: return &conn_BYTE_02;
      case 3: return &conn_BYTE_03;
      case 4: return &conn_BYTE_04;
      case 5: return &conn_BYTE_05;
      case 6: return &conn_BYTE_06;
      case 7: return &conn_BYTE_07;
    }
    return nullptr;
  }

  void FORTE_SPLIT_LWORD_INTO_BYTES::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    func_SPLIT_LWORD_INTO_BYTES(var_IN, var_BYTE_00, var_BYTE_01, var_BYTE_02, var_BYTE_03, var_BYTE_04, var_BYTE_05, var_BYTE_06, var_BYTE_07);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void func_SPLIT_LWORD_INTO_BYTES(const CIEC_LWORD &st_lv_IN, CAnyBitOutputParameter<CIEC_BYTE> st_lv_BYTE_00, CAnyBitOutputParameter<CIEC_BYTE> st_lv_BYTE_01, CAnyBitOutputParameter<CIEC_BYTE> st_lv_BYTE_02, CAnyBitOutputParameter<CIEC_BYTE> st_lv_BYTE_03, CAnyBitOutputParameter<CIEC_BYTE> st_lv_BYTE_04, CAnyBitOutputParameter<CIEC_BYTE> st_lv_BYTE_05, CAnyBitOutputParameter<CIEC_BYTE> st_lv_BYTE_06, CAnyBitOutputParameter<CIEC_BYTE> st_lv_BYTE_07) {
    COutputGuard st_guard_BYTE_00(st_lv_BYTE_00);
    COutputGuard st_guard_BYTE_01(st_lv_BYTE_01);
    COutputGuard st_guard_BYTE_02(st_lv_BYTE_02);
    COutputGuard st_guard_BYTE_03(st_lv_BYTE_03);
    COutputGuard st_guard_BYTE_04(st_lv_BYTE_04);
    COutputGuard st_guard_BYTE_05(st_lv_BYTE_05);
    COutputGuard st_guard_BYTE_06(st_lv_BYTE_06);
    COutputGuard st_guard_BYTE_07(st_lv_BYTE_07);
    (*st_lv_BYTE_00) = 0_BYTE;
    (*st_lv_BYTE_01) = 0_BYTE;
    (*st_lv_BYTE_02) = 0_BYTE;
    (*st_lv_BYTE_03) = 0_BYTE;
    (*st_lv_BYTE_04) = 0_BYTE;
    (*st_lv_BYTE_05) = 0_BYTE;
    (*st_lv_BYTE_06) = 0_BYTE;
    (*st_lv_BYTE_07) = 0_BYTE;

    #line 19 "SPLIT_LWORD_INTO_BYTES.fct"
    (*st_lv_BYTE_00) = st_lv_IN.cpartial<CIEC_BYTE>(0);
    #line 20 "SPLIT_LWORD_INTO_BYTES.fct"
    (*st_lv_BYTE_01) = st_lv_IN.cpartial<CIEC_BYTE>(1);
    #line 21 "SPLIT_LWORD_INTO_BYTES.fct"
    (*st_lv_BYTE_02) = st_lv_IN.cpartial<CIEC_BYTE>(2);
    #line 22 "SPLIT_LWORD_INTO_BYTES.fct"
    (*st_lv_BYTE_03) = st_lv_IN.cpartial<CIEC_BYTE>(3);
    #line 23 "SPLIT_LWORD_INTO_BYTES.fct"
    (*st_lv_BYTE_04) = st_lv_IN.cpartial<CIEC_BYTE>(4);
    #line 24 "SPLIT_LWORD_INTO_BYTES.fct"
    (*st_lv_BYTE_05) = st_lv_IN.cpartial<CIEC_BYTE>(5);
    #line 25 "SPLIT_LWORD_INTO_BYTES.fct"
    (*st_lv_BYTE_06) = st_lv_IN.cpartial<CIEC_BYTE>(6);
    #line 26 "SPLIT_LWORD_INTO_BYTES.fct"
    (*st_lv_BYTE_07) = st_lv_IN.cpartial<CIEC_BYTE>(7);

  }

}