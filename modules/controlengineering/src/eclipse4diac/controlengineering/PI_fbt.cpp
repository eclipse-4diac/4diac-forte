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

#include "forte/eclipse4diac/controlengineering/PI_fbt.h"

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_real.h"
#include "forte/datatypes/forte_time.h"
#include "forte/forte_st_util.h"
#include "forte/iec61131_functions/func_ADD.h"
#include "forte/iec61131_functions/func_AND.h"
#include "forte/iec61131_functions/func_DIV.h"
#include "forte/iec61131_functions/func_GE.h"
#include "forte/iec61131_functions/func_GT.h"
#include "forte/iec61131_functions/func_LE.h"
#include "forte/iec61131_functions/func_LREAL_TO_REAL.h"
#include "forte/iec61131_functions/func_LT.h"
#include "forte/iec61131_functions/func_MUL.h"
#include "forte/iec61131_functions/func_NOT.h"
#include "forte/iec61131_functions/func_OR.h"
#include "forte/iec61131_functions/func_SUB.h"
#include "forte/iec61131_functions/func_TIME_IN_S_TO_LREAL.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::controlengineering {
  namespace {
    constexpr std::string_view TypeHash =""sv;

    const auto cEventInputNames = std::array{"INIT"_STRID, "REQ"_STRID};
    const auto cEventOutputNames = std::array{"INITO"_STRID, "CNF"_STRID};
    const auto cDataInputNames = std::array{"QI"_STRID, "ACTUAL"_STRID, "SET_POINT"_STRID, "KP"_STRID, "TN"_STRID, "Timestep"_STRID};
    const auto cDataOutputNames = std::array{"QO"_STRID, "OUT"_STRID};
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

    const auto cInternalsNames = std::array{"ERR"_STRID, "I_SUM"_STRID, "DT_SEC"_STRID, "P_TERM"_STRID, "I_TERM"_STRID};
  }

  DEFINE_FIRMWARE_FB(FORTE_PI, "eclipse4diac::controlengineering::PI"_STRID, TypeHash)

  FORTE_PI::FORTE_PI(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CBasicFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cInternalsNames),
      var_ERR(0_REAL),
      var_I_SUM(0_REAL),
      var_DT_SEC(0_REAL),
      var_P_TERM(0_REAL),
      var_I_TERM(0_REAL),
      var_QI(0_BOOL),
      var_ACTUAL(0_REAL),
      var_SET_POINT(0_REAL),
      var_KP(0_REAL),
      var_TN(0_REAL),
      var_Timestep(0_TIME),
      var_QO(0_BOOL),
      var_OUT(0_REAL),
      conn_INITO(*this, 0),
      conn_CNF(*this, 1),
      conn_QI(nullptr),
      conn_ACTUAL(nullptr),
      conn_SET_POINT(nullptr),
      conn_KP(nullptr),
      conn_TN(nullptr),
      conn_Timestep(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_OUT(*this, 1, var_OUT) {
  }

  void FORTE_PI::setInitialValues() {
    CBasicFB::setInitialValues();
    var_ERR = 0_REAL;
    var_I_SUM = 0_REAL;
    var_DT_SEC = 0_REAL;
    var_P_TERM = 0_REAL;
    var_I_TERM = 0_REAL;
    var_QI = 0_BOOL;
    var_ACTUAL = 0_REAL;
    var_SET_POINT = 0_REAL;
    var_KP = 0_REAL;
    var_TN = 0_REAL;
    var_Timestep = 0_TIME;
    var_QO = 0_BOOL;
    var_OUT = 0_REAL;
  }

  void FORTE_PI::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    do {
      switch(mECCState) {
        case scmStateSTART:
          if(scmEventINITID == paEIID) enterStateINIT(paECET);
          else
          if(scmEventREQID == paEIID) enterStateREQ(paECET);
          else return; //no transition cleared
          break;
        case scmStateINIT:
          if(1) enterStateSTART(paECET);
          else return; //no transition cleared
          break;
        case scmStateREQ:
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

  void FORTE_PI::enterStateSTART(CEventChainExecutionThread *const) {
    mECCState = scmStateSTART;
  }

  void FORTE_PI::enterStateINIT(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateINIT;
    alg_ALG_INIT();
    sendOutputEvent(scmEventINITOID, paECET);
  }

  void FORTE_PI::enterStateREQ(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateREQ;
    alg_ALG_REQ();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_PI::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventINITID: {
        readData(0, var_QI, conn_QI);
        readData(1, var_ACTUAL, conn_ACTUAL);
        readData(2, var_SET_POINT, conn_SET_POINT);
        readData(3, var_KP, conn_KP);
        readData(4, var_TN, conn_TN);
        readData(5, var_Timestep, conn_Timestep);
        break;
      }
      case scmEventREQID: {
        readData(0, var_QI, conn_QI);
        readData(1, var_ACTUAL, conn_ACTUAL);
        readData(2, var_SET_POINT, conn_SET_POINT);
        break;
      }
      default:
        break;
    }
  }

  void FORTE_PI::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventINITOID: {
        writeData(6, var_QO, conn_QO);
        writeData(7, var_OUT, conn_OUT);
        break;
      }
      case scmEventCNFID: {
        writeData(6, var_QO, conn_QO);
        writeData(7, var_OUT, conn_OUT);
        break;
      }
      default:
        break;
    }
  }

  CIEC_ANY *FORTE_PI::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_QI;
      case 1: return &var_ACTUAL;
      case 2: return &var_SET_POINT;
      case 3: return &var_KP;
      case 4: return &var_TN;
      case 5: return &var_Timestep;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_PI::getDO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_QO;
      case 1: return &var_OUT;
    }
    return nullptr;
  }

  CEventConnection *FORTE_PI::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_PI::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_ACTUAL;
      case 2: return &conn_SET_POINT;
      case 3: return &conn_KP;
      case 4: return &conn_TN;
      case 5: return &conn_Timestep;
    }
    return nullptr;
  }

  CDataConnection *FORTE_PI::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_OUT;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_PI::getVarInternal(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_ERR;
      case 1: return &var_I_SUM;
      case 2: return &var_DT_SEC;
      case 3: return &var_P_TERM;
      case 4: return &var_I_TERM;
    }
    return nullptr;
  }

  void FORTE_PI::alg_ALG_INIT(void) {

    #line 2 "PI.fbt"
    var_QO = var_QI;
    #line 3 "PI.fbt"
    var_ERR = 0.0_REAL;
    #line 4 "PI.fbt"
    var_I_SUM = 0.0_REAL;
    #line 5 "PI.fbt"
    var_DT_SEC = 0.0_REAL;
    #line 6 "PI.fbt"
    var_P_TERM = 0.0_REAL;
    #line 7 "PI.fbt"
    var_I_TERM = 0.0_REAL;
    #line 8 "PI.fbt"
    var_OUT = 0.0_REAL;
  }

  void FORTE_PI::alg_ALG_REQ(void) {

    #line 12 "PI.fbt"
    var_QO = var_QI;
    #line 14 "PI.fbt"
    if (func_NOT<CIEC_BOOL>(var_QI)) {
      #line 15 "PI.fbt"
      var_OUT = 0.0_REAL;
    }
    else {
      #line 17 "PI.fbt"
      var_DT_SEC = func_LREAL_TO_REAL(func_TIME_IN_S_TO_LREAL(var_Timestep));
      #line 19 "PI.fbt"
      if (func_LE(var_DT_SEC, 0.0_REAL)) {
        #line 20 "PI.fbt"
        var_DT_SEC = 0.001_REAL;
      }
      #line 23 "PI.fbt"
      var_ERR = func_SUB<CIEC_REAL>(var_SET_POINT, var_ACTUAL);
      #line 24 "PI.fbt"
      var_P_TERM = func_MUL<CIEC_REAL>(var_KP, var_ERR);
      #line 26 "PI.fbt"
      if (func_GT(var_TN, 0.0_REAL)) {
        #line 27 "PI.fbt"
        if (func_NOT<CIEC_BOOL>(func_OR<CIEC_BOOL>(func_AND<CIEC_BOOL>(func_GE(var_OUT, 100.0_REAL), func_GT(var_ERR, 0.0_REAL)), func_AND<CIEC_BOOL>(func_LE(var_OUT, 0.0_REAL), func_LT(var_ERR, 0.0_REAL))))) {
          #line 28 "PI.fbt"
          var_I_SUM = func_ADD<CIEC_REAL>(var_I_SUM, func_DIV<CIEC_REAL>(func_MUL<CIEC_REAL>(func_MUL<CIEC_REAL>(var_KP, var_ERR), var_DT_SEC), var_TN));
        }
      }
      #line 32 "PI.fbt"
      var_I_TERM = var_I_SUM;
      #line 34 "PI.fbt"
      var_OUT = func_ADD<CIEC_REAL>(var_P_TERM, var_I_TERM);
      #line 36 "PI.fbt"
      if (func_GT(var_OUT, 100.0_REAL)) {
        #line 37 "PI.fbt"
        var_OUT = 100.0_REAL;
      }
      else if (func_LT(var_OUT, 0.0_REAL)) {
        #line 39 "PI.fbt"
        var_OUT = 0.0_REAL;
      }
    }
  }

}