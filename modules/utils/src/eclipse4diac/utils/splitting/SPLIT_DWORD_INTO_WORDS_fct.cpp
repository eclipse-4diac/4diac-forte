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
 *** Name: SPLIT_DWORD_INTO_WORDS
 *** Description: this Function extracts the 2 WORD from a dword
 *** Version:
 ***     3.0.1: 2026-09-29/Martin Jobst -  - fix ST package declaration
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.1: 2024-07-24/Moritz Ortmeier - HR Agrartechnik - rename Function and change Output Variables
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#include "forte/eclipse4diac/utils/splitting/SPLIT_DWORD_INTO_WORDS_fct.h"

#include "forte/datatypes/forte_dword.h"
#include "forte/datatypes/forte_word.h"
#include "forte/forte_st_util.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::utils::splitting {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"IN"_STRID};
    const auto cDataOutputNames = std::array{"WORD_00"_STRID, "WORD_01"_STRID};
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

    void
    func_SPLIT_DWORD_INTO_WORDS__impl(const CIEC_DWORD &st_lv_IN, CIEC_WORD &st_lv_WORD_00, CIEC_WORD &st_lv_WORD_01);
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_SPLIT_DWORD_INTO_WORDS,
                     "eclipse4diac::utils::splitting::SPLIT_DWORD_INTO_WORDS"_STRID,
                     TypeHash)

  FORTE_SPLIT_DWORD_INTO_WORDS::FORTE_SPLIT_DWORD_INTO_WORDS(const StringId paInstanceNameId,
                                                             CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_IN(0_DWORD),
      var_WORD_00(0_WORD),
      var_WORD_01(0_WORD),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_WORD_00(*this, 0, var_WORD_00),
      conn_WORD_01(*this, 1, var_WORD_01) {
  }

  void FORTE_SPLIT_DWORD_INTO_WORDS::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_IN = 0_DWORD;
    var_WORD_00 = 0_WORD;
    var_WORD_01 = 0_WORD;
  }

  void FORTE_SPLIT_DWORD_INTO_WORDS::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_IN, conn_IN);
        break;
      }
      default: break;
    }
  }

  void FORTE_SPLIT_DWORD_INTO_WORDS::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(1, var_WORD_00, conn_WORD_00);
        writeData(2, var_WORD_01, conn_WORD_01);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_SPLIT_DWORD_INTO_WORDS::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_IN;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_SPLIT_DWORD_INTO_WORDS::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_WORD_00;
      case 1: return &var_WORD_01;
    }
    return nullptr;
  }

  CEventConnection *FORTE_SPLIT_DWORD_INTO_WORDS::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_SPLIT_DWORD_INTO_WORDS::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_IN;
    }
    return nullptr;
  }

  CDataConnection *FORTE_SPLIT_DWORD_INTO_WORDS::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_WORD_00;
      case 1: return &conn_WORD_01;
    }
    return nullptr;
  }

  void FORTE_SPLIT_DWORD_INTO_WORDS::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    func_SPLIT_DWORD_INTO_WORDS__impl(var_IN, var_WORD_00, var_WORD_01);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void func_SPLIT_DWORD_INTO_WORDS(const CIEC_DWORD &st_lv_IN,
                                   CAnyBitOutputParameter<CIEC_WORD> st_lv_WORD_00,
                                   CAnyBitOutputParameter<CIEC_WORD> st_lv_WORD_01) {
    COutputGuard st_guard_WORD_00(st_lv_WORD_00);
    COutputGuard st_guard_WORD_01(st_lv_WORD_01);
    func_SPLIT_DWORD_INTO_WORDS__impl(st_lv_IN, *st_lv_WORD_00, *st_lv_WORD_01);
  }

  namespace {
    void
    func_SPLIT_DWORD_INTO_WORDS__impl(const CIEC_DWORD &st_lv_IN, CIEC_WORD &st_lv_WORD_00, CIEC_WORD &st_lv_WORD_01) {
      st_lv_WORD_00 = 0_WORD;
      st_lv_WORD_01 = 0_WORD;

#line 15 "SPLIT_DWORD_INTO_WORDS.fct"
      st_lv_WORD_00 = st_lv_IN.cpartial<CIEC_WORD>(0);
#line 16 "SPLIT_DWORD_INTO_WORDS.fct"
      st_lv_WORD_01 = st_lv_IN.cpartial<CIEC_BOOL>(1);
    }
  } // namespace
} // namespace forte::eclipse4diac::utils::splitting
