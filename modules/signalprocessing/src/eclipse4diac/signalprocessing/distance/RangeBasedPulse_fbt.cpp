/*************************************************************************
 *** Copyright (c) 2024 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: RangeBasedPulse
 *** Description: Distance based Impulse Generator
 *** Version:
 ***     1.0: 2024-10-18/Franz Höpfinger - HR Agrartechnik GmbH - initial Version
 ***     1.1: 2024-10-18/Franz Höpfinger - HR Agrartechnik GmbH - added Offset
 *************************************************************************/

#include "forte/eclipse4diac/signalprocessing/distance/RangeBasedPulse_fbt.h"

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_udint.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

using namespace forte::literals;

namespace forte::eclipse4diac::signalprocessing::distance {
  namespace {
    const auto cDataInputNames = std::array{"DIST_IN"_STRID, "DIST_OFF"_STRID, "DIST_HIGH"_STRID, "DIST_LOW"_STRID};
    const auto cDataOutputNames = std::array{"Q"_STRID};
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

    const auto cInternalsNames = std::array{"DIST_REMAINDER"_STRID};
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_RangeBasedPulse, "eclipse4diac::signalprocessing::distance::RangeBasedPulse"_STRID)

  FORTE_RangeBasedPulse::FORTE_RangeBasedPulse(const forte::StringId paInstanceNameId,
                                               forte::CFBContainer &paContainer) :
      CSimpleFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cInternalsNames),
      conn_CNF(*this, 0),
      conn_DIST_IN(nullptr),
      conn_DIST_OFF(nullptr),
      conn_DIST_HIGH(nullptr),
      conn_DIST_LOW(nullptr),
      conn_Q(*this, 0, var_Q) {
  }

  void FORTE_RangeBasedPulse::setInitialValues() {
    var_DIST_REMAINDER = 0_UDINT;
    var_DIST_IN = 0_UDINT;
    var_DIST_OFF = 0_UDINT;
    var_DIST_HIGH = 0_UDINT;
    var_DIST_LOW = 0_UDINT;
    var_Q = 0_BOOL;
  }

  void FORTE_RangeBasedPulse::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventREQID: enterStateREQ(paECET); break;
      default: break;
    }
  }

  void FORTE_RangeBasedPulse::enterStateREQ(CEventChainExecutionThread *const paECET) {
    alg_REQ();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_RangeBasedPulse::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventREQID: {
        readData(0, var_DIST_IN, conn_DIST_IN);
        readData(1, var_DIST_OFF, conn_DIST_OFF);
        readData(2, var_DIST_HIGH, conn_DIST_HIGH);
        readData(3, var_DIST_LOW, conn_DIST_LOW);
        break;
      }
      default: break;
    }
  }

  void FORTE_RangeBasedPulse::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_Q, conn_Q);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_RangeBasedPulse::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_DIST_IN;
      case 1: return &var_DIST_OFF;
      case 2: return &var_DIST_HIGH;
      case 3: return &var_DIST_LOW;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_RangeBasedPulse::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_Q;
    }
    return nullptr;
  }

  CEventConnection *FORTE_RangeBasedPulse::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_RangeBasedPulse::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_DIST_IN;
      case 1: return &conn_DIST_OFF;
      case 2: return &conn_DIST_HIGH;
      case 3: return &conn_DIST_LOW;
    }
    return nullptr;
  }

  CDataConnection *FORTE_RangeBasedPulse::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_Q;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_RangeBasedPulse::getVarInternal(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_DIST_REMAINDER;
    }
    return nullptr;
  }

  void FORTE_RangeBasedPulse::alg_REQ(void) {

#line 3 "RangeBasedPulse.fbt"
    var_DIST_REMAINDER = func_MOD<CIEC_UDINT>(func_ADD<CIEC_UDINT>(var_DIST_IN, var_DIST_OFF),
                                              func_ADD<CIEC_UDINT>(var_DIST_HIGH, var_DIST_LOW));
#line 5 "RangeBasedPulse.fbt"
    var_Q = func_GE(var_DIST_REMAINDER, var_DIST_HIGH);
  }
} // namespace forte::eclipse4diac::signalprocessing::distance
