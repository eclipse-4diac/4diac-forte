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
 *** Name: ASSEMBLE_WORD_FROM_BYTES
 *** Description: this Function combines the 2 BYTES to a WORD
 *** Version:
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 ***     1.1: 2024-07-28/Moritz Ortmeier - HR Agrartechnik - rename Function and change Input/Output Variables
 *************************************************************************/

#include "forte/eclipse4diac/utils/assembling/ASSEMBLE_WORD_FROM_BYTES_fct.h"

using namespace forte::literals;

#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_word.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"
#include "forte/eclipse4diac/utils/assembling/ASSEMBLE_WORD_FROM_BYTES_fct.h"

namespace forte::eclipse4diac::utils::assembling {
  namespace {
    const auto cDataInputNames = std::array{"BYTE_00"_STRID, "BYTE_01"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_ASSEMBLE_WORD_FROM_BYTES, "eclipse4diac::utils::assembling::ASSEMBLE_WORD_FROM_BYTES"_STRID)

  FORTE_ASSEMBLE_WORD_FROM_BYTES::FORTE_ASSEMBLE_WORD_FROM_BYTES(const forte::StringId paInstanceNameId,
                                                                 CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      conn_CNF(*this, 0),
      conn_BYTE_00(nullptr),
      conn_BYTE_01(nullptr),
      conn_(*this, 0, var_) {
  }

  void FORTE_ASSEMBLE_WORD_FROM_BYTES::setInitialValues() {
    var_BYTE_00 = 0_BYTE;
    var_BYTE_01 = 0_BYTE;
    var_ = 0_WORD;
  }

  void FORTE_ASSEMBLE_WORD_FROM_BYTES::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_BYTE_00, conn_BYTE_00);
        readData(1, var_BYTE_01, conn_BYTE_01);
        break;
      }
      default: break;
    }
  }

  void FORTE_ASSEMBLE_WORD_FROM_BYTES::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_, conn_);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_ASSEMBLE_WORD_FROM_BYTES::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_BYTE_00;
      case 1: return &var_BYTE_01;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_ASSEMBLE_WORD_FROM_BYTES::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_;
    }
    return nullptr;
  }

  CEventConnection *FORTE_ASSEMBLE_WORD_FROM_BYTES::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_ASSEMBLE_WORD_FROM_BYTES::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_BYTE_00;
      case 1: return &conn_BYTE_01;
    }
    return nullptr;
  }

  CDataConnection *FORTE_ASSEMBLE_WORD_FROM_BYTES::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_;
    }
    return nullptr;
  }

  void FORTE_ASSEMBLE_WORD_FROM_BYTES::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    var_ = func_ASSEMBLE_WORD_FROM_BYTES(var_BYTE_00, var_BYTE_01);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  CIEC_WORD func_ASSEMBLE_WORD_FROM_BYTES(CIEC_BYTE st_lv_BYTE_00, CIEC_BYTE st_lv_BYTE_01) {
    CIEC_WORD st_ret_val = 0_WORD;

#line 9 "ASSEMBLE_WORD_FROM_BYTES.fct"
    st_ret_val.partial<CIEC_BYTE>(0) = st_lv_BYTE_00;
#line 10 "ASSEMBLE_WORD_FROM_BYTES.fct"
    st_ret_val.partial<CIEC_BYTE>(1) = st_lv_BYTE_01;

    return st_ret_val;
  }
} // namespace forte::eclipse4diac::utils::assembling
