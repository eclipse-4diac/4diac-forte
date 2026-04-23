/*************************************************************************
 *** Copyright (c) 2013 ACIN, fortiss GmbH, Martin Erich Jobst, HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.1.100.202604172003!
 ***
 *** Name: F_ATAN2
 *** Description: principal arc tan of Y/X
 *** Version:
 ***     1.0: 2026-02-01/Franz Höpfinger - HR Agrartechnik GmbH - copy from F_ATAN an make initial Version
 *************************************************************************/

#include "forte/iec61131/numerical/F_ATAN2_fbt.h"

#include "forte/datatypes/forte_any_real_variant.h"
#include "forte/forte_st_util.h"
#include "forte/iec61131_functions/func_ATAN2.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::iec61131::numerical {
  namespace {

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"Y"_STRID, "X"_STRID};
    const auto cDataOutputNames = std::array{"OUT"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_F_ATAN2, "iec61131::numerical::F_ATAN2"_STRID)

  FORTE_F_ATAN2::FORTE_F_ATAN2(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CSimpleFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      var_Y(CIEC_ANY_REAL_VARIANT()),
      var_X(CIEC_ANY_REAL_VARIANT()),
      var_OUT(CIEC_ANY_REAL_VARIANT()),
      conn_CNF(*this, 0),
      conn_Y(nullptr),
      conn_X(nullptr),
      conn_OUT(*this, 0, var_OUT) {
  }

  void FORTE_F_ATAN2::setInitialValues() {
    CSimpleFB::setInitialValues();
    var_Y = CIEC_ANY_REAL_VARIANT();
    var_X = CIEC_ANY_REAL_VARIANT();
    var_OUT = CIEC_ANY_REAL_VARIANT();
  }

  void FORTE_F_ATAN2::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID: enterStateREQ(paECET); break;
      default: break;
    }
  }

  void FORTE_F_ATAN2::enterStateREQ(CEventChainExecutionThread *const paECET) {
    alg_REQ();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_F_ATAN2::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_Y, conn_Y);
        readData(1, var_X, conn_X);
        break;
      }
      default: break;
    }
  }

  void FORTE_F_ATAN2::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(2, var_OUT, conn_OUT);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_F_ATAN2::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_Y;
      case 1: return &var_X;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_F_ATAN2::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_OUT;
    }
    return nullptr;
  }

  CEventConnection *FORTE_F_ATAN2::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_F_ATAN2::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_Y;
      case 1: return &conn_X;
    }
    return nullptr;
  }

  CDataConnection *FORTE_F_ATAN2::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_OUT;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_F_ATAN2::getVarInternal(size_t) {
    return nullptr;
  }

  void FORTE_F_ATAN2::alg_REQ(void) {

#line 2 "F_ATAN2.fbt"
    var_OUT = std::visit(
        [](auto &&x, auto &&y) -> CIEC_ANY_REAL_VARIANT { return CIEC_ANY_REAL_VARIANT(func_ATAN2(y, x)); },
        static_cast<CIEC_ANY_REAL_VARIANT::variant &>(var_X), static_cast<CIEC_ANY_REAL_VARIANT::variant &>(var_Y));
  }

} // namespace forte::iec61131::numerical
