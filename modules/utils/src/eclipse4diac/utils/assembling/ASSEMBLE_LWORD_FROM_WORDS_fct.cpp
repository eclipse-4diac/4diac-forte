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
 *** Name: ASSEMBLE_LWORD_FROM_WORDS
 *** Description: this Function combines the 4 WORDS to a LWORD
 *** Version:
 ***     1.0: 2026-05-16/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#include "forte/eclipse4diac/utils/assembling/ASSEMBLE_LWORD_FROM_WORDS_fct.h"

#include "forte/datatypes/forte_lword.h"
#include "forte/datatypes/forte_word.h"
#include "forte/eclipse4diac/utils/assembling/ASSEMBLE_LWORD_FROM_WORDS_fct.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::utils::assembling {
  namespace {
    constexpr std::string_view TypeHash =""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"WORD_00"_STRID, "WORD_01"_STRID, "WORD_02"_STRID, "WORD_03"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_ASSEMBLE_LWORD_FROM_WORDS, "eclipse4diac::utils::assembling::ASSEMBLE_LWORD_FROM_WORDS"_STRID, TypeHash)

  FORTE_ASSEMBLE_LWORD_FROM_WORDS::FORTE_ASSEMBLE_LWORD_FROM_WORDS(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_WORD_00(0_WORD),
      var_WORD_01(0_WORD),
      var_WORD_02(0_WORD),
      var_WORD_03(0_WORD),
      var_(0_LWORD),
      conn_CNF(*this, 0),
      conn_WORD_00(nullptr),
      conn_WORD_01(nullptr),
      conn_WORD_02(nullptr),
      conn_WORD_03(nullptr),
      conn_(*this, 0, var_) {
  }

  void FORTE_ASSEMBLE_LWORD_FROM_WORDS::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_WORD_00 = 0_WORD;
    var_WORD_01 = 0_WORD;
    var_WORD_02 = 0_WORD;
    var_WORD_03 = 0_WORD;
    var_ = 0_LWORD;
  }

  void FORTE_ASSEMBLE_LWORD_FROM_WORDS::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventREQID: {
        readData(0, var_WORD_00, conn_WORD_00);
        readData(1, var_WORD_01, conn_WORD_01);
        readData(2, var_WORD_02, conn_WORD_02);
        readData(3, var_WORD_03, conn_WORD_03);
        break;
      }
      default:
        break;
    }
  }

  void FORTE_ASSEMBLE_LWORD_FROM_WORDS::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventCNFID: {
        writeData(4, var_, conn_);
        break;
      }
      default:
        break;
    }
  }

  CIEC_ANY *FORTE_ASSEMBLE_LWORD_FROM_WORDS::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_WORD_00;
      case 1: return &var_WORD_01;
      case 2: return &var_WORD_02;
      case 3: return &var_WORD_03;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_ASSEMBLE_LWORD_FROM_WORDS::getDO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_;
    }
    return nullptr;
  }

  CEventConnection *FORTE_ASSEMBLE_LWORD_FROM_WORDS::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_ASSEMBLE_LWORD_FROM_WORDS::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_WORD_00;
      case 1: return &conn_WORD_01;
      case 2: return &conn_WORD_02;
      case 3: return &conn_WORD_03;
    }
    return nullptr;
  }

  CDataConnection *FORTE_ASSEMBLE_LWORD_FROM_WORDS::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_;
    }
    return nullptr;
  }

  void FORTE_ASSEMBLE_LWORD_FROM_WORDS::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    var_ = func_ASSEMBLE_LWORD_FROM_WORDS(var_WORD_00, var_WORD_01, var_WORD_02, var_WORD_03);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  CIEC_LWORD func_ASSEMBLE_LWORD_FROM_WORDS(const CIEC_WORD &st_lv_WORD_00, const CIEC_WORD &st_lv_WORD_01, const CIEC_WORD &st_lv_WORD_02, const CIEC_WORD &st_lv_WORD_03) {
    CIEC_LWORD st_ret_val = 0_LWORD;

    #line 11 "ASSEMBLE_LWORD_FROM_WORDS.fct"
    st_ret_val.partial<CIEC_WORD>(0) = st_lv_WORD_00;
    #line 12 "ASSEMBLE_LWORD_FROM_WORDS.fct"
    st_ret_val.partial<CIEC_WORD>(1) = st_lv_WORD_01;
    #line 13 "ASSEMBLE_LWORD_FROM_WORDS.fct"
    st_ret_val.partial<CIEC_WORD>(2) = st_lv_WORD_02;
    #line 14 "ASSEMBLE_LWORD_FROM_WORDS.fct"
    st_ret_val.partial<CIEC_WORD>(3) = st_lv_WORD_03;

    return st_ret_val;
  }

}