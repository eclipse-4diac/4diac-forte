/*************************************************************************
 *** Copyright (c) 2012, 2015 Profactor GmbH, fortiss GmbH
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: FB_RANDOM
 *** Description: Generate a REAL Randomly
 *** Version:
 ***     1.0: 2012-05-31/Gerhard Ebenhofer - Profactor GmbH - initial API and implementation and/or initial
 *documentation
 ***     1.1: 2015-01-01/Alois Zoitl - fortiss GmbH -
 *************************************************************************/

#include "forte/eclipse4diac/utils/FB_RANDOM_fbt.h"

using namespace forte::literals;

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

namespace forte::eclipse4diac::utils {
  namespace {
    const auto cDataInputNames = std::array{"SEED"_STRID};
    const auto cDataOutputNames = std::array{"VAL"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_FB_RANDOM, "eclipse4diac::utils::FB_RANDOM"_STRID)

  FORTE_FB_RANDOM::FORTE_FB_RANDOM(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CBasicFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      var_SEED(0_UINT),
      var_VAL(0_REAL),
      conn_INITO(*this, 0),
      conn_CNF(*this, 1),
      conn_SEED(nullptr),
      conn_VAL(*this, 0, var_VAL) {
    mRandomGenerator.seed(mRandomDevice());
  }

  void FORTE_FB_RANDOM::setInitialValues() {
    CBasicFB::setInitialValues();
    var_SEED = 0_UINT;
    var_VAL = 0_REAL;
  }

  void FORTE_FB_RANDOM::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    do {
      switch (mECCState) {
        case scmStateSTART:
          if (scmEventREQID == paEIID)
            enterStateREQ(paECET);
          else if (scmEventINITID == paEIID)
            enterStateState(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateREQ:
          if (1)
            enterStateSTART(paECET);
          else
            return; // no transition cleared
          break;
        case scmStateState:
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

  void FORTE_FB_RANDOM::enterStateSTART(CEventChainExecutionThread *const) {
    mECCState = scmStateSTART;
  }

  void FORTE_FB_RANDOM::enterStateREQ(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateREQ;
    alg_REQ();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_FB_RANDOM::enterStateState(CEventChainExecutionThread *const paECET) {
    mECCState = scmStateState;
    alg_INIT();
    sendOutputEvent(scmEventINITOID, paECET);
  }

  void FORTE_FB_RANDOM::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITID: {
        readData(0, var_SEED, conn_SEED);
        break;
      }
      default: break;
    }
  }

  void FORTE_FB_RANDOM::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_VAL, conn_VAL);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_FB_RANDOM::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_SEED;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_FB_RANDOM::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_VAL;
    }
    return nullptr;
  }

  CEventConnection *FORTE_FB_RANDOM::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_FB_RANDOM::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_SEED;
    }
    return nullptr;
  }

  CDataConnection *FORTE_FB_RANDOM::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_VAL;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_FB_RANDOM::getVarInternal(size_t) {
    return nullptr;
  }

  void FORTE_FB_RANDOM::alg_INIT(void) {
    if (static_cast<CIEC_UINT::TValueType>(var_SEED) == 0) {
      mRandomGenerator.seed(mRandomDevice());
    } else {
      const CIEC_UINT::TValueType seedValue = static_cast<CIEC_UINT::TValueType>(var_SEED);
      mRandomGenerator.seed(seedValue);
    }
  }

  void FORTE_FB_RANDOM::alg_REQ(void) {
    var_VAL = CIEC_REAL(mDistribution(mRandomGenerator));
  }

} // namespace forte::eclipse4diac::utils
