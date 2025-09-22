/*******************************************************************************
 * Copyright (c) 2006 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Martin Melik Merkumians, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "forte/iec61499/events/E_CTU_fbt.h"

using namespace forte::literals;

#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61499::events {
  namespace {
    const auto cDataInputNames = std::array{"PV"_STRID};
    const auto cDataOutputNames = std::array{"Q"_STRID, "CV"_STRID};
    const auto cEventInputNames = std::array{"CU"_STRID, "R"_STRID};
    const auto cEventInputTypeIds = std::array{"Event"_STRID, "Event"_STRID};
    const auto cEventOutputNames = std::array{"CUO"_STRID, "RO"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID, "Event"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_E_CTU, "iec61499::events::E_CTU"_STRID)

  FORTE_E_CTU::FORTE_E_CTU(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CBasicFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      conn_CUO(*this, 0),
      conn_RO(*this, 1),
      conn_PV(nullptr),
      conn_Q(*this, 0, var_Q),
      conn_CV(*this, 1, var_CV) {
  }

  void FORTE_E_CTU::setInitialValues() {
    CBasicFB::setInitialValues();
    var_PV = 0_UINT;
    var_Q = 0_BOOL;
    var_CV = 0_UINT;
  }

  void FORTE_E_CTU::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    do {
      switch (mECCState) {
        case scmStateSTART:
          if ((scmEventCUID == paEIID) && (func_LT(var_CV, 65535_UINT)))
            enterStateCU(paECET);
          else if (scmEventRID == paEIID)
            enterStateR(paECET);
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
        default:
          DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 3.",
                       mECCState.operator TForteUInt16());
          mECCState = 0; // 0 is always the initial state
          return;
      }
      paEIID = cgInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
    } while (true);
  }

  void FORTE_E_CTU::enterStateSTART(CEventChainExecutionThread *const) {
    mECCState = scmStateSTART;
  }

  void FORTE_E_CTU::enterStateCU(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateCU;
    alg_CU();
    sendOutputEvent(scmEventCUOID, paECET);
  }

  void FORTE_E_CTU::enterStateR(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateR;
    alg_R();
    sendOutputEvent(scmEventROID, paECET);
  }

  void FORTE_E_CTU::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCUID: {
        readData(0, var_PV, conn_PV);
        break;
      }
      default: break;
    }
  }

  void FORTE_E_CTU::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCUOID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_Q, conn_Q);
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_CV, conn_CV);
        break;
      }
      case scmEventROID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_Q, conn_Q);
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_CV, conn_CV);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_E_CTU::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_PV;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_CTU::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_Q;
      case 1: return &var_CV;
    }
    return nullptr;
  }

  CEventConnection *FORTE_E_CTU::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CUO;
      case 1: return &conn_RO;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_CTU::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_PV;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_CTU::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_Q;
      case 1: return &conn_CV;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_CTU::getVarInternal(size_t) {
    return nullptr;
  }

  void FORTE_E_CTU::alg_R(void) {

#line 2 "E_CTU.fbt"
    var_CV = 0_UINT;
#line 3 "E_CTU.fbt"
    var_Q = false_BOOL;
  }

  void FORTE_E_CTU::alg_CU(void) {

#line 7 "E_CTU.fbt"
    var_CV = func_ADD<CIEC_UINT>(var_CV, 1_UINT);
#line 8 "E_CTU.fbt"
    var_Q = func_GE(var_CV, var_PV);
  }

} // namespace forte::iec61499::events
