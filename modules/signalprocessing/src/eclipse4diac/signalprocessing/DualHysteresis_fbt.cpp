/*************************************************************************
 *** Copyright (c) 2023 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: DualHysteresis
 *** Description: 2-way conversion of Analog to Digital with Hysteresis
 *** Version:
 ***     1.0: 2023-06-06/Franz Höpfinger - HR Agrartechnik GmbH -
 *************************************************************************/

#include "forte/eclipse4diac/signalprocessing/DualHysteresis_fbt.h"

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_real.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

using namespace forte::literals;

namespace forte::eclipse4diac::signalprocessing {
  namespace {
    const auto cDataInputNames = std::array{"QI"_STRID, "MI"_STRID, "DEAD"_STRID, "HYSTERESIS"_STRID, "INPUT"_STRID};
    const auto cDataOutputNames = std::array{"QO"_STRID, "DO_UP"_STRID, "DO_DOWN"_STRID};
    const auto cEventInputNames = std::array{"INIT"_STRID, "REQ"_STRID};
    const auto cEventInputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
    const auto cEventOutputNames = std::array{"INITO"_STRID, "CNF"_STRID};
    const auto cEventOutputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_DualHysteresis, "eclipse4diac::signalprocessing::DualHysteresis"_STRID)

  FORTE_DualHysteresis::FORTE_DualHysteresis(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CBasicFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      var_QI(0_BOOL),
      var_MI(0.5_REAL),
      var_DEAD(0.1_REAL),
      var_HYSTERESIS(0.1_REAL),
      var_INPUT(0_REAL),
      var_QO(0_BOOL),
      var_DO_UP(0_BOOL),
      var_DO_DOWN(0_BOOL),
      conn_INITO(*this, 0),
      conn_CNF(*this, 1),
      conn_QI(nullptr),
      conn_MI(nullptr),
      conn_DEAD(nullptr),
      conn_HYSTERESIS(nullptr),
      conn_INPUT(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_DO_UP(*this, 1, var_DO_UP),
      conn_DO_DOWN(*this, 2, var_DO_DOWN) {
  }

  void FORTE_DualHysteresis::setInitialValues() {
    CBasicFB::setInitialValues();
    var_QI = 0_BOOL;
    var_MI = 0.5_REAL;
    var_DEAD = 0.1_REAL;
    var_HYSTERESIS = 0.1_REAL;
    var_INPUT = 0_REAL;
    var_QO = 0_BOOL;
    var_DO_UP = 0_BOOL;
    var_DO_DOWN = 0_BOOL;
  }

  void FORTE_DualHysteresis::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    do {
      switch (mECCState) {
        case scmStateSTART:
          if ((scmEventINITID == paEIID) && (func_EQ(true_BOOL, var_QI)))
            enterStateInit(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateInit:
          if (scmEventREQID == paEIID)
            enterStateNeutral(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateUP:
          if ((scmEventREQID == paEIID) && (func_LT(var_INPUT, func_ADD<CIEC_REAL>(var_MI, var_DEAD))))
            enterStateNeutral(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateNeutral:
          if ((scmEventINITID == paEIID) && (func_EQ(false_BOOL, var_QI)))
            enterStateDeInit(paECET);
          else if ((scmEventREQID == paEIID) &&
                   (func_GE(var_INPUT, func_ADD<CIEC_REAL>(func_ADD<CIEC_REAL>(var_MI, var_DEAD), var_HYSTERESIS))))
            enterStateUP(paECET);
          else if ((scmEventREQID == paEIID) &&
                   (func_LE(var_INPUT, func_SUB<CIEC_REAL>(func_SUB<CIEC_REAL>(var_MI, var_DEAD), var_HYSTERESIS))))
            enterStateDOWN(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateDeInit:
          if (1)
            enterStateSTART(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateDOWN:
          if ((scmEventREQID == paEIID) && (func_GT(var_INPUT, func_SUB<CIEC_REAL>(var_MI, var_DEAD))))
            enterStateNeutral(paECET);
          else
            return; // no transition cleared
          break;
        default:
          DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 6.",
                       mECCState.operator TForteUInt16());
          mECCState = 0; // 0 is always the initial state
          return;
      }
      paEIID = cgInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
    } while (true);
  }

  void FORTE_DualHysteresis::enterStateSTART(CEventChainExecutionThread *const) {
    mECCState = scmStateSTART;
  }

  void FORTE_DualHysteresis::enterStateInit(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateInit;
    alg_initialize();
    sendOutputEvent(scmEventINITOID, paECET);
  }

  void FORTE_DualHysteresis::enterStateUP(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateUP;
    alg_alUp();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_DualHysteresis::enterStateNeutral(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateNeutral;
    alg_alNeutral();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_DualHysteresis::enterStateDeInit(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateDeInit;
    alg_deInitialize();
    sendOutputEvent(scmEventINITOID, paECET);
  }

  void FORTE_DualHysteresis::enterStateDOWN(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateDOWN;
    alg_alDown();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_DualHysteresis::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITID: {
        readData(0, var_QI, conn_QI);
        break;
      }
      case scmEventREQID: {
        readData(0, var_QI, conn_QI);
        readData(1, var_MI, conn_MI);
        readData(2, var_DEAD, conn_DEAD);
        readData(4, var_INPUT, conn_INPUT);
        readData(3, var_HYSTERESIS, conn_HYSTERESIS);
        break;
      }
      default: break;
    }
  }

  void FORTE_DualHysteresis::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITOID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
        break;
      }
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_DO_UP, conn_DO_UP);
        writeData(cFBInterfaceSpec.getNumDIs() + 2, var_DO_DOWN, conn_DO_DOWN);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_DualHysteresis::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_MI;
      case 2: return &var_DEAD;
      case 3: return &var_HYSTERESIS;
      case 4: return &var_INPUT;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_DualHysteresis::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_DO_UP;
      case 2: return &var_DO_DOWN;
    }
    return nullptr;
  }

  CEventConnection *FORTE_DualHysteresis::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_DualHysteresis::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_MI;
      case 2: return &conn_DEAD;
      case 3: return &conn_HYSTERESIS;
      case 4: return &conn_INPUT;
    }
    return nullptr;
  }

  CDataConnection *FORTE_DualHysteresis::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_DO_UP;
      case 2: return &conn_DO_DOWN;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_DualHysteresis::getVarInternal(size_t) {
    return nullptr;
  }

  void FORTE_DualHysteresis::alg_initialize(void) {

#line 3 "DualHysteresis.fbt"
    var_QO = var_QI;
#line 5 "DualHysteresis.fbt"
    var_DO_UP = false_BOOL;
#line 6 "DualHysteresis.fbt"
    var_DO_DOWN = false_BOOL;
  }

  void FORTE_DualHysteresis::alg_deInitialize(void) {

#line 11 "DualHysteresis.fbt"
    var_QO = false_BOOL;
#line 13 "DualHysteresis.fbt"
    var_DO_UP = false_BOOL;
#line 14 "DualHysteresis.fbt"
    var_DO_DOWN = false_BOOL;
  }

  void FORTE_DualHysteresis::alg_alNeutral(void) {

#line 18 "DualHysteresis.fbt"
    var_QO = var_QI;
#line 20 "DualHysteresis.fbt"
    var_DO_UP = false_BOOL;
#line 21 "DualHysteresis.fbt"
    var_DO_DOWN = false_BOOL;
  }

  void FORTE_DualHysteresis::alg_alUp(void) {

#line 25 "DualHysteresis.fbt"
    var_QO = var_QI;
#line 26 "DualHysteresis.fbt"
    if (func_EQ(true_BOOL, var_QI)) {
#line 28 "DualHysteresis.fbt"
      var_DO_UP = true_BOOL;
#line 29 "DualHysteresis.fbt"
      var_DO_DOWN = false_BOOL;
    }
  }

  void FORTE_DualHysteresis::alg_alDown(void) {

#line 34 "DualHysteresis.fbt"
    var_QO = var_QI;
#line 35 "DualHysteresis.fbt"
    if (func_EQ(true_BOOL, var_QI)) {
#line 37 "DualHysteresis.fbt"
      var_DO_UP = false_BOOL;
#line 38 "DualHysteresis.fbt"
      var_DO_DOWN = true_BOOL;
    }
  }

} // namespace forte::eclipse4diac::signalprocessing
