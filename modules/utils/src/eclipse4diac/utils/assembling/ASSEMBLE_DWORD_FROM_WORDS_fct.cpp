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
 *** Name: ASSEMBLE_DWORD_FROM_WORDS
 *** Description: this Function combines the 2 WORDS to a dword
 *** Version:
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 ***     1.1: 2024-07-28/Moritz Ortmeier - HR Agrartechnik - rename Function and change Input/Output Variables
 *************************************************************************/

#include "forte/eclipse4diac/utils/assembling/ASSEMBLE_DWORD_FROM_WORDS_fct.h"

using namespace forte::literals;

#include "forte/datatypes/forte_dword.h"
#include "forte/datatypes/forte_word.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"
#include "forte/eclipse4diac/utils/assembling/ASSEMBLE_DWORD_FROM_WORDS_fct.h"

namespace forte::eclipse4diac::utils::assembling {
  namespace {
    const auto cDataInputNames = std::array{"WORD_00"_STRID, "WORD_01"_STRID};
    const auto cDataOutputNames = std::array{""_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_ASSEMBLE_DWORD_FROM_WORDS,
                     "eclipse4diac::utils::assembling::ASSEMBLE_DWORD_FROM_WORDS"_STRID)

  FORTE_ASSEMBLE_DWORD_FROM_WORDS::FORTE_ASSEMBLE_DWORD_FROM_WORDS(const forte::StringId paInstanceNameId,
                                                                   CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      conn_CNF(*this, 0),
      conn_WORD_00(nullptr),
      conn_WORD_01(nullptr),
      conn_(*this, 0, var_) {
  }

  void FORTE_ASSEMBLE_DWORD_FROM_WORDS::setInitialValues() {
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
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_, conn_);
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
    var_ = func_ASSEMBLE_DWORD_FROM_WORDS(var_WORD_00, var_WORD_01);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  CIEC_DWORD func_ASSEMBLE_DWORD_FROM_WORDS(CIEC_WORD st_lv_WORD_00, CIEC_WORD st_lv_WORD_01) {
    CIEC_DWORD st_ret_val = 0_DWORD;

#line 9 "ASSEMBLE_DWORD_FROM_WORDS.fct"
    st_ret_val.partial<CIEC_WORD>(0) = st_lv_WORD_00;
#line 10 "ASSEMBLE_DWORD_FROM_WORDS.fct"
    st_ret_val.partial<CIEC_WORD>(1) = st_lv_WORD_01;

    return st_ret_val;
  }
} // namespace forte::eclipse4diac::utils::assembling
