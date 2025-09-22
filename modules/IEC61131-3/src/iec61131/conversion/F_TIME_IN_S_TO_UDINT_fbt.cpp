/*************************************************************************
 *** Copyright (c) 2023 Martin Jobst, HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: F_TIME_IN_S_TO_UDINT
 *** Description: Convert TIME in S to UDINT
 *** Version:
 ***     1.0: 2023-04-28/Martin Jobst -  -
 ***     1.1: 2023-08-21/Franz Höpfinger - HR Agrartechnik GmbH -
 *************************************************************************/

#include "forte/iec61131/conversion/F_TIME_IN_S_TO_UDINT_fbt.h"

using namespace forte::literals;

#include "forte/datatypes/forte_udint.h"
#include "forte/datatypes/forte_ulint.h"
#include "forte/datatypes/forte_any_duration_variant.h"
#include "forte/datatypes/forte_time.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61131::conversion {
  namespace {
    const auto cDataInputNames = std::array{"IN"_STRID};
    const auto cDataOutputNames = std::array{"OUT"_STRID};
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventInputTypeIds = std::array{"Event"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID};
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
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_F_TIME_IN_S_TO_UDINT, "iec61131::conversion::F_TIME_IN_S_TO_UDINT"_STRID)

  FORTE_F_TIME_IN_S_TO_UDINT::FORTE_F_TIME_IN_S_TO_UDINT(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CSimpleFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_OUT(*this, 0, var_OUT) {
  }

  void FORTE_F_TIME_IN_S_TO_UDINT::setInitialValues() {
    var_IN = 0_TIME;
    var_OUT = 0_UDINT;
  }

  void FORTE_F_TIME_IN_S_TO_UDINT::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID: alg_REQ(); break;
      default: break;
    }
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_F_TIME_IN_S_TO_UDINT::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_IN, conn_IN);
        break;
      }
      default: break;
    }
  }

  void FORTE_F_TIME_IN_S_TO_UDINT::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_OUT, conn_OUT);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_F_TIME_IN_S_TO_UDINT::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_IN;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_F_TIME_IN_S_TO_UDINT::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_OUT;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_F_TIME_IN_S_TO_UDINT::getDIO(size_t) {
    return nullptr;
  }

  CEventConnection *FORTE_F_TIME_IN_S_TO_UDINT::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_F_TIME_IN_S_TO_UDINT::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_IN;
    }
    return nullptr;
  }

  CDataConnection *FORTE_F_TIME_IN_S_TO_UDINT::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_OUT;
    }
    return nullptr;
  }

  CInOutDataConnection **FORTE_F_TIME_IN_S_TO_UDINT::getDIOInConUnchecked(TPortId) {
    return nullptr;
  }

  CInOutDataConnection *FORTE_F_TIME_IN_S_TO_UDINT::getDIOOutConUnchecked(TPortId) {
    return nullptr;
  }

  CIEC_ANY *FORTE_F_TIME_IN_S_TO_UDINT::getVarInternal(size_t) {
    return nullptr;
  }

  void FORTE_F_TIME_IN_S_TO_UDINT::alg_REQ(void) {

    var_OUT = func_TIME_IN_S_TO_UDINT(var_IN);
  }

} // namespace forte::iec61131::conversion
