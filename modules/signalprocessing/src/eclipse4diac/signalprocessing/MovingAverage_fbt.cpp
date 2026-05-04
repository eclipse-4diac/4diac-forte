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

#include "forte/eclipse4diac/signalprocessing/MovingAverage_fbt.h"

#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_int.h"
#include "forte/datatypes/forte_real.h"
#include "forte/forte_st_util.h"
#include "forte/iec61131_functions/func_ADD.h"
#include "forte/iec61131_functions/func_DIV.h"
#include "forte/iec61131_functions/func_GT.h"
#include "forte/iec61131_functions/func_INT_TO_REAL.h"
#include "forte/iec61131_functions/func_LE.h"
#include "forte/iec61131_functions/func_LT.h"
#include "forte/iec61131_functions/func_MOD.h"
#include "forte/iec61131_functions/func_NE.h"
#include "forte/iec61131_functions/func_SUB.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::signalprocessing {
  namespace {
    constexpr std::string_view TypeHash =""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID, "RESET"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"IN"_STRID, "WINDOW"_STRID};
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

    const auto cInternalsNames = std::array{"count"_STRID, "sum"_STRID, "index"_STRID, "buffer"_STRID, "effectiveWindow"_STRID, "lastWindow"_STRID, "i"_STRID};
  }

  DEFINE_FIRMWARE_FB(FORTE_MovingAverage, "eclipse4diac::signalprocessing::MovingAverage"_STRID, TypeHash)

  FORTE_MovingAverage::FORTE_MovingAverage(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CBasicFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cInternalsNames),
      var_count(0_INT),
      var_sum(0_REAL),
      var_index(0_INT),
      var_buffer(CIEC_ARRAY_FIXED<CIEC_REAL, 0, 99>{}),
      var_effectiveWindow(0_INT),
      var_lastWindow(0_INT),
      var_i(0_INT),
      var_IN(0_REAL),
      var_WINDOW(0_INT),
      var_OUT(0_REAL),
      conn_CNF(*this, 0),
      conn_IN(nullptr),
      conn_WINDOW(nullptr),
      conn_OUT(*this, 0, var_OUT) {
  }

  void FORTE_MovingAverage::setInitialValues() {
    CBasicFB::setInitialValues();
    var_count = 0_INT;
    var_sum = 0_REAL;
    var_index = 0_INT;
    var_buffer = CIEC_ARRAY_FIXED<CIEC_REAL, 0, 99>{};
    var_effectiveWindow = 0_INT;
    var_lastWindow = 0_INT;
    var_i = 0_INT;
    var_IN = 0_REAL;
    var_WINDOW = 0_INT;
    var_OUT = 0_REAL;
  }

  void FORTE_MovingAverage::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
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

  void FORTE_MovingAverage::enterStateSTART(CEventChainExecutionThread *const) {
    mECCState = scmStateSTART;
  }

  void FORTE_MovingAverage::enterStateREQ(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateREQ;
    alg_ALG_REQ();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_MovingAverage::enterStateRESET(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateRESET;
    alg_ALG_RESET();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_MovingAverage::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventREQID: {
        readData(0, var_IN, conn_IN);
        readData(1, var_WINDOW, conn_WINDOW);
        break;
      }
      case scmEventRESETID: {
        readData(0, var_IN, conn_IN);
        readData(1, var_WINDOW, conn_WINDOW);
        break;
      }
      default:
        break;
    }
  }

  void FORTE_MovingAverage::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventCNFID: {
        writeData(2, var_OUT, conn_OUT);
        break;
      }
      default:
        break;
    }
  }

  CIEC_ANY *FORTE_MovingAverage::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_IN;
      case 1: return &var_WINDOW;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_MovingAverage::getDO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_OUT;
    }
    return nullptr;
  }

  CEventConnection *FORTE_MovingAverage::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_MovingAverage::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_IN;
      case 1: return &conn_WINDOW;
    }
    return nullptr;
  }

  CDataConnection *FORTE_MovingAverage::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_OUT;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_MovingAverage::getVarInternal(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_count;
      case 1: return &var_sum;
      case 2: return &var_index;
      case 3: return &var_buffer;
      case 4: return &var_effectiveWindow;
      case 5: return &var_lastWindow;
      case 6: return &var_i;
    }
    return nullptr;
  }

  void FORTE_MovingAverage::alg_ALG_REQ(void) {

    #line 2 "MovingAverage.fbt"
    if (func_LE(var_WINDOW, 0_INT)) {
      #line 3 "MovingAverage.fbt"
      var_OUT = var_IN;
      #line 4 "MovingAverage.fbt"
      return;
    }
    #line 7 "MovingAverage.fbt"
    var_effectiveWindow = var_WINDOW;
    #line 8 "MovingAverage.fbt"
    if (func_GT(var_effectiveWindow, 100_INT)) {
      #line 9 "MovingAverage.fbt"
      var_effectiveWindow = 100_INT;
    }
    #line 12 "MovingAverage.fbt"
    if (func_NE(var_effectiveWindow, var_lastWindow)) {
      #line 13 "MovingAverage.fbt"
      var_sum = 0.0_REAL;
      #line 14 "MovingAverage.fbt"
      var_index = 0_INT;
      #line 15 "MovingAverage.fbt"
      var_count = 0_INT;
      #line 16 "MovingAverage.fbt"
      var_i = 0_INT;
      #line 17 "MovingAverage.fbt"
      while (func_LT(var_i, 100_INT)) {
        #line 18 "MovingAverage.fbt"
        var_buffer.at(var_i) = 0.0_REAL;
        #line 19 "MovingAverage.fbt"
        var_i = func_ADD<CIEC_INT>(var_i, 1_INT);
      }
      #line 21 "MovingAverage.fbt"
      var_lastWindow = var_effectiveWindow;
    }
    #line 24 "MovingAverage.fbt"
    if (func_LT(var_count, var_effectiveWindow)) {
      #line 25 "MovingAverage.fbt"
      var_buffer.at(var_index) = var_IN;
      #line 26 "MovingAverage.fbt"
      var_sum = func_ADD<CIEC_REAL>(var_sum, var_IN);
      #line 27 "MovingAverage.fbt"
      var_count = func_ADD<CIEC_INT>(var_count, 1_INT);
    }
    else {
      #line 29 "MovingAverage.fbt"
      var_sum = func_SUB<CIEC_REAL>(var_sum, var_buffer.at(var_index));
      #line 30 "MovingAverage.fbt"
      var_buffer.at(var_index) = var_IN;
      #line 31 "MovingAverage.fbt"
      var_sum = func_ADD<CIEC_REAL>(var_sum, var_IN);
    }
    #line 34 "MovingAverage.fbt"
    var_index = func_MOD<CIEC_INT>(func_ADD<CIEC_INT>(var_index, 1_INT), var_effectiveWindow);
    #line 36 "MovingAverage.fbt"
    var_OUT = func_DIV<CIEC_REAL>(var_sum, func_INT_TO_REAL(var_count));
  }

  void FORTE_MovingAverage::alg_ALG_RESET(void) {

    #line 40 "MovingAverage.fbt"
    var_sum = 0.0_REAL;
    #line 41 "MovingAverage.fbt"
    var_index = 0_INT;
    #line 42 "MovingAverage.fbt"
    var_count = 0_INT;
    #line 43 "MovingAverage.fbt"
    var_lastWindow = 0_INT;
    #line 44 "MovingAverage.fbt"
    var_i = 0_INT;
    #line 45 "MovingAverage.fbt"
    while (func_LT(var_i, 100_INT)) {
      #line 46 "MovingAverage.fbt"
      var_buffer.at(var_i) = 0.0_REAL;
      #line 47 "MovingAverage.fbt"
      var_i = func_ADD<CIEC_INT>(var_i, 1_INT);
    }
    #line 49 "MovingAverage.fbt"
    var_OUT = 0.0_REAL;
  }

}