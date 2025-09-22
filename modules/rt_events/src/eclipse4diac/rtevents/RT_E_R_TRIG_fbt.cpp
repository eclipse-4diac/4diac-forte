/*******************************************************************************
 * Copyright (c) 2006 - 2013 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny, Monika Wenger,
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "forte/eclipse4diac/rtevents/RT_E_R_TRIG_fbt.h"

using namespace forte::literals;

namespace forte::eclipse4diac::rtevents {
  namespace {
    const auto cDataInputNames = std::array{"QI"_STRID, "Tmin"_STRID, "Deadline"_STRID, "WCET"_STRID};
    const auto cDataOutputNames = std::array{"QO"_STRID};
    const auto cEventInputNames = std::array{"INIT"_STRID, "EI"_STRID};
    const auto cEventInputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
    const auto cEventOutputNames = std::array{"INITO"_STRID, "EO"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_RT_E_R_TRIG, "eclipse4diac::rtevents::RT_E_R_TRIG"_STRID)

  FORTE_RT_E_R_TRIG::FORTE_RT_E_R_TRIG(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CRTEventSingle(paContainer, cFBInterfaceSpec, paInstanceNameId),
      conn_INITO(*this, 0),
      conn_EO(*this, 1),
      conn_QI(nullptr),
      conn_Tmin(nullptr),
      conn_Deadline(nullptr),
      conn_WCET(nullptr),
      conn_QO(*this, 0, var_QO) {};

  void FORTE_RT_E_R_TRIG::setInitialValues() {
    var_QI = 0_BOOL;
    var_Tmin = 0_TIME;
    var_Deadline = 0_TIME;
    var_WCET = 0_TIME;
    var_QO = 0_BOOL;
  }

  bool FORTE_RT_E_R_TRIG::checkActivation(TEventID) {
    bool bRetval = false;

    if (var_QI && !mWasHigh) {
      bRetval = true;
    }
    mWasHigh = var_QI;
    return bRetval;
  }

  void FORTE_RT_E_R_TRIG::readInputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITID: {
        readData(0, var_QI, conn_QI);
        readData(1, var_Tmin, conn_Tmin);
        readData(2, var_Deadline, conn_Deadline);
        readData(3, var_WCET, conn_WCET);
        break;
      }
      case scmEventEIID: {
        readData(0, var_QI, conn_QI);
        break;
      }
      default: break;
    }
  }

  void FORTE_RT_E_R_TRIG::writeOutputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITOID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
        break;
      }
      case scmEventEOID: {
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_RT_E_R_TRIG::getDI(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_Tmin;
      case 2: return &var_Deadline;
      case 3: return &var_WCET;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_RT_E_R_TRIG::getDO(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
    }
    return nullptr;
  }

  CEventConnection *FORTE_RT_E_R_TRIG::getEOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_EO;
    }
    return nullptr;
  }

  CDataConnection **FORTE_RT_E_R_TRIG::getDIConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_Tmin;
      case 2: return &conn_Deadline;
      case 3: return &conn_WCET;
    }
    return nullptr;
  }

  CDataConnection *FORTE_RT_E_R_TRIG::getDOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
    }
    return nullptr;
  }

} // namespace forte::eclipse4diac::rtevents
