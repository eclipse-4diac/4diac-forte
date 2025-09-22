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

#include "forte/iec61499/events/E_DEMUX_fbt.h"

using namespace forte::literals;

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::iec61499::events {
  namespace {
    const auto cDataInputNames = std::array{"K"_STRID};
    const auto cEventInputNames = std::array{"EI"_STRID};
    const auto cEventInputTypeIds = std::array{"Event"_STRID};
    const auto cEventOutputNames = std::array{"EO0"_STRID, "EO1"_STRID, "EO2"_STRID, "EO3"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID, "Event"_STRID, "Event"_STRID, "Event"_STRID};
    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = cEventInputTypeIds,
        .mEONames = cEventOutputNames,
        .mEOTypeNames = cEventOutputTypeIds,
        .mDINames = cDataInputNames,
        .mDONames = {},
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_DEMUX, "iec61499::events::E_DEMUX"_STRID)

  FORTE_E_DEMUX::FORTE_E_DEMUX(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CBasicFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      conn_EO0(*this, 0),
      conn_EO1(*this, 1),
      conn_EO2(*this, 2),
      conn_EO3(*this, 3),
      conn_K(nullptr) {
  }

  void FORTE_E_DEMUX::setInitialValues() {
    CBasicFB::setInitialValues();
    var_K = 0_UINT;
  }

  void FORTE_E_DEMUX::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    do {
      switch (mECCState) {
        case scmStateSTART:
          if (scmEventEIID == paEIID)
            enterStateState(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateState:
          if (func_EQ(var_K, 0_UINT))
            enterStateState_1(paECET);
          else if (func_EQ(var_K, 1_UINT))
            enterStateState_2(paECET);
          else if (func_EQ(var_K, 2_UINT))
            enterStateState_3(paECET);
          else if (func_EQ(var_K, 3_UINT))
            enterStateState_4(paECET);
          else if (func_GT(var_K, 3_UINT))
            enterStateSTART(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateState_1:
          if (1)
            enterStateSTART(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateState_2:
          if (1)
            enterStateSTART(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateState_3:
          if (1)
            enterStateSTART(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateState_4:
          if (1)
            enterStateSTART(paECET);
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

  void FORTE_E_DEMUX::enterStateSTART(CEventChainExecutionThread *const) {
    mECCState = scmStateSTART;
  }

  void FORTE_E_DEMUX::enterStateState(CEventChainExecutionThread *const) {
    mECCState = scmStateState;
  }

  void FORTE_E_DEMUX::enterStateState_1(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateState_1;
    sendOutputEvent(scmEventEO0ID, paECET);
  }

  void FORTE_E_DEMUX::enterStateState_2(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateState_2;
    sendOutputEvent(scmEventEO1ID, paECET);
  }

  void FORTE_E_DEMUX::enterStateState_3(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateState_3;
    sendOutputEvent(scmEventEO2ID, paECET);
  }

  void FORTE_E_DEMUX::enterStateState_4(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateState_4;
    sendOutputEvent(scmEventEO3ID, paECET);
  }

  void FORTE_E_DEMUX::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventEIID: {
        readData(0, var_K, conn_K);
        break;
      }
      default: break;
    }
  }

  void FORTE_E_DEMUX::writeOutputData(TEventID) {
    // nothing to do
  }

  CIEC_ANY *FORTE_E_DEMUX::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_K;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_E_DEMUX::getDO(size_t) {
    return nullptr;
  }

  CEventConnection *FORTE_E_DEMUX::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_EO0;
      case 1: return &conn_EO1;
      case 2: return &conn_EO2;
      case 3: return &conn_EO3;
    }
    return nullptr;
  }

  CDataConnection **FORTE_E_DEMUX::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_K;
    }
    return nullptr;
  }

  CDataConnection *FORTE_E_DEMUX::getDOConUnchecked(TPortId) {
    return nullptr;
  }

  CIEC_ANY *FORTE_E_DEMUX::getVarInternal(size_t) {
    return nullptr;
  }

} // namespace forte::iec61499::events
