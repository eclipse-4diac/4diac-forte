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
 *** Name: ASSEMBLE_DWORD_FROM_WORDS
 *** Description: this Function combines the 2 WORDS to a dword
 *** Version:
 ***     3.0.1: 2026-09-29/Martin Jobst -  - fix ST package declaration
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.1: 2024-07-28/Moritz Ortmeier - HR Agrartechnik - rename Function and change Input/Output Variables
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 *************************************************************************/

#include "forte/eclipse4diac/utils/assembling/ASSEMBLE_DWORD_FROM_WORDS_fct.h"

#include "forte/datatypes/forte_dword.h"
#include "forte/datatypes/forte_word.h"
#include "forte/eclipse4diac/utils/assembling/ASSEMBLE_DWORD_FROM_WORDS_fct.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::utils::assembling {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"WORD_00"_STRID, "WORD_01"_STRID};
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

    CIEC_DWORD func_ASSEMBLE_DWORD_FROM_WORDS__impl(const CIEC_WORD &st_lv_WORD_00, const CIEC_WORD &st_lv_WORD_01);
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_ASSEMBLE_DWORD_FROM_WORDS,
                     "eclipse4diac::utils::assembling::ASSEMBLE_DWORD_FROM_WORDS"_STRID,
                     TypeHash)

  FORTE_ASSEMBLE_DWORD_FROM_WORDS::FORTE_ASSEMBLE_DWORD_FROM_WORDS(const StringId paInstanceNameId,
                                                                   CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_WORD_00(0_WORD),
      var_WORD_01(0_WORD),
      var_(0_DWORD),
      conn_CNF(*this, 0),
      conn_WORD_00(nullptr),
      conn_WORD_01(nullptr),
      conn_(*this, 0, var_) {
  }

  void FORTE_ASSEMBLE_DWORD_FROM_WORDS::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_WORD_00 = 0_WORD;
    var_WORD_01 = 0_WORD;
    var_ = 0_DWORD;
  }

  void FORTE_ASSEMBLE_DWORD_FROM_WORDS::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_WORD_00, conn_WORD_00);
        readData(1, var_WORD_01, conn_WORD_01);
        break;
      }
      default: break;
    }
  }

  void FORTE_ASSEMBLE_DWORD_FROM_WORDS::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(2, var_, conn_);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_ASSEMBLE_DWORD_FROM_WORDS::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_WORD_00;
      case 1: return &var_WORD_01;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_ASSEMBLE_DWORD_FROM_WORDS::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_;
    }
    return nullptr;
  }

  CEventConnection *FORTE_ASSEMBLE_DWORD_FROM_WORDS::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_ASSEMBLE_DWORD_FROM_WORDS::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_WORD_00;
      case 1: return &conn_WORD_01;
    }
    return nullptr;
  }

  CDataConnection *FORTE_ASSEMBLE_DWORD_FROM_WORDS::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_;
    }
    return nullptr;
  }

  void FORTE_ASSEMBLE_DWORD_FROM_WORDS::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    var_ = func_ASSEMBLE_DWORD_FROM_WORDS__impl(var_WORD_00, var_WORD_01);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  CIEC_DWORD func_ASSEMBLE_DWORD_FROM_WORDS(const CIEC_WORD &st_lv_WORD_00, const CIEC_WORD &st_lv_WORD_01) {
    return func_ASSEMBLE_DWORD_FROM_WORDS__impl(st_lv_WORD_00, st_lv_WORD_01);
  }

  namespace {
    CIEC_DWORD func_ASSEMBLE_DWORD_FROM_WORDS__impl(const CIEC_WORD &st_lv_WORD_00, const CIEC_WORD &st_lv_WORD_01) {
      CIEC_DWORD st_ret_val = 0_DWORD;

#line 11 "ASSEMBLE_DWORD_FROM_WORDS.fct"
      st_ret_val.partial<CIEC_WORD>(0) = st_lv_WORD_00;
#line 12 "ASSEMBLE_DWORD_FROM_WORDS.fct"
      st_ret_val.partial<CIEC_WORD>(1) = st_lv_WORD_01;

      return st_ret_val;
    }
  } // namespace
} // namespace forte::eclipse4diac::utils::assembling
