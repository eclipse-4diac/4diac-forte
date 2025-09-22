/*************************************************************************
 *** Copyright (c) 2012, 2023 TU Wien ACIN, HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: FIELDBUS_PERCENT_TO_WORD
 *** Description: Convert a WORD Value Range 0-FAFF to a REAL in the Range 0.0 to 100.0
 *** Version:
 ***     1.0: 2023-10-21/Franz Höpfinger - HR Agrartechnik GmbH -
 ***     1.1: 2024-09-19/Franz Höpfinger - HR Agrartechnik GmbH - Update to a more Function like Interface
 *************************************************************************/

#include "forte/eclipse4diac/signalprocessing/FIELDBUS_PERCENT_TO_WORD_fct.h"

#include "forte/datatypes/forte_real.h"
#include "forte/datatypes/forte_udint.h"
#include "forte/datatypes/forte_word.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"
#include "forte/eclipse4diac/signalprocessing/FIELDBUS_PERCENT_TO_WORD_fct.h"
#include "forte/eclipse4diac/signalprocessing/FIELDBUS_SIGNAL_gcf.h"

using namespace forte::literals;

namespace forte::eclipse4diac::signalprocessing {
  namespace {
    const auto cDataInputNames = std::array{"RI"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_FIELDBUS_PERCENT_TO_WORD, "eclipse4diac::signalprocessing::FIELDBUS_PERCENT_TO_WORD"_STRID)
  FORTE_FIELDBUS_PERCENT_TO_WORD::FORTE_FIELDBUS_PERCENT_TO_WORD(const forte::StringId paInstanceNameId,
                                                                 CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      conn_CNF(*this, 0),
      conn_RI(nullptr),
      conn_(*this, 0, var_) {
  }

  void FORTE_FIELDBUS_PERCENT_TO_WORD::setInitialValues() {
    var_RI = 0_REAL;
    var_ = 0_WORD;
  }

  void FORTE_FIELDBUS_PERCENT_TO_WORD::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_RI, conn_RI);
        break;
      }
      default: break;
    }
  }

  void FORTE_FIELDBUS_PERCENT_TO_WORD::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_, conn_);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_FIELDBUS_PERCENT_TO_WORD::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_RI;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_FIELDBUS_PERCENT_TO_WORD::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_;
    }
    return nullptr;
  }

  CEventConnection *FORTE_FIELDBUS_PERCENT_TO_WORD::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_FIELDBUS_PERCENT_TO_WORD::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_RI;
    }
    return nullptr;
  }

  CDataConnection *FORTE_FIELDBUS_PERCENT_TO_WORD::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_;
    }
    return nullptr;
  }

  void FORTE_FIELDBUS_PERCENT_TO_WORD::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
    var_ = func_FIELDBUS_PERCENT_TO_WORD(var_RI);
    sendOutputEvent(scmEventCNFID, paECET);
  }

  CIEC_WORD func_FIELDBUS_PERCENT_TO_WORD(CIEC_REAL st_lv_RI) {
    CIEC_WORD st_ret_val = 0_WORD;

#line 9 "FIELDBUS_PERCENT_TO_WORD.fct"
    st_ret_val = func_UDINT_TO_WORD(func_REAL_TO_UDINT(func_MUL<CIEC_REAL>(
        st_lv_RI, func_UDINT_TO_REAL(func_WORD_TO_UDINT(FORTE_FIELDBUS_SIGNAL::var_VALID_SIGNAL_W)))));

    return st_ret_val;
  }
} // namespace forte::eclipse4diac::signalprocessing
