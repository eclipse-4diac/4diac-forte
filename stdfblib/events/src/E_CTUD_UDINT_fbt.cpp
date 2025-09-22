/*************************************************************************
 *** Copyright (c) 2017, 2024 fortiss GmbH, JKU, HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_CTUD_UDINT
 *** Description: Event-Driven Up-Down Counter
 *** Version:
 ***     1.0: 2017-09-22/Alois Zoitl - fortiss GmbH - initial API and implementation and/or initial documentation
 *(E_CTUD)
 ***     1.1: 2019-07-09/Alois Zoitl - JKU - fixed bug in down counting (E_CTUD)
 ***     1.2: 2024-02-10/Franz Hoepfinger - HR Agrartechnik GmbH - Copy over E_CTUD to E_CTUD_UDINT
 *************************************************************************/

#include "forte/iec61499/events/E_CTUD_UDINT_fbt.h"

using namespace forte::literals;

#include "forte/datatypes/forte_udint.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61499::events {
  namespace {
    const auto cDataInputNames = std::array{"PV"_STRID};
    const auto cDataOutputNames = std::array{"QU"_STRID, "QD"_STRID, "CV"_STRID};
    const auto cEventInputNames = std::array{"CU"_STRID, "CD"_STRID, "R"_STRID, "LD"_STRID};
    const auto cEventInputTypeIds = std::array{"Event"_STRID, "Event"_STRID, "Event"_STRID, "Event"_STRID};
    const auto cEventOutputNames = std::array{"CO"_STRID, "RO"_STRID, "LDO"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID, "Event"_STRID, "Event"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_E_CTUD_UDINT, "iec61499::events::E_CTUD_UDINT"_STRID)

  FORTE_E_CTUD_UDINT::FORTE_E_CTUD_UDINT(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CBasicFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      conn_CO(*this, 0),
      conn_RO(*this, 1),
      conn_LDO(*this, 2),
      conn_PV(nullptr),
      conn_QU(*this, 0, var_QU),
      conn_QD(*this, 1, var_QD),
      conn_CV(*this, 2, var_CV) {
  }

  void FORTE_E_CTUD_UDINT::setInitialValues() {
    CBasicFB::setInitialValues();
    var_PV = 0_UDINT;
    var_QU = 0_BOOL;
    var_QD = 0_BOOL;
    var_CV = 0_UDINT;
  }

  void FORTE_E_CTUD_UDINT::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    do {
      switch (mECCState) {
        case scmStateSTART:
          if ((scmEventCUID == paEIID) && (func_LT(var_CV, 4294967295_UDINT)))
            enterStateCU(paECET);
          else if (scmEventRID == paEIID)
            enterStateR(paECET);
          else if ((scmEventCDID == paEIID) && (func_GT(var_CV, 0_UDINT)))
            enterStateCD(paECET);
          else if (scmEventLDID == paEIID)
            enterStateLD(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateCU:
          if (1)
            enterStateSTART(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateR:
          if (1)
            enterStateSTART(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateCD:
          if (1)
            enterStateSTART(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateLD:
          if (1)
            enterStateSTART(paECET);
          else
            return; // no transition cleared
          break;
        default:
          DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 5.",
                       mECCState.operator TForteUInt16());
          mECCState = 0; // 0 is always the initial state
          return;
      }
      paEIID = cgInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
    } while (true);
  }

  void FORTE_E_CTUD_UDINT::enterStateSTART(CEventChainExecutionThread *const) {
    mECCState = scmStateSTART;
  }

  void FORTE_E_CTUD_UDINT::enterStateCU(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateCU;
    alg_CountUp();
    alg_UpdateQUQD();
    sendOutputEvent(scmEventCOID, paECET);
  }

  void FORTE_E_CTUD_UDINT::enterStateR(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateR;
    alg_Reset();
    alg_UpdateQUQD();
    sendOutputEvent(scmEventROID, paECET);
  }

  void FORTE_E_CTUD_UDINT::enterStateCD(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateCD;
    alg_CountDown();
    alg_UpdateQUQD();
    sendOutputEvent(scmEventCOID, paECET);
  }

  void FORTE_E_CTUD_UDINT::enterStateLD(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateLD;
    alg_Load();
    alg_UpdateQUQD();
    sendOutputEvent(scmEventLDOID, paECET);
  }

  void FORTE_E_CTUD_UDINT::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCUID: {
        readData(0, var_PV, conn_PV);
        break;
      }
      case scmEventLDID: {
        readData(0, var_PV, conn_PV);
        break;
      }
      default: break;
    }
  }

  void FORTE_E_CTUD_UDINT::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCOID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QU, conn_QU);
        writeData(cFBInterfaceSpec.getNumDIs() + 2, var_CV, conn_CV);
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_QD, conn_QD);
        break;
      }
      case scmEventROID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QU, conn_QU);
        writeData(cFBInterfaceSpec.getNumDIs() + 2, var_CV, conn_CV);
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_QD, conn_QD);
        break;
      }
      case scmEventLDOID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QU, conn_QU);
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_QD, conn_QD);
        writeData(cFBInterfaceSpec.getNumDIs() + 2, var_CV, conn_CV);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_E_CTUD_UDINT::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_PV;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_CTUD_UDINT::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QU;
      case 1: return &var_QD;
      case 2: return &var_CV;
    }
    return nullptr;
  }

  CEventConnection *FORTE_E_CTUD_UDINT::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CO;
      case 1: return &conn_RO;
      case 2: return &conn_LDO;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_CTUD_UDINT::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_PV;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_CTUD_UDINT::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QU;
      case 1: return &conn_QD;
      case 2: return &conn_CV;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_CTUD_UDINT::getVarInternal(size_t) {
    return nullptr;
  }

  void FORTE_E_CTUD_UDINT::alg_CountUp(void) {

#line 2 "E_CTUD_UDINT.fbt"
    var_CV = func_ADD<CIEC_UDINT>(var_CV, 1_UDINT);
  }

  void FORTE_E_CTUD_UDINT::alg_Reset(void) {

#line 7 "E_CTUD_UDINT.fbt"
    var_CV = 0_UDINT;
  }

  void FORTE_E_CTUD_UDINT::alg_Load(void) {

#line 11 "E_CTUD_UDINT.fbt"
    var_CV = var_PV;
  }

  void FORTE_E_CTUD_UDINT::alg_UpdateQUQD(void) {

#line 15 "E_CTUD_UDINT.fbt"
    var_QU = func_GE(var_CV, var_PV);
#line 16 "E_CTUD_UDINT.fbt"
    var_QD = func_EQ(var_CV, 0_UDINT);
  }

  void FORTE_E_CTUD_UDINT::alg_CountDown(void) {

#line 20 "E_CTUD_UDINT.fbt"
    var_CV = func_SUB<CIEC_UDINT>(var_CV, 1_UDINT);
  }

} // namespace forte::iec61499::events
