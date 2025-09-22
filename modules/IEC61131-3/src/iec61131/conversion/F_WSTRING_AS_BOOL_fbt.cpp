/*******************************************************************************
 * Copyright (c) 2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Jobst
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "forte/iec61131/conversion/F_WSTRING_AS_BOOL_fbt.h"

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

using namespace forte::literals;

namespace forte::iec61131::conversion {
  namespace {
    const auto cDataInputNames = std::array{"IN"_STRID};
    const auto cDataOutputNames = std::array{"OUT"_STRID};
    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_F_WSTRING_AS_BOOL, "iec61131::conversion::F_WSTRING_AS_BOOL"_STRID)

  FORTE_F_WSTRING_AS_BOOL::FORTE_F_WSTRING_AS_BOOL(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CSimpleFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      var_IN(u""_WSTRING),
      var_OUT(false_BOOL),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_OUT(*this, 0, var_OUT) {
  }

  void FORTE_F_WSTRING_AS_BOOL::setInitialValues() {
    var_IN = u""_WSTRING;
    var_OUT = 0_BOOL;
  }

  void FORTE_F_WSTRING_AS_BOOL::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID: enterStateREQ(paECET); break;
      default: break;
    }
  }

  void FORTE_F_WSTRING_AS_BOOL::enterStateREQ(CEventChainExecutionThread *const paECET) {
    alg_REQ();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_F_WSTRING_AS_BOOL::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_IN, conn_IN);
        break;
      }
      default: break;
    }
  }

  void FORTE_F_WSTRING_AS_BOOL::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_OUT, conn_OUT);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_F_WSTRING_AS_BOOL::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_IN;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_F_WSTRING_AS_BOOL::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_OUT;
    }
    return nullptr;
  }

  CEventConnection *FORTE_F_WSTRING_AS_BOOL::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_F_WSTRING_AS_BOOL::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_IN;
    }
    return nullptr;
  }

  CDataConnection *FORTE_F_WSTRING_AS_BOOL::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_OUT;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_F_WSTRING_AS_BOOL::getVarInternal(size_t) {
    return nullptr;
  }

  void FORTE_F_WSTRING_AS_BOOL::alg_REQ(void) {

#line 2 "F_WSTRING_AS_BOOL.fbt"
    var_OUT = func_WSTRING_AS_BOOL(var_IN);
  }

} // namespace forte::iec61131::conversion
