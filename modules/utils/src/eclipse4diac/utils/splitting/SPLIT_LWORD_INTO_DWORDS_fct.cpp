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
 *** Name: SPLIT_LWORD_INTO_DWORDS
 *** Description: this Function extracts the 2 DWORDS from a lword
 *** Version:
 ***     1.0: 2026-05-16/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#include "forte/eclipse4diac/utils/splitting/SPLIT_LWORD_INTO_DWORDS_fct.h"

#include "forte/datatypes/forte_dword.h"
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
    const auto cDataOutputNames = std::array{"DWORD_00"_STRID, "DWORD_01"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_SPLIT_LWORD_INTO_DWORDS, "eclipse4diac::utils::splitting::SPLIT_LWORD_INTO_DWORDS"_STRID, TypeHash)

  FORTE_SPLIT_LWORD_INTO_DWORDS::FORTE_SPLIT_LWORD_INTO_DWORDS(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_IN(0_LWORD),
      var_DWORD_00(0_DWORD),
      var_DWORD_01(0_DWORD),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_DWORD_00(*this, 0, var_DWORD_00),
      conn_DWORD_01(*this, 1, var_DWORD_01) {
  }

  void FORTE_SPLIT_LWORD_INTO_DWORDS::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_IN = 0_LWORD;
    var_DWORD_00 = 0_DWORD;
    var_DWORD_01 = 0_DWORD;
  }

  void FORTE_SPLIT_LWORD_INTO_DWORDS::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventREQID: {
        readData(0, var_IN, conn_IN);
        break;
      }
      default:
        break;
    }
  }

  void FORTE_SPLIT_LWORD_INTO_DWORDS::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventCNFID: {
        writeData(1, var_DWORD_00, conn_DWORD_00);
        writeData(2, var_DWORD_01, conn_DWORD_01);
        break;
      }
      default:
        break;
    }
  }

  CIEC_ANY *FORTE_SPLIT_LWORD_INTO_DWORDS::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_IN;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_SPLIT_LWORD_INTO_DWORDS::getDO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_DWORD_00;
      case 1: return &var_DWORD_01;
    }
    return nullptr;
  }

  CEventConnection *FORTE_SPLIT_LWORD_INTO_DWORDS::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_SPLIT_LWORD_INTO_DWORDS::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_IN;
    }
    return nullptr;
  }

  CDataConnection *FORTE_SPLIT_LWORD_INTO_DWORDS::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_DWORD_00;
      case 1: return &conn_DWORD_01;
    }
    return nullptr;
  }

  void FORTE_SPLIT_LWORD_INTO_DWORDS::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    func_SPLIT_LWORD_INTO_DWORDS(var_IN, var_DWORD_00, var_DWORD_01);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void func_SPLIT_LWORD_INTO_DWORDS(const CIEC_LWORD &st_lv_IN, CAnyBitOutputParameter<CIEC_DWORD> st_lv_DWORD_00, CAnyBitOutputParameter<CIEC_DWORD> st_lv_DWORD_01) {
    COutputGuard st_guard_DWORD_00(st_lv_DWORD_00);
    COutputGuard st_guard_DWORD_01(st_lv_DWORD_01);
    (*st_lv_DWORD_00) = 0_DWORD;
    (*st_lv_DWORD_01) = 0_DWORD;

    #line 13 "SPLIT_LWORD_INTO_DWORDS.fct"
    (*st_lv_DWORD_00) = st_lv_IN.cpartial<CIEC_DWORD>(0);
    #line 14 "SPLIT_LWORD_INTO_DWORDS.fct"
    (*st_lv_DWORD_01) = st_lv_IN.cpartial<CIEC_DWORD>(1);

  }

}