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
 *** Name: ASSEMBLE_LWORD_FROM_DWORDS
 *** Description: this Function combines the 2 DWORDS to a LWORD
 *** Version:
 ***     1.0: 2026-05-16/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#include "forte/eclipse4diac/utils/assembling/ASSEMBLE_LWORD_FROM_DWORDS_fct.h"

#include "forte/datatypes/forte_dword.h"
#include "forte/datatypes/forte_lword.h"
#include "forte/eclipse4diac/utils/assembling/ASSEMBLE_LWORD_FROM_DWORDS_fct.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::utils::assembling {
  namespace {
    constexpr std::string_view TypeHash =""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"DWORD_00"_STRID, "DWORD_01"_STRID};
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
  }

  DEFINE_FIRMWARE_FB(FORTE_ASSEMBLE_LWORD_FROM_DWORDS, "eclipse4diac::utils::assembling::ASSEMBLE_LWORD_FROM_DWORDS"_STRID, TypeHash)

  FORTE_ASSEMBLE_LWORD_FROM_DWORDS::FORTE_ASSEMBLE_LWORD_FROM_DWORDS(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_DWORD_00(0_DWORD),
      var_DWORD_01(0_DWORD),
      var_(0_LWORD),
      conn_CNF(*this, 0),
      conn_DWORD_00(nullptr),
      conn_DWORD_01(nullptr),
      conn_(*this, 0, var_) {
  }

  void FORTE_ASSEMBLE_LWORD_FROM_DWORDS::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_DWORD_00 = 0_DWORD;
    var_DWORD_01 = 0_DWORD;
    var_ = 0_LWORD;
  }

  void FORTE_ASSEMBLE_LWORD_FROM_DWORDS::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventREQID: {
        readData(0, var_DWORD_00, conn_DWORD_00);
        readData(1, var_DWORD_01, conn_DWORD_01);
        break;
      }
      default:
        break;
    }
  }

  void FORTE_ASSEMBLE_LWORD_FROM_DWORDS::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventCNFID: {
        writeData(2, var_, conn_);
        break;
      }
      default:
        break;
    }
  }

  CIEC_ANY *FORTE_ASSEMBLE_LWORD_FROM_DWORDS::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_DWORD_00;
      case 1: return &var_DWORD_01;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_ASSEMBLE_LWORD_FROM_DWORDS::getDO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_;
    }
    return nullptr;
  }

  CEventConnection *FORTE_ASSEMBLE_LWORD_FROM_DWORDS::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_ASSEMBLE_LWORD_FROM_DWORDS::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_DWORD_00;
      case 1: return &conn_DWORD_01;
    }
    return nullptr;
  }

  CDataConnection *FORTE_ASSEMBLE_LWORD_FROM_DWORDS::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_;
    }
    return nullptr;
  }

  void FORTE_ASSEMBLE_LWORD_FROM_DWORDS::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    var_ = func_ASSEMBLE_LWORD_FROM_DWORDS(var_DWORD_00, var_DWORD_01);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  CIEC_LWORD func_ASSEMBLE_LWORD_FROM_DWORDS(const CIEC_DWORD &st_lv_DWORD_00, const CIEC_DWORD &st_lv_DWORD_01) {
    CIEC_LWORD st_ret_val = 0_LWORD;

    #line 9 "ASSEMBLE_LWORD_FROM_DWORDS.fct"
    st_ret_val.partial<CIEC_DWORD>(0) = st_lv_DWORD_00;
    #line 10 "ASSEMBLE_LWORD_FROM_DWORDS.fct"
    st_ret_val.partial<CIEC_DWORD>(1) = st_lv_DWORD_01;

    return st_ret_val;
  }

}