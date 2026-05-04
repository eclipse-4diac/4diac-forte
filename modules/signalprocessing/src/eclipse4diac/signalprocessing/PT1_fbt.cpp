/*******************************************************************************
 * Copyright (c) 2026 Thomas Mayr
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Mayr - Initial implementation.
 *******************************************************************************/

#include "forte/eclipse4diac/signalprocessing/PT1_fbt.h"

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_real.h"
#include "forte/forte_st_util.h"
#include "forte/iec61131_functions/func_ADD.h"
#include "forte/iec61131_functions/func_DIV.h"
#include "forte/iec61131_functions/func_LE.h"
#include "forte/iec61131_functions/func_MUL.h"
#include "forte/iec61131_functions/func_NOT.h"
#include "forte/iec61131_functions/func_SUB.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::signalprocessing {
  namespace {
    constexpr std::string_view TypeHash =""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID, "RESET"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"IN"_STRID, "T1"_STRID, "TS"_STRID};
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

    const auto cInternalsNames = std::array{"alpha"_STRID, "state"_STRID, "initialized"_STRID};
  }

  DEFINE_FIRMWARE_FB(FORTE_PT1, "eclipse4diac::signalprocessing::PT1"_STRID, TypeHash)

  FORTE_PT1::FORTE_PT1(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CBasicFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cInternalsNames),
      var_alpha(0_REAL),
      var_state(0_REAL),
      var_initialized(0_BOOL),
      var_IN(0_REAL),
      var_T1(0_REAL),
      var_TS(0_REAL),
      var_OUT(0_REAL),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_T1(nullptr),
      conn_TS(nullptr),
      conn_OUT(*this, 0, var_OUT) {
  }

  void FORTE_PT1::setInitialValues() {
    CBasicFB::setInitialValues();
    var_alpha = 0_REAL;
    var_state = 0_REAL;
    var_initialized = 0_BOOL;
    var_IN = 0_REAL;
    var_T1 = 0_REAL;
    var_TS = 0_REAL;
    var_OUT = 0_REAL;
  }

  void FORTE_PT1::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    do {
      switch(mECCState) {
        case scmStateSTART:
          if(scmEventREQID == paEIID) enterStateREQ(paECET);
          else
          if(scmEventRESETID == paEIID) enterStateRESET(paECET);
          else return; //no transition cleared
          break;
        case scmStateREQ:
          if(1) enterStateSTART(paECET);
          else return; //no transition cleared
          break;
        case scmStateRESET:
          if(1) enterStateSTART(paECET);
          else return; //no transition cleared
          break;
        default:
          DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 3.", mECCState.operator TForteUInt16 ());
          mECCState = 0; // 0 is always the initial state
          return;
      }
      paEIID = cgInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
    } while(true);
  }

  void FORTE_PT1::enterStateSTART(CEventChainExecutionThread *const) {
    mECCState = scmStateSTART;
  }

  void FORTE_PT1::enterStateREQ(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateREQ;
    alg_ALG_REQ();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_PT1::enterStateRESET(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateRESET;
    alg_ALG_RESET();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_PT1::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventREQID: {
        readData(0, var_IN, conn_IN);
        readData(1, var_T1, conn_T1);
        readData(2, var_TS, conn_TS);
        break;
      }
      case scmEventRESETID: {
        readData(0, var_IN, conn_IN);
        readData(1, var_T1, conn_T1);
        readData(2, var_TS, conn_TS);
        break;
      }
      default:
        break;
    }
  }

  void FORTE_PT1::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventCNFID: {
        writeData(3, var_OUT, conn_OUT);
        break;
      }
      default:
        break;
    }
  }

  CIEC_ANY *FORTE_PT1::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_IN;
      case 1: return &var_T1;
      case 2: return &var_TS;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_PT1::getDO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_OUT;
    }
    return nullptr;
  }

  CEventConnection *FORTE_PT1::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_PT1::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_IN;
      case 1: return &conn_T1;
      case 2: return &conn_TS;
    }
    return nullptr;
  }

  CDataConnection *FORTE_PT1::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_OUT;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_PT1::getVarInternal(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_alpha;
      case 1: return &var_state;
      case 2: return &var_initialized;
    }
    return nullptr;
  }

  void FORTE_PT1::alg_ALG_REQ(void) {

    #line 2 "PT1.fbt"
    if (func_LE(var_TS, 0.0_REAL)) {
      #line 3 "PT1.fbt"
      var_OUT = var_IN;
      #line 4 "PT1.fbt"
      var_state = var_OUT;
      #line 5 "PT1.fbt"
      var_initialized = true_BOOL;
      #line 6 "PT1.fbt"
      return;
    }
    #line 9 "PT1.fbt"
    if (func_LE(var_T1, 0.0_REAL)) {
      #line 10 "PT1.fbt"
      var_OUT = var_IN;
      #line 11 "PT1.fbt"
      var_state = var_OUT;
      #line 12 "PT1.fbt"
      var_initialized = true_BOOL;
      #line 13 "PT1.fbt"
      return;
    }
    #line 16 "PT1.fbt"
    var_alpha = func_DIV<CIEC_REAL>(var_TS, func_ADD<CIEC_REAL>(var_T1, var_TS));
    #line 18 "PT1.fbt"
    if (func_NOT<CIEC_BOOL>(var_initialized)) {
      #line 19 "PT1.fbt"
      var_state = var_IN;
      #line 20 "PT1.fbt"
      var_initialized = true_BOOL;
    }
    else {
      #line 22 "PT1.fbt"
      var_state = func_ADD<CIEC_REAL>(var_state, func_MUL<CIEC_REAL>(var_alpha, func_SUB<CIEC_REAL>(var_IN, var_state)));
    }
    #line 25 "PT1.fbt"
    var_OUT = var_state;
  }

  void FORTE_PT1::alg_ALG_RESET(void) {

    #line 29 "PT1.fbt"
    var_state = 0.0_REAL;
    #line 30 "PT1.fbt"
    var_alpha = 0.0_REAL;
    #line 31 "PT1.fbt"
    var_initialized = false_BOOL;
    #line 32 "PT1.fbt"
    var_OUT = 0.0_REAL;
  }

}