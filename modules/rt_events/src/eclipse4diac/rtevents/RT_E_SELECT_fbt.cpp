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
#include "forte/eclipse4diac/rtevents/RT_E_SELECT_fbt.h"

using namespace forte::literals;

namespace forte::eclipse4diac::rtevents {
  namespace {
    const auto cDataInputNames = std::array{"QI"_STRID, "G"_STRID, "Tmin"_STRID, "Deadline"_STRID, "WCET"_STRID};
    const auto cDataOutputNames = std::array{"QO"_STRID};
    const auto cEventInputNames = std::array{"INIT"_STRID, "EI0"_STRID, "EI1"_STRID};
    const auto cEventInputTypeIds = std::array{"EInit"_STRID, "Event"_STRID, "Event"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_RT_E_SELECT, "eclipse4diac::rtevents::RT_E_SELECT"_STRID)

  FORTE_RT_E_SELECT::FORTE_RT_E_SELECT(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CRTEventSingle(paContainer, cFBInterfaceSpec, paInstanceNameId),
      conn_INITO(*this, 0),
      conn_EO(*this, 1),
      conn_QI(nullptr),
      conn_G(nullptr),
      conn_Tmin(nullptr),
      conn_Deadline(nullptr),
      conn_WCET(nullptr),
      conn_QO(*this, 0, var_QO) {};

  void FORTE_RT_E_SELECT::setInitialValues() {
    var_QI = 0_BOOL;
    var_G = 0_BOOL;
    var_Tmin = 0_TIME;
    var_Deadline = 0_TIME;
    var_WCET = 0_TIME;
    var_QO = 0_BOOL;
  }

  bool FORTE_RT_E_SELECT::checkActivation(TEventID paEIID) {
    return (scmEventEI0ID == paEIID && !var_G) || (scmEventEI1ID == paEIID && var_G);
  }

  void FORTE_RT_E_SELECT::readInputData(TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITID: {
        readData(0, var_QI, conn_QI);
        readData(2, var_Tmin, conn_Tmin);
        readData(3, var_Deadline, conn_Deadline);
        readData(4, var_WCET, conn_WCET);
        break;
      }
      case scmEventEI0ID: {
        readData(1, var_G, conn_G);
        break;
      }
      case scmEventEI1ID: {
        readData(1, var_G, conn_G);
        break;
      }
      default: break;
    }
  }

  void FORTE_RT_E_SELECT::writeOutputData(TEventID paEIID) {
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

  CIEC_ANY *FORTE_RT_E_SELECT::getDI(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_G;
      case 2: return &var_Tmin;
      case 3: return &var_Deadline;
      case 4: return &var_WCET;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_RT_E_SELECT::getDO(size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
    }
    return nullptr;
  }

  CEventConnection *FORTE_RT_E_SELECT::getEOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_EO;
    }
    return nullptr;
  }

  CDataConnection **FORTE_RT_E_SELECT::getDIConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_G;
      case 2: return &conn_Tmin;
      case 3: return &conn_Deadline;
      case 4: return &conn_WCET;
    }
    return nullptr;
  }

  CDataConnection *FORTE_RT_E_SELECT::getDOConUnchecked(TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
    }
    return nullptr;
  }

} // namespace forte::eclipse4diac::rtevents
