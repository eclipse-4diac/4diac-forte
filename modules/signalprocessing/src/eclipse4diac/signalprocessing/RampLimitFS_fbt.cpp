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
 *** This file was generated using the 4DIAC FORTE Export Filter 3.2.100.202608212006!
 ***
 *** Name: RampLimitFS
 *** Description: Setpoint Ramp: Step up and down Values with Fast and Slow mode
 *** Version:
 ***     3.1: 2026-08-12/Franz Höpfinger - HR Agrartechnik GmbH - add init and correct WITH, add upper and lower limit
 ***                                       reached indicators
 ***     3.0: 2025-04-14/Patrick Aigner  - changed package
 ***     1.1: 2024-10-02/Franz Höpfinger - HR Agrartechnik GmbH - Rename to RampLimitFS
 ***     1.0: 2024-09-20/Franz Höpfinger - HR Agrartechnik GmbH -
 *************************************************************************/

#include "forte/eclipse4diac/signalprocessing/RampLimitFS_fbt.h"

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_dint.h"
#include "forte/forte_st_util.h"
#include "forte/iec61131_functions/func_ADD.h"
#include "forte/iec61131_functions/func_GE.h"
#include "forte/iec61131_functions/func_GT.h"
#include "forte/iec61131_functions/func_LE.h"
#include "forte/iec61131_functions/func_LT.h"
#include "forte/iec61131_functions/func_SUB.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::signalprocessing {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"INIT"_STRID,      "ZERO"_STRID,      "UP_SLOW"_STRID, "UP_FAST"_STRID,
                                             "DOWN_SLOW"_STRID, "DOWN_FAST"_STRID, "FULL"_STRID,    "LOAD"_STRID};
    const auto cEventInputTypeIds = std::array{"EInit"_STRID, "Event"_STRID, "Event"_STRID, "Event"_STRID,
                                               "Event"_STRID, "Event"_STRID, "Event"_STRID, "Event"_STRID};
    const auto cEventOutputNames = std::array{"INITO"_STRID, "CNF"_STRID};
    const auto cEventOutputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
    const auto cDataInputNames = std::array{"PV"_STRID, "VAL_ZERO"_STRID, "SLOW"_STRID, "FAST"_STRID, "VAL_FULL"_STRID};
    const auto cDataOutputNames = std::array{"OUT"_STRID, "qAtZero"_STRID, "qAtFull"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_RampLimitFS, "eclipse4diac::signalprocessing::RampLimitFS"_STRID, TypeHash)

  FORTE_RampLimitFS::FORTE_RampLimitFS(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CSimpleFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      var_PV(0_DINT),
      var_VAL_ZERO(0_DINT),
      var_SLOW(0_DINT),
      var_FAST(0_DINT),
      var_VAL_FULL(0_DINT),
      var_OUT(0_DINT),
      var_qAtZero(0_BOOL),
      var_qAtFull(0_BOOL),
      conn_INITO(*this, 0),
      conn_CNF(*this, 1),
      conn_PV(nullptr),
      conn_VAL_ZERO(nullptr),
      conn_SLOW(nullptr),
      conn_FAST(nullptr),
      conn_VAL_FULL(nullptr),
      conn_OUT(*this, 0, var_OUT),
      conn_qAtZero(*this, 1, var_qAtZero),
      conn_qAtFull(*this, 2, var_qAtFull) {
  }

  void FORTE_RampLimitFS::setInitialValues() {
    CSimpleFB::setInitialValues();
    var_PV = 0_DINT;
    var_VAL_ZERO = 0_DINT;
    var_SLOW = 0_DINT;
    var_FAST = 0_DINT;
    var_VAL_FULL = 0_DINT;
    var_OUT = 0_DINT;
    var_qAtZero = 0_BOOL;
    var_qAtFull = 0_BOOL;
  }

  void FORTE_RampLimitFS::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventINITID: enterStateINIT(paECET); break;
      case scmEventZEROID: enterStateZERO(paECET); break;
      case scmEventUP_SLOWID: enterStateUP_SLOW(paECET); break;
      case scmEventUP_FASTID: enterStateUP_FAST(paECET); break;
      case scmEventDOWN_SLOWID: enterStateDOWN_SLOW(paECET); break;
      case scmEventDOWN_FASTID: enterStateDOWN_FAST(paECET); break;
      case scmEventFULLID: enterStateFULL(paECET); break;
      case scmEventLOADID: enterStateLOAD(paECET); break;
      default: break;
    }
  }

  void FORTE_RampLimitFS::enterStateINIT(CEventChainExecutionThread *const paECET) {
    alg_INIT();
    sendOutputEvent(scmEventINITOID, paECET);
  }

  void FORTE_RampLimitFS::enterStateZERO(CEventChainExecutionThread *const paECET) {
    alg_ZERO();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_RampLimitFS::enterStateUP_SLOW(CEventChainExecutionThread *const paECET) {
    alg_UP_SLOW();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_RampLimitFS::enterStateUP_FAST(CEventChainExecutionThread *const paECET) {
    alg_UP_FAST();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_RampLimitFS::enterStateDOWN_SLOW(CEventChainExecutionThread *const paECET) {
    alg_DOWN_SLOW();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_RampLimitFS::enterStateDOWN_FAST(CEventChainExecutionThread *const paECET) {
    alg_DOWN_FAST();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_RampLimitFS::enterStateFULL(CEventChainExecutionThread *const paECET) {
    alg_FULL();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_RampLimitFS::enterStateLOAD(CEventChainExecutionThread *const paECET) {
    alg_LOAD();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_RampLimitFS::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITID: {
        readData(0, var_PV, conn_PV);
        readData(1, var_VAL_ZERO, conn_VAL_ZERO);
        readData(2, var_SLOW, conn_SLOW);
        readData(3, var_FAST, conn_FAST);
        readData(4, var_VAL_FULL, conn_VAL_FULL);
        break;
      }
      case scmEventZEROID: {
        readData(1, var_VAL_ZERO, conn_VAL_ZERO);
        readData(4, var_VAL_FULL, conn_VAL_FULL);
        break;
      }
      case scmEventUP_SLOWID: {
        readData(2, var_SLOW, conn_SLOW);
        readData(1, var_VAL_ZERO, conn_VAL_ZERO);
        readData(4, var_VAL_FULL, conn_VAL_FULL);
        break;
      }
      case scmEventUP_FASTID: {
        readData(3, var_FAST, conn_FAST);
        readData(1, var_VAL_ZERO, conn_VAL_ZERO);
        readData(4, var_VAL_FULL, conn_VAL_FULL);
        break;
      }
      case scmEventDOWN_SLOWID: {
        readData(2, var_SLOW, conn_SLOW);
        readData(1, var_VAL_ZERO, conn_VAL_ZERO);
        readData(4, var_VAL_FULL, conn_VAL_FULL);
        break;
      }
      case scmEventDOWN_FASTID: {
        readData(3, var_FAST, conn_FAST);
        readData(1, var_VAL_ZERO, conn_VAL_ZERO);
        readData(4, var_VAL_FULL, conn_VAL_FULL);
        break;
      }
      case scmEventFULLID: {
        readData(1, var_VAL_ZERO, conn_VAL_ZERO);
        readData(4, var_VAL_FULL, conn_VAL_FULL);
        break;
      }
      case scmEventLOADID: {
        readData(0, var_PV, conn_PV);
        readData(1, var_VAL_ZERO, conn_VAL_ZERO);
        readData(4, var_VAL_FULL, conn_VAL_FULL);
        break;
      }
      default: break;
    }
  }

  void FORTE_RampLimitFS::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITOID: {
        writeData(5, var_OUT, conn_OUT);
        writeData(6, var_qAtZero, conn_qAtZero);
        writeData(7, var_qAtFull, conn_qAtFull);
        break;
      }
      case scmEventCNFID: {
        writeData(5, var_OUT, conn_OUT);
        writeData(6, var_qAtZero, conn_qAtZero);
        writeData(7, var_qAtFull, conn_qAtFull);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_RampLimitFS::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_PV;
      case 1: return &var_VAL_ZERO;
      case 2: return &var_SLOW;
      case 3: return &var_FAST;
      case 4: return &var_VAL_FULL;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_RampLimitFS::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_OUT;
      case 1: return &var_qAtZero;
      case 2: return &var_qAtFull;
    }
    return nullptr;
  }

  CEventConnection *FORTE_RampLimitFS::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_RampLimitFS::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_PV;
      case 1: return &conn_VAL_ZERO;
      case 2: return &conn_SLOW;
      case 3: return &conn_FAST;
      case 4: return &conn_VAL_FULL;
    }
    return nullptr;
  }

  CDataConnection *FORTE_RampLimitFS::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_OUT;
      case 1: return &conn_qAtZero;
      case 2: return &conn_qAtFull;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_RampLimitFS::getVarInternal(size_t) {
    return nullptr;
  }

  void FORTE_RampLimitFS::alg_INIT(void) {

#line 7 "RampLimitFS.fbt"
    var_OUT = var_VAL_ZERO;
#line 8 "RampLimitFS.fbt"
    var_qAtZero = func_LE(var_OUT, var_VAL_ZERO);
#line 9 "RampLimitFS.fbt"
    var_qAtFull = func_GE(var_OUT, var_VAL_FULL);
  }

  void FORTE_RampLimitFS::alg_ZERO(void) {

#line 17 "RampLimitFS.fbt"
    var_OUT = var_VAL_ZERO;
#line 18 "RampLimitFS.fbt"
    var_qAtZero = func_LE(var_OUT, var_VAL_ZERO);
#line 19 "RampLimitFS.fbt"
    var_qAtFull = func_GE(var_OUT, var_VAL_FULL);
  }

  void FORTE_RampLimitFS::alg_UP_SLOW(void) {

#line 27 "RampLimitFS.fbt"
    var_OUT = func_ADD(var_OUT, var_SLOW);
#line 28 "RampLimitFS.fbt"
    if (func_GT(var_OUT, var_VAL_FULL)) {
#line 29 "RampLimitFS.fbt"
      var_OUT = var_VAL_FULL;
    }
#line 31 "RampLimitFS.fbt"
    var_qAtZero = func_LE(var_OUT, var_VAL_ZERO);
#line 32 "RampLimitFS.fbt"
    var_qAtFull = func_GE(var_OUT, var_VAL_FULL);
  }

  void FORTE_RampLimitFS::alg_UP_FAST(void) {

#line 40 "RampLimitFS.fbt"
    var_OUT = func_ADD(var_OUT, var_FAST);
#line 41 "RampLimitFS.fbt"
    if (func_GT(var_OUT, var_VAL_FULL)) {
#line 42 "RampLimitFS.fbt"
      var_OUT = var_VAL_FULL;
    }
#line 44 "RampLimitFS.fbt"
    var_qAtZero = func_LE(var_OUT, var_VAL_ZERO);
#line 45 "RampLimitFS.fbt"
    var_qAtFull = func_GE(var_OUT, var_VAL_FULL);
  }

  void FORTE_RampLimitFS::alg_DOWN_SLOW(void) {

#line 53 "RampLimitFS.fbt"
    var_OUT = func_SUB(var_OUT, var_SLOW);
#line 54 "RampLimitFS.fbt"
    if (func_LT(var_OUT, var_VAL_ZERO)) {
#line 55 "RampLimitFS.fbt"
      var_OUT = var_VAL_ZERO;
    }
#line 57 "RampLimitFS.fbt"
    var_qAtZero = func_LE(var_OUT, var_VAL_ZERO);
#line 58 "RampLimitFS.fbt"
    var_qAtFull = func_GE(var_OUT, var_VAL_FULL);
  }

  void FORTE_RampLimitFS::alg_DOWN_FAST(void) {

#line 66 "RampLimitFS.fbt"
    var_OUT = func_SUB(var_OUT, var_FAST);
#line 67 "RampLimitFS.fbt"
    if (func_LT(var_OUT, var_VAL_ZERO)) {
#line 68 "RampLimitFS.fbt"
      var_OUT = var_VAL_ZERO;
    }
#line 70 "RampLimitFS.fbt"
    var_qAtZero = func_LE(var_OUT, var_VAL_ZERO);
#line 71 "RampLimitFS.fbt"
    var_qAtFull = func_GE(var_OUT, var_VAL_FULL);
  }

  void FORTE_RampLimitFS::alg_FULL(void) {

#line 79 "RampLimitFS.fbt"
    var_OUT = var_VAL_FULL;
#line 80 "RampLimitFS.fbt"
    var_qAtZero = func_LE(var_OUT, var_VAL_ZERO);
#line 81 "RampLimitFS.fbt"
    var_qAtFull = func_GE(var_OUT, var_VAL_FULL);
  }

  void FORTE_RampLimitFS::alg_LOAD(void) {

#line 90 "RampLimitFS.fbt"
    var_OUT = var_PV;
#line 91 "RampLimitFS.fbt"
    var_qAtZero = func_LE(var_OUT, var_VAL_ZERO);
#line 92 "RampLimitFS.fbt"
    var_qAtFull = func_GE(var_OUT, var_VAL_FULL);
  }

} // namespace forte::eclipse4diac::signalprocessing
