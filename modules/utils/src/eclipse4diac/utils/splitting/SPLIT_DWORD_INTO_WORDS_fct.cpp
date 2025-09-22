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
 *** Name: SPLIT_DWORD_INTO_WORDS
 *** Description: this Function extracts the 2 WORD from a dword
 *** Version:
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 ***     1.1: 2024-07-24/Moritz Ortmeier - HR Agrartechnik - rename Function and change Output Variables
 *************************************************************************/

#include "forte/eclipse4diac/utils/splitting/SPLIT_DWORD_INTO_WORDS_fct.h"

using namespace forte::literals;

#include "forte/datatypes/forte_dword.h"
#include "forte/datatypes/forte_word.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::utils::splitting {
  namespace {
    const auto cDataInputNames = std::array{"IN"_STRID};
    const auto cDataOutputNames = std::array{"WORD_00"_STRID, "WORD_01"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_SPLIT_DWORD_INTO_WORDS, "eclipse4diac::utils::splitting::SPLIT_DWORD_INTO_WORDS"_STRID)

  FORTE_SPLIT_DWORD_INTO_WORDS::FORTE_SPLIT_DWORD_INTO_WORDS(const forte::StringId paInstanceNameId,
                                                             CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_WORD_00(*this, 0, var_WORD_00),
      conn_WORD_01(*this, 1, var_WORD_01) {
  }

  void FORTE_SPLIT_DWORD_INTO_WORDS::setInitialValues() {
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
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_WORD_00, conn_WORD_00);
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_WORD_01, conn_WORD_01);
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
    func_SPLIT_DWORD_INTO_WORDS(var_IN, var_WORD_00, var_WORD_01);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void func_SPLIT_DWORD_INTO_WORDS(CIEC_DWORD st_lv_IN, CIEC_WORD &st_lv_WORD_00, CIEC_WORD &st_lv_WORD_01) {
    st_lv_WORD_00 = 0_WORD;
    st_lv_WORD_01 = 0_WORD;

#line 13 "SPLIT_DWORD_INTO_WORDS.fct"
    st_lv_WORD_00 = st_lv_IN.cpartial<CIEC_WORD>(0);
#line 14 "SPLIT_DWORD_INTO_WORDS.fct"
    st_lv_WORD_01 = st_lv_IN.cpartial<CIEC_BOOL>(1);
  }
} // namespace forte::eclipse4diac::utils::splitting
